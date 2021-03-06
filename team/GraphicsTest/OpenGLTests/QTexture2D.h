
#ifndef __QTEXTURE2D__
#define __QTEXTURE2D__

#include "GL/glew.h"

#include "QAbstractTexture2D.h"
#include "QSampler2D.h"
#include "QPixelFormatDescriptor.h"
#include "ErrorTracingDefinitions.h"
#include "SQEnumerationToOpenGLConverter.h"


class QTexture2D : public QAbstractTexture2D
{
public:

    enum EQTextureMapping
    {
        E_UV,
        E_Spherical,
        E_Cylindrical,
        E_Cubic,
        E_Planar
    };
    
    QTexture2D(const GLuint textureId, const GLenum type, const GLenum format, const EQTextureFormat::EnumType eFormat, const u32_q uWidth, const u32_q uHeight, const u8_q uMipmapLevels, const u8_q uMultisamplingSamples) :
                                                                                                                            m_textureId(textureId),
                                                                                                                            m_type(type),
                                                                                                                            m_format(format)
    {
        m_eFormat = eFormat;
        m_uWidth = uWidth;
        m_uHeight = uHeight;
        m_uMipmapLevels = uMipmapLevels;
        m_uSamples = uMultisamplingSamples;
    }

   virtual void GetSubtexture(  const i32_q nSourceOffsetX,
                                const i32_q nSourceOffsetY,
                                const u32_q nSourceWidth,
                                const u32_q nSourceHeight,
                                const EQTextureFormat::EnumType eSourceFormat,
                                const u8_q uSourceMipmapLevel,
                                u32_q &uSourceSize,
                                void** ppDestination) const
    {
//        Y AXIS OF TEXTURE MUST BE FLIPPED

        QE_ASSERT_ERROR(nSourceOffsetX + nSourceWidth <= m_uWidth, "The region to be read lays outside the texture's dimensions.");
        QE_ASSERT_ERROR(nSourceOffsetY + nSourceHeight <= m_uHeight, "The region to be read lays outside the texture's dimensions.");
        QE_ASSERT_ERROR(m_uSamples == 0, "Reading a region of a multisample texture is not supported.");
        QE_ASSERT_ERROR(ppDestination != null_q, "The output subtexture cannot be null.");

        if (*ppDestination == null_q)
        {
            QPixelFormatDescriptor pixelFormatDescriptor(scast_q(eSourceFormat, EQPixelFormat::EnumType));

            // Allocate memory
            uSourceSize = pixelFormatDescriptor.GetSize() * nSourceWidth * nSourceHeight;
            *ppDestination = new u8_q[uSourceSize];
        }

        GLenum formatType = SQEnumerationToOpenGLConverter::ConvertPixelFormatToPixelDataType(eSourceFormat);
        GLenum formatLayout = SQEnumerationToOpenGLConverter::ConvertPixelFormatToPixelDataFormatNonSized(eSourceFormat);

        static const GLint FIRST_SLICE = 0;
        static const GLsizei NUM_SLICES = 1;
        glGetTextureSubImage(m_textureId, uSourceMipmapLevel, nSourceOffsetX, nSourceOffsetY, FIRST_SLICE, nSourceWidth, nSourceHeight, NUM_SLICES, formatLayout, formatType, uSourceSize, *ppDestination);

        QE_ASSERT_OPENGL_ERROR("An error occurred when reading from the texture.");
    }

    virtual void GetFullContent(const EQTextureFormat::EnumType eSourceFormat, const u8_q uDestinationMipmapLevel, u32_q &uDestinationSize, void** ppDestination) const
    {
        QE_ASSERT_ERROR(ppDestination != null_q, "The output subtexture cannot be null.");

        if (*ppDestination == null_q)
        {
            QPixelFormatDescriptor pixelFormatDescriptor(scast_q(eSourceFormat, EQPixelFormat::EnumType));

            // Allocate memory
            uDestinationSize = pixelFormatDescriptor.GetSize() * m_uWidth * m_uHeight;
            uDestinationSize *= m_uSamples > 0 ? m_uSamples : 1U;
            *ppDestination = new u8_q[uDestinationSize];
        }

        GLenum formatType = SQEnumerationToOpenGLConverter::ConvertPixelFormatToPixelDataType(eSourceFormat);
        GLenum formatLayout = SQEnumerationToOpenGLConverter::ConvertPixelFormatToPixelDataFormatNonSized(eSourceFormat);

        glGetTextureImage(m_textureId, uDestinationMipmapLevel, formatLayout, formatType, uDestinationSize, *ppDestination);

        QE_ASSERT_OPENGL_ERROR("An error occurred when reading from the texture.");
    }

    // COPY full and region

    virtual void SetFullContent(const u8_q uDestinationMipmapLevel, const EQTextureFormat::EnumType eSourceFormat, const void* pSource)
    {
        GLenum formatType = SQEnumerationToOpenGLConverter::ConvertPixelFormatToPixelDataType(eSourceFormat);
        GLenum formatLayout = SQEnumerationToOpenGLConverter::ConvertPixelFormatToPixelDataFormatNonSized(eSourceFormat);

        glTextureSubImage2D(m_textureId, uDestinationMipmapLevel, 0, 0, m_uWidth, m_uHeight, formatLayout, formatType, pSource);

        QE_ASSERT_OPENGL_ERROR("An error occurred when writing to the texture.");
    }

    virtual void SetSubtexture(const i32_q nDestinationOffsetX, const i32_q nDestinationOffsetY, const u32_q nDestinationWidth, const u32_q nDestinationHeight, const u8_q uDestinationMipmapLevel, const EQTextureFormat::EnumType eSourceFormat, const void* pSource)
    {
        QE_ASSERT_ERROR(nDestinationOffsetX + nDestinationWidth <= m_uWidth, "The region to be written overwrites memory outside the texture's dimensions.");
        QE_ASSERT_ERROR(nDestinationOffsetY + nDestinationHeight <= m_uHeight, "The region to be written overwrites memory outside the texture's dimensions.");

        GLenum formatType = SQEnumerationToOpenGLConverter::ConvertPixelFormatToPixelDataType(eSourceFormat);
        GLenum formatLayout = SQEnumerationToOpenGLConverter::ConvertPixelFormatToPixelDataFormatNonSized(eSourceFormat);

        glTextureSubImage2D(m_textureId, uDestinationMipmapLevel, nDestinationOffsetX, nDestinationOffsetY, nDestinationWidth, nDestinationHeight, formatLayout, formatType, pSource);

        QE_ASSERT_OPENGL_ERROR("An error occurred when writing to the texture.");
    }

    // glInvalidate image, texsubimage, framebuffer, subframebuffer -----> No equivalent in DirectX


    GLuint GetExternalId() const
    {
        return m_textureId;
    }

protected:

    GLuint m_textureId;
    GLenum m_format;
    GLenum m_type;
};

#endif // __QTEXTURE2D__
