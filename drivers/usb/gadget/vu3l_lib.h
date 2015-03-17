/*
 * via u3lite - library header
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

#ifndef __VU3L_LIB_H
#define __VU3L_LIB_H

#include <common.h>
#include <malloc.h>
#include <asm/io.h>
#include <asm/errno.h>
#include <asm/atomic.h>
#include <asm/dma-mapping.h>
#include <linux/compat.h>
#include <linux/stringify.h>
#include <linux/list.h>
#include <usb_defs.h>
#include <usbdevice.h>
#include <pci.h>
#include "ep0.h"
#include <usb/e2k_udc.h>

/* 
 * Driver Settings.
 *
 * VU3L_ON_ELITE_PCIE   : make U3Lite PCIe FPGA works on elite1k EVB.
 * VU3L_AP              : generate a mmio write cycle as HW trigger point on HW Analyer.
 * VU3L_TRAP_ON_NODBG   : make trap valid even if VU3L_DBG isn't defined.
 * VU3L_DBG             : make vu3l_dbg/vu3d_dbg marcos valid.
 * VU3L_MSG_RECORDER    : record print message to msg_recorder memory buffer when print message.
 * VU3L_DBG_MSG         : record vu3l_dbg/vu3d_dbg message to msg_recorder memory buffer when print message.
 */
//#define VU3L_ON_ELITE_PCIE    1
//#define VU3L_DBG              1
//#define VU3L_TRAP_ON_NODBG    1
#define VU3L_AP                 1

/* 
 * declaration which doesn't define in uboot but linux kernel 
 */
#define PTR_ALIGN(p, a) ((typeof(p))ALIGN((unsigned long)(p), (a)))

#define USEC_PER_SEC        1000000L

/* usb 3.0 */
#ifndef USB_REQ_SET_SEL

#define USB_REQ_SET_SEL         0x30

struct usb_set_sel_req {
        __u8    u1_sel;
        __u8    u1_pel;
        __le16  u2_sel;
        __le16  u2_pel;
} __attribute__ ((packed));

#endif

#ifndef USB_REQ_SET_ISOCH_DELAY
#define USB_REQ_SET_ISOCH_DELAY 0x31
#endif

#ifndef USB_DT_BOS
#define USB_DT_BOS              0x0F
#endif

/*
 * New Feature Selectors as added by USB 3.0
 * See USB 3.0 spec Table 9-6
 */
#define USB_DEVICE_U1_ENABLE            48      /* dev may initiate U1 transition */
#define USB_DEVICE_U2_ENABLE            49      /* dev may initiate U2 transition */

/* Bit array elements as returned by the USB_REQ_GET_STATUS request. */
#define USB_DEV_STAT_U1_ENABLED         2       /* transition into U1 state */
#define USB_DEV_STAT_U2_ENABLED         3       /* transition into U2 state */

/* */
#define VU3L_FILE (strrchr(__FILE__, '_') ? strrchr(__FILE__, '_') + 1 : __FILE__)

#ifdef VU3L_MSG_RECORDER

#define NUM_MSG         (128)
#define BYTE_MSG        (128)

extern char msg_recorder[NUM_MSG][BYTE_MSG];   
extern unsigned current_msg_index;

#define VU3L_INIT_MSG_RECORDER()                                        \
        do {                                                            \
                current_msg_index = 0;                                  \
                memset(msg_recorder, '\0', sizeof(msg_recorder));       \
        } while(0)

#define VU3L_ADD_MSG(fmt, args...)                                      \
        do {                                                            \
                snprintf(                                               \
                        msg_recorder[current_msg_index],                \
                        BYTE_MSG,                                       \
                        fmt,                                            \
                        ## args                                         \
                        );                                              \
                current_msg_index = (current_msg_index + 1) % NUM_MSG;  \
        } while(0)
        
#else
#define VU3L_INIT_MSG_RECORDER() do{}while(0)
#define VU3L_ADD_MSG() do{}while(0)
#endif 

/* VU3L_INIT_AP_TRAP_REG definitions */ 
#ifdef VU3L_AP
extern __u32 *vu3l_ap_reg;    
#define VU3L_INIT_AP_TRAP_REG(x) do { vu3l_ap_reg = x; } while(0)
#else
#define VU3L_INIT_AP_TRAP_REG(x) do{}while(0)
#endif 

/* driver print function definitions */
#ifdef VU3L_DBG

#ifdef VU3L_DBG_MSG
#define vu3l_dbg(fmt, args...)  \
        do {                                                                                            \
                printf("[dbg ][%s] %s:%d: "fmt,VU3L_FILE,__FUNCTION__,__LINE__,##args);           \
                VU3L_ADD_MSG("[dbg ][%s] %s:%d: "fmt,VU3L_FILE,__FUNCTION__,__LINE__,##args);     \
        } while(0)        
#else
#define vu3l_dbg(fmt, args...) printf("[dbg ][%s] %s:%d: "fmt,VU3L_FILE,__FUNCTION__,__LINE__,##args)
#endif

#else
#define vu3l_dbg(fmt, args...) do{}while(0)
#endif

#ifdef VU3L_MSG_RECORDER
#define vu3l_info(fmt, args...) \
        do {                                                                                            \
                printf("[info][%s] %s:%d: "fmt,VU3L_FILE,__FUNCTION__,__LINE__,##args);           \
                VU3L_ADD_MSG("[info][%s] %s:%d: "fmt,VU3L_FILE,__FUNCTION__,__LINE__,##args);     \
        } while(0) 

#define vu3l_err(fmt, args...) \
        do {                                                                                            \
                printf("[err ][%s] %s:%d: "fmt,VU3L_FILE,__FUNCTION__,__LINE__,##args);           \
                VU3L_ADD_MSG("[err ][%s] %s:%d: "fmt,VU3L_FILE,__FUNCTION__,__LINE__,##args);     \
        } while(0)      
        
#define vu3l_warn(fmt, args...) \
        do {                                                                                            \
                printf("[warn][%s] %s:%d: "fmt,VU3L_FILE,__FUNCTION__,__LINE__,##args);           \
                VU3L_ADD_MSG("[warn][%s] %s:%d: "fmt,VU3L_FILE,__FUNCTION__,__LINE__,##args);     \
        } while(0)
#else
#define vu3l_info(fmt, args...) printf("[info][%s] %s:%d: "fmt,VU3L_FILE,__FUNCTION__,__LINE__,##args)
#define vu3l_err(fmt, args...)  printf("[err ][%s] %s:%d: "fmt,VU3L_FILE,__FUNCTION__,__LINE__,##args)
#define vu3l_warn(fmt, args...) printf("[warn][%s] %s:%d: "fmt,VU3L_FILE,__FUNCTION__,__LINE__,##args)
#endif

/* dtrap definition */
#ifdef VU3L_BUGON
#define dtrap(x)        \
        do {                                                                            \
                if (!(x)) {                                                             \
                        printf(                                                         \
                                "[trap][%s] %s:%d: '%s'\n",                             \
                                VU3L_FILE, __FUNCTION__, __LINE__, __stringify(x)       \
                                );                                                      \
                        BUG_ON(true);                                                   \
                }                                                                       \
        } while (0)
#else
#define dtrap(x)        \
        do {                                                                            \
                if (!(x)) {                                                             \
                        printf(                                                         \
                                "[trap][%s] %s:%d: '%s'\n",                             \
                                VU3L_FILE, __FUNCTION__, __LINE__, __stringify(x)       \
                                );                                                      \
                }                                                                       \
        } while (0)
#endif

/* trap definition */
#ifdef VU3L_DBG
#define trap(x) dtrap(x)
#else
#ifdef VU3L_TRAP_ON_NODBG
#define trap(x) dtrap(x)
#else
#define trap(x) do{}while(0)
#endif
#endif

/* ap_dtrap definition */
#ifdef VU3L_AP
#define ap_dtrap(x)      \
        do {                                                            \
                if (!(x)) {                                             \
                        vu3l_write32(vu3l_ap_reg, __LINE__);            \
                        dtrap(x);                                       \
                }                                                       \
        } while (0)          
#else
#define ap_dtrap(x) dtrap(x)
#endif

/* ap_trap definition */
#ifdef VU3L_DBG
#define ap_trap(x) ap_dtrap(x)    
#else
#ifdef VU3L_TRAP_ON_NODBG
#define ap_trap(x) ap_dtrap(x) 
#else
#define ap_trap(x) do{}while(0)
#endif
#endif

/* case to string marco */
#define make_case(code) case code: return #code;

/* vudc status definitions, compatible with linux error status */
#define SUCCESS                 (0)
#define VU3L_SUCCESS(status)    (status == 0)
#define VU3L_ERROR(status)      (status != 0)

/* shift value to its bit field */
#define VAL_TO_BF(_value_, _shift_)             \
        ((_value_) << (_shift_))

#define VAL_TO_MBF(_value_, _shift_, _mask_)    \
        ((((_value_) & (_mask_)) << (_shift_)))

#define IS_VALID_TO_BF(_value_, _mask_)         \
        ((_value_) <= (_mask_))

#define BF_TO_VAL(_bf_, _shift_)                \
        ((_bf_) >> (_shift_))

#define MBF_TO_VAL(_bf_, _shift_, _mask_)       \
        (((_bf_) >> (_shift_)) & (_mask_))

/* flag operations */
#define TEST_FLAG(_value_, _mask_)              \
        ((_value_) & (_mask_))
        
#define TEST_FLAGS(_value_, _mask_)             \
        (((_value_) & (_mask_)) == (_mask_))

char*
vu3l_string_usb_req(
    int request
    );

char*
vu3l_string_status(
    int status
    );

int vu3l_handshake8(
        u8 *ptr,
        u8 mask, 
        u8 done, 
        int usec
        );

int vu3l_handshake16(
        u16 *ptr,
        u16 mask, 
        u16 done, 
        int usec
        );

int vu3l_handshake32(
        u32 *ptr,
        u32 mask, 
        u32 done, 
        int usec
        );

int vu3l_set_and_handshake8(
        u8 *ptr,
        u8 mask, 
        u8 done, 
        int usec
        );

int vu3l_set_and_handshake16(
        u16 *ptr,
        u16 mask, 
        u16 done, 
        int usec
        );

int vu3l_set_and_handshake32(
        u32 *ptr,
        u32 mask, 
        u32 done, 
        int usec
        );

int vu3l_clear_and_handshake8(
        u8 *ptr,
        u32 mask, 
        u32 done, 
        int usec
        );

int vu3l_clear_and_handshake16(
        u16 *ptr,
        u32 mask, 
        u32 done, 
        int usec
        );

int vu3l_clear_and_handshake32(
        u32 *ptr,
        u32 mask, 
        u32 done, 
        int usec
        );

#ifdef VU3L_MSG_RECORDER
void vu3l_dump_msg_recorder(
        void
        );
#else
#define vu3l_dump_msg_recorder()        do{}while(0) 
#endif

static inline u8 vu3l_read8(
        u8 *ptr
        )
{
        return readb(ptr);
}

static inline u16 vu3l_read16(
        u16 *ptr
        )
{
        return readw(ptr);
}

static inline u32 vu3l_read32(
        u32 *ptr
        )
{
        return readl(ptr);       
}

static inline void vu3l_write8(
        u8 *ptr,
        u8 val
        )
{
        writeb(val, ptr);
}

static inline void vu3l_write16(
        u16 *ptr,
        u16 val
        )
{
        writew(val, ptr);
}

static inline void vu3l_write32(
        u32 *ptr,
        u32 val
        )
{
        writel(val, ptr);
}

static inline void vu3l_memcpy_fromio(
        void *dest, 
        void *source, 
        unsigned int count
        )
{
        /* memcpy_fromio(dest, source, count); */
        memcpy((dest), (source), (count));
}

static inline void vu3l_memcpy_toio(
        void *dest, 
        void *source, 
        unsigned int count
        )
{
        /* memcpy_toio(dest, source, count); */
        memcpy((dest), (source), (count));
}

static inline void vu3l_bit_modify8(
        u8 *ptr,
        u8 bits,
        u8 value
        )
{
        u8 tmp;
        
        tmp = vu3l_read8(ptr);
        tmp = tmp & (~bits);
        tmp = tmp | value;
        vu3l_write8(ptr, tmp);
}

static inline void vu3l_bit_modify16(
        u16 *ptr,
        u16 bits,
        u16 value
        )
{
        u16 tmp;
        
        tmp = vu3l_read16(ptr);
        tmp = tmp & (~bits);
        tmp = tmp | value;
        vu3l_write16(ptr, tmp);
}

static inline void vu3l_bit_modify32(
        u32 *ptr,
        u32 bits,
        u32 value
        )
{
        u32 tmp;
        
        tmp = vu3l_read32(ptr);
        tmp = tmp & (~bits);
        tmp = tmp | value;
        vu3l_write32(ptr, tmp);
}

static inline void vu3l_bit_clear8(
        u8 *ptr,
        u8 bit
        )
{
        u8 tmp;
        
        tmp = vu3l_read8(ptr);
        tmp &= (~bit);
        vu3l_write8(ptr, tmp);
}

static inline void vu3l_bit_clear16(
        u16 *ptr,
        u16 bit
        )
{
        u16 tmp;
        
        tmp = vu3l_read16(ptr);
        tmp &= (~bit);
        vu3l_write16(ptr, tmp);
}

static inline void vu3l_bit_clear32(
        u32 *ptr,
        u32 bit
        )
{
        u32 tmp;
        
        tmp = vu3l_read32(ptr);
        tmp &= (~bit);
        vu3l_write32(ptr, tmp);
}

static inline void vu3l_bit_set8(
        u8 *ptr,
        u8 bit
        )
{
        u8 tmp;
        
        tmp = vu3l_read8(ptr);
        tmp |= bit;
        vu3l_write8(ptr, tmp);
}

static inline void vu3l_bit_set16(
        u16 *ptr,
        u16 bit
        )
{
        u16 tmp;
        
        tmp = vu3l_read16(ptr);
        tmp |= bit;
        vu3l_write16(ptr, tmp);
}

static inline void vu3l_bit_set32(
        u32 *ptr,
        u32 bit
        )
{
        u32 tmp;
        
        tmp = vu3l_read32(ptr);
        tmp |= bit;
        vu3l_write32(ptr, tmp);
}

static inline u8 vu3l_bit_test8(
        u8 *ptr,
        u8 bit
        )
{
        return (vu3l_read8(ptr) & bit);
}

static inline u16 vu3l_bit_test16(
        u16 *ptr,
        u16 bit
        )
{
        return (vu3l_read16(ptr) & bit);
}

static inline u32 vu3l_bit_test32(
        u32 *ptr,
        u32 bit
        )
{
        return (vu3l_read32(ptr) & bit);
}

#endif
