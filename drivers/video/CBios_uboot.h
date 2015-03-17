/*
** Copyright (c) 2011, S3 Graphics Co., Ltd.
** All Rights Reserved.
**
** This is UNPUBLISHED PROPRIETARY SOURCE CODE of S3 Graphics, Inc.;
** the contents of this file may not be disclosed to third parties, copied or
** duplicated in any form, in whole or in part, without the prior written
** permission of S3 Graphics, Inc.
**
** RESTRICTED RIGHTS LEGEND:
** Use, duplication or disclosure by the Government is subject to restrictions
** as set forth in subdivision (c)(1)(ii) of the Rights in Technical Data
** and Computer Software clause at DFARS 252.227-7013, and/or in similar or
** successor clauses in the FAR, DOD or NASA FAR Supplement. Unpublished -
** rights reserved under the Copyright Laws of the United States.
*/

#ifndef _CBIOS_H_
#define _CBIOS_H_

/*****************************************************************************
**
**  CBios interface define:
**      VBE defines
**      Interface function prototype and parameter defines
**      CBIOS_EXTENSION defines
**
******************************************************************************/

#define CBIOS_IN
#define CBIOS_OUT

#define CBIOS_CHECK_HARDWARE_STATUS 1   /*flag to indicate whether do hardware status check in cbPost */

#define CBIOS_VBLANK_RETRIES                             50000

#if defined(__linux__)
#undef  RegPreFix
#define RegPreFix( x ) (void *)x
#else
#define RegPreFix( x ) L ## x
#endif


#define KEYNAME_DW_HARDCODED_LINKSPEED      RegPreFix("S3GDW_cbHardCodedLinkSpeed")   // 0 => Hard coding disabled, 1 => 1.62Gbps, 2 => 2.7Gbps, others => Hard coding disabled
#define KEYNAME_DW_HARDCODED_LANECOUNT      RegPreFix("S3GDW_cbHardCodedLaneCount")   // 0 => Hard coding disabled, (1, 2, 4) => Number of lanes, others => Hard coding disabled
#define KEYNAME_DW_LINKTRAINING_METHOD      RegPreFix("S3GDW_cbLinktrainingMethod")   // 0 => Hard coding disabled, 1 => Hardware Link Training, 2 => Software Link Training, others => Hard coding disabled
#define KEYNAME_DW_HARDCODED_DP5_EDID       RegPreFix("S3GDW_cbHardCodedDp5EDID")     // 1=>"DELL 2408WFP" HDMI EDID; 2=>"WestingHouse LVM-37w3" HDMI EDID; 3=>EDID forced to all Zero Bytes; 4=>"HpLp2475w" DP EDID; Others=>Disable hardcoding
#define KEYNAME_DW_HARDCODED_EDID_DEVICE    RegPreFix("S3GDW_cbHardCodedEDIDDevice")  // The Devices want to use the hardcoded EDID 
#define KEYNAME_UC_HARDCODED_DEVICE_EDID    RegPreFix("S3GUC_cbHardCodedDeviceEDID")  // The EDID buffer. If longer than 512 bytes, the remainder will be discarded.
#define KEYNAME_DW_HARDCODED_SPECIALMODE    RegPreFix("S3GDW_cbHardCodedSpecialMode") // to some 120Hz mode such as 1280*1024*120Hz for TV wall project
#define KEYNAME_DW_EDP_CP_METHOD            RegPreFix("S3GDW_cbeDPContentProtectionMethod") // 0 => Disable CP, 1 => Enable, use method 3a, 2 => Enable, use method 3b, others are reserved


#define DP_RESET_AUX_WHEN_DEFER     1       // If any defer received, reset aux channel

/*indicate i2c data request from DDCCI or HDCP*/
#define CBIOS_I2CDDCCI 0
#define CBIOS_I2CHDCP 1
#define CBIOS_I2CCAPTURE 2

#define CBIOSVERSION    0x00000001

#define CBIOS_DeviceINFO_CRT                                   0
#define CBIOS_DeviceINFO_DVI                                   1
#define CBIOS_DeviceINFO_LCD                                   2
#define CBIOS_DeviceINFO_CRT2                                  3
#define CBIOS_DeviceINFO_DVI2                                  4
#define CBIOS_DeviceINFO_TV                                    5
#define CBIOS_DeviceINFO_HDTV                                  6
#define CBIOS_DeviceINFO_LCD2                                  7
#define CBIOS_DeviceINFO_HDMI3                                 8
#define CBIOS_DeviceINFO_DVI3                                  9
#define CBIOS_DeviceINFO_DVI4                                  10
#define CBIOS_DeviceINFO_HDMI1                                 11
#define CBIOS_DeviceINFO_HDMI2                                 12
#define CBIOS_DeviceINFO_HDMI4                                 13
#define CBIOS_DeviceINFO_DP5                                   14
#define CBIOS_DeviceINFO_DP6                                   15
#define CBIOS_DeviceINFO_LCD3                                  16
#define CBIOS_DeviceINFO_TV2                                    17
#define CBIOS_DeviceINFO_HDTV2                                  18

/******** VBE defines *********/
#define CBIOS_VBE_STATUS_NOT_SUPPORTED         0x0000
#define CBIOS_VBE_STATUS_SUCCESSFUL            0x004F
#define CBIOS_VBE_STATUS_FAILED                0x014F
#define CBIOS_VBE_STATUE_INVALID_PARAMETER     0x024F
/******** Set clock defines *********/
#define CBIOS_SOFTWARE_TUNE_CLOCK           0
#define CBIOS_HARDWARE_TUNE_CLOCK           1
#define CBIOS_HARDWARE_CLOCK_FULL           1
#define CBIOS_HARDWARE_CLOCK_HALF           2
#define CBIOS_HARDWARE_CLOCK_QUALTER        4
#define CBIOS_HARDWARE_CLOCK_EIGHTH         8
#define CBIOS_HARDWARE_CLOCK_INVALID        0xFFFF

/******** Get clock defines *********/
#define CBIOS_CLOCK_DEFAULT                 0
#define CBIOS_CLOCK_MIN                     1
#define CBIOS_CLOCK_MAX                     2

/* Color Depth Capability */
#define CBIOS_COLORDEPTH8                                      0x04
#define CBIOS_COLORDEPTH16                                     0x02
#define CBIOS_COLORDEPTH32XRGB                                 0x80
#define CBIOS_COLORDEPTH32ARGB                                 0x01
#define CBIOS_COLORDEPTH32ABGR                                 0x08
#define CBIOS_COLORDEPTH2101010ARGB                            0x10
#define CBIOS_COLORDEPTH2101010ABGR                            0x20
#define CBIOS_COLORDEPTH16161616ABGRF                          0x40

/* From driver, color depth + offset to indicate different pixel format with same color depth */
#define CBIOS_COLOR_DEPTCH_ARGB_8888                    0x00000000
#define CBIOS_COLOR_DEPTCH_ABGR_8888                    0x00000001
#define CBIOS_COLOR_DEPTCH_ARGB_2101010                 0x00000002
#define CBIOS_COLOR_DEPTCH_ABGR_2101010                 0x00000003
/*  for 64-bit color depth */
#define CBIOS_COLOR_DEPTCH_ABGRF_16161616               0x00000000

// For MonitorCaps of _CBiosQueryConnectedMonitorAttributeParams use
#define MONITOR_CAPS_REAL_HDMI   0x00000001  // Similar as output Edx of VBE_4F14_GetAttachedDisplayDevices
#define CEA861_MONITOR   0x00000001
#define CEA861_RGB       0x00000002
#define CEA861_YCrCr422  0x00000004
#define CEA861_YCbCr444  0x00000008
#define CBIOS_VIRTUAL_GPIO_FOR_DP5                      8
#define CBIOS_VIRTUAL_GPIO_FOR_DP6                      9

/* Clock Type */
#define CBIOS_MCLKTYPE        0x00
#define CBIOS_DCLK1TYPE       0x01
#define CBIOS_DCLK2TYPE       0x02
#define CBIOS_TVCLKTYPE       0x03
#define CBIOS_ECLKTYPE        0x04
#define CBIOS_ICLKTYPE        0x05
#define CBIOS_CPUFRQTYPE      0x06
#define CBIOS_ALLCLKTYPE      0x07


#ifndef CBIOS_FALSE
#define CBIOS_FALSE   0
#endif

#ifndef CBIOS_TRUE
#define CBIOS_TRUE    1
#endif

#ifndef CBIOS_NULL
#define CBIOS_NULL    ((void*)0)
#endif

typedef unsigned char  CBIOS_UCHAR, *PCBIOS_UCHAR;
typedef unsigned char  CBIOS_U8,    *PCBIOS_U8;
typedef unsigned short CBIOS_U16,   *PCBIOS_U16;
typedef unsigned int   CBIOS_U32,   *PCBIOS_U32;
typedef unsigned long long CBIOS_U64, *PCBIOS_U64;

typedef char           CBIOS_CHAR, *PCBIOS_CHAR;
typedef signed char    CBIOS_S8,   *PCBIOS_S8;
typedef signed short   CBIOS_S16,  *PCBIOS_S16;
typedef signed int     CBIOS_S32,  *PCBIOS_S32;
typedef signed int     CBIOS_BOOL, *PCBIOS_BOOL;
typedef long long          CBIOS_S64, *PCBIOS_S64; 

typedef void                      *CBIOS_HANDLE;
typedef void          CBIOS_VOID, *PCBIOS_VOID;

#if defined(_WIN64)
typedef unsigned __int64 CBIOS_ULONG_PTR, *PCBIOS_ULONG_PTR;
#else
typedef unsigned long    CBIOS_ULONG_PTR, *PCBIOS_ULONG_PTR;
#endif

typedef enum _CBIOS_ACTIVE_TYPE   {
    CBIOS_TYPE_NONE        = 0x00,
    CBIOS_TYPE_CRT         = 0x01,
    CBIOS_TYPE_PANEL       = 0x02,
    CBIOS_TYPE_TV          = 0x04,
    CBIOS_TYPE_HDTV        = 0x08,
    CBIOS_TYPE_DVI2        = 0x10,
    CBIOS_TYPE_DVO         = 0x20,
    CBIOS_TYPE_CRT2        = 0x40,
    CBIOS_TYPE_DUOVIEW     = 0x80,
    CBIOS_TYPE_PANEL2      = 0x100,
    CBIOS_TYPE_HDMI3       = 0x200,
    CBIOS_TYPE_DVI3        = 0x400,
    CBIOS_TYPE_DVI4        = 0x800,
    CBIOS_TYPE_HDMI1       = 0x1000,
    CBIOS_TYPE_HDMI2       = 0x2000,
    CBIOS_TYPE_HDMI4       = 0x4000,
    CBIOS_TYPE_DP5         = 0x8000,
    CBIOS_TYPE_DP6         = 0x10000,
    CBIOS_TYPE_PANEL3      = 0x20000,
    CBIOS_TYPE_TV2         = 0x40000,
    CBIOS_TYPE_HDTV2       = 0x80000
} CBIOS_ACTIVE_TYPE, *PCBIOS_ACTIVE_TYPE;

#define CBIOS_MAX_DEVICE_BITS    32

typedef enum _CBIOS_MONITOR_TYPE   {
    CBIOS_MONITOR_TYPE_NONE     = 0x00,
    CBIOS_MONITOR_TYPE_CRT      = 0x01,
    CBIOS_MONITOR_TYPE_TV       = 0x02,
    CBIOS_MONITOR_TYPE_HDTV     = 0x04,
    CBIOS_MONITOR_TYPE_PANEL    = 0x08,
    CBIOS_MONITOR_TYPE_DVI      = 0x10,
    CBIOS_MONITOR_TYPE_HDMI     = 0x20,
    CBIOS_MONITOR_TYPE_DP       = 0x40
} CBIOS_MONITOR_TYPE, *PCBIOS_MONITOR_TYPE;

typedef enum _CBIOS_HDTV_ENCODER   {
    CBIOS_HDTV_ENCODER_NONE     = 0x00,
    CBIOS_HDTV_ENCODER1         = 0x01,
    CBIOS_HDTV_ENCODER2         = 0x02,
} CBIOS_HDTV_ENCODER, *PCBIOS_HDTV_ENCODER;

typedef enum _CBIOS_HDCP_INDEX   {
    CBIOS_HDCP_INDEX_NONE      = 0x00,
    CBIOS_HDCP_INDEX1         = 0x01,   //HDCP-H1 for HDMI3
    CBIOS_HDCP_INDEX2         = 0x02,   //HDCP-H2 for HDMI4
    CBIOS_HDCP_INDEX3         = 0x03,   //HDCP-D1 for DP5
    CBIOS_HDCP_INDEX4         = 0x04,   //HDCP-D2 for DP6
} CBIOS_HDCP_INDEX, *PCBIOS_HDCP_INDEX;

typedef enum _CBIOS_PM_STATUS 
{
    CBIOS_PM_ON         = 0,
    CBIOS_PM_STANDBY    = 1,
    CBIOS_PM_SUSPEND    = 2,
    CBIOS_PM_OFF        = 4,
    CBIOS_PM_INVALID    = 0xFF
} CBIOS_PM_STATUS, *PCBIOS_PM_STATUS;

/* Castlerock and Innovation */
typedef enum
{
    CHIPID_CMODEL,    /* Model for any chip.               */
    CHIPID_CLB,       /* Columbia                          */
    CHIPID_DST,       /* Destination                       */
    CHIPID_CSR,       /* Castlerock                        */
    CHIPID_INV,       /* Innovation (H3)                   */
    CHIPID_H5,        /* Innovation (H5)                   */
    CHIPID_H5S1,      /* Innovation (H5S1)                 */
    CHIPID_H6S2,      /* Innovation (H5S1)                 */
    CHIPID_CMS,       /* Columbia MS                       */
    CHIPID_METRO,     /* Metropolis                        */
    CHIPID_MANHATTAN, /* manhattan                         */
    CHIPID_MATRIX,    /* Matrix                            */
    CHIPID_DST2,      /* Destination-2                     */
    CHIPID_DST3,      /* Destination-3                     */
    CHIPID_DUMA,      /* Dst family, DUMA                  */
    CHIPID_H6S1,      /* Innovation (H6S1)                 */
    CHIPID_DST4,      /* Destination-4                     */
    CHIPID_EXC,       /* Excalibur -1                      */
    CHIPID_EXC2,      /* Excalibur -2                      */
    CHIPID_M32,       /* M32                               */
    CHIPID_E2UMA,  /* E2UMA                           */
    CHIPID_ELITE,   /* ELITE                             */
    CHIPID_LAST,      /* Maximum number of chips supported.*/
} CHIPID_HW;

/******Interface function prototype and parameter defines *******/
typedef enum _CBIOS_STATUS_tag {
    CBIOS_OK,                        /* 0    OK - no error */
    CBIOS_ER_INVALID_PARAMETER,      /* As is */
    CBIOS_ER_NOT_YET_IMPLEMENTED,    /* Feature / function not yet implemented*/
    CBIOS_ER_INTERNAL,               /* internal error (should never happen) */
    CBIOS_ER_OS_ERROR,               /* Error propagated up from the underlying OS */
    CBIOS_ER_BUFFER_TOO_SMALL,       /* The provided Buffer is too small. */
    CBIOS_ER_HARDWARE_LIMITATION,    /* Hardware limitation. */
    CBIOS_ER_DUPLICATED_REQUEST, /*Duplicated request*/
    CBIOS_ER_LB_PU1_PU2,               /* PU1 and PU2 enabled */
    CBIOS_ER_LB_PU1_PS2,               /* PU1 and PS2 LB share enabled */
    CBIOS_ER_LB_PS2_PU2,               /* PS2 LB share and PU2 enabled */
    CBIOS_ER_LB_PU2_PS1,               /* PU2 and PS1 LB share enabled */
    CBIOS_ER_LB_PS1_PU1,               /* PS1 LB share and PU1 enabled */
    CBIOS_ER_VID_MEM,                /*  video memory can't be read or write effectively. */
    CBIOS_ER_MMIO,              /*   MMIO read/write error */
    CBIOS_ER_IO,                    /*   IO read/write error */
    CBIOS_ER_STRAPPING_RESTORED,           /*   strapping registers were at wrong values and then restored */
    CBIOS_ER_STRAPPING_CANNOT_RESTORE,           /*   strapping registers are at wrong values and cannot be  restored */
    CBIOS_ER_NULLPOINTER,         /*null pointer is send to CBIOS interface*/
    CBIOS_ER_CHIPDISABLE,           /*Chip is not enable*/
    CBIOS_ER_OPERATION_ON_NONE_DEVICE,
    CBIOS_ER_INVALID_HOTPLUG,
    CBIOS_ER_LANE_DECREASED,
    CBIOS_ER_CLAIMEDMEMSIZE_STRAP_RESTORED,
    CBIOS_ER_VBIOS_FEATURE_OFF,
    CBIOS_ER_EDID_INVALID,
    CBIOS_ER_LAST                    /* Number of error codes */
} CBIOS_STATUS;

typedef enum _CBIOS_HW_BLOCK{
    CBIOS_HW_MIU = 0,
}CBIOS_HW_BLOCK;

typedef enum _CBIOS_CHECK_DEV_COMBIN_FLAG{
    CBIOS_CHECK_DEV_COMBIN_SUPPORT  = 0x01, // Bit0
    CBIOS_CHECK_DEV_SIMULTANEOUS    = 0x02, // Bit1
}CBIOS_CHECK_DEV_COMBIN_FLAG,*PCBIOS_CHECK_DEV_COMBIN_FLAG;

typedef enum _CBIOS_VOLATAGEFUNCTION_tag {
    CBIOS_VOLATAGE_GET_PARAMETER  = 1,                        /* 0x1 -  Get Voltage Params from VCP */
    CBIOS_VOLATAGE_SET_VALUE = 2,                                              /* 0x2-Set Voltage  value */
    CBIOS_VOLATAGE_IS_SCALE_SUPPORT  = 4                   /* 0x4-query is voltage scale support */     
} CBIOS_VOLATAGEFUNCTION;

typedef struct _CBIOS_PCI_ADDR_PARA
{
    CBIOS_U8  BusNum;
    CBIOS_U8  DeviceNum;/*Bit 4-0: device number; Bit 7-5: reserved*/
    CBIOS_U8  FunctionNum;/*Bit 2-0: function number; Bit 7-3: reserved*/
}CBIOS_PCI_ADDR_PARA, *PCBIOS_PCI_ADDR_PARA;

typedef enum _CBIOS_FLAGS
{
    CBIOS_FLAGS_DISABLE_DEVBIT_TRANS = 0x01,
}CBIOS_FLAGS, *PCBIOS_FLAGS;

typedef struct _CBIOS_PARAM_INIT {
    CBIOS_U32   Size;
    PCBIOS_VOID RomImage;
    CBIOS_U32   RomImageLength;
    PCBIOS_VOID pSpinLock;
    PCBIOS_VOID pAdapterContext;
    PCBIOS_UCHAR MmioBase;
    PCBIOS_UCHAR RsvdVGAMmioBase;
    CBIOS_U32 MAMMPrimaryAdapter;
    CBIOS_U32 GeneralChipID;
    CBIOS_U32 PCIDeviceID;
    CBIOS_U32 BusType;
    CBIOS_U32 ChipID;
    CBIOS_U32 ChipRevision;
    CBIOS_U32 RsvdFBPhysicalAddressLow;
    CBIOS_U32 RsvdFBPhysicalAddressHigh;
    CBIOS_U32 RsvdAdapterMemorySize;

    CBIOS_U32   RsvdTvChipID;
    PCBIOS_VOID RsvdpSavedRegs;
    CBIOS_U32   bRunOnQT;
    CBIOS_U32   EnablePort80; /* If EnablePort80 == 1, enable port80*/
    CBIOS_PCI_ADDR_PARA PCIAddress;
    CBIOS_FLAGS CBiosFlags; /*Bit 0 == 1: Disable device bit translate
                              other bits are reserved*/
} CBIOS_PARAM_INIT, *PCBIOS_PARAM_INIT;

typedef struct _CBIOS_PARAM_SHADOWINFO{
    PCBIOS_VOID  SysShadowAddr;
    CBIOS_U32    SysShadowLength;
}CBIOS_PARAM_SHADOWINFO, *PCBIOS_PARAM_SHADOWINFO;

typedef enum _CBIOS_PWM_CMD
{
    CBIOS_GET_PWM,
    CBIOS_SET_PWM,
    CBIOS_GET_PWM_CLK_PreDivide,
    CBIOS_SET_PWM_CLK_PreDivide
}CBIOS_PWM_CMD, *PCBIOS_PWM_CMD;

typedef enum _CBIOS_PWM_CLK_PREDIVIDE
{
    PWM_No_PreDivide,
    PWM_PreDivide_by_1point5,
    PWM_PreDivide_by_2,
    PWM_PreDivide_by_3,
    PWM_PreDivide_Reserved
}CBIOS_PWM_CLK_PREDIVIDE;

typedef enum _CBIOS_VCLK_CONFIG_TYPE
{
    VCLK_1quarter_PERICLK = 0x01,       //1/4(300MHz)
    VCLK_2over3_EICLK,                  //2/3(333MHz)
    VCLK_2over7_PERICLK,                //2/7(342MHz)
    VCLK_1over3_PERICLK                 //1/3(400MHz)
}CBIOS_VCLK_CONFIG_TYPE,*PCBIOS_VCLK_CONFIG_TYPE;

/***********************************/
/* from DDK */
typedef struct _CBIOS_x86_BIOS_ARGUMENTS {
    CBIOS_U32  Eax;
    CBIOS_U32  Ebx;
    CBIOS_U32  Ecx;
    CBIOS_U32  Edx;
    CBIOS_U32  Esi;
    CBIOS_U32  Edi;
    CBIOS_U32  Ebp;
}CBIOS_X86_BIOS_ARGUMENTS, *PCBIOS_X86_BIOS_ARGUMENTS;

#define  PVIDEO_X86_BIOS_ARGUMENTS  PCBIOS_X86_BIOS_ARGUMENTS
#define  VIDEO_X86_BIOS_ARGUMENTS   CBIOS_X86_BIOS_ARGUMENTS

typedef union _CBIOS_PWM_SETTING
{
    struct _CBIOS_PWM_ARG
    {
        CBIOS_BOOL PWM_IsEnable;
        CBIOS_U8   PWM_Index;//0:PWM0  1:PWM1 ...
        
        /*DutyCyle value = 256*(x/p), where x = high time of
         *the PWM signal and p = period of the PWM signal*/
        CBIOS_U8   PWM_DutyCycle;
    } CBIOS_PWM_Arg;

    CBIOS_PWM_CLK_PREDIVIDE   PWM_Clk_PreDivide_Value;
    
}CBIOS_PWM_SETTING, *PCBIOS_PWM_SETTING;

typedef struct _CBIOS_PWM_PARAMS
{
    CBIOS_U32          size; /*size of CBIOS_PWM_PARAMS*/
    CBIOS_PWM_CMD      PWM_Command;
    CBIOS_PWM_SETTING  PWM_Setting;
}CBIOS_PWM_PARAMS, *PCBIOS_PWM_PARAMS;

typedef enum _CBIOS_PWM_FUNCTION
{
    PWM_SET_FAN_CTRL_STATUS,
    PWM_GET_FAN_CTRL_STATUS,
    PWM_SET_BACKLIGHT_STATUS,
    PWM_GET_BACKLIGHT_STATUS,
}CBIOS_PWM_FUNCTION;

typedef struct _CBIOS_FAN_CTRL_ARG
{
    CBIOS_U32 ulSize;
    CBIOS_BOOL bIsSupportFanCtrl;
    CBIOS_BOOL bIsEnableFanCtrl;
    CBIOS_U8 byFanSpeedLevel;//level of fan speed, 0x00 is the highest, 0xFF is the lowest.
}CBIOS_FAN_CTRL_ARG, *PCBIOS_FAN_CTRL_ARG;

typedef struct _CBIOS_BACKLIGHT_CTRL_ARG
{
    CBIOS_U32 ulSize;
    CBIOS_BOOL bIsSupportBLCtrl;
    CBIOS_BOOL bIsEnableBLCtrl;
    CBIOS_U8 byBLLevel;//level of backlight. 0xFF is the brightest, 0x00 is the darkest.
    CBIOS_ACTIVE_TYPE DeviceType;
}CBIOS_BACKLIGHT_CTRL_ARG, *PCBIOS_BACKLIGHT_CTRL_ARG;

typedef union _CBIOS_PWM_FUNC_SETTING
{
    CBIOS_FAN_CTRL_ARG FanCtrlArg;
    CBIOS_BACKLIGHT_CTRL_ARG BLctrlArg;

}CBIOS_PWM_FUNC_SETTING, *PCBIOS_PWM_FUNC_SETTING;

typedef struct _CBIOS_PWM_FUNCTION_CTRL_PARAMS
{
    CBIOS_IN CBIOS_U32                  ulSize; /*size of CBIOS_PWM_PARAMS*/
    CBIOS_IN CBIOS_PWM_FUNCTION         PWMFunction;
    CBIOS_IN CBIOS_OUT CBIOS_PWM_FUNC_SETTING     PWMFuncSetting;
}CBIOS_PWM_FUNCTION_CTRL_PARAMS, *PCBIOS_PWM_FUNCTION_CTRL_PARAMS;


typedef struct _CBIOS_TEMP_PARAMS
{
    CBIOS_U32 size;          /*size of CBIOS_TEMP_PARAMS*/
    CBIOS_U16 Temp_mul_100;  /*value = (100 * centigrade temperature)*/
}CBIOS_TEMP_PARAMS, *PCBIOS_TEMP_PARAMS;

typedef struct _CBIOS_PARAM_POST {
    CBIOS_U32    Size;
    CBIOS_VOID*  FBVirtualLinearAddress;
    CBIOS_U32    MappedBufferSize;
} CBIOS_PARAM_POST, *PCBIOS_PARAM_POST;

typedef struct _CBIOS_PARAM_DUMP_REG {
    CBIOS_U32 Size;
    CBIOS_U32 DumpType;          /* by index or by group */
    CBIOS_U32 RegGroupID;        /* or Reg Index */
    CBIOS_U32 RegBufferLen;      /*for validate buffer size for group dump*/
    CBIOS_UCHAR* RegBuffer;
} CBIOS_PARAM_DUMP_REG, *PCBIOS_PARAM_DUMP_REG;

typedef struct _CBIOS_PARAM_GET_EDID {
    CBIOS_U32    Size;
    CBIOS_UCHAR  PortNumber;                /* Edid port number */
    CBIOS_UCHAR* EdidBuffer;
    CBIOS_U32    EdidBufferLen;             /* Edid buffer size */
    CBIOS_ACTIVE_TYPE Device;               /* Device Bitfiled */
    CBIOS_U32  Flag;                        /* Flag to tell  CBios use device bitfield or PortNumber to get Edid: 0: use port number, 1: use device bit field */
    CBIOS_U32    Reserved; 
} CBIOS_PARAM_GET_EDID, *PCBIOS_PARAM_GET_EDID;

typedef enum _CBIOS_HPD_TYPE
{
    CBIOS_TEST_NORMAL_HPD = 0,         /* If hot plug type is normal hot plug, driver usually will light this device after hot plug in*/
    CBIOS_TEST_EDID_READ,              /* This hot plug type, driver must get the device mode list again,*/ 
                                       /* And must set mode using the new prefered timing*/
    CBIOS_TEST_LINK_TRAINING,          /* Driver can do nothing */
    CBIOS_TEST_LINK_MAINTANANCE,       /* Drvier can do nothing */
    CBIOS_TEST_PATTERN,                /* Currently not supported*/ 
    CBIOS_TEST_INVALID_HOTIRQ = 0xFF   /* Means invalid hot plug type */
}CBIOS_HPD_TYPE, *PCBIOS_HPD_TYPE;

typedef enum _CBIOS_DP_PORT_HPD_TYPE
{
    CBIOS_DP_HPD_IN = 0,
    CBIOS_DP_HPD_OUT,
    CBIOS_DUALMODE_HPD_IN,
    CBIOS_DUALMODE_HPD_OUT,
    CBIOS_DP_PORT_HPD_INVALID = 0xFF
}CBIOS_DP_PORT_HPD_TYPE, *PCBIOS_DP_PORT_HPD_TYPE;

typedef struct _CBiosCustmizedDestTiming
{
    CBIOS_U32    Size;
    /*For Display Port device*/
    CBIOS_U32    LinkRate;             /* 6:1.62Gbps, 0xA: 2.7Gbps */
    CBIOS_U32    LaneCount;            /* 1: one lane, 2: two lanes, 4: four lanes */
    CBIOS_U32    TestPattern;          /* 0: No test pattern transmitted */
                                           /* 1: Color Ramps */
                                           /* 2: Black and white vertical lines */
                                           /* 3: color square */
    CBIOS_U32    ClockSynAsyn;         /* 0: Link clock and stream clock asynchronous */
                                           /* 1: Link clock and stream clock synchronous*/
    CBIOS_U32    DynamicRange;         /* 0: Vesa Range, 1: CEA range */
    CBIOS_U32    ColorFormat;          /* 0: RGB, 1: YcbCR422, 2:YCbCr444 */
    CBIOS_U32    YCbCrCoefficients;    /* 0: ITU601, 1: ITU709 */
    CBIOS_U32    BitDepthPerComponet;  /* 0: 6 Bits */
                                           /* 1: 8 bits */
                                           /* 2: 10bits */
                                           /* 3: 12bits */
                                           /* 4: 16bits */
    /*Normal timing settings */                                           
    CBIOS_U32    IsInterlaced;         /* 0: non-interlaced, 1: interlaced */
    CBIOS_U32    HorTotal;             /* Horizontal total of transmitted video stream in pixel count */
    CBIOS_U32    VerTotal;             /* Vertical total of transmitted video stream in line count */
    CBIOS_U32    HorSyncStart;         /* Horizontal active start from Hsync start in pixel count */
    CBIOS_U32    VerSyncStart;         /* Vertical active start from Vsync start in line count */
    CBIOS_U32    HorSyncWidth;         /* HSync width in pixel count */
    CBIOS_U32    HSyncPolarity;        /* 0: Negative, 1: Positive */ 
    CBIOS_U32    VSyncPolarity;        /* 0: Negative, 1: Positive */
    CBIOS_U32    VerSyncWidth;         /* VSync width in line count */
    CBIOS_U32    HorWidth;             /* Active video width in pixel count */
    CBIOS_U32    VerWidth;             /* Active video height in line count */
    CBIOS_U32    DClk;                 /* DClk in Hz / 10000 */
    
}CBiosCustmizedDestTiming, *PCBiosCustmizedDestTiming;

typedef struct _CBIOS_HPD_QUERY_DEVICE_STATUS {
    CBIOS_IN  CBIOS_U32 Size;
    CBIOS_IN  CBIOS_U32 GpioNumber;
    CBIOS_OUT CBIOS_U32 ExistedDevices;
    CBIOS_OUT CBIOS_U32 ExistedDeviceCaps;
    CBIOS_OUT CBIOS_HPD_TYPE HpdType;  /* Tell driver hot plug signal type */
    CBIOS_OUT PCBiosCustmizedDestTiming pRequestModeTiming; /*In normal case, this pointer will be CBIOS_NULL. */
                                                            /*If HpdType is CBIOS_TEST_PATTERN, the driver */
                                                            /*Should check this structure's value. */
    CBIOS_OUT CBIOS_U32 Reserved;
    CBIOS_OUT CBIOS_ACTIVE_TYPE HPDDevices; //Devices using given GPIO
    CBIOS_IN  CBIOS_U32 ulHPDBitMask;
}CBIOS_HPD_QUERY_DEVICE_STATUS, *PCBIOS_HPD_QUERY_DEVICE_STATUS;

typedef struct _CBIOS_HPD_INFO {
    CBIOS_IN  CBIOS_U32         Size;
    CBIOS_IN  CBIOS_U32         ulHPDBitMask;               /* Value of mm8504                                  */
    CBIOS_OUT CBIOS_HPD_TYPE    HpdType;                    /* Tell driver hot plug signal type                 */
    CBIOS_OUT PCBiosCustmizedDestTiming pRequestModeTiming; /* In normal case, this pointer will be CBIOS_NULL. */
                                                            /* If HpdType is CBIOS_TEST_PATTERN, the driver     */
                                                            /* Should check this structure's value.             */
    CBIOS_OUT CBIOS_ACTIVE_TYPE HPDDevices;                 /* Devices using given GPIO                         */
    CBIOS_OUT CBIOS_DP_PORT_HPD_TYPE DPPortHpdType;         /* This member is only for DP port                  */
}CBIOS_HPD_INFO, *PCBIOS_HPD_INFO;

typedef struct _CBIOS_MODE_KEY_PARAM
{
    CBIOS_U32       XRes;
    CBIOS_U32       YRes;
    CBIOS_U32       RefreshRate;    
}CBIOS_MODE_KEY_PARAM,*PCBIOS_MODE_KEY_PARAM;

typedef struct _CBIOS_PARAM_GET_MODELIST_FROM_EDID {
    CBIOS_U32    Size;
    CBIOS_UCHAR* EdidBuffer;
    CBIOS_U32    EdidBufferLen;         /* Edid buffer size */
    PCBIOS_MODE_KEY_PARAM pModeList;
    CBIOS_U32    ModeListBufferLen;               
} CBIOS_PARAM_GET_MODELIST_FROM_EDID, *PCBIOS_PARAM_GET_MODELIST_FROM_EDID;

typedef struct _CBIOS_PARAM_I2C_DATA {
    CBIOS_U32               Size;
    CBIOS_UCHAR             PortNumber;     /* Edid port number */
    CBIOS_UCHAR             SlaveAddress;
    CBIOS_UCHAR             OffSet;
    CBIOS_UCHAR*            Buffer;
    CBIOS_U32               BufferLen;      /* Edid buffer size */
    CBIOS_U32               Flags;          /* from OS, defined in DxgkDdiI2CReceiveDataFromDisplay in DDK*/
    CBIOS_U32               RequestType;    /* CBIOS_I2CDDCCI: from DDCCI; CBIOS_I2CHDCP: from HDCP; use macro */
    CBIOS_S32               bHDCPEnable;    /* indicate use DDC or HDCP */
    CBIOS_BOOL              bUseDevType;    /* = 1: driver send PortType, cbios will get correct I2C bus
                                               = 0: driver send PortNumber directly, for old driver only */
    CBIOS_ACTIVE_TYPE       PortType;
    CBIOS_U32               Reserved;
} CBIOS_PARAM_I2C_DATA, *PCBIOS_PARAM_I2C_DATA;



typedef struct _CBIOS_DEVICE_MODEINFOR{
    CBIOS_U16  XResolution;         /* CBIOS_IN: xReso in word type */
    CBIOS_U16  YResolution;         /* CBIOS_IN: yReso in word type */
    CBIOS_U16  ColorDepth;          /* CBIOS_IN: colordepth in word type */ 
    CBIOS_U16  RefreshRate;         /* CBIOS_IN: refreshrate in byte type */
    CBIOS_U16  DisplayDevice;       /* CBIOS_IN: device type combination */
} CBIOS_MODEINFOR_DEVICE, *PCBIOS_MODEINFOR_DEVICE;

typedef struct _CBIOS_PARAM_GET_TIMEOUT {
    CBIOS_U32    Size;
    PCBIOS_MODEINFOR_DEVICE Mode_On_Iga1;   
    PCBIOS_MODEINFOR_DEVICE Mode_On_Iga2;
    CBIOS_UCHAR  Reg_CR0B_D;              /* CBIOS_OUT: CROB MIU timeout level */
    CBIOS_UCHAR  Reg_CR0F_D;              /* CBIOS_OUT: CROF MIN timeout level */
} CBIOS_PARAM_GET_TIMEOUT, *PCBIOS_PARAM_GET_TIMEOUT;

typedef struct _CBIOS_PARAM_CLOCK {
    CBIOS_U32  ClockType;               /* Indicate the clock type */
    CBIOS_U32  ClockFreq;               /* The clock * 10000 */
} CBIOS_PARAM_CLOCK, *PCBIOS_PARAM_CLOCK;

typedef struct _CBIOS_HDMI_FORMAT_MTX
{
   CBIOS_U16   FormatNum;
   CBIOS_U16   XRes;
   CBIOS_U16   YRes;
   CBIOS_UCHAR Interlace;
   CBIOS_U16   RefRate[2];
   CBIOS_UCHAR AspectRatio;
   CBIOS_UCHAR DefaultRefRateIndex; 
   CBIOS_U32   PixelClock;
   /*CBIOS_U32 IsRealHDMIDevice; */ /* Evevy bit relate with device bit is used to judge if it a read HDMI device.*/
}CBIOS_HDMI_FORMAT_MTX, *PCBIOS_HDMI_FORMAT_MTX;

typedef struct _CBIOS_TIMING_ATTRIB
{
    CBIOS_U32    Size;
    CBIOS_U32    FormatNum;
    CBIOS_U16    XRes;
    CBIOS_U16    YRes;
    CBIOS_U16    RefreshRate;
    CBIOS_U32    PixelClock;     /* pixel clock value */
    CBIOS_UCHAR  AspectRatio;    /* 1 means 16:9, 0 means 4:3*/
    CBIOS_UCHAR  HVPolarity;     /* Hor/Ver Sync Polarity(MISC:11000000B)*/
    CBIOS_U16    HorTotal;       /* CR5F_1-0+CR5D_0+CR0=Round(Value/8)-5 */
    CBIOS_U16    HorDisEnd;      /* CR5F_3-2+CR5D_1+CR1=Round(Value/8)-1 */
    CBIOS_U16    HorBStart;      /* CR5F_5-4+CR5D_2+CR2=Round(Value/8) */
    CBIOS_U16    HorBEnd;        /* CR5_7+CR3_4-0 =Round(Value/8)&   0x003F*/
    CBIOS_U16    HorSyncStart;   /* CR5F_7-6+CR5D_4+CR4 =Round(Value/8)*/
    CBIOS_U16    HorSyncEnd;     /* CR5_B4-0 =Round(Value/8) &   0x001F*/
    CBIOS_U16    VerTotal;       /* CR5E_0+CR7_5+CR7_0+CR6=Value-2 */
    CBIOS_U16    VerDisEnd;      /* CR5E_1+CR7_6+CR7_1+CR12=Value-1 */
    CBIOS_U16    VerBStart;      /* CR5E_2+CR9_5+CR7_3+CR15=Value-1 */
    CBIOS_UCHAR  VerBEnd;        /* CR16 */
    CBIOS_U16    VerSyncStart;   /* CR5E_4+CR7_7+CR7_2+CR10 */
    CBIOS_UCHAR  VerSyncEnd;     /* CR11_3-0 */
}CBIOS_TIMING_ATTRIB, *PCBIOS_TIMING_ATTRIB;

typedef struct _CBIOS_DEVICE_POSITION
{
    CBIOS_U32   Size;
    CBIOS_U32   XMinimum;
    CBIOS_U32   XDefault;
    CBIOS_U32   XMaximum;

    CBIOS_U32   YMinimum;
    CBIOS_U32   YDefault;
    CBIOS_U32   YMaximum;

    CBIOS_U32   XPosition;
    CBIOS_U32   YPosition;
}CBIOS_DEVICE_POSITION, *PCBIOS_DEVICE_POSITION;

typedef struct _CBIOS_I2CCONTROL      /*For DDC-CI */
{
    CBIOS_U32   Size;
    CBIOS_U32   Command;          /* I2C_COMMAND_* */
    CBIOS_U32   dwCookie;         /* Context identifier returned on Open*/
    CBIOS_UCHAR Data;            /* Data to write, or returned byte */
    CBIOS_UCHAR Reserved[3];     /* Filler */
    CBIOS_U32   Flags;            /* I2C_FLAGS_* */
    CBIOS_U32   Status;           /* I2C_STATUS_* */
    CBIOS_U32   ClockRate;        /* Bus clockrate in Hz.*/
}CBIOS_I2CCONTROL, *PCBIOS_I2CCONTROL;

/* The following structure is for new setting mode logic */


typedef struct _CBios_Mode_Info_Ext
{
    CBIOS_U32    Size;
    CBIOS_U32    XRes;
    CBIOS_U32    YRes;
    CBIOS_U32    RefreshRate;
    CBIOS_U32    InterlaceProgressiveCaps;      /* Bit0: Progressive Caps Bit1:Interlace Caps */
    CBIOS_U32    AdapterDeviceFlags;            /* 0: Means adapter mode, 1:Means device mode */
    CBIOS_U32    DeviceFlags;                   /* Bit definition same as device bit definitions */
    CBIOS_U32    ColorDepthCaps;                /* Bit0: 32Bits ARGB color depth capability*/
                                                /* Bit1: 16Bits color depth capability*/
                                                /* Bit2: 8Bits color depth capability*/
                                                /* Bit3: ABGR888 capability */
                                                /* Bit4: ARGB 2101010 capability */ 
                                                /* Bit5: ABGR 2101010 capability */
    CBIOS_U32    AspectRatioCaps;               /* Bit0: 4:3 capability */
                                                /* Bit1: 16:9 capability */
    CBIOS_U32    NativeModeFlags;               /* =0: Means normal mode */
                                                /* =1: Means native mode */
    union
    {
        CBIOS_U32       ModeFlags;      
        struct
        {
            CBIOS_U32   isCEAMode           :1; /* Bit0 = 1, Means is a CE mode */
                                                /*      = 0, Means is a PC normal mode */
            CBIOS_U32   isAddedDevMode      :1; /* Bit1 = 1, Means is a added device mode */
                                                /*           In modes whose XRes x YRes is 1920x1080, 1280x720, or 720x480, we should make */
                                                /*           mode having RefreshRate 6000(3000) and mode having RefreshRate between 5900(2900) and */
                                                /*           5999(2999) paried with each other  */
                                                /*      = 0, Means is a normal mode */
                                                /* Bit2:15  14 bits for different timing types, 5 types at present*/
            CBIOS_U32   isEstablishedTiming :1; /*    bit2 = 1, the mode is from Established timing block */
            CBIOS_U32   isStandardTiming    :1; /*    bit3 = 1, the mode is from Standard timing block */
            CBIOS_U32   isDetailedTiming    :1; /*    bit4 = 1, the mode is from Detailed timing block */
            CBIOS_U32   isSVDTiming         :1; /*    bit5 = 1, the mode is from Short Video Descriptor */
            CBIOS_U32   isDTDTiming         :1; /*    bit6 = 1, the mode is from Detailed Timing Descriptor */
            CBIOS_U32   RsvdModeType        :9; /*    bit7:15  for future mode types use   */
            CBIOS_U32   Reserved            :2; /* Bit 17-16 2 bits reserved */
            CBIOS_U32   isPreferredMode     :1; /* Bit 18: Preferred mode flag*/
                                                /*    bit18 = 1: preferred mode*/
                                                /*    bit18 = 0: not preferred mode*/
            CBIOS_U32   RsvdModeFlags       :13;/* Other bits reserved for future use */
            
        };
    };
       
    
} CBiosModeInfoExt, *PCBiosModeInfoExt;

typedef struct _CBios_Source_Mode_Params
{
    CBIOS_U32    Size;
    CBIOS_U32    XRes;
    CBIOS_U32    YRes;
    CBIOS_U32    ColorDepth;
    CBIOS_U32    MemberInvalidFlags;       /* Bit0=1: XRes parameter is invalid, or is valid. */
                                               /* Bit1=1: YRes parameter is invalid, or is valid.*/
                                               /*  */
}CBiosSourceModeParams, *PCBiosSourceModeParams;

typedef union _CBios_Device_Technology
{
    CBIOS_U32    TVTechnology;     /* =0x00; NTSCJP */
                                       /* =0x04; NTSCUS */
                                       /* =0x08; PAL */
                                       /* =0x09; PAL-M */
                                       /* =0x0A; PAL-NC */
    CBIOS_U32    HDMITchnology;    /* =0x1; RGB signal */
                                       /* =0x2; YCbCr422 signal */
                                       /* =0x4; YCbCr444 signal */
                                       /* DP device will also use this attribute, and is called Color format */
}CBiosDeviceTechnology,*PCBiosDeviceTechnology;

typedef struct _CBios_Dest_Mode_Params
{
    CBIOS_U32    Size;
    CBIOS_U32    XRes;
    CBIOS_U32    YRes;
    CBIOS_U32    RefreshRate;
    CBIOS_U32    InterlaceFlag;     /* =0, Set noninterlace mode; = 1, Set interlace mode; */
    CBIOS_U32    AspectRatioFlag;   /* =0, Default aspect ratio */
                                        /* =1, 4:3 aspect ratio*/
                                        /* =2, 16:9 aspect ratio */
    CBiosDeviceTechnology Technology; 
    CBIOS_U32    ReservedFlags;     /* Bit0: Override Edid timing flags */
                                        /*       =0, Default */
                                        /*       =1, not use edid timing, use vesa table timing */
                                        /* Bit1: Reserved */
    CBIOS_U32    ReservedPtr;       /* Can used as input timing data pointer.  */         
    CBIOS_U32    MemberInvalidFlags; /* Bit0=1: XRes parameter is invalid, or is valid.*/
                                         /* Bit1=1:YRes parameter is invalid, or is valid.*/
                                         /* Bit2=1:RefreshRate parameter is invalid, or is valid.*/
}CBiosDestModeParams, *PCBiosDestModeParams;

typedef struct _CBios_ScalerSize_Params
{
    CBIOS_U32    Size;
    CBIOS_U32    XRes;
    CBIOS_U32    YRes;
    CBIOS_U32    MemberInvalidFlags;   /* Bit0=1: XRes parameter is invalid, or is valid. */
                                           /* Bit1=1:YRes parameter is invalid, or is valid. */
                                           /* ....... */

}CBiosScalerSizeParams, *PCBiosScalerSizeParams;

typedef struct _CBios_DefaultClock_Params
{
    CBIOS_IN             CBIOS_U32     Size;
    CBIOS_IN CBIOS_OUT   CBIOS_U32     *ClockFreq;     /* CBIOS_IN: this pointer must be defined */
    CBIOS_IN             CBIOS_U32     ClockType;      /* CBIOS_MCLKTYPE and so on*/
    CBIOS_IN             CBIOS_UCHAR   ClockTuneMethod;/* 0-software method, 1-hardware frequency division method*/
    CBIOS_IN CBIOS_OUT   CBIOS_U32     HardwareClockFrequencylevel;
    CBIOS_IN             CBIOS_U32     ClockFreqType;/* 0-default freq; 1-min freq; 2-max freq */
    CBIOS_IN             CBIOS_U32     ClockFeature; /* bit[0] = 1: clock up, bit[0] = 0: clock down
                                                            bit[1] = 1: MCLK change, bit[1] = 0: MCLK not change
                                                            bit[2] = 1: ECLK change, bit[2] = 0: ECLK not change
                                                            bit[3] = 1: ICLK change, bit[3] = 0: ICLK not change
                                                         */
    CBIOS_IN             CBIOS_U32     *pMClockFreq;  /* CBIOS_IN: When ClockFeature[1] = 1, this pointer must be defined */
    CBIOS_IN             CBIOS_U32     *pIClockFreq;  /* CBIOS_IN: When ClockFeature[2] = 1, this pointer must be defined */
    CBIOS_IN             CBIOS_U32     *pEClockFreq;  /* CBIOS_IN: When ClockFeature[3] = 1, this pointer must be defined */
}CBiosDefaultClockParams, *PCBiosDefaultClockParams;

typedef struct _CBios_GetClock_Params
{
    CBIOS_IN             CBIOS_U32     Size;
    CBIOS_IN CBIOS_OUT   CBIOS_U32     *ClockFreq;     /* CBIOS_IN: this pointer must be defined */
    CBIOS_IN             CBIOS_U32     ClockType;      /* CBIOS_MCLKTYPE and so on*/
}CBios_GetClock_Params, *PCBios_GetClock_Params;

typedef struct _CBios_SetClock_Params
{
    CBIOS_IN             CBIOS_U32                      Size;
    CBIOS_IN             CBIOS_U32                      ClockFreq;   
    CBIOS_IN             CBIOS_U32                      ClockType;      // CBIOS_ICLKTYPE and CBIOS_VCLKTYPE
    CBIOS_IN             CBIOS_VCLK_CONFIG_TYPE         VclkConfig;     //for vclk only
}CBios_SetClock_Params, *PCBios_SetClock_Params;

typedef struct _CBios_VoltageSetting_Params
{
    CBIOS_U32      Size;                         /*struct size, used for future  revsion contol*/
    CBIOS_UCHAR    IsVoltageScalingSupported; /*where the card supported Voltage Scaling*/
    CBIOS_S32      FunctionNO;            /*1-Get Voltage Params and current Volage, 2-Set Voltage  value,4-query is voltage scale support */                  
    CBIOS_UCHAR    method;               /*0-I2c,1-GPIO*/    
    CBIOS_UCHAR    I2CValueNumber;     /*used for I2C,indicate support how much value levell */
    CBIOS_UCHAR    DefaultVoltageValue;/*VBIOS post VoltageValue*/
    CBIOS_UCHAR    CurrentVoltageValue;/*current VoltageValue*/
    CBIOS_UCHAR    ExpectedValue;    /*driver set ,for I2c, just use the value level,I2CValueNumber-1 indicates the highest level,
                                                        the lowest level is 0
                                                        for GPIO,1- high level,0 -low level*/

}CBiosVoltageSettingParams, *PCBiosVoltageSettingParams;

typedef struct _CBIOS_SUPPORT_BPC_FLAGS
{
    CBIOS_BOOL   IsSupport6BPC   :1;
    CBIOS_BOOL   IsSupport8BPC   :1;
    CBIOS_BOOL   IsSupport10BPC  :1;
    CBIOS_BOOL   IsSupport12BPC  :1;
    CBIOS_BOOL   IsSupport14BPC  :1;
    CBIOS_BOOL   IsSupport16BPC  :1;
    CBIOS_BOOL   RservedBPC      :26;
}CBIOS_SUPPORT_BPC_FLAGS, *PCBIOS_SUPPORT_BPC_FLAGS;

typedef struct _CBiosQueryConnectedMonitorAttributeParams
{
    CBIOS_IN    CBIOS_U32           Size;
    CBIOS_IN    CBIOS_ACTIVE_TYPE   ActiveType;     /*Encoder or legacy active_type*/
    CBIOS_OUT   CBIOS_MONITOR_TYPE  MonitorType;    //if current port is split, this variable stores the monitor type of Y channel
                                                    //if current port is not split, this variable stores the monitor type of this port
    CBIOS_OUT   CBIOS_U32           MonitorCaps;    /*bit[0]=1: device support CEA861*/
                                                    /*bit[1]=1: RGB  support*/
                                                    /*bit[2]=1: YCrCr4:2:2 support*/
                                                    /*bit[3]=1: YCbCr4:4:4 support*/
    CBIOS_OUT   CBIOS_HDTV_ENCODER  HdtvEncoder;
    CBIOS_OUT   CBIOS_BOOL          bSupportAudio;  // Encoder and HDMI monitor both support audio
    CBIOS_OUT   CBIOS_HDCP_INDEX    HdcpIndex;      // Device related HDCP index
    CBIOS_OUT   CBIOS_UCHAR         MonitorID[8];
    CBIOS_OUT   CBIOS_MONITOR_TYPE  MonitorType_ZCH;//Monitor type on Z channel. If current port doesn't split, this variable will be set to CBIOS_TYPE_NONE
    CBIOS_OUT   CBIOS_U32           MonitorCaps_ZCH;//Monitor caps on Z channel. Same definition with MonitorCaps.
    CBIOS_OUT   CBIOS_BOOL          bSupAudio_ZCH;  // support audio on Z channel.
    CBIOS_OUT   CBIOS_UCHAR         ZCH_MonitorID[8];//z channel monitor ID
    CBIOS_OUT   CBIOS_SUPPORT_BPC_FLAGS SupportBPC;
    CBIOS_OUT   CBIOS_U16           MonitorHorSize; //Monitor screen image horizontal size in millimeter(mm), if calculate DPI should convert it to inch, 1 inch = 25.4 mm
    CBIOS_OUT   CBIOS_U16           MonitorVerSize; //Monitor screen image vertical size in millimeter(mm), if calculate DPI should convert it to inch, 1 inch = 25.4 mm
}CBiosQueryConnectedMonitorAttributeParams, *PCBiosQueryConnectedMonitorAttributeParams;

typedef struct _CBiosContentProtectionOnOffParams
{
    CBIOS_IN    CBIOS_U32           Size;
    CBIOS_IN    CBIOS_ACTIVE_TYPE   Devices;
    CBIOS_IN    CBIOS_U32           bHdcpStatus;
}CBiosContentProtectionOnOffParams, *PCBiosContentProtectionOnOffParams;

typedef struct _CBiosAccessDpcdDataParams
{
    CBIOS_IN            CBIOS_U32       Size;
    CBIOS_IN            CBIOS_U32       ReadWriteFlag;   /* 0: Means read, 1: Means write.*/
    CBIOS_IN            CBIOS_ACTIVE_TYPE   RequestConnectedDevice;  /*Encoder or legacy active_type*/
    CBIOS_IN            CBIOS_U32       StartingAddress;
    CBIOS_IN            CBIOS_U32       NumOfBytes;
    CBIOS_IN CBIOS_OUT  CBIOS_UCHAR     *pOutBuffer;
}CBiosAccessDpcdDataParams, *PCBiosAccessDpcdDataParams;

typedef struct _CBiosSpecifyDstTimingSrc
{
    CBIOS_U32 Size;
    CBIOS_U32 Flag;                                /* 0: Use CBIOS default method to select where timing come from*/
                                                   /* 1: Use user input customized timing, if this value is set, pUserCustDestTiming can not be CBIOS_NULL*/
                                                   /* 2: Use automated test DPCD timing*/
                                                   /* other value: reserved */
    CBiosCustmizedDestTiming UserCustDestTiming;   /* If Flag is set to 1, this structure is the user defined timing buffer*/
}CBiosSpecifyDstTimingSrc, *PCBiosSpecifyDstTimingSrc;

#define CB_SET_MODE_PS_SS_SEPARATE_FLAG               BIT3
#define CB_SET_MODE_DIU_PS_ENABLE_COMPRESSION_FLAG    BIT4
#define CB_SET_MODE_DIU_SS_ENABLE_COMPRESSION_FLAG    BIT5

typedef enum _CBIOS_3D_STRUCTURE
{
    FRAME_PACKING = 0x00,
    FIELD_ALTERNATIVE,
    LINE_ALTERNATIVE,
    SIDE_BY_SIDE_FULL,
    L_DEPTH,
    L_DEPTH_GRAPHICS,
    TOP_AND_BOTTOM,
    RESERVED_3D_STRUCTURE,
    SIDE_BY_SIDE_HALF,
    NOT_IN_USE_3D_STRUCTURE = 0x0F,
}CBIOS_3D_STRUCTURE, *PCBIOS_3D_STRUCTURE;

typedef enum _CBIOS_FORMAT
{
    CBIOS_FMT_INVALID                       = 0x00,
    CBIOS_FMT_P8,
    CBIOS_FMT_R5G6B5,
    CBIOS_FMT_A1R5G5B5,
    CBIOS_FMT_A8R8G8B8,
    CBIOS_FMT_A8B8G8R8,
    CBIOS_FMT_X8R8G8B8,
    CBIOS_FMT_X8B8G8R8,
    CBIOS_FMT_A2R10G10B10,
    CBIOS_FMT_A2B10G10R10,
    CBIOS_FMT_CRYCBY422_16BIT,
    CBIOS_FMT_YCRYCB422_16BIT,
    CBIOS_FMT_CRYCBY422_32BIT,
    CBIOS_FMT_YCRYCB422_32BIT,
    CBIOS_FMT_YCBCR8888_32BIT,
    CBIOS_FMT_YCBCR2101010_32BIT,
}CBIOS_FORMAT, *PCBIOS_FORMAT;

typedef struct _CBios_Setting_Mode_Params
{
    CBIOS_U32                  Size;
    CBIOS_U32                  ulCBiosVersion;
    CBiosSourceModeParams      SourcModeParams;
    CBiosDestModeParams        DestModeParams;
    CBiosScalerSizeParams      ScalerSizeParams;
    CBIOS_U32                  IGAIndex;                /* Specify which IGA need to be set mode, this can not be 0. */
    CBIOS_U32                  MemberInvalidFlags;      /* Bit0=1: SourcModeParams parameter is invalid, or is valid. */
                                                        /* Bit1=1: DestModeParams parameter is invalid, or is valid. */
                                                        /* Bit2=1: ViewPortParams parameter is invalid, or is valid. */
                                                        /* Bit3=1: PS/SS are separated on this IGA */
                                                        /* Bit4=1: DIU PS enable compression mode */
                                                        /* Bit5=1: DIU SS enable compression mode */
    CBiosSpecifyDstTimingSrc   SpecifyTiming;           /* CustDestTiming structure value to set destination timing*/
                                                        /* Normally this structure is useless, if driver do not want to specified the dest timing*/
                                                        /* Notices, in this case, the other member parameter still need be set right */
    struct  
    {
        CBIOS_U32              Is3DVideoMode        :1; /* = 1: 3D Video mode, and 3D format is in Video3DFormat; = 0: normal 2D mode*/
        CBIOS_U32              Reserved             :31;/* Reserved for future use*/
    };
    CBIOS_3D_STRUCTURE         Video3DStruct;
    CBIOS_U32                  BitPerComponent;
    CBIOS_FORMAT               Format; 
}CBiosSettingModeParams,*PCBiosSettingModeParams;

typedef struct _CBios_Setting_Downscaler_Params
{
    CBIOS_U32                  Size;
    CBIOS_U32                  DownscalerDestinationBase;
    CBIOS_U32                  SourceModeXResolution;
    CBIOS_U32                  SourceModeYResolution;
    CBIOS_U32                  DownscalerDestinationPitch;
}CBiosSettingDownscalerParams,*PCBiosSettingDownscalerParams;

typedef struct _CBIOS_VERSION
{
    CBIOS_U16     MajorVersionNumber;       /* Big structure refine if change. */
    CBIOS_U16     ChipID;                   /* For which chip. 0 means general version for all chip. */
    CBIOS_U16     ChipRevID;                /* For which revision chip. 0 means no revision need to differ */
    CBIOS_U16     BranchDriverNumber;       /* Branch code number, 0 means trunk code. */
    CBIOS_U16     MediumVersionNumber;      /* Little improvement, Add new feature */
    CBIOS_U16     MinorVersionNumber;       /* Minor fix improvement, mainly focus on bug fix. */
}CBIOS_VERSION,*PCBIOS_VERSION;

typedef struct _CBREGISTER
{
    CBIOS_UCHAR    type;
    CBIOS_UCHAR    mask;
    CBIOS_UCHAR    index;
    CBIOS_UCHAR    value;
} CBREGISTER, *PCBREGISTER;

typedef struct _CBREGISTER_IDX
{
    CBIOS_U16      type_index;
    CBIOS_UCHAR    mask;
} CBREGISTER_IDX, *PCBREGISTER_IDX;

typedef struct _CBIOS_BOOT_STRAP_REGS
{
    CBIOS_U32    Size;
    CBIOS_U32    ulCBiosVersion;
    CBREGISTER   BootStrapReg[10];
    CBIOS_U32    RegCount;
}CBIOS_BOOT_STRAP_REGS,*PCBIOS_BOOT_STRAP_REGS;


typedef enum _CBIOS_REGISTER_BLOCK_TYPE
{
    CBIOS_REGISTER_MMIO = 0,
    CBIOS_REGISTER_PMU,
    CBIOS_REGISTER_GPIO,
    CBIOS_REGISTER_PMC
}CBIOS_REGISTER_BLOCK_TYPE;

typedef struct _CBIOS_CALLBACK_FUNCTIONS
{
    CBIOS_U32       Size;
    CBIOS_VOID*     pFnDbgPrint;
    CBIOS_VOID*     pFnDelayMicroSeconds;
    CBIOS_VOID*     pFnReadUchar;
    CBIOS_VOID*     pFnReadUshort;
    CBIOS_VOID*     pFnReadUlong;
    CBIOS_VOID*     pFnWriteUchar;
    CBIOS_VOID*     pFnWriteUshort;
    CBIOS_VOID*     pFnWriteUlong;
    CBIOS_VOID*     pFnReadBufferUlong;
    CBIOS_VOID*     pFnWriteBufferUlong;
    CBIOS_VOID*     pFnQuerySystemTime;
    CBIOS_VOID*     pFnAllocateNonpagedMemory;
    CBIOS_VOID*     pFnFreeNonpagedMemory;
    CBIOS_VOID*     pFnGetDeviceBase;
    CBIOS_VOID*     pFnFreeDeviceBase;
    CBIOS_VOID*     pFnFreePool;
    CBIOS_VOID*     pFnAcquireSpinLock;
    CBIOS_VOID*     pFnReleaseSpinLock;
    CBIOS_VOID*     pFnReadPortUchar;
    CBIOS_VOID*     pFnWritePortUchar;
    CBIOS_VOID*     pFnGetRegistryParameters;
    CBIOS_VOID*     pFnSetRegistryParameters;
    CBIOS_VOID*     pFnGetRegistryParametersWithCallback;
    CBIOS_VOID*     pFnStrcmp;
    CBIOS_VOID*     pFnStrcpy;
    CBIOS_VOID*     pFnStrncmp;
    CBIOS_VOID*     pFnMemset;
    CBIOS_VOID*     pFnMemcpy;
    CBIOS_VOID*     pFnMemcmp;
    CBIOS_VOID*     pFnDodiv;
    CBIOS_VOID*     pFnVsprintf;
    CBIOS_VOID*     pFnWriteRegisterU32;
    CBIOS_VOID*     pFnReadRegisterU32;
}CBIOS_CALLBACK_FUNCTIONS, *PCBIOS_CALLBACK_FUNCTIONS;

typedef struct _CBIOS_HOTPLUG_PARAM
{
    CBIOS_U32 Size;
    CBIOS_U32 TurnOnHotplug;
}CBIOS_HOTPLUG_PARAM, *PCBIOS_HOTPLUG_PARAM;

typedef struct _CBIOS_VBIOS_DATA_PARAM
{
    CBIOS_U32 Size;
    CBIOS_U32 SyncToVbios;    // 1 means sync from cbios to vbios; 
                                  // 0 means sync from vbios to cbios;
}CBIOS_VBIOS_DATA_PARAM,*PCBIOS_VBIOS_DATA_PARAM;

typedef struct _CBIOS_DISPLAY_TIMING_PARAM
{
    CBIOS_U32 Size;
    CBIOS_U32 DisplayDeviceDWord;
    CBIOS_U32 HorizontalResolution;
    CBIOS_U32 VerticalResolution;
    CBIOS_U32 RefreshRateX100;
}CBIOS_DISPLAY_TIMING_PARAM,*PCBIOS_DISPLAY_TIMING_PARAM;

typedef struct _CBIOS_CHIP_ID
{
    CBIOS_U32 Size;
    CBIOS_U32 GenericChipID;
    CBIOS_U32 ChipID;
}CBIOS_CHIP_ID,*PCBIOS_CHIP_ID;

typedef struct _CBIOS_RESET_HW_BLOCK_PARAM
{
    CBIOS_IN CBIOS_U32 Size;
    CBIOS_IN CBIOS_HW_BLOCK HwBlock;
}CBIOS_RESET_HW_BLOCK_PARAM,*PCBIOS_RESET_HW_BLOCK_PARAM;

typedef struct _CBIOS_TUNNEL_2D_TO_VBIOS_PARAM
{
    CBIOS_IN  CBIOS_U32              Size;
    CBIOS_IN  CBIOS_VOID*            pTunnel; //2D driver need allocat new space for tunnel table and 
                                                  //this pointer point to the new space
    CBIOS_IN  CBIOS_OUT CBIOS_U32    SizeOfTunnel; //CBIOS will return the size of current tunnel table
}CBIOS_TUNNEL_2D_TO_VBIOS_PARAM,*PCBIOS_TUNNEL_2D_TO_VBIOS_PARAM;

typedef struct _CBIOS_PARAM_SET_EDID{
    CBIOS_IN    CBIOS_U32           Size;
    CBIOS_IN    CBIOS_ACTIVE_TYPE   Device;             /* Device Bitfiled */
    CBIOS_IN    PCBIOS_UCHAR        EdidBuffer;         /* When ==NULL, CBIOS will unlock the EDID and
                                                           Driver must do device detect to get the real EDID 
                                                        */
    CBIOS_IN    CBIOS_U32           EdidBufferLen;      /* Edid buffer size */
} CBIOS_PARAM_SET_EDID, *PCBIOS_PARAM_SET_EDID;

typedef struct _CBIOS_GET_ALWAYS_ON_DEVICES_PARAM
{
    CBIOS_IN   CBIOS_U32 Size;
    CBIOS_OUT  CBIOS_U32 AlwaysOnDev;
}CBIOS_GET_ALWAYS_ON_DEVICES_PARAM,*PCBIOS_GET_ALWAYS_ON_DEVICES_PARAM;

typedef struct _CBIOS_CHECK_DEV_COMBIN_PARAM
{
    CBIOS_IN   CBIOS_U32 Size;
    CBIOS_IN   CBIOS_U32 Flag;       /* CBIOS_CHECK_DEV_COMBIN_FLAG:
                                            bit0: Check Device DeviceCombination;
                                            bit1: Check Simultaneous Support */
    CBIOS_IN   CBIOS_U32 DevCombin;
    CBIOS_IN   CBIOS_U32 IGAIndex;
    CBIOS_OUT  CBIOS_UCHAR bSupported;
}CBIOS_CHECK_DEV_COMBIN_PARAM,*PCBIOS_CHECK_DEV_COMBIN_PARAM;

typedef enum _CBIOS_TIMING_TYPE{  //can extend to support 14 different types  
    CBIOS_NONEDID_TIMING = 0x0000,    //The mode is not from EDID
    CBIOS_EST_TIMING = 0x0001,
    CBIOS_STD_TIMING = 0x0002,
    CBIOS_DTL_TIMING = 0x0004,
    CBIOS_DTD_TIMING = 0x0008,        //Detailed timing descriptor
    CBIOS_SVD_TIMING = 0x0010         //CBIOS_S16 video descriptor
}CBIOS_TIMING_TYPE, *PCBIOS_TIMING_TYPE;

typedef struct _CBIOS_GET_HOT_KEY_SWITCH_DEV_PARAM
{
    CBIOS_IN            CBIOS_U32 Size;
    CBIOS_OUT           CBIOS_U32 ulOutputSize;
    CBIOS_IN CBIOS_OUT  CBIOS_U32 *pBuffDev;
    CBIOS_IN            CBIOS_U32 ulBuffDevLen;
}CBIOS_GET_HOT_KEY_SWITCH_DEV_PARAM, *PCBIOS_GET_HOT_KEY_SWITCH_DEV_PARAM;

typedef struct _CBIOS_HW_CHECK
{
    CBIOS_U32                size;
    PCBIOS_BOOT_STRAP_REGS   pCBBootStrapReg;
    CBIOS_VOID*              pFBVirtualLinearAddr; //at least to map more than 16KB for test use
}CBIOS_HW_CHECK, *PCBIOS_HW_CHECK;

typedef struct _CBIOS_MODE_EXT
{
    CBIOS_IN        CBIOS_U32           Size;
    CBIOS_IN        CBIOS_U32           XRes;
    CBIOS_IN        CBIOS_U32           YRes;
    CBIOS_IN        CBIOS_U32           RefreshRate;
    CBIOS_IN        CBIOS_BOOL          bInterlaced;        /* 0: progressive mode;1: interlaced mode; */
    CBIOS_IN        CBIOS_UCHAR         AspectRatioFlag;    /* =0 means 4:3, =1 means 16:9 */
}CBIOS_MODE_EXT, *PCBIOS_MODE_EXT;

typedef struct _CBIOS_TIMING_EXT
{
    CBIOS_IN        CBIOS_U32           Size;
    CBIOS_OUT       CBIOS_U16           XResolution;
    CBIOS_OUT       CBIOS_U16           YResolution;
    CBIOS_OUT       CBIOS_U16           RefreshRate;
    CBIOS_OUT       CBIOS_U32           PixelClock;         /*The arithmetical unit is Hz*/
    CBIOS_OUT       CBIOS_U8            HVPolarity;
    CBIOS_OUT       CBIOS_U16           HorTotal;
    CBIOS_OUT       CBIOS_U16           HorDisEnd;
    CBIOS_OUT       CBIOS_U16           HorBStart;
    CBIOS_OUT       CBIOS_U16           HorBEnd;
    CBIOS_OUT       CBIOS_U16           HorSyncStart;
    CBIOS_OUT       CBIOS_U16           HorSyncEnd;
    CBIOS_OUT       CBIOS_U16           VerTotal;
    CBIOS_OUT       CBIOS_U16           VerDisEnd;
    CBIOS_OUT       CBIOS_U16           VerBStart;
    CBIOS_OUT       CBIOS_U16           VerBEnd;
    CBIOS_OUT       CBIOS_U16           VerSyncStart;
    CBIOS_OUT       CBIOS_U16           VerSyncEnd;
}CBIOS_TIMING_EXT,*PCBIOS_TIMING_EXT;

typedef struct _CBIOS_GET_MODE_TIMING_PARAM
{
    CBIOS_IN                 CBIOS_U32               Size;
    CBIOS_IN                 CBIOS_ACTIVE_TYPE       Device;
    CBIOS_IN                 PCBIOS_MODE_EXT         pModeExt;
    CBIOS_IN   CBIOS_OUT     PCBIOS_TIMING_EXT       pTimingExt;
}CBIOS_GET_MODE_TIMING_PARAM, *PCBIOS_GET_MODE_TIMING_PARAM;

#pragma pack (1)
typedef struct _CBIOS_ELD_MEM_STRUCT
{
    CBIOS_U32    Size;
    union
    {
        struct 
        {
            //ELD Header
            CBIOS_U8 Reserved0          : 3;//reserved bit set to zero.
            CBIOS_U8 ELD_Ver            : 5;//[optional]
            CBIOS_U8 Reserved1;
            CBIOS_U8 BaseLine_Eld_len;      //[optional]
            CBIOS_U8 Reserved2;
            //BaseLineBlock
            CBIOS_U8 MNL                :5;//[required]the Length of Monitor_Name_String,at most 16 bytes, 0 means no name
            CBIOS_U8 CEA_EDID_Ver       :3;//[required]the CEA EDID Timing Extensin Version number of the HDMI sink Device supports.
                                       // 000b: no CEA EDID Timing Extension Block Present
                                       // 001b: CEA-861.
                                       // 010b: CEA-861A.
                                       // 011b: CEA-861B,C,or D.
                                       // other value reserved.
            CBIOS_U8 HDCP               :1;// [optional]
            CBIOS_U8 S_AI               :1;// [optional]
            CBIOS_U8 Conn_Type          :2;// [required]
                                       // 00b: HDMI 
                                       // 01b: Display Port
            CBIOS_U8 SAD_Count          :4;// [required] at most 15 SADs
                                          // the number of CEA SAD following the Monitor_Name_String
            CBIOS_U8 Aud_Synch_Delay;      // [optional]
            CBIOS_U8 FLR                :1;// [required]
            CBIOS_U8 LFE                :1;// [required]
            CBIOS_U8 FC                 :1;// [required]
            CBIOS_U8 RLR                :1;// [required]
            CBIOS_U8 RC                 :1;// [required]
            CBIOS_U8 FLRC               :1;// [required]
            CBIOS_U8 RLRC               :1;// [required]
            CBIOS_U8 Rsvd               :1;// [required]
            CBIOS_U8 Port_ID[8];           // [optional]
            CBIOS_U8 ManufactureName[2];   // [required]
            CBIOS_U8 ProductCode[2];       // [required]
            CBIOS_U8 Monitor_Name_String[16];//[required].
            CBIOS_U8 CEA_SADs[15][3];          //[required].
        }ELD_Data;
        CBIOS_U8 Data[128];// Max Length is 128 byte
    };
}CBIOS_ELD_MEM_STRUCT,*PCBIOS_ELD_MEM_STRUCT;

typedef struct _CBIOS_ELD_FIFO_STRUCTURE
{
    CBIOS_U32 Size;
    union
    {
        struct 
        {
            //ELD Header
            CBIOS_U8 Reserved0          : 3;//reserved bit set to zero.
            CBIOS_U8 ELD_Ver            : 5;//[optional]
            CBIOS_U8 Reserved1;
            CBIOS_U8 BaseLine_Eld_len;      //[optional]
            CBIOS_U8 Reserved2;

            //BaseLineBlock
            CBIOS_U8 MNL                :5;//[required]the Length of Monitor_Name_String
            CBIOS_U8 CEA_EDID_Ver       :3;//[required]the CEA EDID Timing Extensin Version number of the HDMI sink Device supports.
                                       // 000b: no CEA EDID Timing Extension Block Present
                                       // 001b: CEA-861.
                                       // 010b: CEA-861A.
                                       // 011b: CEA-861B,C,or D.
                                       // other value reserved.
            CBIOS_U8 HDCP               :1;// [optional]
            CBIOS_U8 S_AI               :1;// [optional]
            CBIOS_U8 Conn_Type          :2;// [required]
                                       // 00b: HDMI 
                                       // 01b: Display Port
            CBIOS_U8 SAD_Count          :4;// [required]
                                       // the number of CEA SAD following the Monitor_Name_String

            CBIOS_U8 Aud_Synch_Delay;      // [optional]

            CBIOS_U8 FLR                :1;// [required]
            CBIOS_U8 LFE                :1;// [required]
            CBIOS_U8 FC                 :1;// [required]
            CBIOS_U8 RLR                :1;// [required]
            CBIOS_U8 RC                 :1;// [required]
            CBIOS_U8 FLRC               :1;// [required]
            CBIOS_U8 RLRC               :1;// [required]
            CBIOS_U8 Rsvd               :1;// [required]

            //BYTE Port_ID[8];           // [optional]
            //LUID Port_ID;
            struct
                 {
                     CBIOS_U32 LowPart;
                     CBIOS_U32 HighPart;
                 } Port_ID;

            CBIOS_U8 ManufactureName[2];   // [required]
            CBIOS_U8 ProductCode[2];       // [required]
            //BYTE Monitor_Name_String[?];//[required].
            //BYTE CEA_SADs[3];        [required].
            CBIOS_U8 Monitor_Name_String[1];
        }ELD_Data;
        CBIOS_U8 Data[128];// Max Length is 128 byte
    };
}CBIOS_ELD_FIFO_STRUCTURE, *PCBIOS_ELD_FIFO_STRUCTURE;

#pragma pack ()

typedef struct _CBIOS_GET_ELD_PARAM
{
    CBIOS_IN                 CBIOS_U32               Size;
    CBIOS_IN                 CBIOS_ACTIVE_TYPE       Device;
    CBIOS_IN   CBIOS_OUT     PCBIOS_ELD_MEM_STRUCT   pELDMemData;
}CBIOS_GET_ELD_PARAM, *PCBIOS_GET_ELD_PARAM;
 
typedef  struct  _PCBIOS_VBINFO_PARAM  
{
    CBIOS_U32              Size;
    CBIOS_U32              BiosVersion;                //Vbios version
    CBIOS_U32              VBIOSSwitch3;               //Vbios feature switch 3
    CBIOS_U32              MCKTuneMethod    :1;        //Mclk tune method, 0:software, 1:hardware
    CBIOS_U32              ECKTuneMethod    :1;        //Eclk tune method, 0:software, 1:hardware
    CBIOS_U32              ICKTuneMethod    :1;        //Iclk tune method, 0:software, 1:hardware
    CBIOS_U32              PCIClaimedFBSize :4;        //PCI BAR1 claimed frame buffer size
    CBIOS_U32              bNoHDCPSupport   :1;        //E2UMA: strapping CR56[2]
    CBIOS_U32              Reserved         :24;
    CBIOS_U32              DeviceID;                   //Chip Device ID
    CBIOS_U32              RevisionID;                 //Chip revision ID
    CBIOS_U32              MemSize;                    //Memory size read from scratch pad
    CBIOS_U32              SupportDev;                 //VBIOS supported devices
    CBIOS_U32              PortSplitSupport;           //bit definition is the same as CBIOS_ACTIVE_TYPE, 
                                                       //1:corresponding device port can be split
                                                       //0: corresponding device port can not be split
    CBIOS_U32              HPDBitMask;
    CBIOS_U32              HPDDevicesMask;
    CBIOS_ACTIVE_TYPE      BootDevInCMOS;              //for GOP
    CBIOS_U32              PollingDevMask;

}CBIOS_VBINFO_PARAM, *PCBIOS_VBINFO_PARAM;

typedef struct _CBIOS_DEVICEMODELIST_PARA
{
    CBIOS_IN    CBIOS_U32               ulSize;
    CBIOS_IN    CBIOS_ACTIVE_TYPE       DevicePort;
    CBIOS_IN    CBIOS_U32               Reserved;
    CBIOS_IN    CBIOS_OUT   PCBiosModeInfoExt   pModeList;
    CBIOS_IN    CBIOS_OUT   CBIOS_U32   ulBufferSize;
}CBIOS_DEVICEMODELIST_PARA, *PCBIOS_DEVICEMODELIST_PARA;

typedef struct _CBIOS_DEVICEMODELISTSIZE_PARA
{
    CBIOS_IN    CBIOS_U32               ulSize;
    CBIOS_IN    CBIOS_ACTIVE_TYPE       DevicePort;
    CBIOS_IN    CBIOS_U32               Reserved;
    CBIOS_OUT   CBIOS_U32               ulBufferSize;
}CBIOS_DEVICEMODELISTSIZE_PARA, *PCBIOS_DEVICEMODELISTSIZE_PARA;

typedef struct _CBIOS_WHO_SET_MODE_PARA
{
    CBIOS_IN            CBIOS_U32       Size;
    CBIOS_IN            CBIOS_BOOL      bSetState;
    CBIOS_IN CBIOS_OUT  CBIOS_U32       bModeSetByVBios;    //1: current mode is set by VBIOS
                                                            //0: current mode is set by CBIOS
}CBIOS_WHO_SET_MODE_PARA, *PCBIOS_WHO_SET_MODE_PARA;

typedef struct _CBIOS_PERIPHERAL_TYPE
{
    CBIOS_U32       GPUFan              :1;
    CBIOS_U32       ExtPower            :1;
    CBIOS_U32       Reserved            :30;                    //Reserved for new peripherals
}CBIOS_PERIPHERAL_TYPE, *PCBIOS_PERIPHERAL_TYPE;

typedef struct _CBIOS_PERIPHERAL_STATUS_PARA
{
    CBIOS_IN  CBIOS_U32                 Size;
    CBIOS_IN  CBIOS_PERIPHERAL_TYPE     Peripherals;            //the peripherals that driver wants to query
    CBIOS_IN  CBIOS_BOOL                bQuerySupportCaps;      //= true: query whether the given peripherals state can be checked.
                                                                //= false: check the given peripherals' state.
                                                                //Driver should check whether a peripheral's status can be checked first.
    CBIOS_OUT CBIOS_PERIPHERAL_TYPE     Result;                 //same bit definition with Peripherals. 
                                                                //If bQuerySupportCaps = true:
                                                                //  = 1: current peripheral's state can be checked
                                                                //  = 0: current peripheral's state can NOT be checked
                                                                //If bQuerySupportCaps = false:   
                                                                //  = 1: current peripheral is working normally
                                                                //  = 0: current peripheral is in an abnormal state
}CBIOS_PERIPHERAL_STATUS_PARA, *PCBIOS_PERIPHERAL_STATUS_PARA;  

typedef struct _HDMI_3D_STUCTURE_ALL
{
    CBIOS_U16   FramePacking       :1;
    CBIOS_U16   FieldAlternative   :1;
    CBIOS_U16   LineAlternative    :1;
    CBIOS_U16   SideBySideFull     :1;
    CBIOS_U16   LDepth             :1;
    CBIOS_U16   LDepthGraphics     :1;
    CBIOS_U16   TopAndBottom       :1;
    CBIOS_U16   RsvdBits0          :1;
    CBIOS_U16   SideBySideHalf     :1;
    CBIOS_U16   RsvdBits1          :6;
    CBIOS_U16   NotInUse           :1;
}HDMI_3D_STUCTURE_ALL, *PHDMI_3D_STUCTURE_ALL;

typedef struct _CBIOS_3D_VIDEO_MODE_LIST
{
    CBIOS_U32   Size;
    CBIOS_U32   XRes;
    CBIOS_U32   YRes;
    CBIOS_U32   RefreshRate;                //refresh rate * 100
    CBIOS_BOOL  bIsInterlace;
    union
    {
        HDMI_3D_STUCTURE_ALL   SupportStructures;
        CBIOS_U16   SupportCaps;
    };
}CBIOS_3D_VIDEO_MODE_LIST, *PCBIOS_3D_VIDEO_MODE_LIST;

typedef enum _CBIOS_STEREO_VIEW
{
    FIELD_SEQ_RIGHT = 0x1,  // Field sequential stereo, right image when stereo sync signal = 1
    FIELD_SEQ_LEFT,         // Field sequential stereo, left image when stereo sync signal = 1
    TWO_WAY_RIGHT,          // 2-way interleaved stereo, right image on even lines
    TWO_WAY_LEFT,           // 2-way interleaved stereo, left image on even lines
    FOUR_WAY,               // 4-way interleaved stereo
    SIDE_BY_SIDE_INTERLEAVE // Side-by-Side interleaved stereo
}CBIOS_STEREO_VIEW;

typedef struct _CBIOS_MONITOR_3D_CAPABILITY_PARA
{
    CBIOS_IN    CBIOS_U32           Size;
    CBIOS_IN    CBIOS_ACTIVE_TYPE   DeviceType;
    CBIOS_OUT   CBIOS_BOOL          bIsSupport3DVideo;				// for both HDMI 1.4 3D and row-interlace 3D monitor
    CBIOS_OUT   CBIOS_U32           Monitor3DModeNum;				// HDMI 1.4 3D
    CBIOS_OUT   PCBIOS_3D_VIDEO_MODE_LIST   pMonitor3DModeList;     //If pMonitor3DModeList = NULL, means query monitor capability and buffer size
                                                                    //If pMonitor3DModeList != NULL, cbios will copy mode list to this buffer
    CBIOS_OUT	CBIOS_BOOL			bStereoViewSupport;		        // stereo Viewing Support for row-interlace
    CBIOS_OUT   CBIOS_STEREO_VIEW   StereoViewType;                 // stereo view type
}CBIOS_MONITOR_3D_CAPABILITY_PARA, *PCBIOS_MONITOR_3D_CAPABILITY_PARA;

typedef struct _CBIOS_INFOFRAME_PARA
{
	CBIOS_IN	CBIOS_U32	Size;
	CBIOS_IN    CBIOS_ACTIVE_TYPE	Device;
	CBIOS_IN	CBIOS_U32	XRes;
	CBIOS_IN	CBIOS_U32	YRes;
	CBIOS_IN	CBIOS_U32	RRate;
	CBIOS_IN	CBIOS_U32	InterlaceFlag;
	CBIOS_IN	CBIOS_U8	VICCode;
	CBIOS_IN	CBIOS_U8	ColorFormat;
}CBIOS_INFOFRAME_PARA, *PCBIOS_INFOFRAME_PARA;

typedef struct _CBIOS_HDAC_PARA
{
	CBIOS_IN	CBIOS_U32	Size;
	CBIOS_IN	CBIOS_ACTIVE_TYPE	Device;
	CBIOS_IN    CBIOS_BOOL  bPresent;
	CBIOS_IN    CBIOS_BOOL  bEldValid;
    CBIOS_IN    CBIOS_U64   PortId;
    CBIOS_IN    CBIOS_U16   ManufacturerName;
    CBIOS_IN    CBIOS_U16   ProductCode;
}CBIOS_HDAC_PARA, *PCBIOS_HDAC_PARA;

//for CEC
typedef enum _CBIOS_CEC_INDEX
{
    CBIOS_CEC_INDEX1 = 0,
    CBIOS_CEC_INDEX2,
    CBIOS_CEC_INDEX_COUNT
}CBIOS_CEC_INDEX;
typedef struct _CBIOS_CEC_ENABLE_DISABLE_PARA
{
    CBIOS_IN    CBIOS_U32           Size;            // Size of CBIOS_CEC_ENABLE_DISABLE_PARA
    CBIOS_IN    CBIOS_BOOL          bEnableCEC;      // = CBIOS_TRUE: enable CEC; = CBIOS_FALSE: Disable CEC
    CBIOS_IN    CBIOS_CEC_INDEX     CECIndex;        // = 0: CEC1; = 1: CEC2; Other values are reserved
}CBIOS_CEC_ENABLE_DISABLE_PARA, *PCBIOS_CEC_ENABLE_DISABLE_PARA;

typedef struct _CBIOS_CEC_TRANSIMIT_MESSAGE_PARA
{
    CBIOS_IN CBIOS_U32          Size;           // Size of CBIOS_CEC_TRANSIMIT_MESSAGE_PARA
    CBIOS_IN CBIOS_CEC_INDEX    CECIndex;       // CEC index
    CBIOS_IN CBIOS_U8           DestAddr;       // CEC Initiator command destination address
    CBIOS_IN CBIOS_U32          CmdLen;         // CEC Initiator command length. The valid value is [0:16]
    CBIOS_IN CBIOS_U8           Command[16];    // Initiator Command sent by CEC
    CBIOS_IN CBIOS_BOOL         bBroadcast;     // = TRUE: broadcast; = FALSE: direct access  
    CBIOS_IN CBIOS_U8           RetryCnt;       // CEC Initiator Retry times. Valid values must be 1 to 5.
}CBIOS_CEC_TRANSIMIT_MESSAGE_PARA, *PCBIOS_CEC_TRANSIMIT_MESSAGE_PARA;

typedef struct _CBIOS_CEC_RECEIVE_MESSAGE_PARA
{
    CBIOS_IN  CBIOS_U32         Size;           // Size of CBIOS_CEC_RECEIVE_MESSAGE_PARA
    CBIOS_IN  CBIOS_CEC_INDEX   CECIndex;       // CEC index
    CBIOS_OUT CBIOS_U8          SourceAddr;     // CEC Follower received source address
    CBIOS_OUT CBIOS_U32         CmdLen;         // CEC Follower command length. The valid value is [0:16]
    CBIOS_OUT CBIOS_U8          Command[16];    // Follower received command
    CBIOS_OUT CBIOS_BOOL        bBroadcast;     // = TRUE: broadcast; = FALSE: direct access  
}CBIOS_CEC_RECEIVE_MESSAGE_PARA, *PCBIOS_CEC_RECEIVE_MESSAGE_PARA;

typedef enum _CBIOS_CEC_INTERRUPT_TYPE
{
    INVALID_CEC_INTERRUPT = 0,
    NORMAL_CEC_INTERRUPT
}CBIOS_CEC_INTERRUPT_TYPE;
typedef struct _CBIOS_CEC_INTERRUPT_INFO 
{
    CBIOS_IN  CBIOS_U32         Size;                   // Size of CBIOS_CEC_INTERRUPT_INFO
    CBIOS_IN  CBIOS_U32         InterruptBitMask;       // Value of mm8504
    CBIOS_OUT CBIOS_CEC_INTERRUPT_TYPE InterruptType;   // Interrupt type
    CBIOS_OUT CBIOS_U32         CEC1MsgReceived    :1;  // = TRUE: CEC1 module received a message  
    CBIOS_OUT CBIOS_U32         CEC2MsgReceived    :1;  // = TRUE: CEC2 module received a message
    CBIOS_OUT CBIOS_U32         RsvdBits           :30;
}CBIOS_CEC_INTERRUPT_INFO, *PCBIOS_CEC_INTERRUPT_INFO;

typedef struct _CBIOS_INIT_CHIP_PARA
{
    CBIOS_IN  CBIOS_U32   Size;
}CBIOS_INIT_CHIP_PARA, *PCBIOS_INIT_CHIP_PARA;

typedef struct _CBIOS_QUERY_MONITOR_TYPE_PER_PORT
{
    CBIOS_IN    CBIOS_U32           Size;
    CBIOS_IN    CBIOS_ACTIVE_TYPE   ActiveType;
    CBIOS_OUT   CBIOS_MONITOR_TYPE  MonitorType;
}CBIOS_QUERY_MONITOR_TYPE_PER_PORT, *PCBIOS_QUERY_MONITOR_TYPE_PER_PORT;

typedef enum _CBIOS_COLOR_FUNCTION_TYPE
{
    CBIOS_COLOR_FUNCTION_GET = 0,
    CBIOS_COLOR_FUNCTION_SET,
    CBIOS_COLOR_FUNCTION_QUERY_MAX,
    CBIOS_COLOR_FUNCTION_SET_DEFAULT
}CBIOS_COLOR_FUNCTION_TYPE;

typedef enum _CBIOS_COLOR_ATTRIBUTE_TYPE
{
    CBIOS_BRIGHTNESS = 1,
    CBIOS_CONTRAST,
    CBIOS_HUE,
    CBIOS_SATURATION,
}CBIOS_COLOR_ATTRIBUTE_TYPE;

typedef struct _CBIOS_COLOR_CONTROL_PARA
{
    CBIOS_IN        CBIOS_U32                   Size;
    CBIOS_IN        CBIOS_ACTIVE_TYPE           DeviceType;
    CBIOS_IN        CBIOS_COLOR_ATTRIBUTE_TYPE  AttributeType;
    CBIOS_IN        CBIOS_COLOR_FUNCTION_TYPE   FuncType;
    CBIOS_IN CBIOS_OUT  CBIOS_U32               AttributeValue;
}CBIOS_COLOR_CONTROL_PARA, *PCBIOS_COLOR_CONTROL_PARA;

typedef struct _CBIOS_BOOT_DEVICE_PARA
{
    CBIOS_IN    CBIOS_U32    Size;
    CBIOS_IN    CBIOS_U32    ConnectDevice;
    CBIOS_OUT CBIOS_U32    IgaActive[2];
}CBIOS_BOOT_DEVICE_PARA, *PCBIOS_BOOT_DEVICE_PARA;

typedef struct _CBIOS_VCP_DATA_ADDR_PARA
{
    CBIOS_IN    CBIOS_U32    Size;
    CBIOS_IN    CBIOS_U32    VCPBaseAddr;
}CBIOS_VCP_DATA_ADDR_PARA, *PCBIOS_VCP_DATA_ADDR_PARA;

/* CBIOS interfaces begins */
#ifdef DOSDLL
#define DLLEXPORTS __declspec(dllexport)
#define DLLIMPORT __declspec(dllimport)
#else
#define DLLEXPORTS
#define DLLIMPORT
#endif

/*This function pointer definition is for driver reference how to define the interface,*/
/*when get the proc address from interface CBiosGetProcAddress.*/
typedef CBIOS_S32 (*CBIOSPROC)(CBIOS_VOID);

typedef CBIOS_STATUS 
(*PFN_CBiosHpdEnableDisable)(CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_OUT PCBIOS_HOTPLUG_PARAM pHotPlugParams);

typedef CBIOS_STATUS
(*PFN_CBiosVoltageFunction)(CBIOS_VOID* pcbe, PCBiosVoltageSettingParams pVoltageSettingParams);

typedef CBIOS_STATUS
(*PFN_CBiosQueryConnectedMonitorAttribute)(CBIOS_VOID* pcbe, PCBiosQueryConnectedMonitorAttributeParams pQueryConnectedMonitorAttributeParams);

typedef CBIOS_STATUS
(*PFN_CBiosContentProtectionOnOff)(CBIOS_VOID* pcbe, PCBiosContentProtectionOnOffParams pContentProtectionOnOffParams);

typedef CBIOS_STATUS
(*PFN_CBiosCheckAndRestoreStrapReg)(CBIOS_VOID* pcbe, PCBIOS_BOOT_STRAP_REGS pCBBootStrapReg);

typedef CBIOS_STATUS
(*PFN_CBiosAccessDpcdData)(CBIOS_VOID* pcbe, PCBiosAccessDpcdDataParams pAccessDpcdDataParams);

typedef CBIOS_STATUS 
(*PFN_CBiosResetHWBlock)(CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_IN PCBIOS_RESET_HW_BLOCK_PARAM pHwBlockParams);

typedef CBIOS_STATUS 
(*PFN_CBiosGetTunnelToVBIOS)(CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_IN PCBIOS_TUNNEL_2D_TO_VBIOS_PARAM pTunnelParams);

/*
* 1. If faked edid is not a validate EDID, it will return CBIOS_ER_INVALID_PARAMETER. 
*    At present, driver must set validate EDID or do detect device;
* 2. When HPD or device (except for devices without EDID) changed, 
*    CBIOS will automatically unlock the faked EDID and use the real device EDID;
* 3. If driver set faked EDID for device without edid, CBIOS will think of it as device with EDID until driver unlock it;
*/
typedef CBIOS_STATUS 
(*PFN_CBiosSetEdid)(CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_IN PCBIOS_PARAM_SET_EDID pSetEdidParams);

typedef CBIOS_STATUS
(*PFN_CBiosGetAlwaysOnDevice)(CBIOS_IN CBIOS_VOID* pvcbe,
                              CBIOS_IN CBIOS_OUT PCBIOS_GET_ALWAYS_ON_DEVICES_PARAM pGetAlwaysOnDevicesParam);
typedef CBIOS_STATUS
(*PFN_CBiosCheckDeviceCombination)(CBIOS_IN CBIOS_VOID* pvcbe,
                                   CBIOS_IN CBIOS_OUT PCBIOS_CHECK_DEV_COMBIN_PARAM pDevInfo);
typedef CBIOS_STATUS
(*PFN_CBiosGetHotKeySwitchDeviceCombinations)(CBIOS_IN CBIOS_VOID* pvcbe, 
                                              CBIOS_OUT CBIOS_IN PCBIOS_GET_HOT_KEY_SWITCH_DEV_PARAM pHotKeySwitchDevParam);
typedef CBIOS_STATUS
(*PFN_CBiosCheckHwStatus)(CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_IN PCBIOS_HW_CHECK pHwCheck);

typedef CBIOS_STATUS
(*PFN_CBiosGetModeTiming)(CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_IN CBIOS_OUT PCBIOS_GET_MODE_TIMING_PARAM pGetModeTiming);

typedef CBIOS_STATUS
(*PFN_CBiosGetDeviceELD)(CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_IN CBIOS_OUT PCBIOS_GET_ELD_PARAM pGetELDData);

typedef CBIOS_STATUS
(*PFN_CBiosGetVBiosInfo)(CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_IN CBIOS_OUT PCBIOS_VBINFO_PARAM  pVbiosInfo);

typedef CBIOS_STATUS
(*PFN_CBiosVsyncLock)(CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_IN CBIOS_ACTIVE_TYPE Device);

typedef CBIOS_STATUS
(*PFN_CBiosUpdateShadowInfo)(CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_IN PCBIOS_PARAM_SHADOWINFO pShadowInfo);
 
typedef CBIOS_STATUS
(*PFN_CBiosSetMmioEndianMode)(CBIOS_IN CBIOS_VOID* pAdapterContext);

typedef CBIOS_STATUS
(*PFN_CBiosPWMControl)(CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_IN CBIOS_OUT PCBIOS_PWM_PARAMS pPWM_Params);

typedef CBIOS_STATUS
(*PFN_CBiosGetTemperature)(CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_IN CBIOS_OUT PCBIOS_TEMP_PARAMS pTemp_Params);

typedef CBIOS_STATUS
(*PFN_CBiosGetDeviceModeListSizePerChannel)(CBIOS_IN PCBIOS_VOID pvcbe, CBIOS_IN CBIOS_OUT PCBIOS_DEVICEMODELISTSIZE_PARA pDevModeListSizePara);

typedef CBIOS_STATUS
(*PFN_CBiosGetDeviceModeListPerChannel)(CBIOS_IN PCBIOS_VOID pvcbe, CBIOS_IN CBIOS_OUT PCBIOS_DEVICEMODELIST_PARA pDevModeListPara);

typedef CBIOS_STATUS
(*PFN_CBiosPWMFunctionCtrl)(CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_IN CBIOS_OUT PCBIOS_PWM_FUNCTION_CTRL_PARAMS pPWMFuncPara);

typedef CBIOS_STATUS
(*PFN_CBiosGetHpdInfo)(CBIOS_IN PCBIOS_VOID pvcbe, CBIOS_OUT PCBIOS_HPD_INFO pHPDInfo);

typedef CBIOS_STATUS
(*PFN_CBiosSyncWhoSetMode)(CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_IN CBIOS_OUT PCBIOS_WHO_SET_MODE_PARA pWhoSetMode);

typedef CBIOS_STATUS
(*PFN_CBiosCheckPeripheralsStatus)(CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_IN CBIOS_OUT PCBIOS_PERIPHERAL_STATUS_PARA pPeripheralStatus);

typedef CBIOS_STATUS
(*PFN_CBiosQueryMonitor3DCapability) (CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_IN CBIOS_OUT PCBIOS_MONITOR_3D_CAPABILITY_PARA p3DCapability);

typedef CBIOS_STATUS
(*PFN_CBiosSetInfoFrame) (CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_IN PCBIOS_INFOFRAME_PARA pCbiosHDMIInfoFramePara);

typedef CBIOS_STATUS
(*PFN_CBiosSetHDACodecPara) (CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_IN PCBIOS_HDAC_PARA pCbiosHDACPara);

typedef CBIOS_STATUS
(*PFN_CBiosSetHDACConnectStatus) (CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_IN PCBIOS_HDAC_PARA pCbiosHDACPara);

typedef CBIOS_STATUS
(*PFN_CBiosCECEnableDisable) (CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_IN PCBIOS_CEC_ENABLE_DISABLE_PARA pCECEnableDisablePara);

typedef CBIOS_STATUS 
(*PFN_CBiosCECTransmitMessage) (CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_IN PCBIOS_CEC_TRANSIMIT_MESSAGE_PARA pCECPara);

typedef CBIOS_STATUS 
(*PFN_CBiosCECReceiveMessage) (CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_IN PCBIOS_CEC_RECEIVE_MESSAGE_PARA pCECPara);

typedef CBIOS_STATUS 
(*PFN_CBiosGetCECInterruptInfo) (CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_IN PCBIOS_CEC_INTERRUPT_INFO pCECIntInfo);

typedef CBIOS_STATUS 
(*PFN_CBiosInitChip) (CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_IN PCBIOS_INIT_CHIP_PARA pInitChipPara);

typedef CBIOS_STATUS 
(*PFN_CBiosGetSupportedMonitorTypePerPort) (CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_IN CBIOS_OUT PCBIOS_QUERY_MONITOR_TYPE_PER_PORT pCBiosQueryMonitorTypePerPort);

/*CBios external interface definiton.*/
#ifdef __cplusplus
extern "C" {
#endif

DLLEXPORTS CBIOS_STATUS
CBiosGetExtensionSize(PCBIOS_CHIP_ID pCBChipId, CBIOS_U32 *pulExtensionSize);

DLLEXPORTS CBIOS_STATUS 
CBiosGetProcAddress(CBIOS_IN char *strFunName, 
                    CBIOS_IN CBIOS_OUT CBIOSPROC *pulFuncPointer);

DLLEXPORTS CBIOS_STATUS
CBiosInit(CBIOS_VOID* pcbe, PCBIOS_PARAM_INIT pCBParamInit);

//DLLEXPORTS CBIOS_STATUS
//CBiosGetVersion(CBIOS_VOID* pcbe, PCBIOS_VERSION pCbiosVersion);

DLLEXPORTS CBIOS_STATUS
CBiosSetCallBackFunctions(PCBIOS_CALLBACK_FUNCTIONS pFnCallBack);

DLLEXPORTS CBIOS_STATUS
CBiosInt10(CBIOS_VOID* pcbe, CBIOS_VOID* biosArguments);

DLLEXPORTS CBIOS_STATUS
CBiosPost(CBIOS_VOID* pcbe, PCBIOS_PARAM_POST pCBParamPost,PCBIOS_BOOT_STRAP_REGS pCBBootStrapReg);

//DLLEXPORTS CBIOS_STATUS
//CBiosSyncDataWithVbios(CBIOS_VOID* pcbe, PCBIOS_VBIOS_DATA_PARAM pDataParam);

//DLLEXPORTS CBIOS_STATUS
//CBiosDumpReg(CBIOS_VOID* pcbe, PCBIOS_PARAM_DUMP_REG pCBParamDumpReg);

DLLEXPORTS CBIOS_STATUS
CBiosGetEdid(CBIOS_VOID* pcbe, PCBIOS_PARAM_GET_EDID pCBParamGetEdid);

DLLEXPORTS CBIOS_STATUS
CBiosI2CDataRead(CBIOS_VOID* pcbe, PCBIOS_PARAM_I2C_DATA pCBParamI2CData);

DLLEXPORTS CBIOS_STATUS
CBiosI2CDataWrite(CBIOS_VOID* pcbe, PCBIOS_PARAM_I2C_DATA pCBParamI2CData);

//DLLEXPORTS CBIOS_STATUS
//CBiosGetHDMIType(CBIOS_VOID* pcbe, CBIOS_U32 *HDMIType, CBIOS_U32 Devices);

//DLLEXPORTS CBIOS_STATUS
//CBiosGetTMDSType(CBIOS_VOID* pcbe, CBIOS_U32 *TMDSType, CBIOS_U32 Devices);

/* To be obseleted. */
//DLLEXPORTS CBIOS_STATUS
//CBiosGetOptimalFormat(CBIOS_VOID* pcbe, 
//                      CBIOS_IN CBIOS_U32 HorSize, 
//                      CBIOS_IN CBIOS_U32 VerSize,
//                      CBIOS_IN CBIOS_U32 Devices,
//                      CBIOS_OUT CBIOS_U32 *pFormatNum);

//DLLEXPORTS CBIOS_S32 
//CBiosIsDisplayTimingSupportedByDevice(CBIOS_IN CBIOS_VOID* pcbe,
//    CBIOS_IN PCBIOS_DISPLAY_TIMING_PARAM pCBDisplayTimingParam);

//DLLEXPORTS CBIOS_STATUS
//CBiosGetMIUTimeout(CBIOS_VOID* pcbe, PCBIOS_PARAM_GET_TIMEOUT pCBParamGetTimeout);

//DLLEXPORTS CBIOS_STATUS
//CBiosSetClock(CBIOS_VOID* pcbe, PCBiosDefaultClockParams pCBiosDefaultClockParams, CBIOS_UCHAR IGAIndex);

//DLLEXPORTS CBIOS_STATUS
//CBiosGetClock(CBIOS_VOID* pcbe, PCBiosDefaultClockParams pCBiosDefaultClockParams, CBIOS_UCHAR IGAIndex);

//DLLEXPORTS CBIOS_STATUS
//CBiosGetDefaultClockFromVCP(CBIOS_VOID* pcbe, PCBiosDefaultClockParams pCBiosDefaultClockParams);

DLLEXPORTS CBIOS_STATUS
CBiosUnload(CBIOS_VOID* pcbe);

//DLLEXPORTS CBIOS_STATUS 
//CBiosNonDestructiveDeviceDetect(CBIOS_VOID* pcbe,CBIOS_ACTIVE_TYPE DetectDevices, CBIOS_S32* DeviceStatus);

//DLLEXPORTS CBIOS_STATUS
//CBiosDestructiveDeviceDetect(CBIOS_VOID* pcbe, CBIOS_ACTIVE_TYPE DetectDevices, CBIOS_S32* DeviceStatus);

//DLLEXPORTS CBIOS_STATUS
//CBiosCRTHVTiming(CBIOS_VOID* pcbe,PCBIOS_MODE_KEY_PARAM pCBModeParam, CBIOS_UCHAR IGAIndex);

//DLLEXPORTS CBIOS_STATUS
//CBiosGetCurrentFormatAttrib(CBIOS_VOID* pcbe, CBIOS_U32 Devices, PCBIOS_TIMING_ATTRIB pCBHDMIFormatAttrib);

//DLLEXPORTS CBIOS_STATUS
//CBiosQueryHDMIFormatAttrib(CBIOS_VOID* pcbe, CBIOS_U16 FormatNum, CBIOS_U32 Devices, PCBIOS_HDMI_FORMAT_MTX pCBHDMIFormatAttrib);

//DLLEXPORTS CBIOS_STATUS 
//CBiosReadReg(CBIOS_VOID* pcbe, CBIOS_UCHAR type, CBIOS_UCHAR index,CBIOS_UCHAR *result);

//DLLEXPORTS CBIOS_STATUS
//CBiosWriteReg(CBIOS_VOID* pcbe, CBIOS_UCHAR type, CBIOS_UCHAR index, CBIOS_UCHAR value, CBIOS_UCHAR mask);

//DLLEXPORTS CBIOS_STATUS
//CBiosSetHDTVPosition(CBIOS_VOID* pcbe, PCBIOS_DEVICE_POSITION pHDTVPosition);

//DLLEXPORTS CBIOS_STATUS
//CBiosGetHDTVPosition(CBIOS_VOID* pcbe, PCBIOS_DEVICE_POSITION pHDTVPosition);

//DLLEXPORTS CBIOS_STATUS
//CBiosSetTVPosition(CBIOS_VOID* pcbe, PCBIOS_DEVICE_POSITION pTVPosition);

//DLLEXPORTS CBIOS_STATUS
//CBiosGetTVPosition(CBIOS_VOID* pcbe, PCBIOS_DEVICE_POSITION pTVPosition);

//DLLEXPORTS CBIOS_S32 CBiosIsEDIDSupported(CBIOS_IN CBIOS_VOID*  pcbe,
//                          CBIOS_IN CBIOS_U32  DisplayDevice);

//DLLEXPORTS CBIOS_STATUS
//CBiosDDCCII2COpen(CBIOS_VOID* pcbe, CBIOS_S32 bOpen, PCBIOS_I2CCONTROL pI2CControl, CBIOS_UCHAR I2CBUSNum);

//DLLEXPORTS CBIOS_STATUS
//CBiosDDCCII2CAccess(CBIOS_VOID* pcbe, PCBIOS_I2CCONTROL pI2CControl, CBIOS_UCHAR I2CBUSNum);

//DLLEXPORTS CBIOS_STATUS
//CBiosSetIgaScreenOnOffState(CBIOS_VOID* pcbe, CBIOS_S32 status, CBIOS_UCHAR IGAIndex);

//DLLEXPORTS CBIOS_STATUS
//CBiosSetDisplayDevicePowerState(CBIOS_VOID* pcbe, CBIOS_ACTIVE_TYPE Devices, CBIOS_PM_STATUS PMState);

//DLLEXPORTS CBIOS_STATUS
//CBiosGetDisplayDevicePowerState(CBIOS_VOID* pcbe, CBIOS_ACTIVE_TYPE Devices, PCBIOS_PM_STATUS pPMState);

/* The following function is for new setting mode logic */
DLLEXPORTS CBIOS_STATUS
CBiosGetDeviceModeListBufferSize(CBIOS_IN CBIOS_VOID* pvcbe,
                                 CBIOS_IN CBIOS_ACTIVE_TYPE  Device,
                                 CBIOS_OUT CBIOS_U32 * pBufferSize);
DLLEXPORTS CBIOS_STATUS
CBiosGetDeviceModeList(CBIOS_IN CBIOS_VOID* pcbe,
                       CBIOS_IN CBIOS_ACTIVE_TYPE  Device,
                       CBIOS_OUT PCBiosModeInfoExt pModeList,
                       CBIOS_IN CBIOS_OUT CBIOS_U32 * pBufferSize); 
//DLLEXPORTS CBIOS_STATUS
//CBiosGetAdapterModeListBufferSize(CBIOS_IN CBIOS_VOID* pcbe,
//                                  CBIOS_OUT CBIOS_U32 * pBufferSize);                       
                       
//DLLEXPORTS CBIOS_STATUS
//CBiosGetAdapterModeList(CBIOS_IN CBIOS_VOID* pcbe,
//                        CBIOS_OUT PCBiosModeInfoExt pModeList,
//                        CBIOS_IN CBIOS_OUT CBIOS_U32 * pBufferSize);
                        
DLLEXPORTS CBIOS_STATUS
CBiosSetModeToIGA(CBIOS_IN CBIOS_VOID* pcbe,
                  CBIOS_IN PCBiosSettingModeParams pSettingModeParams);

//DLLEXPORTS CBIOS_STATUS 
//CBiosD2D3LinebufferSharing(CBIOS_IN CBIOS_VOID* pvcbe, 
//                           CBIOS_IN CBIOS_UCHAR igaindex, CBIOS_IN CBIOS_S32 LBstatus);

//DLLEXPORTS CBIOS_STATUS 
//CBiosHpdQueryDeviceStatus(CBIOS_IN CBIOS_VOID* pvcbe, 
//                          CBIOS_OUT PCBIOS_HPD_QUERY_DEVICE_STATUS pHpdQueryDeviceStatus);
//DLLEXPORTS CBIOS_STATUS 
//CBiosGetHpdInfo(CBIOS_IN CBIOS_VOID* pvcbe, PCBIOS_HPD_INFO pHPDInfo);

//DLLEXPORTS CBIOS_STATUS 
//CBiosHpdEnableDisable(CBIOS_IN CBIOS_VOID* pvcbe, 
//                      CBIOS_IN PCBIOS_HOTPLUG_PARAM pHPDEnableDisable);

//DLLEXPORTS CBIOS_STATUS
//CBiosVoltageFunction(CBIOS_IN CBIOS_VOID* pvcbe, 
//                     CBIOS_IN PCBiosVoltageSettingParams pVoltageSettingParams);

//DLLEXPORTS CBIOS_STATUS
//CBiosQueryConnectedMonitorAttribute(CBIOS_IN CBIOS_VOID* pvcbe, 
//                                    CBIOS_OUT PCBiosQueryConnectedMonitorAttributeParams pQueryConnectedMonitorAttributeParams);

//DLLEXPORTS CBIOS_STATUS
//CBiosContentProtectionOnOff(CBIOS_IN CBIOS_VOID* pvcbe, 
//                            CBIOS_IN PCBiosContentProtectionOnOffParams pContentProtectionOnOffParams);

//DLLEXPORTS CBIOS_STATUS
//CBiosCheckAndRestoreStrapReg(CBIOS_VOID* pvcbe, 
//                             PCBIOS_BOOT_STRAP_REGS pCBBootStrapReg);

//DLLEXPORTS CBIOS_STATUS
//CBiosAccessDpcdData(CBIOS_VOID* pcbe, 
//                    PCBiosAccessDpcdDataParams pAccessDpcdDataParams);

//DLLEXPORTS CBIOS_STATUS  
//CBiosResetHWBlock(CBIOS_IN CBIOS_VOID* pvcbe, 
//                  CBIOS_IN PCBIOS_RESET_HW_BLOCK_PARAM pBlock);

//DLLEXPORTS CBIOS_STATUS  
//CBiosGetTunnelToVBIOS(CBIOS_IN CBIOS_VOID* pvcbe, 
//                      CBIOS_IN CBIOS_OUT PCBIOS_TUNNEL_2D_TO_VBIOS_PARAM pTunnelParams);

//DLLEXPORTS CBIOS_STATUS
//CBiosSetEdid(CBIOS_IN CBIOS_VOID* pvcbe, 
//             CBIOS_IN PCBIOS_PARAM_SET_EDID pCBParamSetEdid);

//DLLEXPORTS CBIOS_STATUS
//CBiosGetAlwaysOnDevice (CBIOS_IN CBIOS_VOID* pvcbe,
//                        CBIOS_IN CBIOS_OUT PCBIOS_GET_ALWAYS_ON_DEVICES_PARAM pGetAlwaysOnDevicesParam);

//DLLEXPORTS CBIOS_STATUS
//CBiosCheckDeviceCombination(CBIOS_IN CBIOS_VOID* pvcbe,
//                            CBIOS_IN CBIOS_OUT PCBIOS_CHECK_DEV_COMBIN_PARAM pCheckDevCombinParam);
//CBIOS_STATUS
//CBiosGetHotKeySwitchDeviceCombinations(CBIOS_IN CBIOS_VOID* pvcbe, 
//                                       CBIOS_IN CBIOS_OUT PCBIOS_GET_HOT_KEY_SWITCH_DEV_PARAM pGetHotKeySwitchDevParam);

//DLLEXPORTS CBIOS_STATUS 
//CBIOSCheckHWStatus(CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_IN PCBIOS_HW_CHECK pHWCheck);

DLLEXPORTS CBIOS_STATUS
CBiosGetModeTiming(CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_IN CBIOS_OUT PCBIOS_GET_MODE_TIMING_PARAM pGetModeTiming);

//DLLEXPORTS CBIOS_STATUS
//CBiosGetDeviceELD(CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_IN CBIOS_OUT PCBIOS_GET_ELD_PARAM pGetELDData);

DLLEXPORTS CBIOS_STATUS
CBiosGetVBiosInfo(CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_IN CBIOS_OUT PCBIOS_VBINFO_PARAM  pVbiosInfo);

//DLLEXPORTS CBIOS_STATUS
//CBiosVsyncLock(CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_IN CBIOS_ACTIVE_TYPE Device);

//DLLEXPORTS CBIOS_STATUS
//CBiosUpdateShadowInfo(CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_IN PCBIOS_PARAM_SHADOWINFO pShadowInfo);

DLLEXPORTS CBIOS_STATUS
CBiosSetMmioEndianMode(CBIOS_IN CBIOS_VOID* pAdapterContext);

//DLLEXPORTS CBIOS_STATUS
//CBiosPWMControl(CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_IN CBIOS_OUT PCBIOS_PWM_PARAMS pPWM_Params);

//DLLEXPORTS CBIOS_STATUS
//CBiosGetTemperature(CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_IN CBIOS_OUT PCBIOS_TEMP_PARAMS pTemp_Params);

//DLLEXPORTS CBIOS_STATUS
//CBiosGetDeviceModeListPerChannel(CBIOS_IN PCBIOS_VOID pvcbe,
//                       CBIOS_IN CBIOS_OUT PCBIOS_DEVICEMODELIST_PARA pDevModeListPara);

//DLLEXPORTS CBIOS_STATUS
//CBiosGetDeviceModeListSizePerChannel(CBIOS_IN PCBIOS_VOID pvcbe,
//                                 CBIOS_IN CBIOS_OUT PCBIOS_DEVICEMODELISTSIZE_PARA pDevModeListSizePara);

//DLLEXPORTS CBIOS_STATUS
//CBiosPWMFunctionCtrl(CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_IN CBIOS_OUT PCBIOS_PWM_FUNCTION_CTRL_PARAMS pPWMFuncPara);

//DLLEXPORTS CBIOS_STATUS
//CBiosSyncWhoSetMode(CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_IN CBIOS_OUT PCBIOS_WHO_SET_MODE_PARA pWhoSetMode);

//DLLEXPORTS CBIOS_STATUS
//CBiosCheckPeripheralsStatus(CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_IN CBIOS_OUT PCBIOS_PERIPHERAL_STATUS_PARA pPeripheralStatus);

//DLLEXPORTS CBIOS_STATUS
//CBiosQueryMonitor3DCapability(CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_IN CBIOS_OUT PCBIOS_MONITOR_3D_CAPABILITY_PARA p3DCapability);

//DLLEXPORTS CBIOS_STATUS
//CBiosSetInfoFrame(CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_IN PCBIOS_INFOFRAME_PARA pCbiosHDMIInfoFramePara);

//DLLEXPORTS CBIOS_STATUS
//CBiosSetHDACodecPara(CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_IN PCBIOS_HDAC_PARA pCbiosHDACPara);

//DLLEXPORTS CBIOS_STATUS
//CBiosSetHDACConnectStatus(CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_IN PCBIOS_HDAC_PARA pCbiosHDACPara);

//DLLEXPORTS CBIOS_STATUS 
//CBiosCECEnableDisable(CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_IN PCBIOS_CEC_ENABLE_DISABLE_PARA pCECEnableDisablePara);

//DLLEXPORTS CBIOS_STATUS 
//CBiosCECTransmitMessage(CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_IN PCBIOS_CEC_TRANSIMIT_MESSAGE_PARA pCECPara);

//DLLEXPORTS CBIOS_STATUS 
//CBiosCECReceiveMessage(CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_IN PCBIOS_CEC_RECEIVE_MESSAGE_PARA pCECPara);

//DLLEXPORTS CBIOS_STATUS 
//CBiosGetCECInterruptInfo(CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_IN PCBIOS_CEC_INTERRUPT_INFO pCECIntInfo);

DLLEXPORTS CBIOS_STATUS 
CBiosInitChip(CBIOS_IN CBIOS_VOID* pvcbe, CBIOS_IN PCBIOS_INIT_CHIP_PARA pInitChipPara);

//DLLEXPORTS CBIOS_STATUS 
//CBiosGetSupportedMonitorTypePerPort(CBIOS_IN CBIOS_VOID* pvcbe,CBIOS_IN CBIOS_OUT PCBIOS_QUERY_MONITOR_TYPE_PER_PORT pCBiosQueryMonitorTypePerPort);

DLLEXPORTS CBIOS_STATUS
CBiosColorControl(CBIOS_IN PCBIOS_VOID pvcbe, CBIOS_IN CBIOS_OUT PCBIOS_COLOR_CONTROL_PARA pColorPara);

DLLEXPORTS CBIOS_STATUS
CBiosSetDownscaler(CBIOS_IN PCBIOS_VOID pvcbe, CBIOS_IN PCBiosSettingDownscalerParams pSettingDownscalerParams);

DLLEXPORTS CBIOS_U32
CBiosGetBootDevice(CBIOS_IN PCBIOS_VOID pvcbe, CBIOS_IN PCBIOS_BOOT_DEVICE_PARA pCBiosBootDeviceParams);

DLLEXPORTS CBIOS_U32
CBiosSaveVCPData(CBIOS_IN PCBIOS_VOID pvcbe, CBIOS_IN PCBIOS_VCP_DATA_ADDR_PARA pCBiosVCPDataAddrParams);

/*#ifdef KERNEL_BUILD

DLLEXPORTS PCBIOS_VOID 
CBiosVGAInit(CBIOS_IN CBIOS_VOID* mmio, CBIOS_IN CBIOS_VOID* vgaMem);

DLLEXPORTS CBIOS_VOID
CBiosVGADeInit(CBIOS_IN CBIOS_VOID* pVga);

DLLEXPORTS CBIOS_VOID
CBiosVGASave(CBIOS_IN CBIOS_VOID* pVga, CBIOS_IN CBIOS_U32 flag);

DLLEXPORTS CBIOS_VOID
CBiosVGARestore(CBIOS_IN CBIOS_VOID* pVga, CBIOS_IN CBIOS_U32 flag);

#endif */
/*When need add new interface, please add this from this position.*/
#ifdef __cplusplus
}
#endif
/* CBIOS interfaces ends */


#endif /*_CBIOS_H_ */

