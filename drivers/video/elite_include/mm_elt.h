#ifndef __MM_ELT_H__
#define __MM_ELT_H__
#include "cil2_types.h"

typedef enum MM_ALLOCATION_TYPE_ELT
{
    MM_AT_GENERIC_ELT = 0,
    MM_AT_VERTEXBUFFER_ELT,
    MM_AT_INDEXBUFFER_ELT,
    MM_AT_CSHARP_ELT,
    MM_AT_TEXTURE_ELT,
    MM_AT_BCSHARP_ELT,
    MM_AT_RT_TEXTURE_ELT,
    MM_AT_DS_TEXTURE_ELT,
    MM_AT_RENDERTARGET_ELT,
    MM_AT_RENDERTARGET_SSB_ELT,
    MM_AT_DEPTHBUFFER_ELT,
    MM_AT_DEPTHBUFFER_SSB_ELT,
    MM_AT_STENCILBUFFER_ELT,
    MM_AT_STENCILBUFFER_SSB_ELT,
    MM_AT_EUPFSHADER_ELT,
    MM_AT_EUPBSHADER_ELT,
    MM_AT_THREADSPACE_ELT,
    MM_AT_BACKBUFFER_ELT,
    MM_AT_CONTEXT_ELT,
    MM_AT_FENCE_ELT,
    MM_AT_FLAGBUFFER_ELT,
    MM_AT_2D_ELT,
    MM_AT_VIDEO_RESERVEDMEM_ELT,
    MM_AT_VIDEO_DECODE_RT_ELT,
    MM_AT_VIDEO_DECODE_COMPBUFF_ELT,
    MM_AT_VIDEO_VIDEOPROCESS_RT_ELT,
    MM_AT_LAST_ELT,
} MM_ALLOCATION_TYPE_ELT;

typedef enum MM_DRIVERID_ELT
{
    MM_DRIVERID_INVALID = 0,
    // 2D
    MM_DRIVERID_2D_SOURCE,
    MM_DRIVERID_2D_DESTINATION,
    // 3D
    // IA
    MM_DRIVERID_VERTEXBUFFER_ADDRESS,//  32
    MM_DRIVERID_INDEXBUFFER_ADDRESS,
    // STO
    MM_DRIVERID_SO_BUFFER_ADDRESS,
    MM_DRIVERID_SO_BUFFER_BOUND,
    // VS
    MM_DRIVERID_VS_SHADER,
    MM_DRIVERID_VS_TEXTURE,          // 128
    MM_DRIVERID_VS_BCSHARP,

    // PS
    MM_DRIVERID_PS_SHADER,
    MM_DRIVERID_PS_TEXTURE,          // 128

    // OM
    MM_DRIVERID_RENDERTARGET,        //   8
    MM_DRIVERID_DEPTHBUFFER,
    MM_DRIVERID_STENCILBUFFER,

    // EU ThreadSpace
    MM_DRIVERID_THREADSPACE,
    MM_DRIVERID_CSHARP,

    MM_DRIVERID_RANGE_FLAGBUFFER,    //  32
    MM_DRIVERID_RANGE,               //  32
    MM_DRIVERID_RANGE_BOUND,         //  32

    // CS   
    MM_DRIVERID_CS_SHADER,
    MM_DRIVERID_CS_TEXTURE,          // 128
    MM_DRIVERID_CSUNORDEREDACCESS,   //   8

    MM_DRIVERID_3D_LAST,             //  All above are state related

    // RM 3DBLT,Clear,MASS,AutoGen
    MM_DRIVERID_RM_DEPTHBUFFER,
    MM_DRIVERID_RM_STENCILBUFFER,
    MM_DRIVERID_RM_PS_TEXTURE,
    MM_DRIVERID_RM_RENDERTARGET,
    //#if UMD_3DBLT_VB
    MM_DRIVERID_RM_VERTEXBUFFER,
    //#endif

    // MISC
    MM_DRIVERID_CONTEXT,
    MM_DRIVERID_FENCE,    
    MM_DRIVERID_DIP_INDIRECT_BUFFER_ADDRESS,
    MM_DRIVERID_FLAGBUFFER,

    // Video
    MM_DRIVERID_VIDEO_RESERVEDMEM,          // 16
    MM_DRIVERID_VIDEO_SIGNATURE,            // 8
    MM_DRIVERID_VIDEO_FENCE,                // 1
    MM_DRIVERID_VIDEO_PATCH,                // 8
    MM_DRIVERID_VIDEO_DECODE_COMPBUFF,      // 1
    MM_DRIVERID_VIDEO_DECODE_MSVD,          // 20
    MM_DRIVERID_VIDEO_DECODE_CMD,           // 1
    MM_DRIVERID_VIDEO_DECODE_INS,           // 1

    // DIU
    MM_DRIVERID_VIDEO_PS1_ADDR0,
    MM_DRIVERID_VIDEO_PS2_ADDR0,
    MM_DRIVERID_VIDEO_SS1_ADDR0,
    MM_DRIVERID_VIDEO_SS2_ADDR0,
    MM_DRIVERID_VIDEO_SS1_MMIOUPDATE_ADDR0,
    MM_DRIVERID_VIDEO_SS2_MMIOUPDATE_ADDR0,
    MM_DRIVERID_VIDEO_SS1_MMIOFLIP_ADDR0,
    MM_DRIVERID_VIDEO_SS2_MMIOFLIP_ADDR0,

    //ISP
    MM_DRIVERID_ISP_VPP_SRC,
    MM_DRIVERID_ISP_VPP_DST,
    MM_DRIVERID_ISP_CAPTURE,

    // Cache invalidate
    MM_DRIVERID_INVALID_STREAMCACHE_VS_ADDRESS,
    MM_DRIVERID_INVALID_STREAMCACHE_PS_ADDRESS,
    MM_DRIVERID_INVALID_STREAMCACHE_CS_ADDRESS,

    MM_DRIVERID_INVALID_L2_CACHE_ADDRESS,
    MM_DRIVERID_INVALID_L2_CACHE_MASK,

    MM_DRIVERID_LAST,

} MM_DRIVERID_ELT;


typedef struct MM_ALLOCATION_DESC_ELT
{
    MM_ALLOCATION_DESC;
    unsigned int            HWCompressFormat;           // Compress type for HW    
}MM_ALLOCATION_DESC_ELT;

typedef struct
{
    unsigned int    bTileSupported;
    unsigned int    tileSizeInBytes;
    unsigned int    dwTileSizeX[10];        // E supports to 512 bpp
    unsigned int    dwTileSizeY[10];
} KMDTILEINFO_EXC, *PKMDTILEINFO_EXC;

static KMDTILEINFO_EXC hwKMDTileInfo_elt =
{
    TRUE,           // bTileSupported
    0x4000,         // tileSizeInBytes = 16KB
// dwTileSizeX
    0x200,          // dwTileSizeX -   1bit, E1 not support
    0xFFFFFFFF,     // dummy item for Nearestlog2()
    0x0,            // dwTileSizeX -   4bit, E1 not support
    0x80,           // dwTileSizeX -   8bit,
    0x80,           // dwTileSizeX -  16bit
    0x40,           // dwTileSizeX -  32bit
    0x40,           // dwTileSizeX -  64bit,
    0x20,           // dwTileSizeX - 128bit,
    0x20,           // dwTileSizeX - 256bit,
    0x10,           // dwTileSizeX - 512bit,
// dwTileSizeY
    0x100,          // dwTileSizeY -   1bit, E1 not support
    0xFFFFFFFF,     // dummy item for Nearestlog2()
    0x0,            // dwTileSizeY -   4bit, E1 not support
    0x80,           // dwTileSizeY -   8bit, 
    0x40,           // dwTileSizeY -  16bit
    0x40,           // dwTileSizeY -  32bit
    0x20,           // dwTileSizeY -  64bit,
    0x20,           // dwTileSizeX - 128bit,
    0x10,           // dwTileSizeX - 256bit,
    0x10,           // dwTileSizeX - 512bit,
};

/*---------------------------------------------------------------------------*/
/* IsPow2()                                                                  */
/*                                                                           */
/* Tests if a number is a power of 2                                         */
/*                                                                           */
/* FIXME - there must be a nicer way of doing this...                        */
/*---------------------------------------------------------------------------*/

static __inline__ int IsPow2(unsigned int value)
{
    return (!(value & (value-1)));
}

/*---------------------------------------------------------------------------*/
/* NearestLog2()                                                             */
/*                                                                           */
/* Finds the closest integer base-2 log of a value.                          */
/* (0 will return 0xFFFFFFFF)                                                */
/*---------------------------------------------------------------------------*/
static __inline__ unsigned int NearestLog2(unsigned int val)
{
    unsigned int result;
    unsigned int input;

    result = 0xFFFFFFFF;
    input = val;

    while (val & 0xFFFFFFF0)
    {
        val >>= 4;
        result += 4;
    }

    while (val)
    {
        val >>= 1;
        result++;
    }

    //
    // if val is not a pow2, increment by one, since
    // this function floors
    //

    if (IsPow2(input) == 0)
    {
        result++;
    }

    return(result);
}

#define TILE_SIZE_IN_BYTES (hwKMDTileInfo_elt.tileSizeInBytes)
#define TILE_SIZE_X(bpp) (hwKMDTileInfo_elt.dwTileSizeX[NearestLog2(bpp)])
#define TILE_SIZE_Y(bpp) (hwKMDTileInfo_elt.dwTileSizeY[NearestLog2(bpp)])
#define ALIGN_TO_X_TILE_SIZE(value,bpp) (((value) + TILE_SIZE_X(bpp) - 1) & ~(TILE_SIZE_X(bpp) - 1))
#define ALIGN_TO_Y_TILE_SIZE(value,bpp) (((value) + TILE_SIZE_Y(bpp) - 1) & ~(TILE_SIZE_Y(bpp) - 1))
#define NUM_OF_X_TILES(value,bpp) (ALIGN_TO_X_TILE_SIZE((value),(bpp)) / TILE_SIZE_X(bpp))
#define NUM_OF_Y_TILES(value,bpp) (ALIGN_TO_Y_TILE_SIZE((value),(bpp)) / TILE_SIZE_Y(bpp))


#endif

