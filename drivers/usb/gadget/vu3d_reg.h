/*
 * via u3lite udc - hw register header
 *
 * Copyright (C) 2014 VIA Corporation. <yhliu@via.com.tw>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN
 * NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef __U3DC_REG_H
#define __U3DC_REG_H

#include "vu3d_precomp.h"

#define ASYNC_UPKTQ_SIZE                (4096)
#define ASYNC_UPKTQ_ALLOCATION          (ASYNC_UPKTQ_SIZE * 2)
#define ASYNC_UPKTQ_ELEMENT             (256)
#define ASYNC_LAST_ELEMENT              (ASYNC_UPKTQ_ELEMENT - 1)
#define ASYNC_UPKTQ_ALIGNMENT           (4096)
#define ASYNC_MAX_UPKT_TX_BYTE          (1024)

/* 128KB = ASYNC_UPKTQ_ELEMENT/2 * ASYNC_MAX_UPKT_TX_BYTE */
#define ASYNC_MAX_URB_TX_BYTE           (0x20000)     

#define REG_OFFSET(MEMBER)              (offsetof(struct u3lite_register, MEMBER))
#define U3HC_REG_OFFSET(MEMBER)         (offsetof(struct u3lite_hc_register, MEMBER))
#define U3DC_REG_OFFSET(MEMBER)         (offsetof(struct u3lite_register, MEMBER))

#define REG_DUMP_VALUE(MEMBER)          REG_OFFSET(MEMBER), #MEMBER, MEMBER
#define U3HC_REG_DUMP_VALUE(MEMBER)     U3HC_REG_OFFSET(MEMBER), #MEMBER, MEMBER
#define U3DC_REG_DUMP_VALUE(MEMBER)     U3DC_REG_OFFSET(MEMBER), #MEMBER, MEMBER


enum port_speed {
        ps_undefined        = 0,
        ps_full             = 1,
        ps_low              = 2,
        ps_high             = 3,
        ps_super            = 4
};

enum port_link_state {
        pls_u0              = 0,
        pls_u1              = 1,
        pls_u2              = 2,
        pls_u3              = 3,
        pls_disabled        = 4,
        pls_rxdetect        = 5,
        pls_inactive        = 6,
        pls_polling         = 7,
        pls_recovery        = 8,
        pls_hotreset        = 9,
        pls_compliance      = 10,
        pls_test            = 11,
        pls_resume          = 15
};

enum usb_packet_type {
        upkt_type_imp           = 0x00,
        upkt_type_tp            = 0x04,
        upkt_type_dph           = 0x08,
        upkt_type_itp           = 0x0C
};

enum usb_packet_sub_type {
        upkt_sttp_ack           = 0x01,
        upkt_sttp_nrdy          = 0x02,
        upkt_sttp_erdy          = 0x03,
        upkt_sttp_status        = 0x04,
        upkt_sttp_stall         = 0x05,
        upkt_sttp_dev_notify    = 0x06,
        upkt_sttp_ping          = 0x07,
        upkt_sttp_ping_res      = 0x08
};

enum usb_packet_direction {
        upkt_dir_h2d            = 0x00,
        upkt_dir_d2h            = 0x01,
        upkt_dir_ignore         = 0x00
};

enum usb_packet_max_data_size {
        upkt_mds_ctrl           = 512,
        upkt_mds_bulk           = 1024,
        upkt_mds_int            = 1024,
        upkt_mds_isoch          = 1024
};

enum usb_packet_max_data_size_shift {
        upkt_mdss_ctrl          = 9,
        upkt_mdss_bulk          = 10,
        upkt_mdss_int           = 10,
        upkt_mdss_isoch         = 10
};

enum usb_packet_default_seqnum {
        upkt_ds_ep0_read        = 0,
        upkt_ds_ep0_write       = 1,        
        upkt_ds_ep0_status      = 1       
};

enum async_pkt_err_type {
        upkt_err_no             = 0x00,
        upkt_err_tx             = 0x01,
        upkt_err_sp             = 0x02,
        upkt_err_babble         = 0x03
};

enum nrdy_entry_type {
        nrdy_entry_acktp        = 0x00,
        nrdy_entry_dp           = 0x01
};

enum notification_packet_type {
        upkt_dntp_fw            = 0x01,
        upkt_dntp_ltm           = 0x02,
        upkt_dntp_biam          = 0x03,
        upkt_dntp_hrr           = 0x04
};

enum rsp_phase {
        upkt_hrr_initiate       = 0x01,
        upkt_hrr_confirm        = 0x02
};


/* set alignment to 1 byte boundary */
#pragma pack(1) 

/* ACKTP packet queue format */

/* DW0 */
#define BS_TP_TYPE                      0
#define BM_TP_TYPE                      0x1F

#define BS_TP_DEVADDR                   25
#define BM_TP_DEVADDR                   0x3F

#define BS_TP_CYCLE                     31
#define BM_TP_CYCLE                     0x01

/* DW1 */
#define BS_TP_SUBTYPE                   0
#define BM_TP_SUBTYPE                   0x1F

#define BS_TP_NOTIFY_TYPE               4
#define BM_TP_NOTIFY_TYPE               0x0F

#define BS_TP_NOTIFY_TYPE_SPEC1         8
#define BM_TP_NOTIFY_TYPE_SPEC1         0x00FFFFFF


#define BS_TP_RTY                       6
#define BM_TP_RTY                       0x01

#define BS_TP_D                         7
#define BM_TP_D                         0x01

#define BS_TP_EPTNUM                    8
#define BM_TP_EPTNUM                    0x0F

#define BS_TP_HE                        15
#define BM_TP_HE                        0x01

#define BS_TP_NUMP                      16
#define BM_TP_NUMP                      0x1F

#define BS_TP_SEQNUM                    21
#define BM_TP_SEQNUM                    0x1F

/* DW2 */
#define BS_TP_XFER_LENGTH               16
#define BM_TP_XFER_LENGTH               0x7FF

#define BS_TP_PP                        27
#define BM_TP_PP                        0x01

#define BS_TP_CTRL_STATUS               28
#define BM_TP_CTRL_STATUS               0x01

#define BS_TP_CTRL_EP                   29
#define BM_TP_CTRL_EP                   0x01

#define BS_TP_CHAIN                     30
#define BM_TP_CHAIN                     0x01

#define BS_TP_IOC                       31
#define BM_TP_IOC                       0x01

/* DW3 */
#define BS_TP_XFER_ADDR                 0
#define BM_TP_XFER_ADDR                 0xFFFFFFFF

/* DP packet queue format */

/* DW0 */
#define BS_DP_TYPE                      0
#define BM_DP_TYPE                      0x1F

#define BS_DP_DEVADDR                   25
#define BM_DP_DEVADDR                   0x3F

#define BS_DP_CYCLE                     31
#define BM_DP_CYCLE                     0x01

/* DW1 */
#define BS_DP_SEQNUM                    0
#define BM_DP_SEQNUM                    0x1F

#define BS_DP_EOB                       6
#define BM_DP_EOB                       0x01

#define BS_DP_D                         7
#define BM_DP_D                         0x01

#define BS_DP_EPTNUM                    8
#define BM_DP_EPTNUM                    0x0F

#define BS_DP_S                         15
#define BM_DP_S                         0x01

#define BS_DP_DATA_LENGTH               16
#define BM_DP_DATA_LENGTH               0x7FF

/* DW2 */
#define BS_DP_XFER_LENGTH               16
#define BM_DP_XFER_LENGTH               0x7FF

#define BS_DP_PP                        27
#define BM_DP_PP                        0x01

#define BS_DP_CTRLEP                    29
#define BM_DP_CTRL_EP                   0x01

#define BS_DP_CHAIN                     30
#define BM_DP_CHAIN                     0x01

#define BS_DP_IOC                       31
#define BM_DP_IOC                       0x01

/* DW3 */
#define BS_DP_XFER_ADDR                 0
#define BM_DP_XFER_ADDR                 0xFFFFFFFF

struct u3lite_packet {
        __u32 dword[4];
};

/* uhc register definition */

/* Offset: 323-320h XHCI Debug Capability Control Register (DCCTRL) */
#define DBC_DCE                 (1 << 31)

/* Offset: 76D-76Ch (U3IP_SSCFG) SSPORT1 SSPHYLS Option Registers 1 */
#define BS_R_TSPOLDET_SS1       (8)
#define BM_R_TSPOLDET_SS1       (0x07)

/* Offset: 779-778h SSPORT1 SSPMU Option Registers */
#define R_U1LKPMEN_SS1          (1 << 1)
#define R_U2LKPMEN_SS1          (1 << 2)

/* Offset: 77D-77Ch SSPORT1 SSLINK Option Registers2 */
#define R_LFPSDGEN_SS1          (1 << 0)

/* Offset: 787-786h SSPORT1 SSPHYLS Option Registers 7 */
#define R_UPDISEN_SS1           (1 << 7)
#define R_UFPDETEN_SS1          (1 << 10)

/* Offset: 78D-78Ch SS_PORT1 Feature Registers */
#define R_DEGLITCH_EN_SS1       (1 << 4)
#define R_LTSSMDIS_EXITEN_SS1   (1 << 14)
#define R_DIR_RST_SS1           (1 << 28)

/* udc register definition */

/* Offset: D00h xHCI Debug Capability Status INTE Register */
#define DBC_PORTRST_INTE        (1 << 0)
#define DBC_DCECHG_INTE         (1 << 2)
#define CCSCHG_INTE             (1 << 3)
#define U3D_PORTSC_INTE         (1 << 4)

/* Offset: D01h xHCI Debug Capability Control Endpoint INTE Register */
#define CTLEP_RXSETUP_INTE      (1 << 0)

/* Offset: D03-D02h XHCI Debug Capability Status Register */
#define U3D_RUN                 (1 << 1)
#define U3D_RST_FW              (1 << 15)
#define BS_DBC_DEVADDR          (8)
#define BM_DBC_DEVADDR          (0x7F)

/* Offset: D04h XHCI Debug Capability Status Register */
#define DBC_PORTRST             (1 << 0)        /* DBC ONLY */
#define DBC_DCECHG              (1 << 2)        /* DBC ONLY */
#define DBC_CCS_CHG             (1 << 3)

#define DBC_STS_MASK            (DBC_PORTRST | DBC_DCECHG | DBC_CCS_CHG)

/* Offset: D05h XHCI Debug Capability Control Endpoint Status Register */
#define CTLEP_RXSETUPOUT_STS    (1 << 0)
#define CTLEP_RXSETUPIN_STS     (1 << 1)

#define CTLEP_RXSETUP_STS_MASK  (CTLEP_RXSETUPOUT_STS | CTLEP_RXSETUPIN_STS)

/* Offset: D10h XHCI Debug Capability Doorbell and Control INTE Register */
#define DB_INTE                 (1 << 0)
#define BLKIN_HIT_INTE          (1 << 1)
#define BLKOUT_HOT_INTE         (1 << 2)

/* Offset: D11h XHCI Debug Capability IDLE Status Register */
#define DBC_IDLE                (1 << 0)
#define DBC_IDLE_EN             (1 << 1)

/* Offset: D12h XHCI Debug Capability FWDISLINK Register */
#define FWDISLINK               (1 << 0)

/* Offset: D14h XHCI Debug Capability Doorbell and Control Register */
#define BLKIN_DB_STS            (1 << 0)
#define BLKOUT_DB_STS           (1 << 1)
#define BLKIN_HIT_STS           (1 << 2)
#define BLKOUT_HOT_STS          (1 << 3)

#define BLK_DB_MASK             (BLKIN_DB_STS | BLKOUT_DB_STS | BLKIN_HIT_STS | BLKOUT_HOT_STS)

/* Offset: D18h TUSTMOUT_CHKEN Register */
#define TUSTMOUT_CHKEN          (1 << 0)

/* Offset: D21h - D20h XHCI Debug Capability Async Endpoint INTE Register */
#define ASYNCEP_EN              (1 << 0)
#define ASYNCEP_INTE            (1 << 1)
#define ASYNCEP_PKTCMPL_INTE    (1 << 2)
#define ASYNCEP_PKTERR_INTE     (1 << 3)
#define NRDY_ENTRY_INTE         (1 << 4)
#define ASYNCEP_INTE_MASK       (ASYNCEP_INTE | ASYNCEP_PKTCMPL_INTE | ASYNCEP_PKTERR_INTE | NRDY_ENTRY_INTE)

/* Offset: D22h - D23h Async Packet Queue RW Pointer Register */
#define BS_ASYEP_RPTR           (8)
#define BM_ASYEP_RPTR           (0xFF)

/* Offset: D28h Async Packet Queue Ready status register */
#define ASYEP_PKT_RDY_STS       (1 << 0)

/* Offset: D2Ch ASYNC Endpoint Interrupt Status Register */
#define BS_ASYEP_PKT_CRPTR      (0)
#define BM_ASYEP_PKT_CRPTR      (0xFF)

#define BS_ASYEP_PKT_ERR_TYPE   (8)
#define BM_ASYEP_PKT_ERR_TYPE   (0xF)

#define ASYEP_PKTERR_STS        (1 << 12)
#define ASYEP_PKTCMPL_STS       (1 << 13)

#define ASYEP_STS_MASK          (ASYEP_PKTERR_STS | ASYEP_PKTCMPL_STS)

/* Offset: D2Eh Async endpoint RX DP Length register */
#define BS_ASYEP_PKT_DPLEN      (0)
#define BM_ASYEP_PKT_DPLEN      (0x7FF)

/* Offset: D31- D30h NRDY_ENTRY0 Register */
#define NUM_NRDY_ENTRY          (8)
#define LAST_NRDY_ENTRY         (NUM_NRDY_ENTRY - 1)
#define NRDY_ENTRY_VALID_0      (0x80)

#define BS_NRDY_SEQNUM          (0)
#define BM_NRDY_SEQNUM          (0x1F)

#define BS_NRDY_TYPE            (5)
#define BM_NRDY_TYPE            (0x3)

#define BS_NRDY_EPNUM           (7)
#define BM_NRDY_EPNUM           (0xF)

/* Offset: D41h -D40h XHCI Debug Capability Peridic Endpoint INTE Register Register */
#define PEREP_INTE              (1 << 0)
#define TICKTM_INTE             (1 << 7)
#define INTEP_EN                (1 << 8)
#define ISOEP_EN                (1 << 9)

/* Offset: D6F-D6Ch Tick-Timer interrupt Status Register */
#define TICKTMINTSTS            (1 << 0)

/* Offset: D73-D70h Periodic Endpoint Interrupt Status fifo0 register */
#define ACTIVE0                 (1 << 0)

/* Offset: D93-D90h INT Packet Queue Ready Status Register */
#define INTEP_PKT_RDY_STS_EID0  (1 << 0)
#define INTEP_PKT_RDY_STS_EID1  (1 << 1)
#define INTEP_PKT_RDY_STS_EID2  (1 << 2)
#define INTEP_PKT_RDY_STS_EID3  (1 << 3)

/* Offset: DC3-DC0h ISOEP_MAPPING_ENTRY0 Register */
#define ISOEP_MAPPING_ENTRY_V   (1 << 0)
#define ISOEP_MAPPING_ENTRY_IO  (1 << 11)
#define ISOEP_MAPPING_ENTRY_STS (1 << 16)

/* Offset: E03-E00h U3D Port Status and Control Register */
#define U3D_CCS                 (1 << 0)
#define U3D_PED                 (1 << 1)
#define U3D_PR                  (1 << 4)
#define U3D_CSC                 (1 << 17)
#define U3D_PRC                 (1 << 21)
#define U3D_PLC                 (1 << 22)
#define U3D_CEC                 (1 << 23)

#define U3D_CHANGE_MASK         (U3D_CSC | U3D_PRC | U3D_PLC | U3D_CEC)

#define BS_U3D_PLS              (5)
#define BM_U3D_PLS              (0xF)

#define BS_U3D_PORTSPEED        (10)
#define BM_U3D_PORTSPEED        (0xF)

/* Offset: E04h U3D PORTSC related option register */
#define R_FWDISLINK_EN          (1 << 2)

/* Offset: E10h U3LITE Interrupt pending register */
#define DBC_INTSTS              (1 << 0)
#define ASYNC_INTSTS            (1 << 1)
#define PER_INTSTS              (1 << 2)
#define CTLEP_RXSETUP_INTSTS    (1 << 3)
#define NRDY_INTSTS             (1 << 4)
#define TICKTM_INTSTS           (1 << 5)
#define PSCEG_INTSTS            (1 << 6)
#define DB_INTSTS               (1 << 7)
#define BLKIN_HIT_INTSTS        (1 << 8)
#define BLKOUT_HIT_INTSTS       (1 << 9)
#define DBC_PORTRST_INTSTS      (1 << 10)
#define DBC_DCECHG_INTSTS       (1 << 11)
#define DBC_CCSCHG_INTSTS       (1 << 12)

#define DBC_INTSTS_MASK         (DB_INTSTS | BLKIN_HIT_INTSTS | BLKOUT_HIT_INTSTS | DBC_PORTRST_INTSTS | DBC_DCECHG_INTSTS | DBC_CCSCHG_INTSTS)

/* global register definition */

/* Offset: F01-F00h U3Lite OTG Enable Register */
#define U3H_EN                  (1 << 0)
#define U3DC_EN                 (1 << 1)

/* Offset: F02h USB3.0 Port 1 Direction Setting */
#define SS1_PORT_SEL_SW         (1 << 7)

/* SSPort Mode Select. */
#define SSPMS_U3H               (0x01)
#define SSPMS_U3D               (0x02)
#define SSPMS_DBC               (0x04)

/* SSPort Mode Select Status. */
#define SSPMSS_U3H              (0x01)
#define SSPMSS_U3D              (0x02)
#define SSPMSS_DBC              (0x04)

#define BS_SS1_PORT_MODSEL      (0)
#define BM_SS1_PORT_MODSEL      (0x7)

#define BS_SS1_PORT_MODSTS      (4)
#define BM_SS1_PORT_MODSTS      (0x7)

/* Offset: F03h USB3.0 Port 2 Direction Setting */
#define BS_SS2_PORT_MODSEL      (0)
#define BM_SS2_PORT_MODSEL      (0x7)

#define BS_SS2_PORT_MODSTS      (4)
#define BM_SS2_PORT_MODSTS      (0x7)

/* Offset Address: F04h USB3.0 Port Ownership */
#define DBC_U3DC_OWN            (1 << 0)

/* Offset Address: F05h USB3.0 RSP Control */
#define RSP_REQ                 (1 << 0)
#define LGO_INACTIVE            (1 << 1)

struct u3lite_hc_register {

        /* 0x000 - 0x319 */
        __u8 u3h_res1[0x320];

        /* 0x320 */
        __u32 dcctrl;

        /* 0x324 - 0x76B */
        __u8 u3h_res2[0x448];

        /* 0x76C */
        __u16 port1_phyls;

        /* 0x76E - 0x777 */
        __u8 u3h_res3[0x0A];
        
        /* 0x778 */
        __u16 port1_pmu;

        /* 0x77A - 0x77B */
        __u8 u3h_res4[0x02];

        /* 0x77C */
        __u16 port1_link;

        /* 0x77E - 0x785 */
        __u8 u3h_res5[0x08];
        
        /* 0x786 */
        __u16 port1_phy;

        /* 0x787 - 0x78B */
        __u8 u3h_res6[0x04];
        
        /* 0x78C */
        __u32 port1_feature;   

        /* 0x790 - 0xCFF */
        __u8 u3h_res7[0x570]; 
};

struct u3lite_gbl_register {

        /* 0xF00 */
        __u16 otg_en;

        /* 0xF02 */
        __u8 port1_dir;

        /* 0xF03 */
        __u8 port2_dir;

        /* 0xF04 */
        __u8 port_owner; 

        /* 0xF05 */
        __u8 rsp_ctrl;
};

struct u3lite_dc_register {
        /* 0xD00 */
        __u8 udc_inten;

        /* 0xD01 */
        __u8 ep0_inten;

        /* 0xD02 */
        __u16 udc_op;

        /* 0xD04 */
        __u8 udc_sts;

        /* 0xD05 */
        __u8 ep0_sts;

        /* 0xD06 */
        __u8 u3d_res1[2];

        /* 0xD08 */
        union {
                __u8 ctrl_pkt_buf[8];
                struct usb_device_request ctrl_pkt;
        };

        /* 0xD10 */
        __u8 db_inten;

        /* 0xD11 */
        __u8 idle_sts;

        /* 0xD12 */
        __u8 fwdislink;

        /* 0xD13 */
        __u8 reserved5;

        /* 0xD14 */
        __u8 db_sts;

        /* 0xD15 */
        __u8 u3d_res2[3];

        /* 0xD18 */
        __u8 timeout_en;

        /* 0xD19 */
        __u8 timeout_parm;

        /* 0xD1A */
        __u8 u3d_res3[2];

        /* 0xD1C */
        __u8 u1_timeout;

        /* 0xD1D */
        __u8 u2_timeout;
        
        /* 0xD1E */
        __u8 u3d_res4[2];                
        
        /* 0xD20 */
        __u16 async_inten;

        /* 0xD22 */
        union {
                __u16 async_pkt_read_ptr;

                struct {
                        __u8 u3d_res5;
                        __u8 async_rptr;
                };
        };

        /* 0xD24 */
        __u32 async_pkt_base_ptr;

        /* 0xD28 */
        __u8 async_pkt_sts;

        /* 0xD29 */
        __u8 u3d_res6[3];

        /* 0xD2C */
        union {
                __u16 async_intsts;

                struct {
                        __u8 async_crptr;
                        __u8 async_sts;
                };
        };

        /* 0xD2E */
        __u16 async_rx_dp_length;

        /* 0xD30 - 0xD3F */
        __u16 nrdy_entry[NUM_NRDY_ENTRY];

        /* 0xD40 */
        __u16 period_inten;

        /* 0xD42 */
        __u8 u3d_res7[14];

        /* 0xD50 */
        __u32 intep_pkt_rptr;

        /* 0xD54 */
        __u8 u3d_res8[12];

        /* 0xD60 */
        __u32 isoep_pkt_base;

        /* 0xD64 */
        __u16 tick_time_ctrl;

        /* 0xD66 */
        __u8 u3d_res9[2];

        /* 0xD68 */
        __u32 intep_pkt_base;

        /* 0xD6C */
        __u32 tt_sts;

        /* 0xD70 */
        __u32 period_intsts_fifo0;

        /* 0xD74 */
        __u32 period_intsts_fifo1;

        /* 0xD78 */
        __u32 period_intsts_fifo2;

        /* 0xD7C */
        __u32 period_intsts_fifo3;

        /* 0xD80 */
        __u32 period_intsts_fifo4;

        /* 0xD84 */
        __u32 period_intsts_fifo5;

        /* 0xD88 */
        __u32 period_intsts_fifo6;

        /* 0xD8C */
        __u32 period_intsts_fifo7;

        /* 0xD90 */
        __u32 int_pkt_sts;

        /* 0xD94 */
        __u8 u3d_res10[0x2C];

        /* 0xDC0 */
        __u32 iso_map_entry0;

        /* 0xDC4 */
        __u32 iso_map_entry1;

        /* 0xDC8 */
        __u32 iso_map_entry2;

        /* 0xDCC */
        __u32 iso_map_entry3;

        /* 0xDD0 */
        __u32 bus_interval_adj;

        /* 0xDD4 */
        __u32 iso_timestamp;

        /* 0xDD8 */
        union {
                __u8 u3d_res11[4];
                __u32 ap_addr;
        };

        /* 0xDDC */
        __u8 u3d_res12[0x24];

        /* 0xE00 */
        union {
                __u32 portsc;

                struct {
                        __u16 port_status;
                        __u16 port_change;
                };
        };

        /* 0xE04 */
        __u32 portsc_option;

        /* 0xE08 */
        __u8 nrdy_valid;

        /* 0xE09 */
        __u8 u3d_res13[3];

        /* 0xE0C */
        __u8 fifo_active;

        /* 0xE0D */
        __u8 u3d_res14[3];

        /* 0xE10 */
        __u32 udc_intsts; 

        /* 0xE14 - 0xEFF */
        __u8 u3d_res15[0xEC];      
};

struct u3lite_register {

        /* uhc register set below */    
        union {
                struct u3lite_hc_register uhc;

                struct {

                        /* 0x000 - 0x319 */
                        __u8 u3h_res1[0x320];

                        /* 0x320 */
                        __u32 dcctrl;

                        /* 0x324 - 0x76B */
                        __u8 u3h_res2[0x448];

                        /* 0x76C */
                        __u16 port1_phyls;

                        /* 0x76E - 0x777 */
                        __u8 u3h_res3[0x0A];
                        
                        /* 0x778 */
                        __u16 port1_pmu;

                        /* 0x77A - 0x77B */
                        __u8 u3h_res4[0x02];

                        /* 0x77C */
                        __u16 port1_link;

                        /* 0x77E - 0x785 */
                        __u8 u3h_res5[0x08];
                        
                        /* 0x786 */
                        __u16 port1_phy;

                        /* 0x787 - 0x78B */
                        __u8 u3h_res6[0x04];                                               

                        /* 0x78C */
                        __u32 port1_feature;   

                        /* 0x790 - 0xCFF */
                        __u8 u3h_res7[0x570]; 
                };
        };                        
        
        /* udc register set below */  
        union {
                struct u3lite_dc_register udc;

                struct {
                        /* 0xD00 */
                        __u8 udc_inten;

                        /* 0xD01 */
                        __u8 ep0_inten;

                        /* 0xD02 */
                        __u16 udc_op;

                        /* 0xD04 */
                        __u8 udc_sts;

                        /* 0xD05 */
                        __u8 ep0_sts;

                        /* 0xD06 */
                        __u8 u3d_res1[2];

                        /* 0xD08 */
                        union {
                                __u8 ctrl_pkt_buf[8];
                                struct usb_device_request ctrl_pkt;
                        };

                        /* 0xD10 */
                        __u8 db_inten;

                        /* 0xD11 */
                        __u8 idle_sts;

                        /* 0xD12 */
                        __u8 fwdislink;

                        /* 0xD13 */
                        __u8 reserved5;

                        /* 0xD14 */
                        __u8 db_sts;

                        /* 0xD15 */
                        __u8 u3d_res2[3];

                        /* 0xD18 */
                        __u8 timeout_en;

                        /* 0xD19 */
                        __u8 timeout_parm;

                        /* 0xD1A */
                        __u8 u3d_res3[2];

                        /* 0xD1C */
                        __u8 u1_timeout;

                        /* 0xD1D */
                        __u8 u2_timeout;
                        
                        /* 0xD1E */
                        __u8 u3d_res4[2];                
                        
                        /* 0xD20 */
                        __u16 async_inten;

                        /* 0xD22 */
                        union {
                                __u16 async_pkt_read_ptr;

                                struct {
                                        __u8 u3d_res5;
                                        __u8 async_rptr;
                                };
                        };

                        /* 0xD24 */
                        __u32 async_pkt_base_ptr;

                        /* 0xD28 */
                        __u8 async_pkt_sts;

                        /* 0xD29 */
                        __u8 u3d_res6[3];

                        /* 0xD2C */
                        union {
                                __u16 async_intsts;

                                struct {
                                        __u8 async_crptr;
                                        __u8 async_sts;
                                };
                        };

                        /* 0xD2E */
                        __u16 async_rx_dp_length;

                        /* 0xD30 - 0xD3F */
                        __u16 nrdy_entry[NUM_NRDY_ENTRY];

                        /* 0xD40 */
                        __u16 period_inten;

                        /* 0xD42 */
                        __u8 u3d_res7[14];

                        /* 0xD50 */
                        __u32 intep_pkt_rptr;

                        /* 0xD54 */
                        __u8 u3d_res8[12];

                        /* 0xD60 */
                        __u32 isoep_pkt_base;

                        /* 0xD64 */
                        __u16 tick_time_ctrl;

                        /* 0xD66 */
                        __u8 u3d_res9[2];

                        /* 0xD68 */
                        __u32 intep_pkt_base;

                        /* 0xD6C */
                        __u32 tt_sts;

                        /* 0xD70 */
                        __u32 period_intsts_fifo0;

                        /* 0xD74 */
                        __u32 period_intsts_fifo1;

                        /* 0xD78 */
                        __u32 period_intsts_fifo2;

                        /* 0xD7C */
                        __u32 period_intsts_fifo3;

                        /* 0xD80 */
                        __u32 period_intsts_fifo4;

                        /* 0xD84 */
                        __u32 period_intsts_fifo5;

                        /* 0xD88 */
                        __u32 period_intsts_fifo6;

                        /* 0xD8C */
                        __u32 period_intsts_fifo7;

                        /* 0xD90 */
                        __u32 int_pkt_sts;

                        /* 0xD94 */
                        __u8 u3d_res10[0x2C];

                        /* 0xDC0 */
                        __u32 iso_map_entry0;

                        /* 0xDC4 */
                        __u32 iso_map_entry1;

                        /* 0xDC8 */
                        __u32 iso_map_entry2;

                        /* 0xDCC */
                        __u32 iso_map_entry3;

                        /* 0xDD0 */
                        __u32 bus_interval_adj;

                        /* 0xDD4 */
                        __u32 iso_timestamp;

                        /* 0xDD8 */
                        union {
                                __u8 u3d_res11[4];
                                __u32 ap_addr;
                        };

                        /* 0xDDC */
                        __u8 u3d_res12[0x24];

                        /* 0xE00 */
                        union {
                                __u32 portsc;

                                struct {
                                        __u16 port_status;
                                        __u16 port_change;
                                };
                        };

                        /* 0xE04 */
                        __u32 portsc_option;

                        /* 0xE08 */
                        __u8 nrdy_valid;

                        /* 0xE09 */
                        __u8 u3d_res13[3];

                        /* 0xE0C */
                        __u8 fifo_active;

                        /* 0xE0D */
                        __u8 u3d_res14[3];

                        /* 0xE10 */
                        __u32 udc_intsts; 

                        /* 0xE14 - 0xEFF */
                        __u8 u3d_res15[0xEC];      
                };
        };

        /* uhc register set below */    
        union {
                struct u3lite_gbl_register gbl;

                struct {

                        /* 0xF00 */
                        __u16 otg_en;

                        /* 0xF02 */
                        __u8 port1_dir;

                        /* 0xF03 */
                        __u8 port2_dir;

                        /* 0xF04 */
                        __u8 port_owner; 

                        /* 0xF05 */
                        __u8 rsp_ctrl;
                };
        };
};

/* restore alignment to default */
#pragma pack() 

/* upkt marcos */
static inline void upkt_build_acktp(
        struct u3lite_packet *upkt,
        u8 devaddr,
        u8 cycle,
        u8 dir,
        u8 eptnum,
        u8 nump,
        u8 seqnum,
        u16 xfer_length,
        u8 ctrl_status,
        u8 ctrl_ep,
        u8 chain,
        u8 ioc,        
        u32 xfer_addr
        )
{
        trap(devaddr <= BM_TP_DEVADDR);
        trap(cycle <= BM_TP_CYCLE);
        trap(dir <= BM_TP_D);
        trap(eptnum <= BM_TP_EPTNUM);
        trap(nump <= BM_TP_NUMP);
        trap(seqnum <= BM_TP_SEQNUM);
        trap(xfer_length <= BM_TP_XFER_LENGTH);
        trap(ctrl_status <= BM_TP_CTRL_STATUS);
        trap(ctrl_ep <= BM_TP_CTRL_EP);
        trap(chain <= BM_TP_CHAIN);
        trap(ioc <= BM_TP_IOC);
        trap(xfer_addr <= BM_DP_XFER_ADDR);
        
        upkt->dword[0] = (
                (upkt_type_tp << BS_TP_TYPE) |
                (devaddr << BS_TP_DEVADDR) |
                (cycle << BS_TP_CYCLE)
                );

        upkt->dword[1] = (
                (upkt_sttp_ack << BS_TP_SUBTYPE) |
                (dir << BS_TP_D) |
                (eptnum << BS_TP_EPTNUM) |
                (nump << BS_TP_NUMP) |
                (seqnum << BS_TP_SEQNUM)
                );

        upkt->dword[2] = (
                (xfer_length << BS_TP_XFER_LENGTH) |
                (ctrl_status << BS_TP_CTRL_STATUS) |
                (ctrl_ep << BS_TP_CTRL_EP) |
                (chain << BS_TP_CHAIN) |
                (ioc << BS_TP_IOC)
                );

        upkt->dword[3] = xfer_addr << BS_DP_XFER_ADDR;
}

static inline void upkt_build_acktp_bulk_nonlast(
        struct u3lite_packet *upkt,
        u8 devaddr,
        u8 cycle,
        u8 eptnum,
        u8 seqnum,      
        u32 xfer_addr
        )
{
        trap(devaddr <= BM_TP_DEVADDR);
        trap(cycle <= BM_TP_CYCLE);
        trap(eptnum <= BM_TP_EPTNUM);
        trap(seqnum <= BM_TP_SEQNUM); 
        trap(xfer_addr <= BM_DP_XFER_ADDR);
        
        upkt->dword[0] = (
                (upkt_type_tp << BS_TP_TYPE) |
                (devaddr << BS_TP_DEVADDR) |
                (cycle << BS_TP_CYCLE)
                );

        upkt->dword[1] = (
                (upkt_sttp_ack << BS_TP_SUBTYPE) |
                (eptnum << BS_TP_EPTNUM) |
                (seqnum << BS_TP_SEQNUM)
                );

        upkt->dword[2] = (upkt_mds_bulk << BS_TP_XFER_LENGTH);
        upkt->dword[3] = xfer_addr;
}

static inline void upkt_build_acktp_bulk_last(
        struct u3lite_packet *upkt,
        u8 devaddr,
        u8 cycle,
        u8 eptnum,
        u8 seqnum,
        u16 xfer_length,
        u32 xfer_addr
        )
{
        trap(devaddr <= BM_TP_DEVADDR);
        trap(cycle <= BM_TP_CYCLE);
        trap(eptnum <= BM_TP_EPTNUM);
        trap(seqnum <= BM_TP_SEQNUM);
        trap(xfer_length <= BM_TP_XFER_LENGTH);
        trap(xfer_addr <= BM_DP_XFER_ADDR);
        
        upkt->dword[0] = (
                (upkt_type_tp << BS_TP_TYPE) |
                (devaddr << BS_TP_DEVADDR) |
                (cycle << BS_TP_CYCLE)
                );

        upkt->dword[1] = (
                (upkt_sttp_ack << BS_TP_SUBTYPE) |
                (eptnum << BS_TP_EPTNUM) |
                (1 << BS_TP_NUMP) |
                (seqnum << BS_TP_SEQNUM)
                );

        upkt->dword[2] = (
                (xfer_length << BS_TP_XFER_LENGTH) |                
                (1 << BS_TP_IOC)
                );

        upkt->dword[3] = xfer_addr;
}

static inline void upkt_build_dp(
        struct u3lite_packet *upkt,
        u8 devaddr,
        u8 cycle,
        u8 seqnum,
        u8 eob,
        u8 dir,
        u8 eptnum,
        u16 length,
        u8 ctrlep,
        u8 chain,
        u8 ioc,        
        u32 buffer
        )
{
        upkt->dword[0] = (
                (upkt_type_dph << BS_DP_TYPE) |
                (devaddr << BS_DP_DEVADDR) |
                (cycle << BS_DP_CYCLE)
                );

        upkt->dword[1] = (
                (seqnum << BS_DP_SEQNUM) |
                (eob << BS_DP_EOB) |
                (dir << BS_DP_D) |
                (eptnum << BS_DP_EPTNUM) |
                (length << BS_DP_DATA_LENGTH)
                );

        upkt->dword[2] = (
                (ctrlep << BS_DP_CTRLEP) |
                (chain << BS_DP_CHAIN) |
                (ioc << BS_DP_IOC)
                );

        upkt->dword[3] = buffer;
}

static inline void upkt_build_dp_bulk_nonlast(
        struct u3lite_packet *upkt,
        u8 devaddr,
        u8 cycle,
        u8 seqnum,
        u8 eptnum,    
        u32 buffer
        )          
{
        upkt->dword[0] = (
                (upkt_type_dph << BS_DP_TYPE) |
                (devaddr << BS_DP_DEVADDR) |
                (cycle << BS_DP_CYCLE)
                );

        upkt->dword[1] = (
                (seqnum << BS_DP_SEQNUM) |
                (1 << BS_DP_EOB) |
                (upkt_dir_d2h << BS_DP_D) |
                (eptnum << BS_DP_EPTNUM) |
                (upkt_mds_bulk << BS_DP_DATA_LENGTH)
                );

        upkt->dword[2] = 0;
        upkt->dword[3] = buffer;
}

static inline void upkt_build_dp_bulk_last(
        struct u3lite_packet *upkt,
        u8 devaddr,
        u8 cycle,
        u8 seqnum,
        u8 eptnum,
        u16 length,
        u32 buffer
        )
{
        upkt->dword[0] = (
                (upkt_type_dph << BS_DP_TYPE) |
                (devaddr << BS_DP_DEVADDR) |
                (cycle << BS_DP_CYCLE)
                );

        upkt->dword[1] = (
                (seqnum << BS_DP_SEQNUM) |
                (upkt_dir_d2h << BS_DP_D) |
                (eptnum << BS_DP_EPTNUM) |
                (length << BS_DP_DATA_LENGTH)
                );

        upkt->dword[2] = (1 << BS_DP_IOC);
        upkt->dword[3] = buffer;
}

static inline void upkt_build_dntp(
        struct u3lite_packet *upkt,
        u8 devaddr,
        u8 cycle,      
        u8 notify_type,
        u32 notify_type_spec1,
        u32 notify_type_spec2,
        u8 ioc
        )
{
        trap(devaddr <= BM_TP_DEVADDR);
        trap(cycle <= BM_TP_CYCLE);
        trap(notify_type <= BM_TP_NOTIFY_TYPE);
        trap(notify_type_spec1 <= BM_TP_NOTIFY_TYPE_SPEC1);
        
        upkt->dword[0] = (
                (upkt_type_tp << BS_TP_TYPE) |
                (devaddr << BS_TP_DEVADDR) |
                (cycle << BS_TP_CYCLE)
                );

        upkt->dword[1] = (
                (upkt_sttp_dev_notify << BS_TP_SUBTYPE) |
                (notify_type << BS_TP_NOTIFY_TYPE) |
                (notify_type_spec1 << BS_TP_NOTIFY_TYPE_SPEC1)
                );

        upkt->dword[2] = notify_type_spec2 | (ioc << BS_TP_IOC);
        upkt->dword[3] = 0;
}

static inline void upkt_build_erdy(
        struct u3lite_packet *upkt,
        u8 devaddr,
        u8 cycle,      
        u8 dir,
        u8 eptnum    
        )
{
        trap(devaddr <= BM_TP_DEVADDR);
        trap(cycle <= BM_TP_CYCLE);
        trap(dir <= BM_TP_D);
        trap(eptnum <= BM_TP_EPTNUM);
        
        upkt->dword[0] = (
                (upkt_type_tp << BS_TP_TYPE) |
                (devaddr << BS_TP_DEVADDR) |
                (cycle << BS_TP_CYCLE)
                );

        upkt->dword[1] = (
                (upkt_sttp_erdy << BS_TP_SUBTYPE) |
                (dir << BS_TP_D) |
                (eptnum << BS_TP_EPTNUM) |
                (1 << BS_TP_NUMP)
                );

        upkt->dword[2] = 0;
        upkt->dword[3] = 0;
}

static inline void upkt_build_stalltp(
        struct u3lite_packet *upkt,
        u8 devaddr,
        u8 cycle,
        u8 dir,
        u8 eptnum,
        u8 ctrl_status,
        u8 ctrl_ep,
        u8 ioc
        )
{
        trap(devaddr <= BM_TP_DEVADDR);
        trap(cycle <= BM_TP_CYCLE);
        trap(dir <= BM_TP_D);
        trap(eptnum <= BM_TP_EPTNUM);
        trap(ctrl_status <= BM_TP_CTRL_STATUS);
        trap(ctrl_ep <= BM_TP_CTRL_EP);
        trap(ioc <= BM_TP_IOC);
        
        upkt->dword[0] = (
                (upkt_type_tp << BS_TP_TYPE) |
                (devaddr << BS_TP_DEVADDR) |
                (cycle << BS_TP_CYCLE)
                );

        upkt->dword[1] = (
                (upkt_sttp_stall << BS_TP_SUBTYPE) |
                (dir << BS_TP_D) |
                (eptnum << BS_TP_EPTNUM)
                );

        upkt->dword[2] = (
                (ctrl_status << BS_TP_CTRL_STATUS) |
                (ctrl_ep << BS_TP_CTRL_EP) |
                (ioc << BS_TP_IOC)
                );

        upkt->dword[3] = 0;
}

#endif
