//
//  REGISTERCOMMANDS.H
//
//  Hardware-specific header file for Excalibur-Lite chip.
//
#ifndef _REGISTERCOMMANDS_ELT_H_
#define _REGISTERCOMMANDS_ELT_H_

#if (defined(_WINNT) || defined(WINNT))
#if defined(DEBUG) || defined(_DEBUG)
#ifndef _WINDDI_
typedef LONG_PTR (APIENTRY *PFN)();

#ifdef __cplusplus
extern "C" {
#endif

VOID APIENTRY EngDebugBreak(
    VOID
    );

#ifdef __cplusplus
}  // extern "C"
#endif


#endif
#endif
#endif

typedef unsigned char BYTE;
typedef unsigned int  UINT;
typedef unsigned int  DWORD;
typedef unsigned int  ULONG;
typedef int  LONG;
typedef int  BOOL;

/***********************
    Commands
************************/
#include "blockid_elt.h"
//current interface file
#include "bci_opcode_elt.h"
#include "ff_registers_elt.h"
#include "surface_format_elt.h"


#define ELT_BUSY_STATUS_REG                 (MMIO_BCI_START_ADDRESS + 4*Reg_Block_Busy_Bits_Offset)

typedef struct RECT_ELT
{
    union 
    {
        Cmd_Block_Command_Setup_Sg_Dword0   DW0;
        UINT Value1;
    };
    union 
    {
        Cmd_Block_Command_Setup_Sg_Dword1   DW1;
        UINT Value2;
    };
} RECT_ELT;

// block command bits definition for dx, dy
typedef struct _BC_DX_DY_ELT
{
    union 
    {
        Cmd_Block_Command_Setup_Sg_Blt_Dword2   DW2;
        UINT                                    Value;
    };
} BC_DX_DY_ELT;

typedef struct _S3_E2d_regs
{
    Ff_regs             FF2DRegs;    
    RECT_ELT            DstRect;
    BC_DX_DY_ELT        DxDy;
    ULONG               DrawFlags;
    DWORD               FillColor[2];
    ULONG               ImageSize;
} S3_E2d_regs;

#define BFSET(dst, dstMSB, dstLSB, src) \
    ((((dst) & ~(((2 <<((dstMSB) -(dstLSB))) -1) <<(dstLSB))) | \
    (((src) & ((2 <<((dstMSB) -(dstLSB))) -1)) <<(dstLSB))))

#define BFGET(src, srcMSB, srcLSB) \
    ((((src) >>(srcLSB)) & ((2 <<((srcMSB) -(srcLSB))) -1)))

#define BF64SET(dst, dstMSB, dstLSB, src) \
    ((((dst) & ~((((UINT64)2 <<((dstMSB) -(dstLSB))) -1) <<(dstLSB))) | \
    (((src) & (((UINT64)2 <<((dstMSB) -(dstLSB))) -1)) <<(dstLSB))))

#define BF64GET(src, srcMSB, srcLSB) \
    ((((src) >>(srcLSB)) & (((UINT64)2 <<((srcMSB) -(srcLSB))) -1)))
#if 0
static __inline__ int __test_and_set_bit(DWORD nr, volatile unsigned int * addr)
{
    int oldbit;
#if defined(__i386__) || defined(__x86_64__)
    __asm__ __volatile__(
        "btsl %2,%1\n\tsbbl %0,%0"
        :"=r" (oldbit),"+m" (*(addr))
        :"Ir" (nr));
#else
    if( (*addr) & (1 <<(nr&31)))
    {
        (*addr) &= ~(1 <<(nr&31));
        oldbit = 1;
    }
    else
    {
        oldbit = 0;
    }

#endif
    return oldbit;
}
#endif

static __inline__ unsigned char _BitScanForward(volatile unsigned int *Index, DWORD Mask)
{
    unsigned int i = 0;
    if(Mask==0)
    {
        return 0;
    }
#if defined(__i386__) || defined(__x86_64__)
    __asm__ __volatile__( 
      "bsfl %1, %0"
       :"=r"(Mask)     
       :"r" (Mask)  
       );
#else
    if(Mask != 0)
    {
    while( (Mask&0x1) == 0)
    {
        Mask >>= 1;
        i++;
    }
    }
    Mask = i;
#endif  
    *Index = Mask;
    return 1;
}

static __inline__ unsigned char _bittestandreset(volatile unsigned int *a, LONG b)
{
    register unsigned char ret;
    ret = (((*a) >> b) & 1);
#if defined(__i386__) || defined(__x86_64__)
    __asm__ __volatile__(
      "btrl %1,%0"
      :"=m"(*(a))
      :"r"(b)
      );
#else
    if( (*a) & (1<<b))
    {
    (*a) &= ~(1<<b);
    }
#endif
    return ret;
}

static __inline__ unsigned char _bittestandset(LONG *a, DWORD b)
{
 
    return (unsigned char)__test_and_set_bit(b, (volatile unsigned int *)a);
}

static DWORD __inline SEND_SKIP_CMD(DWORD SkipCount)
{
    Bci_Opcodes_cmd SkipCmd = { {0} };

    SkipCmd.cmd_Skip.Dwc          = SkipCount-1;
    SkipCmd.cmd_Skip.Major_Opcode = BCI_OPCODE_Skip;;

    return SkipCmd.uint;
}

// Immediate Mode: The register data is followed by the command, usually in the DMA buffer. This 
// is used to set those frequently changed simple short states
// No maskable SetRegister command
static DWORD __inline__ SET_REGISTER_ELT_FD(DWORD Block, DWORD Offset, DWORD Dwf)
{
    Bci_Opcodes_cmd SRCmd={{0}};

    SRCmd.cmd_Set_Register.Major_Opcode = BCI_OPCODE_Set_Register;
    SRCmd.cmd_Set_Register.Block_Id     = Block;
    SRCmd.cmd_Set_Register.Start_Offset = Offset;
    SRCmd.cmd_Set_Register.Dwc          = Dwf;

    return SRCmd.uint;
}

// No maskable SetRegister command
static DWORD __inline SET_REGISTER_ELT_SHORT(DWORD Block, 
                                      DWORD Offset, 
                                      DWORD NibOffset,
                                      DWORD RegValue)
{
    Bci_Opcodes_cmd SRCmd={{0}};

    SRCmd.cmd_Set_Register_Short.Major_Opcode   = BCI_OPCODE_Set_Register_Short;
    SRCmd.cmd_Set_Register_Short.Block_Id       = Block;
    SRCmd.cmd_Set_Register_Short.Start_Offset   = Offset;
    SRCmd.cmd_Set_Register_Short.Nibble_Offset  = NibOffset;
    SRCmd.cmd_Set_Register_Short.Set_Whole_Byte = 
                                SET_REGISTER_SHORT_SET_WHOLE_BYTE_SET_WHOLE_BYTE;
    SRCmd.cmd_Set_Register_Short.Register_Value = RegValue;

    return SRCmd.uint;
}

// Maskable SetRegister command
static DWORD __inline SET_REGISTER_ELT_SHORT_MASK(DWORD Block, 
                                           DWORD Offset, 
                                           DWORD NibOffset,
                                           DWORD NibMask,
                                           DWORD RegValue)
{
    Bci_Opcodes_cmd SRCmd={{0}};

    SRCmd.cmd_Set_Register_Short.Major_Opcode   = BCI_OPCODE_Set_Register_Short;
    SRCmd.cmd_Set_Register_Short.Block_Id       = Block;
    SRCmd.cmd_Set_Register_Short.Start_Offset   = Offset;
    SRCmd.cmd_Set_Register_Short.Nibble_Offset  = NibOffset;
    SRCmd.cmd_Set_Register_Short.Set_Whole_Byte = 
                                SET_REGISTER_SHORT_SET_WHOLE_BYTE_SET_NIBBLE_MASK;
    SRCmd.cmd_Set_Register_Short.Nibble_Mask    = NibMask;
    SRCmd.cmd_Set_Register_Short.Register_Value = RegValue;

    return SRCmd.uint;
}

#define SR_SKIP_CMDSIZE_CHECK   0xFFFFFFFF
// Generate command buffer: 
// Input 32bit mask,32bit regvalue and CmdSize, using SET_REGISTER_ELT_SHORT_MASK() and 
// SET_REGISTER_ELT_SHORT() to generate command buffer, 
// 1, CmdSize == SR_SKIP_CMDSIZE_CHECK, function return CmdSize, caller adjust its cmd buf.
// 2, CmdSize != SR_SKIP_CMDSIZE_CHECK, caller give pre-calculate CmdSize, if it mismatch 
//    with actual CmdSize,assert.This is useful for cache command in state object.
static DWORD __inline SET_REGISTER_ELT_MASK(DWORD Block,
                                     DWORD Offset,
                                     DWORD RegValue,
                                     DWORD Mask,
                                     DWORD CmdSize,
                                     DWORD* pCmd)
{
    DWORD   All1Per4 = 0, ValidPer4 = 0, Value, i;
    DWORD*  pCmd0;

    pCmd0 = pCmd;

    for (i=0; i < 8 ; i++)
    {
        Value = BFGET(Mask, i*4+3, i*4);

        if (Value == 0xf)
        {
            _bittestandset((LONG*)&All1Per4, i);
        }
        else if (Value)
        {
            _bittestandset((LONG*)&ValidPer4, i);
        }
    }

    while (_BitScanForward(&i, All1Per4))
    {
        _bittestandreset(&All1Per4, i);
        if (_bittestandreset(&All1Per4, i+1))
        {
            *pCmd++ = SET_REGISTER_ELT_SHORT(Block, Offset, i,
                                             BFGET(RegValue, i*4+7, i*4));
        }
        else
        {
            *pCmd++ = SET_REGISTER_ELT_SHORT_MASK(Block, Offset, i, 0xf, 
                                                  BFGET(RegValue, i*4+3, i*4));
        }

    }

    while (_BitScanForward(&i, ValidPer4))
    {
        _bittestandreset(&ValidPer4, i);
        *pCmd++ = SET_REGISTER_ELT_SHORT_MASK(Block, Offset, i, 
                                              BFGET(Mask, i*4+3, i*4), 
                                              BFGET(RegValue, i*4+3, i*4));
    }

    return (pCmd - pCmd0);
}

// The register settings are in a memory where the second DW points to can be used to restore 
// saved context.
static DWORD __inline SET_REGISTER_ELT_ADDR(DWORD Block, DWORD Offset, DWORD Dwf)
{
    Bci_Opcodes_cmd SRCmd={{0}};

    SRCmd.cmd_Set_Register_Addr.Major_Opcode = BCI_OPCODE_Set_Register_Addr;
    SRCmd.cmd_Set_Register_Addr.Block_Id     = Block;
    SRCmd.cmd_Set_Register_Addr.Start_Offset = Offset;
    SRCmd.cmd_Set_Register_Addr.Dwc          = Dwf;

    return SRCmd.uint;
}

static DWORD __inline SEND_FFCACHE_INVALIDATE_COMMAND_ELT(void)
{
    Bci_Opcodes_cmd FfInv={{0}};

    FfInv.cmd_Block_Command_Cch_Flsh.Major_Opcode = BCI_OPCODE_Block_Command_Cch_Flsh;
    FfInv.cmd_Block_Command_Cch_Flsh.Block_Id     = FIXED_FUNCTION_BLOCK;
    FfInv.cmd_Block_Command_Cch_Flsh.Target       = BLOCK_COMMAND_CCH_FLSH_TARGET_ALL;
    FfInv.cmd_Block_Command_Cch_Flsh.Type         = BLOCK_COMMAND_TEMPLATE_TYPE_INVALIDATE_CACHE;
    FfInv.cmd_Block_Command_Cch_Flsh.Dwc          = 0;

    return FfInv.uint;
}

static DWORD __inline SEND_DCACHE_INVALIDATE_COMMAND_ELT(void)
{
    Bci_Opcodes_cmd RtInv={{0}};

    RtInv.cmd_Block_Command_Cch_Flsh.Major_Opcode = BCI_OPCODE_Block_Command_Cch_Flsh;
    RtInv.cmd_Block_Command_Cch_Flsh.Block_Id     = FIXED_FUNCTION_BLOCK;
    RtInv.cmd_Block_Command_Cch_Flsh.Target       = BLOCK_COMMAND_CCH_FLSH_TARGET_D;
    RtInv.cmd_Block_Command_Cch_Flsh.Type         = BLOCK_COMMAND_TEMPLATE_TYPE_INVALIDATE_CACHE;
    RtInv.cmd_Block_Command_Cch_Flsh.Dwc          = 0;

    return RtInv.uint;
}

static DWORD __inline SEND_DEPTH_INVALIDATE_COMMAND_ELT(void)
{
    Bci_Opcodes_cmd ZInv={{0}};

    ZInv.cmd_Block_Command_Cch_Flsh.Major_Opcode = BCI_OPCODE_Block_Command_Cch_Flsh;
    ZInv.cmd_Block_Command_Cch_Flsh.Block_Id     = FIXED_FUNCTION_BLOCK;
    ZInv.cmd_Block_Command_Cch_Flsh.Target       = BLOCK_COMMAND_CCH_FLSH_TARGET_Z;
    ZInv.cmd_Block_Command_Cch_Flsh.Type         = BLOCK_COMMAND_TEMPLATE_TYPE_INVALIDATE_CACHE;
    ZInv.cmd_Block_Command_Cch_Flsh.Dwc          = 0;

    return ZInv.uint;
}

static DWORD __inline SEND_STENCIL_INVALIDATE_COMMAND_ELT(void)
{
    Bci_Opcodes_cmd SInv={{0}};

    SInv.cmd_Block_Command_Cch_Flsh.Major_Opcode = BCI_OPCODE_Block_Command_Cch_Flsh;
    SInv.cmd_Block_Command_Cch_Flsh.Block_Id     = FIXED_FUNCTION_BLOCK;
    SInv.cmd_Block_Command_Cch_Flsh.Target       = BLOCK_COMMAND_CCH_FLSH_TARGET_S;
    SInv.cmd_Block_Command_Cch_Flsh.Type         = BLOCK_COMMAND_TEMPLATE_TYPE_INVALIDATE_CACHE;
    SInv.cmd_Block_Command_Cch_Flsh.Dwc          = 0;

    return SInv.uint;
}

static DWORD __inline SEND_FFCACHE_FLUSH_COMMAND_ELT(void)
{
    Bci_Opcodes_cmd FfFlushCmd={{0}};

    FfFlushCmd.cmd_Block_Command_Cch_Flsh.Major_Opcode = BCI_OPCODE_Block_Command_Cch_Flsh;
    FfFlushCmd.cmd_Block_Command_Cch_Flsh.Block_Id     = FIXED_FUNCTION_BLOCK;
    FfFlushCmd.cmd_Block_Command_Cch_Flsh.Target       = BLOCK_COMMAND_CCH_FLSH_TARGET_ALL;
    FfFlushCmd.cmd_Block_Command_Cch_Flsh.Type         = BLOCK_COMMAND_TEMPLATE_TYPE_FLUSH_CACHE;
    FfFlushCmd.cmd_Block_Command_Cch_Flsh.Dwc          = 0;

    return FfFlushCmd.uint;
};

static DWORD __inline SEND_DCACHE_FLUSH_COMMAND_ELT(void)
{
    Bci_Opcodes_cmd RtFlushCmd={{0}};

    RtFlushCmd.cmd_Block_Command_Cch_Flsh.Major_Opcode = BCI_OPCODE_Block_Command_Cch_Flsh;
    RtFlushCmd.cmd_Block_Command_Cch_Flsh.Block_Id     = FIXED_FUNCTION_BLOCK;
    RtFlushCmd.cmd_Block_Command_Cch_Flsh.Target       = BLOCK_COMMAND_CCH_FLSH_TARGET_D;
    RtFlushCmd.cmd_Block_Command_Cch_Flsh.Type         = BLOCK_COMMAND_TEMPLATE_TYPE_FLUSH_CACHE;
    RtFlushCmd.cmd_Block_Command_Cch_Flsh.Dwc          = 0;

    return RtFlushCmd.uint;
};

static DWORD __inline SEND_DEPTH_FLUSH_COMMAND_ELT(void)
{
    Bci_Opcodes_cmd ZFlushCmd={{0}};

    ZFlushCmd.cmd_Block_Command_Cch_Flsh.Major_Opcode = BCI_OPCODE_Block_Command_Cch_Flsh;
    ZFlushCmd.cmd_Block_Command_Cch_Flsh.Block_Id     = FIXED_FUNCTION_BLOCK;
    ZFlushCmd.cmd_Block_Command_Cch_Flsh.Target       = BLOCK_COMMAND_CCH_FLSH_TARGET_Z;
    ZFlushCmd.cmd_Block_Command_Cch_Flsh.Type         = BLOCK_COMMAND_TEMPLATE_TYPE_FLUSH_CACHE;
    ZFlushCmd.cmd_Block_Command_Cch_Flsh.Dwc          = 0;

    return ZFlushCmd.uint;
}

static DWORD __inline SEND_STENCIL_FLUSH_COMMAND_ELT(void)
{
    Bci_Opcodes_cmd SFlushCmd={{0}};

    SFlushCmd.cmd_Block_Command_Cch_Flsh.Major_Opcode = BCI_OPCODE_Block_Command_Cch_Flsh;
    SFlushCmd.cmd_Block_Command_Cch_Flsh.Block_Id     = FIXED_FUNCTION_BLOCK;
    SFlushCmd.cmd_Block_Command_Cch_Flsh.Target       = BLOCK_COMMAND_CCH_FLSH_TARGET_S;
    SFlushCmd.cmd_Block_Command_Cch_Flsh.Type         = BLOCK_COMMAND_TEMPLATE_TYPE_FLUSH_CACHE;
    SFlushCmd.cmd_Block_Command_Cch_Flsh.Dwc          = 0;

    return SFlushCmd.uint;
}

static DWORD __inline SEND_RT_CLEAR_COMMAND_ELT(BOOL bTiled)
{
    Bci_Opcodes_cmd     Rtc={{0}};

    Rtc.cmd_Block_Command_Setup_Sg.Major_Opcode = BCI_OPCODE_Block_Command_Setup_Sg;
    Rtc.cmd_Block_Command_Setup_Sg.Block_Id     = FIXED_FUNCTION_BLOCK;
    Rtc.cmd_Block_Command_Setup_Sg.Area_Target  = BLOCK_COMMAND_SETUP_SG_AREA_TARGET_D; 
    Rtc.cmd_Block_Command_Setup_Sg.Action_Type  = 
        (bTiled) ? BLOCK_COMMAND_SETUP_SG_ACTION_TYPE_FAST_CLEAR_TILE
        : BLOCK_COMMAND_SETUP_SG_ACTION_TYPE_FAST_CLEAR_LINEAR;
    Rtc.cmd_Block_Command_Setup_Sg.Setup_Type   = BLOCK_COMMAND_SETUP_SG_SETUP_TYPE_AREA_GEN;
    Rtc.cmd_Block_Command_Setup_Sg.Type         = BLOCK_COMMAND_TEMPLATE_TYPE_SETUP_SG;
    Rtc.cmd_Block_Command_Setup_Sg.Dwc          = 4;

    return (DWORD)Rtc.uint;
}

static DWORD __inline SEND_DEPTH_CLEAR_COMMAND_ELT(UINT ZFormat)
{
    Bci_Opcodes_cmd     Zc={{0}};

    Zc.cmd_Block_Command_Setup_Sg.Major_Opcode = BCI_OPCODE_Block_Command_Setup_Sg;
    Zc.cmd_Block_Command_Setup_Sg.Block_Id     = FIXED_FUNCTION_BLOCK;
    Zc.cmd_Block_Command_Setup_Sg.Area_Target  = (ZFormat == ZV_FMT_FORMAT_Z16) ? BLOCK_COMMAND_SETUP_SG_AREA_TARGET_Z16 : BLOCK_COMMAND_SETUP_SG_AREA_TARGET_Z24;
    Zc.cmd_Block_Command_Setup_Sg.Action_Type  = BLOCK_COMMAND_SETUP_SG_ACTION_TYPE_FAST_CLEAR_TILE;
    Zc.cmd_Block_Command_Setup_Sg.Setup_Type   = BLOCK_COMMAND_SETUP_SG_SETUP_TYPE_AREA_GEN;
    Zc.cmd_Block_Command_Setup_Sg.Type         = BLOCK_COMMAND_TEMPLATE_TYPE_SETUP_SG;
    Zc.cmd_Block_Command_Setup_Sg.Dwc          = 4;

    return (DWORD)Zc.uint;
}

static DWORD __inline SEND_STENCIL_CLEAR_COMMAND_ELT(void)
{
    Bci_Opcodes_cmd     Sc={{0}};

    Sc.cmd_Block_Command_Setup_Sg.Major_Opcode = BCI_OPCODE_Block_Command_Setup_Sg;
    Sc.cmd_Block_Command_Setup_Sg.Block_Id     = FIXED_FUNCTION_BLOCK;
    Sc.cmd_Block_Command_Setup_Sg.Area_Target  = BLOCK_COMMAND_SETUP_SG_AREA_TARGET_S;
    Sc.cmd_Block_Command_Setup_Sg.Action_Type  = BLOCK_COMMAND_SETUP_SG_ACTION_TYPE_FAST_CLEAR_TILE;
    Sc.cmd_Block_Command_Setup_Sg.Setup_Type   = BLOCK_COMMAND_SETUP_SG_SETUP_TYPE_AREA_GEN;
    Sc.cmd_Block_Command_Setup_Sg.Type         = BLOCK_COMMAND_TEMPLATE_TYPE_SETUP_SG;
    Sc.cmd_Block_Command_Setup_Sg.Dwc          = 4;

    return (DWORD)Sc.uint;
}

static DWORD __inline SEND_2DCOPY_COMMAND_ELT(void)
{
    Bci_Opcodes_cmd     Copy2d={{0}};

    Copy2d.cmd_Block_Command_Setup_Sg.Major_Opcode = BCI_OPCODE_Block_Command_Setup_Sg;
    Copy2d.cmd_Block_Command_Setup_Sg.Block_Id     = FIXED_FUNCTION_BLOCK;
    Copy2d.cmd_Block_Command_Setup_Sg.Action_Type  = BLOCK_COMMAND_SETUP_SG_ACTION_TYPE_BIT_BLT;
    Copy2d.cmd_Block_Command_Setup_Sg.Setup_Type   = BLOCK_COMMAND_SETUP_SG_SETUP_TYPE_AREA_GEN;
    Copy2d.cmd_Block_Command_Setup_Sg.Area_Target  = BLOCK_COMMAND_SETUP_SG_AREA_TARGET_D;
    Copy2d.cmd_Block_Command_Setup_Sg.Type         = BLOCK_COMMAND_TEMPLATE_TYPE_SETUP_SG;
    Copy2d.cmd_Block_Command_Setup_Sg.Dwc          = 3;

    return (DWORD)Copy2d.uint;
}

static DWORD __inline SEND_GRADIENTFILL_COMMAND_ELT(void)
{
    Bci_Opcodes_cmd     GrFill={{0}};

    GrFill.cmd_Block_Command_Setup_Sg.Major_Opcode = BCI_OPCODE_Block_Command_Setup_Sg;
    GrFill.cmd_Block_Command_Setup_Sg.Block_Id     = FIXED_FUNCTION_BLOCK;
    GrFill.cmd_Block_Command_Setup_Sg.Action_Type  = BLOCK_COMMAND_SETUP_SG_ACTION_TYPE_GRADIENT_FILL;
    GrFill.cmd_Block_Command_Setup_Sg.Setup_Type   = BLOCK_COMMAND_SETUP_SG_SETUP_TYPE_AREA_GEN;
    GrFill.cmd_Block_Command_Setup_Sg.Area_Target  = BLOCK_COMMAND_SETUP_SG_AREA_TARGET_D;
    GrFill.cmd_Block_Command_Setup_Sg.Type         = BLOCK_COMMAND_TEMPLATE_TYPE_SETUP_SG;
    GrFill.cmd_Block_Command_Setup_Sg.Dwc          = 14;

    return (DWORD)GrFill.uint;
}

static DWORD __inline SEND_2D_ROTATION_ELT(void)
{
    Bci_Opcodes_cmd   Rotation={{0}};

    Rotation.cmd_Block_Command_Setup_Sg.Major_Opcode = BCI_OPCODE_Block_Command_Setup_Sg;
    Rotation.cmd_Block_Command_Setup_Sg.Block_Id     = FIXED_FUNCTION_BLOCK;
    Rotation.cmd_Block_Command_Setup_Sg.Action_Type  = BLOCK_COMMAND_SETUP_SG_ACTION_TYPE_ROTATION;
    Rotation.cmd_Block_Command_Setup_Sg.Setup_Type   = BLOCK_COMMAND_SETUP_SG_SETUP_TYPE_AREA_GEN;
    Rotation.cmd_Block_Command_Setup_Sg.Area_Target  = BLOCK_COMMAND_SETUP_SG_AREA_TARGET_D;
    Rotation.cmd_Block_Command_Setup_Sg.Type         = BLOCK_COMMAND_TEMPLATE_TYPE_SETUP_SG;
    Rotation.cmd_Block_Command_Setup_Sg.Dwc          = 1;
    
    return (DWORD)Rotation.uint;
}

static DWORD __inline SEND_STRETCHBLT_COMMAND_ELT(void)
{
    Bci_Opcodes_cmd   StretchBlt={{0}};

    StretchBlt.cmd_Block_Command_Setup_Sg.Major_Opcode = BCI_OPCODE_Block_Command_Setup_Sg;
    StretchBlt.cmd_Block_Command_Setup_Sg.Block_Id     = FIXED_FUNCTION_BLOCK;
    StretchBlt.cmd_Block_Command_Setup_Sg.Action_Type  = BLOCK_COMMAND_SETUP_SG_ACTION_TYPE_SCALING;
    StretchBlt.cmd_Block_Command_Setup_Sg.Setup_Type   = BLOCK_COMMAND_SETUP_SG_SETUP_TYPE_AREA_GEN;
    StretchBlt.cmd_Block_Command_Setup_Sg.Area_Target  = BLOCK_COMMAND_SETUP_SG_AREA_TARGET_D;
    StretchBlt.cmd_Block_Command_Setup_Sg.Type         = BLOCK_COMMAND_TEMPLATE_TYPE_SETUP_SG;
    StretchBlt.cmd_Block_Command_Setup_Sg.Dwc          = 9;

    return (DWORD)StretchBlt.uint;
}

static DWORD __inline SEND_COPY_IMM_COMMAND_ELT(DWORD Dwf)
{
    Bci_Opcodes_cmd     CopyImm={{0}};

    CopyImm.cmd_Block_Command_Img_Trn.Major_Opcode = BCI_OPCODE_Block_Command_Img_Trn;
    CopyImm.cmd_Block_Command_Img_Trn.Block_Id     = FIXED_FUNCTION_BLOCK; 
    CopyImm.cmd_Block_Command_Img_Trn.Type         = BLOCK_COMMAND_TEMPLATE_TYPE_IMAGE_TRANSFER; 
    CopyImm.cmd_Block_Command_Img_Trn.Dwc          = Dwf;

    return (DWORD)CopyImm.uint;
}

static DWORD __inline SEND_DP_LINE_COMMAND_ELT(BOOL ColorIncluded)
{
    Bci_Opcodes_cmd     DPLine={{0}};

    DPLine.cmd_Block_Command_Tas.Major_Opcode           = BCI_OPCODE_Block_Command_Tas;
    DPLine.cmd_Block_Command_Tas.Type                   = ColorIncluded;
    DPLine.cmd_Block_Command_Tas.Command_Specific_Field = 
                                         BLOCK_COMMAND_TAS_COMMAND_SPECIFIC_FIELD_DP_LINE;
    DPLine.cmd_Block_Command_Tas.Block_Id               = TAS_BLOCK;
    DPLine.cmd_Block_Command_Tas.Dwc                    = ColorIncluded ? 3 : 2;

    return (DWORD)DPLine.uint;
}

static DWORD __inline SEND_DRAWAUTO_COMMAND_ELT(DWORD P_Type)
{
    Bci_Opcodes_cmd     DIPCmd={{0}};

    DIPCmd.cmd_Dip.Major_Opcode    = BCI_OPCODE_Dip;
    DIPCmd.cmd_Dip.P_Type          = P_Type;
    DIPCmd.cmd_Dip.Draw_Auto_En    = DIP_DRAW_AUTO_EN_ENABLED;

    return (DWORD)DIPCmd.uint;
}

// Internal Write Fence Command
static DWORD __inline SEND_INTERNAL_WRITEFENCE_ELT(DWORD Block, DWORD Slot, DWORD Value)
{
    Bci_Opcodes_cmd     FenceCmd={{0}};

    FenceCmd.cmd_Internal_Fence.Major_Opcode        = BCI_OPCODE_Internal_Fence;
    FenceCmd.cmd_Internal_Fence.Block_Id            = Block;
    FenceCmd.cmd_Internal_Fence.Slot_Id             = Slot;
    FenceCmd.cmd_Internal_Fence.Update_Value        = Value;
    FenceCmd.cmd_Internal_Fence.Fence_Update_Mode   = INTERNAL_FENCE_FENCE_UPDATE_MODE_COPY;
    FenceCmd.cmd_Internal_Fence.Fence_Update_Timing = 
                    INTERNAL_FENCE_FENCE_UPDATE_TIMING_UPDATE_AFTER_WRITE;

    return (DWORD)FenceCmd.uint;
}

static DWORD __inline UPDATE_INTERNAL_WRITEFENCE_ELT(DWORD Block, DWORD Slot, DWORD Value)
{
    Bci_Opcodes_cmd     FenceCmd={{0}};

    FenceCmd.cmd_Internal_Fence.Major_Opcode        = BCI_OPCODE_Internal_Fence;
    FenceCmd.cmd_Internal_Fence.Block_Id            = Block;
    FenceCmd.cmd_Internal_Fence.Slot_Id             = Slot;
    FenceCmd.cmd_Internal_Fence.Update_Value        = Value;
    FenceCmd.cmd_Internal_Fence.Fence_Update_Mode   = INTERNAL_FENCE_FENCE_UPDATE_MODE_OR;
    FenceCmd.cmd_Internal_Fence.Fence_Update_Timing = 
                    INTERNAL_FENCE_FENCE_UPDATE_TIMING_UPDATE_AFTER_WRITE;

    return (DWORD)FenceCmd.uint;
}

// Internal Read Fence Command
static DWORD __inline SEND_INTERNAL_READFENCE_ELT(DWORD Block, DWORD Slot, DWORD Value)
{
    Bci_Opcodes_cmd     FenceCmd={{0}};

    FenceCmd.cmd_Internal_Fence.Major_Opcode        = BCI_OPCODE_Internal_Fence;
    FenceCmd.cmd_Internal_Fence.Block_Id            = Block;
    FenceCmd.cmd_Internal_Fence.Slot_Id             = Slot;
    FenceCmd.cmd_Internal_Fence.Update_Value        = Value;
    FenceCmd.cmd_Internal_Fence.Fence_Update_Mode   = INTERNAL_FENCE_FENCE_UPDATE_MODE_COPY;
    FenceCmd.cmd_Internal_Fence.Fence_Update_Timing = 
                    INTERNAL_FENCE_FENCE_UPDATE_TIMING_UPDATE_AFTER_READ;

    return (DWORD)FenceCmd.uint;
}
static DWORD __inline UPDATE_INTERNAL_READFENCE_ELT(DWORD Block, DWORD Slot, DWORD Value)
{
    Bci_Opcodes_cmd     FenceCmd={{0}};

    FenceCmd.cmd_Internal_Fence.Major_Opcode        = BCI_OPCODE_Internal_Fence;
    FenceCmd.cmd_Internal_Fence.Block_Id            = Block;
    FenceCmd.cmd_Internal_Fence.Slot_Id             = Slot;
    FenceCmd.cmd_Internal_Fence.Update_Value        = Value;
    FenceCmd.cmd_Internal_Fence.Fence_Update_Mode   = INTERNAL_FENCE_FENCE_UPDATE_MODE_OR;
    FenceCmd.cmd_Internal_Fence.Fence_Update_Timing = 
                    INTERNAL_FENCE_FENCE_UPDATE_TIMING_UPDATE_AFTER_READ;

    return (DWORD)FenceCmd.uint;
}

// Internal Wait Command
static DWORD __inline SEND_INTERNAL_WAIT_ELT(DWORD Slot, DWORD Value, DWORD WaitMethod)
{
    Bci_Opcodes_cmd     WaitCmd={{0}};

    WaitCmd.cmd_Wait.Major_Opcode = BCI_OPCODE_Wait;
    WaitCmd.cmd_Wait.Slot_Id      = Slot;
    WaitCmd.cmd_Wait.Ref_Value    = Value;
    WaitCmd.cmd_Wait.Mode         = WAIT_MODE_INTERNAL_WAIT;
    WaitCmd.cmd_Wait.Method       = WaitMethod;

    return (DWORD)WaitCmd.uint;
}

// External wait command
static DWORD __inline SEND_EXTERNAL_WAIT_ELT(DWORD Slot, DWORD Value, DWORD WaitMethod)
{
    Bci_Opcodes_cmd     WaitCmd={{0}};

    WaitCmd.cmd_Wait.Major_Opcode = BCI_OPCODE_Wait;
    WaitCmd.cmd_Wait.Slot_Id      = Slot;
    WaitCmd.cmd_Wait.Ref_Value    = Value;
    WaitCmd.cmd_Wait.Mode         = WAIT_MODE_EXTERNAL_WAIT;
    WaitCmd.cmd_Wait.Method       = WaitMethod;

    return (DWORD)WaitCmd.uint;
}

// Disable shadow buffer in FF
static DWORD __inline DISABLE_FF_SHADOW(void)
{
    Bci_Opcodes_cmd     DisableCmd={{0}};

    DisableCmd.cmd_Block_Command_Tas.Major_Opcode           = BCI_OPCODE_Block_Command_Tas;
    DisableCmd.cmd_Block_Command_Tas.Block_Id               = TAS_BLOCK;
    DisableCmd.cmd_Block_Command_Tas.Command_Specific_Field = 
            BLOCK_COMMAND_TAS_COMMAND_SPECIFIC_FIELD_DISABLE_SBUF;

    return (DWORD)DisableCmd.uint;
}

// Enable shadow buffer in FF and restore reg from shadow buffer
static DWORD __inline RESTORE_AND_ENABLE_FF_SHADOW(void)
{
    Bci_Opcodes_cmd     EnableCmd={{0}};

    EnableCmd.cmd_Block_Command_Tas.Major_Opcode           = BCI_OPCODE_Block_Command_Tas;
    EnableCmd.cmd_Block_Command_Tas.Block_Id               = TAS_BLOCK;
    EnableCmd.cmd_Block_Command_Tas.Command_Specific_Field = 
        BLOCK_COMMAND_TAS_COMMAND_SPECIFIC_FIELD_RESTORE_3D_REG_FROM_SBUF;

    return (DWORD)EnableCmd.uint;
}

// Dump context of block from offset  
// Dump size is DumpSize(of DWORD)
// context include:
// BCI
// EU VS/GP
// EU PS
// Setup * SG => RU?
// TU ZU FFC
// WBU
// Shared Memory => blockid? offset?
// can dump consecutive blocks
static DWORD __inline SEND_CONTEXT_DUMP_ELT(DWORD Block, DWORD Offset, DWORD DumpSize)
{
    Bci_Opcodes_cmd     DumpCmd={{0}};

    DumpCmd.cmd_Query_Dump.Major_Opcode     = BCI_OPCODE_Query_Dump;
    DumpCmd.cmd_Query_Dump.Block_Id         = Block;
    DumpCmd.cmd_Query_Dump.Reg_Offset       = Offset;
    DumpCmd.cmd_Query_Dump.Option           = QUERY_DUMP_OPTION_CONTEXT_QUERY;
    DumpCmd.cmd_Query_Dump.Dwc_Or_Fence_Tag = DumpSize;

    return (DWORD)DumpCmd.uint;
}

// dump status to address where the second DW points,
// dump size is DumpSize(of DWORD)
// status includes
// perfomanece counter // ILA
// timestamp
// engine busy status
// fence counter
static DWORD __inline SEND_QUERY_STATUS_ELT(DWORD Offset, DWORD DumpSize)
{
    Bci_Opcodes_cmd     DumpCmd={{0}};

    DumpCmd.cmd_Query_Dump.Major_Opcode     = BCI_OPCODE_Query_Dump;
    DumpCmd.cmd_Query_Dump.Block_Id         = QUERY_DUMP_BLOCK_ID_BCI;
    DumpCmd.cmd_Query_Dump.Reg_Offset       = Offset;
    DumpCmd.cmd_Query_Dump.Option           = QUERY_DUMP_OPTION_STATUS_QUERY;
    DumpCmd.cmd_Query_Dump.Dwc_Or_Fence_Tag = DumpSize;

    return (DWORD)DumpCmd.uint;
}

// write externel fence to the address the second DW points
static DWORD __inline SEND_EXTERNAL_FENCE_ELT(BOOL bNeedIrq)
{
    Bci_Opcodes_cmd     DumpCmd={{0}};

    DumpCmd.cmd_Block_Command_Mxu.Major_Opcode = BCI_OPCODE_Block_Command_Mxu;
    DumpCmd.cmd_Block_Command_Mxu.Block_Id	   = QUERY_DUMP_BLOCK_ID_MXU;
    DumpCmd.cmd_Block_Command_Mxu.Irq          = bNeedIrq;
    DumpCmd.cmd_Block_Command_Mxu.Dwc		   = 3;
    DumpCmd.cmd_Block_Command_Mxu.External_Fence = BLOCK_COMMAND_MXU_EXTERNAL_FENCE_EXTERNAL_FENCE;    

    return (DWORD)DumpCmd.uint;
}

// Gp command for executing a CS shader
static DWORD __inline SEND_GP_COMMAND_ELT(void)
{
    Bci_Opcodes_cmd     GpCmd={{0}};

    GpCmd.cmd_Gp.Major_Opcode               = BCI_OPCODE_Gp;
    GpCmd.cmd_Gp.Block_Id                   = EU_VS_BLOCK;
    GpCmd.cmd_Gp.Dwc                        = 6;

    return (DWORD)GpCmd.uint;
}

// Drain VS
static DWORD __inline SENT_DRAIN_VS_COMMAND_ELT(void)
{
    Bci_Opcodes_cmd     BlockCommandVcs={{0}};

    BlockCommandVcs.cmd_Block_Command_Vcs.Major_Opcode  = BCI_OPCODE_Block_Command_Vcs;
    BlockCommandVcs.cmd_Block_Command_Vcs.Block_Id      = EU_VS_BLOCK;
    BlockCommandVcs.cmd_Block_Command_Vcs.Type          = BLOCK_COMMAND_VCS_TYPE_DRAIN_VS;
    BlockCommandVcs.cmd_Block_Command_Vcs.Dwc           = 0;

    return (DWORD)BlockCommandVcs.uint;
}

// Drain CS
static DWORD __inline SEND_DRAIN_CS_COMMAND_ELT(void)
{
    Bci_Opcodes_cmd     BlockCommandVcs={{0}};

    BlockCommandVcs.cmd_Block_Command_Vcs.Major_Opcode  = BCI_OPCODE_Block_Command_Vcs;
    BlockCommandVcs.cmd_Block_Command_Vcs.Block_Id      = EU_VS_BLOCK;
    BlockCommandVcs.cmd_Block_Command_Vcs.Type          = BLOCK_COMMAND_VCS_TYPE_DRAIN_CS;
    BlockCommandVcs.cmd_Block_Command_Vcs.Dwc           = 0;

    return (DWORD)BlockCommandVcs.uint;
}

// Drain PS
static DWORD __inline SEND_DRAIN_PS_COMMAND_ELT(void)
{
    Bci_Opcodes_cmd     BlockCommandPs={{0}};

    BlockCommandPs.cmd_Block_Command_Ps.Major_Opcode    = BCI_OPCODE_Block_Command_Ps;
    BlockCommandPs.cmd_Block_Command_Ps.Block_Id        = EU_PS_BLOCK;
    BlockCommandPs.cmd_Block_Command_Ps.Type            = BLOCK_COMMAND_PS_TYPE_DRAIN_PS;
    BlockCommandPs.cmd_Block_Command_Ps.Dwc             = 0;

    return (DWORD)BlockCommandPs.uint;
}

// Invalidate VS/CS L1 instruction cache
static DWORD __inline SEND_VCS_L1I_INVALIDATE_COMMAND_ELT(void)
{
    Bci_Opcodes_cmd     BlockCommandVcs={{0}};

    BlockCommandVcs.cmd_Block_Command_Vcs.Major_Opcode  = BCI_OPCODE_Block_Command_Vcs;
    BlockCommandVcs.cmd_Block_Command_Vcs.Block_Id      = EU_VS_BLOCK;
    BlockCommandVcs.cmd_Block_Command_Vcs.Type          = BLOCK_COMMAND_VCS_TYPE_INVALIDATE_L1I;
    BlockCommandVcs.cmd_Block_Command_Vcs.Dwc           = 2;

    return (DWORD)BlockCommandVcs.uint;
}

// Invalidate PS L1 instruction cache
static DWORD __inline SEND_PS_L1I_INVALIDATE_COMMAND_ELT(void)
{
    Bci_Opcodes_cmd     BlockCommandPs={{0}};

    BlockCommandPs.cmd_Block_Command_Ps.Major_Opcode    = BCI_OPCODE_Block_Command_Ps;
    BlockCommandPs.cmd_Block_Command_Ps.Block_Id        = EU_PS_BLOCK;
    BlockCommandPs.cmd_Block_Command_Ps.Type            = BLOCK_COMMAND_PS_TYPE_INVALIDATE_L1I;
    BlockCommandPs.cmd_Block_Command_Ps.Dwc             = 2;

    return (DWORD)BlockCommandPs.uint;
}


#endif //_REGISTERCOMMANDS_H_
