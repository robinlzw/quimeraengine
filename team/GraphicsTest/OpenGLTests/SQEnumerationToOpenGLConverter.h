﻿
#ifndef __SQENUMERATIONTOOPENGLCONVERTER__
#define __SQENUMERATIONTOOPENGLCONVERTER__

#include <GL/glew.h>

class SQEnumerationToOpenGLConverter
{
private:

    SQEnumerationToOpenGLConverter();

public:

    static GLenum ConvertPixelFormatToPixelDataFormatSized(const u32_q eFormat)
    {
        static const GLenum EQUIVALENT_VALUES[] = {
                                                    GL_R8,//E_R8UI_Normalized,
                                                    GL_R16,//E_R16UI_Normalized,
                                                    GL_RG8,//E_RG8UI_Normalized,
                                                    GL_RG16,//E_RG16UI_Normalized,
                                                    GL_R3_G3_B2,//E_R3G3B2UI_Normalized,
                                                    GL_RGB4,//E_RGB4UI_Normalized,
                                                    GL_RGB5,//E_RGB5UI_Normalized,
                                                    GL_RGB8,//E_RGB8UI_Normalized,
                                                    GL_RGB10,//E_RGB10UI_Normalized,
                                                    GL_RGB12,//E_RGB12UI_Normalized,
                                                    GL_RGB16,//E_RGB16UI_Normalized,
                                                    GL_RGB565,//E_RGB565UI_Normalized
                                                    GL_RGBA2,//E_RGBA2UI_Normalized,
                                                    GL_RGBA4,//E_RGBA4UI_Normalized,
                                                    GL_RGB5_A1,//E_RGB5A1UI_Normalized,
                                                    GL_RGBA8,//E_RGBA8UI_Normalized,
                                                    GL_RGB10_A2,//E_RGB10A2UI_Normalized,
                                                    GL_RGBA12,//E_RGBA12UI_Normalized,
                                                    GL_RGBA16,//E_RGBA16UI_Normalized,
                                                    GL_SRGB8,//E_RGB8_Compressed,
                                                    GL_SRGB8_ALPHA8,//E_RGB8A8_Compressed,
                                                    GL_R16F,//E_R16F,
                                                    GL_R32F,//E_R32F,
                                                    GL_RG16F,//E_RG16F,
                                                    GL_RG32F,//E_RG32F,
                                                    GL_RGB16F,//E_RGB16F,
                                                    GL_RGB32F,//E_RGB32F,
                                                    GL_RGBA16F,//E_RGBA16F,
                                                    GL_RGBA32F,//E_RGBA32F,
                                                    GL_R11F_G11F_B10F,//E_R11FG11FB10F,
                                                    GL_RGB9_E5,//E_RGB9F_E5,
                                                    GL_R8I,//E_R8I,
                                                    GL_R8UI,//E_R8UI,
                                                    GL_R16I,//E_R16I,
                                                    GL_R16UI,//E_R16UI,
                                                    GL_R32I,//E_R32I,
                                                    GL_R32UI,//E_R32UI,
                                                    GL_RG8I,//E_RG8I,
                                                    GL_RG8UI,//E_RG8UI,
                                                    GL_RG16I,//E_RG16I,
                                                    GL_RG16UI,//E_RG16UI,
                                                    GL_R32I,//E_RG32I,
                                                    GL_R32UI,//E_RG32UI,
                                                    GL_RGB8I,//E_RGB8I,
                                                    GL_RGB8UI,//E_RGB8UI,
                                                    GL_RGB16I,//E_RGB16I,
                                                    GL_RGB16UI,//E_RGB16UI,
                                                    GL_RGB32I,//E_RGB32I,
                                                    GL_RGB32UI,//E_RGB32UI,
                                                    GL_RGBA8I,//E_RGBA8I,
                                                    GL_RGBA8UI,//E_RGBA8UI,
                                                    GL_RGBA16I,//E_RGBA16I,
                                                    GL_RGBA16UI,//E_RGBA16UI,
                                                    GL_RGBA32I,//E_RGBA32I,
                                                    GL_RGBA32UI,//E_RGBA32UI,
                                                    GL_RGB10_A2UI,//E_RGB10A2UI
                                                    GL_R8_SNORM,//E_R8I_Normalized,
                                                    GL_R16_SNORM,//E_R16I_Normalized,
                                                    GL_RG8_SNORM,//E_RG8I_Normalized,
                                                    GL_RG16_SNORM,//E_RG16I_Normalized,
                                                    GL_RGB8_SNORM,//E_RGB8I_Normalized,
                                                    GL_RGB16_SNORM,//E_RGB16I_Normalized,
                                                    GL_RGBA8_SNORM,//E_RGBA8I_Normalized,
                                                    GL_RGBA16_SNORM,//E_RGBA16I_Normalized,
                                                    0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                    0, 0, 0, 0, 0, 0, 0,
                                                    GL_DEPTH_COMPONENT16,//E_D16,
                                                    GL_DEPTH_COMPONENT24,//E_D24,
                                                    GL_DEPTH_COMPONENT32,//E_D32,
                                                    GL_DEPTH_COMPONENT32F,//E_D32F,
                                                    0, 0, 0, 0, 0, 0,
                                                    GL_STENCIL_INDEX1,//E_S1,
                                                    GL_STENCIL_INDEX4,//E_S4,
                                                    GL_STENCIL_INDEX8,//E_S8,
                                                    GL_STENCIL_INDEX16,//E_S16,
                                                    0, 0, 0, 0, 0, 0,
                                                    GL_DEPTH24_STENCIL8,//E_D24S8,
                                                    GL_DEPTH32F_STENCIL8,//E_D32FS8
                                                    0, 0, 0
                                                };
        return EQUIVALENT_VALUES[eFormat];
    }
    
    static GLenum ConvertPixelFormatToPixelDataFormatNonSized(const u32_q eFormat)
    {
        static const GLenum EQUIVALENT_VALUES[] = {
                                                    GL_RED,//E_R8UI_Normalized,
                                                    GL_RED,//E_R16UI_Normalized,
                                                    GL_RG,//E_RG8UI_Normalized,
                                                    GL_RG,//E_RG16UI_Normalized,
                                                    GL_RGB,//E_R3G3B2UI_Normalized,
                                                    GL_RGB,//E_RGB4UI_Normalized,
                                                    GL_RGB,//E_RGB5UI_Normalized,
                                                    GL_RGB,//E_RGB8UI_Normalized,
                                                    GL_RGB,//E_RGB10UI_Normalized,
                                                    GL_RGB,//E_RGB12UI_Normalized,
                                                    GL_RGB,//E_RGB16UI_Normalized,
                                                    GL_RGB,//E_RGB565UI_Normalized
                                                    GL_RGBA,//E_RGBA2UI_Normalized,
                                                    GL_RGBA,//E_RGBA4UI_Normalized,
                                                    GL_RGBA,//E_RGB5A1UI_Normalized,
                                                    GL_RGBA,//E_RGBA8UI_Normalized,
                                                    GL_RGBA,//E_RGB10A2UI_Normalized,
                                                    GL_RGBA,//E_RGBA12UI_Normalized,
                                                    GL_RGBA,//E_RGBA16UI_Normalized,
                                                    GL_RGB,//E_RGB8_Compressed,
                                                    GL_RGBA,//E_RGB8A8_Compressed,
                                                    GL_RED,//E_R16F,
                                                    GL_RED,//E_R32F,
                                                    GL_RG,//E_RG16F,
                                                    GL_RG,//E_RG32F,
                                                    GL_RGB,//E_RGB16F,
                                                    GL_RGB,//E_RGB32F,
                                                    GL_RGBA,//E_RGBA16F,
                                                    GL_RGBA,//E_RGBA32F,
                                                    GL_RGB,//E_R11FG11FB10F,
                                                    GL_RGBA,//E_RGB9F_E5,
                                                    GL_RED_INTEGER,//E_R8I,
                                                    GL_RED_INTEGER,//E_R8UI,
                                                    GL_RED_INTEGER,//E_R16I,
                                                    GL_RED_INTEGER,//E_R16UI,
                                                    GL_RED_INTEGER,//E_R32I,
                                                    GL_RED_INTEGER,//E_R32UI,
                                                    GL_RG_INTEGER,//E_RG8I,
                                                    GL_RG_INTEGER,//E_RG8UI,
                                                    GL_RG_INTEGER,//E_RG16I,
                                                    GL_RG_INTEGER,//E_RG16UI,
                                                    GL_RED_INTEGER,//E_RG32I,
                                                    GL_RED_INTEGER,//E_RG32UI,
                                                    GL_RGB_INTEGER,//E_RGB8I,
                                                    GL_RGB_INTEGER,//E_RGB8UI,
                                                    GL_RGB_INTEGER,//E_RGB16I,
                                                    GL_RGB_INTEGER,//E_RGB16UI,
                                                    GL_RGB_INTEGER,//E_RGB32I,
                                                    GL_RGB_INTEGER,//E_RGB32UI,
                                                    GL_RGBA_INTEGER,//E_RGBA8I,
                                                    GL_RGBA_INTEGER,//E_RGBA8UI,
                                                    GL_RGBA_INTEGER,//E_RGBA16I,
                                                    GL_RGBA_INTEGER,//E_RGBA16UI,
                                                    GL_RGBA_INTEGER,//E_RGBA32I,
                                                    GL_RGBA_INTEGER,//E_RGBA32UI,
                                                    GL_RGBA_INTEGER,//E_RGB10A2UI
                                                    GL_RED,//E_R8I_Normalized,
                                                    GL_RED,//E_R16I_Normalized,
                                                    GL_RG,//E_RG8I_Normalized,
                                                    GL_RG,//E_RG16I_Normalized,
                                                    GL_RGB,//E_RGB8I_Normalized,
                                                    GL_RGB,//E_RGB16I_Normalized,
                                                    GL_RGBA,//E_RGBA8I_Normalized,
                                                    GL_RGBA,//E_RGBA16I_Normalized,
                                                    0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                    0, 0, 0, 0, 0, 0, 0,
                                                    GL_DEPTH_COMPONENT,//E_D16,
                                                    GL_DEPTH_COMPONENT,//E_D24,
                                                    GL_DEPTH_COMPONENT,//E_D32,
                                                    GL_DEPTH_COMPONENT,//E_D32F,
                                                    0, 0, 0, 0, 0, 0,
                                                    GL_STENCIL_INDEX,//E_S1,
                                                    GL_STENCIL_INDEX,//E_S4,
                                                    GL_STENCIL_INDEX,//E_S8,
                                                    GL_STENCIL_INDEX,//E_S16,
                                                    0, 0, 0, 0, 0, 0,
                                                    GL_DEPTH_STENCIL,//E_D24S8,
                                                    GL_DEPTH_STENCIL,//E_D32FS8
                                                    0, 0, 0
                                                };
        return EQUIVALENT_VALUES[eFormat];
    }
    
    static GLenum ConvertPixelFormatToPixelDataType(const u32_q eFormat)
    {
        static const GLenum EQUIVALENT_VALUES[] = {
                                                    GL_UNSIGNED_BYTE,//E_R8UI_Normalized,
                                                    GL_UNSIGNED_SHORT,//E_R16UI_Normalized,
                                                    GL_UNSIGNED_BYTE,//E_RG8UI_Normalized,
                                                    GL_UNSIGNED_SHORT,//E_RG16UI_Normalized,
                                                    GL_UNSIGNED_BYTE_3_3_2,//E_R3G3B2UI_Normalized,
                                                    GL_UNSIGNED_SHORT_4_4_4_4,//E_RGB4UI_Normalized,
                                                    GL_UNSIGNED_SHORT_5_5_5_1,//E_RGB5UI_Normalized,
                                                    GL_UNSIGNED_BYTE,//E_RGB8UI_Normalized,
                                                    GL_UNSIGNED_INT_10_10_10_2,//E_RGB10UI_Normalized,
                                                    0,//E_RGB12UI_Normalized,
                                                    GL_UNSIGNED_SHORT,//E_RGB16UI_Normalized,
                                                    GL_UNSIGNED_SHORT_5_6_5,//E_RGB565UI_Normalized
                                                    0,//E_RGBA2UI_Normalized,
                                                    GL_UNSIGNED_SHORT_4_4_4_4,//E_RGBA4UI_Normalized,
                                                    GL_UNSIGNED_SHORT_5_5_5_1,//E_RGB5A1UI_Normalized,
                                                    GL_UNSIGNED_BYTE,//E_RGBA8UI_Normalized,
                                                    GL_UNSIGNED_INT_10_10_10_2,//E_RGB10A2UI_Normalized,
                                                    0,//E_RGBA12UI_Normalized,
                                                    GL_UNSIGNED_SHORT,//E_RGBA16UI_Normalized,
                                                    GL_UNSIGNED_BYTE,//E_RGB8_Compressed,
                                                    GL_UNSIGNED_BYTE,//E_RGB8A8_Compressed,
                                                    GL_HALF_FLOAT,//E_R16F,
                                                    GL_FLOAT,//E_R32F,
                                                    GL_HALF_FLOAT,//E_RG16F,
                                                    GL_FLOAT,//E_RG32F,
                                                    GL_HALF_FLOAT,//E_RGB16F,
                                                    GL_FLOAT,//E_RGB32F,
                                                    GL_HALF_FLOAT,//E_RGBA16F,
                                                    GL_FLOAT,//E_RGBA32F,
                                                    GL_UNSIGNED_INT_10F_11F_11F_REV,//E_R11FG11FB10F,
                                                    GL_UNSIGNED_INT_5_9_9_9_REV,//E_RGB9F_E5,
                                                    GL_BYTE,//E_R8I,
                                                    GL_UNSIGNED_BYTE,//E_R8UI,
                                                    GL_SHORT,//E_R16I,
                                                    GL_UNSIGNED_SHORT,//E_R16UI,
                                                    GL_INT,//E_R32I,
                                                    GL_UNSIGNED_INT,//E_R32UI,
                                                    GL_BYTE,//E_RG8I,
                                                    GL_UNSIGNED_BYTE,//E_RG8UI,
                                                    GL_SHORT,//E_RG16I,
                                                    GL_UNSIGNED_SHORT,//E_RG16UI,
                                                    GL_INT,//E_RG32I,
                                                    GL_UNSIGNED_INT,//E_RG32UI,
                                                    GL_BYTE,//E_RGB8I,
                                                    GL_UNSIGNED_BYTE,//E_RGB8UI,
                                                    GL_SHORT,//E_RGB16I,
                                                    GL_UNSIGNED_SHORT,//E_RGB16UI,
                                                    GL_INT,//E_RGB32I,
                                                    GL_UNSIGNED_INT,//E_RGB32UI,
                                                    GL_BYTE,//E_RGBA8I,
                                                    GL_UNSIGNED_BYTE,//E_RGBA8UI,
                                                    GL_SHORT,//E_RGBA16I,
                                                    GL_UNSIGNED_SHORT,//E_RGBA16UI,
                                                    GL_INT,//E_RGBA32I,
                                                    GL_UNSIGNED_INT,//E_RGBA32UI,
                                                    GL_UNSIGNED_INT_10_10_10_2,//E_RGB10A2UI
                                                    GL_BYTE,//E_R8I_Normalized,
                                                    GL_SHORT,//E_R16I_Normalized,
                                                    GL_BYTE,//E_RG8I_Normalized,
                                                    GL_SHORT,//E_RG16I_Normalized,
                                                    GL_BYTE,//E_RGB8I_Normalized,
                                                    GL_SHORT,//E_RGB16I_Normalized,
                                                    GL_BYTE,//E_RGBA8I_Normalized,
                                                    GL_SHORT,//E_RGBA16I_Normalized,
                                                    0, 0, 0, 0, 0, 0, 0, 0, 0,
                                                    0, 0, 0, 0, 0, 0, 0,
                                                    GL_UNSIGNED_SHORT,//E_D16,
                                                    GL_UNSIGNED_INT_24_8,//E_D24,
                                                    GL_UNSIGNED_INT,//E_D32,
                                                    GL_FLOAT,//E_D32F,
                                                    0, 0, 0, 0, 0, 0,
                                                    0,//E_S1,
                                                    0,//E_S4,
                                                    GL_UNSIGNED_BYTE,//E_S8,
                                                    GL_UNSIGNED_SHORT,//E_S16,
                                                    0, 0, 0, 0, 0, 0,
                                                    GL_UNSIGNED_INT_24_8,//E_D24S8,
                                                    GL_FLOAT_32_UNSIGNED_INT_24_8_REV,//E_D32FS8
                                                    0, 0, 0
                                                };
        return EQUIVALENT_VALUES[eFormat];
    }

};

#endif // __SQENUMERATIONTOOPENGLCONVERTER__