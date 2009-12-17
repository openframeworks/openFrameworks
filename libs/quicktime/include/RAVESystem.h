/*
     File:       RAVESystem.h
 
     Contains:   Interfaces needed when building RAVE engines
 
     Version:    Technology: Quickdraw 3D 1.6
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1995-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __RAVESYSTEM__
#define __RAVESYSTEM__

#ifndef __CONDITIONALMACROS__
#include <ConditionalMacros.h>
#endif

#ifndef __RAVE__
#include <RAVE.h>
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
        #define __RAVESYSTEM__RESTORE_TWOBYTEINTS
        #pragma fourbyteints on
    #endif
    #pragma enumsalwaysint on
#elif PRAGMA_ENUM_OPTIONS
    #pragma option enum=int
#elif PRAGMA_ENUM_PACK
    #if __option(pack_enums)
        #define __RAVESYSTEM__RESTORE_PACKED_ENUMS
        #pragma options(!pack_enums)
    #endif
#endif


/************************************************************************************************
 *
 * Typedefs of texture/bitmap method functions provided by the drawing engine.
 *
 ***********************************************************************************************/
/* TQAColorTableNew parameter descriptions */
/* TQAColorTableType    pixelType           Depth, color space, etc. */
/* void                 *pixelData          lookup table entries in pixelType format */
/* long                 transparentIndex    boolean, false means no transparency, true means index 0 is transparent */
/* TQAColorTable        **newTable          (Out) Newly created TQAColorTable */
typedef CALLBACK_API_C( TQAError , TQAColorTableNew )(TQAColorTableType pixelType, void *pixelData, long transparentIndex, TQAColorTable **newTable);
/* TQAColorTableDelete  parameter descriptions */
/* TQAColorTable        *colorTable     Previously allocated by QAColorTableNew() */
typedef CALLBACK_API_C( void , TQAColorTableDelete )(TQAColorTable *colorTable);
/* TQATextureNew    parameter descriptions */
/*  unsigned long       flags               Mask of kQATexture_xxx flags */
/*  TQAImagePixelType   pixelType           Depth, color space, etc. */
/*  const TQAImage      images[]            Image(s) for texture */
/*  TQATexture          **newTexture        (Out) Newly created TQATexture, or NULL on error */
typedef CALLBACK_API_C( TQAError , TQATextureNew )(unsigned long flags, TQAImagePixelType pixelType, const TQAImage images[], TQATexture **newTexture);
/* TQATextureDetach parameter descriptions */
/*  TQATexture          *texture            Previously allocated by QATextureNew() */
typedef CALLBACK_API_C( TQAError , TQATextureDetach )(TQATexture *texture);
/* TQATextureDelete parameter descriptions */
/*  TQATexture          *texture            Previously allocated by QATextureNew() */
typedef CALLBACK_API_C( void , TQATextureDelete )(TQATexture *texture);
/* TQATextureBindColorTable parameter descriptions */
/*  TQATexture          *texture            Previously allocated by QATextureNew() */
/*  TQAColorTable       *colorTable         Previously allocated by QAColorTableNew() */
typedef CALLBACK_API_C( TQAError , TQATextureBindColorTable )(TQATexture *texture, TQAColorTable *colorTable);
/* TQABitmapNew parameter descriptions */
/*  unsigned long       flags               Mask of kQABitmap_xxx flags */
/*  TQAImagePixelType   pixelType           Depth, color space, etc. */
/*  const TQAImage      *image              Image */
/*  TQABitmap           **newBitmap         (Out) Newly created TQABitmap, or NULL on error */
typedef CALLBACK_API_C( TQAError , TQABitmapNew )(unsigned long flags, TQAImagePixelType pixelType, const TQAImage *image, TQABitmap **newBitmap);
/* TQABitmapDetach  parameter descriptions */
/*  TQABitmap           *bitmap         Previously allocated by QABitmapNew() */
typedef CALLBACK_API_C( TQAError , TQABitmapDetach )(TQABitmap *bitmap);
/* TQABitmapDelete  parameter descriptions */
/*  TQABitmap           *bitmap         Previously allocated by QABitmapNew() */
typedef CALLBACK_API_C( void , TQABitmapDelete )(TQABitmap *bitmap);
/* TQABitmapBindColorTable  parameter descriptions */
/*  TQABitmap           *bitmap         Previously allocated by QABitmapNew() */
/*  TQAColorTable       *colorTable     Previously allocated by QAColorTableNew() */
typedef CALLBACK_API_C( TQAError , TQABitmapBindColorTable )(TQABitmap *bitmap, TQAColorTable *colorTable);
/************************************************************************************************
 *
 * Typedefs of private (system-only) functions provided by the drawing engine.
 *
 * The TQADrawPrivateNew function returns a TQADrawPrivate *, which points to the
 * engine-specific private data created for the context. (TQADrawPrivate is a dummy
 * type which is then cast to the correct engine-specific datatype by the engine code.)
 *
 * The TQADrawPrivateDelete function deletes the engine-specific private data.
 *
 * TQAStorePrivateNew and TQAStorePrivateDelete provide the same function as QADrawPrivateNew
 * and TQADrawPrivateDelete, but for the texture and bitmap storage context.
 *
 * TQADrawMethodGet and TQAStoreMethodGet are called by the RAVE manager to retrieve
 * the method pointers for a drawing engine.
 *
 * The TQAEngineCheckDevice function returns TRUE if the engine can render to the
 * indicated GDevice.
 *
 ***********************************************************************************************/
/* TQADrawPrivateNew    parameter descriptions */
/*  TQADrawContext      *newDrawContext     Draw context to initialize */
/*  const TQADevice     *device             Target device */
/*  const TQARect       *rect               Target rectangle (device coordinates) */
/*  const TQAClip       *clip               2D clip region (or NULL) */
/*  unsigned long       flags               Mask of kQAContext_xxx */
typedef CALLBACK_API_C( TQAError , TQADrawPrivateNew )(TQADrawContext *newDrawContext, const TQADevice *device, const TQARect *rect, const TQAClip *clip, unsigned long flags);
/* TQADrawPrivateDelete parameter descriptions */
/*  TQADrawPrivate      *drawPrivate        Private context data to delete */
typedef CALLBACK_API_C( void , TQADrawPrivateDelete )(TQADrawPrivate *drawPrivate);
/* TQAEngineCheckDevice parameter descriptions */
/*  const TQADevice     *device         Target device */
typedef CALLBACK_API_C( TQAError , TQAEngineCheckDevice )(const TQADevice *device);
/* TQAEngineGestalt parameter descriptions */
/*  TQAGestaltSelector  selector            Gestalt parameter being requested */
/*  void                *response           Buffer that receives response */
typedef CALLBACK_API_C( TQAError , TQAEngineGestalt )(TQAGestaltSelector selector, void *response);

/* new engine methods for RAVE 1.6 */
typedef CALLBACK_API_C( TQAError , TQAAccessTexture )(TQATexture *texture, long mipmapLevel, long flags, TQAPixelBuffer *buffer);
typedef CALLBACK_API_C( TQAError , TQAAccessTextureEnd )(TQATexture *texture, const TQARect *dirtyRect);
typedef CALLBACK_API_C( TQAError , TQAAccessBitmap )(TQABitmap *bitmap, long flags, TQAPixelBuffer *buffer);
typedef CALLBACK_API_C( TQAError , TQAAccessBitmapEnd )(TQABitmap *bitmap, const TQARect *dirtyRect);
/************************************************************************************************
 *
 * The TQAEngineMethod union is used to represent a single engine method (it's a
 * parameter to QAEngineGetMethod). TQAEngineMethodTag identifies which method is being
 * requested.
 *
 ***********************************************************************************************/

union TQAEngineMethod {
    TQADrawPrivateNew               drawPrivateNew;             /* Method: Create a private draw context */
    TQADrawPrivateDelete            drawPrivateDelete;          /* Method: Delete a private draw context */
    TQAEngineCheckDevice            engineCheckDevice;          /* Method: Check a device for drawing */
    TQAEngineGestalt                engineGestalt;              /* Method: Gestalt */
    TQATextureNew                   textureNew;                 /* Method: Create a texture (load is non-blocking) */
    TQATextureDetach                textureDetach;              /* Method: Complete load of a texture (blocking) */
    TQATextureDelete                textureDelete;              /* Method: Delete a texture */
    TQABitmapNew                    bitmapNew;                  /* Method: Create a bitmap (load is non-blocking)  */
    TQABitmapDetach                 bitmapDetach;               /* Method: Complete load of a bitmap (blocking) */
    TQABitmapDelete                 bitmapDelete;               /* Method: Delete a bitmap */
    TQAColorTableNew                colorTableNew;              /* Method: Create a new color table */
    TQAColorTableDelete             colorTableDelete;           /* Method: Create a new color table */
    TQATextureBindColorTable        textureBindColorTable;      /* Method: Bind a CLUT to a texture */
    TQABitmapBindColorTable         bitmapBindColorTable;       /* Method: Bind a CLUT to a bitmap */
    TQAAccessTexture                accessTexture;
    TQAAccessTextureEnd             accessTextureEnd;
    TQAAccessBitmap                 accessBitmap;
    TQAAccessBitmapEnd              accessBitmapEnd;
};
typedef union TQAEngineMethod           TQAEngineMethod;

enum TQAEngineMethodTag {
    kQADrawPrivateNew           = 0,
    kQADrawPrivateDelete        = 1,
    kQAEngineCheckDevice        = 2,
    kQAEngineGestalt            = 3,
    kQATextureNew               = 4,
    kQATextureDetach            = 5,
    kQATextureDelete            = 6,
    kQABitmapNew                = 7,
    kQABitmapDetach             = 8,
    kQABitmapDelete             = 9,
    kQAColorTableNew            = 10,
    kQAColorTableDelete         = 11,
    kQATextureBindColorTable    = 12,
    kQABitmapBindColorTable     = 13,
    kQAAccessTexture            = 14,
    kQAAccessTextureEnd         = 15,
    kQAAccessBitmap             = 16,
    kQAAccessBitmapEnd          = 17
};
typedef enum TQAEngineMethodTag TQAEngineMethodTag;

/************************************************************************************************
 *
 * QARegisterEngine() registers a new engine. This is called at boot time by the drawing engine
 * initialization code to register itself with the system. This call takes only one parameter,
 * the engine's function that allows the manager to request the other methods.
 *
 ***********************************************************************************************/
/* TQAEngineGetMethod   parameter descriptions */
/*  TQAEngineMethodTag      methodTag               Method being requested */
/*  TQAEngineMethod         *method                 (Out) Method */
typedef CALLBACK_API_C( TQAError , TQAEngineGetMethod )(TQAEngineMethodTag methodTag, TQAEngineMethod *method);
/* QARegisterEngine parameter descriptions */
/*  TQAEngineGetMethod      engineGetMethod     Engine's getMethod method */
#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQAError )
QARegisterEngine                (TQAEngineGetMethod     engineGetMethod);


/* QARegisterEngineWithRefCon parameter descriptions */
/*  TQAEngineGetMethod      engineGetMethod     Engine's getMethod method */
/*  long                    refCon              Engine RefCon */
EXTERN_API_C( TQAError )
QARegisterEngineWithRefCon      (TQAEngineGetMethod     engineGetMethod,
                                 long                   refCon);

/* QAGetEngineRefCon parameter descriptions */
EXTERN_API_C( long )
QAGetCurrentEngineRefCon        (void);


/************************************************************************************************
 *
 * The TQADrawMethod union is used to represent a single draw context method (it's a
 * parameter to QARegisterDrawMethod). TQADrawMethodTag identifies which method is being
 * passed.
 *
 ***********************************************************************************************/
#endif  /* CALL_NOT_IN_CARBON */


union TQADrawMethod {
    TQASetFloat                     setFloat;                   /* Method: Set a float state variable */
    TQASetInt                       setInt;                     /* Method: Set an unsigned long state variable */
    TQASetPtr                       setPtr;                     /* Method: Set an unsigned long state variable */
    TQAGetFloat                     getFloat;                   /* Method: Get a float state variable */
    TQAGetInt                       getInt;                     /* Method: Get an unsigned long state variable */
    TQAGetPtr                       getPtr;                     /* Method: Get an pointer state variable */
    TQADrawPoint                    drawPoint;                  /* Method: Draw a point */
    TQADrawLine                     drawLine;                   /* Method: Draw a line */
    TQADrawTriGouraud               drawTriGouraud;             /* Method: Draw a Gouraud shaded triangle */
    TQADrawTriTexture               drawTriTexture;             /* Method: Draw a texture mapped triangle */
    TQADrawVGouraud                 drawVGouraud;               /* Method: Draw Gouraud vertices */
    TQADrawVTexture                 drawVTexture;               /* Method: Draw texture vertices */
    TQADrawBitmap                   drawBitmap;                 /* Method: Draw a bitmap */
    TQARenderStart                  renderStart;                /* Method: Initialize for rendering */
    TQARenderEnd                    renderEnd;                  /* Method: Complete rendering and display */
    TQARenderAbort                  renderAbort;                /* Method: Abort any outstanding rendering (blocking) */
    TQAFlush                        flush;                      /* Method: Start render of any queued commands (non-blocking) */
    TQASync                         sync;                       /* Method: Wait for completion of all rendering (blocking) */
    TQASubmitVerticesGouraud        submitVerticesGouraud;      /* Method: Submit Gouraud vertices for trimesh */
    TQASubmitVerticesTexture        submitVerticesTexture;      /* Method: Submit Texture vertices for trimesh */
    TQADrawTriMeshGouraud           drawTriMeshGouraud;         /* Method: Draw a Gouraud triangle mesh */
    TQADrawTriMeshTexture           drawTriMeshTexture;         /* Method: Draw a Texture triangle mesh */
    TQASetNoticeMethod              setNoticeMethod;            /* Method: Set a notice method */
    TQAGetNoticeMethod              getNoticeMethod;            /* Method: Get a notice method */

                                                                /* new in 1.6 */
    TQASubmitMultiTextureParams     submitMultiTextureParams;   /* Method: Submit secondary texture params */
    TQAAccessDrawBuffer             accessDrawBuffer;
    TQAAccessDrawBufferEnd          accessDrawBufferEnd;
    TQAAccessZBuffer                accessZBuffer;
    TQAAccessZBufferEnd             accessZBufferEnd;
    TQAClearDrawBuffer              clearDrawBuffer;
    TQAClearZBuffer                 clearZBuffer;
    TQATextureNewFromDrawContext    textureFromContext;
    TQABitmapNewFromDrawContext     bitmapFromContext;
    TQABusy                         busy;
    TQASwapBuffers                  swapBuffers;
};
typedef union TQADrawMethod             TQADrawMethod;

enum TQADrawMethodTag {
    kQASetFloat                 = 0,
    kQASetInt                   = 1,
    kQASetPtr                   = 2,
    kQAGetFloat                 = 3,
    kQAGetInt                   = 4,
    kQAGetPtr                   = 5,
    kQADrawPoint                = 6,
    kQADrawLine                 = 7,
    kQADrawTriGouraud           = 8,
    kQADrawTriTexture           = 9,
    kQADrawVGouraud             = 10,
    kQADrawVTexture             = 11,
    kQADrawBitmap               = 12,
    kQARenderStart              = 13,
    kQARenderEnd                = 14,
    kQARenderAbort              = 15,
    kQAFlush                    = 16,
    kQASync                     = 17,
    kQASubmitVerticesGouraud    = 18,
    kQASubmitVerticesTexture    = 19,
    kQADrawTriMeshGouraud       = 20,
    kQADrawTriMeshTexture       = 21,
    kQASetNoticeMethod          = 22,
    kQAGetNoticeMethod          = 23,
    kQSubmitMultiTextureParams  = 24,
    kQAccessDrawBuffer          = 25,
    kQAccessDrawBufferEnd       = 26,
    kQAccessZBuffer             = 27,
    kQAccessZBufferEnd          = 28,
    kQClearDrawBuffer           = 29,
    kQClearZBuffer              = 30,
    kQTextureNewFromDrawContext = 31,
    kQBitmapNewFromDrawContext  = 32,
    kQBusy                      = 33,
    kQSwapBuffers               = 34
};
typedef enum TQADrawMethodTag TQADrawMethodTag;

/************************************************************************************************
 *
 * System call to register a new method for an engine. This is called during the engine's
 * draw private new functions (to set the initial value of the draw methods), and possibly
 * at other times when the engine needs to change a draw method.
 *
 ***********************************************************************************************/
/* QARegisterDrawMethod parameter descriptions */
/*  TQADrawContext          *drawContext            Draw context in which to set method */
/*  TQADrawMethodTag        methodTag               Method to set */
/*  TQADrawMethod           method                  Method */
#if CALL_NOT_IN_CARBON
EXTERN_API_C( TQAError )
QARegisterDrawMethod            (TQADrawContext *       drawContext,
                                 TQADrawMethodTag       methodTag,
                                 TQADrawMethod          method);







#endif  /* CALL_NOT_IN_CARBON */


#if PRAGMA_ENUM_ALWAYSINT
    #pragma enumsalwaysint reset
    #ifdef __RAVESYSTEM__RESTORE_TWOBYTEINTS
        #pragma fourbyteints off
    #endif
#elif PRAGMA_ENUM_OPTIONS
    #pragma option enum=reset
#elif defined(__RAVESYSTEM__RESTORE_PACKED_ENUMS)
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

#endif /* __RAVESYSTEM__ */

