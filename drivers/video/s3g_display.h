#ifndef __DISPLAY_MGRI_H__
#define __DISPLAY_MGRI_H__
#include "adapter.h"
#include "fb_lib.h"

#ifndef TRUE
#define TRUE      1
#define FALSE     0
#endif

#ifndef NULL
#define NULL ((void*)0)
#endif

#define MAX_SCREENS         4
#define MAX_IGAS            2

#define S3G_NAME "s3g"

#ifndef bool 
#define  bool  int
#define  true 1
#define  false 0
#endif
//typedef enum _bool{false,true} bool;

#define ALIGN8 __attribute__ ((aligned(8)))

typedef unsigned long long s3g_ptr64_t;

typedef struct
{
    int pipe;
    int output;
    int bpp;
    int width;
    int height;
    int refresh;
    int dstWidth;
    int dstHeight;
    int dstRefresh;
    int interlaceflag;
    int scalertype;
    int compressed;
    int preferred_mode;

}s3g_modeset_t;

typedef struct
{
    // in variables
    int uDev;
    int xRes;
    int yRes;
    int refresh;
    int bInterlaced;
    int aspectRatio;

    // out variables
    int Clock;       /* pixel clock freq (kHz) */
    int HDisplay;    /* horizontal timing */
    int HSyncStart;
    int HSyncEnd;
    int HTotal;
    int VDisplay;    /* vertical timing */
    int VSyncStart;
    int VSyncEnd;
    int VTotal;
} s3g_mode_timing_info_t;


typedef enum _DEVICE_TYPE{
    DEV_HDMI,
    DEV_LCD,
    DEV_LAST,
}DEVICE_TYPE;


/* this type same to the type CBREGISTER in CBios.h*/
typedef struct strap_reg
{
    unsigned char type;
    unsigned char mask;
    unsigned char index;
    unsigned char value;
}strap_reg_t;


/* define the device type  */
#define UT_OUTPUT_NONE      0x00 /* No device. */
#define UT_OUTPUT_CRT1      0x01 /* CRT1 */
#define UT_OUTPUT_LCD       0x02 /* LCD *//*In fact it should be called pannel*/
#define UT_OUTPUT_TV        0x04 /* TV */
#define UT_OUTPUT_DVI       0x08 /* DVI */
#define UT_OUTPUT_CRT2      0x10 /* CRT2 */
#define UT_OUTPUT_HDTV      0x20 /* HDTV */
#define UT_OUTPUT_DVI2      0x40 /* DVI2 */

/* directly use the format of cbios for the high 8 bits */
#define UT_OUTPUT_LCD2      0x100 /*LCD2*/
#define UT_OUTPUT_HDMI3     0x200 /*HDMI(3)*/
#define UT_OUTPUT_DVI3      0x400 /*DVI3*/
#define UT_OUTPUT_DVI4      0x800 /*DVI4*/
#define UT_OUTPUT_HDMI1     0x1000 /*HDMI1*/
#define UT_OUTPUT_HDMI2     0x2000 /*HDMI2*/
#define UT_OUTPUT_HDMI4     0x4000 /*HDMI4*/
#define UT_OUTPUT_DP5       0x8000 /*DP5*/ 
#define UT_OUTPUT_DP6       0x10000 /*DP6*/
#define UT_OUTPUT_LCD3      0x20000  /*LCD3*/

#define UT_OUTPUT_MASK 0x3FF7F
#define UT_OUTPUT_CRTS ( UT_OUTPUT_CRT1 | UT_OUTPUT_CRT2 )
#define UT_OUTPUT_LCDS ( UT_OUTPUT_LCD |UT_OUTPUT_LCD2 )
#define UT_OUTPUT_DVIS ( UT_OUTPUT_DVI |UT_OUTPUT_DVI2 |UT_OUTPUT_DVI3 | UT_OUTPUT_DVI4 )
#define UT_OUTPUT_TVS  ( UT_OUTPUT_TV | UT_OUTPUT_TV2 )
#define UT_OUTPUT_HDMIS ( UT_OUTPUT_HDMI1 | UT_OUTPUT_HDMI2 | UT_OUTPUT_HDMI3 | UT_OUTPUT_HDMI4  )
#define UT_OUTPUT_DPS  ( UT_OUTPUT_DP5 | UT_OUTPUT_DP6)

static inline char* ut_strncpy(char* dst, const char* src, int len)
{
    char * d = dst;
    while(len)
    {
        len--;
        if((*d++ = *src++) == '\0')
             break;
    }

    while(len--)
        *d++ = '\0';

    return dst;
}

static inline char * ut_dev_str(int device)
{
    static char string[64];
    char *p=&string[0];

    if (device & UT_OUTPUT_NONE)
        return 0;

    if (device & UT_OUTPUT_CRT1) 
    {   
        ut_strncpy(p,"CRT1+",5);
        p += 5;
    }

    if (device & UT_OUTPUT_LCD) 
    {
        ut_strncpy(p,"LCD+",4);
        p += 4;
    }
    if (device & UT_OUTPUT_TV)
    {
        ut_strncpy(p,"TV+",3);
        p += 3;
    }
    if (device & UT_OUTPUT_DVI) 
    {
        ut_strncpy(p,"DVI+",4);
        p += 4;
    }
    if (device & UT_OUTPUT_DVI2)
    {
        ut_strncpy(p,"DVI2+",5);
        p += 5;
    }
    if (device & UT_OUTPUT_CRT2) 
    {
        ut_strncpy(p,"CRT2+",5);
        p += 5;
    }
    if (device & UT_OUTPUT_HDTV)
    {
        ut_strncpy(p,"HDTV+",5);
        p += 5;
    }

    if (device & UT_OUTPUT_LCD2)
    {
        ut_strncpy(p,"LCD2+",5);
        p += 5;
    }

    if (device & UT_OUTPUT_HDMI3)
    {
        ut_strncpy(p,"HDMI3+",6);
        p += 6;
    }
    if (device & UT_OUTPUT_DVI3) 
    {
        ut_strncpy(p,"DVI3+",5);
        p += 5;
    }
    if (device & UT_OUTPUT_DVI4) 
    {
        ut_strncpy(p,"DVI4+",5);
        p += 5;
    }
    if (device & UT_OUTPUT_HDMI1) 
    {
        ut_strncpy(p,"HDMI1+",6);
        p += 6;
    }
    if (device & UT_OUTPUT_HDMI2)
    {
        ut_strncpy(p,"HDMI2+",6);
        p += 6;
    }
    if (device & UT_OUTPUT_HDMI4) 
    {
        ut_strncpy(p,"HDMI4+",6);
        p += 6;
    }
    if (device & UT_OUTPUT_DP5)
    {
        ut_strncpy(p,"DP5+",4);
        p += 4;
    }
    if (device & UT_OUTPUT_DP6) 
    {
        ut_strncpy(p,"DP6+",4);
        p += 4;
    }
    if (device & UT_OUTPUT_LCD3)
    {
        ut_strncpy(p,"LCD3+",5);
        p += 5;
    }
    /* delete the last '+' */
    *(p-1) = '\0';

    return string;
}

#endif

