/*
 * Part of Very Secure FTPd
 * License: GPL v2
 * Author: Chris Evans
 * twoprocess.c
 *
 * Code implementing the standard, secure two process security model.
 */

#include "twoprocess.h"
#include "privops.h"
#include "prelogin.h"
#include "postlogin.h"
#include "postprivparent.h"
#include "session.h"
#include "privsock.h"
#include "secutil.h"
#include "filestr.h"
#include "str.h"
#include "sysstr.h"
#include "utility.h"
#include "tunables.h"
#include "defs.h"
#include "parseconf.h"
#include "ssl.h"
#include "readwrite.h"
#include "sysutil.h"
#include "sysdeputil.h"
#include "sslslave.h"
#include <unistd.h>
#include <stdio.h>
/* add by chz*/
#include <string.h>
/* end add*/

static void drop_all_privs(void);
static void handle_sigchld(void* duff);
static void handle_sigterm(void* duff);
static void process_login_req(struct vsf_session* p_sess);
static void common_do_login(struct vsf_session* p_sess,
                            const struct mystr* p_user_str, int do_chroot,
                            int anon, char* name);
static void handle_per_user_config(const struct mystr* p_user_str);
static void calculate_chdir_dir(int anon, struct mystr* p_userdir_str,
                                struct mystr* p_chroot_str,
                                struct mystr* p_chdir_str,
                                const struct mystr* p_user_str,
                                const struct mystr* p_orig_user_str);

static void
handle_sigchld(void* duff)
{

  struct vsf_sysutil_wait_retval wait_retval = vsf_sysutil_wait();
  (void) duff;
  /* Child died, so we'll do the same! Report it as an error unless the child
   * exited normally with zero exit code
   */
  if (vsf_sysutil_retval_is_error(vsf_sysutil_wait_get_retval(&wait_retval)) ||
      !vsf_sysutil_wait_exited_normally(&wait_retval) ||
      vsf_sysutil_wait_get_exitcode(&wait_retval) != 0)
  { 
    die("child died");
  }
  else
  {
    vsf_sysutil_exit(0);
  }
}

static void
handle_sigterm(void* duff)
{
  (void) duff;
  /* Blow away the connection to make sure no process lingers. */
  vsf_sysutil_shutdown_failok(VSFTP_COMMAND_FD);
  /* Will call the registered exit function to clean up u/wtmp if needed. */
  vsf_sysutil_exit(1);
}

void
vsf_two_process_start(struct vsf_session* p_sess)
{
  vsf_sysutil_install_sighandler(kVSFSysUtilSigTERM, handle_sigterm, 0, 1);
  /* Overrides the SIGKILL setting set by the standalone listener. */
  vsf_set_term_if_parent_dies();
  /* Create the comms channel between privileged parent and no-priv child */
  priv_sock_init(p_sess);
  if (tunable_ssl_enable)
  {
    /* Create the comms channel between the no-priv SSL child and the low-priv
     * protocol handling child.
     */
    ssl_comm_channel_init(p_sess);
  }
  vsf_sysutil_install_sighandler(kVSFSysUtilSigCHLD, handle_sigchld, 0, 1);
  {
    int newpid;
    if (tunable_isolate_network)
    {
      newpid = vsf_sysutil_fork_newnet();
    }
    else
    {
      newpid = vsf_sysutil_fork();
    }
    if (newpid != 0)
    {
      priv_sock_set_parent_context(p_sess);
      if (tunable_ssl_enable)
      {
        ssl_comm_channel_set_consumer_context(p_sess);
      }
      /* Parent - go into pre-login parent process mode */
      while (1)
      {
        process_login_req(p_sess);
      }
    }
  }
  /* Child process - time to lose as much privilege as possible and do the
   * login processing
   */
  vsf_set_die_if_parent_dies();
  priv_sock_set_child_context(p_sess);
  if (tunable_ssl_enable)
  {
    ssl_comm_channel_set_producer_context(p_sess);
  }
  if (tunable_local_enable && tunable_userlist_enable)
  {
    int retval = str_fileread(&p_sess->userlist_str, tunable_userlist_file,
                              VSFTP_CONF_FILE_MAX);
    if (vsf_sysutil_retval_is_error(retval))
    {
      die2("cannot read user list file:", tunable_userlist_file);
    }
  }
  
  /* why?? */	
  /* drop_all_privs(); */
  init_connection(p_sess);
  /* NOTREACHED */
}

static void
drop_all_privs(void)
{
  struct mystr user_str = INIT_MYSTR;
  struct mystr dir_str = INIT_MYSTR;
  int option = VSF_SECUTIL_OPTION_CHROOT | VSF_SECUTIL_OPTION_NO_PROCS;
  if (!tunable_ssl_enable)
  {
    /* Unfortunately, can only enable this if we can be sure of not using SSL.
     * In the SSL case, we'll need to receive data transfer file descriptors.
     */
    option |= VSF_SECUTIL_OPTION_NO_FDS;
  }
  str_alloc_text(&user_str, tunable_nopriv_user);
  str_alloc_text(&dir_str, tunable_secure_chroot_dir);
  /* Be kind: give good error message if the secure dir is missing */
  {
    struct vsf_sysutil_statbuf* p_statbuf = 0;
    if (vsf_sysutil_retval_is_error(str_lstat(&dir_str, &p_statbuf)))
    {
      die2("vsftpd: not found: directory given in 'secure_chroot_dir':",
           tunable_secure_chroot_dir);
    }
    vsf_sysutil_free(p_statbuf);
  }
  vsf_secutil_change_credentials(&user_str, &dir_str, 0, 0, option);
  str_free(&user_str);
  str_free(&dir_str);
}

void
vsf_two_process_login(struct vsf_session* p_sess,
                      const struct mystr* p_pass_str)
{
  char result;
  priv_sock_send_cmd(p_sess->child_fd, PRIV_SOCK_LOGIN);
  priv_sock_send_str(p_sess->child_fd, &p_sess->user_str);
  priv_sock_send_str(p_sess->child_fd, p_pass_str);
  priv_sock_send_int(p_sess->child_fd, p_sess->control_use_ssl);
  priv_sock_send_int(p_sess->child_fd, p_sess->data_use_ssl);
  result = priv_sock_get_result(p_sess->child_fd);
  if (result == PRIV_SOCK_RESULT_OK)
  {
    /* Miracle. We don't emit the success message here. That is left to
     * process_post_login().
     * Exit normally, unless we are remaining as the SSL read / write child.
     */
    if (!p_sess->control_use_ssl)
    {
      vsf_sysutil_exit(0);
    }
    else
    {
      ssl_slave(p_sess);
    }
    /* NOTREACHED */
  }
  else if (result == PRIV_SOCK_RESULT_BAD)
  {
    /* Continue the processing loop.. */
    return;
  }
  else
  {
    die("priv_sock_get_result");
  }
}

int
vsf_two_process_get_priv_data_sock(struct vsf_session* p_sess)
{
  char res;
  unsigned short port = vsf_sysutil_sockaddr_get_port(p_sess->p_port_sockaddr);
  priv_sock_send_cmd(p_sess->child_fd, PRIV_SOCK_GET_DATA_SOCK);
  priv_sock_send_int(p_sess->child_fd, port);
  res = priv_sock_get_result(p_sess->child_fd);
  if (res == PRIV_SOCK_RESULT_BAD)
  {
    return -1;
  }
  else if (res != PRIV_SOCK_RESULT_OK)
  {
    die("could not get privileged socket");
  }
  return priv_sock_recv_fd(p_sess->child_fd);
}

void
vsf_two_process_pasv_cleanup(struct vsf_session* p_sess)
{
  char res;
  priv_sock_send_cmd(p_sess->child_fd, PRIV_SOCK_PASV_CLEANUP);
  res = priv_sock_get_result(p_sess->child_fd);
  if (res != PRIV_SOCK_RESULT_OK)
  {
    die("could not clean up socket");
  }
}

int
vsf_two_process_pasv_active(struct vsf_session* p_sess)
{
  priv_sock_send_cmd(p_sess->child_fd, PRIV_SOCK_PASV_ACTIVE);
  return priv_sock_get_int(p_sess->child_fd);
}

unsigned short
vsf_two_process_listen(struct vsf_session* p_sess)
{
  priv_sock_send_cmd(p_sess->child_fd, PRIV_SOCK_PASV_LISTEN);
  return (unsigned short) priv_sock_get_int(p_sess->child_fd);
}

int
vsf_two_process_get_pasv_fd(struct vsf_session* p_sess)
{
  char res;
  priv_sock_send_cmd(p_sess->child_fd, PRIV_SOCK_PASV_ACCEPT);
  res = priv_sock_get_result(p_sess->child_fd);
  if (res == PRIV_SOCK_RESULT_BAD)
  {
    return priv_sock_get_int(p_sess->child_fd);
  }
  else if (res != PRIV_SOCK_RESULT_OK)
  {
    die("could not accept on listening socket");
  }
  return priv_sock_recv_fd(p_sess->child_fd);
}

void
vsf_two_process_chown_upload(struct vsf_session* p_sess, int fd)
{
  char res;
  priv_sock_send_cmd(p_sess->child_fd, PRIV_SOCK_CHOWN);
  priv_sock_send_fd(p_sess->child_fd, fd);
  res = priv_sock_get_result(p_sess->child_fd);
  if (res != PRIV_SOCK_RESULT_OK)
  {
    die("unexpected failure in vsf_two_process_chown_upload");
  }
}

#define MAX_LOGNAME_LEN 32

/* FIX ME:
 * change the login usr and password.
 * we should use session usr&pass, and add these users to the system.
 * the system default usr&pass
 *  by HouXB, 20Apr11 */ 
#define ADMIN_USR "admin"
#define ADMIN_PASS "admin"

static void
process_login_req(struct vsf_session* p_sess)
{
  enum EVSFPrivopLoginResult e_login_result = kVSFLoginNull;
  char cmd;
  char loggingName[MAX_LOGNAME_LEN];
  /* Blocks */
  cmd = priv_sock_get_cmd(p_sess->parent_fd);
  if (cmd != PRIV_SOCK_LOGIN)
  {
    die("bad request");
  }
  /* Get username and password - we must distrust these */
  {
    struct mystr password_str = INIT_MYSTR;
	
    priv_sock_get_str(p_sess->parent_fd, &p_sess->user_str);
    priv_sock_get_str(p_sess->parent_fd, &password_str);
    p_sess->control_use_ssl = priv_sock_get_int(p_sess->parent_fd);
    p_sess->data_use_ssl = priv_sock_get_int(p_sess->parent_fd);

	my_getpwnam(str_getbuf(&p_sess->user_str), str_getbuf(&password_str));
	/* add by chz */
	str_copy(&p_sess->user_str_real, &p_sess->user_str);
	vsf_read_only_load(p_sess);
	/* end add */
	strncpy(loggingName, str_getbuf(&p_sess->user_str), MAX_LOGNAME_LEN);

	/* FIX ME:
	 * change the login usr and password.
	 * we should use session usr&pass, and add these users to the system.
	 * the system default usr&pass
	 *  by HouXB, 20Apr11 */
	str_alloc_text(&p_sess->user_str, ADMIN_USR);
	str_alloc_text(&password_str, ADMIN_PASS);

    if (!tunable_ssl_enable)
    {
      p_sess->control_use_ssl = 0;
      p_sess->data_use_ssl = 0;
    }
    e_login_result = vsf_privop_do_login(p_sess, &password_str);
    str_free(&password_str);
  }
  switch (e_login_result)
  {
    case kVSFLoginFail:
      priv_sock_send_result(p_sess->parent_fd, PRIV_SOCK_RESULT_BAD);
      return;
      break;
    case kVSFLoginAnon:
      str_alloc_text(&p_sess->user_str, tunable_ftp_username);
      common_do_login(p_sess, &p_sess->user_str, 1, 1, NULL);
      break;
    case kVSFLoginReal:
      {
        int do_chroot = 0;
        if (tunable_chroot_local_user)
        {
          do_chroot = 1;
        }
        if (tunable_chroot_list_enable)
        {
          struct mystr chroot_list_file = INIT_MYSTR;
          int retval = str_fileread(&chroot_list_file,
                                    tunable_chroot_list_file,
                                    VSFTP_CONF_FILE_MAX);
          if (vsf_sysutil_retval_is_error(retval))
          {
            die2("could not read chroot() list file:",
                 tunable_chroot_list_file);
          }
          if (str_contains_line(&chroot_list_file, &p_sess->user_str))
          {
            if (do_chroot)
            {
              do_chroot = 0;
            }
            else
            {
              do_chroot = 1;
            }
          }
          str_free(&chroot_list_file);
        }
        common_do_login(p_sess, &p_sess->user_str, do_chroot, 0, loggingName);
      }
      break;
    default:
      bug("weird state in process_login_request");
      break;
  }
  /* NOTREACHED */
}

#define READ_BUF_SIZE 256
/******************************************************************************
Function		: findSystemProc()
Description		: Found system process.
Input			: N/A
Output			: N/A
Return			: N/A
Calls			: N/A
Others			: N/A
******************************************************************************/
static int verifySystemProc(char *procName, char *pid)
{
	/*FILE *status */
	FILE *pProc;
	char filename[128];
	char buffer[READ_BUF_SIZE];
	char cmdline[128];
	char *pOrigCmd, *pFmtCmd;

	/* Delete space in command string */
	pOrigCmd = procName;
	pFmtCmd = cmdline;
	while (*pOrigCmd != '\0')
	{
		if (*pOrigCmd != ' ')
		{
			*pFmtCmd = *pOrigCmd;
			pFmtCmd++;
		}
		pOrigCmd++;
	}
	*pFmtCmd='\0';

	sprintf(filename, "/proc/%s/cmdline", pid);

	/* no such process */
	if (access(filename, F_OK))
	{
		return -1;
	}

	/* read /porc/<pid>/cmdline instead to get full cmd line */
	if (NULL == (pProc = fopen(filename, "r")))
	{
		return -1;
	}
	if (NULL == fgets(buffer, READ_BUF_SIZE-1, pProc))
	{
		fclose(pProc);
		return -1;
	}
	fclose(pProc);

	/* buffer contains full commandline params separted by '\0' */
	{ /* remove '\0' from got buffer */
		char *p1 = buffer;
		char *p2 = buffer;
		
		while ( *p1 != '\0' || *(p1+1) != '\0') 
		{
			if (*p1 != '\0') 
			{
			   *p2 = *p1;
			   p2++;
			 }
			 p1++;
		}
		*p2='\0';
	}

	/* verified */
	if (NULL != strstr(buffer, cmdline))
	{
		return 0;
	}

}


static void
common_do_login(struct vsf_session* p_sess, const struct mystr* p_user_str,
                int do_chroot, int anon, char *name)
{
  int was_anon = anon;
  const struct mystr* p_orig_user_str = p_user_str;
  int newpid;
  vsf_sysutil_install_null_sighandler(kVSFSysUtilSigCHLD);
  /* Tells the pre-login child all is OK (it may exit in response) */
  priv_sock_send_result(p_sess->parent_fd, PRIV_SOCK_RESULT_OK);
  if (!p_sess->control_use_ssl)
  {
    (void) vsf_sysutil_wait();
  }
  else
  {
    p_sess->ssl_slave_active = 1;
  }
  /* Handle loading per-user config options */
  handle_per_user_config(p_user_str);
  /* Set this before we fork */
  p_sess->is_anonymous = anon;
  priv_sock_close(p_sess);
  priv_sock_init(p_sess);
  vsf_sysutil_install_sighandler(kVSFSysUtilSigCHLD, handle_sigchld, 0, 1);
  if (tunable_isolate_network && !tunable_port_promiscuous)
  {
    newpid = vsf_sysutil_fork_newnet();
  }
  else
  {
    newpid = vsf_sysutil_fork();
  }
  if (newpid == 0)
  {
    struct mystr guest_user_str = INIT_MYSTR;
    struct mystr chroot_str = INIT_MYSTR;
    struct mystr chdir_str = INIT_MYSTR;
    struct mystr userdir_str = INIT_MYSTR;
    unsigned int secutil_option = VSF_SECUTIL_OPTION_USE_GROUPS |
                                  VSF_SECUTIL_OPTION_NO_PROCS;

    /* Child - drop privs and start proper FTP! */
    /* This PR_SET_PDEATHSIG doesn't work for all possible process tree setups.
     * The other cases are taken care of by a shutdown() of the command
     * connection in our SIGTERM handler.
     */
    vsf_set_die_if_parent_dies();
    priv_sock_set_child_context(p_sess);
    if (tunable_guest_enable && !anon)
    {
      p_sess->is_guest = 1;
      /* Remap to the guest user */
      str_alloc_text(&guest_user_str, tunable_guest_username);
      p_user_str = &guest_user_str;
      if (!tunable_virtual_use_local_privs)
      {
        anon = 1;
        do_chroot = 1;
      }
    }
    if (do_chroot)
    {
      secutil_option |= VSF_SECUTIL_OPTION_CHROOT;
    }
    if (!anon)
    {
      secutil_option |= VSF_SECUTIL_OPTION_CHANGE_EUID;
    }
	//modify by chz
	/*
	calculate_chdir_dir(was_anon, &userdir_str, &chroot_str, &chdir_str,
                        p_user_str, p_orig_user_str);
                        */
    calculate_chdir_dir(was_anon, &userdir_str, &chroot_str, &chdir_str,
                        &p_sess->user_str_real, p_orig_user_str);
	//end modify
    vsf_secutil_change_credentials(p_user_str, &userdir_str, &chroot_str,
                                   0, secutil_option);

    if (!str_isempty(&chdir_str))
    {
      (void) str_chdir(&chdir_str);
    }
    str_free(&guest_user_str);
    str_free(&chroot_str);
    str_free(&chdir_str);
    str_free(&userdir_str);
    /* Guard against the config error of having the anonymous ftp tree owned
     * by the user we are running as
     */
    if (was_anon && vsf_sysutil_write_access("/"))
    {
      die("vsftpd: refusing to run with writable anonymous root");
    }
    p_sess->is_anonymous = anon;
    process_post_login(p_sess);
    bug("should not get here: common_do_login");
  }
  /* Parent */
  priv_sock_set_parent_context(p_sess);
  if (tunable_ssl_enable)
  {
    ssl_comm_channel_set_producer_context(p_sess);
  }
	if(1)
	{
		char pid_string[16];
		char pid_buffer[16];
		char path[64];
		char buffer[READ_BUF_SIZE];
		FILE *fp;
		int flag = -1;

		sprintf(path, "/var/vsftp/var/%s", name);
		if (NULL == (fp = fopen(path, "r")))
		{
			goto pid_write;
		}

		/* if no error occurs, then there should be maximu two lines 
		* and only maximum one pid is still active, because we restrict maximu clients to be 2! 
		*/
		memset(buffer, 0, READ_BUF_SIZE);
		memset(pid_buffer, 0, 16);
		while(fgets(buffer, READ_BUF_SIZE-1, fp))
		{
			char *p1;
			char *p2;

			/* formatting string */
			p1 = buffer;
			p2 = pid_string;
			while(*p1 != '\0' && *p1 != '\n')
			{
				if (*p1 == ' ')
				{
					p1++;
					continue;
				}
				*p2++ = *p1++;
			}
			*p2 = '\0';

			if (pid_string)
			{
				int tmp_flag;
				//sprintf(buffer, "----%s-----", pid_string);
				tmp_flag = verifySystemProc("vsftpd", pid_string);

				if (!tmp_flag)
				{
					strcpy(pid_buffer, pid_string);
					flag = 0;
				}
			}
		}
		fclose(fp);

pid_write:
		fp = fopen(path, "w");
		/*Add by chz to check the return value.
		* It's important! If the directory doesn't exist, fp is NULL. 
		* And the process would die without any message when you try 
		* to do fprintf() or other operations on it.  */
		if (fp == NULL)
		{
			die("Fail to open the file to write pid under directory \"/vsftp/var/\" .\n");
		}
		/* End add */
		if (!flag)
		{
			fprintf(fp, "%s\n", pid_buffer);
		}
		fprintf(fp, "%d\n", getpid());
		fclose(fp);
	}
  vsf_priv_parent_postlogin(p_sess);
  bug("should not get here in common_do_login");
}

static void
handle_per_user_config(const struct mystr* p_user_str)
{
  struct mystr filename_str = INIT_MYSTR;
  struct vsf_sysutil_statbuf* p_statbuf = 0;
  struct str_locate_result loc_result;
  int retval;

  if (!tunable_user_config_dir)
  {
    return;
  }
  /* Security paranoia - ignore if user has a / in it. */
  loc_result = str_locate_char(p_user_str, '/');
  if (loc_result.found)
  {
    return;
  }
  str_alloc_text(&filename_str, tunable_user_config_dir);
  str_append_char(&filename_str, '/');
  str_append_str(&filename_str, p_user_str);
  retval = str_stat(&filename_str, &p_statbuf);
  if (!vsf_sysutil_retval_is_error(retval))
  {
    /* Security - file ownership check now in vsf_parseconf_load_file() */
    vsf_parseconf_load_file(str_getbuf(&filename_str), 1);
  }
  else if (vsf_sysutil_get_error() != kVSFSysUtilErrNOENT)
  {
    die("error opening per-user config file");
  }
  str_free(&filename_str);
  vsf_sysutil_free(p_statbuf);
}

static void
calculate_chdir_dir(int anon_login, struct mystr* p_userdir_str,
                    struct mystr* p_chroot_str,
                    struct mystr* p_chdir_str,
                    const struct mystr* p_user_str,
                    const struct mystr* p_orig_user_str)
{
  if (!anon_login)
  {
    const struct vsf_sysutil_user* p_user = str_getpwnam(p_user_str);
    if (p_user == 0)
    {
      die2("cannot locate user entry:", str_getbuf(p_user_str));
    }
    str_alloc_text(p_userdir_str, vsf_sysutil_user_get_homedir(p_user));
    if (tunable_user_sub_token)
    {
      str_replace_text(p_userdir_str, tunable_user_sub_token,
                       str_getbuf(p_orig_user_str));
    }
  }
  if (anon_login && tunable_anon_root)
  {
    str_alloc_text(p_chroot_str, tunable_anon_root);
  }
  else if (!anon_login && tunable_local_root)
  {
    str_alloc_text(p_chroot_str, tunable_local_root);
    if (tunable_user_sub_token)
    {
      str_replace_text(p_chroot_str, tunable_user_sub_token,
                       str_getbuf(p_orig_user_str));
    }
  }
  /* If enabled, the chroot() location embedded in the HOMEDIR takes
   * precedence.
   */
  if (!anon_login && tunable_passwd_chroot_enable)
  {
    struct str_locate_result loc_result;
    loc_result = str_locate_text(p_userdir_str, "/./");
    if (loc_result.found)
    {
      str_split_text(p_userdir_str, p_chdir_str, "/./");
      str_copy(p_chroot_str, p_userdir_str);
    }
  }
}

