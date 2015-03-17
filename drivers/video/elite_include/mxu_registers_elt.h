//    Spec Last Modified Time: 8/3/2011 2:48:21 PM
#ifndef _MXU_REGISTERS_ELT_H
#define _MXU_REGISTERS_ELT_H


#ifndef        MXU_BLOCKBASE_INF
    #define    MXU_BLOCKBASE_INF
    #define    BLOCK_MXU_VERSION 1
    #define    BLOCK_MXU_TIMESTAMP "8/3/2011 2:48:21 PM"
    #define    MXU_BLOCK                                                  0x7 // match with BlockID.h
    #define    MXU_REG_START                                              0x0 // match with BlockID.h
    #define    MXU_REG_END                                                0x9 // match with BlockID.h
    #define    MXU_REG_LIMIT                                              0x9 // match with BlockID.h
#endif

// Register offset definition
#define        Reg_Mxu_Skip_Offset                                        0x0
#define        Reg_Mxu_Misc_Offset                                        0x2
#define        Reg_Page_Table_System_Address_Offset                       0x3
#define        Reg_Mxu_Inv_Pagetbl_Addr_Offset                            0x4
#define        Reg_Mxu_Inv_Pagetbl_Mask_Offset                            0x5
#define        Reg_Mxu_Inv_Pagetbl_Trig_Offset                            0x6
#define        Reg_Mxu_Channel_Control_Offset                             0x7
#define        Reg_Mxu_Extra_Offset                                       0x8

// Block constant definition
typedef enum
{
    MXU_MISC_SYS_VID_BOUNDARY_MB_32          = 0,   // 000: 32 MB
    MXU_MISC_SYS_VID_BOUNDARY_MB_64          = 1,   // 001: 64 MB
    MXU_MISC_SYS_VID_BOUNDARY_MB_128         = 2,   // 010: 128 MB
    MXU_MISC_SYS_VID_BOUNDARY_MB_256         = 3,   // 011: 256 MB (default)
    MXU_MISC_SYS_VID_BOUNDARY_MB_512         = 4,   // 100: 512 MB
    MXU_MISC_SYS_VID_BOUNDARY_GB_1           = 5,   // 101: 1 GB
    MXU_MISC_SYS_VID_BOUNDARY_GB_2           = 6,   // 110: 2 GB
    MXU_MISC_SYS_VID_BOUNDARY_GB_4           = 7,   // 111: 4 GB
} MXU_MISC_SYS_VID_BOUNDARY;
typedef enum
{
    MXU_INV_PAGETBL_TRIG_TLB_TYPE_LPT        = 0,   // 00: LP Table (a valid option for this product)
    MXU_INV_PAGETBL_TRIG_TLB_TYPE_GPT        = 1,   // 01: GP Table, not valid 
    MXU_INV_PAGETBL_TRIG_TLB_TYPE_HPT        = 2,   // 10: HP Table, not valid 
    MXU_INV_PAGETBL_TRIG_TLB_TYPE_CPT        = 3,   // 11: CP Table, not valid 
} MXU_INV_PAGETBL_TRIG_TLB_TYPE;
typedef enum
{
    MXU_INV_PAGETBL_TRIG_EXT_OPCODE_MODULE_INVALID_TLB_CACHELINE= 0,
                                                    // 0: INVALID TLB COMMAND
} MXU_INV_PAGETBL_TRIG_EXT_OPCODE_MODULE;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////            MXU Block (Block ID = 7) Register Definitions                             ///////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
typedef union
{
    struct
    {
        unsigned int     uint;
    } reg;
    unsigned int uint;
} Reg_Mxu_Skip;
 
typedef union
{
    struct
    {
        unsigned int Sys_Vid_Boundary          : 3;   // RESERVED
        unsigned int Reserved                  : 29;  // Reserved
    } reg;
    unsigned int uint;
} Reg_Mxu_Misc;
 
typedef union
{
    struct
    {
        unsigned int Reserved                  : 6;   // Reserved
        unsigned int Addr                      : 26;  // Physical address bits D31:D5 of system memory page table
                                                      // (32-byte aligned)
    } reg;
    unsigned int uint;
} Reg_Page_Table_System_Address;
 
typedef union
{
    struct
    {
        unsigned int     uint;
    } reg;
    unsigned int uint;
} Reg_Mxu_Inv_Pagetbl_Addr;
 
typedef union
{
    struct
    {
        unsigned int     uint;
    } reg;
    unsigned int uint;
} Reg_Mxu_Inv_Pagetbl_Mask;
 
typedef union
{
    struct
    {
        unsigned int Reserved                  : 10;  // RESERVED
        unsigned int Tlb_Type                  : 2;   // Table Type
        unsigned int Ext_Opcode_Module         : 8;   // External OPCODE Module
        unsigned int Block_Id                  : 6;   // Block ID
        unsigned int Dwf                       : 2;   // DWords to Follow
        unsigned int Major_Opcode              : 4;   // Major OPCODE
    } reg;
    unsigned int uint;
} Reg_Mxu_Inv_Pagetbl_Trig;
 
typedef union
{
    struct
    {
        unsigned int Single_Channel_Enable     : 1;   // 0: Two channels, 1: One channel
        unsigned int Large_Channel_Swizzle_Enable : 1;// When it is enable, and address is within large channel
                                                      // swizzle boundaary, we will use A17 as channel selector;
                                                      // otherwise, still use A8^A9.
        unsigned int Reserved                  : 20;  // RESERVED
        unsigned int Large_Channel_Swizzle_Boudnary : 10;
                                                      // The boundary is always aligned in 4MB.
    } reg;
    unsigned int uint;
} Reg_Mxu_Channel_Control;
 
typedef union
{
    struct
    {
        unsigned int Slice1_Valild             : 1;   // 1: Two slice are both valid. 0: Only slice 0 is valid.
        unsigned int Reserved                  : 31;  // RESERVED
    } reg;
    unsigned int uint;
} Reg_Mxu_Extra;
 
typedef struct _Mxu_regs
{
    Reg_Mxu_Skip                     reg_Mxu_Skip[2];
    Reg_Mxu_Misc                     reg_Mxu_Misc;
    Reg_Page_Table_System_Address    reg_Page_Table_System_Address;
    Reg_Mxu_Inv_Pagetbl_Addr         reg_Mxu_Inv_Pagetbl_Addr;
    Reg_Mxu_Inv_Pagetbl_Mask         reg_Mxu_Inv_Pagetbl_Mask;
    Reg_Mxu_Inv_Pagetbl_Trig         reg_Mxu_Inv_Pagetbl_Trig;
    Reg_Mxu_Channel_Control          reg_Mxu_Channel_Control;
    Reg_Mxu_Extra                    reg_Mxu_Extra;
} Mxu_regs;

#endif
