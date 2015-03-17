/*
 * via u3lite udc - endpoint function
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

#include "vu3d_lib.h"

static u8 usb_bos_descriptor[] = {
        0x05,                   /*  __u8 bLength, 5 bytes */
        0x0F,                   /*  __u8 bDescriptorType */
        0x0F, 0x00,             /*  __le16 wTotalLength, 15 bytes */
        0x1,                    /*  __u8 bNumDeviceCaps */
        
        /* First device capability */
        0x0A,                   /*  __u8 bLength, 10 bytes */
        0x10,                   /* Device Capability */
        0x03,                   /* bDevCapabilityType, SUPERSPEED_USB */
        0x00,                   /* bmAttributes, LTM off by default */
        0x08, 0x00,             /* wSpeedsSupported, 5Gbps only */
        0x03,                   /* bFunctionalitySupport, USB 3.0 speed only */
        0x0A,                   /* bU1DevExitLat, set to worst case */
        0xFF, 0x07              /* __le16 bU2DevExitLat, set to worst case */
};

void epbi_endpoint_write(
        struct via_ep *vep,
        u16 nrdy
        )
{             
        struct usb_endpoint_instance *usbe;
        struct urb *urb;        

        if (nrdy) { 

                /* save nrdy to build upkt later */
                trap(MBF_TO_VAL(nrdy, BS_NRDY_EPNUM, BM_NRDY_EPNUM) > 0);
                vep->nrdy = nrdy;
        
                /* has footboot ask us to do transfer ? No, wait fastboot. */
                if (!vep->is_udc_write) {
                        trap(!vep->is_nrdy_pending);
                        vep->is_nrdy_pending = 1;
                        
                        return; 
                }

                vep->is_udc_write = 0;
        }
        else {       
                /* has received host's request ? No, wait host. */
                if (!vep->is_nrdy_pending) {                         
                        trap(!vep->is_udc_write);
                        vep->is_udc_write = 1;
                        
                        return;
                }

                vep->is_nrdy_pending = 0;
        }
        trap((!vep->is_udc_write) && (!vep->is_nrdy_pending));
                  
        usbe = vep->usbe;        
        urb = usbe->tx_urb;
        trap(urb == vep->urb);
        
        if (!urb || !usbe) {
                vep_warn("invalid usbe %p urb %p\n", usbe, urb);                
                return;
        }

        if ((!urb->actual_length) || (urb->actual_length <= usbe->sent)) {
                vep_warn(
                        "invalid last, actual_length %X sent %X\n", 
                        urb->actual_length, usbe->sent
                        );  
                return;
        }
        
        vudc_start_async(vep->vudc, vep);
}


static void epbi_build_upkt(
        struct via_ep *vep
        )
{
        struct via_udc *vudc;
        struct urb *urb;
        u8 seqnum;
        uintptr_t buffer;
        
        vudc = vep_get_vudc(vep); 
        urb = vep->usbe->tx_urb;
        trap(vep->urb == urb);

        trap(vep->usbe->tx_packetSize == upkt_mds_bulk);
        buffer = (uintptr_t)urb->buffer + vep->usbe->sent;
        
        trap(urb->actual_length > vep->usbe->sent);
        vep->async_length = min((urb->actual_length - vep->usbe->sent), upkt_mds_bulk);
        
        vep_dbg(
                ">> urb %p buffer %X length %X\n", 
                urb, buffer, vep->async_length
                );
                      
        /* DP's seqnum = received ACKTP's seqnum */
        seqnum = MBF_TO_VAL(vep->nrdy, BS_NRDY_SEQNUM, BM_NRDY_SEQNUM); 
        trap(seqnum < BM_NRDY_SEQNUM);
        
        /* Bulk-In: U3DC HW uses internal 1K SRAM as buffer to
         * transfer/receive packet. Hence, it can handle one DP only. 
         * So driver return 
         * - ERDY(nump = 1) and DP(eob = 1) for non-last ACKTP.
         * - ERDY(nump = 1) and DP(eob = 0) for last ACKTP. 
         */                     
        vudc_insert_erdy(vudc, upkt_dir_d2h, vep->epnum);                               
        vudc_insert_dp_bulk_last(
                vudc, seqnum, vep->epnum, vep->async_length, buffer
                );       

        vep_dbg("<< urb %p\n", urb);
}

static void epbi_irq_async_xfer(
        struct via_ep *vep, 
        u8 errtype, 
        u16 dp_length, 
        u8 upkt_num
        )
{             
        if (errtype == upkt_err_no) {                
                vep->usbe->last = vep->async_length;
                usbd_tx_complete(vep->usbe);      
                
                return;
        }
        
        dtrap(errtype != upkt_err_sp);                
}

static void epbi_irq_nrdy(
        struct via_ep *vep,
        u16 nrdy
        )
{                   
        epbi_endpoint_write(vep, nrdy);             
}
        
static void epbo_build_upkt(
        struct via_ep *vep
        )
{
        struct via_udc *vudc;
        struct urb *urb;
        u16 seqnum;
        uintptr_t buffer;
        unsigned int reminder;
        u8 ipkt, npkt;
        
        vudc = vep_get_vudc(vep);                          
        urb = vep->usbe->rcv_urb;
        trap(vep->urb == urb);

        trap(vep->usbe->rcv_packetSize == upkt_mds_bulk);
        buffer = (uintptr_t)urb->buffer + urb->actual_length;

        trap(urb->buffer_length > urb->actual_length);
        vep->async_length = min(
                (urb->buffer_length - urb->actual_length), 
                ASYNC_MAX_URB_TX_BYTE
                );                
        reminder = (vep->async_length & (upkt_mds_bulk - 1));
        npkt = (vep->async_length >> upkt_mdss_bulk) + (reminder ? 1 : 0);        
        
        vep_dbg(
                ">> urb %p buffer %X length %X npkt %X nrdy %X\n", 
                urb, buffer, vep->async_length, npkt, vep->nrdy
                );       
        
        /* ACKTP's seqnum = received DP's seqnum + 1.
         * NOTE: seqnum range is 0 ~ 31, remerber to wrap 
         * back to 0 if seqnum = 32. 
         */
        seqnum = MBF_TO_VAL((vep->nrdy + 1), BS_NRDY_SEQNUM, BM_NRDY_SEQNUM); 
        
        /* Bulk-Out: U3DC HW uses internal 1K SRAM as buffer to
         * transfer/receive packet. Hence, it can handle one DP only. 
         * So driver return 
         * - ERDY(nump = 1) and ACKTP(nump = 0) for non-last DP.
         * - ERDY(nump = 1) and ACKTP(nump = 1) for last DP. 
         */ 
        for (ipkt = 0; ipkt < (npkt - 1); ipkt++) {
                vudc_insert_erdy(vudc, upkt_dir_h2d, vep->epnum);                               
                vudc_insert_acktp_bulk_nonlast(
                        vudc, 
                        vep->epnum, 
                        ((seqnum + ipkt) & BM_NRDY_SEQNUM),                                 
                        buffer + (ipkt << upkt_mdss_bulk)
                        );            
        }
        
        vudc_insert_erdy(vudc, upkt_dir_h2d, vep->epnum);                               
        vudc_insert_acktp_bulk_last(
                vudc, 
                vep->epnum, 
                ((seqnum + ipkt) & BM_NRDY_SEQNUM),                         
                (reminder ? reminder : upkt_mds_bulk),
                buffer + (ipkt << upkt_mdss_bulk)
                );        

        vep_dbg("<< urb %p\n", urb);
}

static void epbo_irq_async_xfer(
        struct via_ep *vep, 
        u8 errtype, 
        u16 dp_length, 
        u8 upkt_num
        )
{             
        unsigned int xfer_length, rcv_length;        
               
        if ((errtype != upkt_err_no) && (errtype != upkt_err_sp)) {             
                usbd_rcv_complete(vep->usbe, 0, 1); 
                return;
        }
        
        /* evaluate xfer_length */
        if (errtype == upkt_err_no) {                     
                xfer_length = vep->async_length;
        }
        else if (errtype == upkt_err_sp) {     

                trap(dp_length);
                
                xfer_length = dp_length;
                if (upkt_num) {
                        ap_trap(!(upkt_num & 0x01));
                        xfer_length += ((upkt_num >> 1) << upkt_mdss_bulk);  
                }                 
                trap(xfer_length < vep->async_length);   
        }

        /* notify fastboot how many data is received */
        trap(vep->usbe->rcv_packetSize == upkt_mds_bulk);  
        while (xfer_length) {
                
                rcv_length = min(xfer_length, upkt_mds_bulk);                        
                usbd_rcv_complete(vep->usbe, rcv_length, 0);  
                
                xfer_length -= rcv_length;
        } 
}

static void epbo_irq_nrdy(
        struct via_ep *vep,
        u16 nrdy
        )
{           
        /* save nrdy to build upkt later */
        vep->nrdy = nrdy;
        
        /* forward async to hw */
        vudc_start_async(vep->vudc, vep);      
}

static void ep0_build_upkt_dataout(
        struct via_ep *vep
        )
{
        struct via_udc *vudc;
        uintptr_t buffer;
        u16 reminder, transferred;
        u8 seqnum;

        trap(!vep->epnum);        
        vudc = vep_get_vudc(vep);        

        /* Data Stage - ACKTP */
        buffer = (uintptr_t)vep->urb->buffer;
        reminder = vep->urb->actual_length;        
        seqnum = upkt_ds_ep0_write;
            
        while (reminder) {

                transferred = min(reminder, (u16)upkt_mds_ctrl);
        
                 vudc_insert_acktp(
                        vudc,
                        upkt_dir_ignore,        /* dir */
                        0,                      /* eptnum */
                        1,                      /* nump */
                        seqnum,                 /* seqnum */
                        transferred,            /* xfer_length */
                        0,                      /* ctrl_status */
                        1,                      /* ctrl_ep */
                        0,                      /* chain */
                        0,                      /* ioc */
                        buffer                  /* xfer_addr */
                        );

                /* */
                seqnum++;
                reminder -= transferred;
                buffer += transferred;
        }

        /* Status Stage - ACKTP */
        vudc_insert_acktp(
                vudc,
                upkt_dir_ignore,        /* dir */
                0,                      /* eptnum */
                0,                      /* nump */
                upkt_ds_ep0_status,     /* seqnum */
                0,                      /* xfer_length */
                1,                      /* ctrl_status */
                1,                      /* ctrl_ep */
                0,                      /* chain */
                1,                      /* ioc */
                0                       /* xfer_addr */
                );
}

static void ep0_build_upkt_datain(
        struct via_ep *vep
        )
{
        struct via_udc *vudc;
        uintptr_t buffer;
        u16 reminder, transferred;
        u8 seqnum;

        trap(!vep->epnum);        
        vudc = vep_get_vudc(vep);        

        /*
         * HW will auto reply ACK with NumP 0 to host 
         * then host will wait until U3DC send ERDY to host for
         * asking re-start transaction.
         * SW only need to preapre data stage/status packets.
         * After packets ready, set "status ready" register to 
         * send packets.
         * U3DC set default cycle bit as to the cycle bit of 
         * first fetched upkt. And HW will auto toggle when wrap
         * upkt queue.
         * U3DC check cycle, sequental number, pkt type.         
         */
        
        /* Data Stage - DP */
        buffer = (uintptr_t)vep->urb->buffer;
        reminder = vep->urb->actual_length;        
        seqnum = upkt_ds_ep0_read;
           
        while (reminder) {

                transferred = min(reminder, (u16)upkt_mds_ctrl);
        
                vudc_insert_dp(
                        vudc,
                        seqnum,                 /* seqnum */
                        0,                      /* eob */
                        upkt_dir_ignore,        /* dir */
                        0,                      /* eptnum */
                        transferred,            /* length */
                        1,                      /* ctrlep */
                        0,                      /* chain */
                        0,                      /* ioc */
                        buffer                  /* buffer */
                        );

                /* */
                seqnum++;
                reminder -= transferred;
                buffer += transferred;
        }

        /* Status Stage - ACKTP */
        vudc_insert_acktp(
                vudc,
                upkt_dir_ignore,        /* dir */
                0,                      /* eptnum */
                0,                      /* nump */
                upkt_ds_ep0_status,     /* seqnum */
                0,                      /* xfer_length */
                1,                      /* ctrl_status */
                1,                      /* ctrl_ep */
                0,                      /* chain */
                1,                      /* ioc */
                0                       /* xfer_addr */
                );
}

static void ep0_build_upkt_nondata(
        struct via_ep *vep
        )
{
        struct via_udc *vudc;
        u8 devaddr;
        
        trap(!vep->epnum);
        
        vudc = vep_get_vudc(vep);               
        devaddr = (vep->urb_setup->bRequest == USB_REQ_SET_ADDRESS) ? 
                0 : vudc->usb_address;
        
        /* Status Stage - ACKTP */   
        upkt_build_acktp(
                vudc->async_upktq_current, 
                devaddr,
                vudc->async_pcs, 
                upkt_dir_ignore,        /* dir */
                0,                      /* eptnum */
                0,                      /* nump */
                upkt_ds_ep0_status,     /* seqnum */
                0,                      /* xfer_length */
                1,                      /* ctrl_status */
                1,                      /* ctrl_ep */
                0,                      /* chain */
                1,                      /* ioc */
                0                       /* xfer_addr */
                );
        
        vudc_advance_async_upktq(vudc);
}

static void ep0_build_upkt(
        struct via_ep *vep
        )
{         
        struct usb_device_request *setup;
                       
        setup = vep->urb_setup;

        vep_dbg(
                ">> urb %p buffer %p length %X\n", 
                vep->urb, vep->urb->buffer, vep->urb->actual_length
                );
        
        /* need to reply stall pkt to host ? */
        if (vep->is_stall) {   

                enum usb_packet_direction dir;

                if (setup->wLength) {
                        dir = (setup->bmRequestType & USB_DIR_IN) ? 
                                upkt_dir_d2h : 
                                upkt_dir_h2d;
                }
                else {
                        dir = upkt_dir_ignore;
                }
                        
                vep_warn(
                        "build stalltp %s, dir %.1X ctrl_sts %.1X\n",
                        vu3l_string_usb_req(setup->bRequest), dir, !setup->wLength                        
                        ); 

                vudc_insert_stalltp(
                        vep->vudc, 
                        dir,                    /* dir */
                        0,                      /* eptnum */
                        !setup->wLength,        /* ctrl_status */
                        1,                      /* ctrl_ep */
                        1                       /* ioc */
                        );

                vep->is_stall = 0;
                return;
        }
        
        /* start ep0 hw based on ep0 state */
        if (!setup->wLength)
                ep0_build_upkt_nondata(vep);
        else if (setup->bmRequestType & USB_DIR_IN)
                ep0_build_upkt_datain(vep);  
        else
                ep0_build_upkt_dataout(vep); 

        vep_dbg("<< urb %p\n", vep->urb);
}

static void ep0_stall_setup(
        struct via_ep *vep        
        )
{       
        struct usb_device_request *setup;

        setup = vep->urb_setup;
        
        vep_info(
                ">> %s bmRequestType %.2X bRequest %.2X "\
                "wValue %.4X wIndex %.4X wLength %.4X\n",
                vu3l_string_usb_req(setup->bRequest),
                setup->bmRequestType,
                setup->bRequest,
                le16_to_cpu(setup->wValue),
                le16_to_cpu(setup->wIndex),
                le16_to_cpu(setup->wLength)
                ); 
                
        dtrap(!vep->is_stall);
        vep->is_stall = 1;
        
        vudc_start_async(vep->vudc, vep);
}

static int ep0_forward_setup(
        struct via_ep *vep
        )
{       
        struct urb *urb;

        urb = vep->urb;
            
        if (ep0_recv_setup(urb)) {
                
                vep_warn("ep0_recv_setup failed, stall ep0.\n");
                
                ep0_stall_setup(vep);
                return -EPROTO;                        
        }
        
        vudc_start_async(vep->vudc, vep);
        
        return 0;
}

static int ep0_get_desc(
        struct via_ep *vep,
        struct usb_device_request *setup
        )
{
        struct urb *urb;
        u8 type;

        urb = vep->urb;
        type = le16_to_cpu(setup->wValue) >> 8;
        
        if (type != USB_DT_BOS)
                return 1;
        
        BUILD_BUG_ON(sizeof(usb_bos_descriptor) != 0x0F);
        urb->actual_length = min(
                le16_to_cpu(setup->wLength), sizeof(usb_bos_descriptor)
                );
        memcpy(urb->buffer, usb_bos_descriptor, urb->actual_length);
        
        vudc_start_async(vep->vudc, vep);
        
        return 0;
}  
        
static int ep0_set_isoch_delay(
        struct via_ep *vep,
        struct usb_device_request *setup
        )
{
        if (setup->wLength || setup->wIndex) {
                ep0_stall_setup(vep);
                return 0;
        }
        
        vudc_start_async(vep->vudc, vep);
        
        return 0;
}

static int ep0_set_sel(
        struct via_ep *vep,
        struct usb_device_request *setup
        )
{
        u16 wLength;

        if ((!vudc_is_equal_uus(vep->vudc, uus_addressed)) && 
                (!vudc_is_equal_uus(vep->vudc, uus_configured))) {
                
                vep_warn(
                        "invalid uus %X to do sel_sel\n",  
                        vudc_get_usb_state(vep->vudc)
                        );
                
                ep0_stall_setup(vep);
                return 0;
        }

        wLength = le16_to_cpu(setup->wLength);       
        if (wLength != sizeof(struct usb_set_sel_req)) {
                vep_warn("Set_Sel should be 6 bytes, got %d\n", wLength);                
                ep0_stall_setup(vep);
                return 0;
        }

        /* we need to receive 6 bytes from host in handling Set_SEL. */          
        vep->urb->actual_length = wLength;
        
        vudc_start_async(vep->vudc, vep);
        
        return 0;
}

static int ep0_set_configuration(
        struct via_ep *vep,
        struct usb_device_request *setup
        )
{
        enum udc_usb_event uue;

        uue = setup->wValue ? uue_configure : uue_deconfigure;        
        vudc_handle_usb_event(vep->vudc, uue, 0);

        return 1;
}

static int ep0_set_address(
        struct via_ep *vep,
        struct usb_device_request *setup
        )
{ 
        int status;

        /* inform usbcore a set_address is received(completed). */   
        status = ep0_recv_setup(vep->urb);
        if (!VU3L_SUCCESS(status)) {
                ep0_stall_setup(vep);
                return 0;
        }
        
        /* update usb state to address state */            
        status = vudc_handle_usb_event(
                vep->vudc, uue_address, le16_to_cpu(setup->wValue)
                );        
        if (!VU3L_SUCCESS(status)) {
                ep0_stall_setup(vep);
                return 0;
        }

        vudc_start_async(vep->vudc, vep); 
        
        return 0;
}

static int ep0_get_status(
        struct via_ep *vep,
        struct usb_device_request *setup
        )
{        
        u16 usb_status;
                
        if ((setup->bmRequestType & USB_RECIP_OTHER) != USB_RECIP_DEVICE) {
                return 1;           
        }

        /* build "status" for get status request */
        usb_status = 1 << USB_STATUS_SELFPOWERED;
                        
        if (vudc_is_u1_enabled(vep->vudc))
                usb_status |= 1 << USB_DEV_STAT_U1_ENABLED;
        if (vudc_is_u2_enabled(vep->vudc))
                usb_status |= 1 << USB_DEV_STAT_U2_ENABLED;        
                
        /* setup & foward urb */   
        vep->urb->actual_length = sizeof(usb_status);
        *((u16*)vep->urb->buffer) = cpu_to_le16(usb_status);  
        
        vudc_start_async(vep->vudc, vep);
        
        return 0;
}

static int ep0_set_feature(
        struct via_ep *vep,
        struct usb_device_request *setup,
        bool is_set
        )
{
        struct via_udc *vudc;
        u16 wValue;    

        vudc = vep_get_vudc(vep);
        wValue = le16_to_cpu(setup->wValue);
        
        if ((setup->bmRequestType & USB_RECIP_OTHER) != USB_RECIP_DEVICE) {
                return 1;           
        }
        else if ((wValue != USB_DEVICE_U1_ENABLE) && 
                (wValue != USB_DEVICE_U2_ENABLE)) {
                return 1;           
        }
        
        if (!vudc_is_equal_uus(vudc, uus_configured)) {
                dtrap(false);            
                return 1;
        }

        /* because u3dc doesn't support device-initiate u1/u2
         * hence, we don't need to configure hw here.
         */                           
        if (wValue == USB_DEVICE_U1_ENABLE)
                vudc_u1_enable(vudc, is_set);
        else
                vudc_u2_enable(vudc, is_set);

        vudc_start_async(vudc, vep);

        return 0;
}        

static void ep0_irq_nrdy(
        struct via_ep *vep,
        u16 nrdy
        )
{                   
        /* never */
        dtrap(false);          
}

static void ep0_irq_async_xfer(
        struct via_ep *vep, 
        u8 errtype, 
        u16 dp_length, 
        u8 upkt_num
        )
{                         
        /* do nothing */        
}                 

void ep0_irq_setup_pkt(
        struct via_ep *vep
        )
{        
        struct u3lite_dc_register *udc_reg;
        struct usb_device_request *setup;
        bool is_forward;

        /* copy received setup packet from hw */
        udc_reg = vudc_get_udc_register(vep->vudc);
        setup = vep->urb_setup;
        
        vu3l_memcpy_fromio(setup, udc_reg->ctrl_pkt_buf, sizeof(*setup));               
        
        vep_dbg(
                ">> %s bmRequestType %.2X bRequest %.2X"\
                " wValue %.4X wIndex %.4X wLength %.4X\n", 
                vu3l_string_usb_req(setup->bRequest),
                setup->bmRequestType,
                setup->bRequest,
                le16_to_cpu(setup->wValue),
                le16_to_cpu(setup->wIndex),
                le16_to_cpu(setup->wLength)
                );  

        /* clear actual length which need be transferred. */
        trap((uintptr_t)vep->urb->buffer == (uintptr_t)vep->urb->buffer_data);
        vep->urb->actual_length = 0;        
        
        switch (setup->bRequest) {
         
        case USB_REQ_GET_STATUS: {
                is_forward = ep0_get_status(vep, setup);
        } break;
        
        case USB_REQ_CLEAR_FEATURE: {
                is_forward = ep0_set_feature(vep, setup, false);
        } break;
        
        case USB_REQ_SET_FEATURE: {
                is_forward = ep0_set_feature(vep, setup, true);
        } break;

        case USB_REQ_SET_ADDRESS: {
                is_forward = ep0_set_address(vep, setup);
        } break;

        case USB_REQ_GET_DESCRIPTOR: {
                is_forward = ep0_get_desc(vep, setup);
        } break;        

        case USB_REQ_SET_CONFIGURATION: {                
                is_forward = ep0_set_configuration(vep, setup);
        } break;

        case USB_REQ_SET_SEL: {
                is_forward = ep0_set_sel(vep, setup);
        } break;

        case USB_REQ_SET_ISOCH_DELAY: {
                is_forward = ep0_set_isoch_delay(vep, setup);
        } break;

        default: {
                is_forward = true;      
        }
        
        }

        if (is_forward) 
                ep0_forward_setup(vep);        
}

static struct via_ep_info vep_info[NUM_VEP] = {        
        { 
                .name           = "ep0",        
                .maxpacket      = upkt_mds_ctrl,
                .build_upkt     = ep0_build_upkt,
                .irq_async_xfer = ep0_irq_async_xfer,
                .irq_nrdy       = ep0_irq_nrdy,
        },
        { 
                .name           = "epb-out",        
                .maxpacket      = upkt_mds_bulk,
                .build_upkt     = epbo_build_upkt,
                .irq_async_xfer = epbo_irq_async_xfer,
                .irq_nrdy       = epbo_irq_nrdy,
        },
        { 
                .name           = "epb-in",  
                .maxpacket      = upkt_mds_bulk,
                .build_upkt     = epbi_build_upkt,
                .irq_async_xfer = epbi_irq_async_xfer,
                .irq_nrdy       = epbi_irq_nrdy,
        },
};

void vep_setup(
        struct via_ep *vep,
        struct via_udc *vudc,
        unsigned int index,
        struct usb_endpoint_instance *usbe        
        )
{ 
        struct via_ep_info *info;

        dtrap(vep && usbe && (index < NUM_VEP));
            
        info = &vep_info[index];
        
        /* */
        vep->signature = SIGNATURE_WEP;        
        snprintf(vep->name, sizeof(vep->name), "%s", info->name); 
        
        vep->vudc = vudc;
        vep->usbe = usbe;
        
        vep->build_upkt = info->build_upkt;  
        vep->irq_async_xfer = info->irq_async_xfer;
        vep->irq_nrdy = info->irq_nrdy;
                 
        vep->epnum = usbe->endpoint_address & USB_ENDPOINT_NUMBER_MASK;
        vep->dir_in = (usbe->endpoint_address & USB_ENDPOINT_DIR_MASK) == USB_DIR_IN;        

        if (vep->epnum) {
                vep->urb = vep->dir_in ? usbe->tx_urb : usbe->rcv_urb;
                vep->urb_setup = NULL;
        }  
        else {
                vep->urb = vudc_get_ep0_urb(vudc);
                vep->urb_setup = &vep->urb->device_request;
        }
        
        vep->nrdy = 0;
        
        vep->is_nrdy_pending = 0;
        vep->is_udc_write = 0;
        vep->is_inflight = 0;
        vep->is_stall = 0;

        /* */
        vep->is_initialized = 1;
}
