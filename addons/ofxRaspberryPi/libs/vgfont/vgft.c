/*
Copyright (c) 2012, Broadcom Europe Ltd
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the copyright holder nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

// Font handling for graphicsx

#include <assert.h>
#include <stdlib.h>

#include "graphics_x_private.h"
#include "vgft.h"

static FT_Library lib;

int vgft_init(void)
{
   if (FT_Init_FreeType(&lib) == 0)
      return 0;
   else
   {
      return -1;
   }
}

void vgft_term(void)
{
   FT_Done_FreeType(lib);
}

#define SEGMENTS_COUNT_MAX 256
#define COORDS_COUNT_MAX 1024

static VGuint segments_count;
static VGubyte segments[SEGMENTS_COUNT_MAX];
static VGuint coords_count;
static VGfloat coords[COORDS_COUNT_MAX];

static VGfloat float_from_26_6(FT_Pos x)
{
   return (VGfloat)x / 64.0f;
}

static void convert_contour(const FT_Vector *points, const char *tags, short points_count)
{
   int first_coords = coords_count;

   int first = 1;
   char last_tag = 0;
   int c = 0;

   for (; points_count != 0; ++points, ++tags, --points_count) {
      ++c;

      char tag = *tags;
      if (first) {
         assert(tag & 0x1);
         assert(c==1); c=0;
         segments[segments_count++] = VG_MOVE_TO;
         first = 0;
      } else if (tag & 0x1) {
         /* on curve */

         if (last_tag & 0x1) {
            /* last point was also on -- line */
            assert(c==1); c=0;
            segments[segments_count++] = VG_LINE_TO;
         } else {
            /* last point was off -- quad or cubic */
            if (last_tag & 0x2) {
               /* cubic */
               assert(c==3); c=0;
               segments[segments_count++] = VG_CUBIC_TO;
            } else {
               /* quad */
               assert(c==2); c=0;
               segments[segments_count++] = VG_QUAD_TO;
            }
         }
      } else {
         /* off curve */

         if (tag & 0x2) {
            /* cubic */

            assert((last_tag & 0x1) || (last_tag & 0x2)); /* last either on or off and cubic */
         } else {
            /* quad */

            if (!(last_tag & 0x1)) {
               /* last was also off curve */

               assert(!(last_tag & 0x2)); /* must be quad */

               /* add on point half-way between */
               assert(c==2); c=1;
               segments[segments_count++] = VG_QUAD_TO;
               VGfloat x = (coords[coords_count - 2] + float_from_26_6(points->x)) * 0.5f;
               VGfloat y = (coords[coords_count - 1] + float_from_26_6(points->y)) * 0.5f;
               coords[coords_count++] = x;
               coords[coords_count++] = y;
            }
         }
      }
      last_tag = tag;

      coords[coords_count++] = float_from_26_6(points->x);
      coords[coords_count++] = float_from_26_6(points->y);
   }

   if (last_tag & 0x1) {
      /* last point was also on -- line (implicit with close path) */
      assert(c==0);
   } else {
      ++c;

      /* last point was off -- quad or cubic */
      if (last_tag & 0x2) {
         /* cubic */
         assert(c==3); c=0;
         segments[segments_count++] = VG_CUBIC_TO;
      } else {
         /* quad */
         assert(c==2); c=0;
         segments[segments_count++] = VG_QUAD_TO;
      }

      coords[coords_count++] = coords[first_coords + 0];
      coords[coords_count++] = coords[first_coords + 1];
   }

   segments[segments_count++] = VG_CLOSE_PATH;
}

static void convert_outline(const FT_Vector *points, const char *tags, const short *contours, short contours_count, short points_count)
{
   segments_count = 0;
   coords_count = 0;

   short last_contour = 0;
   for (; contours_count != 0; ++contours, --contours_count) {
      short contour = *contours + 1;
      convert_contour(points + last_contour, tags + last_contour, contour - last_contour);
      last_contour = contour;
   }
   assert(last_contour == points_count);

   assert(segments_count <= SEGMENTS_COUNT_MAX); /* oops... we overwrote some memory */
   assert(coords_count <= COORDS_COUNT_MAX);
}

VCOS_STATUS_T vgft_font_init(VGFT_FONT_T *font)
{
   font->ft_face = NULL;
   font->vg_font = vgCreateFont(0);
   if (font->vg_font == VG_INVALID_HANDLE)
   {
      return VCOS_ENOMEM;
   }
   return VCOS_SUCCESS;
}

VCOS_STATUS_T vgft_font_load_mem(VGFT_FONT_T *font, void *mem, size_t len)
{
   if (FT_New_Memory_Face(lib, mem, len, 0, &font->ft_face))
   {
      return VCOS_EINVAL;
   }
   return VCOS_SUCCESS;
}

VCOS_STATUS_T vgft_font_load_file(VGFT_FONT_T *font, const char *file)
{
   if (FT_New_Face(lib, file, 0, &font->ft_face)) {
      return VCOS_EINVAL;
   }
   return VCOS_SUCCESS;
}

VCOS_STATUS_T vgft_font_convert_glyphs(VGFT_FONT_T *font, unsigned int char_height, unsigned int dpi_x, unsigned int dpi_y)
{
   FT_UInt glyph_index;
   FT_ULong ch;

   if (FT_Set_Char_Size(font->ft_face, 0, char_height, dpi_x, dpi_y))
   {
      FT_Done_Face(font->ft_face);
      vgDestroyFont(font->vg_font);
      return VCOS_EINVAL;
   }

   ch = FT_Get_First_Char(font->ft_face, &glyph_index);

   while (ch != 0)
   {
      if (FT_Load_Glyph(font->ft_face, glyph_index, FT_LOAD_DEFAULT)) {
         FT_Done_Face(font->ft_face);
         vgDestroyFont(font->vg_font);
         return VCOS_ENOMEM;
      }

      VGPath vg_path;
      FT_Outline *outline = &font->ft_face->glyph->outline;
      if (outline->n_contours != 0) {
         vg_path = vgCreatePath(VG_PATH_FORMAT_STANDARD, VG_PATH_DATATYPE_F, 1.0f, 0.0f, 0, 0, VG_PATH_CAPABILITY_ALL);
         assert(vg_path != VG_INVALID_HANDLE);

         convert_outline(outline->points, outline->tags, outline->contours, outline->n_contours, outline->n_points);
         vgAppendPathData(vg_path, segments_count, segments, coords);
      } else {
         vg_path = VG_INVALID_HANDLE;
      }

      VGfloat origin[] = { 0.0f, 0.0f };
      VGfloat escapement[] = { float_from_26_6(font->ft_face->glyph->advance.x), float_from_26_6(font->ft_face->glyph->advance.y) };
      vgSetGlyphToPath(font->vg_font, glyph_index, vg_path, VG_FALSE, origin, escapement);

      if (vg_path != VG_INVALID_HANDLE) {
         vgDestroyPath(vg_path);
      }
      ch = FT_Get_Next_Char(font->ft_face, ch, &glyph_index);
   }

   return VCOS_SUCCESS;
}

void vgft_font_term(VGFT_FONT_T *font)
{
   if (font->ft_face)
      FT_Done_Face(font->ft_face);
   if (font->vg_font)
      vgDestroyFont(font->vg_font);
   memset(font, 0, sizeof(*font));
}

#define GLYPHS_COUNT_MAX 200

static VGuint glyph_indices[GLYPHS_COUNT_MAX];
static VGfloat adjustments_x[GLYPHS_COUNT_MAX];
static VGfloat adjustments_y[GLYPHS_COUNT_MAX];

static void draw_line(VGFT_FONT_T *font, VGfloat x, VGfloat y, const char *text, int glyphs_count, VGbitfield paint_modes)
{
   if (glyphs_count == 0) return;

   assert(glyphs_count <= GLYPHS_COUNT_MAX);

   VGfloat glor[] = { x, y };
   vgSetfv(VG_GLYPH_ORIGIN, 2, glor);

   int i;
   int prev_glyph_index = 0;
   for (i = 0; i != glyphs_count; ++i) {

      int glyph_index = FT_Get_Char_Index(font->ft_face, text[i]);
      if (!glyph_index) { return; }

      glyph_indices[i] = glyph_index;

      if (i != 0) {
         FT_Vector kern;
         if (FT_Get_Kerning(font->ft_face, prev_glyph_index, glyph_index, FT_KERNING_DEFAULT, &kern)) {
            assert(0);
         }

         adjustments_x[i - 1] = float_from_26_6(kern.x);
         adjustments_y[i - 1] = float_from_26_6(kern.y);
      }

      prev_glyph_index = glyph_index;
   }

   adjustments_x[glyphs_count - 1] = 0.0f;
   adjustments_y[glyphs_count - 1] = 0.0f;

   vgDrawGlyphs(font->vg_font, glyphs_count, glyph_indices, adjustments_x, adjustments_y, paint_modes, VG_FALSE);
}

void vgft_font_draw(VGFT_FONT_T *font, VGfloat x, VGfloat y, const char *text, unsigned text_length, VGbitfield paint_modes)
{
   VGfloat descent = float_from_26_6(font->ft_face->size->metrics.descender);
   int last_draw = 0;
   int i = 0;
   y -= descent;
   for (;;) {
      int last = !text[i] || (text_length && i==text_length);

      if ((text[i] == '\n') || last)
      {
         draw_line(font, x, y, text + last_draw, i - last_draw, paint_modes);
         last_draw = i+1;
         y -= float_from_26_6(font->ft_face->size->metrics.height);
      }
      if (last)
      {
         break;
      }
      ++i;
   }
}

// Get text extents for a single line

static void line_extents(VGFT_FONT_T *font, VGfloat *x, VGfloat *y, const char *text, int chars_count)
{
   int i;
   int prev_glyph_index = 0;
   if (chars_count == 0) return;

   for (i=0; i < chars_count; i++)
   {
      int glyph_index = FT_Get_Char_Index(font->ft_face, text[i]);
      if (!glyph_index) return;

      if (i != 0)
      {
         FT_Vector kern;
         if (FT_Get_Kerning(font->ft_face, prev_glyph_index, glyph_index,
                            FT_KERNING_DEFAULT, &kern))
         {
            assert(0);
         }
         *x += float_from_26_6(kern.x);
         *y += float_from_26_6(kern.y);
      }
      FT_Load_Glyph(font->ft_face, glyph_index, FT_LOAD_DEFAULT);
      *x += float_from_26_6(font->ft_face->glyph->advance.x);
   }
}

// Text extents for some ASCII text.
//
// Use text_length if non-zero, otherwise look for trailing '\0'.

void vgft_get_text_extents(VGFT_FONT_T *font,
                           const char *text,
                           unsigned text_length,
                           VGfloat x, VGfloat y,
                           VGfloat *w, VGfloat *h)
{
   int last_draw = 0;
   int i = 0;
   VGfloat start_x = x;
   VGfloat start_y = y;
   VGfloat max_x = x;

   for (;;) 
   {
      int last = !text[i] || (text_length && i==text_length);
      if ((text[i] == '\n') || last)
      {
         line_extents(font, &x, &y, text + last_draw, i - last_draw);
         last_draw = i+1;
         y -= float_from_26_6(font->ft_face->size->metrics.height);
         if (x > max_x)
            max_x = x;
      }
      if (last)
      {
         break;
      }


      ++i;
   }
   *w = max_x - start_x;
   *h = start_y - y;
}



