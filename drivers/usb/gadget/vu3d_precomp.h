/*
 * via u3lite udc - common header
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

#ifndef __U3DC_PRECOMP_H
#define __U3DC_PRECOMP_H

#include "vu3l_lib.h"

/* driver print function definitions */
#ifdef VU3L_DBG

#ifdef VU3L_DBG_MSG    
#define vu3d_dbg(fmt, args...)  \
        do {                                                                                            \
                printf("[dbg ][%s][%s] %s:%d: "fmt,VU3L_FILE,vudc->name,__FUNCTION__,__LINE__,##args);           \
                VU3L_ADD_MSG("[dbg ][%s][%s] %s:%d: "fmt,VU3L_FILE,vudc->name,__FUNCTION__,__LINE__,##args);     \
        } while(0)

#define vep_dbg(fmt, args...)  \
        do {                                                                                            \
                printf("[dbg ][%s][%s] %s:%d: "fmt,VU3L_FILE,vep->name,__FUNCTION__,__LINE__,##args);           \
                VU3L_ADD_MSG("[dbg ][%s][%s] %s:%d: "fmt,VU3L_FILE,vep->name,__FUNCTION__,__LINE__,##args);     \
        } while(0)        
#else
#define vu3d_dbg(fmt, args...) printf("[dbg ][%s][%s] %s:%d: "fmt,VU3L_FILE,vudc->name,__FUNCTION__,__LINE__,##args)
#define vep_dbg(fmt, args...) printf("[dbg ][%s][%s] %s:%d: "fmt,VU3L_FILE,vep->name,__FUNCTION__,__LINE__,##args)
#endif

#else
#define vu3d_dbg(fmt, args...) do{}while(0)
#define vep_dbg(fmt, args...) do{}while(0)
#endif

#ifdef VU3L_MSG_RECORDER
#define vu3d_info(fmt, args...) \
        do {                                                                                            \
                printf("[info][%s][%s] %s:%d: "fmt,VU3L_FILE,vudc->name,__FUNCTION__,__LINE__,##args);           \
                VU3L_ADD_MSG("[info][%s][%s] %s:%d: "fmt,VU3L_FILE,vudc->name,__FUNCTION__,__LINE__,##args);     \
        } while(0)  
        
#define vu3d_err(fmt, args...) \
        do {                                                                                            \
                printf("[err ][%s][%s] %s:%d: "fmt,VU3L_FILE,vudc->name,__FUNCTION__,__LINE__,##args);           \
                VU3L_ADD_MSG("[err ][%s][%s] %s:%d: "fmt,VU3L_FILE,vudc->name,__FUNCTION__,__LINE__,##args);     \
        } while(0)      
        
#define vu3d_warn(fmt, args...) \
        do {                                                                                            \
                printf("[warn][%s][%s] %s:%d: "fmt,VU3L_FILE,vudc->name,__FUNCTION__,__LINE__,##args);           \
                VU3L_ADD_MSG("[warn][%s][%s] %s:%d: "fmt,VU3L_FILE,vudc->name,__FUNCTION__,__LINE__,##args);     \
        } while(0)
        
#define vep_info(fmt, args...) \
        do {                                                                                            \
                printf("[info][%s][%s] %s:%d: "fmt,VU3L_FILE,vep->name,__FUNCTION__,__LINE__,##args);           \
                VU3L_ADD_MSG("[info][%s][%s] %s:%d: "fmt,VU3L_FILE,vep->name,__FUNCTION__,__LINE__,##args);     \
        } while(0)  
        
#define vep_err(fmt, args...) \
        do {                                                                                            \
                printf("[err ][%s][%s] %s:%d: "fmt,VU3L_FILE,vep->name,__FUNCTION__,__LINE__,##args);           \
                VU3L_ADD_MSG("[err ][%s][%s] %s:%d: "fmt,VU3L_FILE,vep->name,__FUNCTION__,__LINE__,##args);     \
        } while(0)      
        
#define vep_warn(fmt, args...) \
        do {                                                                                            \
                printf("[warn][%s][%s] %s:%d: "fmt,VU3L_FILE,vep->name,__FUNCTION__,__LINE__,##args);           \
                VU3L_ADD_MSG("[warn][%s][%s] %s:%d: "fmt,VU3L_FILE,vep->name,__FUNCTION__,__LINE__,##args);     \
        } while(0)        
#else
#define vu3d_info(fmt, args...) printf("[info][%s][%s] %s:%d: "fmt,VU3L_FILE,vudc->name,__FUNCTION__,__LINE__,##args)
#define vu3d_err(fmt, args...)  printf("[err ][%s][%s] %s:%d: "fmt,VU3L_FILE,vudc->name,__FUNCTION__,__LINE__,##args)
#define vu3d_warn(fmt, args...) printf("[warn][%s][%s] %s:%d: "fmt,VU3L_FILE,vudc->name,__FUNCTION__,__LINE__,##args)

#define vep_info(fmt, args...) printf("[info][%s][%s] %s:%d: "fmt,VU3L_FILE,vep->name,__FUNCTION__,__LINE__,##args)
#define vep_err(fmt, args...)  printf("[err ][%s][%s] %s:%d: "fmt,VU3L_FILE,vep->name,__FUNCTION__,__LINE__,##args)
#define vep_warn(fmt, args...) printf("[warn][%s][%s] %s:%d: "fmt,VU3L_FILE,vep->name,__FUNCTION__,__LINE__,##args)
#endif

struct via_udc;
struct via_ep;

#endif
