/*
 * via u3lite - library function
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

#include "vu3l_lib.h"

#ifdef VU3L_MSG_RECORDER
char msg_recorder[NUM_MSG][BYTE_MSG];  
unsigned current_msg_index = 0;
#endif 

#ifdef VU3L_AP
__u32 *vu3l_ap_reg = NULL;           
#endif 

char*
vu3l_string_usb_req(
    int request
    )
{
        switch (request) {
        make_case(USB_REQ_GET_STATUS)
        make_case(USB_REQ_CLEAR_FEATURE)
        make_case(USB_REQ_SET_FEATURE)
        make_case(USB_REQ_SET_ADDRESS)
        make_case(USB_REQ_GET_DESCRIPTOR)
        make_case(USB_REQ_SET_DESCRIPTOR)
        make_case(USB_REQ_GET_CONFIGURATION)
        make_case(USB_REQ_SET_CONFIGURATION)
        make_case(USB_REQ_GET_INTERFACE) 
        make_case(USB_REQ_SET_INTERFACE)
        make_case(USB_REQ_SYNCH_FRAME)    
        make_case(USB_REQ_SET_SEL)
        make_case(USB_REQ_SET_ISOCH_DELAY)
        }

        return "unknown";
}

char*
vu3l_string_status(
    int status
    )
{
        switch (status) {
        make_case(SUCCESS)
        make_case(-EINVAL)
        make_case(-ESHUTDOWN)
        make_case(-EOVERFLOW)
        make_case(-ENOMEM)
        make_case(-EINPROGRESS)
        make_case(-EOPNOTSUPP)
        make_case(-ENODEV)
        make_case(-EBUSY) 
        make_case(-ETIMEDOUT)
        make_case(-ENXIO)
        make_case(-ECONNRESET)
        make_case(-EAGAIN)
        make_case(-EPROTO)
        make_case(-EDOM)
        }

        return "unknown";
}

int vu3l_handshake8(
        u8 *ptr,
        u8 mask, 
        u8 done, 
        int usec
        )
{
        u8 result;

        do {
                result = vu3l_read8(ptr);

                /* card removed ? */
                if (result == ~(u8)0)                
                        return -ENODEV;

                /* match ? */
                result &= mask;
                if (result == done)
                        return 0;                
                
                udelay(1);
                usec--;
                
        } while (usec > 0);
        
        return -ETIMEDOUT;
}

int vu3l_handshake16(
        u16 *ptr,
        u16 mask, 
        u16 done, 
        int usec
        )
{
        u16 result;

        do {
                result = vu3l_read16(ptr);

                /* card removed ? */
                if (result == ~(u16)0)                
                        return -ENODEV;

                /* match ? */
                result &= mask;
                if (result == done)
                        return 0;
                
                udelay(1);
                usec--;
                
        } while (usec > 0);
        
        return -ETIMEDOUT;
}

int vu3l_handshake32(
        u32 *ptr,
        u32 mask, 
        u32 done, 
        int usec
        )
{
        u32 result;

        do {
                result = vu3l_read32(ptr);
               
                /* card removed ? */
                if (result == ~(u32)0)                
                        return -ENODEV;

                /* match ? */
                result &= mask;
                if (result == done)
                        return 0;
                
                udelay(1);
                usec--;
                
        } while (usec > 0);
        
        return -ETIMEDOUT;
}

int vu3l_set_and_handshake8(
        u8 *ptr,
        u8 mask, 
        u8 done, 
        int usec
        )
{
        int status;
               
        vu3l_bit_set8(ptr, mask);

        status = vu3l_handshake8(ptr, mask, done, usec);
        return status;
}

int vu3l_set_and_handshake16(
        u16 *ptr,
        u16 mask, 
        u16 done, 
        int usec
        )
{
        int status;
             
        vu3l_bit_set16(ptr, mask);

        status = vu3l_handshake16(ptr, mask, done, usec);
        return status;
}

int vu3l_set_and_handshake32(
        u32 *ptr,
        u32 mask, 
        u32 done, 
        int usec
        )
{
        int status;
             
        vu3l_bit_set32(ptr, mask);

        status = vu3l_handshake32(ptr, mask, done, usec);
        return status;
}

int vu3l_clear_and_handshake8(
        u8 *ptr,
        u32 mask, 
        u32 done, 
        int usec
        )
{
        int status;
              
        vu3l_bit_clear8(ptr, mask);

        status = vu3l_handshake8(ptr, mask, done, usec);
        return status;
}

int vu3l_clear_and_handshake16(
        u16 *ptr,
        u32 mask, 
        u32 done, 
        int usec
        )
{
        int status;
              
        vu3l_bit_clear16(ptr, mask);

        status = vu3l_handshake16(ptr, mask, done, usec);
        return status;
}

int vu3l_clear_and_handshake32(
        u32 *ptr,
        u32 mask, 
        u32 done, 
        int usec
        )
{
        int status;
              
        vu3l_bit_clear32(ptr, mask);

        status = vu3l_handshake32(ptr, mask, done, usec);
        return status;
}

#ifdef VU3L_MSG_RECORDER
void vu3l_dump_msg_recorder(
        void
        )
{
        unsigned i, imsg;
        
        for (i = 0; i < NUM_MSG; i++) {
                imsg = (current_msg_index + i) % NUM_MSG;
                printf("msg[%.3d] = %s\n", imsg, msg_recorder[imsg]);  
        }
}
#endif