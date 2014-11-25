/*
 * << Haru Free PDF Library 2.0.8 >> -- hpdf.h
 *
 * URL http://libharu.sourceforge.net/
 *
 * Copyright (c) 1999-2006 Takeshi Kanno
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.
 * It is provided "as is" without express or implied warranty.
 *
 */

#ifndef _HPDF_H
#define _HPDF_H

#ifdef HPDF_DLL_MAKE
#    define HPDF_EXPORT(A)  __declspec(dllexport) A  __stdcall
#else
#    ifdef HPDF_DLL_MAKE_CDECL
#        define HPDF_EXPORT(A)  __declspec(dllexport) A
#    else
#        ifdef HPDF_SHARED_MAKE
#            define HPDF_EXPORT(A)  extern A
#        endif /* HPDF_SHARED_MAKE */
#    endif /* HPDF_DLL_MAKE_CDECL */
#endif /* HPDF_DLL_MAKE */

#ifdef HPDF_DLL
#    define HPDF_SHARED
#    define HPDF_EXPORT(A)  __declspec(dllimport) A  __stdcall
#else
#    ifdef HPDF_DLL_CDECL
#        define HPDF_SHARED
#        define HPDF_EXPORT(A)  __declspec(dllimport) A
#    endif /* HPDF_DLL_CDECL */
#endif /* HPDF_DLL */

#ifdef HPDF_SHARED

#ifndef HPDF_EXPORT
#define HPDF_EXPORT(A) extern A
#endif /* HPDF_EXPORT */

#include "hpdf_consts.h"
#include "hpdf_types.h"

typedef void         *HPDF_HANDLE;
typedef HPDF_HANDLE   HPDF_Doc;
typedef HPDF_HANDLE   HPDF_Page;
typedef HPDF_HANDLE   HPDF_Pages;
typedef HPDF_HANDLE   HPDF_Stream;
typedef HPDF_HANDLE   HPDF_Image;
typedef HPDF_HANDLE   HPDF_Font;
typedef HPDF_HANDLE   HPDF_Outline;
typedef HPDF_HANDLE   HPDF_Encoder;
typedef HPDF_HANDLE   HPDF_Destination;
typedef HPDF_HANDLE   HPDF_XObject;
typedef HPDF_HANDLE   HPDF_Annotation;
typedef HPDF_HANDLE   HPDF_ExtGState;

#else

#ifndef HPDF_EXPORT
#define HPDF_EXPORT(A)  A
#endif /* HPDF_EXPORT  */

#include "hpdf_consts.h"
#include "hpdf_doc.h"

#endif /* HPDF_SHARED */

#ifdef __cplusplus
extern "C" {
#endif

HPDF_EXPORT(const char *)
HPDF_GetVersion  (void);


HPDF_EXPORT(HPDF_Doc)
HPDF_NewEx  (HPDF_Error_Handler   user_error_fn,
             HPDF_Alloc_Func      user_alloc_fn,
             HPDF_Free_Func       user_free_fn,
             HPDF_UINT            mem_pool_buf_size,
             void                *user_data);

HPDF_EXPORT(HPDF_Doc)
HPDF_New  (HPDF_Error_Handler   user_error_fn,
           void                *user_data);


HPDF_EXPORT(HPDF_STATUS)
HPDF_SetErrorHandler  (HPDF_Doc            pdf,
                       HPDF_Error_Handler  user_error_fn);


HPDF_EXPORT(void)
HPDF_Free  (HPDF_Doc  pdf);


HPDF_EXPORT(HPDF_STATUS)
HPDF_NewDoc  (HPDF_Doc  pdf);


HPDF_EXPORT(void)
HPDF_FreeDoc  (HPDF_Doc  pdf);


HPDF_EXPORT(HPDF_BOOL)
HPDF_HasDoc  (HPDF_Doc  pdf);


HPDF_EXPORT(void)
HPDF_FreeDocAll  (HPDF_Doc  pdf);


HPDF_EXPORT(HPDF_STATUS)
HPDF_SaveToStream  (HPDF_Doc   pdf);


HPDF_EXPORT(HPDF_UINT32)
HPDF_GetStreamSize  (HPDF_Doc   pdf);


HPDF_EXPORT(HPDF_STATUS)
HPDF_ReadFromStream  (HPDF_Doc       pdf,
                      HPDF_BYTE     *buf,
                      HPDF_UINT32   *size);


HPDF_EXPORT(HPDF_STATUS)
HPDF_ResetStream  (HPDF_Doc     pdf);


HPDF_EXPORT(HPDF_STATUS)
HPDF_SaveToFile  (HPDF_Doc     pdf,
                  const char  *file_name);


HPDF_EXPORT(HPDF_STATUS)
HPDF_GetError  (HPDF_Doc   pdf);


HPDF_EXPORT(HPDF_STATUS)
HPDF_GetErrorDetail  (HPDF_Doc   pdf);

HPDF_EXPORT(void)
HPDF_ResetError  (HPDF_Doc   pdf);


HPDF_EXPORT(HPDF_STATUS)
HPDF_SetPagesConfiguration  (HPDF_Doc    pdf,
                             HPDF_UINT   page_per_pages);


HPDF_EXPORT(HPDF_Page)
HPDF_GetPageByIndex  (HPDF_Doc    pdf,
                      HPDF_UINT   index);


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

HPDF_EXPORT(HPDF_PageLayout)
HPDF_GetPageLayout  (HPDF_Doc   pdf);


HPDF_EXPORT(HPDF_STATUS)
HPDF_SetPageLayout  (HPDF_Doc          pdf,
                     HPDF_PageLayout   layout);


HPDF_EXPORT(HPDF_PageMode)
HPDF_GetPageMode  (HPDF_Doc   pdf);


HPDF_EXPORT(HPDF_STATUS)
HPDF_SetPageMode  (HPDF_Doc        pdf,
                   HPDF_PageMode   mode);


HPDF_EXPORT(HPDF_UINT)
HPDF_GetViewerPreference  (HPDF_Doc   pdf);


HPDF_EXPORT(HPDF_STATUS)
HPDF_SetViewerPreference  (HPDF_Doc     pdf,
                           HPDF_UINT    value);


HPDF_EXPORT(HPDF_STATUS)
HPDF_SetOpenAction  (HPDF_Doc           pdf,
                     HPDF_Destination   open_action);


/*---------------------------------------------------------------------------*/
/*----- page handling -------------------------------------------------------*/


HPDF_EXPORT(HPDF_Page)
HPDF_GetCurrentPage  (HPDF_Doc  pdf);


HPDF_EXPORT(HPDF_Page)
HPDF_AddPage  (HPDF_Doc   pdf);


HPDF_EXPORT(HPDF_Page)
HPDF_InsertPage  (HPDF_Doc    pdf,
                  HPDF_Page   page);


HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_SetWidth  (HPDF_Page   page,
                     HPDF_REAL   value);


HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_SetHeight  (HPDF_Page   page,
                      HPDF_REAL   value);


HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_SetSize  (HPDF_Page            page,
                    HPDF_PageSizes       size,
                    HPDF_PageDirection   direction);

HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_SetRotate  (HPDF_Page     page,
                      HPDF_UINT16   angle);

/*---------------------------------------------------------------------------*/
/*----- font handling -------------------------------------------------------*/


HPDF_EXPORT(HPDF_Font)
HPDF_GetFont  (HPDF_Doc     pdf,
               const char  *font_name,
               const char  *encoding_name);


HPDF_EXPORT(const char*)
HPDF_LoadType1FontFromFile  (HPDF_Doc     pdf,
                             const char  *afm_file_name,
                             const char  *data_file_name);


HPDF_EXPORT(const char*)
HPDF_LoadTTFontFromFile (HPDF_Doc     pdf,
                         const char  *file_name,
                         HPDF_BOOL    embedding);


HPDF_EXPORT(const char*)
HPDF_LoadTTFontFromFile2 (HPDF_Doc     pdf,
                          const char  *file_name,
                          HPDF_UINT    index,
                          HPDF_BOOL    embedding);


HPDF_EXPORT(HPDF_STATUS)
HPDF_AddPageLabel  (HPDF_Doc            pdf,
                    HPDF_UINT           page_num,
                    HPDF_PageNumStyle   style,
                    HPDF_UINT           first_page,
                    const char         *prefix);


HPDF_EXPORT(HPDF_STATUS)
HPDF_UseJPFonts   (HPDF_Doc   pdf);


HPDF_EXPORT(HPDF_STATUS)
HPDF_UseKRFonts   (HPDF_Doc   pdf);


HPDF_EXPORT(HPDF_STATUS)
HPDF_UseCNSFonts   (HPDF_Doc   pdf);


HPDF_EXPORT(HPDF_STATUS)
HPDF_UseCNTFonts   (HPDF_Doc   pdf);


/*--------------------------------------------------------------------------*/
/*----- outline ------------------------------------------------------------*/


HPDF_EXPORT(HPDF_Outline)
HPDF_CreateOutline  (HPDF_Doc       pdf,
                     HPDF_Outline   parent,
                     const char    *title,
                     HPDF_Encoder   encoder);


HPDF_EXPORT(HPDF_STATUS)
HPDF_Outline_SetOpened  (HPDF_Outline  outline,
                         HPDF_BOOL     opened);


HPDF_EXPORT(HPDF_STATUS)
HPDF_Outline_SetDestination (HPDF_Outline      outline,
                             HPDF_Destination  dst);


/*--------------------------------------------------------------------------*/
/*----- destination --------------------------------------------------------*/

HPDF_EXPORT(HPDF_Destination)
HPDF_Page_CreateDestination  (HPDF_Page   page);


HPDF_EXPORT(HPDF_STATUS)
HPDF_Destination_SetXYZ  (HPDF_Destination  dst,
                          HPDF_REAL         left,
                          HPDF_REAL         top,
                          HPDF_REAL         zoom);


HPDF_EXPORT(HPDF_STATUS)
HPDF_Destination_SetFit  (HPDF_Destination  dst);


HPDF_EXPORT(HPDF_STATUS)
HPDF_Destination_SetFitH  (HPDF_Destination  dst,
                           HPDF_REAL         top);


HPDF_EXPORT(HPDF_STATUS)
HPDF_Destination_SetFitV  (HPDF_Destination  dst,
                           HPDF_REAL         left);


HPDF_EXPORT(HPDF_STATUS)
HPDF_Destination_SetFitR  (HPDF_Destination  dst,
                           HPDF_REAL         left,
                           HPDF_REAL         bottom,
                           HPDF_REAL         right,
                           HPDF_REAL         top);


HPDF_EXPORT(HPDF_STATUS)
HPDF_Destination_SetFitB  (HPDF_Destination  dst);


HPDF_EXPORT(HPDF_STATUS)
HPDF_Destination_SetFitBH  (HPDF_Destination  dst,
                            HPDF_REAL         top);


HPDF_EXPORT(HPDF_STATUS)
HPDF_Destination_SetFitBV  (HPDF_Destination  dst,
                            HPDF_REAL         left);

/*--------------------------------------------------------------------------*/
/*----- encoder ------------------------------------------------------------*/

HPDF_EXPORT(HPDF_Encoder)
HPDF_GetEncoder  (HPDF_Doc     pdf,
                  const char  *encoding_name);


HPDF_EXPORT(HPDF_Encoder)
HPDF_GetCurrentEncoder  (HPDF_Doc   pdf);


HPDF_EXPORT(HPDF_STATUS)
HPDF_SetCurrentEncoder  (HPDF_Doc     pdf,
                         const char  *encoding_name);


HPDF_EXPORT(HPDF_EncoderType)
HPDF_Encoder_GetType  (HPDF_Encoder   encoder);


HPDF_EXPORT(HPDF_ByteType)
HPDF_Encoder_GetByteType  (HPDF_Encoder    encoder,
                           const char     *text,
                           HPDF_UINT       index);


HPDF_EXPORT(HPDF_UNICODE)
HPDF_Encoder_GetUnicode  (HPDF_Encoder   encoder,
                          HPDF_UINT16    code);


HPDF_EXPORT(HPDF_WritingMode)
HPDF_Encoder_GetWritingMode (HPDF_Encoder   encoder);


HPDF_EXPORT(HPDF_STATUS)
HPDF_UseJPEncodings   (HPDF_Doc   pdf);



HPDF_EXPORT(HPDF_STATUS)
HPDF_UseKREncodings   (HPDF_Doc   pdf);



HPDF_EXPORT(HPDF_STATUS)
HPDF_UseCNSEncodings   (HPDF_Doc   pdf);



HPDF_EXPORT(HPDF_STATUS)
HPDF_UseCNTEncodings   (HPDF_Doc   pdf);


/*--------------------------------------------------------------------------*/
/*----- annotation ---------------------------------------------------------*/

HPDF_EXPORT(HPDF_Annotation)
HPDF_Page_CreateTextAnnot  (HPDF_Page       page,
                            HPDF_Rect       rect,
                            const char     *text,
                            HPDF_Encoder    encoder);


HPDF_EXPORT(HPDF_Annotation)
HPDF_Page_CreateLinkAnnot  (HPDF_Page          page,
                            HPDF_Rect          rect,
                            HPDF_Destination   dst);


HPDF_EXPORT(HPDF_Annotation)
HPDF_Page_CreateURILinkAnnot  (HPDF_Page     page,
                               HPDF_Rect     rect,
                               const char   *uri);


HPDF_EXPORT(HPDF_STATUS)
HPDF_LinkAnnot_SetHighlightMode  (HPDF_Annotation           annot,
                                  HPDF_AnnotHighlightMode   mode);


HPDF_EXPORT(HPDF_STATUS)
HPDF_LinkAnnot_SetBorderStyle  (HPDF_Annotation  annot,
                                HPDF_REAL        width,
                                HPDF_UINT16      dash_on,
                                HPDF_UINT16      dash_off);


HPDF_EXPORT(HPDF_STATUS)
HPDF_TextAnnot_SetIcon  (HPDF_Annotation   annot,
                         HPDF_AnnotIcon    icon);


HPDF_EXPORT(HPDF_STATUS)
HPDF_TextAnnot_SetOpened  (HPDF_Annotation   annot,
                          HPDF_BOOL          opened);



/*--------------------------------------------------------------------------*/
/*----- image data ---------------------------------------------------------*/

HPDF_EXPORT(HPDF_Image)
HPDF_LoadPngImageFromFile (HPDF_Doc      pdf,
                           const char    *filename);


HPDF_EXPORT(HPDF_Image)
HPDF_LoadPngImageFromFile2 (HPDF_Doc      pdf,
                            const char    *filename);


HPDF_EXPORT(HPDF_Image)
HPDF_LoadJpegImageFromFile (HPDF_Doc      pdf,
                            const char    *filename);


HPDF_EXPORT(HPDF_Image)
HPDF_LoadRawImageFromFile  (HPDF_Doc           pdf,
                            const char         *filename,
                            HPDF_UINT          width,
                            HPDF_UINT          height,
                            HPDF_ColorSpace    color_space);


HPDF_EXPORT(HPDF_Image)
HPDF_LoadRawImageFromMem  (HPDF_Doc           pdf,
                           const HPDF_BYTE   *buf,
                           HPDF_UINT          width,
                           HPDF_UINT          height,
                           HPDF_ColorSpace    color_space,
                           HPDF_UINT          bits_per_component);


HPDF_EXPORT(HPDF_Point)
HPDF_Image_GetSize (HPDF_Image  image);


HPDF_EXPORT(HPDF_STATUS)
HPDF_Image_GetSize2 (HPDF_Image  image, HPDF_Point *size);


HPDF_EXPORT(HPDF_UINT)
HPDF_Image_GetWidth  (HPDF_Image   image);


HPDF_EXPORT(HPDF_UINT)
HPDF_Image_GetHeight  (HPDF_Image   image);


HPDF_EXPORT(HPDF_UINT)
HPDF_Image_GetBitsPerComponent (HPDF_Image  image);


HPDF_EXPORT(const char*)
HPDF_Image_GetColorSpace (HPDF_Image  image);


HPDF_EXPORT(HPDF_STATUS)
HPDF_Image_SetColorMask (HPDF_Image   image,
                         HPDF_UINT    rmin,
                         HPDF_UINT    rmax,
                         HPDF_UINT    gmin,
                         HPDF_UINT    gmax,
                         HPDF_UINT    bmin,
                         HPDF_UINT    bmax);


HPDF_EXPORT(HPDF_STATUS)
HPDF_Image_SetMaskImage  (HPDF_Image   image,
                          HPDF_Image   mask_image);


/*--------------------------------------------------------------------------*/
/*----- info dictionary ----------------------------------------------------*/


HPDF_EXPORT(HPDF_STATUS)
HPDF_SetInfoAttr (HPDF_Doc        pdf,
                  HPDF_InfoType   type,
                  const char     *value);


HPDF_EXPORT(const char*)
HPDF_GetInfoAttr (HPDF_Doc        pdf,
                  HPDF_InfoType   type);


HPDF_EXPORT(HPDF_STATUS)
HPDF_SetInfoDateAttr (HPDF_Doc        pdf,
                      HPDF_InfoType   type,
                      HPDF_Date       value);


/*--------------------------------------------------------------------------*/
/*----- encryption ---------------------------------------------------------*/

HPDF_EXPORT(HPDF_STATUS)
HPDF_SetPassword  (HPDF_Doc      pdf,
                   const char   *owner_passwd,
                   const char   *user_passwd);


HPDF_EXPORT(HPDF_STATUS)
HPDF_SetPermission  (HPDF_Doc    pdf,
                     HPDF_UINT   permission);


HPDF_EXPORT(HPDF_STATUS)
HPDF_SetEncryptionMode  (HPDF_Doc           pdf,
                         HPDF_EncryptMode   mode,
                         HPDF_UINT          key_len);


/*--------------------------------------------------------------------------*/
/*----- compression --------------------------------------------------------*/

HPDF_EXPORT(HPDF_STATUS)
HPDF_SetCompressionMode  (HPDF_Doc    pdf,
                          HPDF_UINT   mode);


/*--------------------------------------------------------------------------*/
/*----- font ---------------------------------------------------------------*/

HPDF_EXPORT(const char*)
HPDF_Font_GetFontName  (HPDF_Font    font);


HPDF_EXPORT(const char*)
HPDF_Font_GetEncodingName  (HPDF_Font    font);


HPDF_EXPORT(HPDF_INT)
HPDF_Font_GetUnicodeWidth  (HPDF_Font       font,
                            HPDF_UNICODE    code);

HPDF_EXPORT(HPDF_Box)
HPDF_Font_GetBBox  (HPDF_Font    font);


HPDF_EXPORT(HPDF_INT)
HPDF_Font_GetAscent  (HPDF_Font  font);


HPDF_EXPORT(HPDF_INT)
HPDF_Font_GetDescent  (HPDF_Font  font);


HPDF_EXPORT(HPDF_UINT)
HPDF_Font_GetXHeight  (HPDF_Font  font);


HPDF_EXPORT(HPDF_UINT)
HPDF_Font_GetCapHeight  (HPDF_Font  font);


HPDF_EXPORT(HPDF_TextWidth)
HPDF_Font_TextWidth  (HPDF_Font          font,
                      const HPDF_BYTE   *text,
                      HPDF_UINT          len);


HPDF_EXPORT(HPDF_UINT)
HPDF_Font_MeasureText (HPDF_Font          font,
                       const HPDF_BYTE   *text,
                       HPDF_UINT          len,
                       HPDF_REAL          width,
                       HPDF_REAL          font_size,
                       HPDF_REAL          char_space,
                       HPDF_REAL          word_space,
                       HPDF_BOOL          wordwrap,
                       HPDF_REAL         *real_width);


/*--------------------------------------------------------------------------*/
/*----- extended graphics state --------------------------------------------*/

HPDF_EXPORT(HPDF_ExtGState)
HPDF_CreateExtGState  (HPDF_Doc  pdf);


HPDF_EXPORT(HPDF_STATUS)
HPDF_ExtGState_SetAlphaStroke  (HPDF_ExtGState   ext_gstate,
                                HPDF_REAL        value);


HPDF_EXPORT(HPDF_STATUS)
HPDF_ExtGState_SetAlphaFill  (HPDF_ExtGState   ext_gstate,
                              HPDF_REAL        value);



HPDF_EXPORT(HPDF_STATUS)
HPDF_ExtGState_SetBlendMode  (HPDF_ExtGState   ext_gstate,
                              HPDF_BlendMode   mode);


/*--------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------*/

HPDF_EXPORT(HPDF_REAL)
HPDF_Page_TextWidth  (HPDF_Page    page,
                      const char  *text);


HPDF_EXPORT(HPDF_UINT)
HPDF_Page_MeasureText  (HPDF_Page    page,
                        const char  *text,
                        HPDF_REAL    width,
                        HPDF_BOOL    wordwrap,
                        HPDF_REAL   *real_width);


HPDF_EXPORT(HPDF_REAL)
HPDF_Page_GetWidth  (HPDF_Page   page);


HPDF_EXPORT(HPDF_REAL)
HPDF_Page_GetHeight  (HPDF_Page   page);


HPDF_EXPORT(HPDF_UINT16)
HPDF_Page_GetGMode  (HPDF_Page   page);


HPDF_EXPORT(HPDF_Point)
HPDF_Page_GetCurrentPos  (HPDF_Page   page);


HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_GetCurrentPos2  (HPDF_Page    page,
                           HPDF_Point  *pos);


HPDF_EXPORT(HPDF_Point)
HPDF_Page_GetCurrentTextPos (HPDF_Page   page);


HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_GetCurrentTextPos2  (HPDF_Page    page,
                               HPDF_Point  *pos);


HPDF_EXPORT(HPDF_Font)
HPDF_Page_GetCurrentFont  (HPDF_Page   page);


HPDF_EXPORT(HPDF_REAL)
HPDF_Page_GetCurrentFontSize  (HPDF_Page   page);


HPDF_EXPORT(HPDF_TransMatrix)
HPDF_Page_GetTransMatrix  (HPDF_Page   page);


HPDF_EXPORT(HPDF_REAL)
HPDF_Page_GetLineWidth  (HPDF_Page   page);


HPDF_EXPORT(HPDF_LineCap)
HPDF_Page_GetLineCap  (HPDF_Page   page);


HPDF_EXPORT(HPDF_LineJoin)
HPDF_Page_GetLineJoin  (HPDF_Page   page);


HPDF_EXPORT(HPDF_REAL)
HPDF_Page_GetMiterLimit  (HPDF_Page   page);


HPDF_EXPORT(HPDF_DashMode)
HPDF_Page_GetDash  (HPDF_Page   page);


HPDF_EXPORT(HPDF_REAL)
HPDF_Page_GetFlat  (HPDF_Page   page);


HPDF_EXPORT(HPDF_REAL)
HPDF_Page_GetCharSpace  (HPDF_Page   page);


HPDF_EXPORT(HPDF_REAL)
HPDF_Page_GetWordSpace  (HPDF_Page   page);


HPDF_EXPORT(HPDF_REAL)
HPDF_Page_GetHorizontalScalling  (HPDF_Page   page);


HPDF_EXPORT(HPDF_REAL)
HPDF_Page_GetTextLeading  (HPDF_Page   page);


HPDF_EXPORT(HPDF_TextRenderingMode)
HPDF_Page_GetTextRenderingMode  (HPDF_Page   page);


/* This function is obsolete. Use HPDF_Page_GetTextRise.  */
HPDF_EXPORT(HPDF_REAL)
HPDF_Page_GetTextRaise  (HPDF_Page   page);


HPDF_EXPORT(HPDF_REAL)
HPDF_Page_GetTextRise  (HPDF_Page   page);


HPDF_EXPORT(HPDF_RGBColor)
HPDF_Page_GetRGBFill  (HPDF_Page   page);


HPDF_EXPORT(HPDF_RGBColor)
HPDF_Page_GetRGBStroke  (HPDF_Page   page);


HPDF_EXPORT(HPDF_CMYKColor)
HPDF_Page_GetCMYKFill  (HPDF_Page   page);


HPDF_EXPORT(HPDF_CMYKColor)
HPDF_Page_GetCMYKStroke  (HPDF_Page   page);


HPDF_EXPORT(HPDF_REAL)
HPDF_Page_GetGrayFill  (HPDF_Page   page);


HPDF_EXPORT(HPDF_REAL)
HPDF_Page_GetGrayStroke  (HPDF_Page   page);


HPDF_EXPORT(HPDF_ColorSpace)
HPDF_Page_GetStrokingColorSpace (HPDF_Page   page);


HPDF_EXPORT(HPDF_ColorSpace)
HPDF_Page_GetFillingColorSpace (HPDF_Page   page);


HPDF_EXPORT(HPDF_TransMatrix)
HPDF_Page_GetTextMatrix  (HPDF_Page   page);


HPDF_EXPORT(HPDF_UINT)
HPDF_Page_GetGStateDepth  (HPDF_Page   page);


/*--------------------------------------------------------------------------*/
/*----- GRAPHICS OPERATORS -------------------------------------------------*/


/*--- General graphics state ---------------------------------------------*/

/* w */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_SetLineWidth  (HPDF_Page  page,
                         HPDF_REAL  line_width);

/* J */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_SetLineCap  (HPDF_Page     page,
                       HPDF_LineCap  line_cap);

/* j */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_SetLineJoin  (HPDF_Page      page,
                        HPDF_LineJoin  line_join);

/* M */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_SetMiterLimit  (HPDF_Page  page,
                          HPDF_REAL  miter_limit);

/* d */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_SetDash  (HPDF_Page           page,
                    const HPDF_UINT16  *dash_ptn,
                    HPDF_UINT           num_param,
                    HPDF_UINT           phase);



/* ri --not implemented yet */

/* i */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_SetFlat  (HPDF_Page    page,
                    HPDF_REAL    flatness);

/* gs */

HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_SetExtGState  (HPDF_Page        page,
                         HPDF_ExtGState   ext_gstate);


/*--- Special graphic state operator --------------------------------------*/

/* q */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_GSave  (HPDF_Page    page);

/* Q */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_GRestore  (HPDF_Page    page);

/* cm */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_Concat  (HPDF_Page    page,
                   HPDF_REAL    a,
                   HPDF_REAL    b,
                   HPDF_REAL    c,
                   HPDF_REAL    d,
                   HPDF_REAL    x,
                   HPDF_REAL    y);

/*--- Path construction operator ------------------------------------------*/

/* m */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_MoveTo  (HPDF_Page    page,
                   HPDF_REAL    x,
                   HPDF_REAL    y);

/* l */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_LineTo  (HPDF_Page    page,
                   HPDF_REAL    x,
                   HPDF_REAL    y);

/* c */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_CurveTo  (HPDF_Page    page,
                    HPDF_REAL    x1,
                    HPDF_REAL    y1,
                    HPDF_REAL    x2,
                    HPDF_REAL    y2,
                    HPDF_REAL    x3,
                    HPDF_REAL    y3);

/* v */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_CurveTo2  (HPDF_Page    page,
                     HPDF_REAL    x2,
                     HPDF_REAL    y2,
                     HPDF_REAL    x3,
                     HPDF_REAL    y3);

/* y */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_CurveTo3  (HPDF_Page  page,
                     HPDF_REAL  x1,
                     HPDF_REAL  y1,
                     HPDF_REAL  x3,
                     HPDF_REAL  y3);

/* h */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_ClosePath  (HPDF_Page  page);

/* re */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_Rectangle  (HPDF_Page  page,
                      HPDF_REAL  x,
                      HPDF_REAL  y,
                      HPDF_REAL  width,
                      HPDF_REAL  height);


/*--- Path painting operator ---------------------------------------------*/

/* S */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_Stroke  (HPDF_Page  page);

/* s */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_ClosePathStroke  (HPDF_Page  page);

/* f */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_Fill  (HPDF_Page  page);

/* f* */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_Eofill  (HPDF_Page  page);

/* B */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_FillStroke  (HPDF_Page  page);

/* B* */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_EofillStroke  (HPDF_Page  page);

/* b */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_ClosePathFillStroke  (HPDF_Page  page);

/* b* */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_ClosePathEofillStroke  (HPDF_Page  page);

/* n */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_EndPath  (HPDF_Page  page);


/*--- Clipping paths operator --------------------------------------------*/

/* W */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_Clip  (HPDF_Page  page);

/* W* */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_Eoclip  (HPDF_Page  page);


/*--- Text object operator -----------------------------------------------*/

/* BT */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_BeginText  (HPDF_Page  page);

/* ET */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_EndText  (HPDF_Page  page);

/*--- Text state ---------------------------------------------------------*/

/* Tc */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_SetCharSpace  (HPDF_Page  page,
                         HPDF_REAL  value);

/* Tw */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_SetWordSpace  (HPDF_Page  page,
                         HPDF_REAL  value);

/* Tz */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_SetHorizontalScalling  (HPDF_Page  page,
                                  HPDF_REAL  value);

/* TL */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_SetTextLeading  (HPDF_Page  page,
                           HPDF_REAL  value);

/* Tf */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_SetFontAndSize  (HPDF_Page  page,
                           HPDF_Font  font,
                           HPDF_REAL  size);

/* Tr */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_SetTextRenderingMode  (HPDF_Page               page,
                                 HPDF_TextRenderingMode  mode);

/* Ts */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_SetTextRise  (HPDF_Page   page,
                        HPDF_REAL   value);

/* This function is obsolete. Use HPDF_Page_SetTextRise.  */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_SetTextRaise  (HPDF_Page   page,
                         HPDF_REAL   value);

/*--- Text positioning ---------------------------------------------------*/

/* Td */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_MoveTextPos  (HPDF_Page   page,
                        HPDF_REAL   x,
                        HPDF_REAL   y);

/* TD */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_MoveTextPos2  (HPDF_Page  page,
                         HPDF_REAL   x,
                         HPDF_REAL   y);

/* Tm */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_SetTextMatrix  (HPDF_Page         page,
                          HPDF_REAL    a,
                          HPDF_REAL    b,
                          HPDF_REAL    c,
                          HPDF_REAL    d,
                          HPDF_REAL    x,
                          HPDF_REAL    y);


/* T* */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_MoveToNextLine  (HPDF_Page  page);

/*--- Text showing -------------------------------------------------------*/

/* Tj */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_ShowText  (HPDF_Page    page,
                     const char  *text);

/* TJ */

/* ' */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_ShowTextNextLine  (HPDF_Page    page,
                             const char  *text);

/* " */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_ShowTextNextLineEx  (HPDF_Page    page,
                               HPDF_REAL    word_space,
                               HPDF_REAL    char_space,
                               const char  *text);


/*--- Color showing ------------------------------------------------------*/

/* cs --not implemented yet */
/* CS --not implemented yet */
/* sc --not implemented yet */
/* scn --not implemented yet */
/* SC --not implemented yet */
/* SCN --not implemented yet */

/* g */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_SetGrayFill  (HPDF_Page   page,
                        HPDF_REAL   gray);

/* G */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_SetGrayStroke  (HPDF_Page   page,
                          HPDF_REAL   gray);

/* rg */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_SetRGBFill  (HPDF_Page  page,
                       HPDF_REAL  r,
                       HPDF_REAL  g,
                       HPDF_REAL  b);

/* RG */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_SetRGBStroke  (HPDF_Page  page,
                         HPDF_REAL  r,
                         HPDF_REAL  g,
                         HPDF_REAL  b);

/* k */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_SetCMYKFill  (HPDF_Page  page,
                        HPDF_REAL  c,
                        HPDF_REAL  m,
                        HPDF_REAL  y,
                        HPDF_REAL  k);

/* K */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_SetCMYKStroke  (HPDF_Page  page,
                          HPDF_REAL  c,
                          HPDF_REAL  m,
                          HPDF_REAL  y,
                          HPDF_REAL  k);

/*--- Shading patterns ---------------------------------------------------*/

/* sh --not implemented yet */

/*--- In-line images -----------------------------------------------------*/

/* BI --not implemented yet */
/* ID --not implemented yet */
/* EI --not implemented yet */

/*--- XObjects -----------------------------------------------------------*/

/* Do */
HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_ExecuteXObject  (HPDF_Page     page,
                           HPDF_XObject  obj);

/*--- Marked content -----------------------------------------------------*/

/* BMC --not implemented yet */
/* BDC --not implemented yet */
/* EMC --not implemented yet */
/* MP --not implemented yet */
/* DP --not implemented yet */

/*--- Compatibility ------------------------------------------------------*/

/* BX --not implemented yet */
/* EX --not implemented yet */

HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_DrawImage  (HPDF_Page    page,
                      HPDF_Image   image,
                      HPDF_REAL    x,
                      HPDF_REAL    y,
                      HPDF_REAL    width,
                      HPDF_REAL    height);


HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_Circle  (HPDF_Page     page,
                   HPDF_REAL     x,
                   HPDF_REAL     y,
                   HPDF_REAL     ray);


HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_Ellipse  (HPDF_Page   page,
                    HPDF_REAL   x,
                    HPDF_REAL   y,
                    HPDF_REAL  xray,
                    HPDF_REAL  yray);


HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_Arc  (HPDF_Page    page,
                HPDF_REAL    x,
                HPDF_REAL    y,
                HPDF_REAL    ray,
                HPDF_REAL    ang1,
                HPDF_REAL    ang2);


HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_TextOut  (HPDF_Page    page,
                    HPDF_REAL    xpos,
                    HPDF_REAL    ypos,
                    const char  *text);


HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_TextRect  (HPDF_Page            page,
                     HPDF_REAL            left,
                     HPDF_REAL            top,
                     HPDF_REAL            right,
                     HPDF_REAL            bottom,
                     const char          *text,
                     HPDF_TextAlignment   align,
                     HPDF_UINT           *len);


HPDF_EXPORT(HPDF_STATUS)
HPDF_Page_SetSlideShow  (HPDF_Page              page,
                         HPDF_TransitionStyle   type,
                         HPDF_REAL              disp_time,
                         HPDF_REAL              trans_time);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _HPDF_H */

