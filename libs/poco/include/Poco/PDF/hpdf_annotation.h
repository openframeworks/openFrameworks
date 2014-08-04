/*
 * << Haru Free PDF Library 2.0.0 >> -- hpdf_annotation.h
 *
 * Copyright (c) 1999-2006 Takeshi Kanno <takeshi_kanno@est.hi-ho.ne.jp>
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.
 * It is provided "as is" without express or implied warranty.
 *
 */

#ifndef _HPDF_ANNOTATION_H
#define _HPDF_ANNOTATION_H

#include "hpdf_objects.h"

#ifdef __cplusplus
extern "C" {
#endif

/*----------------------------------------------------------------------------*/
/*------ HPDF_Annotation -----------------------------------------------------*/


HPDF_Annotation
HPDF_Annotation_New  (HPDF_MMgr       mmgr,
                      HPDF_Xref       xref,
                      HPDF_AnnotType  type,
                      HPDF_Rect       rect);


HPDF_Annotation
HPDF_LinkAnnot_New  (HPDF_MMgr           mmgr,
                     HPDF_Xref         xref,
                     HPDF_Rect         rect,
                     HPDF_Destination  dst);


HPDF_Annotation
HPDF_URILinkAnnot_New  (HPDF_MMgr          mmgr,
                        HPDF_Xref          xref,
                        HPDF_Rect          rect,
                        const char   *uri);


HPDF_Annotation
HPDF_TextAnnot_New  (HPDF_MMgr        mmgr,
                     HPDF_Xref        xref,
                     HPDF_Rect        rect,
                     const char  *text,
                     HPDF_Encoder     encoder);


HPDF_STATUS
HPDF_Annotation_SetBorderStyle  (HPDF_Annotation  annot,
                                 HPDF_BSSubtype   subtype,
                                 HPDF_REAL        width,
                                 HPDF_UINT16      dash_on,
                                 HPDF_UINT16      dash_off,
                                 HPDF_UINT16      dash_phase);


HPDF_BOOL
HPDF_Annotation_Validate (HPDF_Annotation  annot);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _HPDF_ANNOTATION_H */

