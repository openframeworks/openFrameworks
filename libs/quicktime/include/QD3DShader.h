/*
     File:       QD3DShader.h
 
     Contains:   QuickDraw 3D Shader / Color Routines
 
     Version:    Technology: Quickdraw 3D 1.6
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1995-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __QD3DSHADER__
#define __QD3DSHADER__

#ifndef __QD3D__
#include <QD3D.h>
#endif

#ifndef __MOVIES__
#include <Movies.h>
#endif




#if PRAGMA_ONCE
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if PRAGMA_IMPORT
#pragma import on
#endif

#if PRAGMA_STRUCT_ALIGN
    #pragma options align=power
#elif PRAGMA_STRUCT_PACKPUSH
    #pragma pack(push, 2)
#elif PRAGMA_STRUCT_PACK
    #pragma pack(2)
#endif

#if PRAGMA_ENUM_ALWAYSINT
    #if defined(__fourbyteints__) && !__fourbyteints__ 
        #define __QD3DSHADER__RESTORE_TWOBYTEINTS
        #pragma fourbyteints on
    #endif
    #pragma enumsalwaysint on
#elif PRAGMA_ENUM_OPTIONS
    #pragma option enum=int
#elif PRAGMA_ENUM_PACK
    #if __option(pack_enums)
        #define __QD3DSHADER__RESTORE_PACKED_ENUMS
        #pragma options(!pack_enums)
    #endif
#endif

/******************************************************************************
 **                                                                          **
 **                             RGB Color routines                           **
 **                                                                          **
 *****************************************************************************/
#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3ColorRGB *)
Q3ColorRGB_Set                  (TQ3ColorRGB *          color,
                                 float                  r,
                                 float                  g,
                                 float                  b);

EXTERN_API_C( TQ3ColorARGB *)
Q3ColorARGB_Set                 (TQ3ColorARGB *         color,
                                 float                  a,
                                 float                  r,
                                 float                  g,
                                 float                  b);

EXTERN_API_C( TQ3ColorRGB *)
Q3ColorRGB_Add                  (const TQ3ColorRGB *    c1,
                                 const TQ3ColorRGB *    c2,
                                 TQ3ColorRGB *          result);

EXTERN_API_C( TQ3ColorRGB *)
Q3ColorRGB_Subtract             (const TQ3ColorRGB *    c1,
                                 const TQ3ColorRGB *    c2,
                                 TQ3ColorRGB *          result);

EXTERN_API_C( TQ3ColorRGB *)
Q3ColorRGB_Scale                (const TQ3ColorRGB *    color,
                                 float                  scale,
                                 TQ3ColorRGB *          result);

EXTERN_API_C( TQ3ColorRGB *)
Q3ColorRGB_Clamp                (const TQ3ColorRGB *    color,
                                 TQ3ColorRGB *          result);

EXTERN_API_C( TQ3ColorRGB *)
Q3ColorRGB_Lerp                 (const TQ3ColorRGB *    first,
                                 const TQ3ColorRGB *    last,
                                 float                  alpha,
                                 TQ3ColorRGB *          result);

EXTERN_API_C( TQ3ColorRGB *)
Q3ColorRGB_Accumulate           (const TQ3ColorRGB *    src,
                                 TQ3ColorRGB *          result);

#endif  /* CALL_NOT_IN_CARBON */

#if CALL_NOT_IN_CARBON
EXTERN_API_C( float *)
Q3ColorRGB_Luminance            (const TQ3ColorRGB *    color,
                                 float *                luminance);

#endif  /* CALL_NOT_IN_CARBON */

/******************************************************************************
 **                                                                          **
 **                             Shader Types                                 **
 **                                                                          **
 *****************************************************************************/

enum TQ3ShaderUVBoundary {
    kQ3ShaderUVBoundaryWrap     = 0,
    kQ3ShaderUVBoundaryClamp    = 1
};
typedef enum TQ3ShaderUVBoundary TQ3ShaderUVBoundary;


/******************************************************************************
 **                                                                          **
 **                             Shader Routines                              **
 **                                                                          **
 *****************************************************************************/
#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQ3ObjectType )
Q3Shader_GetType                (TQ3ShaderObject        shader);

EXTERN_API_C( TQ3Status )
Q3Shader_Submit                 (TQ3ShaderObject        shader,
                                 TQ3ViewObject          view);

EXTERN_API_C( TQ3Status )
Q3Shader_SetUVTransform         (TQ3ShaderObject        shader,
                                 const TQ3Matrix3x3 *   uvTransform);

EXTERN_API_C( TQ3Status )
Q3Shader_GetUVTransform         (TQ3ShaderObject        shader,
                                 TQ3Matrix3x3 *         uvTransform);

EXTERN_API_C( TQ3Status )
Q3Shader_SetUBoundary           (TQ3ShaderObject        shader,
                                 TQ3ShaderUVBoundary    uBoundary);

EXTERN_API_C( TQ3Status )
Q3Shader_SetVBoundary           (TQ3ShaderObject        shader,
                                 TQ3ShaderUVBoundary    vBoundary);

EXTERN_API_C( TQ3Status )
Q3Shader_GetUBoundary           (TQ3ShaderObject        shader,
                                 TQ3ShaderUVBoundary *  uBoundary);

EXTERN_API_C( TQ3Status )
Q3Shader_GetVBoundary           (TQ3ShaderObject        shader,
                                 TQ3ShaderUVBoundary *  vBoundary);


/******************************************************************************
 **                                                                          **
 **                         Illumination Shader Classes                      **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( TQ3ObjectType )
Q3IlluminationShader_GetType    (TQ3ShaderObject        shader);

EXTERN_API_C( TQ3ShaderObject )
Q3PhongIllumination_New         (void);

EXTERN_API_C( TQ3ShaderObject )
Q3LambertIllumination_New       (void);

EXTERN_API_C( TQ3ShaderObject )
Q3NULLIllumination_New          (void);


/******************************************************************************
 **                                                                          **
 **                              Surface Shader                              **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( TQ3ObjectType )
Q3SurfaceShader_GetType         (TQ3SurfaceShaderObject  shader);


/******************************************************************************
 **                                                                          **
 **                             Texture Shader                               **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( TQ3ShaderObject )
Q3TextureShader_New             (TQ3TextureObject       texture);

EXTERN_API_C( TQ3Status )
Q3TextureShader_GetTexture      (TQ3ShaderObject        shader,
                                 TQ3TextureObject *     texture);

EXTERN_API_C( TQ3Status )
Q3TextureShader_SetTexture      (TQ3ShaderObject        shader,
                                 TQ3TextureObject       texture);


/******************************************************************************
 **                                                                          **
 **                             Texture Objects                              **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( TQ3ObjectType )
Q3Texture_GetType               (TQ3TextureObject       texture);

EXTERN_API_C( TQ3Status )
Q3Texture_GetWidth              (TQ3TextureObject       texture,
                                 unsigned long *        width);

EXTERN_API_C( TQ3Status )
Q3Texture_GetHeight             (TQ3TextureObject       texture,
                                 unsigned long *        height);


/******************************************************************************
 **                                                                          **
 **                             Pixmap Texture                               **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( TQ3TextureObject )
Q3PixmapTexture_New             (const TQ3StoragePixmap * pixmap);

EXTERN_API_C( TQ3Status )
Q3PixmapTexture_GetPixmap       (TQ3TextureObject       texture,
                                 TQ3StoragePixmap *     pixmap);

EXTERN_API_C( TQ3Status )
Q3PixmapTexture_SetPixmap       (TQ3TextureObject       texture,
                                 const TQ3StoragePixmap * pixmap);


/******************************************************************************
 **                                                                          **
 **                             Mipmap Texture                               **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( TQ3TextureObject )
Q3MipmapTexture_New             (const TQ3Mipmap *      mipmap);

EXTERN_API_C( TQ3Status )
Q3MipmapTexture_GetMipmap       (TQ3TextureObject       texture,
                                 TQ3Mipmap *            mipmap);

EXTERN_API_C( TQ3Status )
Q3MipmapTexture_SetMipmap       (TQ3TextureObject       texture,
                                 const TQ3Mipmap *      mipmap);

/******************************************************************************
 **                                                                          **
 **                 Compressed Pixmap Texture                                **
 **                                                                          **
 *****************************************************************************/
EXTERN_API_C( TQ3TextureObject )
Q3CompressedPixmapTexture_New   (const TQ3CompressedPixmap * compressedPixmap);

EXTERN_API_C( TQ3Status )
Q3CompressedPixmapTexture_GetCompressedPixmap (TQ3TextureObject  texture,
                                 TQ3CompressedPixmap *  compressedPixmap);

EXTERN_API_C( TQ3Status )
Q3CompressedPixmapTexture_SetCompressedPixmap (TQ3TextureObject  texture,
                                 const TQ3CompressedPixmap * compressedPixmap);

EXTERN_API_C( TQ3Status )
Q3CompressedPixmapTexture_CompressImage (TQ3CompressedPixmap * compressedPixmap,
                                 PixMapHandle           sourcePixMap,
                                 CodecType              codecType,
                                 CodecComponent         codecComponent,
                                 short                  codedDepth,
                                 CodecQ                 codecQuality);






#endif  /* CALL_NOT_IN_CARBON */


#if PRAGMA_ENUM_ALWAYSINT
    #pragma enumsalwaysint reset
    #ifdef __QD3DSHADER__RESTORE_TWOBYTEINTS
        #pragma fourbyteints off
    #endif
#elif PRAGMA_ENUM_OPTIONS
    #pragma option enum=reset
#elif defined(__QD3DSHADER__RESTORE_PACKED_ENUMS)
    #pragma options(pack_enums)
#endif

#if PRAGMA_STRUCT_ALIGN
    #pragma options align=reset
#elif PRAGMA_STRUCT_PACKPUSH
    #pragma pack(pop)
#elif PRAGMA_STRUCT_PACK
    #pragma pack()
#endif

#ifdef PRAGMA_IMPORT_OFF
#pragma import off
#elif PRAGMA_IMPORT
#pragma import reset
#endif

#ifdef __cplusplus
}
#endif

#endif /* __QD3DSHADER__ */

