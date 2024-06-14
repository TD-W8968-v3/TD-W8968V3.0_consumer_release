/*
<:label-BRCM:2012:proprietary:standard 

 This program is the proprietary software of Broadcom Corporation and/or its
 licensors, and may only be used, duplicated, modified or distributed pursuant
 to the terms and conditions of a separate, written license agreement executed
 between you and Broadcom (an "Authorized License").  Except as set forth in
 an Authorized License, Broadcom grants no license (express or implied), right
 to use, or waiver of any kind with respect to the Software, and Broadcom
 expressly reserves all rights in and to the Software and all intellectual
 property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE
 NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY
 BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.

 Except as expressly set forth in the Authorized License,

 1. This program, including its structure, sequence and organization,
    constitutes the valuable trade secrets of Broadcom, and you shall use
    all reasonable efforts to protect the confidentiality thereof, and to
    use this information only in connection with your use of Broadcom
    integrated circuit products.

 2. TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
    AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
    WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
    RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND
    ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT,
    FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR
    COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE
    TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF USE OR
    PERFORMANCE OF THE SOFTWARE.

 3. TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR
    ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
    INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY
    WAY RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN
    IF BROADCOM HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES;
    OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE
    SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS
    SHALL APPLY NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF ANY
    LIMITED REMEDY.
:>

*/
/***********************************************************************/
/*                                                                     */
/*   MODULE:  6318_common.h                                           */
/*   DATE:    11/09/10                                                 */
/*   PURPOSE: Define addresses of major hardware components of         */
/*            BCM6318                                                 */
/*                                                                     */
/***********************************************************************/
#ifndef __BCM6318_MAP_COMMON_H
#define __BCM6318_MAP_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__KERNEL__) || defined (_CFE_) || defined(FAP_4KE)
/* Access to registers from kernelspace */
#define REG_BASE                 0xb0000000
#else
/* Access to registers from userspace, see bcm_mmap.h for api */
#define REG_BASE                 (bcm_mmap_info.mmap_addr)
#define BCM_MMAP_INFO_BASE       0x10000000
#define BCM_MMAP_INFO_SIZE       0x01000000
#endif

#define PERF_BASE                   (REG_BASE + 0x00000000)  /* chip control */
#define TIMR_BASE                   (REG_BASE + 0x00000040)  /* timer registers */
#define GPIO_BASE                   (REG_BASE + 0x00000080)  /* gpio registers */
#define UART_BASE                   (REG_BASE + 0x00000100)  /* uart registers */
#define OTP_BASE                    (REG_BASE + 0x00000180)
#define LED_BASE                    (REG_BASE + 0x00000200)  /* LED control registers */
#define MISC_BASE                   (REG_BASE + 0x00000280)  /* Miscellaneous Registers */
#define XTAL_BASE                   (REG_BASE + 0x00000800)  /* XTAL registers */
#define PLL_PWR_CTRL_BASE           (REG_BASE + 0x00000880)  /* PLL Power Control Registers */
#define STRAP_BASE                  (REG_BASE + 0x00000900)  /* Miscellaneous Registers */
#define AVS_MONITOR                 (REG_BASE + 0x00001000)
#define HSSPIM_BASE                 (REG_BASE + 0x00003000)  /* High-Speed SPI registers */
#define SDRAM_CTRL_BASE             (REG_BASE + 0x00004000)
#ifdef __KERNEL__
#define USB_EHCI_BASE               0x10005000  /* USB host registers */
#define USB_OHCI_BASE               0x10005100  /* USB host registers */
#endif
#define USBH_CFG_BASE               (REG_BASE + 0x00005200)
#define USB_CTL_BASE                (REG_BASE + 0x00006000)  /* USB 2.0 device control */
#define USB_DMA_BASE                (REG_BASE + 0x00006800)  /* USB 2.0 device DMA regiseters */
#define SAR_BASE                    (REG_BASE + 0x00008000)
#define SAR_DMA_BASE                (REG_BASE + 0x0000c000)  /* XTM SAR DMA control */
#define PCIE_BASE                   (REG_BASE + 0x00010000)
#define SWITCH_BASE                 (REG_BASE + 0x00080000)
#define SWITCH_DMA_BASE             (REG_BASE + 0x00088000)


/*
#####################################################################
# System PLL Control Register
#####################################################################
*/


/*
#####################################################################
# GPIO Control Registers
#####################################################################
*/


/*
#####################################################################
# Miscellaneous Registers
#####################################################################
*/
#define   STRAP_OVERRIDE_BUS                             (0x0)
#define   STRAP_OVERRIDE_BUS_SDRC_MASK_SHIFT              26
#define   STRAP_OVERRIDE_BUS_SDRC_MASK                   (0x3 << STRAP_OVERRIDE_BUS_SDRC_MASK_SHIFT)
#define   STRAP_OVERRIDE_BUS_SDR_DDR1_SELECT_MASK_SHIFT   17
#define   STRAP_OVERRIDE_BUS_SDR_DDR1_SELECT_MASK         (1 << STRAP_OVERRIDE_BUS_SDR_DDR1_SELECT_MASK_SHIFT)

/*
#####################################################################
# Memory Control Registers
#####################################################################
*/
#define SDR_CFG_SDR_CFG      0x00
#define SDR_CFG_PRI_CFG      0x14
#define SDR_CFG_PID_SELECT0  0x18
#define SDR_CFG_PID_SELECT1  0x1C
#define SDR_CFG_AUTO_REFRESH 0x24
#define SDR_CFG_TIMING_PARAM 0x28
    
#define SDR_PRECHARGE_CMD    0x80
#define SDR_AUTOREFRESH_CMD  0x90
#define SDR_LMR_CMD          0xA0
#define SDR_LMRX_CMD         0xA4

/*
#####################################################################
# UART Control Registers
#####################################################################
*/
#define UART0CONTROL     0x01
#define UART0CONFIG      0x02
#define UART0RXTIMEOUT   0x03
#define UART0BAUD        0x04
#define UART0FIFOCFG     0x0a
#define UART0INTMASK     0x10
#define UART0INTSTAT     0x12
#define UART0DATA        0x17

#define BRGEN            0x80   /* Control register bit defs */
#define TXEN             0x40
#define RXEN             0x20
#define LOOPBK           0x10
#define TXPARITYEN       0x08
#define TXPARITYEVEN     0x04
#define RXPARITYEN       0x02
#define RXPARITYEVEN     0x01

#define XMITBREAK        0x40   /* Config register */
#define BITS5SYM         0x00
#define BITS6SYM         0x10
#define BITS7SYM         0x20
#define BITS8SYM         0x30
#define ONESTOP          0x07
#define TWOSTOP          0x0f

#define DELTAIP         0x0001
#define TXUNDERR        0x0002
#define TXOVFERR        0x0004
#define TXFIFOTHOLD     0x0008
#define TXREADLATCH     0x0010
#define TXFIFOEMT       0x0020
#define RXUNDERR        0x0040
#define RXOVFERR        0x0080
#define RXTIMEOUT       0x0100
#define RXFIFOFULL      0x0200
#define RXFIFOTHOLD     0x0400
#define RXFIFONE        0x0800
#define RXFRAMERR       0x1000
#define RXPARERR        0x2000
#define RXBRK           0x4000


/*
#####################################################################
# GPIO Control Registers
#####################################################################
*/
#define GPIO_DATA_HI               (GPIO_BASE + 0x8)
#define GPIO_DATA                  (GPIO_BASE + 0xC)


#ifdef __cplusplus
}
#endif

#endif
