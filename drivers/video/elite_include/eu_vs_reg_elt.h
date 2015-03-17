//    Spec Last Modified Time: 2010/12/16 17:05:50
#ifndef _EU_VS_REG_ELT_H
#define _EU_VS_REG_ELT_H


#ifndef        EU_VS_BLOCKBASE_INF
    #define    EU_VS_BLOCKBASE_INF
    #define    BLOCK_EU_VS_VERSION 1
    #define    BLOCK_EU_VS_TIMESTAMP "2010/12/16 17:05:50"
    #define    EU_VS_BLOCK                                                0x1 // match with BlockID.h
    #define    EU_VS_REG_START                                            0x0 // match with BlockID.h
    #define    EU_VS_REG_END                                              0x8D // match with BlockID.h
    #define    EU_VS_REG_LIMIT                                            0x8D // remember to manually change this every time! match with BlockID.h
#endif

// Register offset definition
#define        Reg_Vcs_Cfg_Offset                                         0x0
#define        Reg_Vcs_Ctrl_Offset                                        0x1
#define        Reg_Gp_Ctrl_Offset                                         0x2
#define        Reg_Vcs_Pc_Offset                                          0x3
#define        Reg_Th_Mem_Offset                                          0x4
#define        Reg_Th_Mem_Range_Offset                                    0x5
#define        Reg_Vs_Output_Mask_Offset                                  0x6
#define        Reg_Vs_Output_Mask_Ex_Offset                               0x7
#define        Reg_So_Addr_Offset                                         0x8
#define        Reg_So_Range_Offset                                        0x9
#define        Reg_So_Stride_Offset                                       0xA
#define        Reg_So_Prim_Cnt_Offset                                     0xB
#define        Reg_So_Prim_Needed_Offset                                  0xC
#define        Reg_Vcs_Const_Offset                                       0xD
#define        Reg_Int_Pc_Offset                                          0x8D
#define        Reg_Resume_Pc_Offset                                       0x8E
#define        Reg_Debug_Cfg_Offset                                       0x8F
#define        Reg_Debug_Stat_Offset                                      0x90
#define        Reg_Bp_Pc_Offset                                           0x91
#define        Reg_Th_Pc_Offset                                           0x99

#define        Reg_Vcs_Const_HWAddr                                       0x80
// Block constant definition
typedef enum
{
    VCS_CFG_GP_MODE_EN_DISABLED              = 0,   // 0: Disable: Operate in 3D mode
    VCS_CFG_GP_MODE_EN_ENABLED               = 1,   // 1: Enable GP operation mode, the VS shader registers are
                                                    // used for GP shader
} VCS_CFG_GP_MODE_EN;
typedef enum
{
    VCS_CFG_IDX_AND_V_MASK_DISABLED          = 0,   // 0: Disable
    VCS_CFG_IDX_AND_V_MASK_ENABLED           = 1,   // 1: Enable
} VCS_CFG_IDX_AND_V_MASK;
typedef enum
{
    VCS_CFG_IC_MODE_LINE                     = 0,   // 0: Line based, the entire line is requested
    VCS_CFG_IC_MODE_SECTOR                   = 1,   // 1: Sector based, only the missed sector is requested
} VCS_CFG_IC_MODE;
typedef enum
{
    VCS_CFG_CB_MODE_DOUBLE_CONTEXT           = 0,   // 0: Double context mode, the driver programs this mode when
                                                    // the total VS constant size is less than or equal to 16.
    VCS_CFG_CB_MODE_SINGLE_CONTEXT           = 1,   // 1: Single context mode, the driver programs this mode when
                                                    // the total VS constant size is greater than 16.
} VCS_CFG_CB_MODE;
typedef enum
{
    VCS_CFG_DEBUG_EN_DISABLED                = 0,   // 0: Disable, normal mode
    VCS_CFG_DEBUG_EN_ENABLED                 = 1,   // 1: Enable, the EU enables breakpoint handling based on
                                                    // MMIO shader debugging register settings
} VCS_CFG_DEBUG_EN;
typedef enum
{
    VCS_CFG_PWR_MODE_OFF                     = 0,   // 0: EU power down
    VCS_CFG_PWR_MODE_SET_01_ON               = 1,   // 1: Set 0/1 on, set 2/3 off
    VCS_CFG_PWR_MODE_BOTH_ON                 = 2,   // 2: Both set 0/1, set 2/3 on
} VCS_CFG_PWR_MODE;
typedef enum
{
    VCS_CFG_SO_EN_DISABLED                   = 0,   // 0: Disable, default mode
    VCS_CFG_SO_EN_ENABLED                    = 1,   // 1: Enable, the EU needs to read vertex data from the VC,
                                                    // assembe the primitives and write to memory. No raster
                                                    // output in this case.
} VCS_CFG_SO_EN;
typedef enum
{
    VCS_CFG_SO_CNT_EN_DISABLED               = 0,   // 0: Disable, default mode
    VCS_CFG_SO_CNT_EN_ENABLED                = 1,   // 1: Enable, the EU enables SO counter increment.
} VCS_CFG_SO_CNT_EN;
typedef enum
{
    VCS_CFG_2_SLC_EN_DISABLED                = 0,   // 0: Disable, single slice mode.
    VCS_CFG_2_SLC_EN_ENABLED                 = 1,   // 1: Enable, two slices mode.
} VCS_CFG_2_SLC_EN;
typedef enum
{
    VCS_CTRL_VID_EN_VID_NONE                 = 0,   // 0: no VID input to VS
    VCS_CTRL_VID_EN_VID_AV                   = 1,   // 1: has VID input to VS
} VCS_CTRL_VID_EN;
typedef enum
{
    VCS_CTRL_IID_EN_IID_NONE                 = 0,   // 0: no IID input to VS
    VCS_CTRL_IID_EN_IID_AV                   = 1,   // 1: has IID input to VS
} VCS_CTRL_IID_EN;
typedef enum
{
    VCS_CTRL_RD_MODE_NEAR                    = 0,   // 00: Round toward nearest even
    VCS_CTRL_RD_MODE_ZERO                    = 1,   // 01: Round to Zero
    VCS_CTRL_RD_MODE_POS                     = 2,   // 10: Round to Positive Infinity
    VCS_CTRL_RD_MODE_NEG                     = 3,   // 11: Round to Negative infinity
} VCS_CTRL_RD_MODE;
typedef enum
{
    VCS_CTRL_FP_MODE_STD_IEEE                = 0,   // 00: Standard IEEE-745
    VCS_CTRL_FP_MODE_NSTD_IEEE               = 1,   // 01: Non-Standard IEEE-745
    VCS_CTRL_FP_MODE_MS_ALT                  = 2,   // 10: Microsoft Alt float point
    VCS_CTRL_FP_MODE_RESERVED                = 3,   // 11: Reserved
} VCS_CTRL_FP_MODE;
typedef enum
{
    VCS_CTRL_ATTR_PACK_EN_DISABLED           = 0,   // 0: attribute packing in TAS is disabled
    VCS_CTRL_ATTR_PACK_EN_ENABLED            = 1,   // 1: attribute packing in TAS is enabled, the EU needs to
                                                    // discard the attribute when all 4 compoments are not valid
} VCS_CTRL_ATTR_PACK_EN;
typedef enum
{
    VCS_CTRL_OPENGL_DX                       = 0,   // 0: DX WinMobile
    VCS_CTRL_OPENGL_OGL                      = 1,   // 1: OpenGL ES
} VCS_CTRL_OPENGL;
typedef enum
{
    GP_CTRL_EXEC_MODE_FP                     = 0,   // 0: Full precision mode
    GP_CTRL_EXEC_MODE_PP                     = 1,   // 1: Partial precision mode
} GP_CTRL_EXEC_MODE;
typedef enum
{
    GP_CTRL_SM_SET_CFG_1_SET                 = 0,   // 0: Thread group resides in 1 set, no across sets SM
                                                    // read/write
    GP_CTRL_SM_SET_CFG_2_SETS                = 1,   // 1: Thread group resides in 2 sets, set 0/1 or set 2/3. The SM
                                                    // address space starts from set 0 or 2, continues to across 2
                                                    // sets. The SM size in each set is same.
    GP_CTRL_SM_SET_CFG_4_SETS                = 2,   // 2: Thread group resides in 4 sets, The SM address space
                                                    // starts from set 0, continues to across 4 sets.  The SM size in
                                                    // each set is same.
} GP_CTRL_SM_SET_CFG;
typedef enum
{
    TH_MEM_RANGE_ELEMENT_STRIDE_1KBIT        = 0, 
    TH_MEM_RANGE_ELEMENT_STRIDE_2KBIT        = 1, 
    TH_MEM_RANGE_ELEMENT_STRIDE_4KBIT        = 2, 
    TH_MEM_RANGE_ELEMENT_STRIDE_8KBIT        = 3, 
} TH_MEM_RANGE_ELEMENT_STRIDE;
typedef enum
{
    TH_MEM_RANGE_TH_MEM_SIZE_64              = 0, 
    TH_MEM_RANGE_TH_MEM_SIZE_128             = 1, 
    TH_MEM_RANGE_TH_MEM_SIZE_256             = 2, 
    TH_MEM_RANGE_TH_MEM_SIZE_512             = 3, 
    TH_MEM_RANGE_TH_MEM_SIZE_1024            = 4, 
    TH_MEM_RANGE_TH_MEM_SIZE_2048            = 5, 
    TH_MEM_RANGE_TH_MEM_SIZE_4096            = 6, 
    TH_MEM_RANGE_TH_MEM_SIZE_4160            = 7, 
} TH_MEM_RANGE_TH_MEM_SIZE;
typedef enum
{
    DEBUG_CFG_EXEC_EN_DISABLED               = 0,   // 0: Disable, the EU clears this flag when it reaches one of
                                                    // the breakpoints, and waits until the debugger sets this
                                                    // flag to resume execution
    DEBUG_CFG_EXEC_EN_ENABLED                = 1,   // 1: Enable, the debugger sets this flag to notify the EU to
                                                    // start/resume execution
} DEBUG_CFG_EXEC_EN;
typedef enum
{
    DEBUG_CFG_TH_MODE_SINGLE                 = 0,   // 0: Single, the EU starts/resumes one thread only,
                                                    // specified by the hardware thread ID
    DEBUG_CFG_TH_MODE_ALL                    = 1,   // 1: All, the EU starts/resumes all available threads
} DEBUG_CFG_TH_MODE;
typedef enum
{
    DEBUG_CFG_INT_EN_DISABLED                = 0,   // 0: Disable, the EU will NOT call the interrupt routine when
                                                    // it hits the breakpoints
    DEBUG_CFG_INT_EN_ENABLED                 = 1,   // 1: Enable, the EU will call the interrupt routine when it
                                                    // hits the breakpoints
} DEBUG_CFG_INT_EN;
typedef enum
{
    DEBUG_CFG_RESUME_EN_DISABLED             = 0,   // 0: Disable, the EU will NOT call the resume routine when it
                                                    // resumes execution.
    DEBUG_CFG_RESUME_EN_ENABLED              = 1,   // 1: Enable, the EU will call the resume routine when it
                                                    // resumes execution.
} DEBUG_CFG_RESUME_EN;
typedef enum
{
    DEBUG_CFG_RESUME_MODE_SINGLE             = 0,   // 0: Single, the EU runs the resume routine for one thread
                                                    // only, specified by the resume routine hardware thread ID
    DEBUG_CFG_RESUME_MODE_ALL                = 1,   // 1: All, the EU runs the resume routine for all available
                                                    // threads
} DEBUG_CFG_RESUME_MODE;
typedef enum
{
    DEBUG_STAT_BP_HIT_DISABLED               = 0,   // 0: Disable, the EU hasn't hit the breakpoints. The EU needs
                                                    // to clear this flag when it receives the debugger's write in
                                                    // Debug_cfg to set exec_en
    DEBUG_STAT_BP_HIT_ENABLED                = 1,   // 1: Enable, when the EU hits one of the breakpoints, it will
                                                    // suspend all threads, execute the interrupt routine then
                                                    // set this flag
} DEBUG_STAT_BP_HIT;
typedef enum
{
    BP_PC_VALID_DISABLED                     = 0,   // 0: Disable, the current breakpoint slot is disabled
    BP_PC_VALID_ENABLED                      = 1,   // 1: Enable, the current breakpoint slot is enabled
} BP_PC_VALID;
typedef enum
{
    BP_PC_CHK_MODE_SINGLE                    = 0,   // 0: Single, the EU checks one thread's PC only, specified by
                                                    // th_id in Bp_pc
    BP_PC_CHK_MODE_ALL                       = 1,   // 1: All, the EU checks all available threads' PC
} BP_PC_CHK_MODE;
typedef enum
{
    TH_PC_VALID_DISABLED                     = 0,   // 0: Disable, the current thread is not valid
    TH_PC_VALID_ENABLED                      = 1,   // 1: Enable, the current thread is valid
} TH_PC_VALID;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////            EU_VS Block (Block ID = 1) Register Definitions                           ///////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
typedef union
{
    struct
    {
        unsigned int Gp_Mode_En                : 1;   // General Purpose Shaders Enable
        unsigned int Idx_And_V_Mask            : 1;   // Use valid mask P7 for CRF indexing instruction split, in PS
                                                      // the quad based valid mask is used, in other shaders the
                                                      // valid mask is used
        unsigned int Ic_Mode                   : 1;   // Instruction cache mode
        unsigned int Cb_Mode                   : 1;   // VS Constant Buffer update mode
        unsigned int Max_Threads               : 4;   // Maximum number of total VS/CS threads kickoff in the EU.
                                                      // Value 0~15 for 1~16 threads.
        unsigned int Sm_Conf                   : 8;   // Total shared CRF/SM RAM configured for SM usage (0~128
                                                      // lines per CRF set)
        unsigned int Debug_En                  : 1;   // GP shader debugging mode enable
        unsigned int Pwr_Mode                  : 2;   // EU power-gating mode
        unsigned int So_En                     : 1;   // VS Stream Out enable
        unsigned int So_Cnt_En                 : 1;   // VS Stream Out counter increment enable
        unsigned int two_Slc_En                : 1;   // Two slices enable
        unsigned int Reserved                  : 10;  // Reserved
    } reg;
    unsigned int uint;
} Reg_Vcs_Cfg;
 
typedef union
{
    struct
    {
        unsigned int Thread_Size               : 2;   // Thread packing size. Value is 0/1/2 for 1/2/4 segments,
                                                      // each segment contains 8V
        unsigned int Crf_Size                  : 6;   // Total Common Register File (CRF) space. Value is 1~32
                                                      // 4-component elements, including temp, input, output
                                                      // registers.
        unsigned int Out_Size                  : 4;   // VS Output Size. Value is 1~16 4-component elements, note
                                                      // size of 0 is not allowed
        unsigned int In_Size                   : 4;   // VS Input Size. Value is 1~8 4-component elements.
                                                      //  
        unsigned int Vid_En                    : 1;   // VertexID available as VS inputs, when it's available the
                                                      // hardware needs to store it in the thread CRF address 0.
        unsigned int Iid_En                    : 1;   // InstanceID available as VS inputs, when it's available
                                                      // the hardware needs to store it in the thread CRF address 1.
        unsigned int Rd_Mode                   : 2;   // EU floating point rounding mode
        unsigned int Fp_Mode                   : 2;   // EU floating point operation mode
        unsigned int Attr_Pack_En              : 1;   // Attribute packing in TAS enable
        unsigned int Opengl                    : 1;   // API selection, used for API specific handling, strip
                                                      // winding direction, leading vertex, etc
        unsigned int Reserved                  : 8;   // Reserved
    } reg;
    unsigned int uint;
} Reg_Vcs_Ctrl;
 
typedef union
{
    struct
    {
        unsigned int Sm_Size                   : 8;   // Shared Memory size (0~128 lines) for each Thread Group.
        unsigned int Th_Num                    : 4;   // Total number (1~8) of EU hardware threads for each Thread
                                                      // Group.
        unsigned int Exec_Mode                 : 1;   // GP initial execution mode, this affects how the CSC
                                                      // inserts the thread IDs into the CRF
        unsigned int Sm_Set_Cfg                : 2;   // SM set configuration
        unsigned int Reserved                  : 17;  // Reserved
    } reg;
    unsigned int uint;
} Reg_Gp_Ctrl;
 
typedef union
{
    struct
    {
        unsigned int Address                   : 32;  // VS/CS Instruction Base Address
    } reg;
    unsigned int uint;
} Reg_Vcs_Pc;
 
typedef union
{
    struct
    {
        unsigned int Address                   : 32;  // EU Thread Local Memory Base Address
    } reg;
    unsigned int uint;
} Reg_Th_Mem;
 
typedef union
{
    struct
    {
        unsigned int Th_Mem_Range              : 14;  // Range of thread external storage in memory including call
                                                      // stacks, register spill, OCL private memory or CS x#
                                                      // buffers
        unsigned int Element_Stride            : 2;   // Element stride of thread memory
        unsigned int Th_Mem_Size               : 4;   // Size of thread external storage in memory including call
                                                      // stacks, register spill, OCL private memory or CS x#
                                                      // buffers
        unsigned int Reserved1                 : 12;  // Reserved
    } reg;
    unsigned int uint;
} Reg_Th_Mem_Range;
 
typedef union
{
    struct
    {
        unsigned int O0_Mask                   : 4;   // VS output O0 mask
        unsigned int O1_Mask                   : 4;   // VS output O1 mask
        unsigned int O2_Mask                   : 4;   // VS output O2 mask
        unsigned int O3_Mask                   : 4;   // VS output O3 mask
        unsigned int O4_Mask                   : 4;   // VS output O4 mask
        unsigned int O5_Mask                   : 4;   // VS output O5 mask
        unsigned int O6_Mask                   : 4;   // VS output O6 mask
        unsigned int O7_Mask                   : 4;   // VS output O7 mask
    } reg;
    unsigned int uint;
} Reg_Vs_Output_Mask;
 
typedef union
{
    struct
    {
        unsigned int O8_Mask                   : 4;   // VS output O8 mask
        unsigned int O9_Mask                   : 4;   // VS output O9 mask
        unsigned int O10_Mask                  : 4;   // VS output O10 mask
        unsigned int O11_Mask                  : 4;   // VS output O11 mask
        unsigned int O12_Mask                  : 4;   // VS output O12 mask
        unsigned int O13_Mask                  : 4;   // VS output O13 mask
        unsigned int O14_Mask                  : 4;   // VS output O14 mask
        unsigned int O15_Mask                  : 4;   // VS output O15 mask
    } reg;
    unsigned int uint;
} Reg_Vs_Output_Mask_Ex;
 
typedef union
{
    struct
    {
        unsigned int Address                   : 32;  // Stream Out buffer base address
    } reg;
    unsigned int uint;
} Reg_So_Addr;
 
typedef union
{
    struct
    {
        unsigned int Range                     : 32;  // Stream Out buffer range
    } reg;
    unsigned int uint;
} Reg_So_Range;
 
typedef union
{
    struct
    {
        unsigned int Stride                    : 32;  // Stream Out vertex stride, next vertex start address =
                                                      // current vertex start address + stride
    } reg;
    unsigned int uint;
} Reg_So_Stride;
 
typedef union
{
    struct
    {
        unsigned int Prim_Cnt                  : 32;  // Stream Out number of primitives written
    } reg;
    unsigned int uint;
} Reg_So_Prim_Cnt;
 
typedef union
{
    struct
    {
        unsigned int Prim_Needed               : 32;  // Stream Out number of primitives needed
    } reg;
    unsigned int uint;
} Reg_So_Prim_Needed;
 
typedef union
{
    struct
    {
        unsigned int Const                     : 32;  // VS constant buffer 32*128-bit
    } reg;
    unsigned int uint;
} Reg_Vcs_Const;
 
typedef union
{
    struct
    {
        unsigned int Address                   : 32;  // GP debugging interrupt routine PC base address
    } reg;
    unsigned int uint;
} Reg_Int_Pc;
 
typedef union
{
    struct
    {
        unsigned int Address                   : 32;  // GP debugging resume routine PC base address
    } reg;
    unsigned int uint;
} Reg_Resume_Pc;
 
typedef union
{
    struct
    {
        unsigned int Exec_En                   : 1;   // Thread execution enable
        unsigned int Th_Mode                   : 1;   // Thread debugging execution mode
        unsigned int Int_En                    : 1;   // Interrupt routine enable
        unsigned int Th_Id                     : 5;   // Hardware thread ID (0~31) for the thread to execute in
                                                      // single thread mode
        unsigned int Resume_En                 : 1;   // Resume routine enable
        unsigned int Resume_Mode               : 1;   // Resume routine execution mode
        unsigned int Resume_Th_Id              : 5;   // Hardware thread ID (0~31) for the thread to run the resume
                                                      // routine in single thread mode
		unsigned int Th_Alloc_Order_En         : 1;   // Thread ID allocation in order enable, to debug HW local testbench
        unsigned int Reserved                  : 16;  // Reserved
    } reg;
    unsigned int uint;
} Reg_Debug_Cfg;
 
typedef union
{
    struct
    {
        unsigned int Bp_Hit                    : 1;   // Thread hits breakpoints
        unsigned int Bp_Id                     : 3;   // Breakpoint ID that gets hit
        unsigned int Th_Id                     : 5;   // Hardware thread ID that hits breakpoint
        unsigned int Reserved                  : 23;  // Reserved
    } reg;
    unsigned int uint;
} Reg_Debug_Stat;
 
typedef union
{
    struct
    {
        unsigned int Valid                     : 1;   // Breakpoint slot 0~3 valid
        unsigned int Chk_Mode                  : 1;   // Breakpoint check mode
        unsigned int Th_Id                     : 5;   // Hardware thread ID (0~31) for the thread to check PC
        unsigned int Reserved                  : 7;   // Reserved
        unsigned int Offset                    : 18;  // Breakpoint PC offset, 2's comp signed integer number. The
                                                      // EU hardware needs to calculate the PC address from this
                                                      // offset and base PC address.
    } reg;
    unsigned int uint;
} Reg_Bp_Pc;
 
typedef union
{
    struct
    {
        unsigned int Valid                     : 1;   // Hardware thread slot 0~31 valid
        unsigned int Address                   : 31;  // The current hardware thread's PC address
    } reg;
    unsigned int uint;
} Reg_Th_Pc;
 
typedef struct _Eu_Vs_regs
{
    Reg_Vcs_Cfg                      reg_Vcs_Cfg;
    Reg_Vcs_Ctrl                     reg_Vcs_Ctrl;
    Reg_Gp_Ctrl                      reg_Gp_Ctrl;
    Reg_Vcs_Pc                       reg_Vcs_Pc;
    Reg_Th_Mem                       reg_Th_Mem;
    Reg_Th_Mem_Range                 reg_Th_Mem_Range;
    Reg_Vs_Output_Mask               reg_Vs_Output_Mask;
    Reg_Vs_Output_Mask_Ex            reg_Vs_Output_Mask_Ex;
    Reg_So_Addr                      reg_So_Addr;
    Reg_So_Range                     reg_So_Range;
    Reg_So_Stride                    reg_So_Stride;
    Reg_So_Prim_Cnt                  reg_So_Prim_Cnt;
    Reg_So_Prim_Needed               reg_So_Prim_Needed;
    Reg_Vcs_Const                    reg_Vcs_Const[128];
    Reg_Int_Pc                       reg_Int_Pc;
    Reg_Resume_Pc                    reg_Resume_Pc;
    Reg_Debug_Cfg                    reg_Debug_Cfg;
    Reg_Debug_Stat                   reg_Debug_Stat;
    Reg_Bp_Pc                        reg_Bp_Pc[8];
    Reg_Th_Pc                        reg_Th_Pc[16];
} Eu_Vs_regs;

#endif
