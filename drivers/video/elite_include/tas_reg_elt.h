//    Spec Last Modified Time: 2011/5/13 8:46:50
#ifndef _TAS_REG_ELT_H
#define _TAS_REG_ELT_H


#ifndef        TAS_BLOCKBASE_INF
#define    TAS_BLOCKBASE_INF
#define    BLOCK_TAS_VERSION 1
    #define    BLOCK_TAS_TIMESTAMP "2011/5/13 8:46:50"
#define    TAS_BLOCK                                                  0x2 // match with BlockID.h
#define    TAS_REG_START                                              0x0 // match with BlockID.h
    #define    TAS_REG_END                                                0x8 // match with BlockID.h
    #define    TAS_REG_LIMIT                                              0x8 // match with BlockID.h
#endif

// Register offset definition
#define        Reg_Tas_Ctrl_Offset                                        0x0
#define        Reg_Scissor_X_Offset                                       0x1
#define        Reg_Scissor_Y_Offset                                       0x2
#define        Reg_Z_Scale_Factor_Offset                                  0x3
#define        Reg_Z_Bias_Offset                                          0x4
#define        Reg_Path_Bbox_X_Offset                                     0x5
#define        Reg_Path_Bbox_Y_Offset                                     0x6
#define        Reg_Const_Wrap_Mask_Offset                                 0x7

// Block constant definition
typedef enum
{
    TAS_CTRL_AS_EN_DISABLED                  = 0,   // 0: attribute setup disabled
    TAS_CTRL_AS_EN_ENABLED                   = 1,   // 1: attribute setup enabled
} TAS_CTRL_AS_EN;
typedef enum
{
    TAS_CTRL_OPENGL_DX                       = 0,   // 0: DX WinMobile
    TAS_CTRL_OPENGL_OGL                      = 1,   // 1: OpenGL ES 2.0
} TAS_CTRL_OPENGL;
typedef enum
{
    TAS_CTRL_ZERO_DET_MODE_FORCE_REJECT      = 0,   // 0: force reject triangle
    TAS_CTRL_ZERO_DET_MODE_BACK_FACE         = 1,   // 1: set backfacing
} TAS_CTRL_ZERO_DET_MODE;
typedef enum
{
    TAS_CTRL_CULL_EN_DISABLED                = 0,   // 0: disabled
    TAS_CTRL_CULL_EN_ENABLED                 = 1,   // 1: enabled
} TAS_CTRL_CULL_EN;
typedef enum
{
    TAS_CTRL_CULL_MODE_CULL_BACK             = 0,   // 0: cull backface
    TAS_CTRL_CULL_MODE_CULL_FRONT            = 1,   // 1: cull frontface
} TAS_CTRL_CULL_MODE;
typedef enum
{
    TAS_CTRL_WINDING_FRONT_CW                = 0,   // 0: front cw
    TAS_CTRL_WINDING_FRONT_CCW               = 1,   // 1: front ccw
} TAS_CTRL_WINDING;
typedef enum
{
    TAS_CTRL_ZCLIP_EN_DISABLED               = 0,   // 0: disabled
    TAS_CTRL_ZCLIP_EN_ENABLED                = 1,   // 1: enabled
} TAS_CTRL_ZCLIP_EN;
typedef enum
{
    TAS_CTRL_ZOFFSET_EN_DISABLED             = 0,   // 0: disabled
    TAS_CTRL_ZOFFSET_EN_ENABLED              = 1,   // 1: enabled
} TAS_CTRL_ZOFFSET_EN;
typedef enum
{
    TAS_CTRL_FLAT_SHADE_EN_DISABLED          = 0,   // 0: disabled
    TAS_CTRL_FLAT_SHADE_EN_ENABLED           = 1,   // 1: enabled
} TAS_CTRL_FLAT_SHADE_EN;
typedef enum
{
    TAS_CTRL_LINE_CAP_EN_DISABLED            = 0,   // 0: disabled, no line cap only 2 edges
    TAS_CTRL_LINE_CAP_EN_ENABLED             = 1,   // 1: enabled, include line cap 4 edges
} TAS_CTRL_LINE_CAP_EN;
typedef enum
{
    TAS_CTRL_POINT_SPRITE_EN_DISABLED        = 0,   // 0: disabled
    TAS_CTRL_POINT_SPRITE_EN_ENABLED         = 1,   // 1: enabled, use point size in VS output to setup point
    // sprite UV
} TAS_CTRL_POINT_SPRITE_EN;
typedef enum
{
    TAS_CTRL_Z_ONLY_DISABLED                 = 0,   // 0: disabled, 4-component
    TAS_CTRL_Z_ONLY_ENABLED                  = 1,   // 1: enabled, 1-component
} TAS_CTRL_Z_ONLY;
typedef enum
{
    TAS_CTRL_PATH_MODE_NORMAL                = 0,   // 0: normal rendering
    TAS_CTRL_PATH_MODE_BUILD_BBOX            = 1,   // 1: accumulate path bounding box
    TAS_CTRL_PATH_MODE_BUILD_ST              = 2,   // 0: build stencil area
    TAS_CTRL_PATH_MODE_BUILD_BOTH            = 3,   // 1: build path bounding box and stencil
} TAS_CTRL_PATH_MODE;
typedef enum
{
    TAS_CTRL_USE_PATH_BBOX_USE_SCISSOR       = 0,   // 0: use scissor window register
    TAS_CTRL_USE_PATH_BBOX_USE_PATH_BBOX     = 1,   // 1: use path bbox register
} TAS_CTRL_USE_PATH_BBOX;
typedef enum
{
    TAS_CTRL_MSAA_EN_DISABLED                = 0,   // 0: MSAA disabled
    TAS_CTRL_MSAA_EN_ENABLED                 = 1,   // 1: MSAA enabled
} TAS_CTRL_MSAA_EN;
typedef enum
{
    TAS_CTRL_POINT_SIZE_EN_DISABLED          = 0,   // 0: disabled, use point_size register for point setup.
    // Point sprite must be disabled.
    TAS_CTRL_POINT_SIZE_EN_ENABLED           = 1,   // 1: enabled, the point size is in VS output slot #0 channel w,
    // packed with rhw sign.
} TAS_CTRL_POINT_SIZE_EN;
typedef enum
{
    TAS_CTRL_PACKING_EN_DISABLED             = 0,   // 0: disabled, no attribute packing.
    TAS_CTRL_PACKING_EN_ENABLED              = 1,   // 1: enabled, pack attributes based on the component mask
    // passed by the EU.
} TAS_CTRL_PACKING_EN;
typedef enum
{
    TAS_CTRL_LAST_PIXEL_DISABLED             = 0,   // 0: draw last pixel disabled.
    TAS_CTRL_LAST_PIXEL_ENABLED              = 1,   // 1: draw last pixel enabled.
} TAS_CTRL_LAST_PIXEL;
typedef enum
{
    TAS_CTRL_D3D_LEADING_VTX_VTX0            = 0,   // 0: vertex 0, for D3D list and strip.
    TAS_CTRL_D3D_LEADING_VTX_VTX1            = 1,   // 1: vertex 1, for D3D fan.
} TAS_CTRL_D3D_LEADING_VTX;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////            TAS Block (Block ID = 2) Register Definitions                             ///////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef union
{
    struct
    {
        unsigned int As_En                     : 1;   // Attribute setup enable
        unsigned int Opengl                    : 1;   // API selection, used for API specific handling, strip
        // winding direction, etc
        unsigned int Zero_Det_Mode             : 1;   // Zero determinate mode
        unsigned int Cull_En                   : 1;   // Culling enable
        unsigned int Cull_Mode                 : 1;   // Culling mode
        unsigned int Winding                   : 1;   // Winding direction
        unsigned int Zclip_En                  : 1;   // Z clipping enable, used for z trivial rejection
        unsigned int Zoffset_En                : 1;   // Polygon offset enable
        unsigned int Flat_Shade_En             : 1;   // Flat shading enable
        unsigned int Line_Cap_En               : 1;   // Line cap enable
        unsigned int Point_Sprite_En           : 1;   // Point sprite enable
        unsigned int Z_Only                    : 1;   // Point sprite enable
        unsigned int Line_Width                : 4;   // Value 1~8 for 1~8 pixels wide
        unsigned int Point_Size                : 8;   // Value 1~128 for 1~128 pixels wide
        unsigned int Path_Mode                 : 2;   // OpenVG path rendering mode
        unsigned int Use_Path_Bbox             : 1;   // OpenVG use path bbox
        unsigned int Msaa_En                   : 1;   // MSAA enable
        unsigned int Point_Size_En             : 1;   // Point size enable
        unsigned int Packing_En                : 1;   // Attribute packing enable
        unsigned int Last_Pixel                : 1;   // Line draw last pixel on/off
        unsigned int D3d_Leading_Vtx           : 1;   // D3D leading vertex selection
    } reg;
    unsigned int uint;
} Reg_Tas_Ctrl;

typedef union
{
    struct
    {
        unsigned int X_Min                     : 16;  // X minimum
        unsigned int X_Max                     : 16;  // X maximum
    } reg;
    unsigned int uint;
} Reg_Scissor_X;

typedef union
{
    struct
    {
        unsigned int Y_Min                     : 16;  // Y minimum
        unsigned int Y_Max                     : 16;  // Y maximum
    } reg;
    unsigned int uint;
} Reg_Scissor_Y;

typedef union
{
    struct
    {
        unsigned int Factor                    : 32;  // s23e8,since TAS don't support fp28, change this register
        // to standard fp32
    } reg;
    unsigned int uint;
} Reg_Z_Scale_Factor;

typedef union
{
    struct
    {
        unsigned int Zbias                     : 32;  // s23e8,since TAS don't support fp28, change this register
        // to standard fp32
    } reg;
    unsigned int uint;
} Reg_Z_Bias;

typedef union
{
    struct
    {
        unsigned int X_Min                     : 16;  // X minimum
        unsigned int X_Max                     : 16;  // X maximum
    } reg;
    unsigned int uint;
} Reg_Path_Bbox_X;

typedef union
{
    struct
    {
        unsigned int Y_Min                     : 16;  // Y minimum
        unsigned int Y_Max                     : 16;  // Y maximum
    } reg;
    unsigned int uint;
} Reg_Path_Bbox_Y;

typedef union
{
    struct
    {
        unsigned int Attr0_Const               : 1;   // Attribute 0 const flag, 0=varying, 1=const
        unsigned int Attr1_Const               : 1;   // Attribute 1 const flag, 0=varying, 1=const
        unsigned int Attr2_Const               : 1;   // Attribute 2 const flag, 0=varying, 1=const
        unsigned int Attr3_Const               : 1;   // Attribute 3 const flag, 0=varying, 1=const
        unsigned int Attr4_Const               : 1;   // Attribute 4 const flag, 0=varying, 1=const
        unsigned int Attr5_Const               : 1;   // Attribute 5 const flag, 0=varying, 1=const
        unsigned int Attr6_Const               : 1;   // Attribute 6 const flag, 0=varying, 1=const
        unsigned int Attr7_Const               : 1;   // Attribute 7 const flag, 0=varying, 1=const
        unsigned int Attr8_Const               : 1;   // Attribute 8 const flag, 0=varying, 1=const
        unsigned int Attr9_Const               : 1;   // Attribute 9 const flag, 0=varying, 1=const
        unsigned int Attr10_Const              : 1;   // Attribute 10 const flag, 0=varying, 1=const
        unsigned int Attr11_Const              : 1;   // Attribute 11 const flag, 0=varying, 1=const
        unsigned int Attr12_Const              : 1;   // Attribute 12 const flag, 0=varying, 1=const
        unsigned int Attr13_Const              : 1;   // Attribute 13 const flag, 0=varying, 1=const
        unsigned int Attr14_Const              : 1;   // Attribute 14 const flag, 0=varying, 1=const
        unsigned int Attr15_Const              : 1;   // Attribute 15 const flag, 0=varying, 1=const
        unsigned int Attr0_Wrap                : 1;   // Attribute 0 D3Dwrap flag, 0=normal, 1=wrap enabled
        unsigned int Attr1_Wrap                : 1;   // Attribute 1 D3Dwrap flag, 0=normal, 1=wrap enabled
        unsigned int Attr2_Wrap                : 1;   // Attribute 2 D3Dwrap flag, 0=normal, 1=wrap enabled
        unsigned int Attr3_Wrap                : 1;   // Attribute 3 D3Dwrap flag, 0=normal, 1=wrap enabled
        unsigned int Attr4_Wrap                : 1;   // Attribute 4 D3Dwrap flag, 0=normal, 1=wrap enabled
        unsigned int Attr5_Wrap                : 1;   // Attribute 5 D3Dwrap flag, 0=normal, 1=wrap enabled
        unsigned int Attr6_Wrap                : 1;   // Attribute 6 D3Dwrap flag, 0=normal, 1=wrap enabled
        unsigned int Attr7_Wrap                : 1;   // Attribute 7 D3Dwrap flag, 0=normal, 1=wrap enabled
        unsigned int Attr8_Wrap                : 1;   // Attribute 8 D3Dwrap flag, 0=normal, 1=wrap enabled
        unsigned int Attr9_Wrap                : 1;   // Attribute 9 D3Dwrap flag, 0=normal, 1=wrap enabled
        unsigned int Attr10_Wrap               : 1;   // Attribute 10 D3Dwrap flag, 0=normal, 1=wrap enabled
        unsigned int Attr11_Wrap               : 1;   // Attribute 11 D3Dwrap flag, 0=normal, 1=wrap enabled
        unsigned int Attr12_Wrap               : 1;   // Attribute 12 D3Dwrap flag, 0=normal, 1=wrap enabled
        unsigned int Attr13_Wrap               : 1;   // Attribute 13 D3Dwrap flag, 0=normal, 1=wrap enabled
        unsigned int Attr14_Wrap               : 1;   // Attribute 14 D3Dwrap flag, 0=normal, 1=wrap enabled
        unsigned int Attr15_Wrap               : 1;   // Attribute 15 D3Dwrap flag, 0=normal, 1=wrap enabled
    } reg;
    unsigned int uint;
} Reg_Const_Wrap_Mask;
 
typedef struct _Tas_regs
{
    Reg_Tas_Ctrl                     reg_Tas_Ctrl;
    Reg_Scissor_X                    reg_Scissor_X;
    Reg_Scissor_Y                    reg_Scissor_Y;
    Reg_Z_Scale_Factor               reg_Z_Scale_Factor;
    Reg_Z_Bias                       reg_Z_Bias;
    Reg_Path_Bbox_X                  reg_Path_Bbox_X;
    Reg_Path_Bbox_Y                  reg_Path_Bbox_Y;
    Reg_Const_Wrap_Mask              reg_Const_Wrap_Mask;
} Tas_regs;

#endif
