//    Spec Last Modified Time: 2011-3-31 14:37:19
#ifndef _SURFACE_FORMAT_ELT_H
#define _SURFACE_FORMAT_ELT_H


#define SURFACE_FORMAT_TIMESTAMP  "2011-3-31 14:37:19"
typedef enum
{
    HSF_R4G4B4A4_UNORM                       = 0, 
    HSF_R5G5B5A1_UNORM                       = 1, 
    HSF_R5G6B5_UNORM                         = 2,   // D3DFMT_R5G6B5
    HSF_R8G8B8A8_UNORM                       = 3, 
    HSF_YUYV                                 = 4,   // No blendable/MSAA. But WBU will be able to convert pixel
    // YUV to packed YUV422 by averaging neighbourhood pixel's
    // UV. Use swizzle bit for UYVY format.
    HSF_L4A4_UNORM                           = 5,   // D3DFMT_A4L4 (for Video)
    HSF_R16_FLOAT                            = 6, 
    HSF_R32_FLOAT                            = 7,   // No blendable/MSAA.
    HSF_R16G16_FLOAT                         = 8, 
    HSF_R16G16B16A16_FLOAT                   = 9, 
    HSF_R32G32_FLOAT                         = 10,  // No blendable/MSAA.
    HSF_R32G32B32A32_FLOAT                   = 11,  // No blendable/MSAA.
    HSF_R8_UNORM                             = 12, 
    HSF_R8G8_UNORM                           = 13, 
    HSF_R10G10B10A2_UNORM                    = 14, 
    HSF_R11G11B10_FLOAT                      = 15, 
    HSF_UNKNOWN                              = 16, 
    HSF_R32G32B32A32_TYPELESS                = 17, 
    HSF_R32G32B32A32_UINT                    = 18, 
    HSF_R32G32B32A32_SINT                    = 19, 
    HSF_R32G32B32_TYPELESS                   = 20, 
    HSF_R32G32B32_FLOAT                      = 21, 
    HSF_R32G32B32_UINT                       = 22, 
    HSF_R32G32B32_SINT                       = 23, 
    HSF_R24_FLOAT                            = 24, 
    HSF_R16G16B16A16_UNORM                   = 25, 
    HSF_R16G16B16A16_UINT                    = 26, 
    HSF_R16G16B16A16_SNORM                   = 27, 
    HSF_R16G16B16A16_SINT                    = 28, 
    HSF_R32G32_TYPELESS                      = 29, 
    HSF_R32G32_UINT                          = 30, 
    HSF_R32G32_SINT                          = 31, 
    HSF_R10G10B10A2_TYPELESS                 = 32, 
    HSF_B10G10R10X2_UNORM                    = 33, 
    HSF_R10G10B10A2_UINT                     = 34, 
    HSF_R8G8B8A8_TYPELESS                    = 35, 
    HSF_R8G8B8A8_UNORM_SRGB                  = 36, 
    HSF_R8G8B8A8_UINT                        = 37, 
    HSF_R8G8B8A8_SNORM                       = 38, 
    HSF_R8G8B8A8_SINT                        = 39, 
    HSF_D24_UNORM                            = 40, 
    HSF_R16G16_UNORM                         = 41, 
    HSF_R16G16_UINT                          = 42, 
    HSF_R16G16_SNORM                         = 43, 
    HSF_R16G16_SINT                          = 44, 
    HSF_R32_TYPELESS                         = 45, 
    HSF_R32_UINT                             = 46, 
    HSF_R32_SINT                             = 47, 
    HSF_BAYER_12                             = 48,  // ISP format, 12-bit per channel
    HSF_R8G8_UINT                            = 49, 
    HSF_R8G8_SNORM                           = 50, 
    HSF_R8G8_SINT                            = 51, 
    HSF_R16_TYPELESS                         = 52, 
    HSF_D16_UNORM                            = 53, 
    HSF_R16_UNORM                            = 54, 
    HSF_R16_UINT                             = 55, 
    HSF_R16_SNORM                            = 56, 
    HSF_R16_SINT                             = 57, 
    HSF_A4L4_UNORM                           = 58, 
    HSF_R8_UINT                              = 59, 
    HSF_R8_SNORM                             = 60, 
    HSF_R8_SINT                              = 61, 
    HSF_A8_UNORM                             = 62, 
    HSF_B10G11R11_FLOAT                      = 63, 
    HSF_R9G9B9E5_SHAREDEXP                   = 64, 
    HSF_BC1_UNORM                            = 65, 
    HSF_BC1_UNORM_SRGB                       = 66, 
    HSF_BC2_UNORM                            = 67, 
    HSF_BC2_UNORM_SRGB                       = 68, 
    HSF_BC3_UNORM                            = 69, 
    HSF_BC3_UNORM_SRGB                       = 70,  // Belows are DX9 legacy formats
    HSF_B8G8R8A8_UNORM                       = 71,  // D3DFMT_A8R8G8B8
    HSF_B8G8R8X8_UNORM                       = 72,  // D3DFMT_X8R8G8B8
    HSF_R8G8B8X8_UNORM                       = 73,  // D3DFMT_X8B8G8R8
    HSF_B10G10R10A2_UNORM                    = 74,  // D3DFMT_A2R10G10B10
    HSF_B5G5R5X1_UNORM                       = 75,  // D3DFMT_X1R5G5B5
    HSF_B5G5R5A1_UNORM                       = 76,  // D3DFMT_A1R5G5B5
    HSF_B4G4R4A4_UNORM                       = 77,  // D3DFMT_A4R4G4B4 
    HSF_B4G4R4X4_UNORM                       = 78,  // D3DFMT_X4R4G4B4
    HSF_U5V5L6_XNORM                         = 79,  // D3DFMT_L6V5U5
    HSF_L16_UNORM                            = 80,  // D3DFMT_L16
    HSF_L8A8_UNORM                           = 81,  // Used in 3DMark06. R=G=B=L
    HSF_L8_UNORM                             = 82,  // D3DFMT_L8  
    HSF_BAYER                                = 83,  // ISP format, 8-bit or 10-bit per channel
    HSF_B8G8R8A8_UNORM_SRGB                  = 84, 
    HSF_B8G8R8X8_UNORM_SRGB                  = 85, 
    HSF_R16G16B16A16_USCALE                  = 86, 
    HSF_R16G16B16A16_SSCALE                  = 87,  // STMDECLTYPE_SHORT4
    HSF_R10G10B10X2_UINT                     = 88,  // D3DDECLTYPE_UDEC3
    HSF_R10G10B10X2_SNORM                    = 89,  // D3DDECLTYPE_DEC3N
    HSF_R10G10B10A2_SNORM                    = 90,  // D3DFMT_A2W10V10U10
    HSF_B8G8R8A8_XNORM                       = 91,  // D3DFMT_X8L8U8V8
    HSF_R16G16_USCALE                        = 92, 
    HSF_R16G16_SSCALE                        = 93,  // STMDECLTYPE_SHORT2
    HSF_R8G8B8A8_USCALE                      = 94,  // STMDECLTYPE_UBYTE4
    HSF_R8G8B8A8_SSCALE                      = 95, 
    HSF_R10G10B10X2_USCALE                   = 96,  // STMDECLTYPE_UDEC3
    HSF_R16_USCALE                           = 97, 
    HSF_R16_SSCALE                           = 98, 
    HSF_R8G8_USCALE                          = 99, 
    HSF_R8G8_SSCALE                          = 100, 
    HSF_R8_USCALE                            = 101, 
    HSF_R8_SSCALE                            = 102, 
    HSF_R32G32B32A32_FIX                     = 103, 
    HSF_R32G32B32_FIX                        = 104, 
    HSF_R32G32_FIX                           = 105, 
    HSF_R32_FIX                              = 106, 
    HSF_A2B10G10R10_UNORM                    = 107, 
    HSF_A2B10G10R10_SNORM                    = 108, 
    HSF_X8R8G8B8_UNORM                       = 109, 
    HSF_X8B8G8R8_UNORM                       = 110, 
    HSF_A8R8G8B8_UNORM                       = 111, 
    HSF_A8B8G8R8_UNORM                       = 112, 
    HSF_A1R5G5B5_UNORM                       = 113, 
    HSF_A1B5G5R5_UNORM                       = 114, 
    HSF_B5G6R5_UNORM                         = 115, 
    HSF_A4R4G4B4_UNORM                       = 116, 
    HSF_A4B4G4R4_UNORM                       = 117, 
    HSF_G8_UINT                              = 118, // Used for Stencil
    HSF_YCRCB_MB_8_420                       = 119, 
    HSF_NV12                                 = 120, // planar video format, only linear
    HSF_YV12                                 = 121, // planar video format, only linear
    HSF_Y216_VIDEO                           = 122, // packed video format
    HSF_Y210_VIDEO                           = 123, // packed video format
    HSF_AYUV_VIDEO                           = 124, // packed video format
    HSF_A2B10G10R10_USCALE                   = 125, 
    HSF_A2B10G10R10_SSCALE                   = 126, 
    HSF_UYVY                                 = 127, // TU can use
} Hw_Surf_Format;

#define BIT_COUNT_TABLE \
{\
    16,  /*HSF_R4G4B4A4_UNORM                       = 0 */\
    16,  /*HSF_R5G5B5A1_UNORM                       = 1 */\
    16,  /*HSF_R5G6B5_UNORM                         = 2 */\
    32,  /*HSF_R8G8B8A8_UNORM                       = 3,*/\
    16,  /*HSF_YUYV                                 = 4,*/\
    8,   /*HSF_L4A4_UNORM                           = 5,*/\
    16,  /*HSF_R16_FLOAT                            = 6,*/\
    32,  /*HSF_R32_FLOAT                            = 7,*/\
    32,  /*HSF_R16G16_FLOAT                         = 8,*/\
    64,  /*HSF_R16G16B16A16_FLOAT                   = 9,*/\
    64,  /*HSF_R32G32_FLOAT                         = 10,*/\
    128, /*HSF_R32G32B32A32_FLOAT                   = 11,*/\
    8,   /*HSF_R8_UNORM                             = 12,*/\
    16,  /*HSF_R8G8_UNORM                           = 13,*/\
    32,  /*HSF_R10G10B10A2_UNORM                    = 14,*/\
    32,  /*HSF_R11G11B10_FLOAT                      = 15,*/\
    8,   /*HSF_UNKNOWN                              = 16,*/\
    128, /*HSF_R32G32B32A32_TYPELESS                = 17,*/\
    128, /*HSF_R32G32B32A32_UINT                    = 18,*/\
    128, /*HSF_R32G32B32A32_SINT                    = 19,*/\
    96,  /*HSF_R32G32B32_TYPELESS                   = 20, 96bpp is Treated as 128bpp in tiled format */\
    96,  /*HSF_R32G32B32_FLOAT                      = 21,*/\
    96,  /*HSF_R32G32B32_UINT                       = 22,*/\
    96,  /*HSF_R32G32B32_SINT                       = 23,*/\
    32,  /*HSF_R24_FLOAT                            = 24,*/\
    64,  /*HSF_R16G16B16A16_UNORM                   = 25,*/\
    64,  /*HSF_R16G16B16A16_UINT                    = 26,*/\
    64,  /*HSF_R16G16B16A16_SNORM                   = 27,*/\
    64,  /*HSF_R16G16B16A16_SINT                    = 28,*/\
    64,  /*HSF_R32G32_TYPELESS                      = 29,*/\
    64,  /*HSF_R32G32_UINT                          = 30,*/\
    64,  /*HSF_R32G32_SINT                          = 31,*/\
    32,  /*HSF_R10G10B10A2_TYPELESS                 = 32,*/\
    32,  /*HSF_B10G10R10X2_UNORM                    = 33,*/\
    32,  /*HSF_R10G10B10A2_UINT                     = 34,*/\
    32,  /*HSF_R8G8B8A8_TYPELESS                    = 35,*/\
    32,  /*HSF_R8G8B8A8_UNORM_SRGB                  = 36,*/\
    32,  /*HSF_R8G8B8A8_UINT                        = 37,*/\
    32,  /*HSF_R8G8B8A8_SNORM                       = 38,*/\
    32,  /*HSF_R8G8B8A8_SINT                        = 39,*/\
    32,  /*HSF_D24_UNORM                            = 40,*/\
    32,  /*HSF_R16G16_UNORM                         = 41,*/\
    32,  /*HSF_R16G16_UINT                          = 42,*/\
    32,  /*HSF_R16G16_SNORM                         = 43,*/\
    32,  /*HSF_R16G16_SINT                          = 44,*/\
    32,  /*HSF_R32_TYPELESS                         = 45,*/\
    32,  /*HSF_R32_UINT                             = 46,*/\
    32,  /*HSF_R32_SINT                             = 47,*/\
    16,  /*HSF_BAYER_12                             = 48,*/\
    16,  /*HSF_R8G8_UINT                            = 49,*/\
    16,  /*HSF_R8G8_SNORM                           = 50,*/\
    16,  /*HSF_R8G8_SINT                            = 51,*/\
    16,  /*HSF_R16_TYPELESS                         = 52,*/\
    16,  /*HSF_D16_UNORM                            = 53,*/\
    16,  /*HSF_R16_UNORM                            = 54,*/\
    16,  /*HSF_R16_UINT                             = 55,*/\
    16,  /*HSF_R16_SNORM                            = 56,*/\
    16,  /*HSF_R16_SINT                             = 57,*/\
    8,   /*HSF_A4L4_UNORM                           = 58,*/\
    8,   /*HSF_R8_UINT                              = 59,*/\
    8,   /*HSF_R8_SNORM                             = 60,*/\
    8,   /*HSF_R8_SINT                              = 61,*/\
    8,   /*HSF_A8_UNORM                             = 62,*/\
    32,  /*HSF_B10G11R11_FLOAT                      = 63,*/\
    32,  /*HSF_R9G9B9E5_SHAREDEXP                   = 64,*/\
    4,   /*HSF_BC1_UNORM                            = 65,*/\
    4,   /*HSF_BC1_UNORM_SRGB                       = 66,*/\
    8,   /*HSF_BC2_UNORM                            = 67,*/\
    8,   /*HSF_BC2_UNORM_SRGB                       = 68,*/\
    8,   /*HSF_BC3_UNORM                            = 69,*/\
    8,   /*HSF_BC3_UNORM_SRGB                       = 70,*/\
    32,  /*HSF_B8G8R8A8_UNORM                       = 71,D3DFMT_A8R8G8B8 (DX9 legacy formats)   */\
    32,  /*HSF_B8G8R8X8_UNORM                       = 72,D3DFMT_X8R8G8B8 (DX9 legacy formats)   */\
    32,  /*HSF_R8G8B8X8_UNORM                       = 73,D3DFMT_X8B8G8R8 (DX9 legacy formats)   */\
    32,  /*HSF_B10G10R10A2_UNORM                    = 74,D3DFMT_A2R10G10B10(DX9 legacy formats) */\
    16,  /*HSF_B5G5R5X1_UNORM                       = 75,D3DFMT_X1R5G5B5 (DX9 legacy formats)   */\
    16,  /*HSF_B5G5R5A1_UNORM                       = 76,D3DFMT_A1R5G5B5 (DX9 legacy formats)   */\
    16,  /*HSF_B4G4R4A4_UNORM                       = 77,D3DFMT_A4R4G4B4 (DX9 legacy formats)   */\
    16,  /*HSF_B4G4R4X4_UNORM                       = 78,D3DFMT_X4R4G4B4 (DX9 legacy formats)   */\
    16,  /*HSF_U5V5L6_XNORM                         = 79,D3DFMT_L6V5U5   (DX9 legacy formats) */\
    16,  /*HSF_L16_UNORM                            = 80,D3DFMT_L16      (DX9 legacy formats) */\
    16,  /*HSF_L8A8_UNORM                           = 81,*/\
    8,   /*HSF_L8_UNORM                             = 82,D3DFMT_L8       (DX9 legacy formats) */\
    16,  /*HSF_BAYER                                = 83,*/\
    32,  /*HSF_B8G8R8A8_UNORM_SRGB                  = 84,*/\
    32,  /*HSF_B8G8R8X8_UNORM_SRGB                  = 85,*/\
    64,  /*HSF_R16G16B16A16_USCALE                  = 86,*/\
    64,  /*HSF_R16G16B16A16_SSCALE                  = 87,*/\
    32,  /*HSF_R10G10B10X2_UINT                     = 88,D3DDECLTYPE_UDEC3 */\
    32,  /*HSF_R10G10B10X2_SNORM                    = 89,D3DDECLTYPE_DEC3N */\
    32,  /*HSF_R10G10B10A2_SNORM                    = 90,D3DFMT_A2W10V10U10 */\
    32,  /*HSF_B8G8R8A8_XNORM                       = 91,D3DFMT_X8L8U8V8    */\
    32,  /*HSF_R16G16_USCALE                        = 92,*/\
    32,  /*HSF_R16G16_SSCALE                        = 93,*/\
    32,  /*HSF_R8G8B8A8_USCALE                      = 94,STMDECLTYPE_UBYTE4 */\
    32,  /*HSF_R8G8B8A8_SSCALE                      = 95,*/\
    32,  /*HSF_R10G10B10X2_USCALE                   = 96,STMDECLTYPE_UDEC3  */\
    16,  /*HSF_R16_USCALE                           = 97,*/\
    16,  /*HSF_R16_SSCALE                           = 98,*/\
    16,  /*HSF_R8G8_USCALE                          = 99,*/\
    16,  /*HSF_R8G8_SSCALE                          = 100,*/\
    8,   /*HSF_R8_USCALE                            = 101,*/\
    8,   /*HSF_R8_SSCALE                            = 102,*/\
    128, /*HSF_R32G32B32A32_FIX                     = 103,*/\
    96,  /*HSF_R32G32B32_FIX                        = 104,*/\
    64,  /*HSF_R32G32_FIX                           = 105,*/\
    32,  /*HSF_R32_FIX                              = 106,*/\
    32,  /*HSF_A2B10G10R10_UNORM                    = 107,*/\
    32,  /*HSF_A2B10G10R10_SNORM                    = 108,*/\
    32,  /*HSF_X8R8G8B8_UNORM                       = 109,*/\
    32,  /*HSF_X8B8G8R8_UNORM                       = 110,*/\
    32,  /*HSF_A8R8G8B8_UNORM                       = 111,*/\
    32,  /*HSF_A8B8G8R8_UNORM                       = 112,*/\
    16,  /*HSF_A1R5G5B5_UNORM                       = 113,*/\
    16,  /*HSF_A1B5G5R5_UNORM                       = 114,*/\
    16,  /*HSF_B5G6R5_UNORM                         = 115,D3DFMT_R5G6B5   (DX9 legacy formats)   */\
    16,  /*HSF_A4R4G4B4_UNORM                       = 116,*/\
    16,  /*HSF_A4B4G4R4_UNORM                       = 117,*/\
    8,   /*HSF_G8_UINT                              = 118,Used for Stencil*/\
    8,   /*HSF_YCRCB_MB_8_420                       = 119,*/\
    8,   /*HSF_NV12                                 = 120,*/\
    16,  /*HSF_YV12                                 = 121,*/\
    32,  /*HSF_Y216_VIDEO                           = 122,Y216*/\
    32,  /*HSF_Y210_VIDEO                           = 123,Y210*/\
    32,  /*HSF_AYUV_VIDEO                           = 124,*/\
    32,  /*HSF_A2B10G10R10_USCALE                   = 125,*/\
    32,  /*HSF_A2B10G10R10_SSCALE                   = 126,*/\
    16,  /*HSF_UYVY                                 = 127,*/\
    }\

#define SURFACE_FORMAT_MODE \
{\
    8,  /*HSF_R4G4B4A4_UNORM                       = 0,*/\
    8,  /*HSF_R5G5B5A1_UNORM                       = 1,*/\
    8,  /*HSF_R5G6B5_UNORM                         = 2,*/\
    8,  /*HSF_R8G8B8A8_UNORM                       = 3,*/\
    8,  /*HSF_YUYV                                 = 4,*/\
    8,  /*HSF_L4A4_UNORM                           = 5,*/\
    1,  /*HSF_R16_FLOAT                            = 6,*/\
    0,  /*HSF_R32_FLOAT                            = 7,*/\
    1,  /*HSF_R16G16_FLOAT                         = 8,*/\
    1,  /*HSF_R16G16B16A16_FLOAT                   = 9,*/\
    0,  /*HSF_R32G32_FLOAT                         = 10,*/\
    12,  /*HSF_R32G32B32A32_FLOAT                  = 11,*/\
    8,  /*HSF_R8_UNORM                             = 12,*/\
    8,  /*HSF_R8G8_UNORM                           = 13,*/\
    7,  /*HSF_R10G10B10A2_UNORM                    = 14,*/\
    1,  /*HSF_R11G11B10_FLOAT                      = 15,*/\
    0,  /*HSF_UNKNOWN                              = 16,*/\
    4,  /*HSF_R32G32B32A32_TYPELESS                = 17,*/\
    4,  /*HSF_R32G32B32A32_UINT                    = 18,*/\
    2,  /*HSF_R32G32B32A32_SINT                    = 19,*/\
    0,  /*HSF_R32G32B32_TYPELESS                   = 20,*/\
    0,  /*HSF_R32G32B32_FLOAT                      = 21,*/\
    4,  /*HSF_R32G32B32_UINT                       = 22,*/\
    2,  /*HSF_R32G32B32_SINT                       = 23,*/\
    0,  /*HSF_R24_FLOAT                            = 24,*/\
    6,  /*HSF_R16G16B16A16_UNORM                   = 25,*/\
    5,  /*HSF_R16G16B16A16_UINT                    = 26,*/\
    0,  /*HSF_R16G16B16A16_SNORM                   = 27,*/\
    3,  /*HSF_R16G16B16A16_SINT                    = 28,*/\
    4,  /*HSF_R32G32_TYPELESS                      = 29,*/\
    4,  /*HSF_R32G32_UINT                          = 30,*/\
    2,  /*HSF_R32G32_SINT                          = 31,*/\
    7,  /*HSF_R10G10B10A2_TYPELESS                 = 32,*/\
    7,  /*HSF_B10G10R10X2_UNORM                    = 33,*/\
    5,  /*HSF_R10G10B10A2_UINT                     = 34,*/\
    0,  /*HSF_R8G8B8A8_TYPELESS                    = 35,*/\
    8,  /*HSF_R8G8B8A8_UNORM_SRGB                  = 36,*/\
    10, /*HSF_R8G8B8A8_UINT                        = 37,*/\
    0,  /*HSF_R8G8B8A8_SNORM                       = 38,*/\
    9,  /*HSF_R8G8B8A8_SINT                        = 39,*/\
    11,  /*HSF_D24_UNORM                           = 40,*/\
    6,  /*HSF_R16G16_UNORM                         = 41,*/\
    5,  /*HSF_R16G16_UINT                          = 42,*/\
    0,  /*HSF_R16G16_SNORM                         = 43,*/\
    3,  /*HSF_R16G16_SINT                          = 44,*/\
    4,  /*HSF_R32_TYPELESS                         = 45,*/\
    4,  /*HSF_R32_UINT                             = 46,*/\
    2,  /*HSF_R32_SINT                             = 47,*/\
    0,  /*HSF_BAYER_12                             = 48,*/\
    10, /*HSF_R8G8_UINT                            = 49,*/\
    0,  /*HSF_R8G8_SNORM                           = 50,*/\
    9, /*HSF_R8G8_SINT                             = 51,*/\
    6,  /*HSF_R16_TYPELESS                         = 52,*/\
    0,  /*HSF_D16_UNORM                            = 53,*/\
    6,  /*HSF_R16_UNORM                            = 54,*/\
    5,  /*HSF_R16_UINT                             = 55,*/\
    0,  /*HSF_R16_SNORM                            = 56,*/\
    3,  /*HSF_R16_SINT                             = 57,*/\
    8,  /*HSF_A4L4_UNORM                           = 58,*/\
    10, /*HSF_R8_UINT                              = 59,*/\
    0,  /*HSF_R8_SNORM                             = 60,*/\
    9,  /*HSF_R8_SINT                              = 61,*/\
    8,  /*HSF_A8_UNORM                             = 62,*/\
    0,  /*HSF_B10G11R11_FLOAT                      = 63,*/\
    0,  /*HSF_R9G9B9E5_SHAREDEXP                   = 64,*/\
    0,  /*HSF_BC1_UNORM                            = 65,*/\
    0,  /*HSF_BC1_UNORM_SRGB                       = 66,*/\
    0,  /*HSF_BC2_UNORM                            = 67,*/\
    0,  /*HSF_BC2_UNORM_SRGB                       = 68,*/\
    0,  /*HSF_BC3_UNORM                            = 69,*/\
    0,  /*HSF_BC3_UNORM_SRGB                       = 70,*/\
    8,  /*HSF_B8G8R8A8_UNORM                       = 71,*/\
    8,  /*HSF_B8G8R8X8_UNORM                       = 72,*/\
    8,  /*HSF_R8G8B8X8_UNORM                       = 73,*/\
    7,  /*HSF_B10G10R10A2_UNORM                    = 74,*/\
    8,  /*HSF_B5G5R5X1_UNORM                       = 75,*/\
    8,  /*HSF_B5G5R5A1_UNORM                       = 76,*/\
    8,  /*HSF_B4G4R4A4_UNORM                       = 77,*/\
    8,  /*HSF_B4G4R4X4_UNORM                       = 78,*/\
    0,  /*HSF_U5V5L6_XNORM                         = 79,*/\
    6,  /*HSF_L16_UNORM                            = 80,*/\
    8,  /*HSF_L8A8_UNORM                           = 81,*/\
    8,  /*HSF_L8_UNORM                             = 82,*/\
    0,  /*HSF_BAYER                                = 83,*/\
    8,  /*HSF_B8G8R8A8_UNORM_SRGB                  = 84,*/\
    8,  /*HSF_B8G8R8X8_UNORM_SRGB                  = 85,*/\
    0,  /*HSF_R16G16B16A16_USCALE                  = 86,*/\
    0,  /*HSF_R16G16B16A16_SSCALE                  = 87,*/\
    5,  /*HSF_R10G10B10X2_UINT                     = 88,*/\
    0,  /*HSF_R10G10B10X2_SNORM                    = 89,*/\
    0,  /*HSF_R10G10B10A2_SNORM                    = 90,*/\
    0,  /*HSF_B8G8R8A8_XNORM                       = 91,*/\
    0,  /*HSF_R16G16_USCALE                        = 92,*/\
    0,  /*HSF_R16G16_SSCALE                        = 93,*/\
    0,  /*HSF_R8G8B8A8_USCALE                      = 94,*/\
    0,  /*HSF_R8G8B8A8_SSCALE                      = 95,*/\
    0,  /*HSF_R10G10B10X2_USCALE                   = 96,*/\
    0,  /*HSF_R16_USCALE                           = 97,*/\
    0,  /*HSF_R16_SSCALE                           = 98,*/\
    0,  /*HSF_R8G8_USCALE                          = 99,*/\
    0,  /*HSF_R8G8_SSCALE                          = 100,*/\
    0,  /*HSF_R8_USCALE                            = 101,*/\
    0,  /*HSF_R8_SSCALE                            = 102,*/\
    0,  /*HSF_R32G32B32A32_FIX                     = 103,*/\
    0,  /*HSF_R32G32B32_FIX                        = 104,*/\
    0,  /*HSF_R32G32_FIX                           = 105,*/\
    0,  /*HSF_R32_FIX                              = 106,*/\
    7,  /*HSF_A2B10G10R10_UNORM                    = 107,*/\
    0,  /*HSF_A2B10G10R10_SNORM                    = 108,*/\
    8,  /*HSF_X8R8G8B8_UNORM                       = 109,*/\
    8,  /*HSF_X8B8G8R8_UNORM                       = 110,*/\
    8,  /*HSF_A8R8G8B8_UNORM                       = 111,*/\
    8,  /*HSF_A8B8G8R8_UNORM                       = 112,*/\
    8,  /*HSF_A1R5G5B5_UNORM                       = 113,*/\
    8,  /*HSF_A1B5G5R5_UNORM                       = 114,*/\
    8,  /*HSF_B5G6R5_UNORM                         = 115,*/\
    8,  /*HSF_A4R4G4B4_UNORM                       = 116,*/\
    8,  /*HSF_A4B4G4R4_UNORM                       = 117,*/\
    10, /*HSF_G8_UINT                              = 118,*/\
    0,  /*HSF_YCRCB_MB_8_420                       = 119,*/\
    0,  /*HSF_NV12                                 = 120,*/\
    0,  /*HSF_YV12                                 = 121,*/\
    0,  /*HSF_RESERVED8                            = 122,*/\
    0,  /*HSF_RESERVED9                            = 123,*/\
    0,  /*HSF_AYUV_VIDEO                           = 124,*/\
    0,  /*HSF_A2B10G10R10_USCALE                     = 125,*/\
    0,  /*HSF_A2B10G10R10_SSCALE                     = 126,*/\
    8,  /*HSF_UYVY                                 = 127,*/\
}

// NOTE: the last bit "1" represents stencil; Other 2-4 bit represent different Depth format
typedef enum                   
{
    DS_NONE                = 0,                    // Nature D/S fmt       // Compatible D/S fmt
    S8_UINT                = 0x1,
    D16_UNORM              = 0x2,                  // D16_UNORM,           R16_TYPELESS,   R16_UNORM
    D24_UNORM              = 0x4,                  // D24_UNORM,
} DEPTH_STENCIL_FORMAT;

#define CHANNEL_MASK_R              1
#define CHANNEL_MASK_G              2
#define CHANNEL_MASK_B              4
#define CHANNEL_MASK_A              8

typedef enum CHANNEL_MASK
{
    C_NONE                 = 0,
    C_R                    = CHANNEL_MASK_R,
    C_A                    = CHANNEL_MASK_A,
    C_GR                   = CHANNEL_MASK_G | CHANNEL_MASK_R,
    C_BGR                  = CHANNEL_MASK_B | CHANNEL_MASK_G | CHANNEL_MASK_R,
    C_BGRA                 = CHANNEL_MASK_B | CHANNEL_MASK_G | CHANNEL_MASK_R | CHANNEL_MASK_A,
} CHANNEL_MASK;

typedef enum COMPRESS_HWFORMAT
{
    CP_NCMP         = 0,
    CP_RGBX8888     = 1, 
    CP_RGBA8888     = 2,
    CP_XRGB8888     = 3,
    CP_ARGB8888     = 4,
    CP_RGBA1010102  = 5,
    CP_ABGR2101010  = 6,
    CP_RGB111110    = 7,
    CP_RGB565       = 8,
    CP_YCrCb        = 9,
    CP_YUYV         = 10,
    CP_UYUV         = 11,
    CP_D16          = 12,
    CP_D24          = 13,
    CP_BAYER        = 14,
    CP_OPTIONAL     = 15,
}COMPRESS_HWFORMAT;

typedef enum COMPRESS_MXUFORMAT
{
    RANGE_OFF          = 0,
    RGBAX8888_TILE     = 1,
    RGBAX8888_LINEAR   = 2,
    XARGB8888_TILE     = 3,
    XARGB8888_LINEAR   = 4,
    R10G10B10A2_LINEAR = 5,
    A2B10G10R10_LINEAR = 6,
    R11G11B10_LINEAR   = 7,
    R5G6B5_TILE        = 8,
    YCRCB_MB_8_420     = 9,
    YUYV_LINEAR        = 10,
    UYVY_LINEAR        = 11,
    Z16                = 12,
    Z24                = 13,
    BAYER              = 14,
    RANGE_RESERVED     = 15,
}COMPRESS_MXUFORMAT;

typedef struct HWFORMAT_TABLE_ENTRY_ELT
{
    Hw_Surf_Format        MappedDstFormat;
    Hw_Surf_Format        SuppressAlphaFormat;
    DEPTH_STENCIL_FORMAT  CompatibleDsFmt;
    COMPRESS_HWFORMAT     CompressFmt;
    BYTE                  ChannelMask;
    BYTE                  bRtSupport;             // Can this format be used as RT
    BYTE                  bRBSwizzledAfterMap;    // Compare with mapped format RB channle is swizzled
    BYTE                  bASwizzledAfterMap;     // Compare with mapped format A  channle is swizzled
    BYTE                  bBlockCompressed;       // Is this format block compressed
    BYTE                  bYUY2;                  // Is this format YUY2
    BYTE                  b96bpp;                 // Is this format 96bpp
    BYTE                  bUnorm;                 // Is this format UNORM
    BYTE                  bSnorm;                 // Is this format SNORM
    BYTE                  bSrgb;                  // Is this format SRGB
    BYTE                  bTypeless;              // Is this format typeless
} HWFORMAT_TABLE_ENTRY_ELT;

static const  HWFORMAT_TABLE_ENTRY_ELT hw_format_table[] =
{                                                                                                                                                                  
    //                                                                                ChannelMask                                                                      
    //                                                                                |       bRtSupport                                                               
    //                                                                                |       |   bRBSwizzledAfterMap                                                  
    //                                                                                |       |   |   bASwizzledAfterMap                                               
    //                                                                                |       |   |   |   bBlockCompressed                                             
    //                                                                                |       |   |   |   |   bYUY2                                                    
    //                                                                                |       |   |   |   |   |   b96bpp                                               
    //                                                                                |       |   |   |   |   |   |   bUnorm                                           
    //                                                                                |       |   |   |   |   |   |   |   bSnorm                                       
    //                                                                                |       |   |   |   |   |   |   |   |   bSrgb                                    
    //  MappedDstFormat         SuppressAlphaFormat   CompatibleDsFmt CompressFmt     |       |   |   |   |   |   |   |   |   |   bTypeless                            
    //  |                       |                     |               |               |       |   |   |   |   |   |   |   |   |   |                                    
    {HSF_R4G4B4A4_UNORM,     HSF_UNKNOWN,          DS_NONE,        CP_RGBA8888,    C_BGRA, 1,  0,  0,  0,  0,  0,  1,  0,  0,  0},/*HSF_R4G4B4A4_UNORM       = 0,*/  
    {HSF_R5G5B5A1_UNORM,     HSF_UNKNOWN,          DS_NONE,        CP_NCMP,        C_BGRA, 1,  0,  0,  0,  0,  0,  1,  0,  0,  0},/*HSF_R5G5B5A1_UNORM       = 1,*/  
    {HSF_R5G6B5_UNORM,       HSF_UNKNOWN,          DS_NONE,        CP_RGB565,      C_BGR,  1,  0,  0,  0,  0,  0,  1,  0,  0,  0},/*HSF_R5G6B5_UNORM         = 2,*/  
    {HSF_R8G8B8A8_UNORM,     HSF_R8G8B8X8_UNORM,   DS_NONE,        CP_RGBA8888,    C_BGRA, 1,  0,  0,  0,  0,  0,  1,  0,  0,  0},/*HSF_R8G8B8A8_UNORM       = 3,*/  
    {HSF_YUYV,               HSF_UNKNOWN,          DS_NONE,        CP_YUYV,        C_BGRA, 1,  0,  0,  0,  1,  0,  0,  0,  0,  0},/*HSF_YUYV                 = 4,*/  
    {HSF_L4A4_UNORM,         HSF_UNKNOWN,          DS_NONE,        CP_RGBA8888,    C_BGRA, 1,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_L4A4_UNORM           = 5,*/  
    {HSF_R16_FLOAT,          HSF_UNKNOWN,          DS_NONE,        CP_D16,         C_R,    1,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R16_FLOAT            = 6,*/  
    {HSF_R32_FLOAT,          HSF_UNKNOWN,          D16_UNORM,      CP_RGBA8888,    C_R,    1,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R32_FLOAT            = 7,*/  
    {HSF_R16G16_FLOAT,       HSF_UNKNOWN,          DS_NONE,        CP_RGBA8888,    C_GR,   1,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R16G16_FLOAT         = 8,*/  
    {HSF_R16G16B16A16_FLOAT, HSF_UNKNOWN,          DS_NONE,        CP_RGBA8888,    C_BGRA, 1,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R16G16B16A16_FLOAT   = 9,*/  
    {HSF_R32G32_FLOAT,       HSF_UNKNOWN,          DS_NONE,        CP_RGBA8888,    C_GR,   1,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R32G32_FLOAT         = 10,*/ 
    {HSF_R32G32B32A32_FLOAT, HSF_UNKNOWN,          DS_NONE,        CP_RGBA8888,    C_BGRA, 1,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R32G32B32A32_FLOAT   = 11,*/ 
    {HSF_R8_UNORM,           HSF_UNKNOWN,          DS_NONE,        CP_RGBA8888,    C_R,    1,  0,  0,  0,  0,  0,  1,  0,  0,  0},/*HSF_R8_UNORM             = 12,*/ 
    {HSF_R8G8_UNORM,         HSF_UNKNOWN,          DS_NONE,        CP_RGBA8888,    C_GR,   1,  0,  0,  0,  0,  0,  1,  0,  0,  0},/*HSF_R8G8_UNORM           = 13,*/ 
    {HSF_R10G10B10A2_UNORM,  HSF_UNKNOWN,          DS_NONE,        CP_RGBA1010102, C_BGRA, 1,  0,  0,  0,  0,  0,  1,  0,  0,  0},/*HSF_R10G10B10A2_UNORM    = 14,*/ 
    {HSF_R11G11B10_FLOAT,    HSF_UNKNOWN,          DS_NONE,        CP_RGB111110,   C_BGR,  1,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R11G11B10_FLOAT      = 15,*/ 
    {HSF_UNKNOWN,            HSF_UNKNOWN,          DS_NONE,        CP_NCMP,        C_NONE, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_UNKNOWN              = 16,*/ 
    {HSF_R32G32B32A32_FLOAT, HSF_UNKNOWN,          DS_NONE,        CP_NCMP,        C_BGRA, 0,  0,  0,  0,  0,  0,  0,  0,  0,  1},/*HSF_R32G32B32A32_TYPELESS= 17,*/ 
    {HSF_R32G32B32A32_FLOAT, HSF_UNKNOWN,          DS_NONE,        CP_RGBA8888,    C_BGRA, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R32G32B32A32_UINT    = 18,*/ 
    {HSF_R32G32B32A32_FLOAT, HSF_UNKNOWN,          DS_NONE,        CP_RGBA8888,    C_BGRA, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R32G32B32A32_SINT    = 19,*/ 
    {HSF_R32G32B32A32_FLOAT, HSF_UNKNOWN,          DS_NONE,        CP_NCMP,        C_BGR,  0,  0,  0,  0,  0,  1,  0,  0,  0,  1},/*HSF_R32G32B32_TYPELESS   = 20,*/ 
    {HSF_R32G32B32A32_FLOAT, HSF_UNKNOWN,          DS_NONE,        CP_RGBA8888,    C_BGR,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0},/*HSF_R32G32B32_FLOAT      = 21,*/ 
    {HSF_R32G32B32A32_FLOAT, HSF_UNKNOWN,          DS_NONE,        CP_RGBA8888,    C_BGR,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0},/*HSF_R32G32B32_UINT       = 22,*/ 
    {HSF_R32G32B32A32_FLOAT, HSF_UNKNOWN,          DS_NONE,        CP_RGBA8888,    C_BGR,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0},/*HSF_R32G32B32_SINT       = 23,*/ 
    {HSF_R16G16B16A16_FLOAT, HSF_UNKNOWN,          DS_NONE,        CP_NCMP,        C_BGRA, 0,  0,  0,  0,  0,  0,  0,  0,  0,  1},/*HSF_R16G16B16A16_TYPELESS= 24,*/ 
    {HSF_R16G16B16A16_FLOAT, HSF_UNKNOWN,          DS_NONE,        CP_RGBA8888,    C_BGRA, 0,  0,  0,  0,  0,  0,  1,  0,  0,  0},/*HSF_R16G16B16A16_UNORM   = 25,*/ 
    {HSF_R16G16B16A16_FLOAT, HSF_UNKNOWN,          DS_NONE,        CP_RGBA8888,    C_BGRA, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R16G16B16A16_UINT    = 26,*/ 
    {HSF_R16G16B16A16_FLOAT, HSF_UNKNOWN,          DS_NONE,        CP_RGBA8888,    C_BGRA, 0,  0,  0,  0,  0,  0,  0,  1,  0,  0},/*HSF_R16G16B16A16_SNORM   = 27,*/ 
    {HSF_R16G16B16A16_FLOAT, HSF_UNKNOWN,          DS_NONE,        CP_RGBA8888,    C_BGRA, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R16G16B16A16_SINT    = 28,*/ 
    {HSF_R32G32_FLOAT,       HSF_UNKNOWN,          DS_NONE,        CP_NCMP,        C_GR,   0,  0,  0,  0,  0,  0,  0,  0,  0,  1},/*HSF_R32G32_TYPELESS      = 29,*/ 
    {HSF_R32G32_FLOAT,       HSF_UNKNOWN,          DS_NONE,        CP_RGBA8888,    C_GR,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R32G32_UINT          = 30,*/ 
    {HSF_R32G32_FLOAT,       HSF_UNKNOWN,          DS_NONE,        CP_RGBA8888,    C_GR,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R32G32_SINT          = 31,*/ 
    //                                                                                ChannelMask                                                                      
    //                                                                                |       bRtSupport                                                               
    //                                                                                |       |   bRBSwizzledAfterMap                                                  
    //                                                                                |       |   |   bASwizzledAfterMap                                               
    //                                                                                |       |   |   |   bBlockCompressed                                             
    //                                                                                |       |   |   |   |   bYUY2                                                    
    //                                                                                |       |   |   |   |   |   b96bpp                                               
    //                                                                                |       |   |   |   |   |   |   bUnorm                                           
    //                                                                                |       |   |   |   |   |   |   |   bSnorm                                       
    //                                                                                |       |   |   |   |   |   |   |   |   bSrgb                                    
    //  MappedDstFormat         SuppressAlphaFormat   CompatibleDsFmt CompressFmt     |       |   |   |   |   |   |   |   |   |   bTypeless                            
    //  |                       |                     |               |               |       |   |   |   |   |   |   |   |   |   |                                    
    {HSF_R10G10B10A2_UNORM,  HSF_UNKNOWN,          DS_NONE,        CP_NCMP,        C_BGRA, 0,  0,  0,  1,  0,  0,  0,  0,  0,  1},/*HSF_R10G10B10A2_TYPELESS = 32,*/ 
    {HSF_R10G10B10A2_UNORM,  HSF_UNKNOWN,          DS_NONE,        CP_RGBA1010102, C_BGR,  0,  1,  0,  0,  0,  0,  1,  0,  0,  0},/*HSF_B10G10R10X2_UNORM    = 33,*/ 
    {HSF_R10G10B10A2_UNORM,  HSF_R10G10B10X2_UINT, DS_NONE,        CP_RGBA1010102, C_BGRA, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R10G10B10A2_UINT     = 34,*/ 
    {HSF_R8G8B8A8_UNORM,     HSF_UNKNOWN,          DS_NONE,        CP_NCMP,        C_BGRA, 0,  0,  0,  0,  0,  0,  0,  0,  0,  1},/*HSF_R8G8B8A8_TYPELESS    = 35,*/ 
    {HSF_R8G8B8A8_UNORM,     HSF_UNKNOWN,          DS_NONE,        CP_RGBA8888,    C_BGRA, 0,  0,  0,  0,  0,  0,  0,  0,  1,  0},/*HSF_R8G8B8A8_UNORM_SRGB  = 36,*/ 
    {HSF_R8G8B8A8_UNORM,     HSF_UNKNOWN,          DS_NONE,        CP_RGBA8888,    C_BGRA, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R8G8B8A8_UINT        = 37,*/ 
    {HSF_R8G8B8A8_UNORM,     HSF_UNKNOWN,          DS_NONE,        CP_RGBA8888,    C_BGRA, 0,  0,  0,  0,  0,  0,  0,  1,  0,  0},/*HSF_R8G8B8A8_SNORM       = 38,*/ 
    {HSF_R8G8B8A8_UNORM,     HSF_UNKNOWN,          DS_NONE,        CP_RGBA8888,    C_BGRA, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R8G8B8A8_SINT        = 39,*/ 
    {HSF_R32_FLOAT,          HSF_UNKNOWN,          D24_UNORM,      CP_D24,         C_R,    0,  0,  0,  0,  0,  0,  1,  0,  0,  0},/*HSF_D24_UNORM            = 40,*/ 
    {HSF_R16G16_FLOAT,       HSF_UNKNOWN,          DS_NONE,        CP_RGBA8888,    C_GR,   0,  0,  0,  0,  0,  0,  1,  0,  0,  0},/*HSF_R16G16_UNORM         = 41,*/ 
    {HSF_R16G16_FLOAT,       HSF_UNKNOWN,          DS_NONE,        CP_RGBA8888,    C_GR,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R16G16_UINT          = 42,*/ 
    {HSF_R16G16_FLOAT,       HSF_UNKNOWN,          DS_NONE,        CP_RGBA8888,    C_GR,   0,  0,  0,  0,  0,  0,  0,  1,  0,  0},/*HSF_R16G16_SNORM         = 43,*/ 
    {HSF_R16G16_FLOAT,       HSF_UNKNOWN,          DS_NONE,        CP_RGBA8888,    C_GR,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R16G16_SINT          = 44,*/ 
    {HSF_R32_FLOAT,          HSF_UNKNOWN,          D16_UNORM,      CP_NCMP,        C_R,    0,  0,  0,  0,  0,  0,  0,  0,  0,  1},/*HSF_R32_TYPELESS         = 45,*/ 
    {HSF_R32_FLOAT,          HSF_UNKNOWN,          DS_NONE,        CP_RGBA8888,    C_R,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R32_UINT             = 46,*/ 
    {HSF_R32_FLOAT,          HSF_UNKNOWN,          DS_NONE,        CP_NCMP,        C_R,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R32_SINT             = 47,*/ 
    {HSF_R8G8_UNORM,         HSF_UNKNOWN,          DS_NONE,        CP_BAYER,       C_GR,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R8G8_TYPELESS        = 48,*/ 
    {HSF_R8G8_UNORM,         HSF_UNKNOWN,          DS_NONE,        CP_RGBA8888,    C_GR,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R8G8_UINT            = 49,*/ 
    {HSF_R8G8_UNORM,         HSF_UNKNOWN,          DS_NONE,        CP_RGBA8888,    C_GR,   0,  0,  0,  0,  0,  0,  0,  1,  0,  0},/*HSF_R8G8_SNORM           = 50,*/ 
    {HSF_R8G8_UNORM,         HSF_UNKNOWN,          DS_NONE,        CP_RGBA8888,    C_GR,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R8G8_SINT            = 51,*/ 
    {HSF_R16_FLOAT,          HSF_UNKNOWN,          D16_UNORM,      CP_NCMP,        C_R,    0,  0,  0,  0,  0,  0,  0,  0,  0,  1},/*HSF_R16_TYPELESS         = 52,*/ 
    {HSF_R16_FLOAT,          HSF_UNKNOWN,          D16_UNORM,      CP_D16,         C_R,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_D16_UNORM            = 53,*/ 
    {HSF_R16_FLOAT,          HSF_UNKNOWN,          D16_UNORM,      CP_D16,         C_R,    0,  0,  0,  0,  0,  0,  1,  0,  0,  0},/*HSF_R16_UNORM            = 54,*/ 
    {HSF_R16_FLOAT,          HSF_UNKNOWN,          DS_NONE,        CP_D16,         C_R,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R16_UINT             = 55,*/ 
    {HSF_R16_FLOAT,          HSF_UNKNOWN,          DS_NONE,        CP_D16,         C_R,    0,  0,  0,  0,  0,  0,  0,  1,  0,  0},/*HSF_R16_SNORM            = 56,*/ 
    {HSF_R16_FLOAT,          HSF_UNKNOWN,          DS_NONE,        CP_D16,         C_R,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R16_SINT             = 57,*/ 
    {HSF_R8_UNORM,           HSF_UNKNOWN,          DS_NONE,        CP_NCMP,        C_R,    0,  0,  0,  0,  0,  0,  0,  0,  0,  1},/*HSF_R8_TYPELESS          = 58,*/ 
    {HSF_R8_UNORM,           HSF_UNKNOWN,          DS_NONE,        CP_RGBA8888,    C_R,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R8_UINT              = 59,*/ 
    {HSF_R8_UNORM,           HSF_UNKNOWN,          DS_NONE,        CP_RGBA8888,    C_R,    0,  0,  0,  0,  0,  0,  0,  1,  0,  0},/*HSF_R8_SNORM             = 60,*/ 
    {HSF_R8_UNORM,           HSF_UNKNOWN,          DS_NONE,        CP_RGBA8888,    C_R,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R8_SINT              = 61,*/ 
    {HSF_R8_UNORM,           HSF_UNKNOWN,          DS_NONE,        CP_RGBA8888,    C_A,    0,  0,  1,  0,  0,  0,  0,  0,  0,  0},/*HSF_A8_UNORM             = 62,*/ 
    {HSF_UNKNOWN,            HSF_UNKNOWN,          DS_NONE,        CP_NCMP,        C_BGR,  0,  1,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_B10G11R11_FLOAT      = 63,*/ 
    //                                                                                ChannelMask                                                                      
    //                                                                                |       bRtSupport                                                               
    //                                                                                |       |   bRBSwizzledAfterMap                                                  
    //                                                                                |       |   |   bASwizzledAfterMap                                               
    //                                                                                |       |   |   |   bBlockCompressed                                             
    //                                                                                |       |   |   |   |   bYUY2                                                    
    //                                                                                |       |   |   |   |   |   b96bpp                                               
    //                                                                                |       |   |   |   |   |   |   bUnorm                                           
    //                                                                                |       |   |   |   |   |   |   |   bSnorm                                       
    //                                                                                |       |   |   |   |   |   |   |   |   bSrgb                                    
    //  MappedDstFormat         SuppressAlphaFormat   CompatibleDsFmt CompressFmt     |       |   |   |   |   |   |   |   |   |   bTypeless                            
    //  |                       |                     |               |               |       |   |   |   |   |   |   |   |   |   |                                    
    {HSF_R8G8B8A8_UNORM,     HSF_UNKNOWN,          DS_NONE,        CP_OPTIONAL,    C_NONE, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R9G9B9E5_SHAREDEXP   = 64,*/ 
    {HSF_R16G16B16A16_FLOAT, HSF_UNKNOWN,          DS_NONE,        CP_NCMP,        C_NONE, 0,  0,  0,  1,  0,  0,  0,  0,  0,  0},/*HSF_BC1_UNORM            = 65,*/ 
    {HSF_R16G16B16A16_FLOAT, HSF_UNKNOWN,          DS_NONE,        CP_NCMP,        C_NONE, 0,  0,  0,  1,  0,  0,  0,  0,  1,  0},/*HSF_BC1_UNORM_SRGB       = 66,*/ 
    {HSF_R32G32B32A32_FLOAT, HSF_UNKNOWN,          DS_NONE,        CP_NCMP,        C_NONE, 0,  0,  0,  1,  0,  0,  0,  0,  0,  0},/*HSF_BC2_UNORM            = 67,*/ 
    {HSF_R32G32B32A32_FLOAT, HSF_UNKNOWN,          DS_NONE,        CP_NCMP,        C_NONE, 0,  0,  0,  1,  0,  0,  0,  0,  1,  0},/*HSF_BC2_UNORM_SRGB       = 68,*/ 
    {HSF_R32G32B32A32_FLOAT, HSF_UNKNOWN,          DS_NONE,        CP_NCMP,        C_NONE, 0,  0,  0,  1,  0,  0,  0,  0,  0,  0},/*HSF_BC3_UNORM            = 69,*/ 
    {HSF_R32G32B32A32_FLOAT, HSF_UNKNOWN,          DS_NONE,        CP_NCMP,        C_NONE, 0,  0,  0,  1,  0,  0,  0,  0,  1,  0},/*HSF_BC3_UNORM_SRGB       = 70,*/ 
    {HSF_R8G8B8A8_UNORM,     HSF_B8G8R8X8_UNORM,   DS_NONE,        CP_RGBA8888,    C_BGRA, 0,  1,  0,  0,  0,  0,  1,  0,  0,  0},/*HSF_B8G8R8A8_UNORM       = 71,*/ 
    {HSF_R8G8B8A8_UNORM,     HSF_UNKNOWN,          DS_NONE,        CP_RGBX8888,    C_BGR,  0,  1,  0,  0,  0,  0,  1,  0,  0,  0},/*HSF_B8G8R8X8_UNORM       = 72,*/ 
    {HSF_R8G8B8A8_UNORM,     HSF_UNKNOWN,          DS_NONE,        CP_RGBX8888,    C_BGR,  0,  0,  0,  0,  0,  0,  1,  0,  0,  0},/*HSF_R8G8B8X8_UNORM       = 73,*/ 
    {HSF_R10G10B10A2_UNORM,  HSF_UNKNOWN,          DS_NONE,        CP_RGBA1010102, C_BGRA, 0,  1,  0,  0,  0,  0,  1,  0,  0,  0},/*HSF_B10G10R10A2_UNORM    = 74,*/ 
    {HSF_R5G5B5A1_UNORM,     HSF_UNKNOWN,          DS_NONE,        CP_NCMP,        C_BGR,  0,  1,  0,  0,  0,  0,  1,  0,  0,  0},/*HSF_B5G5R5X1_UNORM       = 75,*/ 
    {HSF_R5G5B5A1_UNORM,     HSF_B5G5R5X1_UNORM,   DS_NONE,        CP_NCMP,        C_BGRA, 0,  1,  0,  0,  0,  0,  1,  0,  0,  0},/*HSF_B5G5R5A1_UNORM       = 76,*/ 
    {HSF_R4G4B4A4_UNORM,     HSF_B4G4R4X4_UNORM,   DS_NONE,        CP_OPTIONAL,    C_BGRA, 0,  1,  0,  0,  0,  0,  1,  0,  0,  0},/*HSF_B4G4R4A4_UNORM       = 77,*/ 
    {HSF_R4G4B4A4_UNORM,     HSF_UNKNOWN,          DS_NONE,        CP_OPTIONAL,    C_BGR,  0,  1,  0,  0,  0,  0,  1,  0,  0,  0},/*HSF_B4G4R4X4_UNORM       = 78,*/ 
    {HSF_UNKNOWN,            HSF_UNKNOWN,          DS_NONE,        CP_OPTIONAL,    C_NONE, 0,  0,  0,  0,  0,  0,  0,  1,  0,  0},/*HSF_U5V5L6_XNORM         = 79,*/ 
    {HSF_R8G8_UNORM,         HSF_UNKNOWN,          DS_NONE,        CP_D16,         C_NONE, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_L16_UNORM            = 80,*/ 
    {HSF_R8G8_UNORM,         HSF_UNKNOWN,          DS_NONE,        CP_RGBA8888,    C_BGRA, 0,  0,  0,  0,  0,  0,  1,  0,  0,  0},/*HSF_L8A8_UNORM           = 81,*/ 
    {HSF_R8_UNORM,           HSF_UNKNOWN,          S8_UINT,        CP_RGBA8888,    C_BGRA, 0,  0,  0,  0,  0,  0,  1,  0,  0,  0},/*HSF_L8_UNORM             = 82,*/ 
    {HSF_R8_UNORM,           HSF_UNKNOWN,          DS_NONE,        CP_BAYER,       C_NONE, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_L4A4_VIDEO           = 83,*/ 
    {HSF_R8G8B8A8_UNORM,     HSF_UNKNOWN,          DS_NONE,        CP_RGBA8888,    C_BGRA, 0,  1,  0,  0,  0,  0,  0,  0,  1,  0},/*HSF_B8G8R8A8_UNORM_SRGB  = 84,*/ 
    {HSF_R8G8B8A8_UNORM,     HSF_UNKNOWN,          DS_NONE,        CP_RGBX8888,    C_BGR,  0,  1,  0,  0,  0,  0,  0,  0,  1,  0},/*HSF_B8G8R8X8_UNORM_SRGB  = 85,*/ 
    {HSF_R16G16B16A16_FLOAT, HSF_UNKNOWN,          DS_NONE,        CP_NCMP,        C_BGRA, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R16G16B16A16_USCALE  = 86,*/ 
    {HSF_R16G16B16A16_FLOAT, HSF_UNKNOWN,          DS_NONE,        CP_NCMP,        C_BGRA, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R16G16B16A16_SSCALE  = 87,*/ 
    {HSF_R10G10B10A2_UNORM,  HSF_UNKNOWN,          DS_NONE,        CP_RGBA1010102, C_BGR,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R10G10B10X2_UINT     = 88,*/ 
    {HSF_R10G10B10A2_UNORM,  HSF_UNKNOWN,          DS_NONE,        CP_RGBA1010102, C_BGR,  0,  0,  0,  0,  0,  0,  0,  1,  0,  0},/*HSF_R10G10B10X2_SNORM    = 89,*/ 
    {HSF_R10G10B10A2_UNORM,  HSF_R10G10B10X2_SNORM,DS_NONE,        CP_RGBA1010102, C_BGRA, 0,  0,  0,  0,  0,  0,  0,  1,  0,  0},/*HSF_R10G10B10A2_SNORM    = 90,*/ 
    {HSF_R8G8B8A8_UNORM,     HSF_UNKNOWN,          DS_NONE,        CP_RGBA8888,    C_BGRA, 0,  1,  0,  0,  0,  0,  0,  1,  0,  0},/*HSF_B8G8R8A8_XNORM       = 91,*/ 
    {HSF_R16G16_FLOAT,       HSF_UNKNOWN,          DS_NONE,        CP_NCMP,        C_GR,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R16G16_USCALE        = 92,*/ 
    {HSF_R16G16_FLOAT,       HSF_UNKNOWN,          DS_NONE,        CP_NCMP,        C_GR,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R16G16_SSCALE        = 93,*/ 
    {HSF_R8G8B8A8_UNORM,     HSF_UNKNOWN,          DS_NONE,        CP_NCMP,        C_BGRA, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R8G8B8A8_USCALE      = 94,*/ 
    {HSF_R8G8B8A8_UNORM,     HSF_UNKNOWN,          DS_NONE,        CP_NCMP,        C_BGRA, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R8G8B8A8_SSCALE      = 95,*/ 
    //                                                                                ChannelMask                                                                      
    //                                                                                |       bRtSupport                                                               
    //                                                                                |       |   bRBSwizzledAfterMap                                                  
    //                                                                                |       |   |   bASwizzledAfterMap                                               
    //                                                                                |       |   |   |   bBlockCompressed                                             
    //                                                                                |       |   |   |   |   bYUY2                                                    
    //                                                                                |       |   |   |   |   |   b96bpp                                               
    //                                                                                |       |   |   |   |   |   |   bUnorm                                           
    //                                                                                |       |   |   |   |   |   |   |   bSnorm                                       
    //                                                                                |       |   |   |   |   |   |   |   |   bSrgb                                    
    //  MappedDstFormat         SuppressAlphaFormat   CompatibleDsFmt CompressFmt     |       |   |   |   |   |   |   |   |   |   bTypeless                            
    //  |                       |                     |               |               |       |   |   |   |   |   |   |   |   |   |                                    
    {HSF_R10G10B10A2_UNORM,  HSF_UNKNOWN,          DS_NONE,        CP_NCMP,        C_BGR,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R10G10B10X2_USCALE   = 96,*/ 
    {HSF_R16_FLOAT,          HSF_UNKNOWN,          DS_NONE,        CP_NCMP,        C_R,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R16_USCALE           = 97,*/ 
    {HSF_R16_FLOAT,          HSF_UNKNOWN,          DS_NONE,        CP_NCMP,        C_R,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R16_SSCALE           = 98,*/ 
    {HSF_R8G8_UNORM,         HSF_UNKNOWN,          DS_NONE,        CP_NCMP,        C_R,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R8G8_USCALE          = 99,*/ 
    {HSF_R8G8_UNORM,         HSF_UNKNOWN,          DS_NONE,        CP_NCMP,        C_R,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R8G8_SSCALE          = 100,*/
    {HSF_R8_UNORM,           HSF_UNKNOWN,          DS_NONE,        CP_NCMP,        C_R,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R8_USCALE            = 101,*/
    {HSF_R8_UNORM,           HSF_UNKNOWN,          DS_NONE,        CP_NCMP,        C_R,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R8_SSCALE            = 102,*/
    {HSF_UNKNOWN,            HSF_UNKNOWN,          DS_NONE,        CP_RGBA8888,    C_BGRA, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R32G32B32A32_FIX     = 103,*/
    {HSF_UNKNOWN,            HSF_UNKNOWN,          DS_NONE,        CP_RGBA8888,    C_BGR,  0,  0,  0,  0,  0,  1,  0,  0,  0,  0},/*HSF_R32G32B32_FIX        = 104,*/
    {HSF_UNKNOWN,            HSF_UNKNOWN,          DS_NONE,        CP_RGBA8888,    C_GR,   0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R32G32_FIX           = 105,*/
    {HSF_UNKNOWN,            HSF_UNKNOWN,          DS_NONE,        CP_RGBA8888,    C_R,    0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_R32_FIX              = 106,*/
    {HSF_R10G10B10A2_UNORM,  HSF_UNKNOWN,          DS_NONE,        CP_ABGR2101010, C_BGRA, 0,  1,  1,  0,  0,  0,  1,  0,  0,  0},/*HSF_A2B10G10R10_UNORM    = 107,*/
    {HSF_R10G10B10A2_UNORM,  HSF_UNKNOWN,          DS_NONE,        CP_ABGR2101010, C_BGRA, 0,  1,  1,  0,  0,  0,  0,  1,  0,  0},/*HSF_A2B10G10R10_SNORM    = 108,*/
    {HSF_R8G8B8A8_UNORM,     HSF_UNKNOWN,          DS_NONE,        CP_XRGB8888,    C_BGR,  0,  0,  1,  0,  0,  0,  1,  0,  0,  0},/*HSF_X8R8G8B8_UNORM       = 109,*/
    {HSF_R8G8B8A8_UNORM,     HSF_UNKNOWN,          DS_NONE,        CP_XRGB8888,    C_BGR,  0,  1,  1,  0,  0,  0,  1,  0,  0,  0},/*HSF_X8B8G8R8_UNORM       = 110,*/
    {HSF_R8G8B8A8_UNORM,     HSF_X8R8G8B8_UNORM,   DS_NONE,        CP_ARGB8888,    C_BGRA, 0,  0,  1,  0,  0,  0,  1,  0,  0,  0},/*HSF_A8R8G8B8_UNORM       = 111,*/
    {HSF_R8G8B8A8_UNORM,     HSF_X8B8G8R8_UNORM,   DS_NONE,        CP_ARGB8888,    C_BGRA, 0,  1,  1,  0,  0,  0,  1,  0,  0,  0},/*HSF_A8B8G8R8_UNORM       = 112,*/
    {HSF_R5G5B5A1_UNORM,     HSF_UNKNOWN,          DS_NONE,        CP_NCMP,        C_BGRA, 0,  0,  1,  0,  0,  0,  1,  0,  0,  0},/*HSF_A1R5G5B5_UNORM       = 113,*/
    {HSF_R5G5B5A1_UNORM,     HSF_UNKNOWN,          DS_NONE,        CP_NCMP,        C_BGRA, 0,  1,  1,  0,  0,  0,  1,  0,  0,  0},/*HSF_A1B5G5R5_UNORM       = 114,*/
    {HSF_R5G6B5_UNORM,       HSF_UNKNOWN,          DS_NONE,        CP_RGB565,      C_BGR,  0,  1,  0,  0,  0,  0,  1,  0,  0,  0},/*HSF_B5G6R5_UNORM         = 115,*/
    {HSF_R4G4B4A4_UNORM,     HSF_UNKNOWN,          DS_NONE,        CP_RGBA8888,    C_BGRA, 0,  0,  1,  0,  0,  0,  1,  0,  0,  0},/*HSF_A4R4G4B4_UNORM       = 116,*/
    {HSF_R4G4B4A4_UNORM,     HSF_UNKNOWN,          DS_NONE,        CP_RGBA8888,    C_BGRA, 0,  1,  1,  0,  0,  0,  1,  0,  0,  0},/*HSF_A4B4G4R4_UNORM       = 117,*/
    {HSF_R8_UNORM,           HSF_UNKNOWN,          S8_UINT,        CP_RGBA8888,    C_NONE, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_G8_UINT              = 118,*/
    {HSF_UNKNOWN,            HSF_UNKNOWN,          DS_NONE,        CP_YCrCb,       C_NONE, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_YCRCB_MB_8_420       = 119,*/
    {HSF_UNKNOWN,            HSF_UNKNOWN,          DS_NONE,        CP_YUYV,        C_NONE, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_NV12                 = 120,*/
    {HSF_UNKNOWN,            HSF_UNKNOWN,          DS_NONE,        CP_YUYV,        C_NONE, 0,  0,  0,  0,  1,  0,  0,  0,  0,  0},/*HSF_YV12                 = 121,*/
    {HSF_UNKNOWN,            HSF_UNKNOWN,          DS_NONE,        CP_NCMP,        C_NONE, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_Y216_VIDEO           = 122,*/
    {HSF_UNKNOWN,            HSF_UNKNOWN,          DS_NONE,        CP_NCMP,        C_NONE, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_Y210_VIDEO           = 123,*/
    {HSF_UNKNOWN,            HSF_UNKNOWN,          DS_NONE,        CP_NCMP,        C_NONE, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_AYUV_VIDEO           = 124,*/
    {HSF_UNKNOWN,            HSF_UNKNOWN,          DS_NONE,        CP_NCMP,        C_NONE, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_RESERVED_FMT_125     = 125,*/
    {HSF_UNKNOWN,            HSF_UNKNOWN,          DS_NONE,        CP_NCMP,        C_NONE, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0},/*HSF_RESERVED_FMT_126     = 126,*/
    {HSF_UNKNOWN,            HSF_UNKNOWN,          DS_NONE,        CP_UYUV,        C_BGRA, 0,  0,  0,  0,  1,  0,  0,  0,  0,  0},/*HSF_UYVY                 = 127,*/
};

#endif
