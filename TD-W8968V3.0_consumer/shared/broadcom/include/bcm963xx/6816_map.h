/*
 Copyright (c) 2007-2012  Broadcom Corporation
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
/*   MODULE:  6816_map.h                                               */
/*   DATE:    05/30/08                                                 */
/*   PURPOSE: Define addresses of major hardware components of         */
/*            BCM6816                                                  */
/*                                                                     */
/***********************************************************************/
#ifndef __BCM6816_MAP_H
#define __BCM6816_MAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include "bcmtypes.h"
#include "6816_common.h"
#include "6816_intr.h"
#include "6816_map_part.h"
/* non-proprietary code is in xxxx_map_part.h */

#if defined(__KERNEL__) && !defined(MODULE)
#error "PRIVATE FILE INCLUDED IN KERNEL"
#endif

/* macro to convert logical data addresses to physical */
/* DMA hardware must see physical address */
#define LtoP( x )       ( (uint32)x & 0x1fffffff )
#define PtoL( x )       ( LtoP(x) | 0xa0000000 )


/*
** OTP
*/
typedef struct Otp {
    uint32      Config;                 /* 0x0 */
    uint32      Control;                /* 0x4 */
    uint32      Addr;                   /* 0x8 */
    uint32      WriteData;              /* 0xc */
    uint32      Status;                 /* 0x10 */
    uint32      DOut;                   /* 0x14 */
    uint32      unused1[2];             /* 0x18 - 0x1c */
    uint32      UserBits[6];            /* 0x20 - 0x34 */
    uint32      unused2[2];             /* 0x38 - 0x3c */
    uint32      RAMRepair[12];          /* 0x40 - 0x6c */
} Otp;

#define OTP ((volatile Otp * const) OTP_BASE)

/* Word order is reversed for User OTP bits */
#define OTP_GET_USER_BIT(x)             ((OTP->UserBits[((sizeof(OTP->UserBits)/4) - (x)/32 - 1)] >> ((x) % 32)) & 1)


/*
** Pcm Controller
*/

typedef struct PcmControlRegisters
{
   uint32   pcm_ctrl;         // (0x200) PCM main control
#define   PCM_ENABLE             0x80000000   // PCM block master enable
#define   PCM_ENABLE_SHIFT       31
#define   PCM_SLAVE_SEL          0x40000000   // PCM TDM slave mode select (1 - TDM slave, 0 - TDM master)
#define   PCM_SLAVE_SEL_SHIFT    30

#define   PCM_CLOCK_INV          0x20000000   // PCM SCLK invert select (1 - invert, 0 - normal)
#define   PCM_CLOCK_INV_SHIFT    29

#define   PCM_FS_INVERT          0x10000000   // PCM FS invert select (1 - invert, 0 - normal)
#define   PCM_FS_INVERT_SHIFT    28

#define   PCM_FS_FREQ_16_8       0x08000000   // PCM FS 16/8 Khz select (1 - 16Khz, 0 - 8Khz)
#define   PCM_FS_FREQ_16_8_SHIFT 27

#define   PCM_FS_LONG            0x04000000   // PCM FS long/short select (1 - long FS, 0 - short FS)
#define   PCM_FS_LONG_SHIFT      26

#define   PCM_FS_TRIG            0x02000000   // PCM FS trigger (1 - falling edge, 0 - rising edge trigger)
#define   PCM_FS_TRIG_SHIFT      25

#define   PCM_DATA_OFF           0x01000000   // PCM data offset from FS (1 - one clock from FS, 0 - no offset)
#define   PCM_DATA_OFF_SHIFT     24

#define   PCM_DATA_16_8          0x00800000   // PCM data word length (1 - 16 bits, 0 - 8 bits)
#define   PCM_DATA_16_8_SHIFT    23

#define   PCM_CLOCK_SEL          0x00700000   // PCM SCLK freq select
#define   PCM_CLOCK_SEL_SHIFT    20
                                              // 000 - 8192 Khz
                                              // 001 - 4096 Khz
                                              // 010 - 2048 Khz
                                              // 011 - 1024 Khz
                                              // 100 - 512 Khz
                                              // 101 - 256 Khz
                                              // 110 - 128 Khz
                                              // 111 - reserved

#define   PCM_LSB_FIRST         0x00040000   // PCM shift direction (1 - LSBit first, 0 - MSBit first)
#define   PCM_LSB_FIRST_SHIFT   18
#define   PCM_LOOPBACK          0x00020000   // PCM diagnostic loobback enable (<TBD>, <NYI>)
#define   PCM_LOOPBACK_SHIFT    17
#define   PCM_EXTCLK_SEL        0x00010000   // PCM external timing clock select (<TBD>, <NYI>)
#define   PCM_EXTCLK_SEL_SHIFT  16
#define  PCM_NTR_ENABLE          0x00008000     // PCM NTR counter enable -- Nayve removed in 6816
#define  PCM_NTR_ENABLE_SHIFT    15
#define  PCM_BITS_PER_FRAME_1024 0x00000400     // 1024 - Max
#define  PCM_BITS_PER_FRAME_256  0x00000100     // 256
#define  PCM_BITS_PER_FRAME_8    0x00000008     // 8    - Min
#define   PCM_BITS_PER_FRAME   0x00000ffe    // PCM bits per frame
#define   PCM_AP_SEL           0x00000001   // set to give pads to PCM, reset for AP to use pads

   uint32   pcm_chan_ctrl;         // (0x204) PCM channel control
#define   PCM_TX0_EN           0x00000001   // PCM transmit channel 0 enable
#define   PCM_TX1_EN           0x00000002   // PCM transmit channel 1 enable
#define   PCM_TX2_EN           0x00000004   // PCM transmit channel 2 enable
#define   PCM_TX3_EN           0x00000008   // PCM transmit channel 3 enable
#define   PCM_TX4_EN           0x00000010   // PCM transmit channel 4 enable
#define   PCM_TX5_EN           0x00000020   // PCM transmit channel 5 enable
#define   PCM_TX6_EN           0x00000040   // PCM transmit channel 6 enable
#define   PCM_TX7_EN           0x00000080   // PCM transmit channel 7 enable
#define   PCM_TX_CHANNELS      0x000000FF   // mask for all PCM Rx channels

#define   PCM_RX0_EN           0x00000100   // PCM receive channel 0 enable
#define   PCM_RX1_EN           0x00000200   // PCM receive channel 1 enable
#define   PCM_RX2_EN           0x00000400   // PCM receive channel 2 enable
#define   PCM_RX3_EN           0x00000800   // PCM receive channel 3 enable
#define   PCM_RX4_EN           0x00001000   // PCM receive channel 4 enable
#define   PCM_RX5_EN           0x00002000   // PCM receive channel 5 enable
#define   PCM_RX6_EN           0x00004000   // PCM receive channel 6 enable
#define   PCM_RX7_EN           0x00008000   // PCM receive channel 7 enable
#define   PCM_RX_CHANNELS      0x0000FF00   // mask for all PCM Rx channels

#define   PCM_RX_PACKET_SIZE        0x00ff0000   // PCM Rx DMA quasi-packet size
#define   PCM_RX_PACKET_SIZE_SHIFT  16

   uint32   pcm_int_pending;         // (0x208) PCM IRQ pending
   uint32   pcm_int_mask;           // (0x20c) PCM IRQ mask

#define   PCM_TX_UNDERFLOW        0x00000001   // PCM DMA receive overflow
#define   PCM_RX_OVERFLOW        0x00000002   // PCM DMA transmit underflow
#define   PCM_TDM_FRAME           0x00000004   // PCM frame boundary
#define   PCM_RX_IRQ              0x00000008     // IUDMA interrupts
#define   PCM_TX_IRQ              0x00000010


   uint32   reg_pcm_clk_cntl_0;        // (0x210) PCM Clock Control 0 (NCO_FCW_MISC)
   uint32   reg_pcm_clk_cntl_1;        // (0x214) PCM Clock Control 1 (NCO_SCALE)
   uint32   reg_pcm_clk_cntl_2;        // (0x218) PCM Clock Control 2
#define   PCM_NCO_SHIFT                   0x0000000f
#define   PCM_NCO_MUX_CNTL                0x00000030
#define   PCM_NCO_LOAD_MISC               0x00000040
#define   PCM_NCO_SOFT_INIT               0x00000080

  uint32    spacer[9];

#define  PCM_MAX_TIMESLOT_REGS 16 // number of time slots in the table
  uint32    pcm_slot_alloc_tbl[PCM_MAX_TIMESLOT_REGS];      // (0x240-0x27f) TS Allocation RAM
#define  PCM_TS_VALID           0x8      // valid marker for TS alloc ram entry
} PcmControlRegisters;

#define PCM ((volatile PcmControlRegisters * const) PCM_BASE)

typedef struct HvgControlRegisters
{
   uint16        reg_hvg_scale_vtip;
   uint16        reg_hvg_scale_vhvg;

   uint16        reg_hvg_scale_vloop;
   uint16        reg_hvg_scale_vring;

   uint16        reg_hvg_scale_vcal;
   uint16        reg_hvg_scale_vinput;

   uint16        reg_hvg_offset_vtip;
   uint16        reg_hvg_offset_vhvg;

   uint16        reg_hvg_offset_vloop;
   uint16        reg_hvg_offset_vring;

   uint16        reg_hvg_offset_vcal;
   uint16        reg_hvg_offset_vinput;

   uint32        reg_hvg_shift;
#define   COND_SHIFT_V_HVG                0x0000000f
#define   COND_SHIFT_V_TIP                0x000000f0
#define   COND_SHIFT_V_RING               0x00000f00
#define   COND_SHIFT_V_LOOP               0x0000f000
#define   COND_SHIFT_V_INPUT              0x000f0000
#define   COND_SHIFT_CAL                  0x00f00000
#define   DIODE_DROP                      0x0f000000
#define   HVG_IGNORE_RESET                0x10000000
#define   BG_TRIM_SOURCE                  0x20000000
#define   HVG_SOFT_INIT                   0x40000000

   uint32        reg_hvg_turns_ratio;
#define   TURNS_RATIO                     0x00000ffc
#define   RING_RECT_LINE                  0xffff0000

   uint32        reg_hvg_duty_cycle;
#define   MAX_DUTY_CYCLE                  0x000000ff
#define   MIN_DUTY_CYCLE                  0x0000ff00
#define   MIN_ON_TIME                     0x00ff0000
#define   SHORT_CIRCUIT_THSHLD            0xff000000

   uint32        reg_hvg_ring;
#define   RING_GAIN                       0x00000fff
#define   RING_OVHD                       0x0fff0000
#define   HVG_COMMON_RING_REF             0x80000000
#define   RING_GAIN_SHIFT                          0
#define   RING_OVHD_SHIFT                         16
#define   HVG_COMMON_RING_REF_SHIFT               31

   uint32        reg_hvg_off_hook;
#define   OFF_HOOK_OVHD                   0x000001ff
#define   OFF_HOOK_MIN                    0x0003fe00
#define   PULSE_START_PHASE               0xff000000

   uint32        reg_hvg_bg;
#define   BG_CTAT                         0x0000003f
#define   BG_PTAT                         0x00003f00
#define   BG_PTAT_SHIFT                            8
#define   RING_DELAY                      0x001f0000
#define   HVG_SER_TST_INJECT              0x00200000

   uint32        reg_hvg_vbat;
#define   SCALE_V_BAT                     0x00007fff
#define   OFFSET_V_BAT                    0x0fff8000
#define   SHIFT_V_BAT                     0xf0000000

   uint32        reg_hvg_status1;
#define   HVG_OOB_B                       0x40000000
#define   HVG_INST_VALID_B                0x20000000
#define   HVG_INST_B                      0x1fff0000
#define   HVG_OOB_A                       0x00004000
#define   HVG_INST_VALID_A                0x00002000
#define   HVG_INST_A                      0x00001fff

   uint32        reg_hvg_status2;
#define   TIP_OOB_B                       0x40000000
#define   TIP_INST_VALID_B                0x20000000
#define   TIP_INST_B                      0x1fff0000
#define   TIP_OOB_A                       0x00004000
#define   TIP_INST_VALID_A                0x00002000
#define   TIP_INST_A                      0x00001fff

   uint32        reg_hvg_status3;
#define   RING_OOB_B                      0x40000000
#define   RING_INST_VALID_B               0x20000000
#define   RING_INST_B                     0x1fff0000
#define   RING_OOB_A                      0x00004000
#define   RING_INST_VALID_A               0x00002000
#define   RING_INST_A                     0x00001fff

   uint32        reg_hvg_status4;
#define   LOOP_OOB_B                      0x40000000
#define   LOOP_INST_VALID_B               0x20000000
#define   LOOP_INST_B                     0x1fff0000
#define   LOOP_OOB_A                      0x00004000
#define   LOOP_INST_VALID_A               0x00002000
#define   LOOP_INST_A                     0x00001fff

   uint32        reg_hvg_status5;
#define   INPUT_OOB_B                     0x40000000
#define   INPUT_INST_VALID_B              0x20000000
#define   INPUT_INST_B                    0x1fff0000
#define   INPUT_OOB_A                     0x00004000
#define   INPUT_INST_VALID_A              0x00002000
#define   INPUT_INST_A                    0x00001fff

   uint32        reg_hvg_status6;
#define   CAL_OOB_B                       0x40000000
#define   CAL_OOB_INST_VALID_B            0x20000000
#define   CAL_INST_B                      0x1fff0000
#define   CAL_OOB_A                       0x00004000
#define   CAL_OOB_INST_VALID_A            0x00002000
#define   CAL_INST_A                      0x00001fff

   uint32        reg_hvg_status7;
#define   HVG_DUTY_CYCLE_B                0xff000000
#define   HVG_DUTY_CYCLE_A                0x00ff0000
#define   HVG_SHORT_FLAG_B                0x00000800
#define   HVG_SHORT_FLAG_A                0x00000400
#define   HVG_DUTY_CYCLE_LIMITED_B        0x00000200
#define   HVG_DUTY_CYCLE_LIMITED_A        0x00000100
#define   MAX_DIV_OOB_B                   0x00000080
#define   MAX_DIV_OOB_A                   0x00000040
#define   MAX_MULT_OOB_B                  0x00000020
#define   MAX_MULT_OOB_A                  0x00000010
#define   ARBITER_ERR_B                   0x00000008
#define   ARBITER_ERR_A                   0x00000004
#define   AVG_BLOCK_DONE_B                0x00000002
#define   AVG_BLOCK_DONE_A                0x00000001

   uint32        reg_hvg_status8;
#define   HVG_AVG_VALID                   0x20000000
#define   HVG_SAT                         0x10000000
#define   HVG_WINDOW_SLEW                 0x0f000000
#define   HVG_MEAN                        0x001fffff

   uint32        reg_hvg_status9;
   uint32        reg_hvg_status10;
#define   TIP_AVG_VALID                   0x20000000
#define   TIP_SAT                         0x10000000
#define   TIP_WINDOW_SLEW                 0x0f000000
#define   TIP_MEAN                        0x001fffff

   uint32        reg_hvg_status11;
   uint32        reg_hvg_status12;
#define   RING_AVG_VALID                  0x20000000
#define   RING_SAT                        0x10000000
#define   RING_WINDOW_SLEW                0x0f000000
#define   RING_MEAN                       0x001fffff

   uint32        reg_hvg_status13;
   uint32        reg_hvg_status14;
#define   TP_RG_AVG_VALID                 0x20000000
#define   TP_RG_SAT                       0x10000000
#define   TP_RG_WINDOW_SLEW               0x0f000000
#define   TP_RG_MEAN                      0x001fffff

   uint32        reg_hvg_status15;
   uint32        reg_hvg_status16;
#define   I_LOOP_AVG_VALID                0x20000000
#define   I_LOOP_SAT                      0x10000000
#define   I_LOOP_WINDOW_SLEW              0x0f000000
#define   I_LOOP_MEAN                     0x001fffff

   uint32        reg_hvg_status17;
   uint32        reg_hvg_status18;
#define   INPUT_AVG_VALID                 0x20000000
#define   INPUT_SAT                       0x10000000
#define   INPUT_WINDOW_SLEW               0x0f000000
#define   INPUT_MEAN                      0x001fffff

   uint32        reg_hvg_status19;
   uint32        reg_hvg_status20;
#define   CAL_AVG_VALID                   0x20000000
#define   CAL_SAT                         0x10000000
#define   CAL_WINDOW_SLEW                 0x0f000000
#define   CAL_MEAN                        0x001fffff

   uint32        reg_hvg_status21;
   uint32        reg_hvg_status22;
   uint32        reg_hvg_status23;
   uint32        reg_hvg_status24;
   uint32        reg_hvg_status25;
   uint32        reg_hvg_status26;
   uint32        reg_hvg_status27;
   uint32        reg_hvg_status28;
   uint32        reg_hvg_status29;
   uint32        reg_hvg_status30;
   uint32        reg_hvg_status31;
   uint32        reg_hvg_status32;
   uint32        reg_hvg_status33;
   uint32        reg_hvg_status34;
   uint32        reg_hvg_status35;

   uint32        reg_hvg_track_max;
#define   RING_TRACK_MAX                  0x00000fff
#define   OFF_HOOK_MAX                    0x0fff0000

   uint32        hvg_spacer1[15];

   uint32        reg_hvg_cha_window_ctrl;
#define   HVG_WINDOW_SIZE                 0x0000000f
#define   TIP_WINDOW_SIZE                 0x000000f0
#define   RING_WINDOW_SIZE                0x00000f00
#define   LOOP_WINDOW_SIZE                0x0000f000
#define   INPUT_WINDOW_SIZE               0x000f0000
#define   TP_RG_WINDOW_SIZE               0x00f00000
#define   CAL_WINDOW_SIZE                 0x0f000000
#define   SLEW_WINDOW_SIZES               0x10000000
#define   NEW_BLK_RQST                    0x20000000

   uint16        reg_hvg_cha_max_hvg_slic;
   uint16        reg_hvg_cha_const_volt_goal;

   uint32        reg_hvg_cha_misc;
#define   K_PROP                          0x0000000f
#define   K_INTEG                         0x000000f0
#define   SER_TST_OUTPUT_SEL              0x00000700
#define   CONT_OR_BLOCK                   0x00000800
#define   HVG_MODE                        0x00003000
#define   HVG_MODE_OFFHOOK_TRACKING       0x00001000
#define   HVG_MODE_ONHOOK_FIXED           0x00002000
#define   HVG_SOFT_INIT_0                 0x00004000
#define   HVG_RR_SINGLE                   0x00008000

   uint32        reg_hvg_cha_spare;
   uint32        hvg_spacer2[28];

   uint32        reg_hvg_chb_window_ctrl;

   uint16        reg_hvg_chb_max_hvg_slic;
   uint16        reg_hvg_chb_const_volt_goal;

   uint32        reg_hvg_chb_misc;
   uint32        reg_hvg_chb_spare;

} HvgControlRegisters;

#define HVG ((volatile HvgControlRegisters * const) APM_HVG_BASE)

/*
** BCM6816 APM Register Structure and definitions
*/
typedef struct
{
    uint16      apm_dev_diag_sel;
#define   APM_DIAG_HI_SEL                 0xff00
#define   APM_DIAG_LO_SEL                 0x00ff

    uint16      apm_dev_irq_status;                     // (0x00) read only block interrupt status
#define   DEV_INTERRUPT                   0x0001        // DMA interrupt pending
#define   APM_INTERRUPT_1                 0x0002        // APM interrupt pending from bank 1
#define   APM_INTERRUPT_2                 0x0004        // APM interrupt pending from bank 2
#define   APM_INTERRUPT_3                 0x0008        // APM interrupt pending from bank 3
#define   PCM_INTERRUPT                   0x0010        // PCM interrupt pending

// semantic from IUDMA perpesctive
// Tx mem to APM
// Rx APM to mem
  uint32        apm_dev_irq_pend;                       // (0x04) DMA interrupt pending register
#define   DMA_A_RX                        0x00000001    // Ch A receive channel interrupt
#define   DMA_A_TX                        0x00000002    // Ch A transmit channel interrupt
#define   DMA_B_RX                        0x00000004    // Ch B receive channel interrupt
#define   DMA_B_TX                        0x00000008    // Ch B transmit channel interrupt
#define   DMA_TX_UNDERFLOW_A              0x00000010    // Ch A transmit channel underflow
#define   DMA_TX_UNDERFLOW_B              0x00000020    // Ch B transmit channel underflow
#define   DMA_RX_OVERFLOW_A               0x00000040    // Ch A receive channel overflow
#define   DMA_RX_OVERFLOW_B               0x00000080    // Ch B receive channel overflow
#define   DMA_PCM_RX                      0x00000100    // PCM Rx DMA IRQ
#define   DMA_PCM_TX                      0x00000200    // PCM Tx DMA IRQ
#define   DEV_BMU_IRQ                     0x00000400    // BMU block IRQ

  uint32        apm_dev_irq_mask;                       // (0x08)DMA interrupt mask register

// Note semantic change
// IUDMA refers to mem to periph as Tx
// and periph to mem as Rx
// APM core refers to path from mem as Rx
// and path to mem as Tx

  uint32        apm_dev_control;                        // (0x0c) Device control register
#define   RX_PACKET_SIZE_A                0x000000ff    // Number of samples to form quasi packet to mem channel A
#define   RX_PACKET_SIZE_B                0x0000ff00    // Number of samples to form quasi packet to mem channel B

#define   RX_DMA_ENABLE_A                 0x10000000    // Enable for Ch A DMA to mem
#define   RX_DMA_ENABLE_A_SHIFT           28

#define   RX_DMA_ENABLE_B                 0x20000000    // Enable for Ch B DMA to mem
#define   RX_DMA_ENABLE_B_SHIFT           29

#define   TX_DMA_ENABLE_A                 0x40000000    // Enable for Ch A DMA from mem
#define   TX_DMA_ENABLE_A_SHIFT           30

#define   TX_DMA_ENABLE_B                 0x80000000    // Enable for Ch B DMA from mem
#define   TX_DMA_ENABLE_B_SHIFT           31

// APM core registers
  uint32        reg_apm_coeff_wr_data;                  // (0x10) 20 bit 2's comp coefficient to be written into coeff RAM
#define   COEFF_PROG_INPUT                0x000FFFFF

  uint32        reg_apm_coeff_config;                   // (0x14)
#define   COEFF_PROG_ADDR                 0x000000FF    //8 bit address into coefficient RAM space
#define   COEFF_PROG_WE                   0x00000100    //1=write into memory 0= read from memory
#define   EQ_TX_ACCESS_COEFF_RAM_A        0x00010000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a equalizer
#define   EQ_RX_ACCESS_COEFF_RAM_A        0x00020000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a equalizer
#define   HISPD_HYBAL_ACCESS_COEFF_RAM_A  0x00040000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a asrc interpolator
#define   LOSPD_HYBAL_ACCESS_COEFF_RAM_A  0x00080000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a asrc interpolator
#define   YFLTR_ACCESS_COEFF_RAM_A        0x00100000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a hybrid balance
#define   ASRC_INT_ACCESS_COEFF_RAM_A     0x00200000    //1=processor intrf has control of coefficient RAM 0=normal operation channel b hybrid balance
#define   ASRC_DEC_ACCESS_COEFF_RAM_A     0x00400000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a y filter
#define   EQ_TX_ACCESS_COEFF_RAM_B        0x00800000    //1=processor intrf has control of coefficient RAM 0=normal operation channel b y filter
#define   EQ_RX_ACCESS_COEFF_RAM_B        0x01000000    //1=processor intrf has control of coefficient RAM 0=normal operation channel b y filter
#define   HISPD_HYBAL_ACCESS_COEFF_RAM_B  0x02000000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a asrc interpolator
#define   LOSPD_HYBAL_ACCESS_COEFF_RAM_B  0x04000000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a asrc interpolator
#define   YFLTR_ACCESS_COEFF_RAM_B        0x08000000    //1=processor intrf has control of coefficient RAM 0=normal operation channel b asrc interpolator
#define   ASRC_INT_ACCESS_COEFF_RAM_B     0x10000000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a asrc decimator
#define   ASRC_DEC_ACCESS_COEFF_RAM_B     0x20000000    //1=processor intrf has control of coefficient RAM 0=normal operation channel b asrc decimator

#define   APM6816_HYBAL_NUM_TAPS          5
#define   APM6816_YFLTR_FIR_NUM_TAPS      18
#define   APM6816_YFLTR_IIR1_NUM_TAPS     1
#define   APM6816_YFLTR_IIR2_NUM_TAPS     5
#define   APM6816_YFLTR_BLOCK_SIZE        28
#define   APM6816_RX_EQ_NUM_TAPS          80
#define   APM6816_TX_EQ_NUM_TAPS          80
#define   APM6816_ASRC_DEC_NUM_TAPS       120
#define   APM6816_ASRC_INT_NUM_TAPS       120

  uint32        reg_cha_test_config;                    // (0x18)
#define   RX_IN_MUX_CTRL                  0x00000003    //2 bits mux control  at eq input 0=test stimulus 1=rx_data_in 2=loopback from tx path channel a
#define   RX_IN_MUX_CTRL_SHIFT            0

#define   NETWORK_LPBK                    0x00000004    //1=take data inputs and loopback to data outputs (network side) channel a
#define   NETWORK_LPBK_SHIFT              2

#define   RX_98K_MUX_CTRL                 0x00000018    //1=bypass 0=use eq output on rx side channel a
#define   RX_98K_MUX_CTRL_SHIFT           3

#define   ADC_MUX_CTRL                    0x00000060    //2 bits mux at input to tx path 0=loopback 1=adc out 2=test stimulus channel a
#define   ADC_MUX_CTRL_SHIFT              5

#define   ADC_DAC_LPBK                    0x00000080    //1=take 4 bit ADC codes and send them to the DEM and DAC
#define   ADC_DAC_LPBK_SHIFT              7

#define   RX_TX_98K_LPBK                  0x00000100    //1=loopback from rx to tx path channel a
#define   RX_TX_98K_LPBK_SHIFT            8

#define   SLEW_INSTANT                    0x00000200    //1=slew scale, 0=make scale changes instantaneously
#define   SLEW_INSTANT_SHIFT              9

#define   ASRC_EN                         0x00000400    //1=asrc and eq are active 0=inactive
#define   ASRC_EN_SHIFT                   10


  uint32        reg_chb_test_config;                    // (0x1c)

  uint32        reg_cha_eq_config;                      // (0x20)
#define   EQ_RX_NUM_TAPS                  0x0000007f    //7 bit number of recieve taps
#define   EQ_RX_NUM_TAPS_SHIFT            0

#define   EQ_TX_NUM_TAPS                  0x00007f00    //7 bit number of transmit taps
#define   EQ_TX_NUM_TAPS_SHIFT            8

#define   EQ_TX_SHFT                      0x000f0000    //4 bits controls output shifter (powers of 2 gain control) for rx path channel a
#define   EQ_TX_SHFT_SHIFT                16

#define   EQ_RX_SHFT                      0x00f00000    //4 bits controls output shifter (powers of 2 gain control) for tx path channel a
#define   EQ_RX_SHFT_SHIFT                20

#define   EQ_RX_IMP_RESP                  0x01000000    //1=use ext RAM for eq coefficients 0=use int ROM for eq coefficients channel a
#define   EQ_RX_IMP_RESP_SHIFT            24

#define   EQ_TX_IMP_RESP                  0x02000000    //1 the eq has an impulse shape response, 0= use the RAM or ROM for eq coefficients
#define   EQ_TX_IMP_RESP_SHIFT            25

#define   EQ_RX_ENABLE                    0x04000000    //equalizer transmit enable
#define   EQ_RX_ENABLE_SHIFT              26

#define   EQ_TX_ENABLE                    0x08000000    //equalizer receive enable
#define   EQ_TX_ENABLE_SHIFT              27

#define   SOFT_INIT                       0x80000000    //initializes state machines and registers
#define   SOFT_INIT_SHIFT                 31

  uint32        reg_cha_hybal_config;                   // (0x24)
#define   HISPD_HYBAL_SHFT                0x00000007    //
#define   HISPD_HYBAL_SHFT_SHIFT          0

#define   LOSPD_HYBAL_SHFT                0x00000038    //
#define   LOSPD_HYBAL_SHFT_SHIFT          3

#define   HISPD_HYBAL_EN                  0x00000080    //
#define   HISPD_HYBAL_EN_SHIFT            7

#define   LOSPD_HYBAL_EN                  0x00000100    //
#define   LOSPD_HYBAL_EN_SHIFT            8

#define   HISPD_HYBAL_SMPL_OFFSET         0x00007000    //
#define   HISPD_HYBAL_SMPL_OFFSET_SHIFT   12

#define   YFLTR_EN                        0x00008000    //1=y-filter is active 0=y-filter outputs 0 only
#define   YFLTR_EN_SHIFT                  15

#define   HPF_EN                          0x00010000    //1=high pass filter is active 0=high pass filter outputs 0 only
#define   HPF_EN_SHIFT                    16

#define   LOSPD_HYBAL_SMPL_OFFSET         0x3f000000    //
#define   LOSPD_HYBAL_SMPL_OFFSET_SHIFT   24

  uint32        reg_cha_int_config_1;                   // (0x28)
#define   ASRC_INT_NUM_SECT               0x001f0000    //5 bits--if symmetric it represents 1/2 of the number of sections (minus 1) in the filter channel a
#define   ASRC_INT_NUM_SECT_SHIFT         16            //  if asymmetric, it represents the number of sections (minus 1) channel a

#define   ASRC_INT_HRNR_SHFT              0x03000000    //2 bits defines shifting at output of asrc int hrnr channel a
#define   ASRC_INT_HRNR_SHFT_SHIFT        24

#define   ASRC_INT_SYM_ASYM               0x04000000    //1=filter coefficients are symmetrical (only 1/2 are stored) channel a
#define   ASRC_INT_SYM_ASYM_SHIFT         26

#define   ASRC_INT_COEFF_SEL              0x18000000    //
#define   ASRC_INT_COEFF_SEL_SHIFT        27

  uint32        reg_cha_int_config_2;                   // (0x2c)
#define   ASRC_INT_SCALE                  0x0000ffff    //16 bit scale value channel a
#define   ASRC_INT_SCALE_SHIFT            0

#define   ASRC_INT_FIR_SHFT               0x00070000    //3 bits defines shifting at output of mac before writing into data RAM channel a
#define   ASRC_INT_FIR_SHFT_SHIFT         16

#define   ASRC_SLEW_SPEED                 0x00780000    //
#define   ASRC_SLEW_SPEED_SHIFT           19

  uint32        reg_cha_pm_config_1;                    // (0x30)
#define   PM_ON_TIME                      0x00000fff    //12 bit number in 2 msec units for on-time channel a
#define   PM_ON_TIME_SHIFT                0

#define   PM_OFF_TIME                     0x0fff0000    //12 bit number in 2 msec units for off-time channel a
#define   PM_OFF_TIME_SHIFT               16

#define   PM_FREQ_16_12                   0x10000000    //1=16 kHz frequency 0=12 kHz frequency channel a
#define   PM_FREQ_16_12_SHIFT             28

#define   PM_TIME_MS                      2 / 5


  uint32        reg_cha_pm_config_2;                    // (0x34)
#define   PM_SCALE                        0x0000ffff    //16 bit 2's complement number that scales the output from full scale channel a
#define   PM_SCALE_SHIFT                  0

#define   PM_RISE_INC                     0x00ff0000    //8 bit number goes into accumulator for rise time channel a
#define   PM_RISE_INC_SHIFT               16

#define   PM_FALL_INC                     0xff000000    //8 bit number goes into accumulator for fall time channel a
#define   PM_FALL_INC_SHIFT               24

  uint32        reg_cha_pm_config_3;                    // (0x38)
#define   PM_BURST_LENGTH                 0x0000ffff    //16 bit number defines the number of bursts to send out
#define   PM_BURST_LENGTH_SHIFT           0

#define   PM_BURST_CONT                   0x00010000    //1=send out infinite number of bursts 0=send out "pm_burst_length"
#define   PM_BURST_CONT_SHIFT             16

#define   PM_BURST_START                  0x00020000    //strobe the indicates that a burst should start (this will be edge detected)
#define   PM_BURST_START_SHIFT            17

#define   PM_ENABLE                       0x00040000    //1=pulse metering is active and on, 0=inactive and off channel a
#define   PM_ENABLE_SHIFT                 18

  uint32        reg_cha_cic_config;                     // (0x3c)
#define   CIC_DEC_SHFT                    0x00000007    //3 bits controls output shifter (powers of 2 gain control) channel a
#define   CIC_DEC_SHFT_SHIFT              0

#define   CIC_INC_SHFT                    0x00000038    //3 bits controls output shifter (powers of 2 gain control) channel a
#define   CIC_INC_SHFT_SHIFT              3

  uint32        reg_cha_asrc_dec_config;                // (0x40)
#define   ASRC_DEC_SCALE                  0x0000ffff    //16 bit 2's complement scale value channel a
#define   ASRC_DEC_SCALE_SHIFT            0

#define   ASRC_DEC_NUM_SECT               0x001f0000    //5 bit number 0-20 defines the number of sections in the filter channel a
#define   ASRC_DEC_NUM_SECT_SHIFT         16            //   if symmetric it equals the number of sections/2 -1
                                                        //   if asymmetric it equals the number of sections -1
#define   ASRC_DEC_SHFT                   0x0f000000    //4 bits defines msb location at input [38:31] channel a
#define   ASRC_DEC_SHFT_SHIFT             24

#define   ASRC_DEC_SYM_ASYM               0x10000000    //1=symmetric filter 0=asymmetric filter channel a
#define   ASRC_DEC_SYM_ASYM_SHIFT         28

#define   ASRC_DEC_COEFF_SEL              0x60000000    //ASRC DEC coefficient select
#define   ASRC_DEC_COEFF_SEL_SHIFT        29

  uint32        reg_cha_fcw_config_1;                   // (0x44)
#define   FCW_SCALE                       0xffffffff    //32 bit unsigned scale value for frequency control word channel a

  uint32        reg_cha_fcw_config_2;                   // (0x48)
#define   FCW_SHFT                        0x0000000f    //4 bit shift control for fcw channel a
#define   FCW_SHFT_SHIFT                  0

#define   CLOCK_SOURCE                    0x00000000    //2 bit mux control 0=DPLL 1=misc 2=NTP 3=TRC
#define   CLOCK_SOURCE_SHIFT              4


  uint32        reg_cha_digmod_config;                  // (0x4c)
#define   DIGMOD_DEM_TONE                 0x00000001    //1=suppress tones at fs/2 channel a
#define   DIGMOD_DEM_TONE_SHIFT           0

#define   DIGMOD_DEM_DIS                  0x00000002    //1=use balanced code as output instead of dynamic element matcher channel a
#define   DIGMOD_DEM_DIS_SHIFT            1

#define   DIGMOD_DITH_SCALE               0x0000ff00    // new 10/1
#define   DIGMOD_DITH_SCALE_SHIFT         8


// CH B use same bit position defines as Ch A
  uint32        reg_chb_eq_config;                      // (0x50)
  uint32        reg_chb_hybal_config;                   // (0x54)
  uint32        reg_chb_int_config_1;                   // (0x58)
  uint32        reg_chb_int_config_2;                   // (0x5c)

  uint32        reg_chb_pm_config_1;                    // (0x60)
  uint32        reg_chb_pm_config_2;                    // (0x64)
  uint32        reg_chb_pm_config_3;                    // (0x68)
  uint32        reg_chb_cic_config;                     // (0x6c)
  uint32        reg_chb_asrc_dec_config;                // (0x70)
  uint32        reg_chb_fcw_config_1;                   // (0x74)
  uint32        reg_chb_fcw_config_2;                   // (0x78)
  uint32        reg_chb_digmod_config;                  // (0x7c)

  uint32        reg_fcw_config_1;                       // (0x80)
#define   FCW_REG                         0xffffffff    //32 bit frequency control word from NTP block

  uint32        reg_fcw_config_2;                       // (0x84)
#define   LOAD_NTP                        0x00000001    // triggers pulse to load FCW to NTP
#define   LOAD_NTP_SHIFT                  0

#define   LOAD_MISC_A                     0x00000002    // triggers pulse to load FCW to ch A
#define   LOAD_MISC_A_SHIFT               1

#define   LOAD_MISC_B                     0x00000004    // triggers pulse to load FCW to ch B
#define   LOAD_MISC_B_SHIFT               2

#define   LOAD_FCW_NOM_A                  0x00000008    //strobe to load nominal FCW value
#define   LOAD_FCW_NOM_A_SHIFT            3

#define   LOAD_FCW_NOM_B                  0x00000010    //strobe to load nominal FCW value
#define   LOAD_FCW_NOM_B_SHIFT            4

#define   FCW_LIMIT                       0x00000f00    //4 bits defines allowable error on FCW relative to nominal
#define   FCW_LIMIT_SHIFT                 8

  uint32        reg_ntp_config_1;                       // (0x88)
#define   NTP_TIME_DVD                    0x0000ffff    //16 bit divide value for time counter (16'd374 is default)

  uint32        reg_ntp_config_2;                       // (0x8c)
#define   NTP_READ_TIME                   0x00000001    //strobe asks for new time count value

  uint32        reg_ring_config_1;                      // (0x90)
#define   RING_OFFSET_A                   0x000000ff    //
#define   RING_OFFSET_A_SHIFT             0

#define   RING_SCALE_A                    0x0000ff00    //
#define   RING_SCALE_A_SHIFT              8

#define   RING_OFFSET_B                   0x00ff0000    //
#define   RING_OFFSET_B_SHIFT             16

#define   RING_SCALE_B                    0xff000000    //
#define   RING_SCALE_B_SHIFT              24

  uint32        reg_ring_config_2;                      // (0x94)
#define   RING_CLIP_A                     0x00007fff    //15 bit number defining the clipping value to flatten
#define   RING_CLIP_A_SHIFT               0

#define   RING_GEN_ENABLE_A               0x00008000    //1=enabled
#define   RING_GEN_ENABLE_A_SHIFT         15

#define   RING_CLIP_B                     0x7fff0000    //15 bit number defining the clipping value to flatten
#define   RING_CLIP_B_SHIFT               16

#define   RING_GEN_ENABLE_B               0x80000000    //1=enabled
#define   RING_GEN_ENABLE_B_SHIFT         31

  uint32        reg_ring_config_3;                      // (0x98)
#define   RING_OMEGA_NORM_A               0x000000ff    //the omega man is Charlton Heston on channel A
#define   RING_OMEGA_NORM_A_SHIFT         0

#define   RING_OMEGA_NORM_B               0x0000ff00    //the omega man is Charlton Heston on channel B
#define   RING_OMEGA_NORM_B_SHIFT         8

#define   RING_BREAK_PHASE_A              0x003f0000    //break phase
#define   RING_BREAK_PHASE_A_SHIFT        16

#define   RING_BREAK_PHASE_B              0x0fc00000    //break phase
#define   RING_BREAK_PHASE_B_SHIFT        22

#define   RING_BREAK_PHASE_SIGN_A         0x10000000    //1=break phase sign
#define   RING_BREAK_PHASE_SIGN_A_SHIFT   28

#define   RING_BREAK_PHASE_SIGN_B         0x20000000    //1=break phase sign
#define   RING_BREAK_PHASE_SIGN_B_SHIFT   29

#define   RING_START_CADENCE_A            0x40000000    //1=start cadence
#define   RING_START_CADENCE_A_SHIFT      30

#define   RING_START_CADENCE_B            0x80000000    //1=start cadence
#define   RING_START_CADENCE_B_SHIFT      31

  uint32        reg_ring_config_4;                      // (0x9c)
#define   RING_STOP_IMMED_A               0x00000001    // stop ring now!
#define   RING_STOP_IMMED_A_SHIFT         0

#define   RING_STOP_IMMED_B               0x00000002    // stop ring now!
#define   RING_STOP_IMMED_B_SHIFT         1

#define   RING_STOP_NEXT_BREAK_A          0x00000004    // stop ring at next break
#define   RING_STOP_NEXT_BREAK_A_SHIFT    2

#define   RING_STOP_NEXT_BREAK_B          0x00000008    // stop ring at next break
#define   RING_STOP_NEXT_BREAK_B_SHIFT    3

  uint32        reg_ser_config;                         // (0xa0)
#define   SER_MUX_SEL                     0x00000003
#define   SER_MUX_SEL_SHIFT               0

//#define   SER_TST_SHFT_CTRL               0x07800000    //
#define   AUD_SER_TST_FCW_MUX_CTRL        0x08000000    //
#define   AUD_SER_TST_FCW_MUX_CTRL_SHIFT  27

//#define   SER_TST_LOAD_REGFILE            0x20000000    //1->use channel b requests for transfer 0-> use channel a
#define   SER_TST_CLKS                    0x40000000    //1=use clocks supplied by SER 0=generate internal clocks
#define   SER_TST_CLKS_SHIFT              30

#define   SOFT_INIT_AP                    0x80000000
#define   SOFT_INIT_AP_SHIFT              31

  uint32        reg_spare;                              // (0xa4)
//#define   SER_TST_SCALE                   0xffffffff

  uint32        reg_stats_config;                            // (0xa8)
#define   START_STATS                     0x00008000    //[15]
#define   STATS_MUX_CTL_VADCA             0x00000800    //[11]
#define   STATS_MUX_CTL_VADCB             0x00000400    //[10]
#define   STATS_CHOP_ENABLE_A             0x00000200    //[9]
#define   STATS_CHOP_ENABLE_B             0x00000100    //[8]
#define   STATS_DURATION                  0x000000f0    //[7:4]
#define   STATS_CHOP_PERIOD               0x0000000f    //[3:0]

  // APM Status Registers
  uint32        reg_apm_status_1;                       // (0xac)
#define   DIGMOD_SAT_QUANT                0x01000000    //strobe indicating quantizer has saturated
#define   DIGMOD_SAT_COUNT_0              0x00ff0000    //8 bit counter of saturation on integrator 0 channel a
#define   DIGMOD_SAT_COUNT_1              0x0000ff00    //8 bit counter of saturation on integrator 1 channel a
#define   DIGMOD_SAT_COUNT_QUANT          0x000000ff    //8 bit counter of saturation on quantizer channel a

  uint32        reg_apm_status_2;                       // (0xb0) same as status 1 for ch B

  uint32        reg_apm_status_3;                       // (0xb4)
#define   AP_XMT_UFLOW                    0x00200000    //level indicating xnt fifo is empty when read attempted
#define   AP_XMT_OVLOW                    0x00100000    //level indicating xmt fifo is full when write attempted
#define   AP_NCO_SATURATED                0x00080000    // NCO is saturated
#define   AP_XMT_FIFO_DEPTH               0x00070000    //3 bit depth number on xmt fifo
#define   AP_RCV_UFLOW                    0x00000020    //level indicating rcv fifo is empty when read attempted
#define   AP_RCV_OVLOW                    0x00000010    //level indicating rcv fifo is full when write attempted
#define   AP_RCV_FIFO_DEPTH               0x00000007    //3 bit depth number on receive fifo

  uint32        reg_apm_status_4;                       // (0xb8)
#define   EQ_RX_COEFF                     0xffff0000    //16 bit 2's complement coefficients from RAM
#define   EQ_TX_COEFF                     0x0000ffff    //16 bit coefficient read from RAM or ROM

  uint32        reg_apm_status_5;                       // (0xbc)
#define   ASRC_INT_COEFF                  0xffff0000    //16 bit 2's comp current coefficient value
#define   ASRC_DEC_COEFF                  0x0000ffff    //16 bit coefficient from RAM or ROM

  uint32        reg_apm_status_6;                       // (0xc0)
#define   YFLTR_COEFF                     0x000fffff    //20 bit 2's complement coefficients from RAM

// Status 9-7 same as 6-4 for Ch B
  uint32        reg_apm_status_7;                       // (0xc4)
  uint32        reg_apm_status_8;                       // (0xc8)
  uint32        reg_apm_status_9;                       // (0xcc)

  uint32        reg_apm_status_10;                      // (0xd0)
#define   NTP_TIME_COUNT_INTEGER          0x0001ffff    //17 integer bits (in seconds)

  uint32        reg_apm_status_11;                      // (0xd4)
#define   NTP_TIME_COUNT_FRACT            0x0003ffff    //18 fractional bits (in seconds)

  uint32        reg_apm_status_12;                      // (0xd8)
#define   RING_CADENCE_STOPPED_A          0x00000008    //
#define   RING_ON_OFF_A                   0x00000004    //
#define   RING_CADENCE_STOPPED_B          0x00000002    //
#define   RING_ON_OFF_B                   0x00000001    //

  uint32        reg_apm_status_13;                      // (0xdc)
#define   HISPD_HYBAL_COEFF_A             0xffff0000    //
#define   HISPD_HYBAL_COEFF_B             0x0000ffff    //

  uint32        reg_apm_status_14;                      // (0xe0)
#define   LOSPD_HYBAL_COEFF_A             0xffff0000    //
#define   LOSPD_HYBAL_COEFF_B             0x0000ffff    //

  uint32        reg_apm_stats_1;                        // (0xe4)
#define   MEAN                            0xffffffff    // 16 bit mean stats

  uint32        reg_apm_stats_2;                        // (0xe8), same as reg_apm_stats_1 for channel B
  uint32        reg_apm_stats_3;                        // (0xec)
#define   MEAN_SQUARE_LO                  0xffffffff    // lower 32 bits of 40 bit mean square value

  uint32        reg_apm_stats_4;                        // (0xf0)
#define   MEAN_SAT                        0x00040000
#define   MEAN_SQUARE_SAT                 0x00020000
#define   STATS_BUSY                      0x00010000    // 1 bit stats busy
#define   MEAN_SQUARE_HI                  0x00003fff    // upper 14 bits of 46 bit mean square value

  uint32        reg_apm_stats_5;                        // (0xf4), same as reg_apm_stats_3 for channel B
  uint32        reg_apm_stats_6;                        // (0xf8), same as reg_apm_stats_4 for channel B

  uint32        reg_apm_audio_fcw_a;                    // (0xfc)
  uint32        reg_apm_audio_fcw_b;                    // (0x100)

  uint32        reg_apm_irq_pend_1;                     // (0x104) Ch A status
#define   PM_BURST_DONE                   0x80000000    //strobe indicating that PM burst has completed
#define   ASRC_INT_DONE_SLEWING           0x40000000    //strobe indicating that slewing has completed
#define   ASRC_DEC_DONE_SLEWING           0x20000000    //strobe indicating that slewing has completed
#define   RX_PATH_SAT                     0x10000000    //strobed high when audio + pulse meter + y-filter exceeds 16 bit number
#define   EQ_TX_SAT                       0x08000000    //strobe indicates that the result on the eq tx path was saturated
#define   EQ_RX_SAT                       0x04000000    //strobe indicates that the result on the rx path was saturated at output
#define   EQ_TX_RQST_ERR                  0x02000000    //strobe indicating a tx_rqst was asked for while a tx was pending
#define   EQ_RX_RQST_ERR                  0x01000000    //strobe indicating a rx_rqst was asked for while a rx was pending
#define   LOSPD_HYBAL_SAT                 0x00800000    //strobe indicates that the result on the tx path was saturated at output
#define   HISPD_HYBAL_SAT                 0x00400000    //strobe indicates that the result on the tx path was saturated at output
#define   HISPD_HYBAL_RQST_ERR            0x00200000    //strobe indicating a output_rqst was asked for while computation engine was busy
#define   YFLTR_IIR1_SAT                  0x00100000    //strobe indicating iir-1 is saturating
#define   YFLTR_FIR_SAT                   0x00080000    //strobe indicating fir is saturating
#define   YFLTR_IIR2_SAT                  0x00040000    //strobe indicating iir-2 is saturating
#define   YFLTR_IIR1_ACCUM_SAT            0x00020000    //strobe indicating iir1 accumulator saturated
#define   YFLTR_FIR_ACCUM_SAT             0x00010000    //strobe indicating fir accumulator saturated
#define   YFLTR_IIR2_ACCUM_SAT            0x00008000    //strobe indicating iir2 accumulator saturated
#define   YFLTR_RQST_ERR                  0x00004000    //strobe indicating a output_rqst was asked for while computation engine was busy
#define   NCO_SAT                         0x00002000    //strobe indicates scaling and shifting saturated the result
#define   ASRC_INT_HRNR_TIM_ERR           0x00001000    //strobe indicating delta has rolled over too quickly before fir has completed computation
#define   ASRC_INT_HRNR_SAT               0x00000800    //strobe indicating data has saturated at the mac output
#define   ASRC_INT_FIR_TIM_ERR            0x00000400    //strobe indicating delta has rolled over too quickly before fir has completed computation
#define   ASRC_INT_FIR_BANK_ERR           0x00000200    //strobe indicating that horner ngn asked for a bank switch while fir was busy
#define   ASRC_INT_FIR_SAT                0x00000100    //strobe indicating data has saturated at the mac output
#define   ASRC_INT_HRNR_MAC_SAT           0x00000080    //strobe indicating ser mac in hrnr block has saturated
#define   CIC_INT_SAT                     0x00000040    //strobe indicates that the result on the tx path was saturated at output
#define   DIGMOD_SAT_0                    0x00000020    //strobe indicating the integrator 0 has saturated
#define   DIGMOD_SAT_1                    0x00000010    //strobe indicating integrator 1 has saturated
#define   TX_PATH_SAT                     0x00000008
#define   CIC_DEC_SAT                     0x00000004    //strobe indicates that the result on the tx path was saturated at output
#define   ASRC_DEC_TIM_ERR                0x00000002    //strobe asserted when "input_data_ready" is asserted while ngn is busy
#define   ASRC_DEC_SAT                    0x00000001    //strobe indicating that the input was saturated

  uint32        reg_apm_irq_mask_1;                     // (0x108)

  uint32        reg_apm_irq_pend_2;                     // (0x10c) same as APM_IRQ_1 for Ch B
  uint32        reg_apm_irq_mask_2;                     // (0x110)

  uint32        reg_apm_irq_pend_3;                     // (0x114)
#define   STATS_DONE_A                    0x00000400    //strobe indicating statistics gathered and computed chA
#define   STATS_DONE_B                    0x00000200    //strobe indicating statistics gathered and computed chB
#define   FCW_OOB_A                       0x00000100    //level indicating that FCW error is too large
#define   FCW_OOB_B                       0x00000080    //level indicating that FCW error is too large
#define   HPF_SAT_A                       0x00000040    //strobe indicating high pass filter has saturated, Channel A
#define   HPF_SAT_B                       0x00000020    //strobe indicating high pass filter has saturated, Channel B
#define   ASRC_INT_DELTA_ADJ_DONE_A       0x00000010    //strobe indicating TODO
#define   ASRC_INT_DELTA_ADJ_DONE_B       0x00000008    //strobe indicating TODO
#define   LOSPD_HYBAL_RQST_ERR_A          0x00000004    //strobe indicating a output_rqst was asked for while computation engine was busy, channel A
#define   LOSPD_HYBAL_RQST_ERR_B          0x00000002    //strobe indicating a output_rqst was asked for while computation engine was busy, channel B
#define   NTP_TIME_VALID                  0x00000001    //strobe in clk_200 domain (4 clock periods)

  uint32        reg_apm_irq_mask_3;                     // (0x118)
  uint32        reg_spare_2;                            // (0x11c)

  uint32        reg_codec_config_0;                     // (0x120)
#define   REF_CONTROL                     0x00000007    // control for bandgap voltage switching matrix
#define   REF_REFAMP_PD                   0x00000010    // power down for 50uA ref amp
#define   REF_BIASAMP_PD                  0x00000020    // power down for 10uA bias amp
#define   REF_BANDGAP_PD                  0x00000040    // power down for bandgap
#define   REF_SPARE                       0x00000f00    // reserved for future use

  uint32        reg_codec_config_1;                     // (0x124) VTX1
#define   VTX_PGA_MUX                     0x00000300    // analog loopback VDAC out to MICPGA in
#define   VTX_PG                          0x00001800    // gain setting
#define   VTX_CALEN                       0x00002000    // calibration enable
#define   VTX_OSCRST                      0x00004000    // unstable status reset
#define   VTX_PWD                         0x00008000    // ADC power down
#define   VTX_VOCM_CTL                    0x00070000    // opamp output common mode control
#define   VTX_FBIAS_CTL                   0x00300000    // flash bias control
#define   VTX_INT1BIAS_CTL                0x00c00000    // int1 bias control
#define   VTX_INT2BIAS_CTL                0x03000000    // int2 bias control
#define   VTX_VOS_ENA                     0x04000000    // ADC voltage offset enable
#define   VTX_SHUFF_DIS                   0x08000000    // shuffle disable
#define   VTX_SHUFF_DELAY_DIS             0x10000000    // shuffle feedback delay disable
#define   VTX_DITHER_DIS                  0x20000000    // dither disable
#define   VTX_DCANCEL_DIS                 0x40000000    // dither cancellation algorithm disable

#define   PGA_CTL_VADC                    0x30000000
#define   PGA_CTL_VADC_SHIFT                      28

#define   PGA_GAIN_CTL_VDAC_A             0x00000100
#define   PGA_GAIN_CTL_VDAC_A_SHIFT                8

#define   PGA_GAIN_CTL_VDAC_B             0x00000010
#define   PGA_GAIN_CTL_VDAC_B_SHIFT                4

  uint32        reg_codec_config_2;                     // (0x128) same as config 16 for VTX2

  uint32        reg_codec_config_3;                     // (0x12c) VRX1
#define   VRX_PG                          0x00000003    // PGA gain setting
#define   VRX_ASP_BYPASS                  0x00000080    // bypass testmode
#define   VRX_AUXPG                       0x00000300    // aux PGA gain setting
#define   VRX_PD                          0x00008000    // power down
#define   VRX_CMCTL                       0x00070000    // common mode voltage control
#define   VRX_PDDELAY                     0x00300000
#define   VRX_IDAC_CM_CTL                 0x03000000    // IDAC common mode voltage control
#define   VRX_ALOOPBACK                   0x0c000000    // Analog loopback test mode

  uint32        reg_codec_config_4;                     // (0x130) same as config 18 for VRX2
#define   VREF_CTL_VDAC                   0x70000000
#define   PWRDN_REF_VDAC                  0x01000000
#define   PWRDN_REF_VDAC_A                0x00020000
#define   TESTEN_VDAC_A                   0x00010000
#define   DRVVOCMCTL_VDAC                 0x00007000
#define   PGA_GAIN_CTL_VDAC_A             0x00000100
#define   PWRDN_REF_VDAC_B                0x00000040
#define   TESTEN_VDAC_B                   0x00000020
#define   PGA_GAIN_CTL_VDAC_B             0x00000010

  uint32        reg_codec_config_5;                     // (0x134) 32Khz Ring Gen config
#define   RING_PD                         0x00000001
#define   RING_RESETB                     0x00000002
#define   REF_RESETB                      0x00000004
#define   VTX1_RESETB                     0x00000008
#define   VTX2_RESETB                     0x00000010
#define   VRX1_RESETB                     0x00000020
#define   VRX2_RESETB                     0x00000040

  uint32        reg_codec_config_6;                     // (0x138) reserved
  uint32        reg_codec_config_7;                     // (0x13c) reserved
  uint32        reg_codec_config_8;                     // (0x140) reserved

  uint32        reg_apm_codec_fb_0;                     // (0x144) 32Khz Feedback
  uint32        reg_mbist_0;                            // (0x148) MBIST control register
#define   P16_CORE_INST_MEM_TM            0x0000000f
#define   P16_CORE_INST_ROM_TM            0x000001f0
#define   BMU_BUS_MAIN_IF_TM              0x00000600
#define   APM_EQ_DATA_RAM_TM_RX_A         0x00001800
#define   APM_EQ_DATA_RAM_TM_RX_B         0x00006000
#define   APM_EQ_DATA_RAM_TM_TX_A         0x00018000
#define   APM_EQ_DATA_RAM_TM_TX_B         0x00060000
#define   APM_EQ_COEFF_RAM_TM_RX_A        0x00180000
#define   APM_EQ_COEFF_RAM_TM_RX_B        0x00600000
#define   APM_EQ_COEFF_RAM_TM_TX_A        0x01800000
#define   APM_EQ_COEFF_RAM_TM_TX_B        0x06000000
#define   APM_Y_FLTR_TM_A                 0x18000000
#define   APM_Y_FLTR_TM_B                 0x60000000
  uint32        reg_mbist_1;                            // (0x14c) MBIST control register
#define   ASRC_DEC_COEFF_RAM_TM_A         0x00000003
#define   ASRC_DEC_DATA_RAM_TM_A          0x0000000c
#define   ASRC_DEC_COEFF_RAM_TM_B         0x00000030
#define   ASRC_DEC_DATA_RAM_TM_B          0x000000c0
#define   ASRC_INTERPFIR_COEFF_RAM_TM_A   0x00000300
#define   ASRC_INTERPFIR_DATA_RAM_TM_A    0x00000c00
#define   ASRC_INTERPFIR_COEFF_RAM_TM_B   0x00003000
#define   ASRC_INTERPFIR_DATA_RAM_TM_B    0x0000c000
  uint32        reg_otp_0;                              // (0x150) OTP read back, otp_apm[31:0]
#define   APM_OTP_TRIM                    0x00000FC0
#define   APM_OTP_TRIM_SHIFT                       6
  uint32        reg_otp_1;                              // (0x154) OTP read back, otp_apm[63:32]
  uint32        reg_diag_readback;                      // (0x158) diag bus read back

  uint32        audio_spare2;                           // (0x15c) Audio Spare Register 2
  uint32        dpll_control;                           // (0x160) DPLL control register
#define   DPLL_SOFT_INIT                  0x80000000
#define   DPLL_FORCE_ACQ                  0x40000000
#define   DPLL_OPEN_LOOP                  0x20000000
#define   DPLL_CONST_K                    0x10000000
#define   DPLL_PHS_THSHLD_SHIFT           4
  uint32        dpll_nom_freq;                          // (0x164) DPLL nominal frequency (control)
  uint32        dpll_div;                               // (0x168) DPLL divide register
#define   DPLL_REF_DIV_SHIFT              16
  uint32        dpll_acq_freq;                          // (0x16c) DPLL acquired frequency
  uint32        dpll_status;                            // (0x170) DPLL status register
#define   DPLL_IN_SYNC                    0x80000000
#define   DPLL_ACQ_FREQ_VALID             0x40000000
#define   DPLL_IN_SYNC                    0x80000000
#define   DPLL_K0_SHIFT                   8

   /* PCM registers - these are detailed in pcm6816.h */
   /* HVG registers - these are detailed in hvg6816.h */
} ApmControlRegisters;

#define APM               ((volatile ApmControlRegisters *)APM_BASE)

#define BCM6816_IUDMA_MAX_CHANNELS          6

typedef struct ApmPcmIudmaRegisters
{
	uint16 numChp;
#define BCM6816_IUDMA_REGS_NUM_CHP_MASK     0x0f00
#define BCM6816_IUDMA_REGS_NUM_CHP_SHIFT    8
   uint16 ctrlConfig;
#define BCM6816_IUDMA_REGS_CTRLCONFIG_MASTER_EN        0x0001
#define BCM6816_IUDMA_REGS_CTRLCONFIG_FLOWC_CH1_EN     0x0002
#define BCM6816_IUDMA_REGS_CTRLCONFIG_FLOWC_CH3_EN     0x0004

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

   uint16 reserved7;
   uint16 channelReset;

   uint16 reserved8;
   uint16 channelDebug;

   uint32 reserved9;

   uint32 intStatus;

   uint32 intMask;

   // Unused words
   uint16 reserved_u16s[104];
} ApmPcmIudmaRegisters;

/***** TBD. This is the BCM6368 definition.  Need BCM6816 definition. *****/
typedef struct ApmPcmIudmaChannelCtrl
{
    uint16 reserved1;
    uint16 config;
#define BCM6816_IUDMA_CONFIG_ENDMA       0x0001
#define BCM6816_IUDMA_CONFIG_PKTHALT     0x0002
#define BCM6816_IUDMA_CONFIG_BURSTHALT   0x0004

    uint16 reserved2;
    uint16 intStat;
#define BCM6816_IUDMA_INTSTAT_BDONE   0x0001
#define BCM6816_IUDMA_INTSTAT_PDONE   0x0002
#define BCM6816_IUDMA_INTSTAT_NOTVLD  0x0004
#define BCM6816_IUDMA_INTSTAT_MASK    0x0007
#define BCM6816_IUDMA_INTSTAT_ALL     BCM6816_IUDMA_INTSTAT_MASK

    uint16 reserved3;
    uint16 intMask;
#define BCM6816_IUDMA_INTMASK_BDONE   0x0001
#define BCM6816_IUDMA_INTMASK_PDONE   0x0002
#define BCM6816_IUDMA_INTMASK_NOTVLD  0x0004

    uint32 maxBurst;
#define BCM6816_IUDMA_MAXBURST_SIZE 16 /* 32-bit words */

} ApmPcmIudmaChannelCtrl;


typedef struct ApmPcmIudmaStateRam
{
   uint32 baseDescPointer;                /* pointer to first buffer descriptor */

   uint32 stateBytesDoneRingOffset;       /* state info: how manu bytes done and the offset of the
                                             current descritor in process */
#define BCM6816_IUDMA_STRAM_DESC_RING_OFFSET 0x3fff


   uint32 flagsLengthStatus;              /* Length and status field of the current descriptor */

   uint32 currentBufferPointer;           /* pointer to the current descriptor */

} ApmPcmIudmaStateRam;


typedef struct ApmPcmIudma
{
   ApmPcmIudmaRegisters regs;                                        //
   ApmPcmIudmaChannelCtrl ctrl[BCM6816_IUDMA_MAX_CHANNELS];        //
   uint32 reserved[4*10];
   ApmPcmIudmaStateRam stram[BCM6816_IUDMA_MAX_CHANNELS];          //

} ApmPcmIudma;

#define APM_PCM_IUDMA ((volatile ApmPcmIudma * const) APM_PCM_IUDMA_BASE)




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
    byte imp_rgmii_ctrl_p2; /* 0x64 */
    byte imp_rgmii_ctrl_p3; /* 0x65 */
    byte reserved8[6]; /* 0x66 - 0x6b */
    byte rgmii_timing_delay_p2; /* 0x6c */
    byte gmii_timing_delay_p3; /* 0x6d */
    byte reserved9[11]; /* 0x6e - 0x78 */
    byte software_reset; /* 0x79 */
    byte reserved13[6]; /* 0x7a - 0x7f */
    byte pause_frame_detection; /* 0x80 */
    byte reserved10[7]; /* 0x81 - 0x87 */
    byte fast_aging_ctrl; /* 0x88 */
    byte fast_aging_port; /* 0x89 */
    byte fast_aging_vid; /* 0x8a */
    byte reserved11[21]; /* 0x8b - 0x9f */
    unsigned int swpkt_ctrl_usb; /*0xa0 */
    unsigned int swpkt_ctrl_gpon; /*0xa4 */
    unsigned int iudma_ctrl; /*0xa8 */
    unsigned int iudma_queue_ctrl; /*0xac */
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

