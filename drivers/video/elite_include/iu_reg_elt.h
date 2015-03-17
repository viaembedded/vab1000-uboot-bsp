//    Spec Last Modified Time: 12/8/2009 12:15:49 PM
#ifndef _IU_REG_ELT_H
#define _IU_REG_ELT_H

#ifndef        IU_BLOCKBASE_INF
    #define    IU_BLOCKBASE_INF
    #define    BLOCK_IU_VERSION 1
    #define    BLOCK_IU_TIMESTAMP "12/8/2009 12:15:49 PM"
    #define    IU_BLOCK                                                0x4 // match with BlockID.h
    #define    IU_REG_START                                            0x0 // match with BlockID.h
    #define    IU_REG_END                                              0x2 // match with BlockID.h
    #define    IU_REG_LIMIT                                            0x2 // match with BlockID.h
#endif

#define Reg_IU_Ctrl_Address           0x0
#define Reg_IU_Mapping_Address        0x1
#define Reg_IU_Mapping_Address_Ex     0x2

// Block constant definition
typedef enum
{
    IU_CTRL_INSERT_POS_DISABLED              = 0,   // 0: no position in PS inputs
    IU_CTRL_INSERT_POS_ENABLED               = 1,   // 1: insert position following normal varying attributes
} IU_CTRL_INSERT_POS;

typedef enum
{
    IU_CTRL_INSERT_FACE_DISABLED             = 0,   // 0: no face ID in PS inputs
    IU_CTRL_INSERT_FACE_ENABLED              = 1,   // 1: insert face ID following normal varying attributes and
                                                    // position if exists
} IU_CTRL_INSERT_FACE;

typedef enum
{
    IU_CTRL_YINVERT_EN_DISABLED          = 0,   // 0: no y invert
    IU_CTRL_YINVERT_EN_ENABLED           = 1,   // 1: y invert
} IU_CTRL_YINVERT_EN;

typedef enum
{
    IU_CTRL_ZBIAS_EN_DISABLED          = 0,   // 0: no y invert
    IU_CTRL_ZBIAS_EN_ENABLED           = 1,   // 1: y invert
} IU_CTRL_ZBIAS_EN;

typedef enum
{
    IU_CTRL_OUTPUT_FP                 = 0,   // 0: floating s12e5, leading 1 included
    IU_CTRL_OUTPUT_FX                 = 1,   // 1: fixed point 12.4
} IU_CTRL_OUTPUT_FMT;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////            IU Block (Block ID = 17) Register Definitions                             ///////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// some other description about the command usage: Start
typedef union
{
    struct                   // IU Control Register
   {
        unsigned int     Ps_Attr_Num           : 4;  // Number of total attributes, 1~10
        unsigned int     Ab_Attr_Num           : 4;  // Number of total attributes, 1~11
        unsigned int     Insert_Pos            : 1;  // Insert position
        unsigned int     Insert_Face           : 1;  // Insert face ID
        unsigned int     Y_Invert_En           : 1;  // PS input position y invert adjustment
        unsigned int     Z_Bias_En             : 1;
        unsigned int     Outp_Fmt              : 1;  // Output data format; 0:s12e5 1:FX12.4
        unsigned int     Md_4x1_En             : 1;  // 4x1 mode enable; 0:2x2 mode 1: 4x1 mode
        unsigned int     Pack_Z                : 1;  // insert z in 3rd component
        unsigned int     Pack_W                : 1;  // insert w in 4th component
        unsigned int     Rt_Height             : 13; // OES height adjustment
        unsigned int     Is_8p_Mode            : 1;  // 4p or 8p mode
        unsigned int     Pp_Mode               : 1;  // PS precision mode PP/FP
                                                     // 0: PS runs in PP mode, IU outputs attribute in FP24 at 8p4c
                                                     // 1: PS runs in FP mode, IU outputs attribute in FP32 at 2p4c
		unsigned int     Opengl                : 1; // API selection, used for API specific handling, raster rule, etc
                                                    // 0: windows dx9
                                                    // 1: OpenGL ES 2.0/3.0
    } reg;
    unsigned int uint;
}Register_Iu_Ctrl;

typedef union
{
    struct                // IU Attribute Mapping Table
   {
        unsigned int     Attr0_Id              : 4;  // Attribute ID in AB (0~9)
        unsigned int     Attr1_Id              : 4;  // Attribute ID in AB (0~9)
        unsigned int     Attr2_Id              : 4;  // Attribute ID in AB (0~9)
        unsigned int     Attr3_Id              : 4;  // Attribute ID in AB (0~9)
        unsigned int     Attr4_Id              : 4;  // Attribute ID in AB (0~9)
        unsigned int     Attr5_Id              : 4;  // Attribute ID in AB (0~9)
        unsigned int     Attr6_Id              : 4;  // Attribute ID in AB (0~9)
        unsigned int     Attr7_Id              : 4;  // Attribute ID in AB (0~9)
        
    }  reg;
    unsigned int uint;
}Register_Iu_Mapping;

typedef union
{
    struct
    {
        unsigned int     Attr8_Id              : 4;  // Attribute ID in AB (0~9)
        unsigned int     Attr9_Id              : 4;  // Attribute ID in AB (0~9)
        unsigned int     Reserved              : 24;  // Reserved
    } reg;
    unsigned int uint;
} Register_Iu_Mapping_Ex;

typedef struct _Iu_regs
{
   Register_Iu_Ctrl             reg_Iu_Ctrl;
   Register_Iu_Mapping          reg_Iu_Mapping;
   Register_Iu_Mapping_Ex       reg_Iu_Mapping_Ex;
}Iu_regs;

#endif
