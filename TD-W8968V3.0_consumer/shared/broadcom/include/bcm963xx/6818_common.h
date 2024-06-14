/*
    <:copyright-BRCM:2011:proprietary:standard
    
       Copyright (c) 2011 Broadcom Corporation
       All Rights Reserved
    
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
/*   MODULE:  6818_common.h                                             */
/*   DATE:    11/28/11                                                 */
/*   PURPOSE: Define addresses of major hardware components of         */
/*            BCM6818                                                  */
/*                                                                     */
/***********************************************************************/
#ifndef __BCM6818_MAP_COMMON_H
#define __BCM6818_MAP_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#if defined (__KERNEL__) || defined (_CFE_) || defined(FAP_4KE)
/* Access to registers from kernelspace */
#define REG_BASE                 0xb0000000
#else
/* Access to registers from userspace, see bcm_mmap.h for api */
#define REG_BASE                 (bcm_mmap_info.mmap_addr)
#define BCM_MMAP_INFO_BASE       0x10000000
#define BCM_MMAP_INFO_SIZE       0x01000000
#endif

#define PERF_BASE                (REG_BASE + 0x00000000)
#define TIMR_BASE                (REG_BASE + 0x00000040)
#define NAND_INTR_BASE           (REG_BASE + 0x00000070)
#define GPIO_BASE                (REG_BASE + 0x00000080)
#define UART_BASE                (REG_BASE + 0x00000100)
#define UART1_BASE               (REG_BASE + 0x00000120)
//#define I2C_BASE                 (REG_BASE + 0x00000180)
#define OTP_BASE                 (REG_BASE + 0x00000400)
#define UBUS_STAT_BASE           (REG_BASE + 0x00000500)
#define SPI_BASE                 (REG_BASE + 0x00000800)
#define HSSPIM_BASE              (REG_BASE + 0x00001000)
#define MISC_BASE                (REG_BASE + 0x00001800)
#define NAND_REG_BASE            (REG_BASE + 0x00002000)
#define NAND_ACC_CONTROL         (REG_BASE + 0x00002040)
#define NAND_CACHE_BASE          (REG_BASE + 0x00002200)
#define MPI_BASE                 (REG_BASE + 0x00002400)
#define USB_CTL_BASE             (REG_BASE + 0x00002500)
#ifdef __KERNEL__
#define USB_EHCI_BASE                        0x10002600
#define USB_OHCI_BASE                        0x10002700
#endif
#define USBH_CFG_BASE            (REG_BASE + 0x00002800)
#define IPSEC_BASE               (REG_BASE + 0x00002900)
#define MEMC_BASE                (REG_BASE + 0x00003000)
#define GPON_BASE                (REG_BASE + 0x00004000)
#define GPON_SERDES_BASE         (REG_BASE + 0x00004800)
#define GPON_SERDES_MISC         (GPON_SERDES_BASE + 0x50)
#define USB_DMA_BASE             (REG_BASE + 0x0000c000)
#define GPON_DMA_BASE            (REG_BASE + 0x0000c800)
#define IPSEC_DMA_BASE           (REG_BASE + 0x0000d000)
#define SWITCH_DMA_BASE          (REG_BASE + 0x0000d800)
#define SWITCH_DMA_CONFIG        (REG_BASE + 0x0000da00)
#define SWITCH_DMA_STATE         (REG_BASE + 0x0000dc00)
#define GMAC_BASE                (REG_BASE + 0x0000e000)
#define GMAC_DMA_BASE            (REG_BASE + 0x0000e800)
#define PCIE_MEM1M_BASE          (REG_BASE + 0x00100000)
#define APM_BASE                 (REG_BASE + 0x00200000)
#define PCM_BASE                 (REG_BASE + 0x00200200)
#define APM_HVG_BASE             (REG_BASE + 0x00200300)
#define APM_HVG_BASE_REG_15      (REG_BASE + 0x00200408)
#define APM_HVG_BASE_REG_19      (REG_BASE + 0x00200488)
#define APM_IUDMA_BASE           (REG_BASE + 0x00200800)
#define APM_PCM_DMA_BASE         (REG_BASE + 0x00200800)  /* APM/PCM DMA control registers */
#define BMU_BASE                 (REG_BASE + 0x00201000)
#define APM_PICO_IMEM_BASE       (REG_BASE + 0x00210000)
#define PCIE_BASE                (REG_BASE + 0x006e0000)
#define SWITCH_BASE              (REG_BASE + 0x00700000)

#define FAP0_BASE                (REG_BASE + 0x00800000)
#define FAP0_CNTRL_BASE          (REG_BASE + 0x00801000)
#define FAP0_IN_FIFO_BASE        (REG_BASE + 0x00801100)
#define FAP0_OUT_FIFO_BASE       (REG_BASE + 0x00801200)
#define FAP0_DMA0_BASE           (REG_BASE + 0x00801300)
#define FAP0_DMA1_BASE           (REG_BASE + 0x00801320)
#define FAP0_TOKEN_BASE          (REG_BASE + 0x00801400)
#define FAP0_MSG_BASE            (REG_BASE + 0x00801700)
#define FAP0_DQM_BASE            (REG_BASE + 0x00801800)
#define FAP0_DQM_CNTRL_BASE      (REG_BASE + 0x00801a00)
#define FAP0_DQM_DATA_BASE       (REG_BASE + 0x00801c00)
#define FAP0_DQM_MIB_BASE        (REG_BASE + 0x00802000)
#define FAP0_QSM_UBUS_BASE       (REG_BASE + 0x00804000)
#define FAP0_QSM_SMI_BASE        (REG_BASE + 0x00c04000)
#define FAP0_PSM_BASE            (REG_BASE + 0x00820000)
#define FAP0_SMISB               (REG_BASE + 0x00c00000)

#define FAP1_BASE                (REG_BASE + 0x00a00000)
#define FAP1_CNTRL_BASE          (REG_BASE + 0x00a01000)
#define FAP1_IN_FIFO_BASE        (REG_BASE + 0x00a01100)
#define FAP1_OUT_FIFO_BASE       (REG_BASE + 0x00a01200)
#define FAP1_DMA0_BASE           (REG_BASE + 0x00a01300)
#define FAP1_DMA1_BASE           (REG_BASE + 0x00a01320)
#define FAP1_TOKEN_BASE          (REG_BASE + 0x00a01400)
#define FAP1_MSG_BASE            (REG_BASE + 0x00a01700)
#define FAP1_DQM_BASE            (REG_BASE + 0x00a01800)
#define FAP1_DQM_CNTRL_BASE      (REG_BASE + 0x00a01a00)
#define FAP1_DQM_DATA_BASE       (REG_BASE + 0x00a01c00)
#define FAP1_DQM_MIB_BASE        (REG_BASE + 0x00a02000)
#define FAP1_QSM_UBUS_BASE       (REG_BASE + 0x00a04000)
#define FAP1_QSM_SMI_BASE        (REG_BASE + 0x00e04000)
#define FAP1_PSM_BASE            (REG_BASE + 0x00a20000)
#define FAP1_SMISB               (REG_BASE + 0x00e00000)

#define PCIE_MEM2M_BASE          0xa0000000

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
#define GPIO_DATA_HI               (GPIO_BASE + 0x8)
#define GPIO_DATA                  (GPIO_BASE + 0xC)
#define GPIO_SWREG_CONFIG       0x1c
#define GPIO_LIN_VREG_ADJ_SHIFT 0x0
#define GPIO_LIN_VREG_ADJ_MASK  (0xf<<GPIO_LIN_VREG_ADJ_SHIFT)
#define GPIO_SW_VREG_SEL_SHIFT	0x8
#define GPIO_SW_VREG_SEL_MASK	(0xf<<GPIO_SW_VREG_SEL_SHIFT)

/*
#####################################################################
# Miscellaneous Registers
#####################################################################
*/
#define MISC_MEMC_CONTROL                       0x10
#define MISC_MEMC_CONTROL_MC_UBUS_ASYNC_MODE    (1<<3)
#define MISC_MEMC_CONTROL_MC_LMB_ASYNC_MODE     (1<<2)

#define MISC_STRAP_BUS                          0x14
#define MISC_STRAP_BUS_MIPS_PLL_FVCO_SHIFT      27
#define MISC_STRAP_BUS_MIPS_PLL_FVCO_MASK       (0x1F<<MISC_STRAP_BUS_MIPS_PLL_FVCO_SHIFT)
#define MISC_STRAP_BUS_DDR2_DDR3_N_SELECT       (1<<23)


/*
#####################################################################
# Memory Control Registers
#####################################################################
*/
#define MEMC_CTL_CNFG                           0x000 
#define MEMC_CTL_CSST                           0x004 
#define MEMC_CTL_CSEND                          0x008 
#define MEMC_CTL_ROW00_0                        0x010 
#define MEMC_CTL_ROW00_1                        0x014 
#define MEMC_CTL_ROW01_0                        0x018 
#define MEMC_CTL_ROW01_1                        0x01c 
#define MEMC_CTL_ROW20_0                        0x030 
#define MEMC_CTL_ROW20_1                        0x034 
#define MEMC_CTL_ROW21_0                        0x038 
#define MEMC_CTL_ROW21_1                        0x03c 
#define MEMC_CTL_COL00_0                        0x050 
#define MEMC_CTL_COL00_1                        0x054 
#define MEMC_CTL_COL01_0                        0x058 
#define MEMC_CTL_COL01_1                        0x05c 
#define MEMC_CTL_COL20_0                        0x070 
#define MEMC_CTL_COL20_1                        0x074 
#define MEMC_CTL_COL21_0                        0x078 
#define MEMC_CTL_COL21_1                        0x07c 
#define MEMC_CTL_BNK10                          0x090 
#define MEMC_CTL_BNK32                          0x094 
#define MEMC_CTL_DCMD                           0x100 
#define MEMC_CTL_DMODE_0                        0x104 
#define MEMC_CTL_DMODE_2                        0x108 
#define MEMC_CTL_CLKS                           0x10c 
#define MEMC_CTL_ODT                            0x110 
#define MEMC_CTL_TIM1_0                         0x114 
#define MEMC_CTL_TIM1_1                         0x118 
#define MEMC_CTL_TIM2                           0x11c 
#define MEMC_CTL_CTL_CRC                        0x120 
#define MEMC_CTL_DOUT_CRC                       0x124 
#define MEMC_CTL_DIN_CRC                        0x128 
#define MEMC_CTL_DRAM_CFG                       0x134 
#define MEMC_CTL_STAT                           0x138 
    
#define PHY_CONTROL_REGS_REVISION               0x200 
#define PHY_CONTROL_REGS_CLK_PM_CTRL            0x204 
#define PHY_CONTROL_REGS_PLL_STATUS             0x210 
#define PHY_CONTROL_REGS_PLL_CONFIG             0x214 
#define PHY_CONTROL_REGS_PLL_PRE_DIVIDER        0x218 
#define PHY_CONTROL_REGS_PLL_DIVIDER            0x21c 
#define PHY_CONTROL_REGS_PLL_CONTROL1           0x220 
#define PHY_CONTROL_REGS_PLL_CONTROL2           0x224 
#define PHY_CONTROL_REGS_PLL_SS_EN              0x228 
#define PHY_CONTROL_REGS_PLL_SS_CFG             0x22c 
#define PHY_CONTROL_REGS_STATIC_VDL_OVERRIDE    0x230 
#define PHY_CONTROL_REGS_DYNAMIC_VDL_OVERRIDE   0x234 
#define PHY_CONTROL_REGS_IDLE_PAD_CONTROL       0x238 
#define PHY_CONTROL_REGS_ZQ_PVT_COMP_CTL        0x23c 
#define PHY_CONTROL_REGS_DRIVE_PAD_CTL          0x240 
    
#define PHY_BYTE_LANE_0_REVISION                0x300 
#define PHY_BYTE_LANE_0_VDL_CALIBRATE           0x304 
#define PHY_BYTE_LANE_0_VDL_STATUS              0x308 
    #define PHY_BYTE_LANE_0_VDL_STATUS_CALIB_TOTAL_STEP_MAX  31
    #define PHY_BYTE_LANE_0_VDL_STATUS_CALIB_TOTAL_STEP_STRT 8
    #define PHY_BYTE_LANE_0_VDL_STATUS_CALIB_TOTAL_STEP_MASK (0x1f << PHY_BYTE_LANE_0_VDL_STATUS_CALIB_TOTAL_STEP_STRT)
#define PHY_BYTE_LANE_0_VDL_OVERRIDE_0          0x310 
#define PHY_BYTE_LANE_0_VDL_OVERRIDE_1          0x314 
#define PHY_BYTE_LANE_0_VDL_OVERRIDE_2          0x318 
#define PHY_BYTE_LANE_0_VDL_OVERRIDE_3          0x31c 
#define PHY_BYTE_LANE_0_VDL_OVERRIDE_4          0x320 
#define PHY_BYTE_LANE_0_VDL_OVERRIDE_5          0x324 
#define PHY_BYTE_LANE_0_VDL_OVERRIDE_6          0x328 
#define PHY_BYTE_LANE_0_VDL_OVERRIDE_7          0x32c 
#define PHY_BYTE_LANE_0_READ_CONTROL            0x330 
#define PHY_BYTE_LANE_0_READ_FIFO_STATUS        0x334 
#define PHY_BYTE_LANE_0_READ_FIFO_CLEAR         0x338 
#define PHY_BYTE_LANE_0_IDLE_PAD_CONTROL        0x33c 
#define PHY_BYTE_LANE_0_DRIVE_PAD_CTL           0x340 
#define PHY_BYTE_LANE_0_CLOCK_PAD_DISABLE       0x344 
#define PHY_BYTE_LANE_0_WR_PREAMBLE_MODE        0x348 
    
#define PHY_BYTE_LANE_1_REVISION                0x400 
#define PHY_BYTE_LANE_1_VDL_CALIBRATE           0x404 
#define PHY_BYTE_LANE_1_VDL_STATUS              0x408 
#define PHY_BYTE_LANE_1_VDL_OVERRIDE_0          0x410 
#define PHY_BYTE_LANE_1_VDL_OVERRIDE_1          0x414 
#define PHY_BYTE_LANE_1_VDL_OVERRIDE_2          0x418 
#define PHY_BYTE_LANE_1_VDL_OVERRIDE_3          0x41c 
#define PHY_BYTE_LANE_1_VDL_OVERRIDE_4          0x420 
#define PHY_BYTE_LANE_1_VDL_OVERRIDE_5          0x424 
#define PHY_BYTE_LANE_1_VDL_OVERRIDE_6          0x428 
#define PHY_BYTE_LANE_1_VDL_OVERRIDE_7          0x42c 
#define PHY_BYTE_LANE_1_READ_CONTROL            0x430 
#define PHY_BYTE_LANE_1_READ_FIFO_STATUS        0x434 
#define PHY_BYTE_LANE_1_READ_FIFO_CLEAR         0x438 
#define PHY_BYTE_LANE_1_IDLE_PAD_CONTROL        0x43c 
#define PHY_BYTE_LANE_1_DRIVE_PAD_CTL           0x440 
#define PHY_BYTE_LANE_1_CLOCK_PAD_DISABLE       0x444 
#define PHY_BYTE_LANE_1_WR_PREAMBLE_MODE        0x448 
    
#define PHY_BYTE_LANE_2_REVISION                0x500 
#define PHY_BYTE_LANE_2_VDL_CALIBRATE           0x504 
#define PHY_BYTE_LANE_2_VDL_STATUS              0x508 
#define PHY_BYTE_LANE_2_VDL_OVERRIDE_0          0x510 
#define PHY_BYTE_LANE_2_VDL_OVERRIDE_1          0x514 
#define PHY_BYTE_LANE_2_VDL_OVERRIDE_2          0x518 
#define PHY_BYTE_LANE_2_VDL_OVERRIDE_3          0x51c 
#define PHY_BYTE_LANE_2_VDL_OVERRIDE_4          0x520 
#define PHY_BYTE_LANE_2_VDL_OVERRIDE_5          0x524 
#define PHY_BYTE_LANE_2_VDL_OVERRIDE_6          0x528 
#define PHY_BYTE_LANE_2_VDL_OVERRIDE_7          0x52c 
#define PHY_BYTE_LANE_2_READ_CONTROL            0x530 
#define PHY_BYTE_LANE_2_READ_FIFO_STATUS        0x534 
#define PHY_BYTE_LANE_2_READ_FIFO_CLEAR         0x538 
#define PHY_BYTE_LANE_2_IDLE_PAD_CONTROL        0x53c 
#define PHY_BYTE_LANE_2_DRIVE_PAD_CTL           0x540 
#define PHY_BYTE_LANE_2_CLOCK_PAD_DISABLE       0x544 
#define PHY_BYTE_LANE_2_WR_PREAMBLE_MODE        0x548 
    
#define PHY_BYTE_LANE_3_REVISION                0x600 
#define PHY_BYTE_LANE_3_VDL_CALIBRATE           0x604 
#define PHY_BYTE_LANE_3_VDL_STATUS              0x608 
#define PHY_BYTE_LANE_3_VDL_OVERRIDE_0          0x610 
#define PHY_BYTE_LANE_3_VDL_OVERRIDE_1          0x614 
#define PHY_BYTE_LANE_3_VDL_OVERRIDE_2          0x618 
#define PHY_BYTE_LANE_3_VDL_OVERRIDE_3          0x61c 
#define PHY_BYTE_LANE_3_VDL_OVERRIDE_4          0x620 
#define PHY_BYTE_LANE_3_VDL_OVERRIDE_5          0x624 
#define PHY_BYTE_LANE_3_VDL_OVERRIDE_6          0x628 
#define PHY_BYTE_LANE_3_VDL_OVERRIDE_7          0x62c 
#define PHY_BYTE_LANE_3_READ_CONTROL            0x630 
#define PHY_BYTE_LANE_3_READ_FIFO_STATUS        0x634 
#define PHY_BYTE_LANE_3_READ_FIFO_CLEAR         0x638 
#define PHY_BYTE_LANE_3_IDLE_PAD_CONTROL        0x63c 
#define PHY_BYTE_LANE_3_DRIVE_PAD_CTL           0x640 
#define PHY_BYTE_LANE_3_CLOCK_PAD_DISABLE       0x644 
#define PHY_BYTE_LANE_3_WR_PREAMBLE_MODE        0x648 
    
#define MEMC_CTL_GCFG                           0x800 
    #define MEMC_CTL_GCFG_MEMINITDONE                   (1 << 8)
#define MEMC_CTL_VERS                           0x804 
#define MEMC_CTL_ARB                            0x80c 
#define MEMC_CTL_PI_GCF                         0x810 
#define MEMC_CTL_PI_UBUS_CTL                    0x814 
#define MEMC_CTL_PI_MIPS_CTL                    0x818 
#define MEMC_CTL_PI_DSL_MIPS_CTL                0x81c 
#define MEMC_CTL_PI_DSL_PHY_CTL                 0x820 
#define MEMC_CTL_PI_UBUS_ST                     0x824 
#define MEMC_CTL_PI_MIPS_ST                     0x828 
#define MEMC_CTL_PI_DSL_MIPS_ST                 0x82c 
#define MEMC_CTL_PI_DSL_PHY_ST                  0x830 
#define MEMC_CTL_PI_UBUS_SMPL                   0x834
    #define MEMC_CTL_PI_UBUS_SMPL_SAMPLING_PERIOD_SHIFT 28
#define MEMC_CTL_TESTMODE                       0x838 
#define MEMC_CTL_TEST_CFG1                      0x83c 
#define MEMC_CTL_TEST_PAT                       0x840 
#define MEMC_CTL_TEST_COUNT                     0x844 
#define MEMC_CTL_TEST_CURR_COUNT                0x848 
#define MEMC_CTL_TEST_ADDR_UPDT                 0x84c 
#define MEMC_CTL_TEST_ADDR                      0x850 
#define MEMC_CTL_TEST_DATA0_0                   0x854 
#define MEMC_CTL_TEST_DATA0_1                   0x858 
#define MEMC_CTL_TEST_DATA0_2                   0x85c 
#define MEMC_CTL_TEST_DATA0_3                   0x860 
#define MEMC_CTL_TEST_DATA1_0                   0x864 
#define MEMC_CTL_TEST_DATA1_1                   0x868 
#define MEMC_CTL_TEST_DATA1_2                   0x86c 
#define MEMC_CTL_TEST_DATA1_3                   0x870 
#define MEMC_CTL_REPLY_DATA0                    0x874 
#define MEMC_CTL_REPLY_DATA1                    0x878 
#define MEMC_CTL_REPLY_DATA2                    0x87c 
#define MEMC_CTL_REPLY_DATA3                    0x880 
#define MEMC_CTL_REPLY_STAT                     0x884 
#define MEMC_CTL_LBIST_CFG                      0x888 
#define MEMC_CTL_LBIST_SEED                     0x88c 
#define MEMC_CTL_PI_MIPS_SMPL                   0x890 
    #define MEMC_CTL_PI_MIPS_SMPL_SAMPLING_PERIOD_SHIFT 28


/*
#####################################################################
# MPI Control Registers
#####################################################################
*/
#define CS0BASE         0x00
#define CS0CNTL         0x04

/*
# CSxBASE settings
#   Size in low 4 bits
#   Base Address for match in upper 24 bits
#
*/
#define EBI_SIZE_8K         0
#define EBI_SIZE_16K        1
#define EBI_SIZE_32K        2
#define EBI_SIZE_64K        3
#define EBI_SIZE_128K       4
#define EBI_SIZE_256K       5
#define EBI_SIZE_512K       6
#define EBI_SIZE_1M         7
#define EBI_SIZE_2M         8
#define EBI_SIZE_4M         9
#define EBI_SIZE_8M         10
#define EBI_SIZE_16M        11
#define EBI_SIZE_32M        12
#define EBI_SIZE_64M        13
#define EBI_SIZE_128M       14
#define EBI_SIZE_256M       15

/* CSxCNTL settings */
#define EBI_ENABLE          0x00000001  /* .. enable this range */
#define EBI_WAIT_STATES     0x0000000e  /* .. mask for wait states */
#define ZEROWT              0x00000000  /* ..  0 WS */
#define ONEWT               0x00000002  /* ..  1 WS */
#define TWOWT               0x00000004  /* ..  2 WS */
#define THREEWT             0x00000006  /* ..  3 WS */
#define FOURWT              0x00000008  /* ..  4 WS */
#define FIVEWT              0x0000000a  /* ..  5 WS */
#define SIXWT               0x0000000c  /* ..  6 WS */
#define SEVENWT             0x0000000e  /* ..  7 WS */
#define EBI_WORD_WIDE       0x00000010  /* .. 16-bit peripheral, else 8 */
#define EBI_TS_TA_MODE      0x00000080  /* .. use TS/TA mode */
#define EBI_TS_SEL          0x00000100  /* .. drive tsize, not bs_b */
//#define EBI_FIFO            0x00000200  /* .. enable fifo */
#define EBI_RE              0x00000400  /* .. Reverse Endian */

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

#ifdef __cplusplus
}
#endif

#endif
