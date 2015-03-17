/*
 * via u3lite udc - trace function
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

char*
vudc_string_pls(
    int pls
    )
{
    switch (pls) {
    make_case(pls_u0)
    make_case(pls_u1)
    make_case(pls_u2)
    make_case(pls_u3)
    make_case(pls_disabled)
    make_case(pls_rxdetect)
    make_case(pls_inactive)
    make_case(pls_polling)
    make_case(pls_recovery)
    make_case(pls_hotreset)
    make_case(pls_compliance)
    make_case(pls_resume)
    }

    return "unknown";
}

char*
vudc_string_port_speed(
    int pls
    )
{
    switch (pls) {
    make_case(ps_full)
    make_case(ps_low)
    make_case(ps_high)
    make_case(ps_super)
    }

    return "unknown";
}
     
char*
vudc_string_uus(
    int uus
    )
{
    switch (uus) {
    make_case(uus_invalid)
    make_case(uus_powered)
    make_case(uus_attached)
    make_case(uus_default)
    make_case(uus_addressed)
    make_case(uus_configured)
    make_case(uus_sus_default)
    make_case(uus_sus_addressed)
    make_case(uus_sus_configured)
    }

    return "unknown";
}

char*
vudc_string_uue(
    int uue
    )
{
        switch (uue) {
        make_case(uue_invalid)
        make_case(uue_portsc)
        make_case(uue_address)
        make_case(uue_configure)
        make_case(uue_deconfigure)
        make_case(uue_hw_failed)
        make_case(uue_udc_stop)            
        }

        return "unknown";
}           

char*
vudc_string_sspms(
    int sspms
    )
{
        switch (sspms) {
        make_case(SSPMS_U3H)
        make_case(SSPMS_U3D) 
        make_case(SSPMS_DBC)
        }

        return "unknown";
}

char*
vudc_string_sspmss(
    int sspmss
    )
{
        switch (sspmss) {
        make_case(SSPMSS_U3H)
        make_case(SSPMSS_U3D) 
        make_case(SSPMSS_DBC)
        }

        return "unknown";
}

char*
vudc_string_nrdy_entry_type(
    int nrdy_entry_type
    )
{
        switch (nrdy_entry_type) {
        make_case(nrdy_entry_acktp)
        make_case(nrdy_entry_dp) 
        }

        return "unknown";
}

char*
vudc_string_upkterr(
    int upkterr
    )
{
        switch (upkterr) {
        make_case(upkt_err_no)
        make_case(upkt_err_tx) 
        make_case(upkt_err_sp)
        make_case(upkt_err_babble)
        }

        return "unknown";
}

char*
vudc_string_upktt(
    int upktt
    )
{
        switch (upktt) {
        make_case(upkt_type_imp)
        make_case(upkt_type_tp) 
        make_case(upkt_type_dph)
        make_case(upkt_type_itp)
        }

        return "unknown";
}

char*
vudc_string_upktsttp(
    int upktsttp
    )
{
        switch (upktsttp) {
        make_case(upkt_sttp_ack)
        make_case(upkt_sttp_nrdy) 
        make_case(upkt_sttp_erdy)
        make_case(upkt_sttp_status)
        make_case(upkt_sttp_stall)
        make_case(upkt_sttp_dev_notify) 
        make_case(upkt_sttp_ping)
        make_case(upkt_sttp_ping_res)
        }

        return "unknown";
}

void vudc_dump_uue_related_info(
        struct via_udc *vudc,
        u32 uue,
        u32 uue_para
        )
{
        if (uue == uue_portsc) {
                vu3d_info(
                        ">> %s %X %s %s %s %s %s %s %s %s %s\n", 
                        vudc_string_uue(uue), uue_para,
                        (uue_para & U3D_CCS) ? "CCS" : "",
                        (uue_para & U3D_PED) ? "PED" : "",
                        (uue_para & U3D_PR)  ? "PR"  : "",
                        (uue_para & U3D_CSC) ? "CSC" : "",
                        (uue_para & U3D_PRC) ? "PRC" : "",
                        (uue_para & U3D_PLC) ? "PLC" : "",
                        (uue_para & U3D_CEC) ? "CEC" : "",
                        vudc_string_pls(MBF_TO_VAL(uue_para, BS_U3D_PLS, BM_U3D_PLS)),
                        vudc_string_port_speed(MBF_TO_VAL(uue_para, BS_U3D_PORTSPEED, BM_U3D_PORTSPEED)) 
                        ); 
        }
        else {
                vu3d_info(
                        ">> %s %X\n", 
                        vudc_string_uue(uue), uue_para
                        ); 
        }
}

#ifdef VU3L_DBG

void vudc_dump_async(
        struct via_udc *vudc
        )
{
        struct via_ep *vep;
        struct usb_endpoint_instance *usbe;
        struct urb *urb;

        vep = vudc->async_vep;
        if (!vep_is_valid(vep)) {
                dtrap(false);
                return;
        }
        
        usbe = vep_get_usbe(vep);
        urb = vep_get_urb(vep);
        
        vep_dbg(
                "%s urb %p rcv_urb %p tx_urb %p sent %X last %X "\
                "buffer %p buffer_length %X actual_length %X\n", 
                vep->name, urb, usbe->rcv_urb, usbe->tx_urb, 
                usbe->sent, usbe->last, 
                urb->buffer, urb->buffer_length, urb->actual_length     
                );
}

void vudc_dump_upkt(
        struct via_udc *vudc,
        struct u3lite_packet *upkt
        )
{   
        u32 type;

        type = MBF_TO_VAL(upkt->dword[0], BS_TP_TYPE, BM_TP_TYPE);

        switch (type) {

        case upkt_type_imp: {
                
                
        } break;
        
        case upkt_type_tp: {

                u32 subtype;

                subtype = MBF_TO_VAL(upkt->dword[1], BS_TP_SUBTYPE, BM_TP_SUBTYPE);

                switch (subtype) {

                case upkt_sttp_ack: {
               
                        vu3d_info(               
                                "upkt %p %s %s addr %.2X cycle %.1X "\
                                "rty %.1X dir %.1X epnum %.1X he %.1X "\
                                "nump %.2X seqnum %.2X xferlen %.3X pp %.1X "\
                                "ctrlsts %.1X ctrl %.1X chain %.1X ioc %.1X xferaddr %.8X"\
                                "[%.8X %.8X %.8X %.8X]\n",                 
                                upkt,                                
                                vudc_string_upktt(MBF_TO_VAL(upkt->dword[0], BS_TP_TYPE, BM_TP_TYPE)),
                                vudc_string_upktsttp(MBF_TO_VAL(upkt->dword[1], BS_TP_SUBTYPE, BM_TP_SUBTYPE)),
                                MBF_TO_VAL(upkt->dword[0], BS_TP_DEVADDR, BM_TP_DEVADDR),
                                MBF_TO_VAL(upkt->dword[0], BS_TP_CYCLE, BM_TP_CYCLE),
                                MBF_TO_VAL(upkt->dword[1], BS_TP_RTY, BM_TP_RTY),
                                MBF_TO_VAL(upkt->dword[1], BS_TP_D, BM_TP_D),
                                MBF_TO_VAL(upkt->dword[1], BS_TP_EPTNUM, BM_TP_EPTNUM),
                                MBF_TO_VAL(upkt->dword[1], BS_TP_HE, BM_TP_HE),
                                MBF_TO_VAL(upkt->dword[1], BS_TP_NUMP, BM_TP_NUMP),
                                MBF_TO_VAL(upkt->dword[1], BS_TP_SEQNUM, BM_TP_SEQNUM),
                                MBF_TO_VAL(upkt->dword[2], BS_TP_XFER_LENGTH, BM_TP_XFER_LENGTH),
                                MBF_TO_VAL(upkt->dword[2], BS_TP_PP, BM_TP_PP),
                                MBF_TO_VAL(upkt->dword[2], BS_TP_CTRL_STATUS, BM_TP_CTRL_STATUS),
                                MBF_TO_VAL(upkt->dword[2], BS_TP_CTRL_EP, BM_TP_CTRL_EP),
                                MBF_TO_VAL(upkt->dword[2], BS_TP_CHAIN, BM_TP_CHAIN),
                                MBF_TO_VAL(upkt->dword[2], BS_TP_IOC, BM_TP_IOC),
                                MBF_TO_VAL(upkt->dword[3], BS_TP_XFER_ADDR, BM_TP_XFER_ADDR),
                                upkt->dword[0],
                                upkt->dword[1],
                                upkt->dword[2],
                                upkt->dword[3]
                                );
                } break;

                case upkt_sttp_erdy: {               

                        vu3d_info(               
                                "upkt %p %s %s addr %.2X cycle %.1X "\
                                "dir %.1X epnum %.1X nump %.2X "\
                                "[%.8X %.8X %.8X %.8X]\n",                 
                                upkt,
                                vudc_string_upktt(MBF_TO_VAL(upkt->dword[0], BS_TP_TYPE, BM_TP_TYPE)),
                                vudc_string_upktsttp(MBF_TO_VAL(upkt->dword[1], BS_TP_SUBTYPE, BM_TP_SUBTYPE)),                                
                                MBF_TO_VAL(upkt->dword[0], BS_TP_DEVADDR, BM_TP_DEVADDR),
                                MBF_TO_VAL(upkt->dword[0], BS_TP_CYCLE, BM_TP_CYCLE),
                                MBF_TO_VAL(upkt->dword[1], BS_TP_D, BM_TP_D),
                                MBF_TO_VAL(upkt->dword[1], BS_TP_EPTNUM, BM_TP_EPTNUM),
                                MBF_TO_VAL(upkt->dword[1], BS_TP_NUMP, BM_TP_NUMP),                                
                                upkt->dword[0],
                                upkt->dword[1],
                                upkt->dword[2],
                                upkt->dword[3]
                                );
                 } break;

                 case upkt_sttp_stall: {               
        
                        vu3d_info(               
                                "upkt %p %s %s addr %.2X cycle %.1X "\
                                "dir %.1X epnum %.1X ctrlsts %.1X "\
                                "ctrl %.1X chain %.1X ioc %.1X"\
                                "[%.8X %.8X %.8X %.8X]\n",                 
                                upkt,
                                vudc_string_upktt(MBF_TO_VAL(upkt->dword[0], BS_TP_TYPE, BM_TP_TYPE)),
                                vudc_string_upktsttp(MBF_TO_VAL(upkt->dword[1], BS_TP_SUBTYPE, BM_TP_SUBTYPE)), 
                                MBF_TO_VAL(upkt->dword[0], BS_TP_DEVADDR, BM_TP_DEVADDR),
                                MBF_TO_VAL(upkt->dword[0], BS_TP_CYCLE, BM_TP_CYCLE),
                                MBF_TO_VAL(upkt->dword[1], BS_TP_D, BM_TP_D),
                                MBF_TO_VAL(upkt->dword[1], BS_TP_EPTNUM, BM_TP_EPTNUM),
                                MBF_TO_VAL(upkt->dword[2], BS_TP_CTRL_STATUS, BM_TP_CTRL_STATUS),
                                MBF_TO_VAL(upkt->dword[2], BS_TP_CTRL_EP, BM_TP_CTRL_EP),
                                MBF_TO_VAL(upkt->dword[2], BS_TP_CHAIN, BM_TP_CHAIN),
                                MBF_TO_VAL(upkt->dword[2], BS_TP_IOC, BM_TP_IOC),                                
                                upkt->dword[0],
                                upkt->dword[1],
                                upkt->dword[2],
                                upkt->dword[3]
                                );
                 } break;

                 case upkt_sttp_dev_notify: {               

                        vu3d_info(               
                                "upkt %p %s %s addr %.2X cycle %.1X "\
                                "notify %.1X ns1 %.6X "\
                                "[%.8X %.8X %.8X %.8X]\n",                 
                                upkt,
                                vudc_string_upktt(MBF_TO_VAL(upkt->dword[0], BS_TP_TYPE, BM_TP_TYPE)),
                                vudc_string_upktsttp(MBF_TO_VAL(upkt->dword[1], BS_TP_SUBTYPE, BM_TP_SUBTYPE)),                                
                                MBF_TO_VAL(upkt->dword[0], BS_TP_DEVADDR, BM_TP_DEVADDR),
                                MBF_TO_VAL(upkt->dword[0], BS_TP_CYCLE, BM_TP_CYCLE),
                                MBF_TO_VAL(upkt->dword[1], BS_TP_NOTIFY_TYPE, BM_TP_NOTIFY_TYPE),
                                MBF_TO_VAL(upkt->dword[1], BS_TP_NOTIFY_TYPE_SPEC1, BM_TP_NOTIFY_TYPE_SPEC1),                          
                                upkt->dword[0],
                                upkt->dword[1],
                                upkt->dword[2],
                                upkt->dword[3]
                                );
                 } break;
                 
                 default: {
                        vu3d_info(               
                                "upkt %p [%.8X %.8X %.8X %.8X]\n",                 
                                upkt,
                                upkt->dword[0],
                                upkt->dword[1],
                                upkt->dword[2],
                                upkt->dword[3]
                                );
                 }

                 }
                
        } break;
        
        case upkt_type_dph: {

                vu3d_info(               
                        "upkt %p %s addr %.2X cycle %.1X "\
                        "seqnum %.2X eob %.1X dir %.1X epnum %.1X "\
                        "datalen %.3X pp %.1X ctrl %.1X "\
                        "chain %.1X ioc %.1X xferaddr %.8X"\
                        "[%.8X %.8X %.8X %.8X]\n",                 
                        upkt,
                        vudc_string_upktt(MBF_TO_VAL(upkt->dword[0], BS_TP_TYPE, BM_TP_TYPE)),
                        MBF_TO_VAL(upkt->dword[0], BS_DP_DEVADDR, BM_DP_DEVADDR),
                        MBF_TO_VAL(upkt->dword[0], BS_DP_CYCLE, BM_DP_CYCLE),
                        MBF_TO_VAL(upkt->dword[1], BS_DP_SEQNUM, BM_DP_SEQNUM),
                        MBF_TO_VAL(upkt->dword[1], BS_DP_EOB, BM_DP_EOB),
                        MBF_TO_VAL(upkt->dword[1], BS_DP_D, BM_DP_D),
                        MBF_TO_VAL(upkt->dword[1], BS_DP_EPTNUM, BM_DP_EPTNUM),
                        MBF_TO_VAL(upkt->dword[1], BS_DP_DATA_LENGTH, BM_DP_DATA_LENGTH),
                        MBF_TO_VAL(upkt->dword[2], BS_DP_PP, BM_DP_PP),
                        MBF_TO_VAL(upkt->dword[2], BS_DP_CTRLEP, BM_DP_CTRL_EP),
                        MBF_TO_VAL(upkt->dword[2], BS_DP_CHAIN, BM_DP_CHAIN),
                        MBF_TO_VAL(upkt->dword[2], BS_DP_IOC, BM_DP_IOC),
                        MBF_TO_VAL(upkt->dword[3], BS_DP_XFER_ADDR, BM_DP_XFER_ADDR),
                        upkt->dword[0],
                        upkt->dword[1],
                        upkt->dword[2],
                        upkt->dword[3]
                        );

        } break;
        
        case upkt_type_itp: {
                
                vu3d_info(               
                        "upkt %p [%.8X %.8X %.8X %.8X]\n",                 
                        upkt,
                        upkt->dword[0],
                        upkt->dword[1],
                        upkt->dword[2],
                        upkt->dword[3]
                        );
        } break;

        default: {
                
                vu3d_info(               
                        "upkt %p [%.8X %.8X %.8X %.8X]\n",                 
                        upkt,
                        upkt->dword[0],
                        upkt->dword[1],
                        upkt->dword[2],
                        upkt->dword[3]
                        );
        }
        
        }
}

void vudc_dump_upkt_on_range(
        struct via_udc *vudc, 
        struct u3lite_packet *upkt_start,
        struct u3lite_packet *upkt_end 
        )
{
        struct u3lite_packet *upkt;        
                                        
        upkt = upkt_start;
        
        do {                
                vudc_dump_upkt(vudc, upkt);
                upkt = vudc_query_next_upkt(vudc, upkt);
        } while (upkt != upkt_end);
}

void vudc_dump_otg_en(
        struct via_udc *vudc
        )
{   
        u16 otg_en;
         
        otg_en = vu3l_read16(&vudc->gbl_reg->otg_en);   

        vu3d_dbg(
                "RX%.3X %.22s %.4X u3h_en %.1X u3c_en %.1X\n", 
                REG_DUMP_VALUE(otg_en),              
                (otg_en & U3H_EN) ? 1 : 0,
                (otg_en & U3DC_EN) ? 1 : 0
                );  

}

void vudc_dump_port1_dir(
        struct via_udc *vudc
        )
{   
        u8 port1_dir;
         
        port1_dir = vu3l_read8(&vudc->gbl_reg->port1_dir);   
        
        vu3d_dbg(
                "RX%.3X %.22s %.2X %s %s\n", 
                REG_DUMP_VALUE(port1_dir),
                vudc_string_sspms(MBF_TO_VAL(port1_dir, BS_SS1_PORT_MODSEL, BM_SS1_PORT_MODSEL)),
                vudc_string_sspmss(MBF_TO_VAL(port1_dir, BS_SS1_PORT_MODSTS, BM_SS1_PORT_MODSTS))   
                );        
}

void vudc_dump_port1_phy(
        struct via_udc *vudc
        )
{   
        u16 port1_phy;
         
        port1_phy = vu3l_read16(&vudc->uhc_reg->port1_phy);   

        vu3d_dbg(
                "RX%.3X %.22s %.4X r_updisen_ss1 %.1X r_ufpdeten_ss1 %.1X\n", 
                REG_DUMP_VALUE(port1_phy),
                (port1_phy & R_UPDISEN_SS1) ? 1 : 0,
                (port1_phy & R_UFPDETEN_SS1) ? 1 : 0
                ); 

}

void vudc_dump_port1_feature(
        struct via_udc *vudc
        )
{   
        u32 port1_feature;
         
        port1_feature = vu3l_read32(&vudc->uhc_reg->port1_feature);   
     
        vu3d_dbg(
                "RX%.3X %.22s %.4X ltssmdis_exiten_ss1 %.1X r_dir_rst_ss1 %.1X\n", 
                REG_DUMP_VALUE(port1_feature),
                (port1_feature & R_LTSSMDIS_EXITEN_SS1) ? 1 : 0,
                (port1_feature & R_DIR_RST_SS1) ? 1 : 0
                ); 
}

void vudc_dump_udc_inten(
        struct via_udc *vudc
        )
{   
        u8 udc_inten;
         
        udc_inten = vu3l_read8(&vudc->udc_reg->udc_inten);   
        
        vu3d_dbg(
                "RX%.3X %.22s %.2X portrst %.1X dcechg %.1X ccschg %.1X portsc %.1X\n", 
                REG_DUMP_VALUE(udc_inten),
                (udc_inten & DBC_PORTRST_INTE) ? 1 : 0,
                (udc_inten & DBC_DCECHG_INTE) ? 1 : 0,
                (udc_inten & CCSCHG_INTE) ? 1 : 0,
                (udc_inten & U3D_PORTSC_INTE) ? 1 : 0
                );
}

void vudc_dump_ep0_inten(
        struct via_udc *vudc
        )
{   
        u8 ep0_inten;
         
        ep0_inten = vu3l_read8(&vudc->udc_reg->ep0_inten);   

        vu3d_dbg(
                "RX%.3X %.22s %.2X rxsetup %.1X\n", 
                REG_DUMP_VALUE(ep0_inten),
                (ep0_inten & CTLEP_RXSETUP_INTE) ? 1 : 0
                );
}

void vudc_dump_udc_op(
        struct via_udc *vudc
        )
{   
        u16 udc_op;
         
        udc_op = vu3l_read16(&vudc->udc_reg->udc_op);   
        
        vu3d_dbg(
                "RX%.3X %.22s %.4X run %.1X rst %.1X Addr %.2X\n", 
                REG_DUMP_VALUE(udc_op),
                (udc_op & U3D_RUN) ? 1 : 0,
                (udc_op & U3D_RST_FW) ? 1 : 0,
                (MBF_TO_VAL(udc_op, BS_DBC_DEVADDR, BM_DBC_DEVADDR))
                ); 
}

void vudc_dump_udc_sts(
        struct via_udc *vudc
        )
{   
        u8 udc_sts;
         
        udc_sts = vu3l_read8(&vudc->udc_reg->udc_sts);   

        vu3d_dbg(
                "RX%.3X %.22s %.2X portrst %.1X dcechg %.1X ccschg %.1X\n", 
                REG_DUMP_VALUE(udc_sts),
                (udc_sts & DBC_PORTRST) ? 1 : 0,
                (udc_sts & DBC_DCECHG) ? 1 : 0,
                (udc_sts & DBC_CCS_CHG) ? 1 : 0
                );       
}

void vudc_dump_ep0_sts(
        struct via_udc *vudc
        )
{   
        u8 ep0_sts;
         
        ep0_sts = vu3l_read8(&vudc->udc_reg->ep0_sts);   

        vu3d_dbg(
                "RX%.3X %.22s %.2X setupout %.1X setupin %.1X\n", 
                REG_DUMP_VALUE(ep0_sts),
                (ep0_sts & CTLEP_RXSETUPOUT_STS) ? 1 : 0,
                (ep0_sts & CTLEP_RXSETUPIN_STS) ? 1 : 0
                );  
}

void vudc_dump_fwdislink(
        struct via_udc *vudc
        )
{   
        u8 fwdislink;
         
        fwdislink = vu3l_read8(&vudc->udc_reg->fwdislink);   
        
        vu3d_dbg(
                "RX%.3X %.22s %.2X dislink_en %.1X\n", 
                REG_DUMP_VALUE(fwdislink),
                (fwdislink & R_FWDISLINK_EN) ? 1 : 0
                ); 
}

void vudc_dump_db_sts(
        struct via_udc *vudc
        )
{   
        u8 db_sts;
         
        db_sts = vu3l_read8(&vudc->udc_reg->db_sts);   

        vu3d_dbg(
                "RX%.3X %.22s %.2X blkin_db %.1X blkout_db %.1X blkin_hit %.1X blkout_hit %.1X\n", 
                REG_DUMP_VALUE(db_sts),
                (db_sts & BLKIN_DB_STS) ? 1 : 0,
                (db_sts & BLKOUT_DB_STS) ? 1 : 0,
                (db_sts & BLKIN_HIT_STS) ? 1 : 0,
                (db_sts & BLKOUT_HOT_STS) ? 1 : 0
                );  
}

void vudc_dump_async_inten(
        struct via_udc *vudc
        )
{   
        u16 async_inten;
         
        async_inten = vu3l_read16(&vudc->udc_reg->async_inten);   

        vu3d_dbg(
                "RX%.3X %.22s %.4X en %.1X inte %.1X pktcmpl %.1X pkterr %.1X nrdy %.1X\n", 
                REG_DUMP_VALUE(async_inten),
                (async_inten & ASYNCEP_EN) ? 1 : 0,
                (async_inten & ASYNCEP_INTE) ? 1 : 0,
                (async_inten & ASYNCEP_PKTCMPL_INTE) ? 1 : 0,
                (async_inten & ASYNCEP_PKTERR_INTE) ? 1 : 0,
                (async_inten & NRDY_ENTRY_INTE) ? 1 : 0
                );
}

void vudc_dump_async_pkt_read_ptr(
        struct via_udc *vudc
        )
{   
        u16 async_pkt_read_ptr;
         
        async_pkt_read_ptr = vu3l_read16(&vudc->udc_reg->async_pkt_read_ptr);   

        vu3d_dbg(
                "RX%.3X %.22s %.4X async_rptr %.2X\n", 
                REG_DUMP_VALUE(async_pkt_read_ptr),
                MBF_TO_VAL(async_pkt_read_ptr, BS_ASYEP_RPTR, BM_ASYEP_RPTR)
                );
}

void vudc_dump_async_pkt_base_ptr(
        struct via_udc *vudc
        )
{   
        u32 async_pkt_base_ptr;
         
        async_pkt_base_ptr = vu3l_read32(&vudc->udc_reg->async_pkt_base_ptr);   

        vu3d_dbg(
                "RX%.3X %.22s %.8X\n", 
                REG_DUMP_VALUE(async_pkt_base_ptr)
                );
}

void vudc_dump_async_pkt_sts(
        struct via_udc *vudc
        )
{   
        u8 async_pkt_sts;
         
        async_pkt_sts = vu3l_read8(&vudc->udc_reg->async_pkt_sts);   

        vu3d_dbg(
                "RX%.3X %.22s %.2X rdy_sts %.1X\n", 
                REG_DUMP_VALUE(async_pkt_sts),
               (async_pkt_sts & ASYEP_PKT_RDY_STS) ? 1 : 0
                );
}

void vudc_dump_async_intsts(
        struct via_udc *vudc
        )
{   
        u16 async_intsts;
         
        async_intsts = vu3l_read16(&vudc->udc_reg->async_intsts);   

        vu3d_dbg(
                "RX%.3X %.22s %.4X crptr %.2X %s pkterr %.1X pktcmpl %.1X\n", 
                REG_DUMP_VALUE(async_intsts),
                MBF_TO_VAL(async_intsts, BS_ASYEP_PKT_CRPTR, BM_ASYEP_PKT_CRPTR),
                vudc_string_upkterr(MBF_TO_VAL(async_intsts, BS_ASYEP_PKT_ERR_TYPE, BM_ASYEP_PKT_ERR_TYPE)),
                (async_intsts & ASYEP_PKTERR_STS) ? 1 : 0,
                (async_intsts & ASYEP_PKTCMPL_STS) ? 1 : 0
                );
}

void vudc_dump_async_rx_dp_length(
        struct via_udc *vudc
        )
{   
        u16 async_rx_dp_length;
         
        async_rx_dp_length = vu3l_read16(&vudc->udc_reg->async_rx_dp_length);   

        vu3d_dbg(
                "RX%.3X %.22s %.4X dplen %.3X\n", 
                REG_DUMP_VALUE(async_rx_dp_length),
                MBF_TO_VAL(async_rx_dp_length, BS_ASYEP_PKT_DPLEN, BM_ASYEP_PKT_DPLEN)
                );
}

void vudc_dump_nrdy_entry(
        struct via_udc *vudc,
        u8 index
        )
{   
        u16 nrdy_entry;

        trap(index < NUM_NRDY_ENTRY);
        nrdy_entry = vu3l_read16(&vudc->udc_reg->nrdy_entry[index]);   

        vu3d_dbg(
                "RX%.3X %.22s %.4X seqnum %.2X %s epnum %.1X\n", 
                REG_DUMP_VALUE(nrdy_entry),
                MBF_TO_VAL(nrdy_entry, BS_NRDY_SEQNUM, BM_NRDY_SEQNUM),
                vudc_string_nrdy_entry_type(MBF_TO_VAL(nrdy_entry, BS_NRDY_TYPE, BM_NRDY_TYPE)),
                MBF_TO_VAL(nrdy_entry, BS_NRDY_EPNUM, BM_NRDY_EPNUM)
                );
}

void vudc_dump_tt_sts(
        struct via_udc *vudc
        )
{   
        u32 tt_sts;
         
        tt_sts = vu3l_read32(&vudc->udc_reg->tt_sts);   

        vu3d_dbg(
                "RX%.3X %.22s %.4X tm %.1X\n", 
                REG_DUMP_VALUE(tt_sts),
                (tt_sts & TICKTMINTSTS) ? 1 : 0
                );  
}

void vudc_dump_portsc_option(
        struct via_udc *vudc
        )
{   
        u32 portsc_option;
         
        portsc_option = vu3l_read32(&vudc->udc_reg->portsc_option);   
        
        vu3d_dbg(
                "RX%.3X %.22s %.8X dislink_en %.1X\n", 
                REG_DUMP_VALUE(portsc_option),
                (portsc_option & R_FWDISLINK_EN) ? 1 : 0
                ); 
}

void vudc_dump_portsc(
        struct via_udc *vudc
        )
{   
        u32 portsc;
         
        portsc = vu3l_read32(&vudc->udc_reg->portsc);   

        vu3d_dbg(
                "RX%.3X %.22s %.8X ccs %.1X ped %.1X pr %.1X csc %.1X prc %.1X plc %.1X cec %.1X %s %s\n", 
                REG_DUMP_VALUE(portsc),
                (portsc & U3D_CCS) ? 1 : 0,
                (portsc & U3D_PED) ? 1 : 0,
                (portsc & U3D_PR) ? 1 : 0,
                (portsc & U3D_CSC) ? 1 : 0,
                (portsc & U3D_PRC) ? 1 : 0,
                (portsc & U3D_PLC) ? 1 : 0,
                (portsc & U3D_CEC) ? 1 : 0,
                vudc_string_pls(MBF_TO_VAL(portsc, BS_U3D_PLS, BM_U3D_PLS)),
                vudc_string_port_speed(MBF_TO_VAL(portsc, BS_U3D_PORTSPEED, BM_U3D_PORTSPEED)) 
                );  
}

void vudc_dump_nrdy_valid(
        struct via_udc *vudc
        )
{   
        u8 nrdy_valid;
         
        nrdy_valid = vu3l_read8(&vudc->udc_reg->nrdy_valid);   

        vu3d_dbg(
                "RX%.3X %.22s %.2X %.1X.%.1X.%.1X.%.1X.%.1X.%.1X.%.1X.%.1X\n", 
                REG_DUMP_VALUE(nrdy_valid),
                (nrdy_valid & 0x01) ? 1 : 0,
                (nrdy_valid & 0x02) ? 1 : 0,
                (nrdy_valid & 0x04) ? 1 : 0,
                (nrdy_valid & 0x08) ? 1 : 0,
                (nrdy_valid & 0x10) ? 1 : 0,
                (nrdy_valid & 0x20) ? 1 : 0,
                (nrdy_valid & 0x40) ? 1 : 0,
                (nrdy_valid & 0x80) ? 1 : 0
                );  
}

void vudc_dump_fifo_active(
        struct via_udc *vudc
        )
{   
        u8 fifo_active;
         
        fifo_active = vu3l_read8(&vudc->udc_reg->fifo_active);   

        vu3d_dbg(
                "RX%.3X %.22s %.2X %.1X.%.1X.%.1X.%.1X.%.1X.%.1X.%.1X.%.1X\n", 
                REG_DUMP_VALUE(fifo_active),
                (fifo_active & 0x01) ? 1 : 0,
                (fifo_active & 0x02) ? 1 : 0,
                (fifo_active & 0x04) ? 1 : 0,
                (fifo_active & 0x08) ? 1 : 0,
                (fifo_active & 0x10) ? 1 : 0,
                (fifo_active & 0x20) ? 1 : 0,
                (fifo_active & 0x40) ? 1 : 0,
                (fifo_active & 0x80) ? 1 : 0
                );  
}

void vudc_dump_udc_intsts(
        struct via_udc *vudc
        )
{   
        u32 udc_intsts;
         
        udc_intsts = vu3l_read32(&vudc->udc_reg->udc_intsts);   
        
        vu3d_dbg(
                "RX%.3X %.22s %.8X u3d %.1X async %.1X period %.1X setup %.1X nrdy %.1X tm %.1X psc %.1X\n", 
                REG_DUMP_VALUE(udc_intsts),
                (udc_intsts & DBC_INTSTS) ? 1 : 0,
                (udc_intsts & ASYNC_INTSTS) ? 1 : 0,
                (udc_intsts & PER_INTSTS) ? 1 : 0,
                (udc_intsts & CTLEP_RXSETUP_INTSTS) ? 1 : 0,
                (udc_intsts & NRDY_INTSTS) ? 1 : 0,
                (udc_intsts & TICKTM_INTSTS) ? 1 : 0,
                (udc_intsts & PSCEG_INTSTS) ? 1 : 0
                );        
}

void vudc_dump_intsts(
        struct via_udc *vudc
        )
{   
        vudc_dump_udc_sts(vudc);
        vudc_dump_ep0_sts(vudc);
        vudc_dump_db_sts(vudc);
        vudc_dump_async_intsts(vudc);
        vudc_dump_tt_sts(vudc);
        vudc_dump_portsc(vudc);
        vudc_dump_nrdy_valid(vudc);
        vudc_dump_fifo_active(vudc);
        vudc_dump_udc_intsts(vudc);
}

void vudc_dump_async_io(
        struct via_udc *vudc
        )
{     
        u8 i;
        
        vudc_dump_async_inten(vudc);    
        vudc_dump_async_pkt_read_ptr(vudc);        
        vudc_dump_async_pkt_base_ptr(vudc); 
        vudc_dump_async_pkt_sts(vudc); 
        vudc_dump_async_intsts(vudc);
        vudc_dump_async_rx_dp_length(vudc);
        for (i = 0; i < NUM_NRDY_ENTRY; i++) 
                vudc_dump_nrdy_entry(vudc, i);
}

void vudc_dump_udc_hw(
        struct via_udc *vudc
        )
{   
        vudc_dump_otg_en(vudc);
        vudc_dump_port1_dir(vudc); 
        vudc_dump_port1_phy(vudc);
        vudc_dump_port1_feature(vudc);
        vudc_dump_udc_inten(vudc); 
        vudc_dump_ep0_inten(vudc);
        vudc_dump_udc_op(vudc);
        vudc_dump_fwdislink(vudc);
        vudc_dump_async_inten(vudc);    
        vudc_dump_async_pkt_read_ptr(vudc);        
        vudc_dump_async_pkt_base_ptr(vudc);        
        vudc_dump_portsc_option(vudc);           
}

void vudc_dump_uhc_register(
        struct via_udc *vudc
        )
{   
        vu3d_dbg("dcctrl                   %p %X\n", &vudc->uhc_reg->dcctrl, vu3l_read32(&vudc->uhc_reg->dcctrl));        
        vu3d_dbg("port1_phy                %p %X\n", &vudc->uhc_reg->port1_phy, vu3l_read16(&vudc->uhc_reg->port1_phy));
        vu3d_dbg("port1_feature            %p %X\n", &vudc->uhc_reg->port1_feature, vu3l_read32(&vudc->uhc_reg->port1_feature));        
}

void vudc_dump_u3d_register(
        struct via_udc *vudc
        )
{   
        vu3d_dbg("udc_inten                %p %X\n", &vudc->udc_reg->udc_inten, vu3l_read8(&vudc->udc_reg->udc_inten));
        vu3d_dbg("ep0_inten                %p %X\n", &vudc->udc_reg->ep0_inten, vu3l_read8(&vudc->udc_reg->ep0_inten));
        vu3d_dbg("udc_op                   %p %X\n", &vudc->udc_reg->udc_op, vu3l_read16(&vudc->udc_reg->udc_op));
        vu3d_dbg("udc_sts                  %p %X\n", &vudc->udc_reg->udc_sts, vu3l_read8(&vudc->udc_reg->udc_sts));
        vu3d_dbg("ep0_sts                  %p %X\n", &vudc->udc_reg->ep0_sts, vu3l_read8(&vudc->udc_reg->ep0_sts));
        vu3d_dbg("ctrl_pkt_buf             %p %X\n", &vudc->udc_reg->ctrl_pkt_buf[0], vu3l_read8(&vudc->udc_reg->ctrl_pkt_buf[0]));
        vu3d_dbg("db_inten                 %p %X\n", &vudc->udc_reg->db_inten, vu3l_read8(&vudc->udc_reg->db_inten));
        vu3d_dbg("idle_sts                 %p %X\n", &vudc->udc_reg->idle_sts, vu3l_read8(&vudc->udc_reg->idle_sts));
        vu3d_dbg("fwdislink                %p %X\n", &vudc->udc_reg->fwdislink, vu3l_read8(&vudc->udc_reg->fwdislink));
        vu3d_dbg("db_sts                   %p %X\n", &vudc->udc_reg->db_sts, vu3l_read8(&vudc->udc_reg->db_sts));
        vu3d_dbg("timeout_en               %p %X\n", &vudc->udc_reg->timeout_en, vu3l_read8(&vudc->udc_reg->timeout_en));
        vu3d_dbg("timeout_parm             %p %X\n", &vudc->udc_reg->timeout_parm, vu3l_read8(&vudc->udc_reg->timeout_parm));
        vu3d_dbg("async_inten              %p %X\n", &vudc->udc_reg->async_inten, vu3l_read16(&vudc->udc_reg->async_inten));
        vu3d_dbg("async_pkt_read_ptr       %p %X\n", &vudc->udc_reg->async_pkt_read_ptr, vu3l_read16(&vudc->udc_reg->async_pkt_read_ptr));
        vu3d_dbg("async_pkt_base_ptr       %p %X\n", &vudc->udc_reg->async_pkt_base_ptr, vu3l_read32(&vudc->udc_reg->async_pkt_base_ptr));
        vu3d_dbg("async_pkt_sts            %p %X\n", &vudc->udc_reg->async_pkt_sts, vu3l_read8(&vudc->udc_reg->async_pkt_sts));
        vu3d_dbg("async_intsts             %p %X\n", &vudc->udc_reg->async_intsts, vu3l_read16(&vudc->udc_reg->async_intsts));
        vu3d_dbg("async_rx_dp_length       %p %X\n", &vudc->udc_reg->async_rx_dp_length, vu3l_read16(&vudc->udc_reg->async_rx_dp_length));
        vu3d_dbg("nrdy_entry               %p %X\n", &vudc->udc_reg->nrdy_entry[0], vu3l_read16(&vudc->udc_reg->nrdy_entry[0]));
        vu3d_dbg("period_inten             %p %X\n", &vudc->udc_reg->period_inten, vu3l_read16(&vudc->udc_reg->period_inten));
        vu3d_dbg("intep_pkt_rptr           %p %X\n", &vudc->udc_reg->intep_pkt_rptr, vu3l_read32(&vudc->udc_reg->intep_pkt_rptr));
        vu3d_dbg("isoep_pkt_base           %p %X\n", &vudc->udc_reg->isoep_pkt_base, vu3l_read32(&vudc->udc_reg->isoep_pkt_base));
        vu3d_dbg("tick_time_ctrl           %p %X\n", &vudc->udc_reg->tick_time_ctrl, vu3l_read16(&vudc->udc_reg->tick_time_ctrl));
        vu3d_dbg("intep_pkt_base           %p %X\n", &vudc->udc_reg->intep_pkt_base, vu3l_read32(&vudc->udc_reg->intep_pkt_base));
        vu3d_dbg("tt_sts                   %p %X\n", &vudc->udc_reg->tt_sts, vu3l_read32(&vudc->udc_reg->tt_sts));
        vu3d_dbg("period_intsts_fifo0      %p %X\n", &vudc->udc_reg->period_intsts_fifo0, vu3l_read32(&vudc->udc_reg->period_intsts_fifo0));
        vu3d_dbg("int_pkt_sts              %p %X\n", &vudc->udc_reg->int_pkt_sts, vu3l_read32(&vudc->udc_reg->int_pkt_sts));
        vu3d_dbg("iso_map_entry0           %p %X\n", &vudc->udc_reg->iso_map_entry0, vu3l_read32(&vudc->udc_reg->iso_map_entry0));
        vu3d_dbg("bus_interval_adj         %p %X\n", &vudc->udc_reg->bus_interval_adj, vu3l_read32(&vudc->udc_reg->bus_interval_adj));
        vu3d_dbg("iso_timestamp            %p %X\n", &vudc->udc_reg->iso_timestamp, vu3l_read32(&vudc->udc_reg->iso_timestamp));
        vu3d_dbg("portsc                   %p %X\n", &vudc->udc_reg->portsc, vu3l_read32(&vudc->udc_reg->portsc));
        vu3d_dbg("portsc_option            %p %X\n", &vudc->udc_reg->portsc_option, vu3l_read32(&vudc->udc_reg->portsc_option));
        vu3d_dbg("nrdy_valid               %p %X\n", &vudc->udc_reg->nrdy_valid, vu3l_read8(&vudc->udc_reg->nrdy_valid));
        vu3d_dbg("fifo_active              %p %X\n", &vudc->udc_reg->fifo_active, vu3l_read8(&vudc->udc_reg->fifo_active));
        vu3d_dbg("udc_intsts               %p %X\n", &vudc->udc_reg->udc_intsts, vu3l_read32(&vudc->udc_reg->udc_intsts));        
}

void vudc_dump_gbl_register(
        struct via_udc *vudc
        )
{   
        vu3d_dbg("otg_en                   %p %X\n", &vudc->gbl_reg->otg_en, vu3l_read16(&vudc->gbl_reg->otg_en));
        vu3d_dbg("port1_dir                %p %X\n", &vudc->gbl_reg->port1_dir, vu3l_read8(&vudc->gbl_reg->port1_dir));
        vu3d_dbg("port2_dir                %p %X\n", &vudc->gbl_reg->port2_dir, vu3l_read8(&vudc->gbl_reg->port2_dir));
        vu3d_dbg("port_owner               %p %X\n", &vudc->gbl_reg->port_owner, vu3l_read8(&vudc->gbl_reg->port_owner));
}

#endif
