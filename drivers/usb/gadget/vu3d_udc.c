/*
 * via u3lite udc - udc function
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

struct via_udc *g_vudc = NULL;

static int vudc_hw_reset(
        struct via_udc *vudc
        )
{
        return vu3l_set_and_handshake16(
                &vudc->udc_reg->udc_op, U3D_RST_FW, 0, USEC_PER_SEC
                );
}

static int vudc_hw_run(
        struct via_udc *vudc,
        bool is_run
        )
{
        int status;
        
        if (is_run) {
                dtrap(!vu3l_bit_test16(&vudc->udc_reg->udc_op, U3D_RUN));
                status = vu3l_set_and_handshake16(
                        &vudc->udc_reg->udc_op, U3D_RUN, U3D_RUN, USEC_PER_SEC
                        );
        }
        else {
                dtrap(vu3l_bit_test16(&vudc->udc_reg->udc_op, U3D_RUN));
                status = vu3l_clear_and_handshake16(
                        &vudc->udc_reg->udc_op, U3D_RUN, 0, USEC_PER_SEC
                        );
        }

        return status;
}

static int vudc_hw_enable(
        struct via_udc *vudc,
        bool is_enable
        )
{
        int status;
        
        if (is_enable) {
                dtrap(!vu3l_bit_test16(&vudc->gbl_reg->otg_en, U3DC_EN));
                status = vu3l_set_and_handshake16(
                        &vudc->gbl_reg->otg_en, U3DC_EN, U3DC_EN, USEC_PER_SEC
                        );
        }
        else {
                dtrap(vu3l_bit_test16(&vudc->gbl_reg->otg_en, U3DC_EN));
                status = vu3l_clear_and_handshake16(
                        &vudc->gbl_reg->otg_en, U3DC_EN, 0, USEC_PER_SEC
                        );
        }

        return status;
}

static void vudc_hw_pullup(
        struct via_udc *vudc,
        int is_on
        )
{
        dtrap(vudc->is_pullup_on ^ (is_on > 0));
                
        /* If U3D_EN = 1 : When FW set this bit¡Aand set 
         * R_FWDISLINK_EN = 1(RXE04[2])¡Athen HW will set SS_PORT_DISABLE = 1 
         */
        if (!vu3l_bit_test16(&vudc->gbl_reg->otg_en, U3DC_EN)) {
                vu3d_err("!U3DC_EN\n");
                return;
        }
        else if (!vu3l_bit_test16(&vudc->uhc_reg->port1_phy, R_UPDISEN_SS1)) {
                vu3d_err("!R_UPDISEN_SS1\n");
                return;
        }
        else if (!vu3l_bit_test32(&vudc->udc_reg->portsc_option, R_FWDISLINK_EN)) {
                vu3d_err("!R_FWDISLINK_EN\n");
                return;
        }
        
        if (is_on) {                 
                /* enable psc interrupt */
                ap_trap(!vu3l_bit_test8(&vudc->udc_reg->udc_inten, U3D_PORTSC_INTE));
                vu3l_bit_set8(&vudc->udc_reg->udc_inten, U3D_PORTSC_INTE);

                /* enable link detection */
                ap_trap(vu3l_bit_test8(&vudc->udc_reg->fwdislink, FWDISLINK)); 
                vu3l_bit_clear8(&vudc->udc_reg->fwdislink, FWDISLINK);  
                
                vudc->is_pullup_on = 1;
        }
        else {       
                /* disable link detection */
                ap_trap(!vu3l_bit_test8(&vudc->udc_reg->fwdislink, FWDISLINK)); 
                vu3l_bit_set8(&vudc->udc_reg->fwdislink, FWDISLINK);  

                /* disable psc interrupt */        
                ap_trap(vu3l_bit_test8(&vudc->udc_reg->udc_inten, U3D_PORTSC_INTE));
                vu3l_bit_clear8(&vudc->udc_reg->udc_inten, U3D_PORTSC_INTE); 
                
                vudc->is_pullup_on = 0;                
        } 
}

static void vudc_hw_usb_address(
        struct via_udc *vudc,
        u16 address
        )
{         
        vu3l_bit_modify16(
                &vudc->udc_reg->udc_op, 
                VAL_TO_BF(BM_DBC_DEVADDR, BS_DBC_DEVADDR),
                VAL_TO_BF(address, BS_DBC_DEVADDR)
                );
}

static int vudc_async_hw_enable(
        struct via_udc *vudc,
        bool is_enable
        )
{        
        int status;
        
        /* HW stop asnyc transfer after completed current upkt when 
         * SW set ASYNCEP_EN bit from 1 to 0.
         */
        if (is_enable) {
                dtrap(!vu3l_bit_test16(&vudc->udc_reg->async_inten, ASYNCEP_EN));
                status = vu3l_set_and_handshake16(
                        &vudc->udc_reg->async_inten, ASYNCEP_EN, ASYNCEP_EN, USEC_PER_SEC
                        );
                ap_dtrap(vu3l_bit_test16(&vudc->udc_reg->async_inten, ASYNCEP_EN));
        }
        else {
                int usec;
                u16 async_inten, async_intsts;

                /* U3D doesn't de-assert ASYNCEP_EN until SW clear 
                 * ASYEP_PKTERR_STS which result of usb disconnect.
                 * Hence, we must w1c ASYEP_PKTERR_STS it during 
                 * polling ASYNCEP_EN. 
                 */
                dtrap(vu3l_bit_test16(&vudc->udc_reg->async_inten, ASYNCEP_EN));

                vu3l_bit_clear16(&vudc->udc_reg->async_inten, ASYNCEP_EN);
               
                usec = USEC_PER_SEC;    
                status = -ETIMEDOUT;
                
                do {
                        async_inten = vu3l_read16(&vudc->udc_reg->async_inten);
                        async_intsts = vu3l_read16(&vudc->udc_reg->async_intsts);
                                
                        if (!(async_inten & ASYNCEP_EN)) {
                                status = 0;
                                break;
                        }

                        if (async_intsts & ASYEP_STS_MASK) {
                                vu3l_write16(
                                        &vudc->udc_reg->async_intsts, 
                                        async_intsts
                                        );
                                
                                ap_trap(!vu3l_bit_test16(
                                        &vudc->udc_reg->async_intsts, 
                                        ASYEP_STS_MASK
                                        ));
                        }
                        
                        udelay(1);
                        usec--;
                        
                } while (usec > 0);
        
                ap_dtrap(!vu3l_bit_test16(
                        &vudc->udc_reg->async_inten, ASYNCEP_EN
                        ));
        }

        return status;            
}  

static int vudc_async_stop_hw(
        struct via_udc *vudc
        )
{        
        int status;
        
        if (!vudc->is_hw_async_started) {
                vu3d_warn("<< !is_hw_async_started\n"); 
                return 0;
        }
        
        status = vudc_async_hw_enable(vudc, false);        
        if (!VU3L_SUCCESS(status)) {
                vu3d_err("<< vudc_async_hw_enable(0) failed %d\n", status);                 
                goto fail_hw;
        }
        
        vu3l_write32(&vudc->udc_reg->async_pkt_base_ptr, 0);
        vu3l_write8(&vudc->udc_reg->async_rptr, 0);

fail_hw:
        
        vudc->is_hw_async_started = 0;
        
        return status;            
} 

static int vudc_async_start_hw(
        struct via_udc *vudc
        )
{        
        int status;
        
        if (vudc->is_hw_async_started) {
                vu3d_warn("<< is_hw_async_started\n"); 
                return 0;
        }
        
        /* set async upktq base & current pkt ptr. 
         * hw will start running from current pkt ptr and use first
         * upkt's cycle as ccs when sw assert pkt ready register bit.
         * besides, hw will toggle its ccs when async upktq wrapped.
         *
         * NOTE: The Higher 28-bit Async Packet Queue base address¡]40bit¡^.
         * AXI read width is 40 bit.
         */ 
         
        vudc->async_pcs = 1;
        vudc->async_upktq_current = vudc->async_upktq_base; 

        /* zero upkt queue content to avoid hw treat garbage data valid. */
        memset(vudc->async_upktq_orignal, 0, ASYNC_UPKTQ_ALLOCATION);    
                
        dtrap(vudc->async_upktq_base_da);
        vu3l_write32(
                &vudc->udc_reg->async_pkt_base_ptr, 
                (vudc->async_upktq_base_da >> 8)
                );
        vu3l_write8(&vudc->udc_reg->async_rptr, 0);

        /* enable async. ep */
        status = vudc_async_hw_enable(vudc, true);   
        if (!VU3L_SUCCESS(status)) {
                vu3d_err("<< vudc_async_hw_enable(1) failed %d\n", status); 
                return status;
        }

        /* async_hw is started, update the corresponding flag */
        vudc->is_hw_async_started = 1;
        
        return status;            
} 

int vudc_async_restart_hw(
        struct via_udc *vudc
        )
{        
        int status;
        
        status = vudc_async_stop_hw(vudc);        
        if (!VU3L_SUCCESS(status)) {
                vu3d_err("vudc_async_stop_hw failed %d\n", status);                 
                goto fail_hw;
        }                
        
        status = vudc_async_start_hw(vudc);  
        if (!VU3L_SUCCESS(status)) {
                vu3d_err("vudc_async_start_hw failed %d\n", status);                 
                goto fail_hw;
        } 
        
fail_hw:
        
        if (!VU3L_SUCCESS(status)) {
                vu3d_err("restart_hw failed %d, disconnect_usb\n", status);
                vudc_handle_usb_event(vudc, uue_hw_failed, 0);
        }
        
        return status;
} 

static int vudc_init_hw(
        struct via_udc *vudc
        )
{        
        int status;
        u8 port1_dir;
        
        if (vudc->is_hw_initialized) {
                vu3d_warn("<< is_hw_initialized\n"); 
                return 0;
        }
        
        /* enable udc. 
         * note: don't need to clear U3H_EN because U3H/U3D HW 
         * are independent. 
         */               
        status = vudc_hw_enable(vudc, true);
        if (!VU3L_SUCCESS(status)) {
                vu3d_err("<< vudc_hw_enable(1) failed %d\n", status);
                return status;
        }                

        /* route port 1 to u3d */
        vu3l_bit_set8(&vudc->gbl_reg->port1_dir, SS1_PORT_SEL_SW);        
        vu3l_bit_modify8(
                &vudc->gbl_reg->port1_dir, 
                BM_SS1_PORT_MODSEL,
                VAL_TO_BF(SSPMS_U3D, BS_SS1_PORT_MODSEL)
                );      

        port1_dir = vu3l_read8(&vudc->gbl_reg->port1_dir);        
        dtrap(MBF_TO_VAL(port1_dir, BS_SS1_PORT_MODSEL, BM_SS1_PORT_MODSEL) == SSPMS_U3D);        
        dtrap(MBF_TO_VAL(port1_dir, BS_SS1_PORT_MODSTS, BM_SS1_PORT_MODSTS) == SSPMSS_U3D);
        
        /* configure port 1's phy setting.
         * - 
         * - Rx77C[0] = Rx78C[4] = 1: Make sure U3DC HW stable on U3 after set. 
         */
        ap_dtrap(vu3l_bit_test32(&vudc->uhc_reg->port1_feature, R_LTSSMDIS_EXITEN_SS1));                
        ap_dtrap(vu3l_bit_test16(&vudc->uhc_reg->port1_link, R_LFPSDGEN_SS1));                    
        ap_dtrap(vu3l_bit_test32(&vudc->uhc_reg->port1_feature, R_DEGLITCH_EN_SS1));
        vu3l_bit_set32(&vudc->uhc_reg->port1_feature, R_LTSSMDIS_EXITEN_SS1);
        vu3l_bit_set16(&vudc->uhc_reg->port1_link, R_LFPSDGEN_SS1);    
        vu3l_bit_set32(&vudc->uhc_reg->port1_feature, R_DEGLITCH_EN_SS1);

        /* reset udc */                 
        status = vudc_hw_reset(vudc);
        if (!VU3L_SUCCESS(status)) {
                vu3d_err("<< vudc_hw_reset failed %d\n", status);
                return status;
        }        

        /* all udc interrupt should be disabled after reset. */        
        ap_dtrap(!vu3l_bit_test8(&vudc->udc_reg->udc_inten, U3D_PORTSC_INTE));
        ap_dtrap(!vu3l_bit_test8(&vudc->udc_reg->ep0_inten, CTLEP_RXSETUP_INTE));
        ap_dtrap(!vu3l_bit_test16(&vudc->udc_reg->async_inten, ASYNCEP_INTE_MASK));
        ap_dtrap(!vu3l_bit_test16(&vudc->udc_reg->period_inten, PEREP_INTE));
        ap_dtrap(!vu3l_bit_test16(&vudc->udc_reg->period_inten, TICKTM_INTE));               

        /* DBC should be disabled in default */
        ap_dtrap(!vu3l_bit_test32(&vudc->uhc_reg->dcctrl, DBC_DCE));
        
        /* a. make sure R_UPDISEN_SS1 & R_FWDISLINK_EN register bits are 1 for 
         * sw to disable/re-enable udc link.
         * b. set R_UFPDETEN_SS1 to 1 to make hw could update pls after detach
         * link.
         */    
        ap_dtrap(vu3l_bit_test16(&vudc->uhc_reg->port1_phy, R_UPDISEN_SS1));
        ap_dtrap(vu3l_bit_test16(&vudc->uhc_reg->port1_phy, R_UFPDETEN_SS1));
        ap_dtrap(vu3l_bit_test32(&vudc->udc_reg->portsc_option, R_FWDISLINK_EN));
        vu3l_bit_set16(&vudc->uhc_reg->port1_phy, (R_UPDISEN_SS1 | R_UFPDETEN_SS1));    
        vu3l_bit_set32(&vudc->udc_reg->portsc_option, R_FWDISLINK_EN);        
        
        /* A. follow hw designer's test result, set TSOS Polarity 
         * Inversion Detection Mode to b100, make sure u3dc can determine 
         * TS1/TS2 from 456 xHC.
         * B. 
         */
        vu3l_bit_modify16(
                &vudc->uhc_reg->port1_phyls, 
                VAL_TO_BF(BM_R_TSPOLDET_SS1, BS_R_TSPOLDET_SS1),
                VAL_TO_BF(0x04, BS_R_TSPOLDET_SS1)
                ); 
        
        /* allow u3dc to accept LGO_U1/LGO_U2 from host */       
        vu3l_bit_set16(
                &vudc->uhc_reg->port1_pmu, 
                (R_U1LKPMEN_SS1 | R_U2LKPMEN_SS1)
                );
        
        /* disable connection:
         * Actually SW doesn't need to disable link itself because 
         * the default value of FWDISLINK is 1 after power-up/reset.
         */                
        if (!vu3l_bit_test8(&vudc->udc_reg->fwdislink, FWDISLINK)) {
                dtrap(false); 
                vudc_hw_pullup(vudc, 0);
        }        

        /* update the corresponding flag */
        vudc->is_hw_initialized = 1;
        
        /* DBG Only: dump udc hw operation related registers */
        vudc_dump_udc_hw(vudc);        
        
        return status;
}

static int vudc_deinit_hw(
        struct via_udc *vudc
        )
{        
        int status;                 

        if (!vudc->is_hw_initialized) {
                vu3d_warn("<< !is_hw_initialized\n"); 
                return 0;
        }
        
        /* disable udc */
        status = vudc_hw_enable(vudc, false);
        if (!VU3L_SUCCESS(status)) {
                vu3d_err("<< vudc_hw_enable(0) failed %d\n", status);                 
                goto fail_hw;
        }         

fail_hw:
        
        vudc->is_hw_initialized = 0;
        
        return status;
}

static int vudc_reinit_hw(
        struct via_udc *vudc
        )
{        
        int status;

        vudc_hw_pullup(vudc, 0); 
        
        status = vudc_deinit_hw(vudc);
        dtrap(VU3L_SUCCESS(status));
        
        status = vudc_init_hw(vudc); 
        if (!VU3L_SUCCESS(status)) {
                vu3d_err("<< vudc_init_hw failed %d\n", status);                 
                return status;
        } 
        
        vudc_hw_pullup(vudc, 1);        
        
        return status;
}


                
static int vudc_stop_hw_tx(
        struct via_udc *vudc
        )
{      
        int status;

        if (!vudc->is_hw_tx_started) {
                vu3d_warn("<< !is_hw_tx_started\n"); 
                return 0;
        }
        
        /* disable async. scheduling */
        status = vudc_async_stop_hw(vudc);
        if (!VU3L_SUCCESS(status)) {
                vu3d_err("<< vudc_async_stop_hw failed %d\n", status);                 
                goto fail_hw;
        }

        /* disable udc hw operations. */     
        status = vudc_hw_run(vudc, false);             
        if (!VU3L_SUCCESS(status)) {
                vu3d_err("<< vudc_hw_run(0) failed %d\n", status);                 
                goto fail_hw;
        }

fail_hw:
        
        vudc->is_hw_tx_started = 0;
        
        return status;
}

static int vudc_start_hw_tx(
        struct via_udc *vudc
        )
{        
        int status;

        if (vudc->is_hw_tx_started) {
                vu3d_warn("<< is_hw_tx_started\n"); 
                return 0;
        }
        
        /* start async. scheduling */
        status = vudc_async_start_hw(vudc);
        if (!VU3L_SUCCESS(status)) {
                vu3d_err("<< vudc_async_start_hw failed %d\n", status);                 
                return status;
        }
        
        
        /* enable udc hw operations. */        
        status = vudc_hw_run(vudc, true);        
        if (!VU3L_SUCCESS(status)) {
                vu3d_err("<< vudc_hw_run(1) failed %d\n", status);                 
                return status;
        }

        /* set flag to determine hw_tx_stop action */        
        vudc->is_hw_tx_started = 1;        

        return status;
}

static int vudc_restart_hw_tx(
        struct via_udc *vudc
        )
{        
        int status;

        /* stop started hw tx */
        dtrap(vudc->is_hw_tx_started);
        if (vudc->is_hw_tx_started) {
                
                status = vudc_stop_hw_tx(vudc);
                if (!VU3L_SUCCESS(status)) {
                        vu3d_err("<< vudc_stop_hw_tx failed %d\n", status);                         
                        return status;
                }
        }
        
        /* start hw tx */
        status = vudc_start_hw_tx(vudc);
        if (!VU3L_SUCCESS(status)) {
                vu3d_err("<< vudc_start_hw_tx failed %d\n", status);                 
                return status;
        }
        
        /* set flag to determine hw_tx_stop action */        
        vudc->is_hw_tx_started = 1;        

        return status;
}        

static void vudc_set_usb_state(
        struct via_udc *vudc,
        enum udc_usb_state next_uus
        )
{
        usb_device_event_t next_usbd_event;
        usb_device_event_t usbd_event[] = {
                DEVICE_UNKNOWN,
                DEVICE_HUB_CONFIGURED,
                DEVICE_HUB_RESET,
                DEVICE_RESET,
                DEVICE_ADDRESS_ASSIGNED,
                DEVICE_CONFIGURED,
                DEVICE_BUS_INACTIVE,
                DEVICE_BUS_INACTIVE,
                DEVICE_BUS_INACTIVE
                };
                
        dtrap(next_uus != uus_invalid);   

        /* update our usb_state */
        vudc->prior_usb_state = vudc->usb_state;
        vudc->usb_state = next_uus;

        /* update usbd state */
        next_usbd_event = (vudc_is_usb_enabled(vudc) && 
                vudc_is_sus_uus(vudc, vudc->prior_usb_state)) ? 
                DEVICE_BUS_ACTIVITY : usbd_event[next_uus];
        
        usbd_device_event_irq(vudc->usbd, next_usbd_event, 0);
        
        vu3d_info(
                "<< %s -> %s.\n", 
                vudc_string_uus(vudc->prior_usb_state), 
                vudc_string_uus(vudc->usb_state)
                );  
}

static void vudc_set_usb_address(
        struct via_udc *vudc,
        u8 address
        )
{
        vu3d_info("address %.2X\n", address); 
        
        vudc->usb_address = address;

        /* After received set address request, sw should update 
         * DEVADDR register field for HW, U3DC will use it to set
         * DEV_ADDR_HW and determine usb pkt.
         * Note: DEV_ADDR_HW Update Rule
         * 1. Clear to 0 iff PED change from 1 to 0.
         * 2. set as DEV_ADDR_SW after each control transfer status transaction completed.
         */
        vudc_hw_usb_address(vudc, vudc->usb_address);
}

static int vudc_reset_usb(
        struct via_udc *vudc,
        u32 portsc
        )
{
        int status;       
        enum port_speed pspd;         

        vu3d_info(">> %s\n", vudc_string_uus(vudc->usb_state)); 
        
        /* valid uus to reset usb ? it only possible to reset
         * usb if usb is connected with host. 
         */
        if (!vudc_is_usb_attached(vudc)) {
                vu3d_warn("<< !vudc_is_usb_attached\n"); 
                return -EPROTO;
        }

        /* valid speed ? */
        pspd = MBF_TO_VAL(portsc, BS_U3D_PORTSPEED, BM_U3D_PORTSPEED);        
        if (pspd != ps_super) {
                vu3d_err("<< !ps_super, portsc %X\n", portsc);                 
                return -EPROTO;
        }
        
        /* clear sw/hw usb address to default address, 0. */
        vudc_set_usb_address(vudc, 0);

        /* clear set_feature(u1_enable/u2_enable) setting */
        vudc_u1_enable(vudc, false);
        vudc_u2_enable(vudc, false);
                        
        /* if udc is enabled (uus > uus_default) before, 
         * we only need to restart transfer.
         * Otherwise, prepare to handle usb transfer. 
         */        
        if (vudc_is_usb_enabled(vudc)) {
                /* re-start hw's scheduling module */
                dtrap(vudc->is_hw_tx_started);
                status = vudc_restart_hw_tx(vudc);

                /* transfer related interrupt should be enabled before. */
                dtrap(vu3l_bit_test8(&vudc->udc_reg->ep0_inten, CTLEP_RXSETUP_INTE));
                dtrap(vu3l_bit_test16(&vudc->udc_reg->async_inten, ASYNCEP_INTE_MASK)); 
        }
        else {
        
                /* start hw's scheduling */
                status = vudc_start_hw_tx(vudc);
                if (!VU3L_SUCCESS(status)) {
                        vu3d_err("<< vudc_start_hw_tx failed %d\n", status);                         
                        return status;
                }                               

                /* enable transfer interrupts. */ 
                dtrap(!vu3l_bit_test8(&vudc->udc_reg->ep0_inten, CTLEP_RXSETUP_INTE));
                dtrap(!vu3l_bit_test16(&vudc->udc_reg->async_inten, ASYNCEP_INTE_MASK));        
                vu3l_bit_set8(&vudc->udc_reg->ep0_inten, CTLEP_RXSETUP_INTE);
                vu3l_bit_set16(&vudc->udc_reg->async_inten, ASYNCEP_INTE_MASK); 
        }

        /* clear pending async & nrdy interrupts */
        vu3l_bit_set16(&vudc->udc_reg->async_intsts, ASYEP_STS_MASK);    
        vu3l_write8(&vudc->udc_reg->nrdy_valid, 0xFF);
        ap_dtrap(!(vu3l_read32(&vudc->udc_reg->udc_intsts) & (ASYNC_INTSTS | NRDY_INTSTS))); 
        
        /* connection between u3d and host is built, inform fastboot 
         * the speed which is operating. 
         */
        udc_update_speed(USB_SPEED_SUPER);        
        
        vu3d_info("<< \n"); 
        
        return status;
}

static int vudc_disconnect_usb(
        struct via_udc *vudc,
        bool is_reinit_hw
        )
{
        int status;
        u32 udc_intsts;
               
        vu3d_info(">> is_reinit_hw %.1X\n", is_reinit_hw);            

        if (!is_reinit_hw && !vudc_is_usb_attached(vudc)) {
                vu3d_warn(
                        "<< %s !vudc_is_usb_attached\n", 
                        vudc_string_uus(vudc->usb_state)
                        ); 

                return 0;
        }
        
        /* u3dc is deconnected,              
         * 1. stop transfer related interrupts.
         * 2. flush async
         * 3. re-init u3dc hw back to the state after probe and pullup completion.
         *    note: actually we only need to stop hw's transfer module.
         *    but the u3dc's transfer module can't be stopped hence we re-init u3dc.
         * 4. clear all interrupt events.
         */
       
        /* disable transfer interrupts */        
        vu3l_bit_clear8(&vudc->udc_reg->ep0_inten, CTLEP_RXSETUP_INTE);
        vu3l_bit_clear16(&vudc->udc_reg->async_inten, ASYNCEP_INTE_MASK);
        
        /* stop hw's scheduling module */
        if (is_reinit_hw) {
                status = -EPROTO;
        }
        else if (vudc_is_usb_enabled(vudc)) {
                status = vudc_stop_hw_tx(vudc);
        }
        else {
                dtrap(!vudc->is_hw_tx_started);
                status = 0;
        }
                
        if (!VU3L_SUCCESS(status)) {

                vu3d_warn(
                        "%s %d, re-init udc.\n", 
                        is_reinit_hw ? "is_reinit_hw" : "vudc_stop_hw_tx failed", 
                        status
                        ); 
                                       
                vudc_reinit_hw(vudc);
        }  

        /* clear pending tx/dbc related interrupts */
        vu3l_bit_set8(&vudc->udc_reg->ep0_sts, CTLEP_RXSETUP_STS_MASK);
        vu3l_bit_set16(&vudc->udc_reg->async_intsts, ASYEP_STS_MASK);    
        vu3l_write8(&vudc->udc_reg->nrdy_valid, 0xFF);
        
        /* warning us if abnormal interrupt pending. */                
        udc_intsts = vu3l_read32(&vudc->udc_reg->udc_intsts);                        
        if (udc_intsts & (~(DBC_INTSTS | PSCEG_INTSTS))) { 
                vu3d_warn(
                        "%s udc_intsts %X "\
                        "portsc %X ep0_sts %X async_intsts %X "\
                        "nrdy_valid %X udc_sts %X db_sts %X\n",
                        vudc_string_uus(vudc->usb_state),
                        vu3l_read32(&vudc->udc_reg->udc_intsts), 
                        vu3l_read32(&vudc->udc_reg->portsc),
                        vu3l_read8(&vudc->udc_reg->ep0_sts),
                        vu3l_read16(&vudc->udc_reg->async_intsts), 
                        vu3l_read8(&vudc->udc_reg->nrdy_valid),
                        vu3l_read8(&vudc->udc_reg->udc_sts),
                        vu3l_read8(&vudc->udc_reg->db_sts)
                        );   
        }
        
        vu3d_info("<< \n");  

        return 0;
}

int vudc_handle_usb_event(
        struct via_udc *vudc,
        enum udc_usb_event uue,
        u32 uue_para
        )
{
        int status;
        enum port_link_state pls; 
        enum port_speed pspd;
        bool psc, addr, config, deconfig, hw_failed, udc_stop;
        u32 ccs, csc, plc, prc;
       
        /* INFO: dump uue & uue_para content */
        vudc_dump_uue_related_info(vudc, uue, uue_para);
        
        psc = (uue == uue_portsc);
        addr = (uue == uue_address);
        config = (uue == uue_configure);
        deconfig = (uue == uue_deconfigure);
        hw_failed = (uue == uue_hw_failed);
        udc_stop = (uue == uue_udc_stop);

        if (hw_failed || udc_stop) {
                status = 0;
                goto disconnect_usb;
        }
        
        pls = psc ? MBF_TO_VAL(uue_para, BS_U3D_PLS, BM_U3D_PLS) : -1;   
        pspd = psc ? MBF_TO_VAL(uue_para, BS_U3D_PORTSPEED, BM_U3D_PORTSPEED) : ps_undefined; 
        ccs = psc ? TEST_FLAG(uue_para, U3D_CCS) : 0;        
        csc = psc ? TEST_FLAG(uue_para, U3D_CSC) : 0;
        plc = psc ? TEST_FLAG(uue_para, U3D_PLC) : 0;
        prc = psc ? TEST_FLAG(uue_para, U3D_PRC) : 0;         
        
        /* usb state machine */
usb_state_update:

        status = 0;        
        switch (vudc->usb_state) {
                
        case uus_powered: {
        
                if (csc && ccs) {
                        csc = 0;                        
                        vudc_set_usb_state(vudc, uus_attached);
                        goto usb_state_update;
                }
                
        } break;

        case uus_attached: {

                if (csc && (!ccs)) {                        
                        csc = 0;   
                        goto disconnect_usb;
                        
                }
                else if (psc && (pls == pls_u0) && (pspd == ps_super)) {                         
                        /* clear prc to avoid reset usb twice */
                        prc = 0;                        
                        goto reset_usb;
                }
                
        } break;
        
        case uus_default: {

                if (csc && (!ccs)) {                        
                        csc = 0;                         
                        goto disconnect_usb;
                }                    
                else if (plc && (pls == pls_u3)) {
                        
                        plc = 0;     
                        
                        vudc_set_usb_state(vudc, uus_sus_default);
                        goto usb_state_update;
                }
                else if (prc && (pls == pls_u0)) {                        
                        prc = 0;                        
                        goto reset_usb;
                }
                else if (addr) {

                        /* set udc's usb device address, 
                         * note: all structures which defined in usb spec. 
                         * are little endian format. 
                         */  
                        addr = 0;
                        
                        dtrap(uue_para <= 0xFF);
                        vudc_set_usb_address(vudc, (u8)uue_para);
                        
                        vudc_set_usb_state(vudc, uus_addressed);                        
                        goto usb_state_update;
                }  
                
        } break;

        case uus_addressed: {

                if (csc && (!ccs)) {                        
                        csc = 0;                         
                        goto disconnect_usb;
                }                    
                else if (psc && (pls == pls_u3)) {
                        vudc_set_usb_state(vudc, uus_sus_addressed);
                        goto usb_state_update;
                }
                else if (prc && (pls == pls_u0)) {                        
                        prc = 0;                        
                        goto reset_usb;
                }
                else if (config) {
                        
                        config = 0;
                        
                        vudc_set_usb_state(vudc, uus_configured);
                        goto usb_state_update;
                }
                
        } break;

        case uus_configured: {

                if (csc && (!ccs)) {                        
                        csc = 0;                         
                        goto disconnect_usb;
                }                    
                else if (psc && (pls == pls_u3)) {                        
                        vudc_set_usb_state(vudc, uus_sus_configured);
                        goto usb_state_update;
                }
                else if (prc && (pls == pls_u0)) {                        
                        prc = 0;                        
                        goto reset_usb;
                }
                else if (deconfig) {
                        
                        deconfig = 0;

                        vudc_u1_enable(vudc, false);
                        vudc_u2_enable(vudc, false);
                        
                        vudc_set_usb_state(vudc, uus_addressed);
                        goto usb_state_update;
                } 
                
        } break;
       
        case uus_sus_default: {

                if (csc && (!ccs)) {                        
                        csc = 0;                         
                        goto disconnect_usb;
                }                    
                else if (plc && (pls == pls_u0)) {
                        
                        plc = 0;
                        
                        vudc_set_usb_state(vudc, uus_default);
                        goto usb_state_update;
                }
                else if (prc && (pls == pls_u0)) {                        
                        prc = 0;                        
                        goto reset_usb;
                }  
                
        } break;

        case uus_sus_addressed: {

                if (csc && (!ccs)) {                        
                        csc = 0;                         
                        goto disconnect_usb;
                }                    
                else if (plc && (pls == pls_u0)) {
                        
                        plc = 0;
                        
                        vudc_set_usb_state(vudc, uus_addressed);
                        goto usb_state_update;
                }
                else if (prc && (pls == pls_u0)) {                        
                        prc = 0;                        
                        goto reset_usb;
                }  
                
        } break;
        
        case uus_sus_configured: {
                
                if (csc && (!ccs)) {                        
                        csc = 0;                         
                        goto disconnect_usb;
                }                    
                else if (plc && (pls == pls_u0)) {
                        
                        plc = 0;
                        
                        vudc_set_usb_state(vudc, uus_configured);
                        goto usb_state_update;
                }
                else if (prc && (pls == pls_u0)) {                        
                        prc = 0;                        
                        goto reset_usb;
                }
                
        } break;

        default: {
                dtrap(false);
                status = -EPROTO;
        }
        
        }         

        goto usb_state_done;
        
reset_usb:

        status = vudc_reset_usb(vudc, uue_para);
        if (!VU3L_SUCCESS(status)) {
                vu3d_err("vudc_reset_usb failed %d\n", status); 

                dtrap(!udc_stop && !hw_failed);
                hw_failed = true;
                goto disconnect_usb;
        }
        
        vudc_set_usb_state(vudc, uus_default);
        goto usb_state_update;
        
disconnect_usb:

        vudc_disconnect_usb(vudc, hw_failed);                        
        vudc_set_usb_state(vudc, uus_powered);
                        
usb_state_done:
        
        vu3d_info("<< %d\n", status); 
        
        return status;
}

static void vudc_irq_portsc(
        struct via_udc *vudc
        )
{        
        u16 port_change;
        
        /* DBG only. dump portsc register */
        vudc_dump_portsc(vudc);

        /* update portsc, we use it to determine next action */       
        vudc->prior_portsc = vudc->portsc;        
        vudc->portsc = vu3l_read32(&vudc->udc_reg->portsc);        
        
        /* w1c port status change bits. 
         * NOTE: 
         * 1. just clear found change bits only to avoid 
         *    un-received interrupt status lost.         
         * 2. HW will set SSLINK_FAKEDIS to 1 and clear PR after 
         *    PED be set to 0 by SW.
         *    And PLS is disable after SSLINK_FAKEDIS to 1.
         *    So SW should avoid setting PED to 0.
         */
        port_change = (vudc->portsc & U3D_CHANGE_MASK) >> 16; 
        vu3l_write16(&vudc->udc_reg->port_change, port_change);
        ap_trap(!vu3l_bit_test16(&vudc->udc_reg->port_change, (U3D_CHANGE_MASK >> 16)));
        ap_trap(!vu3l_bit_test32(&vudc->udc_reg->udc_intsts, PSCEG_INTSTS));
        
        /* handling PSC */        
        vudc_handle_usb_event(vudc, uue_portsc, vudc->portsc);

        vu3d_dbg("<< \n"); 
}

static void vudc_irq_setup_pkt(
        struct via_udc *vudc
        )
{        
        /* DBG only. dump ep0_sts register */
        vudc_dump_ep0_sts(vudc);
        
        /* w1c interrupt source */
        vu3l_bit_set8(&vudc->udc_reg->ep0_sts, CTLEP_RXSETUP_STS_MASK);
        ap_trap(!vu3l_bit_test8(&vudc->udc_reg->ep0_sts, CTLEP_RXSETUP_STS_MASK));
        ap_trap(!vu3l_bit_test32(&vudc->udc_reg->udc_intsts, CTLEP_RXSETUP_INTSTS));
        
        /* USB Spec: Devices must accept all error-free Setup packets. 
         * If a new Setup packet arrive before a previous 
         * control transfer completes, the device must abandon 
         * the previous transfer and start the new one.
         */
        trap(!vudc->async_vep);        

        ep0_irq_setup_pkt(vudc->vep0);
}

static void vudc_irq_nrdy(
        struct via_udc *vudc
        )
{        
        struct via_ep *vep;
        u8 nrdy_valid;
        u16 nrdy;
        u8 i;

        trap(!vudc->async_vep);      
        
        nrdy_valid = vu3l_read8(&vudc->udc_reg->nrdy_valid);
        
        /* DBG only. dump nrdy_valid register */
        vudc_dump_nrdy_valid(vudc);   

        for (i = 0; i < NUM_NRDY_ENTRY; i++) {
                                        
                if (!(nrdy_valid & (1 << i))) 
                        continue;

                /* DBG only. dump nrdy detail */
                vudc_dump_nrdy_entry(vudc, (LAST_NRDY_ENTRY - i));   
                
                nrdy = vu3l_read16(&vudc->udc_reg->nrdy_entry[LAST_NRDY_ENTRY - i]);
                vep = vudc_find_nrdy_vep(vudc, nrdy);
                if (!vep) {                        
                        vu3d_err("<< !vudc_find_nrdy_vep(%X)\n", nrdy);  
                        continue;        
                }
                
                vep->irq_nrdy(vep, nrdy);
        }

        /* w1c interrupt. 
         * NOTE: 
         * 1. sw should save nrdy before clear nrdy_valid to avoid nrdy be 
         *    be overwrited by hardware. 
         * 2. use write not bit_set to avoid pending nrdy interrupt be clear.
         *    because bit_set will read again before write.
         */
        vu3l_write8(&vudc->udc_reg->nrdy_valid, nrdy_valid);
        ap_trap(!vu3l_bit_test8(&vudc->udc_reg->nrdy_valid, nrdy_valid));                    
} 

static void vudc_irq_async_xfer(
        struct via_udc *vudc
        )
{            
        struct via_ep *vep;
        struct u3lite_packet *upkt, *upkt_end;  
        u16 async_intsts;
        u8 errtype; 
        u8 upkt_num;
        u16 dp_length;
        
        /* DBG only. dump async_intsts */
        vudc_dump_async_intsts(vudc);
        
        /* correct interrupt assertion ? */
        ap_trap(vu3l_bit_test16(&vudc->udc_reg->async_intsts, ASYEP_PKTERR_STS) ^ 
                vu3l_bit_test16(&vudc->udc_reg->async_intsts, ASYEP_PKTCMPL_STS));
        
        /* keep async_intsts before w1c intsts */        
        async_intsts = vu3l_read16(&vudc->udc_reg->async_intsts);
        
        /* w1c interrupt */
        vu3l_write16(&vudc->udc_reg->async_intsts, async_intsts);
        ap_trap(!vu3l_bit_test16(&vudc->udc_reg->async_intsts, ASYEP_STS_MASK));
        ap_trap(!vu3l_bit_test32(&vudc->udc_reg->udc_intsts, ASYNC_INTSTS));

        /* assume 0 for non short packet case */
        upkt_num = 0;
        dp_length = 0;
        
        /* async success ? */
        if (async_intsts & ASYEP_PKTCMPL_STS) {                
                errtype = upkt_err_no;
                goto async_completion;
        }
        
        /* PKTERR_STS should asserted here! if not, hw issue */        
        if (!(async_intsts & ASYEP_PKTERR_STS)) {                               
                vu3d_err("!ASYEP_PKTERR_STS\n"); 
                errtype = upkt_err_tx;
                goto fail_async;
        }

        /* handling PKTERR_STS below. 
         * Note: PKT_RDY_STS will be clear here but ASYNC_EN doesn't. 
         * If SW want to continue, set PKT_RDY_STS again.
         * If need to modify CPTR, stop ASYNC_EN then update again.
         * After updated, set ASYNCEP_EN to 1 again, HW will fetch this
         * new async upkt queue base.
         */
        ap_trap(!vu3l_bit_test8(&vudc->udc_reg->async_pkt_sts, ASYEP_PKT_RDY_STS));
        ap_trap(vu3l_bit_test16(&vudc->udc_reg->async_inten, ASYNCEP_EN));
        
        errtype = MBF_TO_VAL(
                async_intsts, BS_ASYEP_PKT_ERR_TYPE, BM_ASYEP_PKT_ERR_TYPE
                );  
        if (errtype != upkt_err_sp) { 
                vu3d_err("!upkt_err_sp %s\n", vudc_string_upkterr(errtype));     
                goto fail_async;
        }
        
        /* short packet: 
         * evalute status & transferred data length to update urb.
         *
         * transferred data length = 
         * last short packet's transferred length +
         * num of DP (or ACKTP) * max package size
         */        
        upkt = vudc_query_next_upkt(vudc, vudc->flight_upkt_base);
        upkt_end = vudc->async_upktq_base + 
                MBF_TO_VAL(async_intsts, BS_ASYEP_PKT_CRPTR, BM_ASYEP_PKT_CRPTR);
      
        ap_trap(MBF_TO_VAL(upkt->dword[0], BS_TP_TYPE, BM_TP_TYPE) == upkt_type_tp);
        ap_trap(MBF_TO_VAL(upkt->dword[1], BS_TP_SUBTYPE, BM_TP_SUBTYPE) == upkt_sttp_ack);                       

        /* hw_crptr should be valid */
        ap_trap(vudc_is_upkt_in_range(
                vudc, upkt_end, upkt, vudc->flight_upkt_next
                ));
                                        
        dp_length = MBF_TO_VAL(
                vu3l_read16(&vudc->udc_reg->async_rx_dp_length), 
                BS_ASYEP_PKT_DPLEN, 
                BM_ASYEP_PKT_DPLEN
                ); 
        trap(dp_length);
        
        if (upkt_end != upkt) {                
                upkt_num = (upkt_end > upkt) ? 
                        (upkt_end - upkt) : 
                        (ASYNC_UPKTQ_ELEMENT - (upkt - upkt_end));   
        }          
        
        /* normally short pkt case should occur in the last DP. 
         * if not then we must update hw's crptr to next async.
         * Here, we will update crptr to async_upktq_base and
         * make next async be scheduled start from async_upktq_base.
         */
        if (vudc->flight_upkt_next == vudc_query_next_upkt(vudc, upkt_end))               
                goto async_completion;
       
fail_async:

        /* restart hw to allow next pkt scheduling */
        vudc_async_restart_hw(vudc);
        
async_completion:       

        vudc_async_restart_hw(vudc);
        
        /* notify vep of async_intsts assertion */
        vep = vudc->async_vep;

        trap(vep_is_valid(vep));
        vep->irq_async_xfer(vep, errtype, dp_length, upkt_num);                          
} 

void vudc_start_async(
        struct via_udc *vudc, 
        struct via_ep *vep
        )
{            
        int status;        
        
        vu3d_dbg(">> vep %p %s\n", vep, vep_get_name(vep));        

        /* vep will be scheduling after assert upkt_ready */
        trap(!vudc->async_vep);
        vudc->async_vep = vep;
        
        /* ok. ready to start async below. 
         * a. record inflight upkt range. 
         * b. build upkt for hw to response to usb host.
         * c. ring hw.
         */
        vudc->flight_upkt_base = vudc->async_upktq_current;        
        vep->build_upkt(vep);        
        vudc->flight_upkt_next = vudc->async_upktq_current;
        
        /* DBG: dump upkt content */ 
        /*
        vudc_dump_upkt_on_range(
                vudc, vudc->flight_upkt_base, vudc->flight_upkt_next
                );    
        */        
        
        /* ask hw to send prepared upkt */
        vudc_assert_upkt_ready(vudc);        

        /* waiting async. upkt completion interrupt ? */
        status = vu3l_handshake32(
                &vudc->udc_reg->udc_intsts, 
                ASYNC_INTSTS, 
                ASYNC_INTSTS, 
                2 * USEC_PER_SEC
                );  
        if (!VU3L_SUCCESS(status)) {
                
                vu3d_err("!ASYNC_INTSTS\n");
                
                vudc_async_restart_hw(vudc); 
                goto async_timeout;
        }
        
        /* handle async interrupt below */
        ap_trap(vu3l_read32(&vudc->udc_reg->udc_intsts) & DBC_INTSTS); 
        vudc_irq_async_xfer(vudc);
              
async_timeout:
        
        /* async is completed or timeout, null async_vep */              
        vudc_complete_async(vudc);
        
        
        vu3d_dbg("<< \n");
}

static void vudc_free_urb(
        struct via_udc *vudc    
        )
{
        if (!vudc->ep0_urb) {
                dtrap(false);
                return;
        }
        
        usbd_dealloc_urb(vudc->ep0_urb);
        vudc->ep0_urb = NULL;
}

static void vudc_allocate_urb(
        struct via_udc *vudc    
        )
{
        if (vudc->ep0_urb) {
                vu3d_warn("urb allocated %p\n", vudc->ep0_urb);
                
        } else {                
                vudc->ep0_urb = usbd_alloc_urb(
                        vudc->usbd, vudc->usbd->bus->endpoint_array
                        );
                dtrap(vudc->ep0_urb);                                                
        }
}

static void vudc_cleanup(
        struct via_udc *vudc
        )
{            
        dtrap(vudc);
        
        vudc_deinit_hw(vudc);
        dtrap(!vudc->is_hw_tx_started);
        dtrap(!vudc->is_hw_async_started);
                 
        memset(vudc, 0, sizeof(*vudc));        
        kfree(vudc); 

        dtrap(g_vudc == vudc);
        g_vudc = NULL;        
}

static int vudc_map_resource(
        struct via_udc *vudc
        )
{    
        u32 mmio0, mmio1;
        
#if VU3L_ON_ELITE_PCIE

        /* get mmio0 & mmio1 */ 
        pci_read_config_dword(VU3L_PCI_BDF, PCI_BASE_ADDRESS_0, &mmio0);
        pci_read_config_dword(VU3L_PCI_BDF, PCI_BASE_ADDRESS_2, &mmio1);

        mmio0 &= PCI_BASE_ADDRESS_MEM_MASK;
        mmio1 &= PCI_BASE_ADDRESS_MEM_MASK;

#else
        /* 5880 SOC memory map (v1.18): 
         * U3H:0xD9090000-0xD9090FFF, 
         * U3D:0xD9091000-0xD9091FFF 
         */
        mmio0 = 0xD9090000;
        mmio1 = 0xD9091000;        
        
#endif

        vudc->uhc_reg = (struct u3lite_hc_register*)(mmio0 + REG_OFFSET(uhc));  
        vudc->udc_reg = (struct u3lite_dc_register*)(mmio1 + REG_OFFSET(udc));  
        vudc->gbl_reg = (struct u3lite_gbl_register*)(mmio0 + REG_OFFSET(gbl)); 
        
        /* init vu3l_ap_reg if VU3L_AP mechanism is enabled. */
        VU3L_INIT_AP_TRAP_REG(&vudc->udc_reg->ap_addr);

        vu3d_info(
                "uhc_reg %p udc_reg %p gbl_reg %p\n", 
                vudc->uhc_reg, vudc->udc_reg, vudc->gbl_reg 
                );
         
        return 0;
}

static int vudc_configure_pci(
        struct via_udc *vudc
        )
{       
#if VU3L_ON_ELITE_PCIE

        u16 vid, did, cmd;             
        
        pci_read_config_word(VU3L_PCI_BDF, PCI_VENDOR_ID, &vid);
        pci_read_config_word(VU3L_PCI_BDF, PCI_DEVICE_ID, &did);

        if((vid != VU3L_PCI_VENDOR_ID) || (did != VU3L_PCI_DEVICE_ID)) {
                vu3d_err(
                        "Non-supported pci (%X.%X) != (%X.%X)\n", 
                        vid, did, VU3L_PCI_VENDOR_ID, VU3L_PCI_DEVICE_ID
                        );
                
                return -ENODEV;
        }

        /* enable io */
        pci_read_config_word(VU3L_PCI_BDF, PCI_COMMAND, &cmd);
        cmd |= PCI_COMMAND_MEMORY | PCI_COMMAND_MASTER;
        pci_write_config_word(VU3L_PCI_BDF, PCI_COMMAND, cmd);

        /* configure latency */
        pci_write_config_byte(VU3L_PCI_BDF, PCI_LATENCY_TIMER, 0x40);
        
#endif

        return 0;
}

/* 
 * export api for fastboot below.
 */

/* Called to start packet transmission. */
int u3d_endpoint_write(
        struct usb_endpoint_instance *endpoint
        )
{                
        epbi_endpoint_write(&g_vudc->vep[2], 0);        
        return 0;
}

/* Associate a physical endpoint with endpoint_instance */
void u3d_setup_ep(
        struct usb_device_instance *device, 
        unsigned int ep, 
        struct usb_endpoint_instance *endpoint
        )
{
        struct via_udc *vudc;
        
        vudc = g_vudc;
        dtrap(vudc_is_valid(vudc));

        vu3d_info(
                ">> device %p ep %d endpoint %p\n", 
                device, ep, endpoint
                ); 
        
        vep_setup(&vudc->vep[ep], vudc, ep, endpoint); 

        vu3d_info("<< \n"); 
}

/* do pseudo interrupts */
void u3d_irq(
    void
    )
{         
        u32 intsts;
        struct via_udc *vudc;
        
        vudc = g_vudc;
        trap(vudc_is_valid(vudc));
        
        intsts = vu3l_read32(&vudc->udc_reg->udc_intsts);                
        if (!(intsts & DBC_INTSTS)) 
                return;        

        /* DBG Only: dump udc_intsts register */
        vudc_dump_udc_intsts(vudc);
        
        /* port status change interrupt ? */
        if (intsts & PSCEG_INTSTS) { 
                
                vudc_irq_portsc(vudc);   

                /* udc_intsts register bit(s) may be clear by vudc_irq_portsc.
                 * Hence, we need to update intsts here again to avoid driver to
                 * handle clear interrupt event twice.
                 *
                 * Ex: PRC -> Driver clear ASYNC_INTSTS -> DON'T need to handle 
                 * ASYNC_INTSTS event which cause by bus reset.
                 */
                intsts = vu3l_read32(&vudc->udc_reg->udc_intsts);
                
                /* it is unnecessary to handle transfer related interrupts 
                 * if udc is not connected to host or in u0. 
                 */
                if (!vudc_is_usb_enabled(vudc)) {
                      
                        if (intsts & (~(DBC_INTSTS | PSCEG_INTSTS))) { 
                                vu3d_warn(
                                        "%s intsts %X "\
                                        "portsc %X ep0_sts %X async_intsts %X "\
                                        "nrdy_valid %X udc_sts %X db_sts %X\n",
                                        vudc_string_uus(vudc->usb_state),
                                        intsts,
                                        vu3l_read32(&vudc->udc_reg->portsc),
                                        vu3l_read8(&vudc->udc_reg->ep0_sts),
                                        vu3l_read16(&vudc->udc_reg->async_intsts), 
                                        vu3l_read8(&vudc->udc_reg->nrdy_valid),
                                        vu3l_read8(&vudc->udc_reg->udc_sts),
                                        vu3l_read8(&vudc->udc_reg->db_sts)
                                        );   
                        }
                        
                        return;
                }
                else if (vudc_is_equal_uus(vudc, uus_default)) {
                        dtrap(!(intsts & (~(CTLEP_RXSETUP_INTSTS | DBC_INTSTS | PSCEG_INTSTS))));
                }
        }

        /* setup upkt received interrupt ? */        
        if (intsts & CTLEP_RXSETUP_INTSTS)
                vudc_irq_setup_pkt(vudc);

        /* handle nrdy if received */ 
        if (intsts & NRDY_INTSTS)
                vudc_irq_nrdy(vudc);             
}

/* Turn off the USB connection by disabling the pullup resistor */
void u3d_disconnect(    
        void
        )
{
        struct via_udc *vudc;
              
        vudc = g_vudc; 
        dtrap(vudc_is_valid(vudc));
        
        vu3d_info(">> \n");  
        
        dtrap(vudc_is_valid(vudc));
        vudc_hw_pullup(vudc, 0); 

        vu3d_info("<< \n");  
}

/* Turn on the USB connection by enabling the pullup resistor */
void u3d_connect(  
        void
        )
{
        struct via_udc *vudc;
              
        vudc = g_vudc; 
        dtrap(vudc_is_valid(vudc));
        
        vu3d_info(">> \n");  
        
        dtrap(vudc_is_valid(vudc));
        vudc_hw_pullup(vudc, 1); 

        vu3d_info("<< \n");  
}

/* Switch off the UDC */
void u3d_disable(    
        void
        )
{
        struct via_udc *vudc;
              
        vudc = g_vudc; 
        dtrap(vudc_is_valid(vudc));

        vu3d_info(">> \n");  
        
        /* make sure usb connection is disabled before disable udc */
        if (vudc->is_pullup_on) {
                dtrap(false);
                vudc_hw_pullup(vudc, 0);
        }        
                
        /* disconnect usb device if attached */        
        if (vudc_is_usb_attached(vudc)) 
                vudc_handle_usb_event(vudc, uue_udc_stop, 0);

        /* free allocated ep0 urb if present */
        vudc_free_urb(vudc);      
        
        /* delink usbd */
        dtrap(vudc->usbd);
        vudc->usbd = NULL;

        vu3d_info("<< \n");  
}

/* Switch on the UDC */
void u3d_enable(
        struct usb_device_instance *device
        )
{
        struct via_udc *vudc;
        
        vudc = g_vudc;                 
        dtrap(vudc_is_valid(vudc)); 

        vu3d_info(">> \n");  
        
        /* udc is enabled ? */
        dtrap(vu3l_bit_test16(&vudc->gbl_reg->otg_en, U3DC_EN));

        /* port is disabled ? */
        dtrap(!vudc->is_pullup_on); 
        dtrap(vu3l_bit_test8(&vudc->udc_reg->fwdislink, FWDISLINK));                        

        /* save the corresponding usbd */
        dtrap(!vudc->usbd);
        vudc->usbd = device;

        /* usbd is ready to allocate urb */
        vudc_allocate_urb(vudc);

        vu3d_info("<< \n");  
}

/* Start to initialize h/w stuff */
int u3d_init(
        void
        )
{
        int status;
        struct via_udc *vudc;        

        /* allocate g_vudc to manage udc operation */
        if (g_vudc) {
                vu3l_warn(
                        "g_vudc %p allocated, is_inited %.1X\n", 
                        g_vudc, g_vudc->is_inited
                        );                    
                return 0;                          
        } 
        
        g_vudc = (struct via_udc*)kzalloc(sizeof(struct via_udc), GFP_KERNEL);
        if (!g_vudc) {
                vu3l_err("kzalloc failed.\n");  
                return -ENOMEM;
        }        
        vudc = g_vudc;         
        
        /* init vudc below */
        vudc->signature = SIGNATURE_VUDC;
        
        snprintf(vudc->name, sizeof(vudc->name), "%s", "vu3d"); 
                        
        /* configrure pci if we are testing U3Lite on PCI FPGA */
        status = vudc_configure_pci(vudc);
        if (!VU3L_SUCCESS(status)) {
                vu3d_err("configure pci fail\n");
                goto fail_probe;
        }  
        
        /* get mmio/irq hw resources & map mmio to cpu addressing space */
        status = vudc_map_resource(vudc);
        if (!VU3L_SUCCESS(status)) {
                vu3d_err("map resource fail\n");
                goto fail_probe;
        }       
        
        /* init ep link */  
        vudc->vep0 = &vudc->vep[0];
        vudc->vepbo = &vudc->vep[1];
        vudc->vepbi = &vudc->vep[2];

        /* the vep which is schedulng trasnfer */
        vudc->async_vep = NULL;
        
        /* init async upkt related members */           
        BUILD_BUG_ON((ASYNC_UPKTQ_SIZE/sizeof(struct u3lite_packet)) != ASYNC_UPKTQ_ELEMENT);  
        dtrap(!vudc->async_upktq_orignal);
        vudc->async_upktq_orignal = dma_alloc_coherent(
                ASYNC_UPKTQ_ALLOCATION, 
                &vudc->async_upktq_orignal_da
                );
        if (!vudc->async_upktq_orignal) {
                dtrap(false);  
                status = -ENOMEM;
                goto fail_probe;
        }  
         
        vudc->async_upktq_base = PTR_ALIGN(vudc->async_upktq_orignal, ASYNC_UPKTQ_ALIGNMENT); 
        vudc->async_upktq_base_da = ALIGN(vudc->async_upktq_orignal_da, ASYNC_UPKTQ_ALIGNMENT);          
        vudc->async_upktq_last = &vudc->async_upktq_base[ASYNC_UPKTQ_ELEMENT - 1];                           

        /* udc plays as an usb device, use usb_state to keep current
         * usb device state. 
         */
        vudc->prior_usb_state = uus_invalid;
        vudc->usb_state = uus_powered;                 
        
        /* init hw to idle state */
        status = vudc_init_hw(vudc);
        if (!VU3L_SUCCESS(status)) {
                vu3d_err("vudc_init_hw failed\n");
                goto fail_probe;
        }           

        /* udc_init init ok, update the corresponding flag bit */
        dtrap(!vudc->is_inited);
        vudc->is_inited = 1;

        goto finish;
        
fail_probe:
        
        /* remove created vudc iff fail */        
        vudc_cleanup(vudc);                  

finish:
        vu3l_info("<< \n");  
        
        return status;
}

