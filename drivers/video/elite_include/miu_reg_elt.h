//    Spec Last Modified Time: 2011-7-13 11:14:15
#ifndef _MIU_REG_ELT_H
#define _MIU_REG_ELT_H


#ifndef        MIU_BLOCKBASE_INF
    #define    MIU_BLOCKBASE_INF
    #define    BLOCK_MIU_VERSION 1
    #define    BLOCK_MIU_TIMESTAMP "2011-7-13 11:14:15"
    #define    MIU_BLOCK                                                  0xF // match with BlockID.h
    #define    MIU_REG_START                                              0x0 // match with BlockID.h
    #define    MIU_REG_END                                                0x38 // match with BlockID.h
    #define    MIU_REG_LIMIT                                              0x38 // match with BlockID.h
#endif

// Register offset definition
#define        Reg_Skip_Offset                                            0x0
#define        Reg_Dynamic_Fb_Control_Offset                              0x35
#define        Reg_Dynamic_Fb_Index_Offset                                0x36
#define        Reg_Dynamic_Fb_Content_Offset                              0x37

// Block constant definition

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////            MIU Block (Block ID = 15) Register Definitions                            ///////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
typedef union
{
    struct
    {
        unsigned int     uint;
    } reg;
    unsigned int uint;
} Reg_Skip;
 
typedef union
{
    struct
    {
        unsigned int Dsref_En                  : 1;   // Dynamic self refresh enable
        unsigned int Pd_En                     : 1;   // enable dynamic power down
        unsigned int Dynamic_Fb_En             : 1;   // enable dynamic frame buffer
        unsigned int Reserved                  : 13;  // RESERVED
        unsigned int Dsref_Cyc                 : 16;  // Number in MCLK of dynamic self refresh entry
    } reg;
    unsigned int uint;
} Reg_Dynamic_Fb_Control;
 
typedef union
{
    struct
    {
        unsigned int Index                     : 7;   // Dynamic FB table register index(0-128)
        unsigned int Reserved                  : 25;  // RESERVED
    } reg;
    unsigned int uint;
} Reg_Dynamic_Fb_Index;
 
typedef union
{
    struct
    {
        unsigned int Reserved                  : 16;  // RESERVED
        unsigned int Page_Disable              : 1;   // Page Disable
        unsigned int Address                   : 15;  // Dynamic FB table content
    } reg;
    unsigned int uint;
} Reg_Dynamic_Fb_Content;
 
typedef struct _Miu_regs
{
    Reg_Skip                         reg_Skip[53];
    Reg_Dynamic_Fb_Control           reg_Dynamic_Fb_Control;
    Reg_Dynamic_Fb_Index             reg_Dynamic_Fb_Index;
    Reg_Dynamic_Fb_Content           reg_Dynamic_Fb_Content;
} Miu_regs;

#endif
