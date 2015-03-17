/*
 * via elite 2k udc(s) - header
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

#ifndef __E2K_UDC_H
#define __E2K_UDC_H

#include <usbdevice.h>

/* public interface for via udc driver */
void udc_update_speed(
        int speed
        );

/* Higher level functions for abstracting away from specific device */
int udc_endpoint_write(
        struct usb_endpoint_instance *endpoint
        );

/* udc_setup_ep - setup endpoint */
void udc_setup_ep(
        struct usb_device_instance *device, 
        unsigned int ep, 
        struct usb_endpoint_instance *endpoint
        );

/* udc_irq - do pseudo interrupts */
void udc_irq(
    void
    );

/* Turn off the USB connection by disabling the pullup resistor */
void udc_disconnect(    
        void
        );

/* Turn on the USB connection by enabling the pullup resistor */
void udc_connect(  
        void
        );

/* Switch off the UDC */
void udc_disable(    
        void
        );

/* Switch on the UDC */
void udc_enable(
        struct usb_device_instance *device
        );

/* udc_startup - allow udc code to do any additional startup */
void udc_startup_events(
        struct usb_device_instance *device
        );

/* Start to initialize h/w stuff */
int udc_init(
        void
        );

#endif

