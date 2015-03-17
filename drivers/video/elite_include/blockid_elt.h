#ifndef _REGISTER_BLOCKID_ELT_H
#define _REGISTER_BLOCKID_ELT_H

// below are each Bloch's top information, time stamp, version, ID, limit 
#ifndef        BCI_GLOBAL_BLOCKBASE_INF
    #define    BCI_GLOBAL_BLOCKBASE_INF
    #define    BLOCK_BCI_GLOBAL_VERSION 1
    #define    BLOCK_BCI_GLOBAL_TIMESTAMP "2010/12/27 13:53:27"
    #define    BCI_GLOBAL_BLOCK                                           0x0 // match with BlockID.h
    //  The following part is purposely set instead of auto generated. So pay attention to it.
    #define    BCI_GLOBAL_REG_START                                       0x1B // match with BlockID.h
    #define    BCI_GLOBAL_REG_END                                         0x1D // match with BlockID.h
    #define    BCI_GLOBAL_REG_LIMIT                                       0x7E // match with BlockID.h
#endif

#ifndef        EU_VS_BLOCKBASE_INF
    #define    EU_VS_BLOCKBASE_INF
    #define    BLOCK_EU_VS_VERSION 1
    #define    BLOCK_EU_VS_TIMESTAMP "2010/10/25 10:00:52"
    #define    EU_VS_BLOCK                                                0x1 // match with BlockID.h
    #define    EU_VS_REG_START                                            0x0 // match with BlockID.h
    #define    EU_VS_REG_END                                              0x8D
    #define    EU_VS_REG_LIMIT                                            0x8D // remember to manually change this every time! match with BlockID.h
#endif

#ifndef        TAS_BLOCKBASE_INF
    #define    TAS_BLOCKBASE_INF
    #define    BLOCK_TAS_VERSION 1
    #define    BLOCK_TAS_TIMESTAMP "2009-12-23 6:19:16"
    #define    TAS_BLOCK                                                  0x2 // match with BlockID.h
    #define    TAS_REG_START                                              0x0 // match with BlockID.h
    #define    TAS_REG_END                                                0x8 // match with BlockID.h
    #define    TAS_REG_LIMIT                                              0x8 // match with BlockID.h
#endif

#ifndef        FIXED_FUNCTION_BLOCKBASE_INF
    #define    FIXED_FUNCTION_BLOCKBASE_INF
    #define    BLOCK_FIXED_FUNCTION_VERSION 1
    #define    BLOCK_FIXED_FUNCTION_TIMESTAMP "3/25/2011 10:49:41 AM"
    #define    FIXED_FUNCTION_BLOCK                                       0x3 // match with BlockID.h
    #define    FIXED_FUNCTION_REG_START                                   0x0 // match with BlockID.h
    #define    FIXED_FUNCTION_REG_END                                     0x1E // match with BlockID.h
    #define    FIXED_FUNCTION_REG_LIMIT                                   0x1E // match with BlockID.h
#endif

#ifndef    IU_BLOCKBASE_INF
	#define    IU_BLOCKBASE_INF
	#define    BLOCK_IU_VERSION 1
	#define    BLOCK_IU_TIMESTAMP "12/8/2009 12:15:49 PM"
	#define    IU_BLOCK													  0x4
	#define    IU_REG_START												  0x0 // match with BlockID.h
	#define    IU_REG_END												  0x3 // match with BlockID.h
	#define    IU_REG_LIMIT												  0x3
#endif

#ifndef        EU_PS_BLOCKBASE_INF
    #define    EU_PS_BLOCKBASE_INF
    #define    BLOCK_EU_PS_VERSION 1
    #define    BLOCK_EU_PS_TIMESTAMP "2009/12/21 15:37:35"
    #define    EU_PS_BLOCK                                                0x5
    #define    EU_PS_REG_START                                            0x0
    #define    EU_PS_REG_END                                              0x85
    #define    EU_PS_REG_LIMIT                                            0x85
#endif

#ifndef        TU_BLOCKBASE_INF
    #define    TU_BLOCKBASE_INF
    #define    BLOCK_TU_VERSION 1
    #define    BLOCK_TU_TIMESTAMP "2010/1/6 13:53:51"
    #define    TU_BLOCK                                                   0x6
    #define    TU_REG_START                                               0x0
    #define    TU_REG_END                                                 0xF4
    #define    TU_REG_LIMIT                                               0xF4
#endif

#ifndef        MXU_BLOCKBASE_INF
    #define    MXU_BLOCKBASE_INF
    #define    BLOCK_MXU_VERSION 1
	#define    BLOCK_MXU_TIMESTAMP "2011-1-21 13:43:57"
    #define    MXU_BLOCK                                                  0x7 // match with BlockID.h
    #define    MXU_REG_START                                              0x0 // match with BlockID.h
    #define    MXU_REG_END                                                0x7 // match with BlockID.h
    #define    MXU_REG_LIMIT                                              0x7 // match with BlockID.h
#endif

//  Manually added to ensure other blocks do not use 0x8.
#ifndef        MXU1_BLOCKBASE_INF
    #define    MXU1_BLOCKBASE_INF
    #define    BLOCK_MXU1_VERSION 1
	#define    BLOCK_MXU1_TIMESTAMP "2011-1-21 13:43:57"
    #define    MXU1_BLOCK                                                  0x8 // match with BlockID.h
    #define    MXU1_REG_START                                              0x0 // match with BlockID.h
    #define    MXU1_REG_END                                                0x100 // match with BlockID.h
    #define    MXU1_REG_LIMIT                                              0x100 // match with BlockID.h
#endif

//  Manually added to ensure other blocks do not use 0x9.
#ifndef        MXU2_BLOCKBASE_INF
    #define    MXU2_BLOCKBASE_INF
    #define    BLOCK_MXU2_VERSION 1
	#define    BLOCK_MXU2_TIMESTAMP "2011-1-21 13:43:57"
    #define    MXU2_BLOCK                                                  0x9 // match with BlockID.h
    #define    MXU2_REG_START                                              0x0 // match with BlockID.h
    #define    MXU2_REG_END                                                0x100 // match with BlockID.h
    #define    MXU2_REG_LIMIT                                              0x100 // match with BlockID.h
#endif

#ifndef        ISP_GLOBAL_BLOCKBASE_INF
    #define    ISP_GLOBAL_BLOCKBASE_INF
    #define    BLOCK_ISP_GLOBAL_VERSION 1
    #define    BLOCK_ISP_GLOBAL_TIMESTAMP "2011/1/5 15:47:10"
    #define    ISP_GLOBAL_BLOCK                                           0xB
    #define    ISP_GLOBAL_REG_START                                       0x0 // match with BlockID.h
    #define    ISP_GLOBAL_REG_END                                         0x64 // match with BlockID.h
    #define    ISP_GLOBAL_REG_LIMIT                                       0x64 // match with BlockID.h
#endif

//  The following modules do not have block id yet. Just use the same definition as ../Interface/BCI_OPCODE.h, QUERY_DUMP_BLOCK_ID.
#ifndef        VIDEO_BLOCKBASE_INF
	#define    VIDEO_BLOCK                                                0xC
#endif

#ifndef        ZL2_BLOCKBASE_INF
	#define    ZL2_BLOCK                                                  0xD
#endif

#ifndef        WBU_BLOCKBASE_INF
	#define    WBU_BLOCK                                                  0xE
#endif

#ifndef        ZL3_BLOCKBASE_INF
	#define    ZL3_BLOCK                                                  0xF
#endif

typedef struct _TimeStampTable_Struct
{
   char BlockName[96];
   char TimeStamp[96];
} TimeStampTable_Struct;

extern TimeStampTable_Struct TimeStampTable[];

#endif
