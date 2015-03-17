/*
 * via elite 2k udc(s) - library
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

/* update code when u2d udc driver is ready to integrate. */
#include "vu2d_lib.h"
#include "vu3d_lib.h"

/* fbt api */
fbt_reinit_instance(int speed);

#define DRIVER_DESC             "Via Elite2K Fastboot UDC Driver"
#define DRIVER_VERSION          "1.00B1"

/* default speed is SUPER which equal to fastboot */
static int udc_speed = USB_SPEED_HIGH;

void udc_update_speed(
        int speed
        )
{       
        /* update udc_speed which be used to determine which 
         * udc is operating transfer. 
         */
        udc_speed = speed;  

        /* reinit fastboot setting related with speed */
        fbt_reinit_instance(speed);
}

int udc_endpoint_write(
        struct usb_endpoint_instance *endpoint
        )
{    
        return (udc_speed == USB_SPEED_SUPER) ? 
                u3d_endpoint_write(endpoint) :
                u2d_endpoint_write(endpoint);
}

void udc_setup_ep(
        struct usb_device_instance *device, 
        unsigned int ep, 
        struct usb_endpoint_instance *endpoint
        )
{
        if (udc_speed == USB_SPEED_SUPER) 
                u3d_setup_ep(device, ep, endpoint);
        else 
                u2d_setup_ep(device, ep, endpoint);
}

void udc_irq(
    void
    )
{
        u3d_irq();
        u2d_irq();
}

void udc_disconnect(    
        void
        )
{
        u3d_disconnect();
        u2d_disconnect();
}

void udc_connect(  
        void
        )
{
        u3d_connect();
        u2d_connect();
}

void udc_disable(    
        void
        )
{
        u3d_disable();
        u2d_disable();
}

void udc_enable(
        struct usb_device_instance *device
        )
{
        u3d_enable(device);
        u2d_enable(device);
}

void udc_startup_events(
        struct usb_device_instance *device
        )
{
        /* The DEVICE_INIT event puts the USB device in the state STATE_INIT. */
        usbd_device_event_irq(device, DEVICE_INIT, 0);

        /* The DEVICE_CREATE event puts the USB device in the state
         * STATE_ATTACHED.
         */
        usbd_device_event_irq(device, DEVICE_CREATE, 0);

        /* do udc specified enable work here */
        udc_enable(device);
}

int udc_init(
        void
        )
{
        vu3l_info(
                "%s %s, built %s %s\n",                 
                DRIVER_DESC, DRIVER_VERSION, __DATE__, __TIME__
                );
        
        /* init msg recorder to record print message into 
         * msg_recorder buffer when print message. 
         */
        VU3L_INIT_MSG_RECORDER();

        /* init u2d and u3d both, actually they are separate hw modules. */
        return (u3d_init() || u2d_init()) ? -1 : 0;
}