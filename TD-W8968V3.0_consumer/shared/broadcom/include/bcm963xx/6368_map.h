/*
 Copyright (c) 2007-2012 Broadcom Corporation 
 All Rights Reserved 

<:label-BRCM:2011:proprietary:standard

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
/*   MODULE:  6368_map.h                                               */
/*   DATE:    02/06/07                                                 */
/*   PURPOSE: Define addresses of major hardware components of         */
/*            BCM6368                                                  */
/*                                                                     */
/***********************************************************************/
#ifndef __BCM6368_MAP_H
#define __BCM6368_MAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "bcmtypes.h"
#include "6368_common.h"
#include "6368_intr.h"
#include "6368_map_part.h"
/* non-proprietary code is in xxxx_map_part.h */

#if defined(__KERNEL__) && !defined(MODULE)
#error "PRIVATE FILE INCLUDED IN KERNEL"
#endif

/* macro to convert logical data addresses to physical */
/* DMA hardware must see physical address */
#define LtoP( x )       ( (uint32)x & 0x1fffffff )
#define PtoL( x )       ( LtoP(x) | 0xa0000000 )



/*
** Pcm Controller
*/

typedef struct PcmControlRegisters
{
    uint32 pcm_ctrl;                            // 00 offset from PCM_BASE
#define  PCM_ENABLE              0x80000000     // PCM block master enable
#define  PCM_ENABLE_SHIFT        31
#define  PCM_SLAVE_SEL           0x40000000     // PCM TDM slave mode select (1 - TDM slave, 0 - TDM master)
#define  PCM_SLAVE_SEL_SHIFT     30
#define  PCM_CLOCK_INV           0x20000000     // PCM SCLK invert select (1 - invert, 0 - normal)
#define  PCM_CLOCK_INV_SHIFT     29
#define  PCM_FS_INVERT           0x10000000     // PCM FS invert select (1 - invert, 0 - normal)
#define  PCM_FS_INVERT_SHIFT     28
#define  PCM_FS_FREQ_16_8        0x08000000     // PCM FS 16/8 Khz select (1 - 16Khz, 0 - 8Khz)
#define  PCM_FS_FREQ_16_8_SHIFT  27
#define  PCM_FS_LONG             0x04000000     // PCM FS long/short select (1 - long FS, 0 - short FS)
#define  PCM_FS_LONG_SHIFT       26
#define  PCM_FS_TRIG             0x02000000     // PCM FS trigger (1 - falling edge, 0 - rising edge trigger)
#define  PCM_FS_TRIG_SHIFT       25
#define  PCM_DATA_OFF            0x01000000     // PCM data offset from FS (1 - one clock from FS, 0 - no offset)
#define  PCM_DATA_OFF_SHIFT      24
#define  PCM_DATA_16_8           0x00800000     // PCM data word length (1 - 16 bits, 0 - 8 bits)
#define  PCM_DATA_16_8_SHIFT     23
#define  PCM_CLOCK_SEL           0x00700000     // PCM SCLK freq select
#define  PCM_CLOCK_SEL_SHIFT     20
                                                  // 000 - 8192 Khz
                                                  // 001 - 4096 Khz
                                                  // 010 - 2048 Khz
                                                  // 011 - 1024 Khz
                                                  // 100 - 512 Khz
                                                  // 101 - 256 Khz
                                                  // 110 - 128 Khz
                                                  // 111 - reserved
#define  PCM_LSB_FIRST           0x00040000     // PCM shift direction (1 - LSBit first, 0 - MSBit first)
#define  PCM_LSB_FIRST_SHIFT     18
#define  PCM_LOOPBACK            0x00020000     // PCM diagnostic loobback enable
#define  PCM_LOOPBACK_SHIFT      17
#define  PCM_EXTCLK_SEL          0x00010000     // PCM external timing clock select -- Maybe removed in 6368
#define  PCM_EXTCLK_SEL_SHIFT    16
#define  PCM_NTR_ENABLE          0x00008000     // PCM NTR counter enable -- Nayve removed in 6368
#define  PCM_NTR_ENABLE_SHIFT    15
#define  PCM_BITS_PER_FRAME_1024 0x00000400     // 1024 - Max
#define  PCM_BITS_PER_FRAME_256  0x00000100     // 256
#define  PCM_BITS_PER_FRAME_8    0x00000008     // 8    - Min

    uint32 pcm_chan_ctrl;                       // 04
#define  PCM_TX0_EN              0x00000001     // PCM transmit channel 0 enable
#define  PCM_TX1_EN              0x00000002     // PCM transmit channel 1 enable
#define  PCM_TX2_EN              0x00000004     // PCM transmit channel 2 enable
#define  PCM_TX3_EN              0x00000008     // PCM transmit channel 3 enable
#define  PCM_TX4_EN              0x00000010     // PCM transmit channel 4 enable
#define  PCM_TX5_EN              0x00000020     // PCM transmit channel 5 enable
#define  PCM_TX6_EN              0x00000040     // PCM transmit channel 6 enable
#define  PCM_TX7_EN              0x00000080     // PCM transmit channel 7 enable
#define  PCM_RX0_EN              0x00000100     // PCM receive channel 0 enable
#define  PCM_RX1_EN              0x00000200     // PCM receive channel 1 enable
#define  PCM_RX2_EN              0x00000400     // PCM receive channel 2 enable
#define  PCM_RX3_EN              0x00000800     // PCM receive channel 3 enable
#define  PCM_RX4_EN              0x00001000     // PCM receive channel 4 enable
#define  PCM_RX5_EN              0x00002000     // PCM receive channel 5 enable
#define  PCM_RX6_EN              0x00004000     // PCM receive channel 6 enable
#define  PCM_RX7_EN              0x00008000     // PCM receive channel 7 enable
#define  PCM_RX_PACKET_SIZE      0x00ff0000     // PCM Rx DMA quasi-packet size
#define  PCM_RX_PACKET_SIZE_SHIFT  16

    uint32 pcm_int_pending;                     // 08
    uint32 pcm_int_mask;                        // 0c
#define  PCM_TX_UNDERFLOW        0x00000001     // PCM DMA receive overflow
#define  PCM_RX_OVERFLOW         0x00000002     // PCM DMA transmit underflow
#define  PCM_TDM_FRAME           0x00000004     // PCM frame boundary
#define  PCM_RX_IRQ              0x00000008     // IUDMA interrupts
#define  PCM_TX_IRQ              0x00000010

    uint32 pcm_pll_ctrl1;                       // 10
#define  PCM_PLL_PWRDN           0x80000000     // PLL PWRDN
#define  PCM_PLL_PWRDN_CH1       0x40000000     // PLL CH PWRDN
#define  PCM_PLL_REFCMP_PWRDN    0x20000000     // PLL REFCMP PWRDN
#define  PCM_CLK16_RESET         0x10000000     // 16.382 MHz PCM interface circuitry reset. 
#define  PCM_PLL_ARESET          0x08000000     // PLL Analog Reset
#define  PCM_PLL_DRESET          0x04000000     // PLL Digital Reset

    uint32 pcm_pll_ctrl2;                       // 14
    uint32 pcm_pll_ctrl3;                       // 18
    uint32 pcm_pll_ctrl4;                       // 1c

    uint32 pcm_pll_stat;                        // 20
#define  PCM_PLL_LOCK            0x00000001     // Asserted when PLL is locked to programmed frequency

    uint32 pcm_ntr_counter;                     // 24

    uint32 unused[6];

#define  PCM_MAX_TIMESLOT_REGS   16             // Number of PCM time slot registers in the table.
                                                // Each register provides the settings for 8 timeslots (4 bits per timeslot)
    uint32 pcm_slot_alloc_tbl[PCM_MAX_TIMESLOT_REGS];
#define  PCM_TS_VALID            0x8            // valid marker for TS alloc ram entry
} PcmControlRegisters;

#define PCM ((volatile PcmControlRegisters * const) PCM_BASE)

typedef struct PcmIudmaRegisters
{
   uint16 reserved0;
   uint16 ctrlConfig;
#define BCM6368_IUDMA_REGS_CTRLCONFIG_MASTER_EN        0x0001
#define BCM6368_IUDMA_REGS_CTRLCONFIG_FLOWC_CH1_EN     0x0002
#define BCM6368_IUDMA_REGS_CTRLCONFIG_FLOWC_CH3_EN     0x0004
#define BCM6368_IUDMA_REGS_CTRLCONFIG_FLOWC_CH5_EN     0x0008
#define BCM6368_IUDMA_REGS_CTRLCONFIG_FLOWC_CH7_EN     0x0010

    // Flow control Ch1
    uint16 reserved1;
    uint16 ch1_FC_Low_Thr;

    uint16 reserved2;
    uint16 ch1_FC_High_Thr;

    uint16 reserved3;
    uint16 ch1_Buff_Alloc;

    // Flow control Ch3
    uint16 reserved4;
    uint16 ch3_FC_Low_Thr;

    uint16 reserved5;
    uint16 ch3_FC_High_Thr;

    uint16 reserved6;
    uint16 ch3_Buff_Alloc;

    // Flow control Ch5
    uint16 reserved7;
    uint16 ch5_FC_Low_Thr;

    uint16 reserved8;
    uint16 ch5_FC_High_Thr;

    uint16 reserved9;
    uint16 ch5_Buff_Alloc;

    // Flow control Ch7
    uint16 reserved10;
    uint16 ch7_FC_Low_Thr;

    uint16 reserved11;
    uint16 ch7_FC_High_Thr;

    uint16 reserved12;
    uint16 ch7_Buff_Alloc;

    // Channel resets
    uint16 reserved13;
    uint16 channel_reset;
    
    uint16 reserved14;
    uint16 channel_debug;
    
    // Spare register
    uint32 dummy1;
    
    // Interrupt status registers
    uint16 reserved15;
    uint16 gbl_int_stat;
    
    // Interrupt mask registers
    uint16 reserved16;
    uint16 gbl_int_mask;        
} PcmIudmaRegisters;

typedef struct PcmIudmaChannelCtrl
{
    uint16 reserved1;
    uint16 config;
#define BCM6368_IUDMA_CONFIG_ENDMA       0x0001
#define BCM6368_IUDMA_CONFIG_PKTHALT     0x0002
#define BCM6368_IUDMA_CONFIG_BURSTHALT   0x0004

    uint16 reserved2;
    uint16 intStat;
#define BCM6368_IUDMA_INTSTAT_BDONE   0x0001
#define BCM6368_IUDMA_INTSTAT_PDONE   0x0002
#define BCM6368_IUDMA_INTSTAT_NOTVLD  0x0004
#define BCM6368_IUDMA_INTSTAT_MASK    0x0007
#define BCM6368_IUDMA_INTSTAT_ALL     BCM6368_IUDMA_INTSTAT_MASK

    uint16 reserved3;
    uint16 intMask;
#define BCM6368_IUDMA_INTMASK_BDONE   0x0001
#define BCM6368_IUDMA_INTMASK_PDONE   0x0002
#define BCM6368_IUDMA_INTMASK_NOTVLD  0x0004

    uint32 maxBurst;
#define BCM6368_IUDMA_MAXBURST_SIZE 16 /* 32-bit words */

} PcmIudmaChannelCtrl;


typedef struct PcmIudmaStateRam
{
   uint32 baseDescPointer;                /* pointer to first buffer descriptor */

   uint32 stateBytesDoneRingOffset;       /* state info: how manu bytes done and the offset of the
                                             current descritor in process */
#define BCM6368_IUDMA_STRAM_DESC_RING_OFFSET 0x3fff


   uint32 flagsLengthStatus;              /* Length and status field of the current descriptor */

   uint32 currentBufferPointer;           /* pointer to the current descriptor */

} PcmIudmaStateRam;

#define BCM6368_MAX_PCM_DMA_CHANNELS 2

typedef struct PcmIudma
{
   PcmIudmaRegisters regs;                                        // 
   uint32 reserved1[110];                                         //            
   PcmIudmaChannelCtrl ctrl[BCM6368_MAX_PCM_DMA_CHANNELS];        //
   uint32 reserved2[120];                                         //
   PcmIudmaStateRam stram[BCM6368_MAX_PCM_DMA_CHANNELS];          //

} PcmIudma;

#define PCM_IUDMA ((volatile PcmIudma * const) PCM_DMA_BASE)


#define IUDMA_MAX_CHANNELS          32

/*
** USB 2.0 Device Registers
*/
typedef struct UsbRegisters {
#define USBD_CONTROL_APP_DONECSR                 0x0001
#define USBD_CONTROL_APP_RESUME                  0x0002
#define USBD_CONTROL_APP_RXFIFIO_INIT            0x0040
#define USBD_CONTROL_APP_TXFIFIO_INIT            0x0080
#define USBD_CONTROL_APP_FIFO_SEL_SHIFT          0x8
#define USBD_CONTROL_APP_FIFO_INIT_SEL(x)        (((x)&0x0f)<<USBD_CONTROL_APP_FIFO_SEL_SHIFT)
#define USBD_CONTROL_APP_AUTO_CSRS               0x2000
#define USBD_CONTROL_APP_AUTO_INS_ZERO_LEN_PKT   0x4000
#define EN_TXZLENINS  (1<<14)
#define EN_RXZSCFG   (1<<12)
#define APPSETUPERRLOCK  (1<<5)
    uint32 usbd_control ;
#define USBD_STRAPS_APP_SELF_PWR    0x0400
#define USBD_STRAPS_APP_DEV_DISCON  0x0200
#define USBD_STRAPS_APP_CSRPRG_SUP  0x0100
#define USBD_STRAPS_APP_RAM_IF      0x0080
#define USBD_STRAPS_APP_DEV_RMTWKUP 0x0040
#define USBD_STRAPS_APP_PHYIF_8BIT  0x0004
#define USBD_STRAPS_FULL_SPEED      0x0003
#define USBD_STRAPS_LOW_SPEED       0x0002
#define USBD_STRAPS_HIGH_SPEED      0x0000
#define APPUTMIDIR(x)  ((x&1)<<3)
#define UNIDIR   0
    uint32 usbd_straps;
#define USB_ENDPOINT_0  0x01
    uint32 usbd_stall;
#define USBD_ENUM_SPEED_SHIFT       12
#define USBD_ENUM_SPEED             0x3000
#define UDC20_ALTINTF(x) ((x>>8)&0xf)
#define UDC20_INTF(x)  ((x>>4)&0xf)
#define UDC20_CFG(x)  ((x>>0)&0xf)
    uint32 usbd_status;
#define USBD_LINK                   (0x1<<10)
#define USBD_SET_CSRS               0x40
#define USBD_SUSPEND                0x20
#define USBD_EARLY_SUSPEND          0x10
#define USBD_SOF                    0x08
#define USBD_ENUMON                 0x04
#define USBD_SETUP                  0x02
#define USBD_USBRESET               0x01
    uint32 usbd_events;
    uint32 usbd_events_irq;
#define UPPER(x)   (16+x)
#define ENABLE(x)   (1<<x)
#define SWP_TXBSY   (15)
#define SWP_RXBSY   (14)
#define SETUP_ERR   (13)
#define APPUDCSTALLCHG  (12)
#define BUS_ERR    (11)
#define USB_LINK                                (10)
#define HST_SETCFG   (9)
#define HST_SETINTF   (8)
#define ERRATIC_ERR   (7)
#define SET_CSRS   (6)
#define SUSPEND    (5)
#define ERLY_SUSPEND  (4)
#define SOF     (3)
#define ENUM_ON    (2)
#define SETUP    (1)
#define USB_RESET   (0)
#define RISING(x)   (0x0<<2*x)
#define FALLING(x)   (0x1<<2*x)
#define USBD_IRQCFG_ENUM_ON_FALLING_EDGE 0x00000010
    uint32 usbd_irqcfg_hi ;
    uint32 usbd_irqcfg_lo ;
#define USBD_USB_RESET_IRQ          0x00000001
#define USBD_USB_SETUP_IRQ          0x00000002 // non-standard setup cmd rcvd
#define USBD_USB_ENUM_ON_IRQ        0x00000004
#define USBD_USB_SOF_IRQ            0x00000008
#define USBD_USB_EARLY_SUSPEND_IRQ  0x00000010
#define USBD_USB_SUSPEND_IRQ        0x00000020 // non-standard setup cmd rcvd
#define USBD_USB_SET_CSRS_IRQ       0x00000040
#define USBD_USB_ERRATIC_ERR_IRQ    0x00000080
#define USBD_USB_SETCFG_IRQ         0x00000200
#define USBD_USB_LINK_IRQ                       0x00000400
    uint32 usbd_events_irq_mask;
    uint32 usbd_swcfg;
    uint32 usbd_swtxctl;
    uint32 usbd_swrxctl;
    uint32 usbd_txfifo_rwptr;
    uint32 usbd_rxfifo_rwptr;
    uint32 usbd_txfifo_st_rwptr;
    uint32 usbd_rxfifo_st_rwptr;
    uint32 usbd_txfifo_config ;
    uint32 usbd_rxfifo_config ;
    uint32 usbd_txfifo_epsize ;
    uint32 usbd_rxfifo_epsize ;
#define USBD_EPNUM_CTRL             0x0
#define USBD_EPNUM_ISO              0x1
#define USBD_EPNUM_BULK             0x2
#define USBD_EPNUM_IRQ              0x3
#define USBD_EPNUM_EPTYPE(x)        (((x)&0x3)<<8)
#define USBD_EPNUM_EPDMACHMAP(x)    (((x)&0xf)<<0)
    uint32 usbd_epnum_typemap ;
    uint32 usbd_reserved [0xB] ;
    uint32 usbd_csr_setupaddr ;
#define USBD_EPNUM_MASK             0xf
#define USBD_EPNUM(x)               ((x&USBD_EPNUM_MASK)<<0)
#define USBD_EPDIR_IN               (1<<4)
#define USBD_EPDIR_OUT              (0<<4)
#define USBD_EPTYP_CTRL             (USBD_EPNUM_CTRL<<5)
#define USBD_EPTYP_ISO              (USBD_EPNUM_ISO<<5)
#define USBD_EPTYP_BULK             (USBD_EPNUM_BULK<<5)
#define USBD_EPTYP_IRQ              (USBD_EPNUM_IRQ<<5)
#define USBD_EPCFG_MASK             0xf
#define USBD_EPCFG(x)               ((x&USBD_EPCFG_MASK)<<7)
#define USBD_EPINTF_MASK            0xf
#define USBD_EPINTF(x)              ((x&USBD_EPINTF_MASK)<<11)
#define USBD_EPAINTF_MASK           0xf
#define USBD_EPAINTF(x)             ((x&USBD_EPAINTF_MASK)<<15)
#define USBD_EPMAXPKT_MSK           0x7ff
#define USBD_EPMAXPKT(x)            ((x&USBD_EPMAXPKT_MSK)<<19)
#define USBD_EPISOPID_MASK          0x3
#define USBD_EPISOPID(x)            ((x&USBD_ISOPID_MASK)<<30)
    uint32 usbd_csr_ep [5] ;
} UsbRegisters;

#define USB ((volatile UsbRegisters * const) USB_CTL_BASE)


typedef struct EthSwRegs{
    byte port_traffic_ctrl[9]; /* 0x00 - 0x08 */
    byte reserved1[2]; /* 0x09 - 0x0a */
    byte switch_mode; /* 0x0b */
    unsigned short pause_quanta; /*0x0c */
    byte imp_port_state; /*0x0e */
    byte led_refresh; /* 0x0f */
    unsigned short led_function[2]; /* 0x10 */
    unsigned short led_function_map; /* 0x14 */
    unsigned short led_enable_map; /* 0x16 */
    unsigned short led_mode_map0; /* 0x18 */
    unsigned short led_function_map1; /* 0x1a */
    byte reserved2[5]; /* 0x1b - 0x20 */
    byte port_forward_ctrl; /* 0x21 */ 
    byte reserved3[2]; /* 0x22 - 0x23 */
    unsigned short protected_port_selection; /* 0x24 */
    unsigned short wan_port_select; /* 0x26 */
    unsigned int pause_capability; /* 0x28 */
    byte reserved4[3]; /* 0x2c - 0x2e */
    byte reserved_multicast_control; /* 0x2f */
    byte reserved5; /* 0x30 */
    byte txq_flush_mode_control; /* 0x31 */
    unsigned short ulf_forward_map; /* 0x32 */
    unsigned short mlf_forward_map; /* 0x34 */
    unsigned short mlf_impc_forward_map; /* 0x36 */
    unsigned short pause_pass_through_for_rx; /* 0x38 */
    unsigned short pause_pass_through_for_tx; /* 0x3a */
    unsigned short disable_learning; /* 0x3c */
    byte reserved6[26]; /* 0x3e - 0x57 */
    byte port_state_override[8]; /* 0x58 - 0x5f */
    byte reserved7[4]; /* 0x60 - 0x63 */
    byte imp_rgmii_ctrl_p4; /* 0x64 */
    byte imp_rgmii_ctrl_p5; /* 0x65 */
    byte reserved8[6]; /* 0x66 - 0x6b */
    byte rgmii_timing_delay_p4; /* 0x6c */
    byte gmii_timing_delay_p5; /* 0x6d */
    byte reserved9[11]; /* 0x6e - 0x78 */
    byte software_reset; /* 0x79 */
    byte reserved13[6]; /* 0x7a - 0x7f */
    byte pause_frame_detection; /* 0x80 */
    byte reserved10[7]; /* 0x81 - 0x87 */
    byte fast_aging_ctrl; /* 0x88 */
    byte fast_aging_port; /* 0x89 */
    byte fast_aging_vid; /* 0x8a */
    byte reserved11[21]; /* 0x8b - 0x9f */
    unsigned int swpkt_ctrl_sar; /*0xa0 */
    unsigned int swpkt_ctrl_usb; /*0xa4 */
    unsigned int iudma_ctrl; /*0xa8 */
    unsigned int rxfilt_ctrl; /*0xac */
    unsigned int mdio_ctrl; /*0xb0 */
    unsigned int mdio_data; /*0xb4 */
    byte reserved12[42]; /* 0xb6 - 0xdf */
    unsigned int sw_mem_test; /*0xe0 */
} EthSwRegs;

#define ETHSWREG ((volatile EthSwRegs * const) SWITCH_BASE)


/*
** NAND Interrupt Controller Registers
*/
typedef struct NandIntrCtrlRegs {
    uint32 NandInterrupt;
#define NINT_ENABLE_MASK    0xffff0000
#define NINT_STS_MASK       0x00000fff
#define NINT_ECC_ERROR_CORR 0x00000080
#define NINT_ECC_ERROR_UNC  0x00000040
#define NINT_DEV_RBPIN      0x00000020
#define NINT_CTRL_READY     0x00000010
#define NINT_PAGE_PGM       0x00000008
#define NINT_COPY_BACK      0x00000004
#define NINT_BLOCK_ERASE    0x00000002
#define NINT_NP_READ        0x00000001

    uint32 NandBaseAddr0;   /* Default address when booting from NAND flash */
    uint32 reserved;
    uint32 NandBaseAddr1;   /* Secondary base address for NAND flash */
} NandIntrCtrlRegs;

#define NAND_INTR ((volatile NandIntrCtrlRegs * const) NAND_INTR_BASE)

/*
** NAND Controller Registers
*/
typedef struct NandCtrlRegs {
    uint32 NandRevision;            /* NAND Revision */
    uint32 NandCmdStart;            /* Nand Flash Command Start */
#define NCMD_MASK           0x0f000000
#define NCMD_BLK_LOCK_STS   0x0d000000
#define NCMD_BLK_UNLOCK     0x0c000000
#define NCMD_BLK_LOCK_DOWN  0x0b000000
#define NCMD_BLK_LOCK       0x0a000000
#define NCMD_FLASH_RESET    0x09000000
#define NCMD_BLOCK_ERASE    0x08000000
#define NCMD_DEV_ID_READ    0x07000000
#define NCMD_COPY_BACK      0x06000000
#define NCMD_PROGRAM_SPARE  0x05000000
#define NCMD_PROGRAM_PAGE   0x04000000
#define NCMD_STS_READ       0x03000000
#define NCMD_SPARE_READ     0x02000000
#define NCMD_PAGE_READ      0x01000000

    uint32 NandCmdExtAddr;          /* Nand Flash Command Extended Address */
    uint32 NandCmdAddr;             /* Nand Flash Command Address */
    uint32 NandCmdEndAddr;          /* Nand Flash Command End Address */
    uint32 NandNandBootConfig;      /* Nand Flash Boot Config */
#define NBC_CS_LOCK         0x80000000
#define NBC_AUTO_DEV_ID_CFG 0x40000000
#define NBC_WR_PROT_BLK0    0x10000000

    uint32 NandCsNandXor;           /* Nand Flash EBI CS Address XOR with */
                                    /*   1FC0 Control */
    uint32 NandReserved;
    uint32 NandSpareAreaReadOfs0;   /* Nand Flash Spare Area Read Bytes 0-3 */
    uint32 NandSpareAreaReadOfs4;   /* Nand Flash Spare Area Read Bytes 4-7 */
    uint32 NandSpareAreaReadOfs8;   /* Nand Flash Spare Area Read Bytes 8-11 */
    uint32 NandSpareAreaReadOfsC;   /* Nand Flash Spare Area Read Bytes 12-15*/
    uint32 NandSpareAreaWriteOfs0;  /* Nand Flash Spare Area Write Bytes 0-3 */
    uint32 NandSpareAreaWriteOfs4;  /* Nand Flash Spare Area Write Bytes 4-7 */
    uint32 NandSpareAreaWriteOfs8;  /* Nand Flash Spare Area Write Bytes 8-11*/
    uint32 NandSpareAreaWriteOfsC;  /* Nand Flash Spare Area Write Bytes12-15*/
    uint32 NandAccControl;          /* Nand Flash Access Control */
    uint32 NandConfig;              /* Nand Flash Config */
#define NC_CONFIG_LOCK      0x80000000
#define NC_PG_SIZE_MASK     0x40000000
#define NC_PG_SIZE_2K       0x40000000
#define NC_PG_SIZE_512B     0x00000000
#define NC_BLK_SIZE_MASK    0x30000000
#define NC_BLK_SIZE_512K    0x30000000
#define NC_BLK_SIZE_128K    0x10000000
#define NC_BLK_SIZE_16K     0x00000000
#define NC_BLK_SIZE_8K      0x20000000
#define NC_DEV_SIZE_MASK    0x0f000000
#define NC_DEV_SIZE_SHIFT   24
#define NC_DEV_WIDTH_MASK   0x00800000
#define NC_DEV_WIDTH_16     0x00800000
#define NC_DEV_WIDTH_8      0x00000000
#define NC_FUL_ADDR_MASK    0x00070000
#define NC_FUL_ADDR_SHIFT   16
#define NC_BLK_ADDR_MASK    0x00000700
#define NC_BLK_ADDR_SHIFT   8

    uint32 NandTiming1;             /* Nand Flash Timing Parameters 1 */
    uint32 NandTiming2;             /* Nand Flash Timing Parameters 2 */
    uint32 NandSemaphore;           /* Semaphore */
    uint32 NandFlashDeviceId;       /* Nand Flash Device ID */
    uint32 NandBlockLockStatus;     /* Nand Flash Block Lock Status */
    uint32 NandIntfcStatus;         /* Nand Flash Interface Status */
#define NIS_CTLR_READY      0x80000000
#define NIS_FLASH_READY     0x40000000
#define NIS_CACHE_VALID     0x20000000
#define NIS_SPARE_VALID     0x10000000
#define NIS_FLASH_STS_MASK  0x000000ff
#define NIS_WRITE_PROTECT   0x00000080
#define NIS_DEV_READY       0x00000040
#define NIS_PGM_ERASE_ERROR 0x00000001

    uint32 NandEccCorrExtAddr;      /* ECC Correctable Error Extended Address*/
    uint32 NandEccCorrAddr;         /* ECC Correctable Error Address */
    uint32 NandEccUncExtAddr;       /* ECC Uncorrectable Error Extended Addr */
    uint32 NandEccUncAddr;          /* ECC Uncorrectable Error Address */
    uint32 NandFlashReadExtAddr;    /* Flash Read Data Extended Address */
    uint32 NandFlashReadAddr;       /* Flash Read Data Address */
    uint32 NandProgramPageExtAddr;  /* Page Program Extended Address */
    uint32 NandProgramPageAddr;     /* Page Program Address */
    uint32 NandCopyBackExtAddr;     /* Copy Back Extended Address */
    uint32 NandCopyBackAddr;        /* Copy Back Address */
    uint32 NandBlockEraseExtAddr;   /* Block Erase Extended Address */
    uint32 NandBlockEraseAddr;      /* Block Erase Address */
    uint32 NandInvReadExtAddr;      /* Flash Invalid Data Extended Address */
    uint32 NandInvReadAddr;         /* Flash Invalid Data Address */
    uint32 NandBlkWrProtect;        /* Block Write Protect Enable and Size */
                                    /*   for EBI_CS0b */
} NandCtrlRegs;

#define NAND ((volatile NandCtrlRegs * const) NAND_REG_BASE)

#define NAND_CACHE ((volatile uint8 * const) NAND_CACHE_BASE)

#ifdef __cplusplus
}
#endif

#endif

