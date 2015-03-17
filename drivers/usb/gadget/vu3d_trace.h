/*
 * via u3lite udc - trace header
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

#ifndef __U3DC_TRACE_H
#define __U3DC_TRACE_H

#include "vu3d_precomp.h"

char*
vudc_string_pls(
    int pls
    );

char*
vudc_string_port_speed(
    int pls
    );
   
char*
vudc_string_uus(
    int uus
    );

char*
vudc_string_uue(
    int uue
    );

char*
vudc_string_sspms(
    int sspms
    );

char*
vudc_string_sspmss(
    int sspmss
    );

char*
vudc_string_nrdy_entry_type(
    int nrdy_entry_type
    );

char*
vudc_string_upkterr(
    int upkterr
    );

char*
vudc_string_upktt(
    int upktt
    );

char*
vudc_string_upktsttp(
    int upktsttp
    );   

void vudc_dump_uue_related_info(
        struct via_udc *vudc,
        u32 uue,
        u32 uue_para
        );

#ifdef VU3L_DBG

void vudc_dump_async(
        struct via_udc *vudc
        );

void vudc_dump_upkt(
        struct via_udc *vudc,
        struct u3lite_packet *upkt
        );

void vudc_dump_upkt_on_range(
        struct via_udc *vudc, 
        struct u3lite_packet *upkt_start,
        struct u3lite_packet *upkt_end 
        );

void vudc_dump_otg_en(
        struct via_udc *vudc
        );

void vudc_dump_port1_dir(
        struct via_udc *vudc
        );

void vudc_dump_port1_phy(
        struct via_udc *vudc
        );

void vudc_dump_port1_feature(
        struct via_udc *vudc
        );

void vudc_dump_udc_inten(
        struct via_udc *vudc
        );

void vudc_dump_ep0_inten(
        struct via_udc *vudc
        );

void vudc_dump_udc_op(
        struct via_udc *vudc
        );

void vudc_dump_udc_sts(
        struct via_udc *vudc
        );

void vudc_dump_ep0_sts(
        struct via_udc *vudc
        );

void vudc_dump_fwdislink(
        struct via_udc *vudc
        );

void vudc_dump_db_sts(
        struct via_udc *vudc
        );

void vudc_dump_async_inten(
        struct via_udc *vudc
        );

void vudc_dump_async_pkt_read_ptr(
        struct via_udc *vudc
        );

void vudc_dump_async_pkt_base_ptr(
        struct via_udc *vudc
        );
        
void vudc_dump_async_pkt_sts(
        struct via_udc *vudc
        );

void vudc_dump_async_intsts(
        struct via_udc *vudc
        );

void vudc_dump_async_rx_dp_length(
        struct via_udc *vudc
        );

void vudc_dump_nrdy_entry(
        struct via_udc *vudc,
        u8 index
        );

void vudc_dump_tt_sts(
        struct via_udc *vudc
        );

void vudc_dump_portsc_option(
        struct via_udc *vudc
        );

void vudc_dump_portsc(
        struct via_udc *vudc
        );

void vudc_dump_nrdy_valid(
        struct via_udc *vudc
        );

void vudc_dump_fifo_active(
        struct via_udc *vudc
        );

void vudc_dump_udc_intsts(
        struct via_udc *vudc
        );

void vudc_dump_intsts(
        struct via_udc *vudc
        );

void vudc_dump_async_io(
        struct via_udc *vudc
        );

void vudc_dump_udc_hw(
        struct via_udc *vudc
        );

void vudc_dump_uhc_register(
        struct via_udc *vudc
        );

void vudc_dump_u3d_register(
        struct via_udc *vudc
        );

void vudc_dump_gbl_register(
        struct via_udc *vudc
        );

#else

#define vudc_dump_async(vep, ...)                do{}while(0) 
#define vudc_dump_upkt(vudc, ...)               do{}while(0) 
#define vudc_dump_upkt_on_range(vudc, ...)      do{}while(0) 
#define vudc_dump_otg_en(vudc, ...)             do{}while(0) 
#define vudc_dump_port1_dir(vudc, ...)          do{}while(0) 
#define vudc_dump_port1_phy(vudc, ...)          do{}while(0) 
#define vudc_dump_port1_feature(vudc, ...)      do{}while(0) 
#define vudc_dump_udc_inten(vudc, ...)          do{}while(0) 
#define vudc_dump_ep0_inten(vudc, ...)          do{}while(0) 
#define vudc_dump_udc_op(vudc, ...)             do{}while(0) 
#define vudc_dump_udc_sts(vudc, ...)            do{}while(0) 
#define vudc_dump_ep0_sts(vudc, ...)            do{}while(0) 
#define vudc_dump_fwdislink(vudc, ...)          do{}while(0) 
#define vudc_dump_db_sts(vudc, ...)             do{}while(0) 
#define vudc_dump_async_inten(vudc, ...)        do{}while(0) 
#define vudc_dump_async_pkt_read_ptr(vudc, ...) do{}while(0) 
#define vudc_dump_async_pkt_base_ptr(vudc, ...) do{}while(0)
#define vudc_dump_async_pkt_sts(vudc, ...)      do{}while(0) 
#define vudc_dump_async_intsts(vudc, ...)       do{}while(0) 
#define vudc_dump_async_rx_dp_length(vudc, ...) do{}while(0) 
#define vudc_dump_nrdy_entry(vudc, ...)         do{}while(0) 
#define vudc_dump_tt_sts(vudc, ...)             do{}while(0) 
#define vudc_dump_portsc_option(vudc, ...)      do{}while(0) 
#define vudc_dump_portsc(vudc, ...)             do{}while(0) 
#define vudc_dump_nrdy_valid(vudc, ...)         do{}while(0) 
#define vudc_dump_fifo_active(vudc, ...)        do{}while(0) 
#define vudc_dump_udc_intsts(vudc, ...)         do{}while(0) 
#define vudc_dump_intsts(vudc, ...)             do{}while(0) 
#define vudc_dump_async_io(vudc, ...)           do{}while(0) 
#define vudc_dump_udc_hw(vudc, ...)             do{}while(0) 
#define vudc_dump_uhc_register(vudc, ...)       do{}while(0)
#define vudc_dump_u3d_register(vudc, ...)       do{}while(0) 
#define vudc_dump_gbl_register(vudc, ...)       do{}while(0) 

#endif

#endif
