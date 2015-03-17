//    Spec Last Modified Time: 8/25/2011 6:35:15 PM
#ifndef _BCI_OPCODE_ELT_H
#define _BCI_OPCODE_ELT_H

#ifndef        COMMAND_OPCODES_BLOCKBASE_INF
    #define    COMMAND_OPCODES_BLOCKBASE_INF
    #define    BLOCK_COMMAND_OPCODES_VERSION 1
    #define    BLOCK_COMMAND_OPCODES_TIMESTAMP "8/25/2011 6:35:15 PM"
#endif

// BCI command opercode definition
#define  BCI_OPCODE_Skip                    0x0            // 0h: SKIP Command
#define  BCI_OPCODE_Dma                     0x1            // 1h: DMA Command
#define  BCI_OPCODE_Wait                    0x2            // 2h: Wait Command
#define  BCI_OPCODE_Query_Dump              0x3            // 3h: Query Dump Command
#define  BCI_OPCODE_Set_Register            0x4            // 4h: Set Register Command
#define  BCI_OPCODE_Set_Register_Short      0x5            // 5h: SetRegisterShort Command
#define  BCI_OPCODE_Set_Register_Addr       0x6            // 6h: Set Register Address Command
#define  BCI_OPCODE_Dip                     0x7            // 7h: DrawIndexedPrimitive Command
#define  BCI_OPCODE_Internal_Fence          0x8            // 8h: InternalFence Command
#define  BCI_OPCODE_Block_Command_Template  0x9            // 9h: BlockCommand Template
#define  BCI_OPCODE_Block_Command_Tas       0x9            // 9h: BlockCommand starting at TAS
#define  BCI_OPCODE_Block_Command_Setup_Sg  0x9            // 9h: BlockCommand starting at Setup/SG
#define  BCI_OPCODE_Block_Command_Img_Trn   0x9            // 9h: BlockCommand for Image Transfer, starting at ZL3
#define  BCI_OPCODE_Block_Command_Cch_Flsh  0x9            // 9h: BlockCommand for Cache Flush, starting at ZL3
#define  BCI_OPCODE_Block_Command_Vcs       0x9            // 9h: BlockCommand for VS/CS
#define  BCI_OPCODE_Block_Command_Ps        0x9            // 9h: BlockCommand for PS
#define  BCI_OPCODE_Block_Command_Mxu       0x9            // 9h: BlockCommand for MXU
#define  BCI_OPCODE_Block_Command_Csp       0x9            // 9h: BlockCommand for CSP
#define  BCI_OPCODE_Block_Command_Video     0x9            // 9h: BlockCommand for VIDEO
#define  BCI_OPCODE_Flip                    0xA            // Ah: Flip Command
#define  BCI_OPCODE_Gp                      0xB            // Bh: General Purpose Computing
#define  BCI_OPCODE_Isp                     0xC            // Ch: ISP Command
#define  BCI_OPCODE_Module_Ext_Vcp          0xD            // Dh: Module Extension VCP Command
#define  BCI_OPCODE_Aes                     0xE            // Eh: AES Command
#define  BCI_OPCODE_Video                   0xF            // Fh: Video Command

// Block constant definition
typedef enum
{
    WAIT_METHOD_BIGEQUAL                     = 0,   // 0: Big Equal
    WAIT_METHOD_EQUAL                        = 1,   // 1: Equal
} WAIT_METHOD;
typedef enum
{
    WAIT_MODE_INTERNAL_WAIT                  = 0,   // 0: Internal Wait
    WAIT_MODE_EXTERNAL_WAIT                  = 1,   // 1: External Wait
} WAIT_MODE;
typedef enum
{
    QUERY_DUMP_IRQ_NOP                       = 0,   // 00: Nop
    QUERY_DUMP_IRQ_INTERRUPT                 = 1,   // 01: interrupt when external fence done
} QUERY_DUMP_IRQ;
typedef enum
{
    QUERY_DUMP_OPTION_STATUS_QUERY           = 0,   // 0: Status query, for bci only
    QUERY_DUMP_OPTION_CONTEXT_QUERY          = 1,   // 1: Context query
} QUERY_DUMP_OPTION;
typedef enum
{
    QUERY_DUMP_BLOCK_ID_BCI                  = 0,   // 0x00: BCI
    QUERY_DUMP_BLOCK_ID_EU_VS                = 1,   // 0x01: EU_VS
    QUERY_DUMP_BLOCK_ID_TAS                  = 2,   // 0x02: TAS
    QUERY_DUMP_BLOCK_ID_FF                   = 3,   // 0x03: FF
    QUERY_DUMP_BLOCK_ID_IU                   = 4,   // 0x04: IU
    QUERY_DUMP_BLOCK_ID_EU_PS                = 5,   // 0x05: EU_PS
    QUERY_DUMP_BLOCK_ID_TU                   = 6,   // 0x06: TU
    QUERY_DUMP_BLOCK_ID_MXU                  = 7,   // 0x07: MXU
    QUERY_DUMP_BLOCK_ID_MXU1                 = 8,   // 0x08: MXU1, for range registers 0-255
    QUERY_DUMP_BLOCK_ID_MXU2                 = 9,   // 0x09: MXU2, for range registers 256-511
    QUERY_DUMP_BLOCK_ID_ISP                  = 11,  // 0xB: ISP
    QUERY_DUMP_BLOCK_ID_VIDEO                = 12,  // 0xC: VIDEO
    QUERY_DUMP_BLOCK_ID_ZL2                  = 13,  // 0xD: ZL2
    QUERY_DUMP_BLOCK_ID_WBU                  = 14,  // 0xE: WBU
    QUERY_DUMP_BLOCK_ID_ZL3                  = 15,  // 0xF: ZL3
} QUERY_DUMP_BLOCK_ID;
typedef enum
{
    SET_REGISTER_SHORT_SET_WHOLE_BYTE_SET_NIBBLE_MASK= 0,
                                                    // 0x00: set nibble with nibble mask
    SET_REGISTER_SHORT_SET_WHOLE_BYTE_SET_WHOLE_BYTE= 1,
                                                    // 0x01: set whole byte, LSB of nibble offset =0
} SET_REGISTER_SHORT_SET_WHOLE_BYTE;
typedef enum
{
    DIP_INDEX_MODE_IB_8                      = 0,   // 0x0: 8 bit
    DIP_INDEX_MODE_IB_16                     = 1,   // 0x1: 16 bit
    DIP_INDEX_MODE_IB_32                     = 2,   // 0x2: 32 bit
} DIP_INDEX_MODE;
typedef enum
{
    DIP_INSTANCE_EN_DISABLED                 = 0,   // 0: Instancing not enabled. Command does not have an
                                                    // Instance Count DWord.
    DIP_INSTANCE_EN_ENABLED                  = 1,   // 1: Instancing Enabled. 
} DIP_INSTANCE_EN;
typedef enum
{
    DIP_DRAW_AUTO_EN_DISABLED                = 0,   // 0x0: Disabled
    DIP_DRAW_AUTO_EN_ENABLED                 = 1,   // 0x1: Enabled, No index to fetch. No second DW for index
                                                    // address. 
} DIP_DRAW_AUTO_EN;
typedef enum
{
    DIP_P_TYPE_POINTLIST                     = 0,   // 0x0: Point List
    DIP_P_TYPE_LINELIST                      = 1,   // 0x1: LineList
    DIP_P_TYPE_LINESTRIP                     = 2,   // 0x2: Line Strip
    DIP_P_TYPE_LINELOOP                      = 3,   // 0x3: Line Loop/ OVG path
    DIP_P_TYPE_TRIANGLELIST                  = 4,   // 0x4: Triangle List
    DIP_P_TYPE_TRIANGLESTRIP                 = 5,   // 0x5: Triangle Strip
    DIP_P_TYPE_TRIANGLEFAN                   = 6,   // 0x6: Triangle Fan
} DIP_P_TYPE;
typedef enum
{
    INTERNAL_FENCE_FENCE_UPDATE_MODE_COPY    = 0,   // 0x0: Copy operation
    INTERNAL_FENCE_FENCE_UPDATE_MODE_OR      = 1,   // 0x1: Or operation
} INTERNAL_FENCE_FENCE_UPDATE_MODE;
typedef enum
{
    INTERNAL_FENCE_FENCE_UPDATE_TIMING_UPDATE_AFTER_READ= 0,
                                                    // 0x0: update after engine finishes read
    INTERNAL_FENCE_FENCE_UPDATE_TIMING_UPDATE_AFTER_WRITE= 1,
                                                    // 0x1: update after engine finishes write
} INTERNAL_FENCE_FENCE_UPDATE_TIMING;
typedef enum
{
    INTERNAL_FENCE_SWAP_ENABLE_INTERNAL_FENCE= 0,   // 0x0: real internal fence cmd
    INTERNAL_FENCE_SWAP_ENABLE_SWAP          = 1,   // 0x1: Lw/ Hi swap cmd
} INTERNAL_FENCE_SWAP_ENABLE;
typedef enum
{
    BLOCK_COMMAND_TEMPLATE_TYPE_IMAGE_TRANSFER= 0,  // 0x00: image transfer
    BLOCK_COMMAND_TEMPLATE_TYPE_FLUSH_CACHE  = 1,   // 0x01: flush cache
    BLOCK_COMMAND_TEMPLATE_TYPE_INVALIDATE_CACHE= 2,// 0x02: invalidate cache
    BLOCK_COMMAND_TEMPLATE_TYPE_SETUP_SG     = 3,   // 0x03: Setup, SG
} BLOCK_COMMAND_TEMPLATE_TYPE;
typedef enum
{
    BLOCK_COMMAND_TAS_TYPE_COLOR_EXCLUDED    = 0,   // color is not followed for DP line. E.g. dw0=vertex0,
                                                    // dw1=vertex1.
    BLOCK_COMMAND_TAS_TYPE_COLOR_INCLUDED    = 1,   // color is followed for DP line besides vertex coordinates.
                                                    // E.g. dw0=color, dw1=vertex0, dw2=vertex1. repeat for
                                                    // more lines. 
} BLOCK_COMMAND_TAS_TYPE;
typedef enum
{
    BLOCK_COMMAND_TAS_COMMAND_SPECIFIC_FIELD_DP_LINE= 0,
                                                    // 0x00: Draw 2D Line with input vertex and color
    BLOCK_COMMAND_TAS_COMMAND_SPECIFIC_FIELD_DP_TRIANGLE= 1,
                                                    // 0x01: Draw 3D triangle Coverage with input vertex. Set
                                                    // DWC=3*n, where n=number of triangle list. Dw0=vertex0,
                                                    // dw1=vertex1, dw2=vertex2. 
    BLOCK_COMMAND_TAS_COMMAND_SPECIFIC_FIELD_RESTORE_3D_REG_FROM_SBUF= 2,
                                                    // 0x02: Restore 3D register and enable mini-shadow buffer.
                                                    // Set DWC=0
    BLOCK_COMMAND_TAS_COMMAND_SPECIFIC_FIELD_DISABLE_SBUF= 3,
                                                    // 0x03: Disable Shadow buffer. Set DWC=0
    BLOCK_COMMAND_TAS_COMMAND_SPECIFIC_FIELD_DP_STRETCHBLT= 4,
                                                    // 0x04: stretch blt with input start vertex position,
                                                    // width/height, start UV and UV deltas. Set DWC=6,
                                                    // dw0=start vertex XY, dw1=width/height, dw2=Us, dw3=Vs,
                                                    // dw4=dU/dX, dw5=dV/dY.
} BLOCK_COMMAND_TAS_COMMAND_SPECIFIC_FIELD;
typedef enum
{
    BLOCK_COMMAND_SETUP_SG_AREA_TARGET_Z16   = 0,   // 0x00: Z16
    BLOCK_COMMAND_SETUP_SG_AREA_TARGET_Z24   = 1,   // 0x01: Z24
    BLOCK_COMMAND_SETUP_SG_AREA_TARGET_S     = 2,   // 0x02: Stencil
    BLOCK_COMMAND_SETUP_SG_AREA_TARGET_D     = 3,   // 0x03: RT
} BLOCK_COMMAND_SETUP_SG_AREA_TARGET;
typedef enum
{
    BLOCK_COMMAND_SETUP_SG_SETUP_TYPE_AREA_GEN= 0,  // 0x00: Area gen, for SG to generate drawing area
    BLOCK_COMMAND_SETUP_SG_SETUP_TYPE_RESERVED= 1,  // 0x01: Reserved
} BLOCK_COMMAND_SETUP_SG_SETUP_TYPE;
typedef enum
{
    BLOCK_COMMAND_SETUP_SG_ACTION_TYPE_FAST_CLEAR_TILE= 0,
                                                    // 0x00: Fast Clear Tiled Surface where STG generates tile
    BLOCK_COMMAND_SETUP_SG_ACTION_TYPE_FAST_CLEAR_LINEAR= 1,
                                                    // 0x01: Fast Clear Linear Surface where ZL3 controls
                                                    // traversing 
    BLOCK_COMMAND_SETUP_SG_ACTION_TYPE_BIT_BLT= 2,  // 0x02: Bit Blt
    BLOCK_COMMAND_SETUP_SG_ACTION_TYPE_GRADIENT_FILL= 3,
                                                    // 0x03: Gradient Fill
    BLOCK_COMMAND_SETUP_SG_ACTION_TYPE_SCALING= 4,  // 0x04: Scaling
    BLOCK_COMMAND_SETUP_SG_ACTION_TYPE_ROTATION= 5, // 0x05: Rotation
    BLOCK_COMMAND_SETUP_SG_ACTION_TYPE_RESERVED= 7, // 0x07: Reserved
} BLOCK_COMMAND_SETUP_SG_ACTION_TYPE;
typedef enum
{
    BLOCK_COMMAND_SETUP_SG_ROTATION_DWORD0_CW_CW= 0,// 0x00: CW
    BLOCK_COMMAND_SETUP_SG_ROTATION_DWORD0_CW_CCW= 1,
                                                    // 0x01: CCW
} BLOCK_COMMAND_SETUP_SG_ROTATION_DWORD0_CW;
typedef enum
{
    BLOCK_COMMAND_SETUP_SG_SCALING_DWORD2_HORIZONTAL_TAP_POINT= 0,
                                                    // 0: point, only one tap
    BLOCK_COMMAND_SETUP_SG_SCALING_DWORD2_HORIZONTAL_TAP_LINEAR= 1,
                                                    // 1: linear, two taps
    BLOCK_COMMAND_SETUP_SG_SCALING_DWORD2_HORIZONTAL_TAP_4TAP= 2,
                                                    // 2: special linear, four taps
} BLOCK_COMMAND_SETUP_SG_SCALING_DWORD2_HORIZONTAL_TAP;
typedef enum
{
    BLOCK_COMMAND_CCH_FLSH_TARGET_ALL        = 0,   // 0x00: all  (including U#)
    BLOCK_COMMAND_CCH_FLSH_TARGET_Z          = 1,   // 0x01: Z
    BLOCK_COMMAND_CCH_FLSH_TARGET_S          = 2,   // 0x02: S
    BLOCK_COMMAND_CCH_FLSH_TARGET_D          = 3,   // 0x03: D
} BLOCK_COMMAND_CCH_FLSH_TARGET;
typedef enum
{
    BLOCK_COMMAND_VCS_TYPE_INVALIDATE_L1I    = 0,   // 0x00: invalidate L1 instruction cache
    BLOCK_COMMAND_VCS_TYPE_DRAIN_VS          = 1,   // 0x01: drain VS, for 3D only.
    BLOCK_COMMAND_VCS_TYPE_DRAIN_CS          = 2,   // 0x02: drain CS, for GP only. The EU needs to make sure all CS
                                                    // threads have finished and the EU-WBU FIFO empty.
} BLOCK_COMMAND_VCS_TYPE;
typedef enum
{
    BLOCK_COMMAND_PS_TYPE_INVALIDATE_L1I     = 0,   // 0x00: invalidate L1 instruction cache
    BLOCK_COMMAND_PS_TYPE_DRAIN_PS           = 1,   // 0x01: drain PS, for 3D only. The EU needs to make sure all PS
                                                    // threads have finished and the EU-WBU FIFO empty.
} BLOCK_COMMAND_PS_TYPE;
typedef enum
{
    BLOCK_COMMAND_MXU_IRQ_NOP                = 0,   // 00: Nop
    BLOCK_COMMAND_MXU_IRQ_INTERRUPT          = 1,   // 01: interrupt when external fence done
} BLOCK_COMMAND_MXU_IRQ;
typedef enum
{
    BLOCK_COMMAND_MXU_EXTERNAL_FENCE_NOP     = 0,   // 0x00: Nop
    BLOCK_COMMAND_MXU_EXTERNAL_FENCE_EXTERNAL_FENCE= 1,
                                                    // 0x01: external fence, 3 DW will be followed: addr (1st),
                                                    // tag0 (2nd), tag1(3rd)
} BLOCK_COMMAND_MXU_EXTERNAL_FENCE;
typedef enum
{
    BLOCK_COMMAND_CSP_INFO_EU_OFF_FF_OFF     = 0,   // Which partition BCI can turn off now. (BCI will do the
                                                    // action when the partition is empty.)
    BLOCK_COMMAND_CSP_INFO_EU_OFF_FF_ON      = 1,   // Which partition BCI can turn off now. (BCI will do the
                                                    // action when the partition is empty.)
    BLOCK_COMMAND_CSP_INFO_EU_ON_FF_OFF      = 2,   // Which partition BCI can turn off now. (BCI will do the
                                                    // action when the partition is empty.)
    BLOCK_COMMAND_CSP_INFO_EU_ON_FF_ON       = 3,   // Which partition BCI can turn off now. (BCI will do the
                                                    // action when the partition is empty.)
    BLOCK_COMMAND_CSP_INFO_SECOND_SLICE_ON   = 4,   // Whether BCI will turn on second partition.
    BLOCK_COMMAND_CSP_INFO_CLEAR_FRAME_BUFFER_INFO= 0,
                                                    // When the type is clear_frame_buffer, driver can set the
                                                    // field with this value. To HW, this filed in such case is
                                                    // discarded in fact.
    BLOCK_COMMAND_CSP_INFO_CONDITIONAL_SAVE_RESTORE= 0,
                                                    // When the type is save/restore, whether the action will do
                                                    // or not depends on status.
    BLOCK_COMMAND_CSP_INFO_FORCE_SAVE_RESTORE= 1,   // When the type is save/restore, the action must do.
} BLOCK_COMMAND_CSP_INFO;
typedef enum
{
    BLOCK_COMMAND_CSP_TYPE_CLEAR_FRAME_BUFFER= 0,   // 0:clearFB cmd
    BLOCK_COMMAND_CSP_TYPE_SAVE              = 1,   // 1:Save cmd
    BLOCK_COMMAND_CSP_TYPE_RESTORE           = 2,   // 2:Restore cmd
    BLOCK_COMMAND_CSP_TYPE_STATUS_INDICATOR  = 3,   // 3:Status indicator cmd (hw will know whether it can turn
                                                    // off some partition, and when eu is off while ff is on, all
                                                    // commands will be sent from BCI to TAS directly)
} BLOCK_COMMAND_CSP_TYPE;
typedef enum
{
    BLOCK_COMMAND_VIDEO_IRQ_NOP              = 0,   // 00: Nop
    BLOCK_COMMAND_VIDEO_IRQ_INTERRUPT        = 1,   // 01: interrupt when external fence done
} BLOCK_COMMAND_VIDEO_IRQ;
typedef enum
{
    BLOCK_COMMAND_VIDEO_EXTERNAL_FENCE_NOP   = 0,   // 0x00: Nop
    BLOCK_COMMAND_VIDEO_EXTERNAL_FENCE_EXTERNAL_FENCE= 1,
                                                    // 0x01: external fence, 3 DW will be followed: addr (1st),
                                                    // tag0 (2nd), tag1(3rd)
} BLOCK_COMMAND_VIDEO_EXTERNAL_FENCE;
typedef enum
{
    FLIP_SHOW_CAN_BE_DROPPED                 = 0,   // 0: Can be dropped
    FLIP_SHOW_CANNOT_BE_DROPPED              = 1,   // 1: Must show
} FLIP_SHOW;
typedef enum
{
    FLIP_SS2_NO_WAIT                         = 0,   // 0: No Wait
    FLIP_SS2_WAIT                            = 1,   // 1: Wait
} FLIP_SS2;
typedef enum
{
    FLIP_SS1_NO_WAIT                         = 0,   // 0: No Wait
    FLIP_SS1_WAIT                            = 1,   // 1: Wait
} FLIP_SS1;
typedef enum
{
    FLIP_HS1_NO_WAIT                         = 0,   // 0: No Wait
    FLIP_HS1_WAIT                            = 1,   // 1: Wait
} FLIP_HS1;
typedef enum
{
    FLIP_PS2_NO_WAIT                         = 0,   // 0: No Wait
    FLIP_PS2_WAIT                            = 1,   // 1: Wait
} FLIP_PS2;
typedef enum
{
    FLIP_PS1_NO_WAIT                         = 0,   // 0: No Wait
    FLIP_PS1_WAIT                            = 1,   // 1: Wait
} FLIP_PS1;
typedef enum
{
    FLIP_HS2_NO_WAIT                         = 0,   // 0: No Wait
    FLIP_HS2_WAIT                            = 1,   // 1: Wait
} FLIP_HS2;
typedef enum
{
    FLIP_FE_BACK_END                         = 0,   // 0: Back End
    FLIP_FE_FRONT_END                        = 1,   // 1: Front End
} FLIP_FE;
typedef enum
{
    FLIP_IRQ_NONE                            = 0,   // 0: None
    FLIP_IRQ_INTERRUPT                       = 1,   // 1: Interrupt CPU
} FLIP_IRQ;
typedef enum
{
    ISP_KICKOFF_IF_SCENE_CHANGE_NORMAL       = 0,   // 0: kickoff this cmd as normal
    ISP_KICKOFF_IF_SCENE_CHANGE_SCENE_CH     = 1,   // 1: kickoff this cmd if scene change, otherwise skip this
                                                    // cmd
} ISP_KICKOFF_IF_SCENE_CHANGE;
typedef enum
{
    ISP_MODE_ISP                             = 0,   // 0x00: Isp
    ISP_MODE_VPP                             = 1,   // 0x01: Vpp
} ISP_MODE;
typedef enum
{
    AES_MODE_NONE                            = 0,   // 00: No encryption operation on source data
    AES_MODE_XOR                             = 1,   // 01: Dest = Source XOR Constant_Key
    AES_MODE_AES_CTR                         = 2,   // 10: AES-CTR Encryption/Decryption
    AES_MODE_CIPHER                          = 3,   // 11: Cascaded Cipher Decryption
} AES_MODE;
typedef enum
{
    VIDEO_TRANSLATE_VIRTUAL                  = 0,   // 0: Use Virtual address,The address shall be translated in
                                                    // the MXU
    VIDEO_TRANSLATE_PHYSICAL                 = 1,   // 1: Use physical address,The address shall not be
                                                    // translated in the MXU
} VIDEO_TRANSLATE;
typedef enum
{
    VIDEO_INTERRUPT_RESERVED                 = 0,   // 00: Reserved
    VIDEO_INTERRUPT_IMMEDIATE                = 1,   // 01: Immediate
    VIDEO_INTERRUPT_LOW_PRIORITY             = 2,   // 10: Low priority
    VIDEO_INTERRUPT_SAVE_START               = 3,   // 11: Save start
} VIDEO_INTERRUPT;
typedef enum
{
    VIDEO_ROUTING_VLD                        = 0,   // 0: PC belong to C0 Core
    VIDEO_ROUTING_VPU                        = 1,   // 1: PC belong to C1 Core
} VIDEO_ROUTING;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////            BCI_OPCODES Block (Block ID = 0) Register Definitions                     ///////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 
typedef struct _Cmd_Skip                    // 0h: SKIP Command
{
    unsigned int     Dwc                   : 22; // The number Command will be skipped
    unsigned int     Reserved              : 6;  // Reserved
    unsigned int     Major_Opcode          : 4;  // 0000: SKIP. 
} Cmd_Skip;
 
typedef struct _Cmd_Dma                     // 1h: DMA Command
{
    unsigned int     Dwc                   : 22; // (0 reseved)number of DW to be fetched after header
    unsigned int     Flag_2d               : 1;  // 0:Default
                                                 //  1:skip 3d Trigger and re_route Fence when 3D is power-down
    unsigned int     Reserved              : 5;  // Reserved
    unsigned int     Major_Opcode          : 4;  // 0001: DMA (do not allow DMA inside DMA). 
} Cmd_Dma;
/////////////////////////////////////////////////////////////////////////
// some other description about the command usage: Start
       typedef struct _Cmd_Dma_Address_Dword        // 1h DWF1: DWORD for DMA_Address
       {
            unsigned int     Reserved              : 2;  // Reserved
            unsigned int     Address               : 30; // DW aligned memory address to fetch
       } Cmd_Dma_Address_Dword;
// some other description about the command usage: End
/////////////////////////////////////////////////////////////////////////
 
 
typedef struct _Cmd_Wait                    // 2h: Wait Command
{
    unsigned int     Ref_Value             : 8; 
    unsigned int     Isp_Wait_Ref_Value_High
                                           : 4; 
    unsigned int     Isp_Wait_Stream_Number
                                           : 1; 
    unsigned int     Enable_Isp_Wait       : 1; 
    unsigned int     Reserved              : 8;  // Reserved
    unsigned int     Method                : 1;  // Wait Method
    unsigned int     Slot_Id               : 4;  // Slot ID
    unsigned int     Mode                  : 1;  // Mode to distinguish Internal or external
    unsigned int     Major_Opcode          : 4;  // 0010:Wait (external/internal wait)
} Cmd_Wait;
 
typedef struct _Cmd_Query_Dump              // 3h: Query Dump Command
{
    unsigned int     Dwc_Or_Fence_Tag      : 13; // Number of DW to be dumped, or tag for external fence dump
    unsigned int     Irq                   : 1;  // Interrupt
    unsigned int     Reserved              : 1; 
    unsigned int     Option                : 1;  // Option
    unsigned int     Reg_Offset            : 8;  // Register offset
    unsigned int     Block_Id              : 4;  // Block ID
    unsigned int     Major_Opcode          : 4;  // 0011: QueryDump (including external fence, dump
                                                 // status/context to host)
} Cmd_Query_Dump;
/////////////////////////////////////////////////////////////////////////
// some other description about the command usage: Start
       typedef struct _Cmd_Query_Dump_Address_Dword // 3h DWF1; DWORD for Query Dump Address
       {
            unsigned int     Reserved              : 2;  // Reserved
            unsigned int     Address               : 30; // DW aligned memory address to dump
       } Cmd_Query_Dump_Address_Dword;
// some other description about the command usage: End
/////////////////////////////////////////////////////////////////////////
 
 
typedef struct _Cmd_Set_Register            // 4h: Set Register Command
{
    unsigned int     Dwc                   : 7;  // the number of following dws, at least 1 DW and up to 64 DW will
                                                 // follow, but not exceeding block boundry
    unsigned int     Reserved              : 9;  // Reserved
    unsigned int     Start_Offset          : 8;  // each block will have up to 64 DWs
    unsigned int     Block_Id              : 4;  // Block ID
    unsigned int     Major_Opcode          : 4;  // 0100: SetRegister  (with multiple DW followed)
} Cmd_Set_Register;
 
typedef struct _Cmd_Set_Register_Short      // 5h: SetRegisterShort Command
{
    unsigned int     Register_Value        : 8;  // The regsiter value this command want to set, only 4 bits are
                                                 // valid (either high or low nibble) when set_whole_byte=0, 
                                                 //  where nibble offset bit 0 determines which nibble is
                                                 // valid.
    unsigned int     Nibble_Mask           : 4;  // Nibble mask
    unsigned int     Set_Whole_Byte        : 1;  // Set whole byte
    unsigned int     Nibble_Offset         : 3;  // Offset of nibbles from LSB, a nibble = 4 bits
    unsigned int     Start_Offset          : 8;  // Start offset, each block will have up to 256 DWs
    unsigned int     Block_Id              : 4;  // Block ID
    unsigned int     Major_Opcode          : 4;  // 0101: SetRegisterShort (without DW followed) 
} Cmd_Set_Register_Short;
 
typedef struct _Cmd_Set_Register_Addr       // 6h: Set Register Address Command
{
    unsigned int     Dwc                   : 16; // Number of following dws, value 0 is reserved,at least 1 DW
                                                 // and up to 2^16-1 DWs needs to be fetch from memory
    unsigned int     Start_Offset          : 8;  // Start offset, each block will have up to 64 DWs
    unsigned int     Block_Id              : 4;  // Block ID
    unsigned int     Major_Opcode          : 4;  // 0110: SetRegisterAddr (with address and size to be
                                                 // loaded)
} Cmd_Set_Register_Addr;
/////////////////////////////////////////////////////////////////////////
// some other description about the command usage: Start
       typedef struct _Cmd_Set_Register_Addr_Dword  // 6h DWF1: DWORD for Set_Register_Address
       {
            unsigned int     Reserved              : 2;  // Reserved
            unsigned int     Address               : 30; // DW aligned memory address to fetch
       } Cmd_Set_Register_Addr_Dword;
// some other description about the command usage: End
/////////////////////////////////////////////////////////////////////////
 
 
typedef struct _Cmd_Dip                     // 7h: DrawIndexedPrimitive Command
{
    unsigned int     Reserved0             : 20; // Reserved
    unsigned int     Index_Mode            : 2;  // Index Mode
    unsigned int     Reserved1             : 1;  // Reserved
    unsigned int     Instance_En           : 1;  // Instancing Enabled. 32-bit instance counter present.
    unsigned int     Draw_Auto_En          : 1;  // Draw auto enable bit
    unsigned int     P_Type                : 3;  // Primitive Type
    unsigned int     Major_Opcode          : 4;  // 0111: DrawIndexPrimitive (with index to be loaded)
} Cmd_Dip;
/////////////////////////////////////////////////////////////////////////
// some other description about the command usage: Start
       typedef struct _Cmd_Dip_Indexcount_Dword1    // 7h DWF1: DWORD for DIP_IndexCount
       {
            unsigned int     Index_Count           : 32; // Index Count
       } Cmd_Dip_Indexcount_Dword1;
       typedef struct _Cmd_Dip_Address_Dword2       // 7h DWF2: DWORD for DIP_Address
       {
            unsigned int     Address               : 32; // Byte aligned memory address to fetch
       } Cmd_Dip_Address_Dword2;
       typedef struct _Cmd_Dip_Drawauto_Dword2      // 7h DWF2: DWORD for DIP_DrawAuto
       {
            unsigned int     Start_Index           : 32; // Start Index when Draw_Auto_En = 1
       } Cmd_Dip_Drawauto_Dword2;
       typedef struct _Cmd_Dip_Instancecount_Dword3 // 7h DWF3: DWORD for DIP_InstanceCount
       {
            unsigned int     Instance_Count        : 32; // Instance Count
       } Cmd_Dip_Instancecount_Dword3;
// some other description about the command usage: End
/////////////////////////////////////////////////////////////////////////
 
 
typedef struct _Cmd_Internal_Fence          // 8h: InternalFence Command
{
    unsigned int     Update_Value          : 8;  // Update value, used to update the counter in update mode
    unsigned int     Slot_Id               : 4;  // Fence Slot ID
    unsigned int     Fence_Update_Mode     : 1;  // Fence update mode
    unsigned int     Fence_Update_Timing   : 1;  // The internal fence value it waits for
    unsigned int     Swap_Enable           : 1; 
    unsigned int     Two_Slice_Enable      : 1;  // This bit will not be touched by driver. BCI will set it
                                                 // according to current ring buffer status indicator. And
                                                 // MXU will according to this bit to decide wait both internal
                                                 // fence from FFC or not.
    unsigned int     Reserved              : 8;  // Reserved
    unsigned int     Block_Id              : 4;  // Block ID
    unsigned int     Major_Opcode          : 4;  // 1000: InternalFence (wait for internal block completion
                                                 // before next command can be executed)
} Cmd_Internal_Fence;
 
typedef struct _Cmd_Block_Command_Template  // 9h: BlockCommand Template
{
    unsigned int     Dwc                   : 16; // Number of dw followed
    unsigned int     Type                  : 2;  // Type
    unsigned int     Command_Specific_Field
                                           : 6;  // Defined at block level.
    unsigned int     Block_Id              : 4;  // Block ID
    unsigned int     Major_Opcode          : 4;  // 1001: BlockCommand(With multiple dw followed, for block
                                                 // level parsing, such as 2D draw)
} Cmd_Block_Command_Template;
 
typedef struct _Cmd_Block_Command_Tas       // 9h: BlockCommand starting at TAS
{
    unsigned int     Dwc                   : 16; // Number of dw followed
    unsigned int     Type                  : 2;  // Type
    unsigned int     Command_Specific_Field
                                           : 6;  // Command for TAS
    unsigned int     Block_Id              : 4;  // Block ID, set to TAS
    unsigned int     Major_Opcode          : 4;  // 1001: BlockCommand(With multiple dw followed, for block
                                                 // level parsing, such as 2D draw)
} Cmd_Block_Command_Tas;
 
typedef struct _Cmd_Block_Command_Setup_Sg  // 9h: BlockCommand starting at Setup/SG
{
    unsigned int     Dwc                   : 16; // Number of dw followed
    unsigned int     Type                  : 2;  // Type
    unsigned int     Area_Target           : 2;  // Area Target
    unsigned int     Setup_Type            : 1;  // Setup type
    unsigned int     Action_Type           : 3;  // Action Type
    unsigned int     Block_Id              : 4;  // Block ID, set to Setup/SG
    unsigned int     Major_Opcode          : 4;  // 1001: BlockCommand(With multiple dw followed, for block
                                                 // level parsing, such as 2D draw)
} Cmd_Block_Command_Setup_Sg;
/////////////////////////////////////////////////////////////////////////
// some other description about the command usage: Start
       typedef struct _Cmd_Block_Command_Setup_Sg_Dword0
                                                    // 9h DWF1: DWORD for BlockCommand Setup/SG
       {
            unsigned int     Xmin                  : 12; // X min.
            unsigned int     Reserved0             : 4;  // Reserved
            unsigned int     Xmax                  : 12; // X max.
            unsigned int     Reserved1             : 4;  // Reserved
       } Cmd_Block_Command_Setup_Sg_Dword0;
       typedef struct _Cmd_Block_Command_Setup_Sg_Rotation_Dword0
                                                    // 9h DWF1: DWORD for BlockCommand Setup/SG. For rotation
       {
            unsigned int     Cw                    : 1; 
            unsigned int     Reserved0             : 31; // Reserved
       } Cmd_Block_Command_Setup_Sg_Rotation_Dword0;
       typedef struct _Cmd_Block_Command_Setup_Sg_Dword1
                                                    // 9h DWF2: DWORD for BlockCommand Setup/SG
       {
            unsigned int     Ymin                  : 12; // Y min.
            unsigned int     Reserved0             : 4;  // Reserved
            unsigned int     Ymax                  : 12; // Y max.
            unsigned int     Reserved1             : 4;  // Reserved
       } Cmd_Block_Command_Setup_Sg_Dword1;
       typedef struct _Cmd_Block_Command_Setup_Sg_Blt_Dword2
                                                    // 9h DWF3: DWORD for BlockCommand Setup/SG. For BitBlt
       {
            unsigned int     Dx                    : 13; // dX from src.
            unsigned int     Reserved0             : 3;  // Reserved
            unsigned int     Dy                    : 13; // dY from src.
            unsigned int     Reserved1             : 3;  // Reserved
       } Cmd_Block_Command_Setup_Sg_Blt_Dword2;
       typedef struct _Cmd_Block_Command_Setup_Sg_Fc_Dword2
                                                    // 9h DWF3: DWORD for BlockCommand Setup/SG. Treat it as whole 32 bit dw
                                                    // for fast clear cmd.
       {
            unsigned int     Clear_Value_Low       : 32; // Clear Value low 32 bits.
       } Cmd_Block_Command_Setup_Sg_Fc_Dword2;
       typedef struct _Cmd_Block_Command_Setup_Sg_Scaling_Dword2
                                                    // 9h DWF3: DWORD for BlockCommand Setup/SG. For Scaling
       {
            unsigned int     Horizontal_Tap        : 2; 
            unsigned int     Vertical_Tap          : 2;  // The same value as HORIZONTAL_TAP
            unsigned int     Reserved0             : 28; // Reserved
       } Cmd_Block_Command_Setup_Sg_Scaling_Dword2;
       typedef struct _Cmd_Block_Command_Setup_Sg_Fc_Dword3
                                                    // 9h DWF4: DWORD for BlockCommand Setup/SG. Treat it as whole 32 bit dw
                                                    // for fast clear cmd.
       {
            unsigned int     Clear_Value_High      : 32; // Clear Value high 32 bits.
       } Cmd_Block_Command_Setup_Sg_Fc_Dword3;
       typedef struct _Cmd_Block_Command_Setup_Sg_Scaling_Dword
                                                    // 9h DWF4-7: DWORD for BlockCommand Setup/SG. For scaling, totally 4
                                                    // dwords, which are Useed, dUdx, Vseed, dVdy.
                                                    //  Here, U/V means the X/Y coordinates in source plane.
                                                    //  The seed is the coordinate of top-left pixel specified in the command
                                                    // that mapped into src surface. In 1/2-tap mode, the top-left is just the
                                                    // center; in 4-tap mode, the top-left just means center minus one in both
                                                    // directions. 
                                                    //  dx is used to specify the src surface coordinate difference in x
                                                    // direction of top-left samples for 2 neighboring pixels in dst
                                                    // surface.
       {
            unsigned int     Data                  : 24; // 12.12
            unsigned int     Reserved              : 8;  // Reserved
       } Cmd_Block_Command_Setup_Sg_Scaling_Dword;
       typedef struct _Cmd_Block_Command_Setup_Gradient_Fill_Dword
                                                    // 9h DWF3-14: DWORD for BlockCommand Setup/SG. For GradientFill
                                                    //  Totally 12 dwords, can be divided into 4 groups.
                                                    //  1st is R, 2nd is G, 3rd is B and 4th is A.
                                                    //  Every group has 3 dwords, 1st is seed, 2nd is dx, 3rd is dy.
                                                    //  The seed is the color of top-left pixel specified in the command, just
                                                    // the (xmin, ymin).
       {
            unsigned int     Color                 : 26; // 13.13
            unsigned int     Reserved              : 6;  // Reserved
       } Cmd_Block_Command_Setup_Gradient_Fill_Dword;
// some other description about the command usage: End
/////////////////////////////////////////////////////////////////////////
 
 
typedef struct _Cmd_Block_Command_Img_Trn   // 9h: BlockCommand for Image Transfer, starting at ZL3
{
    unsigned int     Dwc                   : 16; // Number of dw followed
    unsigned int     Type                  : 2;  // Type
    unsigned int     Reserved              : 6;  // reserved
    unsigned int     Block_Id              : 4;  // Block ID, set to ZL3
    unsigned int     Major_Opcode          : 4;  // 1001: BlockCommand(With multiple dw followed, for block
                                                 // level parsing, such as 2D draw)
} Cmd_Block_Command_Img_Trn;
/////////////////////////////////////////////////////////////////////////
// some other description about the command usage: Start
       typedef struct _Cmd_Block_Command_Img_Trn_Dword0
                                                    // 9h DWF1: DWORD for BlockCommand Image Transfer
       {
            unsigned int     Xmin                  : 12; // X min.
            unsigned int     Is_Dword_Aligned      : 1;  // Whether it is dword aligned or byte aligned.
                                                         //  In practice, only in 1/4/8/16 bpp case this bit is useful.
                                                         //  To driver, set this bit if the data will be dword aligned due
                                                         // to pitch size.
                                                         //  If this bit is not set, hw will always treat the data as byte
                                                         // aligned when x reaches end.
            unsigned int     Reserved0             : 3;  // Reserved
            unsigned int     Xmax                  : 12; // X max.
            unsigned int     Reserved1             : 4;  // Reserved
       } Cmd_Block_Command_Img_Trn_Dword0;
       typedef struct _Cmd_Block_Command_Img_Trn_Dword1
                                                    // 9h DWF2: DWORD for BlockCommand Image Transfer
       {
            unsigned int     Ymin                  : 12; // Y min.
            unsigned int     Reserved0             : 4;  // Reserved
            unsigned int     Ymax                  : 12; // Y max.
            unsigned int     Reserved1             : 4;  // Reserved
       } Cmd_Block_Command_Img_Trn_Dword1;
// some other description about the command usage: End
/////////////////////////////////////////////////////////////////////////
 
 
typedef struct _Cmd_Block_Command_Cch_Flsh  // 9h: BlockCommand for Cache Flush, starting at ZL3
{
    unsigned int     Dwc                   : 16; // Number of dw followed
    unsigned int     Type                  : 2;  // Type
    unsigned int     Target                : 2;  // Target
    unsigned int     Reserved              : 4;  // Reserved
    unsigned int     Block_Id              : 4;  // Block ID, set to ZL3
    unsigned int     Major_Opcode          : 4;  // 1001: BlockCommand(With multiple dw followed, for block
                                                 // level parsing, such as 2D draw)
} Cmd_Block_Command_Cch_Flsh;
 
typedef struct _Cmd_Block_Command_Vcs       // 9h: BlockCommand for VS/CS
{
    unsigned int     Dwc                   : 16; // Number of dw followed
    unsigned int     Type                  : 2;  // Type
    unsigned int     Command_Specific_Field
                                           : 6;  // Defined at block level.
    unsigned int     Block_Id              : 4;  // Block ID set to EU_VS
    unsigned int     Major_Opcode          : 4;  // 1001: BlockCommand(With multiple dw followed, for block
                                                 // level parsing)
} Cmd_Block_Command_Vcs;
 
typedef struct _Cmd_Block_Command_Ps        // 9h: BlockCommand for PS
{
    unsigned int     Dwc                   : 16; // Number of dw followed
    unsigned int     Type                  : 2;  // Type
    unsigned int     Command_Specific_Field
                                           : 6;  // Defined at block level.
    unsigned int     Block_Id              : 4;  // Block ID set to EU_PS
    unsigned int     Major_Opcode          : 4;  // 1001: BlockCommand(With multiple dw followed, for block
                                                 // level parsing)
} Cmd_Block_Command_Ps;
 
typedef struct _Cmd_Block_Command_Mxu       // 9h: BlockCommand for MXU
{
    unsigned int     Dwc                   : 16; // Number of dw followed, set to 3 for external fence
    unsigned int     Irq                   : 1;  // Interrupt
    unsigned int     External_Fence        : 1;  // External fence
    unsigned int     Flush_Cache           : 1;  // Flush flag buffer cache or not
    unsigned int     Invalidate_Cache      : 1;  // Invalidate flag buffer cache or not
    unsigned int     Fence_Update_Timing   : 1;  // When this bit is 1, MXU will send all write requests out then
                                                 // update the fence value.
    unsigned int     Reserved              : 2; 
    unsigned int     Bypass_To_L2          : 1;  // Bypass the command to L2
    unsigned int     Block_Id              : 4;  // Block ID set to MXU
    unsigned int     Major_Opcode          : 4;  // 1001: BlockCommand(With multiple dw followed, for block
                                                 // level parsing)
} Cmd_Block_Command_Mxu;
/////////////////////////////////////////////////////////////////////////
// some other description about the command usage: Start
       typedef struct _Cmd_Block_Command_Mxu_Dword0 // 9h DWF1: DWORD for BlockCommand MXU
       {
            unsigned int     Reserved              : 5;  // Reserved
            unsigned int     Start_Address         : 27; // The address is 256bits aligned.
       } Cmd_Block_Command_Mxu_Dword0;
       typedef struct _Cmd_Block_Command_Mxu_Dword1 // 9h DWF2: DWORD for BlockCommand MXU
       {
            unsigned int     Address_Byte_Mask     : 27; // The mask used to invalidate/flush memory based on start
                                                         // address.
            unsigned int     Reserved              : 5;  // Reserved
       } Cmd_Block_Command_Mxu_Dword1;
       typedef struct _Cmd_Block_Command_Mxu_L2_Dword0
                                                    // 9h DWF1: DWORD for BlockCommand MXU
       {
            unsigned int     Reserved              : 8;  // Reserved
            unsigned int     Address               : 24; // The address is 2048bits aligned.
       } Cmd_Block_Command_Mxu_L2_Dword0;
       typedef struct _Cmd_Block_Command_Mxu_L2_Dword1
                                                    // 9h DWF2: DWORD for BlockCommand MXU
       {
            unsigned int     Reserved              : 8;  // Reserved
            unsigned int     Mask                  : 24; // The mask used to invalidate L2 cache.
       } Cmd_Block_Command_Mxu_L2_Dword1;
// some other description about the command usage: End
/////////////////////////////////////////////////////////////////////////
 
 
typedef struct _Cmd_Block_Command_Csp       // 9h: BlockCommand for CSP
{
    unsigned int     Dwc                   : 2;  // Number of dw followed
    unsigned int     Counter               : 16; // Counter/ContextID
    unsigned int     Info                  : 3;  // Have different meanings according to different type.
    unsigned int     Type                  : 3;  // Type of current cmd
    unsigned int     Block_Id              : 4;  // Block ID set to CSP
    unsigned int     Major_Opcode          : 4;  // 1001: BlockCommand(With multiple dw followed, for block
                                                 // level parsing)
} Cmd_Block_Command_Csp;
/////////////////////////////////////////////////////////////////////////
// some other description about the command usage: Start
       typedef struct _Cmd_Block_Command_Csp_Dword0 // 9h DWF1: DWORD for BlockCommand CSP
       {
            unsigned int     Reserved              : 6;  // Reserved
            unsigned int     Start_Address         : 26; // The address is 512bits aligned. Or Just 18bits for
                                                         // save/restore addr which is 16K bytes aligned
       } Cmd_Block_Command_Csp_Dword0;
       typedef struct _Cmd_Block_Command_Csp_Dword1 // 9h DWF2: DWORD for BlockCommand CSP
       {
            unsigned int     Clear_Value           : 4;  // The clear value means the 4 bit that flag buffer will be
                                                         // cleared. HW will duplicate it to get full 64 bits to send to
                                                         // MXU.
            unsigned int     Reserved              : 28; // Reserved
       } Cmd_Block_Command_Csp_Dword1;
// some other description about the command usage: End
/////////////////////////////////////////////////////////////////////////
 
 
typedef struct _Cmd_Block_Command_Video     // 9h: BlockCommand for VIDEO
{
    unsigned int     Dwc                   : 16; // Number of dw followed, set to 3 for external fence
    unsigned int     Irq                   : 1;  // Interrupt
    unsigned int     External_Fence        : 1;  // External fence
    unsigned int     Reserved_1            : 6;  // Defined at block level.
    unsigned int     Block_Id              : 4;  // Block ID set to VIDEO. Should be set to 0xC.
    unsigned int     Major_Opcode          : 4;  // 1001: BlockCommand(With multiple dw followed, for block
                                                 // level parsing)
} Cmd_Block_Command_Video;
/////////////////////////////////////////////////////////////////////////
// some other description about the command usage: Start
       typedef struct _Cmd_Block_Command_Video_Dword0
                                                    // 9h DWF1: DWORD for BlockCommand VIDEO
       {
            unsigned int     Address               : 32; // The address is supposed to be 32bits aligned.
       } Cmd_Block_Command_Video_Dword0;
       typedef struct _Cmd_Block_Command_Video_Dword1
                                                    // 9h DWF2: DWORD for BlockCommand VIDEO
       {
            unsigned int     Value                 : 32; // Output DWORD0
       } Cmd_Block_Command_Video_Dword1;
       typedef struct _Cmd_Block_Command_Video_Dword2
                                                    // 9h DWF3: DWORD for BlockCommand VIDEO
       {
            unsigned int     Value                 : 32; // Output DWORD1
       } Cmd_Block_Command_Video_Dword2;
// some other description about the command usage: End
/////////////////////////////////////////////////////////////////////////
 
 
typedef struct _Cmd_Flip                    // Ah: Flip Command
{
    unsigned int     Show                  : 1;  // The frame can not be dropped, it must always be shown (for
                                                 // back end Flip only)
    unsigned int     Ss2                   : 1;  // Secondary Stream 2 Vertical SYNC
    unsigned int     Ss1                   : 1;  // Secondary Stream 1 Vertical SYNC
    unsigned int     Hs1                   : 1;  // Controller 1 Horizontal SYNC
    unsigned int     Ps2                   : 1;  // Primary Stream 2 Vertical SYNC
    unsigned int     Ps1                   : 1;  // Primary Stream 1 Vertical SYNC
    unsigned int     Hs2                   : 1;  // Controller 2 Horizontal SYNC
    unsigned int     Id                    : 7;  // Flip ID.
    unsigned int     Fe                    : 1;  // Front End 
    unsigned int     Irq                   : 1;  // Interrupt Request (FE Flip only) The BCI must send an
                                                 // interrupt request to the CPU on the flip¡¯s execution.
    unsigned int     Addr                  : 8;  // The starting address of the first DIU register to program.
    unsigned int     Reserved              : 2;  // Reserved
    unsigned int     Dwc                   : 2;  // DWORDs to follow. This value contains the number of
                                                 // registers to program (up to 3, always 1 for a FE flip).
    unsigned int     Major_Opcode          : 4;  // 1010: BCI Command OPCODE Ah Flip
} Cmd_Flip;
/////////////////////////////////////////////////////////////////////////
// some other description about the command usage: Start
       typedef struct _Cmd_Flip_Fe_Dword            // Ah DWF1: DWORD for FLIP FE Data Start Address
       {
            unsigned int     Reserved              : 2;  // Reserved
            unsigned int     Flip_Data_Start_Address
                                                   : 30; // FLIP DATA Start Base Address, DWord alignment
       } Cmd_Flip_Fe_Dword;
// some other description about the command usage: End
/////////////////////////////////////////////////////////////////////////
 
 
typedef struct _Cmd_Gp                      // Bh: General Purpose Computing
{
    unsigned int     Dwc                   : 6;  // Number of dw followed
    unsigned int     Reserved              : 18; // Reserved
    unsigned int     Block_Id              : 4;  // Block ID set to EUP
    unsigned int     Major_Opcode          : 4;  // 1111: General Purpose Computing
} Cmd_Gp;
 
typedef struct _Cmd_Isp                     // Ch: ISP Command
{
    unsigned int     Reserved_Isp          : 9;  // Reserved for isp command
    unsigned int     Kickoff_If_Scene_Change
                                           : 1;  // kickoff command mode
    unsigned int     Deinterlace_Top       : 1;  // 0: enable film check in VPP DI; 1: disable film check in VPP
                                                 // DI
    unsigned int     Fc_Disable            : 1;  // 0: enable film check in VPP DI; 1: disable film check in VPP
                                                 // DI
    unsigned int     Dn_En                 : 1;  // 0:disable VPP denoise; 1:enable VPP denoise.
    unsigned int     Md_En                 : 1;  // 0:disable VPP motion detection; 1:enable VPP motion
                                                 // detection.
    unsigned int     Fd_En                 : 1;  // 0:disable VPP FilmDetect; 1:enable VPP FilmDetect.
    unsigned int     Di_En                 : 1;  // 0:disable VPP deInterlace; 1:enable VPP deInterlace.
    unsigned int     Dbk_En                : 1;  // 0:disable VPP deblock; 1:enable VPP deblock.
    unsigned int     Sharpness_En          : 1;  // 0:disable VPP sharpness; 1:enable VPP sharpness.
    unsigned int     Two_Frame_En          : 1;  // 0:VPP 1 frame mode; 1:VPP 2 frame mode.
    unsigned int     Top_First             : 1;  // 0:bottom field is first; 1:top field is first; used by VPP
                                                 // FilmDetect.
    unsigned int     Reset_Fd              : 1;  // 1:reset VPP FilmDetect begin counter
    unsigned int     Reset_Fd_2v2          : 1;  // 1:reset VPP FilmDetect2v2 begin counter
    unsigned int     Other_Is_Future       : 1;  // 0:current frame is future; 1:other frame is future. used
                                                 // by VPP.
    unsigned int     Even_Fd_2v2           : 1;  // 0 = use odd field for 2v2 film detection; 1= use even field
                                                 // for 2v2 film detection.
    unsigned int     Dwc                   : 3; 
    unsigned int     Mode                  : 1; 
    unsigned int     Major_Opcode          : 4;  // 1100: ISP Cmd
} Cmd_Isp;
 
typedef struct _Cmd_Module_Ext_Vcp          // Dh: Module Extension VCP Command
{
    unsigned int     Reserved              : 15; // Reserved
    unsigned int     Timeout_Reset         : 1;  // Vcp Time Out Reset enable bit
    unsigned int     Vcp_Reset             : 1;  // Vcp Reset enable bit
    unsigned int     Flush                 : 1;  // FLUSH VCP data cache
    unsigned int     Invalidate_Ic         : 1;  // Invalidate VCP Instruction cache
    unsigned int     Invalidate_Dc         : 1;  // Invalidate VCP Data cache
    unsigned int     Block_Id              : 6;  // The BlockID for the template
    unsigned int     Dwf                   : 2;  // Number of DWORDs to follow this command.
    unsigned int     Major_Opcode          : 4;  // 1101: CSP Command OPCODE Dh Major Opcode Extension
} Cmd_Module_Ext_Vcp;
 
typedef struct _Cmd_Aes                     // Eh: AES Command
{
    unsigned int     Mode                  : 2;  // AES operation mode
    unsigned int     Remove003_En          : 1;  // Enable/disable remove 003 feature, 1 = do remove 003
    unsigned int     To_Memory             : 1;  // 0 = AES output to VCP; 1= AES output to memory
    unsigned int     Size                  : 18; // Transfer size in 128 bits unit for AES Transfer, 4M byte at
                                                 // most
    unsigned int     Dwf                   : 2;  // DWF- Number of DWORDs following. In this command, 1 DWORDs
                                                 // follow for AES=>VCP, 2 dws for AES=>Memory.
    unsigned int     Minor_Opcode          : 4;  // 0010: CSP Command MINOR_OPCODE 2h AES
    unsigned int     Major_Opcode          : 4;  // 1110: CSP Command OPCODE Eh Major Opcode Extension
} Cmd_Aes;
/////////////////////////////////////////////////////////////////////////
// some other description about the command usage: Start
       typedef struct _Cmd_Aes_Addr                 // Eh DWF1: DWORD for AES Source Address
       {
            unsigned int     Start_Byte            : 5;  // Start byte of AES Address, 256-bit aligned
            unsigned int     Aes_Source_Addr       : 27; // AES source Address, must be 256 bits aligned.
       } Cmd_Aes_Addr;
       typedef struct _Cmd_Aes_Dest_Addr            // Eh DWF2: DWORD for AES Destination Address
       {
            unsigned int     End_Byte              : 5;  // End byte of the last output 256 bits
            unsigned int     Aes_Dest_Addr         : 27; // AES Destination Address
       } Cmd_Aes_Dest_Addr;
// some other description about the command usage: End
/////////////////////////////////////////////////////////////////////////
 
 
typedef struct _Cmd_Video                   // Fh: Video Command
{
    unsigned int     Translate             : 1;  // Translate
    unsigned int     Interrupt             : 2;  // Interrupt
    unsigned int     Routing               : 1;  // Specify which processor the kick off is for. 0-PC belong to
                                                 // C0 Core, 1-PC belong to C1 Core
    unsigned int     Pc                    : 24; // Start of the program address, The address is 256 byte
                                                 // aligned
    unsigned int     Major_Opcode          : 4;  // 1111: CSP Command OPCODE Fh Video Command
} Cmd_Video;
/////////////////////////////////////////////////////////////////////////
// some other description about the command usage: Start
       typedef struct _Cmd_Video_Input_Address      // Fh DWF1: DWORD for Video Data Base Address
       {
            unsigned int     Address               : 32; // Data base address, dw alignment
       } Cmd_Video_Input_Address;
       typedef struct _Cmd_Video_Output_Address     // Fh DWF2: DWORD for Video VLD Output Address
       {
            unsigned int     Address               : 32; // VLD output address, data base address, dw alignment
       } Cmd_Video_Output_Address;
 
typedef union Bci_Opcodes_cmds
{
    Cmd_Skip                        cmd_Skip;
    Cmd_Dma                         cmd_Dma;
    Cmd_Wait                        cmd_Wait;
    Cmd_Query_Dump                  cmd_Query_Dump;
    Cmd_Set_Register                cmd_Set_Register;
    Cmd_Set_Register_Short          cmd_Set_Register_Short;
    Cmd_Set_Register_Addr           cmd_Set_Register_Addr;
    Cmd_Dip                         cmd_Dip;
    Cmd_Internal_Fence              cmd_Internal_Fence;
    Cmd_Block_Command_Template      cmd_Block_Command_Template;
    Cmd_Block_Command_Tas           cmd_Block_Command_Tas;
    Cmd_Block_Command_Setup_Sg      cmd_Block_Command_Setup_Sg;
    Cmd_Block_Command_Img_Trn       cmd_Block_Command_Img_Trn;
    Cmd_Block_Command_Cch_Flsh      cmd_Block_Command_Cch_Flsh;
    Cmd_Block_Command_Vcs           cmd_Block_Command_Vcs;
    Cmd_Block_Command_Ps            cmd_Block_Command_Ps;
    Cmd_Block_Command_Mxu           cmd_Block_Command_Mxu;
    Cmd_Block_Command_Csp           cmd_Block_Command_Csp;
    Cmd_Block_Command_Video         cmd_Block_Command_Video;
    Cmd_Flip                        cmd_Flip;
    Cmd_Gp                          cmd_Gp;
    Cmd_Isp                         cmd_Isp;
    Cmd_Module_Ext_Vcp              cmd_Module_Ext_Vcp;
    Cmd_Aes                         cmd_Aes;
    Cmd_Video                       cmd_Video;
    unsigned int                    uint ;
}Bci_Opcodes_cmd;

#endif
