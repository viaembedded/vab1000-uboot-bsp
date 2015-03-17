/*
 * via u3lite udc - endpoint header.
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

#ifndef _U3DC_EP_H_
#define _U3DC_EP_H_

#include "vu3d_precomp.h"

#define SIGNATURE_WEP           (cpu_to_be32((uintptr_t)"vep "))

#define BYTE_VEP_NAME           (16)

#define NUM_VEP                 (3)     /* ep0(1) + bulk(2) */

typedef void fn_ep_build_upkt(struct via_ep *vep);
typedef void fn_ep_irq_async_xfer(struct via_ep *vep, u8 errtype, u16 dp_length, u8 upkt_num);
typedef void fn_ep_irq_nrdy(struct via_ep *vep, u16 nrdy);
        
struct via_ep_info {
        const char *name;
        u16 maxpacket;
        fn_ep_build_upkt *build_upkt;
        fn_ep_irq_async_xfer *irq_async_xfer;
        fn_ep_irq_nrdy *irq_nrdy;
};

struct via_ep {

        u32 signature;
        
        char name[BYTE_VEP_NAME];
        
        /* relations */
        struct via_udc *vudc;       
        struct usb_endpoint_instance* usbe;                                        

        /* i/o callbacks */
        fn_ep_build_upkt *build_upkt;
        fn_ep_irq_async_xfer *irq_async_xfer;
        fn_ep_irq_nrdy *irq_nrdy;        
        
        /* assigned ep addresss */       
        u8 epnum; 
        u8 dir_in;

        /* async */
        u16 nrdy;
        unsigned int async_length;
        
        struct urb *urb;         
        struct usb_device_request *urb_setup;                        
        
        /* flag */
        u32 is_nrdy_pending     : 1;
        u32 is_udc_write        : 1;
        u32 is_inflight         : 1;
        u32 is_stall            : 1;
        u32 is_initialized      : 1;
};

bool vep_nuke_async(
        struct via_ep *vep
        );


void epbi_endpoint_write(
        struct via_ep *vep,
        u16 nrdy
        );       

void ep0_irq_setup_pkt(
        struct via_ep *vep
        );

void vep_setup(
        struct via_ep *vep,
        struct via_udc *vudc,
        unsigned int index,
        struct usb_endpoint_instance *usbe        
        );

static inline bool vep_is_valid(
        struct via_ep *vep
        )
{
        return (vep && (vep->signature == SIGNATURE_WEP));
}

static inline bool vep_is_initialized(
        struct via_ep *vep
        )
{                
        return (bool)vep->is_initialized;
}

static inline struct via_udc* vep_get_vudc(
        struct via_ep *vep
        )
{
        return vep->vudc;
}

static inline char* vep_get_name(
        struct via_ep *vep
        )
{                
        return vep->name;
}

static inline struct usb_endpoint_instance* vep_get_usbe(
        struct via_ep *vep
        )
{
        return vep->usbe;
} 

static inline struct urb * vep_get_urb(
        struct via_ep *vep
        )
{
        return vep->urb;
} 

static inline u8 vep_get_epnum(
        struct via_ep *vep
        )
{
        return vep->epnum;
}

static inline u8 vep_get_dir_in(
        struct via_ep *vep
        )
{
        return vep->dir_in;
}       

#endif
