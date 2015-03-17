/*
 * via u3lite udc - udc header
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

#ifndef _U3DC_UDC_H_
#define _U3DC_UDC_H_

#include "vu3d_precomp.h"
#include "vu3d_ep.h"

#define SIGNATURE_VUDC          (cpu_to_be32((uintptr_t)"vudc"))

#define BYTE_VUDC_NAME          (16)                 
                
#define U3LITE_MAX_SAMPLING     (10)

#if VU3L_ON_ELITE_PCIE

#define VU3L_PCI_VENDOR_ID      (0x1106)
#define VU3L_PCI_DEVICE_ID      (0x3483)

#define VU3L_PCI_BDF            (PCI_BDF(1,0,0))

#endif

enum udc_usb_state {
        uus_invalid             = 0x00,
        uus_powered             = 0x01,
        uus_attached            = 0x02,
        uus_default             = 0x03,
        uus_addressed           = 0x04,
        uus_configured          = 0x05,
        uus_sus_default         = 0x06,
        uus_sus_addressed       = 0x07,
        uus_sus_configured      = 0x08
};

enum udc_usb_event {
        uue_invalid             = 0,
        uue_portsc,                 
        uue_address,
        uue_configure,
        uue_deconfigure,
        uue_hw_failed,
        uue_udc_stop
};
        
struct via_udc {

        u32 signature;
        
        char name[BYTE_VUDC_NAME];

        /* */
        struct usb_device_instance *usbd;       

        /* */
        u8 usb_address;

        /* */
        u32 portsc;
        u32 prior_portsc;
        
        /* usb device state management */
        enum udc_usb_state usb_state;
        enum udc_usb_state prior_usb_state; 
        
        /* io resource */                    
        struct u3lite_dc_register *udc_reg;                       
        struct u3lite_hc_register *uhc_reg;
        struct u3lite_gbl_register *gbl_reg; 

        /* */
        struct via_ep *vep0;
        struct via_ep *vepbi;
        struct via_ep *vepbo;
        struct via_ep vep[NUM_VEP];   

        /* */
        struct urb *ep0_urb;
        
        /* async. io */        
        struct via_ep* async_vep;
        struct u3lite_packet *flight_upkt_base;    
        struct u3lite_packet *flight_upkt_next;
        
        u8 async_pcs;
        struct u3lite_packet *async_upktq_base;         
        struct u3lite_packet *async_upktq_last;                
        struct u3lite_packet *async_upktq_current;
        unsigned long async_upktq_base_da;  
        struct u3lite_packet *async_upktq_orignal;
        unsigned long async_upktq_orignal_da;                  
        
        /* */
        unsigned is_pullup_on           : 1;
        unsigned is_hw_initialized      : 1;
        unsigned is_hw_tx_started       : 1;
        unsigned is_hw_async_started    : 1;
        unsigned is_u1_enabled          : 1;
        unsigned is_u2_enabled          : 1;
        unsigned is_inited              : 1;
};

int vudc_async_restart_hw(
        struct via_udc *vudc
        );      

int vudc_handle_usb_event(
        struct via_udc *vudc,
        enum udc_usb_event uue,
        u32 uue_para
        );

void vudc_start_async(
        struct via_udc *vudc, 
        struct via_ep *vep
        );

/* 
 * export api for fastboot below.
 */
int u3d_endpoint_write(
        struct usb_endpoint_instance *endpoint
        );

void u3d_setup_ep(
        struct usb_device_instance *device, 
        unsigned int ep, 
        struct usb_endpoint_instance *endpoint
        );

void u3d_irq(
    void
    );

void u3d_disconnect(    
        void
        );

void u3d_connect(  
        void
        );

void u3d_disable(    
        void
        );

void u3d_enable(
        struct usb_device_instance *device
        );

int u3d_init(
        void
        );

/*
 * inline function below
 */
static inline void vudc_mark_analysis_point(
    struct via_udc *vudc,
    u32 analysis_point
    )
{
    vu3l_write32(&vudc->udc_reg->ap_addr, analysis_point);     
}

static inline bool vudc_is_valid(
        struct via_udc *vudc
        )
{
        return (vudc && (vudc->signature == SIGNATURE_VUDC));         
}

static inline bool vudc_is_u1_enabled(
        struct via_udc *vudc
        )
{
        return vudc->is_u1_enabled ? true : false;
}

static inline bool vudc_is_u2_enabled(
        struct via_udc *vudc
        )
{
        return vudc->is_u2_enabled ? true : false;
}

static inline void vudc_u2_enable(
        struct via_udc *vudc,
        bool is_enable
        )
{
        if (is_enable) {
                dtrap(!vudc_is_u2_enabled(vudc));
                vudc->is_u2_enabled = 1;
        }
        else {
                vudc->is_u2_enabled = 0;
        }
}

static inline void vudc_u1_enable(
        struct via_udc *vudc,
        bool is_enable
        )
{
        if (is_enable) {
                dtrap(!vudc_is_u1_enabled(vudc));
                vudc->is_u1_enabled = 1;
        }
        else {
                vudc->is_u1_enabled = 0;
        }
}

static inline struct u3lite_dc_register * vudc_get_udc_register(
        struct via_udc *vudc
        )
{
        return vudc->udc_reg;
}

static inline enum udc_usb_state vudc_get_usb_state(
        struct via_udc *vudc
        )
{        
        return vudc->usb_state;
}

static inline bool vudc_is_usb_attached(
        struct via_udc *vudc
        )
{      
        return (vudc_get_usb_state(vudc) >= uus_attached);
}

static inline bool vudc_is_usb_enabled(
        struct via_udc *vudc
        )
{   
        return (vudc_get_usb_state(vudc) >= uus_default);
}

static inline bool vudc_is_equal_uus(
        struct via_udc *vudc,
        enum udc_usb_state uus
        )
{    
        return (vudc_get_usb_state(vudc) == uus);
}

static inline bool vudc_is_sus_uus(
        struct via_udc *vudc,
        enum udc_usb_state uus
        )
{    
        return (vudc_get_usb_state(vudc) >= uus_sus_default);
}

static inline struct usb_device_instance* vudc_get_usbd(
        struct via_udc *vudc
        )
{              
        return vudc->usbd;
}

static inline struct urb* vudc_get_ep0_urb(
        struct via_udc *vudc
        )
{              
        return vudc->ep0_urb;
}
        
static inline struct via_ep* vudc_find_nrdy_vep(
        struct via_udc *vudc,
        u16 nrdy
        )
{        
        u8 epnum;        

        epnum = MBF_TO_VAL(nrdy, BS_NRDY_EPNUM, BM_NRDY_EPNUM);      
        
        if (epnum == vep_get_epnum(vudc->vepbo)) {
                return vudc->vepbo;
        }
        else if (epnum == vep_get_epnum(vudc->vepbi)) {
                return vudc->vepbi;
        }
        else if (epnum == vep_get_epnum(vudc->vep0)) {                
                dtrap(false);
                return vudc->vep0;
        }
        else {
                dtrap(false);
                return NULL;
        }
}

static inline void vudc_assert_upkt_ready(
        struct via_udc *vudc
        )
{
        ap_trap(!vu3l_bit_test8(&vudc->udc_reg->async_pkt_sts, ASYEP_PKT_RDY_STS));
        vu3l_write8(&vudc->udc_reg->async_pkt_sts, 1);
}

static inline void vudc_advance_async_upktq(
        struct via_udc *vudc
        )
{
        if (vudc->async_upktq_current == vudc->async_upktq_last) {
                vudc->async_upktq_current = vudc->async_upktq_base;
                vudc->async_pcs = !vudc->async_pcs;
        }
        else {
                vudc->async_upktq_current++;
        }
}

static inline struct u3lite_packet* vudc_query_next_upkt(
        struct via_udc *vudc,
        struct u3lite_packet *upkt
        )
{
        return (upkt == vudc->async_upktq_last) ?
                vudc->async_upktq_base : (upkt + 1);
}

static inline bool vudc_is_upkt_in_range(
        struct via_udc *vudc,
        struct u3lite_packet *upkt_search,
        struct u3lite_packet *upkt_start,
        struct u3lite_packet *upkt_end        
        )
{                
        if (upkt_end == upkt_start) {
                dtrap(false);
                return false;
        }
        else if (upkt_end > upkt_start) {
                return ((upkt_search >= upkt_start) && (upkt_search < upkt_end));   
        }
        else {
                return (!((upkt_search >= upkt_end) && (upkt_search < upkt_start)));  
        }
}

static inline void vudc_insert_dp(
        struct via_udc *vudc,
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
        upkt_build_dp(
                vudc->async_upktq_current,      /* upkt */
                vudc->usb_address,              /* devaddr */
                vudc->async_pcs,                /* cycle */
                seqnum,                         /* seqnum */
                eob,                            /* eob */
                dir,                            /* dir */
                eptnum,                         /* eptnum */
                length,                         /* length */
                ctrlep,                         /* ctrlep */
                chain,                          /* chain */
                ioc,                            /* ioc */
                buffer                          /* buffer */
                );
        vudc_advance_async_upktq(vudc);
}

static inline void vudc_insert_dp_bulk_nonlast(
        struct via_udc *vudc,
        u8 seqnum,
        u8 eptnum,    
        u32 buffer
        )
{
        upkt_build_dp_bulk_nonlast(
                vudc->async_upktq_current, 
                vudc->usb_address,
                vudc->async_pcs, 
                seqnum, 
                eptnum, 
                buffer
                );
        vudc_advance_async_upktq(vudc);
}

static inline void vudc_insert_dp_bulk_last(
        struct via_udc *vudc,
        u8 seqnum,
        u8 eptnum,
        u16 length,     
        u32 buffer
        )
{
        upkt_build_dp_bulk_last(
                vudc->async_upktq_current, 
                vudc->usb_address,
                vudc->async_pcs, 
                seqnum, 
                eptnum, 
                length, 
                buffer
                );
        vudc_advance_async_upktq(vudc);
}

static inline void vudc_insert_acktp(
        struct via_udc *vudc,
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
        upkt_build_acktp(
                vudc->async_upktq_current, 
                vudc->usb_address,
                vudc->async_pcs, 
                dir, 
                eptnum, 
                nump, 
                seqnum, 
                xfer_length, 
                ctrl_status, 
                ctrl_ep, 
                chain, 
                ioc, 
                xfer_addr
                );
        
        vudc_advance_async_upktq(vudc);
}

static inline void vudc_insert_acktp_bulk_nonlast(
        struct via_udc *vudc,        
        u8 eptnum, 
        u8 seqnum, 
        u32 xfer_addr
        )
{
        upkt_build_acktp_bulk_nonlast(
                vudc->async_upktq_current, 
                vudc->usb_address,
                vudc->async_pcs, 
                eptnum, 
                seqnum, 
                xfer_addr
                );
        
        vudc_advance_async_upktq(vudc);
}

static inline void vudc_insert_acktp_bulk_last(
        struct via_udc *vudc,        
        u8 eptnum,
        u8 seqnum,
        u16 xfer_length,        
        u32 xfer_addr
        )
{
        upkt_build_acktp_bulk_last(
                vudc->async_upktq_current, 
                vudc->usb_address,
                vudc->async_pcs, 
                eptnum, 
                seqnum,
                xfer_length, 
                xfer_addr
                );
        
        vudc_advance_async_upktq(vudc);
}

static inline void vudc_insert_stalltp(
        struct via_udc *vudc,
        u8 dir,
        u8 eptnum,
        u8 ctrl_status,
        u8 ctrl_ep,
        u8 ioc
        )
{
        upkt_build_stalltp(
                vudc->async_upktq_current, 
                vudc->usb_address,
                vudc->async_pcs, 
                dir, 
                eptnum, 
                ctrl_status, 
                ctrl_ep, 
                ioc
                );
        
        vudc_advance_async_upktq(vudc);
}

static inline void vudc_insert_erdy(
        struct via_udc *vudc,
        u8 dir,
        u8 epnum       
        )
{
        upkt_build_erdy(
                vudc->async_upktq_current, 
                vudc->usb_address,
                vudc->async_pcs,        
                dir, 
                epnum
                );        
        vudc_advance_async_upktq(vudc);
}

static inline void vudc_insert_dntp(
        struct via_udc *vudc,
        u8 notify_type,
        u32 notify_type_spec1, 
        u32 notify_type_spec2,
        u8 ioc
        )
{
        upkt_build_dntp(
                vudc->async_upktq_current, 
                vudc->usb_address, 
                vudc->async_pcs, 
                notify_type, 
                notify_type_spec1, 
                notify_type_spec2,
                ioc
                );        
        vudc_advance_async_upktq(vudc);
}

static inline void vudc_complete_async(
        struct via_udc *vudc
        )
{
        /* DBG: dump async related info. */
        vudc_dump_async(vudc);

        trap(vudc->async_vep);        
        vudc->async_vep = NULL;
}

#endif  /* _U3DC_UDC_H_ */

