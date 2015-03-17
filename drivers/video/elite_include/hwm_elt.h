#ifndef __HWM_ELT_H__
#define __HWM_ELT_H__


// Excalibur HW Dependent
#define HW_MAX_VS_INTERNALCONS_ELT        0
#define HW_MAX_PS_INST_ELT              128
#define HW_MAX_PS_TMPREG_ELT             16
#define HW_NORMAL_PS_TMPREG_ELT          10 
#define HW_MAX_TEX_SIZE_ELT            8192
#define HW_MAX_TARGET_SIZE_ELT         8192
#define HW_MAX_TEX_REPEAT_ELT          8192
#define HW_MAX_NUM_VIDEOPOR_ELT           0

// Max HW Dependent of Excalibur Family
#define HW_MAX_TS_ELTFAMILY               8
#define HW_MAX_PS_SAMPLERS_ELTFAMILY     16
#define HW_MAX_VS_SAMPLERS_ELTFAMILY      4
#define HW_MAX_DMAP_SAMPLERS_ELTFAMILY    1
#define HW_MAX_WORLD_MATRIX_ELTFAMILY     4
#define HW_MAX_TARGETS_ELTFAMILY          4
#define HW_MAX_VS_INST_ELTFAMILY        320
#define HW_MAX_VS_CONS_ELTFAMILY        256
#define HW_MAX_VS_CONSB_ELTFAMILY        16
#define HW_MAX_VS_CONSI_ELTFAMILY        16
#define HW_MAX_PS_INST_ELTFAMILY        192
#define HW_MAX_PS_CONS_ELTFAMILY        224
#define HW_MAX_PS_CONSB_ELTFAMILY        16
#define HW_MAX_PS_CONSI_ELTFAMILY        16
#define HW_MAX_PS_TEXREG_ELTFAMILY       40
#define HW_MAX_PS_TMPREG_ELTFAMILY       32
#define HW_MAX_VS_OUTPUT_CACHE_ELTFAMILY 16
#define HW_MAX_CLIP_PLANES_ELTFAMILY      6
#define HW_MAX_MXU_RANGE_ENTRY_ELT1      16
#define MXU_RANGE_ENTRY_RESERVED0         0 // Reserved slot0 for non STM path
#define MXU_RANGE_ENTRY_RESERVED1         1 // Reserved slot1 for non STM path
#define MXU_RANGE_ENTRY_DIU               2

#define BUFFER_FILLED_SIZE_DUMP_SLOT_SIZE 4
#define BUFFER_FILLED_SIZE_DUMP_SLOTS  1024
#define BUFFER_FILLED_SIZE_DUMP_POOL_SIZE BUFFER_FILLED_SIZE_DUMP_SLOT_SIZE*BUFFER_FILLED_SIZE_DUMP_SLOTS

// Excalibur Series
#define ZL1_ENABLE                          RegPreFix("S3GDW_ZL1Enable")
#define ZL2_ENABLE                          RegPreFix("S3GDW_ZL2Enable")
#define ZL1_ENDPIPE                         RegPreFix("S3GDW_ZL1EndPipe")
#define ZL2_ENDPIPE                         RegPreFix("S3GDW_ZL2EndPipe")
#define DISABLE_ZL2_ENDPIPE                 RegPreFix("S3GDW_DisableZL2EndPipe")
#define DISABLE_ZL3_ENDPIPE                 RegPreFix("S3GDW_DisableZL3EndPipe")
#define FORCE_ZL2                           RegPreFix("S3GDW_ForceZL2")
#define Z_AUTOCLEAR                         RegPreFix("S3GDW_ZAutoClear")
#define D_AUTOCLEAR                         RegPreFix("S3GDW_DAutoClear")
#define S_AUTOCLEAR                         RegPreFix("S3GDW_SAutoClear")
#define TEX_AUTOCLEAR                       RegPreFix("S3GDW_TexAutoClear")
#define TEX_COMPRESSION                     RegPreFix("S3GDW_TexCompression")
#define VIDEO_COMPRESSION                   RegPreFix("S3GDW_VideoCompression")
#define VPP_COMPRESSION                     RegPreFix("S3GDW_VPPCompression")
#define ENABLE_XRGB_PACKING                 RegPreFix("S3GDW_XRGBPacking")
#define Z_COMPRESSION                       RegPreFix("S3GDW_ZCompression")
#define D_COMPRESSION                       RegPreFix("S3GDW_DCompression")
#define S_COMPRESSION                       RegPreFix("S3GDW_SCompression")
#define SLICEENABLEMASK                     RegPreFix("S3GDW_SliceEnableMask")
#define BLOCKPOOL                           RegPreFix("S3GDW_BLOCKPOOL")
#define DUMP_CHIPIMAGE_PER_DRAW             RegPreFix("S3GDW_DumpChipImagePerDraw")
#define SKIP_DRAW_FOR_DUMP_SHADER           RegPreFix("S3GDW_Skip_Draw_For_Dump_Shader")
#define KICKOFF_PER_DRAW                    RegPreFix("S3GDW_KickOffPerDraw")
#define FLUSH_PIPE_PER_DRAW                 RegPreFix("S3GDW_FlushPipePerDraw")
#define CPU_HWMBUFCOPY                      RegPreFix("S3GDW_CPUHWMBUFCOPY")
#define ZL2ZL3BOTHENABLE                    RegPreFix("S3GDW_ZL2ZL3BothEnable")
#define FORCE_3D_BLT                        RegPreFix("S3GDW_Force3DBlt")
#define FORCE_CLIP_SPACE_CLIPPING           RegPreFix("S3GDW_ForceClipSpaceClipping")
#define FORCE_SETUP_DOT_4                   RegPreFix("S3GDW_ForceSetupDot4")
#define ZNONOVERLAP_OPT_ENABLE              RegPreFix("S3GDW_ZNonOverlapOptEnable")
#define SNONOVERLAP_OPT_ENABLE              RegPreFix("S3GDW_SNonOverlapOptEnable")
#define DBNONOVERLAP_OPT_ENABLE             RegPreFix("S3GDW_DbNonOverlapOptEnable")
#define TAGFORCEDX10                        RegPreFix("S3GDW_TAGForceDX10")
#define TAGDOT6PRECISION                    RegPreFix("S3GDW_TAGDot6Precision")
#define FORCEZCLAMP                         RegPreFix("S3GDW_ForceZClamp")
#define ANTILOCKFORCEKICKOFF                RegPreFix("S3GDW_AntiLockForceKickoff")
#define ANTILOCKTIMEOUT                     RegPreFix("S3GDW_AntiLockTimeOut")
#define AUTODRAIN                           RegPreFix("S3GDW_AutoDrain")
#define SS_FAST_PATH_ENALBE                 RegPreFix("S3GDW_SsFastPathEn")
#define AS_FAST_PATH_ENALBE                 RegPreFix("S3GDW_AsFastPathEn")
#define SSAS_SLOW_PATH_FORBID               RegPreFix("S3GDW_SsAsSlowPathForbid")
#define D3D_CONST_RENAME                    RegPreFix("S3GDW_D3D_ConstantRename")
#define ENABLE3DTILING                      RegPreFix("S3GDW_Enable3DTiling")
#define ENABLE_AF_TRI_ODD_OPT               RegPreFix("S3GDW_Enable_AF_TRI_Odd_Opt")
#define ENABLE_AF_BL_ODD_OPT                RegPreFix("S3GDW_Enable_AF_BL_Odd_Opt")
#define ENABLE_AF_TRI_ANGLE_OPT             RegPreFix("S3GDW_Enable_AF_TRI_Angle_Opt")
#define AF_TRI_ANGLE_QUALITY_LEVEL          RegPreFix("S3GDW_AF_TRI_Angle_Quality_Level")
#define ENABLE_AF_TRI_RATIO_OPT             RegPreFix("S3GDW_Enable_AF_TRI_Ratio_Opt")
#define AF_TRI_RATIO_QUALITY_LEVEL          RegPreFix("S3GDW_AF_TRI_Ratio_Quality_Level")
#define ENABLE_AF_BL_ANGLE_OPT              RegPreFix("S3GDW_Enable_AF_BL_Angle_Opt")
#define AF_BL_ANGLE_QUALITY_LEVEL           RegPreFix("S3GDW_AF_BL_ANGLE_Quality_Level")
#define ENABLE_AF_BL_RATIO_OPT              RegPreFix("S3GDW_Enable_AF_BL_Ratio_Opt")
#define AF_BL_RATIO_QUALITY_LEVEL           RegPreFix("S3GDW_AF_BL_Ratio_Quality_Level")
#define DRAINPIPE3DBLT                      RegPreFix("S3GDW_DrainPipe3DBlt")
#define VSPREFETCH                          RegPreFix("S3GDW_VsPrefetch")
#define SS_MAX_THREADS                      RegPreFix("S3GDW_SsMaxThreads")
#define AS_MAX_THREADS                      RegPreFix("S3GDW_AsMaxThreads")
#define SIGNATUREMODE                       RegPreFix("S3GDW_SignatureMode")
#define NEW_RANGE                           RegPreFix("S3GDW_New_Range")
#define DISABLE_8PPACK                      RegPreFix("S3GDW_Disable_8PPack")
#define ENABLE_CVSC_PWRMODE_SET01           RegPreFix("S3GDW_Enable_Cvsc_PwrMode_Set01")
#define ENABLE_8P_MODE                      RegPreFix("S3GDW_Enable_8p_Mode")


// OpenCL Chip Dependent Registry setting
#define OCL_ENABLE_CONTINUE_MODE            RegPreFix("S3GDW_OCL_ENABLE_CONTINUE_MODE")
#define OCL_ENABLE_PPMODE_KICKOFF           RegPreFix("S3GDW_OCL_ENABLE_PPMODE_KICKOFF")


#define	USERDEFINE_QUERY_MODELEXTENSION_START				0x40008000
#define USERDEFINE_QUERY_MODELEXTENSION_EX_START            0x4000EFFC
#define USERDEFINE_QUERY_SETREGISTER_START                  0x4000C000

#define MDL_EXT_DRIVER_SUPPORT                   1
#define MDL_EXT_VS_OUTPUT_PSIZE                  2
#define MDL_EXT_POINTSPRITE                      3
#define MDL_LH_PARA_ADD0                         4
#define MDL_LH_PARA_ADD1                         5
#define MDL_LH_PARA_ADD2                         6
#define MDL_LH_PARA_ADD3                         7
#define MDL_EXT_FORCELINEAR                      8
#define MDL_EXT_SMALLRINGBUFFER                  9
#define MDL_EXT_FORCEZL3                         10
#define MDL_EXT_CE_BIT_WRITE_MASK                11
#define MDL_EXT_RANGE_CMD                        12
#define MDL_EXT_S3GCOUNTER                       14
#define MDL_EXT_EUASM_SHADERCODEADDR0            17
#define MDL_EXT_EUASM_SHADERCODEADDR1            18
#define MDL_EXT_EUASM_SHADERCODEADDR2            19
#define MDL_EXT_EUASM_SHADERCODEADDR3            20
#define MDL_EXT_SET_LASTCREATESHADER             21
#define MDL_EXT_Z_OPTIMIZATION                   22
#define MDL_EXT_ENABLEZCOMPRESSION               23
#define MDL_EXT_ENABLEDCOMPRESSION               24
#define MDL_EXT_ENABLESCOMPRESSION               25
#define MDL_EXT_ZAUTOCLEAR                       26
#define MDL_EXT_DAUTOCLEAR                       27
#define MDL_EXT_SAUTOCLEAR                       28
#define MDL_EXT_FLUSH_DZCACHE                    29
#define MDL_EXT_ENABLE_RESOLVE                   30
#define MDL_EXT_ENABLE_CPUFASTCLEAR              31
#define MDL_EXT_ENABLE_ZL2ZL3_BOTH_ON            32
#define MDL_EXT_DUMPPERDRAWTIMESTAMP             33

// QUERY_SETREGISTER_START
#define MDL_EXT_SETREGISTER_REGISTER_NAME_ADDR0  1
#define MDL_EXT_SETREGISTER_REGISTER_NAME_ADDR1  2
#define MDL_EXT_SETREGISTER_REGISTER_NAME_ADDR2  3
#define MDL_EXT_SETREGISTER_REGISTER_NAME_ADDR3  4
#define MDL_EXT_SETREGISTER_FIELD_NAME_ADDR0     5
#define MDL_EXT_SETREGISTER_FIELD_NAME_ADDR1     6
#define MDL_EXT_SETREGISTER_FIELD_NAME_ADDR2     7
#define MDL_EXT_SETREGISTER_FIELD_NAME_ADDR3     8
#define MDL_EXT_SETREGISTER_INDEX                9
#define MDL_EXT_SETREGISTER_VALUE0               10
#define MDL_EXT_SETREGISTER_VALUE1               11
#define MDL_EXT_SETREGISTER_VALUE2               12
#define MDL_EXT_SETREGISTER_VALUE3               13
#define MDL_EXT_SETREGISTER_AUTOFORCE            14

#define MDL_EXT_MMIO_ADD0                        170
#define MDL_EXT_MMIO_ADD1                        171
#define MDL_EXT_MMIO_ADD2                        172
#define MDL_EXT_MMIO_ADD3                        173
#define MDL_EXT_MMIO_CONDITION_ADD0              174
#define MDL_EXT_MMIO_CONDITION_ADD1              175
#define MDL_EXT_MMIO_CONDITION_ADD2              176
#define MDL_EXT_MMIO_CONDITION_ADD3              177
#define MDL_EXT_MMIO_CONDITION_VALUE0            178
#define MDL_EXT_MMIO_CONDITION_VALUE1            179
#define MDL_EXT_MMIO_CONDITION_VALUE2            180
#define MDL_EXT_MMIO_CONDITION_VALUE3            181
//0 write 1 read 2 read_if 3 read_until 4 read_buffer   
#define MDL_EXT_MMIO_RW                          182
#define MDL_EXT_MMIO_VALUE0                      183
#define MDL_EXT_MMIO_VALUE1                      184
#define MDL_EXT_MMIO_VALUE2                      185
#define MDL_EXT_MMIO_VALUE3                      186
#define MDL_EXT_MMIO_MASK0                       187
#define MDL_EXT_MMIO_MASK1                       188
#define MDL_EXT_MMIO_MASK2                       189
#define MDL_EXT_MMIO_MASK3                       190

#define MDL_EXT_GET_THREAD_SPACE_INFO_ADD0       191
#define MDL_EXT_GET_THREAD_SPACE_INFO_ADD1       192
#define MDL_EXT_GET_THREAD_SPACE_INFO_ADD2       193
#define MDL_EXT_GET_THREAD_SPACE_INFO_ADD3       194
#define MDL_EXT_REL_THREAD_SPACE_INFO            195

#define MDL_EXT_LOGICAL_THREAD_ID                196
#define MDL_EXT_MAPPING_TABLE_ID                 197

#define MDL_EXT_UAVBUFFER_OFFSET0                201
#define MDL_EXT_UAVBUFFER_OFFSET1                202
#define MDL_EXT_UAVBUFFER_OFFSET2                203
#define MDL_EXT_UAVBUFFER_OFFSET3                204

#define MDL_EXT_UAVBUFFER_LENGTH0                205
#define MDL_EXT_UAVBUFFER_LENGTH1                206
#define MDL_EXT_UAVBUFFER_LENGTH2                207
#define MDL_EXT_UAVBUFFER_LENGTH3                208

#define MDL_EXT_BUILD_ID_MAPPING_TABLE_SIG       209
#define MDL_EXT_READ_XMEM_BUFFER_SIG             210
#define MDL_EXT_READ_BUFFER_SIG                  211
#define MDL_EXT_READ_BUFFER_IF_SIG               212
#define MDL_EXT_READ_XMEMORY_IF_SIG              213
#define MDL_EXT_WRITE_MMIO_IF_SIG                214
#define MDL_EXT_WRITE_MMIO_WITH_MASK_SIG         215
#define MDL_EXT_WRITE_MMIO_SIG                   216
#define MDL_EXT_READ_MMIO_SIG                    217
#define MDL_EXT_READ_MMIO_IF_SIG                 218
#define MDL_EXT_READ_MMIO_UNTIL_SIG              219
#define MDL_EXT_WRITE_MMIO_LOG_SIG               220
#define MDL_EXT_READ_KERNEL_BUFFER_SIG           221
#define MDL_EXT_READ_KERNEL_BUFFER_IF_SIG        222
#define MDL_EXT_READ_MMIO_WITH_MASK_SIG          230
#define MDL_EXT_READ_MMIO_DBGSTAT_THID_LOGICAL   231
#define MDL_EXT_READ_MMIO_THPC_LOGICAL           232

#define MDL_EXT_CL_KENREL_SM_CONF_SIZE           300


#define QUERY_EXT                                'EQ3S'  // S3 HW Counter    

#define QUERY_EXT_START                          1
#define QUERY_EXT_END                            2
#define QUERY_EXT_GETQUERYDATA                   3
#define QUERY_EXT_START_DEFER_BEFOREDRAW         4
#define QUERY_EXT_START_DEFER_BEFORESTATE        5


#define HQV_IN_USE                               0x00000001
#define MAX_HQV_OVERLAY_COUNT                    4
#define MAX_HQV_ROTATE_COUNT                     2
#define MAX_HQV_BLT_COUNT                        2


typedef enum HWM_FLUSHINV_TYPE_ELT
{
    HWM_FLUSHINV_ZL2ZC = 0x00000001   ,             // Flush ZL2 Z cache
    HWM_FLUSHINV_ZL2SC = 0x00000002   ,             // Flush ZL2 S cache
    HWM_FLUSHINV_DC    = 0x00000004   ,             // Flush D Cache
    HWM_FLUSHINV_ALL   = HWM_FLUSHINV_ZL2ZC|HWM_FLUSHINV_ZL2SC|HWM_FLUSHINV_DC
} HWM_FLUSHINV_TYPE_ELT;

typedef enum HWM_NOTIFY_TYPE_ELT
{
    HWM_NOTIFY_FLUSH,
    HWM_NOTIFY_END_DRAW,
} HWM_NOTIFY_TYPE_ELT;

//////////////////////////////////////////////////////////////////////////////
//
// Internal Sync related functions:
//
//
typedef enum
{
    HWM_SYNC_BCI_SLOT                 = 0,
    HWM_SYNC_FLUSHPIPE_SLOT           = HWM_SYNC_BCI_SLOT,
    HWM_SYNC_DUMP_SLOT                = HWM_SYNC_BCI_SLOT,
    HWM_SYNC_VS_SLOT                  = HWM_SYNC_BCI_SLOT,
    HWM_SYNC_ZL2_SLOT                 = 1,
    HWM_SYNC_WBU_SLOT                 = 2,
    HWM_SYNC_MAX_SLOT                 = HWM_SYNC_WBU_SLOT+1,
    // Above are slots used by 3D UMD
    HWM_SYNC_KMD_SLOT                 = 3,
    HWM_SYNC_CONTEXT_LAST             = HWM_SYNC_KMD_SLOT+1,
    HWM_SYNC_SWITCH_SLOT              = 4,
    // Above are stored in context buffer

    //Slot used by video
    HWM_SYNC_ISP_SLOT                 = 5,
    HWM_SYNC_MSVD_SLOT                = 6,
    HWM_SYNC_VCP_SLOT                 = 7,
} HWM_SYNC_SLOT_ELT;

#define HWM_SYNC_MODE_MASK      0xF0000000
#define HWM_SYNC_ACCESS_MASK    0x0F000000
#define HWM_SYNC_USAGEID_MASK   0x00FF0000

// Operation Mode
#define HWM_SYNC_3D             0x00000000
#define HWM_SYNC_CS             0x10000000
#define HWM_SYNC_OTHER          0x20000000
#define HWM_SYNC_NO_RESOURCE    0x30000000

// Access Type
#define HWM_SYNC_READ           0x00000000
#define HWM_SYNC_WRITE          0x01000000

// resource usages
#define HWM_SYNC_USAGETYPE_MASK 0x000000FF
#define HWM_SYNC_MAX_TYPE       0xb  // must be consistent with the following definitions
#define HWM_SYNC_MAX_READTYPE   0x5  // must be consistent with the following definitions

// normal draw and 3DBlt
#define HWM_SYNC_IB             0x00000000
#define HWM_SYNC_VB             0x00010001
#define HWM_SYNC_VS_CONST       0x00020001
#define HWM_SYNC_VS_TEX         0x00030001
#define HWM_SYNC_Z_R            0x00040002
#define HWM_SYNC_S_R            0x00050002
#define HWM_SYNC_PS_CONST       0x00060003
#define HWM_SYNC_PS_TEX         0x00070003
#define HWM_SYNC_PS_UAV         0x00080003
#define HWM_SYNC_Z_RW           0x01090005
#define HWM_SYNC_S_RW           0x010a0005
#define HWM_SYNC_Z3_RW          0x010b0006
#define HWM_SYNC_S3_RW          0x010c0006
#define HWM_SYNC_RT             0x010d0007
#define HWM_SYNC_DFLUSH         0x010e0005
#define HWM_SYNC_ZFLUSH         0x010f0005
#define HWM_SYNC_SFLUSH         0x01100005
#define HWM_SYNC_UFLUSH         0x01110005
#define HWM_SYNC_2DBLT_RT       0x01120009
#define HWM_SYNC_INVALIDATE_L2_CACHE   0x0112000A

// computer pipeline
#define HWM_SYNC_CS_CONST       0x10400001
#define HWM_SYNC_CS_TEX         0x10410001
#define HWM_SYNC_CS_UAR_R       0x10420001
#define HWM_SYNC_CS_UAR_W       0x11430007

// other operations: blt, clear, compress/msaa resolve, mipgen, query
#define HWM_SYNC_2DBLT_DST      0x21800009
#define HWM_SYNC_2DBLT_SRC      0x20810003
#define HWM_SYNC_3DBLT_DST      0x21820007
#define HWM_SYNC_3DBLT_SRC      0x20830003
#define HWM_SYNC_DFASTCLEAR     0x21840007
#define HWM_SYNC_ZFASTCLEAR     0x21850007
#define HWM_SYNC_SFASTCLEAR     0x21860007
#define HWM_SYNC_UFASTCLEAR     0x21870007
#define HWM_SYNC_COMPRESS_SRC   0x20880004
#define HWM_SYNC_COMPRESS_DST   0x21890007
#define HWM_SYNC_MSAA_SRC       0x208a0004
#define HWM_SYNC_MSAA_DST       0x218b0007
#define HWM_SYNC_AUTOCLEAR      0x218c0007
#define HWM_SYNC_FBCLEAR        0x218d0007

#define HWM_SYNC_QUERY_FE       0x30a00002
#define HWM_SYNC_QUERY_BE       0x30a10007
#define HWM_SYNC_DUMP_FE        0x31a20000
#define HWM_SYNC_DUMP_BE        0x31a30002
#define HWM_SYNC_CS_TO_3D       0x30a40008
#define HWM_SYNC_3D_TO_CS       0x30a50003
#define HWM_SYNC_CS_TO_CS       0x30a60008
#define HWM_SYNC_PRE_CS_TO_CS   0x30a70008
#define HWM_SYNC_3DBLT          0x31aa0007
#define HWM_SYNC_FLUSHPIPE      0x31ab0007
#define HWM_SYNC_PIPE_SAVE      0x31ac0007
#define HWM_SYNC_POST_DRAW      0x31ad0007

#define HWM_SYNC_ZRESOLVE       0x21b10001
#define HWM_SYNC_SRESOLVE       0x21b10001

///////////////////////////////////////////////////////////////////////////////

//
// Query type & constant definition.
//
typedef enum HWM_QUERYTYPE_ELT
{
    HWM_QUERY_EVENT,
    HWM_QUERY_TIMESTAMP,
    HWM_QUERY_TIMESTAMPDISJOINT,

    // D3D10DDI_QUERY_PIPELINESTATS
    HWM_QUERY_IA_VERTEX,
    HWM_QUERY_IA_PRIMITIVE,
    HWM_QUERY_VS_INVOCATION,
    HWM_QUERY_CLIPPER_INVOCATION,
    HWM_QUERY_CLIPPER_PRIMITIVE,
    HWM_QUERY_PS_INVOCATION,

    HWM_QUERY_CS_INVOCATION,

    HWM_QUERY_OCCLUSION,

    HWM_QUERY_LAST,
} HWM_QUERYTYPE_ELT;

#endif
