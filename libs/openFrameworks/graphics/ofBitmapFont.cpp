
#include "ofBitmapFont.h"
#include "ofGraphics.h"


// ==============================================================
// this license below is from freeGlut, (freeglut_font_data.c)

/*
 * The legal status of this file is a bit vague.  The font glyphs
 * themselves come from XFree86 v4.3.0 (as of this writing), and as
 * part of the X server may be subject to the XFree86 copyrights.
 * The original freeglut fonts were extracted by a utility written
 * by Pawel W. Olszta (see below) and the generated fonts contained
 * his copyright exclusively.  Steve Baker asserts that Pawel
 * assigned intellectual property rights to Steve Baker.  Steve
 * Baker also asserts that fonts cannot be copyrighted.  He has
 * neither stripped the copyright from the freeglut fonts nor
 * formally retitled anything in his name.  Since that time, the
 * OpenGLUT project has branched from freeglut, and has made
 * necessary modifications to Pawel's ``genfonts'' utility.
 * To that extent, OpenGLUT may have some title to this file.
 * What is fairly clear is that the font data is licensed under
 * the XFree86 license (which is variously termed ``XFree'' and
 * ``MIT'' by the freeglut project).  It is believed that all
 * title holders wish this file to be as useful as possible, and
 * that either the ``XFree'' or ``MIT'' license works.
 *
 * Portions copyright (c) 2004, the OpenGLUT project contributors.
 * OpenGLUT branched from freeglut in February, 2004.
 *
 * Copyright (c) 1999-2000 by Pawel W. Olszta
 * Written by Pawel W. Olszta, <olszta@sourceforge.net>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Sotware.
 */
// ==============================================================


static const unsigned char bmpChar_8x13_000[] = {  8,  0,  0,  0,170,  0,130,  0,130,  0,130,  0,170,  0,  0};
static const unsigned char bmpChar_8x13_001[] = {  8,  0,  0,  0,  0, 16, 56,124,254,124, 56, 16,  0,  0,  0};
static const unsigned char bmpChar_8x13_002[] = {  8,  0,170, 85,170, 85,170, 85,170, 85,170, 85,170, 85,170};
static const unsigned char bmpChar_8x13_003[] = {  8,  0,  0,  0,  4,  4,  4,  4,174,160,224,160,160,  0,  0};
static const unsigned char bmpChar_8x13_004[] = {  8,  0,  0,  0,  8,  8, 12,  8,142,128,192,128,224,  0,  0};
static const unsigned char bmpChar_8x13_005[] = {  8,  0,  0,  0, 10, 10, 12, 10,108,128,128,128, 96,  0,  0};
static const unsigned char bmpChar_8x13_006[] = {  8,  0,  0,  0,  8,  8, 12,  8,238,128,128,128,128,  0,  0};
static const unsigned char bmpChar_8x13_007[] = {  8,  0,  0,  0,  0,  0,  0,  0,  0, 24, 36, 36, 24,  0,  0};
static const unsigned char bmpChar_8x13_008[] = {  8,  0,  0,  0,  0,124,  0, 16, 16,124, 16, 16,  0,  0,  0};
static const unsigned char bmpChar_8x13_009[] = {  8,  0,  0,  0, 14,  8,  8,  8,168,160,160,160,192,  0,  0};
static const unsigned char bmpChar_8x13_010[] = {  8,  0,  0,  0,  4,  4,  4,  4, 46, 80, 80,136,136,  0,  0};
static const unsigned char bmpChar_8x13_011[] = {  8,  0,  0,  0,  0,  0,  0,  0,240, 16, 16, 16, 16, 16, 16};
static const unsigned char bmpChar_8x13_012[] = {  8,  0, 16, 16, 16, 16, 16, 16,240,  0,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_013[] = {  8,  0, 16, 16, 16, 16, 16, 16, 31,  0,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_014[] = {  8,  0,  0,  0,  0,  0,  0,  0, 31, 16, 16, 16, 16, 16, 16};
static const unsigned char bmpChar_8x13_015[] = {  8,  0, 16, 16, 16, 16, 16, 16,255, 16, 16, 16, 16, 16, 16};
static const unsigned char bmpChar_8x13_016[] = {  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,255};
static const unsigned char bmpChar_8x13_017[] = {  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,255,  0,  0,  0};
static const unsigned char bmpChar_8x13_018[] = {  8,  0,  0,  0,  0,  0,  0,  0,255,  0,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_019[] = {  8,  0,  0,  0,  0,255,  0,  0,  0,  0,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_020[] = {  8,  0,255,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_021[] = {  8,  0, 16, 16, 16, 16, 16, 16, 31, 16, 16, 16, 16, 16, 16};
static const unsigned char bmpChar_8x13_022[] = {  8,  0, 16, 16, 16, 16, 16, 16,240, 16, 16, 16, 16, 16, 16};
static const unsigned char bmpChar_8x13_023[] = {  8,  0,  0,  0,  0,  0,  0,  0,255, 16, 16, 16, 16, 16, 16};
static const unsigned char bmpChar_8x13_024[] = {  8,  0, 16, 16, 16, 16, 16, 16,255,  0,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_025[] = {  8,  0, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16};
static const unsigned char bmpChar_8x13_026[] = {  8,  0,  0,  0,254,  0, 14, 48,192, 48, 14,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_027[] = {  8,  0,  0,  0,254,  0,224, 24,  6, 24,224,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_028[] = {  8,  0,  0,  0, 68, 68, 68, 68, 68,254,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_029[] = {  8,  0,  0,  0, 32, 32,126, 16,  8,126,  4,  4,  0,  0,  0};
static const unsigned char bmpChar_8x13_030[] = {  8,  0,  0,  0,220, 98, 32, 32, 32,112, 32, 34, 28,  0,  0};
static const unsigned char bmpChar_8x13_031[] = {  8,  0,  0,  0,  0,  0,  0,  0, 24,  0,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_032[] = {  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_033[] = {  8,  0,  0,  0, 16,  0, 16, 16, 16, 16, 16, 16, 16,  0,  0};
static const unsigned char bmpChar_8x13_034[] = {  8,  0,  0,  0,  0,  0,  0,  0,  0,  0, 36, 36, 36,  0,  0};
static const unsigned char bmpChar_8x13_035[] = {  8,  0,  0,  0,  0, 36, 36,126, 36,126, 36, 36,  0,  0,  0};
static const unsigned char bmpChar_8x13_036[] = {  8,  0,  0,  0, 16,120, 20, 20, 56, 80, 80, 60, 16,  0,  0};
static const unsigned char bmpChar_8x13_037[] = {  8,  0,  0,  0, 68, 42, 36, 16,  8,  8, 36, 82, 34,  0,  0};
static const unsigned char bmpChar_8x13_038[] = {  8,  0,  0,  0, 58, 68, 74, 48, 72, 72, 48,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_039[] = {  8,  0,  0,  0,  0,  0,  0,  0,  0,  0, 64, 48, 56,  0,  0};
static const unsigned char bmpChar_8x13_040[] = {  8,  0,  0,  0,  4,  8,  8, 16, 16, 16,  8,  8,  4,  0,  0};
static const unsigned char bmpChar_8x13_041[] = {  8,  0,  0,  0, 32, 16, 16,  8,  8,  8, 16, 16, 32,  0,  0};
static const unsigned char bmpChar_8x13_042[] = {  8,  0,  0,  0,  0,  0, 36, 24,126, 24, 36,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_043[] = {  8,  0,  0,  0,  0,  0, 16, 16,124, 16, 16,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_044[] = {  8,  0,  0, 64, 48, 56,  0,  0,  0,  0,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_045[] = {  8,  0,  0,  0,  0,  0,  0,  0,126,  0,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_046[] = {  8,  0,  0, 16, 56, 16,  0,  0,  0,  0,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_047[] = {  8,  0,  0,  0,128,128, 64, 32, 16,  8,  4,  2,  2,  0,  0};
static const unsigned char bmpChar_8x13_048[] = {  8,  0,  0,  0, 24, 36, 66, 66, 66, 66, 66, 36, 24,  0,  0};
static const unsigned char bmpChar_8x13_049[] = {  8,  0,  0,  0,124, 16, 16, 16, 16, 16, 80, 48, 16,  0,  0};
static const unsigned char bmpChar_8x13_050[] = {  8,  0,  0,  0,126, 64, 32, 24,  4,  2, 66, 66, 60,  0,  0};
static const unsigned char bmpChar_8x13_051[] = {  8,  0,  0,  0, 60, 66,  2,  2, 28,  8,  4,  2,126,  0,  0};
static const unsigned char bmpChar_8x13_052[] = {  8,  0,  0,  0,  4,  4,126, 68, 68, 36, 20, 12,  4,  0,  0};
static const unsigned char bmpChar_8x13_053[] = {  8,  0,  0,  0, 60, 66,  2,  2, 98, 92, 64, 64,126,  0,  0};
static const unsigned char bmpChar_8x13_054[] = {  8,  0,  0,  0, 60, 66, 66, 98, 92, 64, 64, 32, 28,  0,  0};
static const unsigned char bmpChar_8x13_055[] = {  8,  0,  0,  0, 32, 32, 16, 16,  8,  8,  4,  2,126,  0,  0};
static const unsigned char bmpChar_8x13_056[] = {  8,  0,  0,  0, 60, 66, 66, 66, 60, 66, 66, 66, 60,  0,  0};
static const unsigned char bmpChar_8x13_057[] = {  8,  0,  0,  0, 56,  4,  2,  2, 58, 70, 66, 66, 60,  0,  0};
static const unsigned char bmpChar_8x13_058[] = {  8,  0,  0, 16, 56, 16,  0,  0, 16, 56, 16,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_059[] = {  8,  0,  0, 64, 48, 56,  0,  0, 16, 56, 16,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_060[] = {  8,  0,  0,  0,  2,  4,  8, 16, 32, 16,  8,  4,  2,  0,  0};
static const unsigned char bmpChar_8x13_061[] = {  8,  0,  0,  0,  0,  0,126,  0,  0,126,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_062[] = {  8,  0,  0,  0, 64, 32, 16,  8,  4,  8, 16, 32, 64,  0,  0};
static const unsigned char bmpChar_8x13_063[] = {  8,  0,  0,  0,  8,  0,  8,  8,  4,  2, 66, 66, 60,  0,  0};
static const unsigned char bmpChar_8x13_064[] = {  8,  0,  0,  0, 60, 64, 74, 86, 82, 78, 66, 66, 60,  0,  0};
static const unsigned char bmpChar_8x13_065[] = {  8,  0,  0,  0, 66, 66, 66,126, 66, 66, 66, 36, 24,  0,  0};
static const unsigned char bmpChar_8x13_066[] = {  8,  0,  0,  0,252, 66, 66, 66,124, 66, 66, 66,252,  0,  0};
static const unsigned char bmpChar_8x13_067[] = {  8,  0,  0,  0, 60, 66, 64, 64, 64, 64, 64, 66, 60,  0,  0};
static const unsigned char bmpChar_8x13_068[] = {  8,  0,  0,  0,252, 66, 66, 66, 66, 66, 66, 66,252,  0,  0};
static const unsigned char bmpChar_8x13_069[] = {  8,  0,  0,  0,126, 64, 64, 64,120, 64, 64, 64,126,  0,  0};
static const unsigned char bmpChar_8x13_070[] = {  8,  0,  0,  0, 64, 64, 64, 64,120, 64, 64, 64,126,  0,  0};
static const unsigned char bmpChar_8x13_071[] = {  8,  0,  0,  0, 58, 70, 66, 78, 64, 64, 64, 66, 60,  0,  0};
static const unsigned char bmpChar_8x13_072[] = {  8,  0,  0,  0, 66, 66, 66, 66,126, 66, 66, 66, 66,  0,  0};
static const unsigned char bmpChar_8x13_073[] = {  8,  0,  0,  0,124, 16, 16, 16, 16, 16, 16, 16,124,  0,  0};
static const unsigned char bmpChar_8x13_074[] = {  8,  0,  0,  0, 56, 68,  4,  4,  4,  4,  4,  4, 31,  0,  0};
static const unsigned char bmpChar_8x13_075[] = {  8,  0,  0,  0, 66, 68, 72, 80, 96, 80, 72, 68, 66,  0,  0};
static const unsigned char bmpChar_8x13_076[] = {  8,  0,  0,  0,126, 64, 64, 64, 64, 64, 64, 64, 64,  0,  0};
static const unsigned char bmpChar_8x13_077[] = {  8,  0,  0,  0,130,130,130,146,146,170,198,130,130,  0,  0};
static const unsigned char bmpChar_8x13_078[] = {  8,  0,  0,  0, 66, 66, 66, 70, 74, 82, 98, 66, 66,  0,  0};
static const unsigned char bmpChar_8x13_079[] = {  8,  0,  0,  0, 60, 66, 66, 66, 66, 66, 66, 66, 60,  0,  0};
static const unsigned char bmpChar_8x13_080[] = {  8,  0,  0,  0, 64, 64, 64, 64,124, 66, 66, 66,124,  0,  0};
static const unsigned char bmpChar_8x13_081[] = {  8,  0,  0,  2, 60, 74, 82, 66, 66, 66, 66, 66, 60,  0,  0};
static const unsigned char bmpChar_8x13_082[] = {  8,  0,  0,  0, 66, 68, 72, 80,124, 66, 66, 66,124,  0,  0};
static const unsigned char bmpChar_8x13_083[] = {  8,  0,  0,  0, 60, 66,  2,  2, 60, 64, 64, 66, 60,  0,  0};
static const unsigned char bmpChar_8x13_084[] = {  8,  0,  0,  0, 16, 16, 16, 16, 16, 16, 16, 16,254,  0,  0};
static const unsigned char bmpChar_8x13_085[] = {  8,  0,  0,  0, 60, 66, 66, 66, 66, 66, 66, 66, 66,  0,  0};
static const unsigned char bmpChar_8x13_086[] = {  8,  0,  0,  0, 16, 40, 40, 40, 68, 68, 68,130,130,  0,  0};
static const unsigned char bmpChar_8x13_087[] = {  8,  0,  0,  0, 68,170,146,146,146,130,130,130,130,  0,  0};
static const unsigned char bmpChar_8x13_088[] = {  8,  0,  0,  0,130,130, 68, 40, 16, 40, 68,130,130,  0,  0};
static const unsigned char bmpChar_8x13_089[] = {  8,  0,  0,  0, 16, 16, 16, 16, 16, 40, 68,130,130,  0,  0};
static const unsigned char bmpChar_8x13_090[] = {  8,  0,  0,  0,126, 64, 64, 32, 16,  8,  4,  2,126,  0,  0};
static const unsigned char bmpChar_8x13_091[] = {  8,  0,  0,  0, 60, 32, 32, 32, 32, 32, 32, 32, 60,  0,  0};
static const unsigned char bmpChar_8x13_092[] = {  8,  0,  0,  0,  2,  2,  4,  8, 16, 32, 64,128,128,  0,  0};
static const unsigned char bmpChar_8x13_093[] = {  8,  0,  0,  0,120,  8,  8,  8,  8,  8,  8,  8,120,  0,  0};
static const unsigned char bmpChar_8x13_094[] = {  8,  0,  0,  0,  0,  0,  0,  0,  0,  0, 68, 40, 16,  0,  0};
static const unsigned char bmpChar_8x13_095[] = {  8,  0,  0,254,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_096[] = {  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  4, 24, 56,  0,  0};
static const unsigned char bmpChar_8x13_097[] = {  8,  0,  0,  0, 58, 70, 66, 62,  2, 60,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_098[] = {  8,  0,  0,  0, 92, 98, 66, 66, 98, 92, 64, 64, 64,  0,  0};
static const unsigned char bmpChar_8x13_099[] = {  8,  0,  0,  0, 60, 66, 64, 64, 66, 60,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_100[] = {  8,  0,  0,  0, 58, 70, 66, 66, 70, 58,  2,  2,  2,  0,  0};
static const unsigned char bmpChar_8x13_101[] = {  8,  0,  0,  0, 60, 66, 64,126, 66, 60,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_102[] = {  8,  0,  0,  0, 32, 32, 32, 32,124, 32, 32, 34, 28,  0,  0};
static const unsigned char bmpChar_8x13_103[] = {  8,  0, 60, 66, 60, 64, 56, 68, 68, 58,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_104[] = {  8,  0,  0,  0, 66, 66, 66, 66, 98, 92, 64, 64, 64,  0,  0};
static const unsigned char bmpChar_8x13_105[] = {  8,  0,  0,  0,124, 16, 16, 16, 16, 48,  0, 16,  0,  0,  0};
static const unsigned char bmpChar_8x13_106[] = {  8,  0, 56, 68, 68,  4,  4,  4,  4, 12,  0,  4,  0,  0,  0};
static const unsigned char bmpChar_8x13_107[] = {  8,  0,  0,  0, 66, 68, 72,112, 72, 68, 64, 64, 64,  0,  0};
static const unsigned char bmpChar_8x13_108[] = {  8,  0,  0,  0,124, 16, 16, 16, 16, 16, 16, 16, 48,  0,  0};
static const unsigned char bmpChar_8x13_109[] = {  8,  0,  0,  0,130,146,146,146,146,236,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_110[] = {  8,  0,  0,  0, 66, 66, 66, 66, 98, 92,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_111[] = {  8,  0,  0,  0, 60, 66, 66, 66, 66, 60,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_112[] = {  8,  0, 64, 64, 64, 92, 98, 66, 98, 92,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_113[] = {  8,  0,  2,  2,  2, 58, 70, 66, 70, 58,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_114[] = {  8,  0,  0,  0, 32, 32, 32, 32, 34, 92,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_115[] = {  8,  0,  0,  0, 60, 66, 12, 48, 66, 60,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_116[] = {  8,  0,  0,  0, 28, 34, 32, 32, 32,124, 32, 32,  0,  0,  0};
static const unsigned char bmpChar_8x13_117[] = {  8,  0,  0,  0, 58, 68, 68, 68, 68, 68,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_118[] = {  8,  0,  0,  0, 16, 40, 40, 68, 68, 68,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_119[] = {  8,  0,  0,  0, 68,170,146,146,130,130,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_120[] = {  8,  0,  0,  0, 66, 36, 24, 24, 36, 66,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_121[] = {  8,  0, 60, 66,  2, 58, 70, 66, 66, 66,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_122[] = {  8,  0,  0,  0,126, 32, 16,  8,  4,126,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_123[] = {  8,  0,  0,  0, 14, 16, 16,  8, 48,  8, 16, 16, 14,  0,  0};
static const unsigned char bmpChar_8x13_124[] = {  8,  0,  0,  0, 16, 16, 16, 16, 16, 16, 16, 16, 16,  0,  0};
static const unsigned char bmpChar_8x13_125[] = {  8,  0,  0,  0,112,  8,  8, 16, 12, 16,  8,  8,112,  0,  0};
static const unsigned char bmpChar_8x13_126[] = {  8,  0,  0,  0,  0,  0,  0,  0,  0,  0, 72, 84, 36,  0,  0};
static const unsigned char bmpChar_8x13_127[] = {  9,  0,  0,  0,  0,  0,  0,170,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,170,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_128[] = {  9,  0,  0,  0,  0,  0,  0,170,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,170,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_129[] = {  9,  0,  0,  0,  0,  0,  0,170,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,170,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_130[] = {  9,  0,  0,  0,  0,  0,  0,170,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,170,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_131[] = {  9,  0,  0,  0,  0,  0,  0,170,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,170,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_132[] = {  9,  0,  0,  0,  0,  0,  0,170,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,170,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_133[] = {  9,  0,  0,  0,  0,  0,  0,170,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,170,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_134[] = {  9,  0,  0,  0,  0,  0,  0,170,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,170,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_135[] = {  9,  0,  0,  0,  0,  0,  0,170,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,170,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_136[] = {  9,  0,  0,  0,  0,  0,  0,170,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,170,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_137[] = {  9,  0,  0,  0,  0,  0,  0,170,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,170,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_138[] = {  9,  0,  0,  0,  0,  0,  0,170,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,170,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_139[] = {  9,  0,  0,  0,  0,  0,  0,170,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,170,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_140[] = {  9,  0,  0,  0,  0,  0,  0,170,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,170,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_141[] = {  9,  0,  0,  0,  0,  0,  0,170,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,170,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_142[] = {  9,  0,  0,  0,  0,  0,  0,170,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,170,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_143[] = {  9,  0,  0,  0,  0,  0,  0,170,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,170,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_144[] = {  9,  0,  0,  0,  0,  0,  0,170,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,170,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_145[] = {  9,  0,  0,  0,  0,  0,  0,170,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,170,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_146[] = {  9,  0,  0,  0,  0,  0,  0,170,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,170,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_147[] = {  9,  0,  0,  0,  0,  0,  0,170,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,170,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_148[] = {  9,  0,  0,  0,  0,  0,  0,170,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,170,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_149[] = {  9,  0,  0,  0,  0,  0,  0,170,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,170,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_150[] = {  9,  0,  0,  0,  0,  0,  0,170,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,170,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_151[] = {  9,  0,  0,  0,  0,  0,  0,170,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,170,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_152[] = {  9,  0,  0,  0,  0,  0,  0,170,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,170,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_153[] = {  9,  0,  0,  0,  0,  0,  0,170,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,170,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_154[] = {  9,  0,  0,  0,  0,  0,  0,170,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,170,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_155[] = {  9,  0,  0,  0,  0,  0,  0,170,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,170,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_156[] = {  9,  0,  0,  0,  0,  0,  0,170,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,170,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_157[] = {  9,  0,  0,  0,  0,  0,  0,170,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,170,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_158[] = {  9,  0,  0,  0,  0,  0,  0,170,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,170,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_159[] = {  9,  0,  0,  0,  0,  0,  0,170,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,130,  0,  0,  0,170,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_160[] = {  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_161[] = {  8,  0,  0,  0, 16, 16, 16, 16, 16, 16, 16,  0, 16,  0,  0};
static const unsigned char bmpChar_8x13_162[] = {  8,  0,  0,  0,  0, 16, 56, 84, 80, 80, 84, 56, 16,  0,  0};
static const unsigned char bmpChar_8x13_163[] = {  8,  0,  0,  0,220, 98, 32, 32, 32,112, 32, 34, 28,  0,  0};
static const unsigned char bmpChar_8x13_164[] = {  8,  0,  0,  0,  0, 66, 60, 36, 36, 60, 66,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_165[] = {  8,  0,  0,  0, 16, 16,124, 16,124, 40, 68,130,130,  0,  0};
static const unsigned char bmpChar_8x13_166[] = {  8,  0,  0,  0, 16, 16, 16, 16,  0, 16, 16, 16, 16,  0,  0};
static const unsigned char bmpChar_8x13_167[] = {  8,  0,  0,  0, 24, 36,  4, 24, 36, 36, 24, 32, 36, 24,  0};
static const unsigned char bmpChar_8x13_168[] = {  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,108,  0,  0};
static const unsigned char bmpChar_8x13_169[] = {  8,  0,  0,  0,  0, 56, 68,146,170,162,170,146, 68, 56,  0};
static const unsigned char bmpChar_8x13_170[] = {  8,  0,  0,  0,  0,  0,124,  0, 60, 68, 60,  4, 56,  0,  0};
static const unsigned char bmpChar_8x13_171[] = {  8,  0,  0,  0,  0, 18, 36, 72,144, 72, 36, 18,  0,  0,  0};
static const unsigned char bmpChar_8x13_172[] = {  8,  0,  0,  0,  0,  2,  2,  2,126,  0,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_173[] = {  8,  0,  0,  0,  0,  0,  0,  0, 60,  0,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_174[] = {  8,  0,  0,  0,  0, 56, 68,170,178,170,170,146, 68, 56,  0};
static const unsigned char bmpChar_8x13_175[] = {  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,126,  0,  0};
static const unsigned char bmpChar_8x13_176[] = {  8,  0,  0,  0,  0,  0,  0,  0,  0, 24, 36, 36, 24,  0,  0};
static const unsigned char bmpChar_8x13_177[] = {  8,  0,  0,  0,  0,124,  0, 16, 16,124, 16, 16,  0,  0,  0};
static const unsigned char bmpChar_8x13_178[] = {  8,  0,  0,  0,  0,  0,  0,  0,120, 64, 48,  8, 72, 48,  0};
static const unsigned char bmpChar_8x13_179[] = {  8,  0,  0,  0,  0,  0,  0,  0, 48, 72,  8, 16, 72, 48,  0};
static const unsigned char bmpChar_8x13_180[] = {  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 16,  8,  0};
static const unsigned char bmpChar_8x13_181[] = {  8,  0,  0, 64, 90,102, 66, 66, 66, 66,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_182[] = {  8,  0,  0,  0, 20, 20, 20, 20, 52,116,116,116, 62,  0,  0};
static const unsigned char bmpChar_8x13_183[] = {  8,  0,  0,  0,  0,  0,  0,  0, 24,  0,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_184[] = {  8,  0, 24,  8,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_185[] = {  8,  0,  0,  0,  0,  0,  0,  0,112, 32, 32, 32, 96, 32,  0};
static const unsigned char bmpChar_8x13_186[] = {  8,  0,  0,  0,  0,  0,  0,120,  0, 48, 72, 72, 48,  0,  0};
static const unsigned char bmpChar_8x13_187[] = {  8,  0,  0,  0,  0,144, 72, 36, 18, 36, 72,144,  0,  0,  0};
static const unsigned char bmpChar_8x13_188[] = {  8,  0,  0,  0,  6, 26, 18, 10,230, 66, 64, 64,192, 64,  0};
static const unsigned char bmpChar_8x13_189[] = {  8,  0,  0,  0, 30, 16, 12,  2,242, 76, 64, 64,192, 64,  0};
static const unsigned char bmpChar_8x13_190[] = {  8,  0,  0,  0,  6, 26, 18, 10,102,146, 16, 32,144, 96,  0};
static const unsigned char bmpChar_8x13_191[] = {  8,  0,  0,  0, 60, 66, 66, 64, 32, 16, 16,  0, 16,  0,  0};
static const unsigned char bmpChar_8x13_192[] = {  8,  0,  0,  0, 66, 66,126, 66, 66, 36, 24,  0,  8, 16,  0};
static const unsigned char bmpChar_8x13_193[] = {  8,  0,  0,  0, 66, 66,126, 66, 66, 36, 24,  0, 16,  8,  0};
static const unsigned char bmpChar_8x13_194[] = {  8,  0,  0,  0, 66, 66,126, 66, 66, 36, 24,  0, 36, 24,  0};
static const unsigned char bmpChar_8x13_195[] = {  8,  0,  0,  0, 66, 66,126, 66, 66, 36, 24,  0, 76, 50,  0};
static const unsigned char bmpChar_8x13_196[] = {  8,  0,  0,  0, 66, 66,126, 66, 66, 36, 24,  0, 36, 36,  0};
static const unsigned char bmpChar_8x13_197[] = {  8,  0,  0,  0, 66, 66,126, 66, 66, 36, 24, 24, 36, 24,  0};
static const unsigned char bmpChar_8x13_198[] = {  8,  0,  0,  0,158,144,144,240,156,144,144,144,110,  0,  0};
static const unsigned char bmpChar_8x13_199[] = {  8,  0, 16,  8, 60, 66, 64, 64, 64, 64, 64, 66, 60,  0,  0};
static const unsigned char bmpChar_8x13_200[] = {  8,  0,  0,  0,126, 64, 64,120, 64, 64,126,  0,  8, 16,  0};
static const unsigned char bmpChar_8x13_201[] = {  8,  0,  0,  0,126, 64, 64,120, 64, 64,126,  0, 16,  8,  0};
static const unsigned char bmpChar_8x13_202[] = {  8,  0,  0,  0,126, 64, 64,120, 64, 64,126,  0, 36, 24,  0};
static const unsigned char bmpChar_8x13_203[] = {  8,  0,  0,  0,126, 64, 64,120, 64, 64,126,  0, 36, 36,  0};
static const unsigned char bmpChar_8x13_204[] = {  8,  0,  0,  0,124, 16, 16, 16, 16, 16,124,  0, 16, 32,  0};
static const unsigned char bmpChar_8x13_205[] = {  8,  0,  0,  0,124, 16, 16, 16, 16, 16,124,  0, 16,  8,  0};
static const unsigned char bmpChar_8x13_206[] = {  8,  0,  0,  0,124, 16, 16, 16, 16, 16,124,  0, 36, 24,  0};
static const unsigned char bmpChar_8x13_207[] = {  8,  0,  0,  0,124, 16, 16, 16, 16, 16,124,  0, 40, 40,  0};
static const unsigned char bmpChar_8x13_208[] = {  8,  0,  0,  0,120, 68, 66, 66,226, 66, 66, 68,120,  0,  0};
static const unsigned char bmpChar_8x13_209[] = {  8,  0,  0,  0,130,134,138,146,162,194,130,  0,152,100,  0};
static const unsigned char bmpChar_8x13_210[] = {  8,  0,  0,  0,124,130,130,130,130,130,124,  0, 16, 32,  0};
static const unsigned char bmpChar_8x13_211[] = {  8,  0,  0,  0,124,130,130,130,130,130,124,  0, 16,  8,  0};
static const unsigned char bmpChar_8x13_212[] = {  8,  0,  0,  0,124,130,130,130,130,130,124,  0, 36, 24,  0};
static const unsigned char bmpChar_8x13_213[] = {  8,  0,  0,  0,124,130,130,130,130,130,124,  0,152,100,  0};
static const unsigned char bmpChar_8x13_214[] = {  8,  0,  0,  0,124,130,130,130,130,130,124,  0, 40, 40,  0};
static const unsigned char bmpChar_8x13_215[] = {  8,  0,  0,  0,  0, 66, 36, 24, 24, 36, 66,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_216[] = {  8,  0,  0, 64, 60, 98, 82, 82, 82, 74, 74, 70, 60,  2,  0};
static const unsigned char bmpChar_8x13_217[] = {  8,  0,  0,  0, 60, 66, 66, 66, 66, 66, 66,  0,  8, 16,  0};
static const unsigned char bmpChar_8x13_218[] = {  8,  0,  0,  0, 60, 66, 66, 66, 66, 66, 66,  0, 16,  8,  0};
static const unsigned char bmpChar_8x13_219[] = {  8,  0,  0,  0, 60, 66, 66, 66, 66, 66, 66,  0, 36, 24,  0};
static const unsigned char bmpChar_8x13_220[] = {  8,  0,  0,  0, 60, 66, 66, 66, 66, 66, 66,  0, 36, 36,  0};
static const unsigned char bmpChar_8x13_221[] = {  8,  0,  0,  0, 16, 16, 16, 16, 40, 68, 68,  0, 16,  8,  0};
static const unsigned char bmpChar_8x13_222[] = {  8,  0,  0,  0, 64, 64, 64,124, 66, 66, 66,124, 64,  0,  0};
static const unsigned char bmpChar_8x13_223[] = {  8,  0,  0,  0, 92, 66, 66, 76, 80, 72, 68, 68, 56,  0,  0};
static const unsigned char bmpChar_8x13_224[] = {  8,  0,  0,  0, 58, 70, 66, 62,  2, 60,  0,  0,  8, 16,  0};
static const unsigned char bmpChar_8x13_225[] = {  8,  0,  0,  0, 58, 70, 66, 62,  2, 60,  0,  0,  8,  4,  0};
static const unsigned char bmpChar_8x13_226[] = {  8,  0,  0,  0, 58, 70, 66, 62,  2, 60,  0,  0, 36, 24,  0};
static const unsigned char bmpChar_8x13_227[] = {  8,  0,  0,  0, 58, 70, 66, 62,  2, 60,  0,  0, 76, 50,  0};
static const unsigned char bmpChar_8x13_228[] = {  8,  0,  0,  0, 58, 70, 66, 62,  2, 60,  0,  0, 36, 36,  0};
static const unsigned char bmpChar_8x13_229[] = {  8,  0,  0,  0, 58, 70, 66, 62,  2, 60,  0, 24, 36, 24,  0};
static const unsigned char bmpChar_8x13_230[] = {  8,  0,  0,  0,108,146,144,124, 18,108,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_231[] = {  8,  0, 16,  8, 60, 66, 64, 64, 66, 60,  0,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_232[] = {  8,  0,  0,  0, 60, 66, 64,126, 66, 60,  0,  0,  8, 16,  0};
static const unsigned char bmpChar_8x13_233[] = {  8,  0,  0,  0, 60, 66, 64,126, 66, 60,  0,  0, 16,  8,  0};
static const unsigned char bmpChar_8x13_234[] = {  8,  0,  0,  0, 60, 66, 64,126, 66, 60,  0,  0, 36, 24,  0};
static const unsigned char bmpChar_8x13_235[] = {  8,  0,  0,  0, 60, 66, 64,126, 66, 60,  0,  0, 36, 36,  0};
static const unsigned char bmpChar_8x13_236[] = {  8,  0,  0,  0,124, 16, 16, 16, 16, 48,  0,  0, 16, 32,  0};
static const unsigned char bmpChar_8x13_237[] = {  8,  0,  0,  0,124, 16, 16, 16, 16, 48,  0,  0, 32, 16,  0};
static const unsigned char bmpChar_8x13_238[] = {  8,  0,  0,  0,124, 16, 16, 16, 16, 48,  0,  0, 72, 48,  0};
static const unsigned char bmpChar_8x13_239[] = {  8,  0,  0,  0,124, 16, 16, 16, 16, 48,  0,  0, 40, 40,  0};
static const unsigned char bmpChar_8x13_240[] = {  8,  0,  0,  0, 60, 66, 66, 66, 66, 60,  4, 40, 24, 36,  0};
static const unsigned char bmpChar_8x13_241[] = {  8,  0,  0,  0, 66, 66, 66, 66, 98, 92,  0,  0, 76, 50,  0};
static const unsigned char bmpChar_8x13_242[] = {  8,  0,  0,  0, 60, 66, 66, 66, 66, 60,  0,  0, 16, 32,  0};
static const unsigned char bmpChar_8x13_243[] = {  8,  0,  0,  0, 60, 66, 66, 66, 66, 60,  0,  0, 16,  8,  0};
static const unsigned char bmpChar_8x13_244[] = {  8,  0,  0,  0, 60, 66, 66, 66, 66, 60,  0,  0, 36, 24,  0};
static const unsigned char bmpChar_8x13_245[] = {  8,  0,  0,  0, 60, 66, 66, 66, 66, 60,  0,  0, 76, 50,  0};
static const unsigned char bmpChar_8x13_246[] = {  8,  0,  0,  0, 60, 66, 66, 66, 66, 60,  0,  0, 36, 36,  0};
static const unsigned char bmpChar_8x13_247[] = {  8,  0,  0,  0,  0, 16, 16,  0,124,  0, 16, 16,  0,  0,  0};
static const unsigned char bmpChar_8x13_248[] = {  8,  0,  0, 64, 60, 98, 82, 74, 70, 60,  2,  0,  0,  0,  0};
static const unsigned char bmpChar_8x13_249[] = {  8,  0,  0,  0, 58, 68, 68, 68, 68, 68,  0,  0, 16, 32,  0};
static const unsigned char bmpChar_8x13_250[] = {  8,  0,  0,  0, 58, 68, 68, 68, 68, 68,  0,  0, 16,  8,  0};
static const unsigned char bmpChar_8x13_251[] = {  8,  0,  0,  0, 58, 68, 68, 68, 68, 68,  0,  0, 36, 24,  0};
static const unsigned char bmpChar_8x13_252[] = {  8,  0,  0,  0, 58, 68, 68, 68, 68, 68,  0,  0, 40, 40,  0};
static const unsigned char bmpChar_8x13_253[] = {  8,  0, 60, 66,  2, 58, 70, 66, 66, 66,  0,  0, 16,  8,  0};
static const unsigned char bmpChar_8x13_254[] = {  8,  0, 64, 64, 92, 98, 66, 66, 98, 92, 64, 64,  0,  0,  0};
static const unsigned char bmpChar_8x13_255[] = {  8,  0, 60, 66,  2, 58, 70, 66, 66, 66,  0,  0, 36, 36,  0};

/* The font characters mapping: */
static const unsigned char* bmpChar_8x13_Map[] = {	bmpChar_8x13_000,bmpChar_8x13_001,bmpChar_8x13_002,bmpChar_8x13_003,bmpChar_8x13_004,bmpChar_8x13_005,bmpChar_8x13_006,bmpChar_8x13_007,bmpChar_8x13_008,bmpChar_8x13_009,bmpChar_8x13_010,bmpChar_8x13_011,bmpChar_8x13_012,bmpChar_8x13_013,bmpChar_8x13_014,bmpChar_8x13_015,
													bmpChar_8x13_016,bmpChar_8x13_017,bmpChar_8x13_018,bmpChar_8x13_019,bmpChar_8x13_020,bmpChar_8x13_021,bmpChar_8x13_022,bmpChar_8x13_023,bmpChar_8x13_024,bmpChar_8x13_025,bmpChar_8x13_026,bmpChar_8x13_027,bmpChar_8x13_028,bmpChar_8x13_029,bmpChar_8x13_030,bmpChar_8x13_031,
													bmpChar_8x13_032,bmpChar_8x13_033,bmpChar_8x13_034,bmpChar_8x13_035,bmpChar_8x13_036,bmpChar_8x13_037,bmpChar_8x13_038,bmpChar_8x13_039,bmpChar_8x13_040,bmpChar_8x13_041,bmpChar_8x13_042,bmpChar_8x13_043,bmpChar_8x13_044,bmpChar_8x13_045,bmpChar_8x13_046,bmpChar_8x13_047,
													bmpChar_8x13_048,bmpChar_8x13_049,bmpChar_8x13_050,bmpChar_8x13_051,bmpChar_8x13_052,bmpChar_8x13_053,bmpChar_8x13_054,bmpChar_8x13_055,bmpChar_8x13_056,bmpChar_8x13_057,bmpChar_8x13_058,bmpChar_8x13_059,bmpChar_8x13_060,bmpChar_8x13_061,bmpChar_8x13_062,bmpChar_8x13_063,
													bmpChar_8x13_064,bmpChar_8x13_065,bmpChar_8x13_066,bmpChar_8x13_067,bmpChar_8x13_068,bmpChar_8x13_069,bmpChar_8x13_070,bmpChar_8x13_071,bmpChar_8x13_072,bmpChar_8x13_073,bmpChar_8x13_074,bmpChar_8x13_075,bmpChar_8x13_076,bmpChar_8x13_077,bmpChar_8x13_078,bmpChar_8x13_079,
													bmpChar_8x13_080,bmpChar_8x13_081,bmpChar_8x13_082,bmpChar_8x13_083,bmpChar_8x13_084,bmpChar_8x13_085,bmpChar_8x13_086,bmpChar_8x13_087,bmpChar_8x13_088,bmpChar_8x13_089,bmpChar_8x13_090,bmpChar_8x13_091,bmpChar_8x13_092,bmpChar_8x13_093,bmpChar_8x13_094,bmpChar_8x13_095,
													bmpChar_8x13_096,bmpChar_8x13_097,bmpChar_8x13_098,bmpChar_8x13_099,bmpChar_8x13_100,bmpChar_8x13_101,bmpChar_8x13_102,bmpChar_8x13_103,bmpChar_8x13_104,bmpChar_8x13_105,bmpChar_8x13_106,bmpChar_8x13_107,bmpChar_8x13_108,bmpChar_8x13_109,bmpChar_8x13_110,bmpChar_8x13_111,
													bmpChar_8x13_112,bmpChar_8x13_113,bmpChar_8x13_114,bmpChar_8x13_115,bmpChar_8x13_116,bmpChar_8x13_117,bmpChar_8x13_118,bmpChar_8x13_119,bmpChar_8x13_120,bmpChar_8x13_121,bmpChar_8x13_122,bmpChar_8x13_123,bmpChar_8x13_124,bmpChar_8x13_125,bmpChar_8x13_126,bmpChar_8x13_032,
													bmpChar_8x13_032,bmpChar_8x13_032,bmpChar_8x13_032,bmpChar_8x13_032,bmpChar_8x13_032,bmpChar_8x13_032,bmpChar_8x13_032,bmpChar_8x13_032,bmpChar_8x13_032,bmpChar_8x13_032,bmpChar_8x13_032,bmpChar_8x13_032,bmpChar_8x13_032,bmpChar_8x13_032,bmpChar_8x13_032,bmpChar_8x13_032,
													bmpChar_8x13_032,bmpChar_8x13_032,bmpChar_8x13_032,bmpChar_8x13_032,bmpChar_8x13_032,bmpChar_8x13_032,bmpChar_8x13_032,bmpChar_8x13_032,bmpChar_8x13_032,bmpChar_8x13_032,bmpChar_8x13_032,bmpChar_8x13_032,bmpChar_8x13_032,bmpChar_8x13_032,bmpChar_8x13_032,bmpChar_8x13_032,
													bmpChar_8x13_160,bmpChar_8x13_161,bmpChar_8x13_162,bmpChar_8x13_163,bmpChar_8x13_164,bmpChar_8x13_165,bmpChar_8x13_166,bmpChar_8x13_167,bmpChar_8x13_168,bmpChar_8x13_169,bmpChar_8x13_170,bmpChar_8x13_171,bmpChar_8x13_172,bmpChar_8x13_173,bmpChar_8x13_174,bmpChar_8x13_175,
													bmpChar_8x13_176,bmpChar_8x13_177,bmpChar_8x13_178,bmpChar_8x13_179,bmpChar_8x13_180,bmpChar_8x13_181,bmpChar_8x13_182,bmpChar_8x13_183,bmpChar_8x13_184,bmpChar_8x13_185,bmpChar_8x13_186,bmpChar_8x13_187,bmpChar_8x13_188,bmpChar_8x13_189,bmpChar_8x13_190,bmpChar_8x13_191,
													bmpChar_8x13_192,bmpChar_8x13_193,bmpChar_8x13_194,bmpChar_8x13_195,bmpChar_8x13_196,bmpChar_8x13_197,bmpChar_8x13_198,bmpChar_8x13_199,bmpChar_8x13_200,bmpChar_8x13_201,bmpChar_8x13_202,bmpChar_8x13_203,bmpChar_8x13_204,bmpChar_8x13_205,bmpChar_8x13_206,bmpChar_8x13_207,
													bmpChar_8x13_208,bmpChar_8x13_209,bmpChar_8x13_210,bmpChar_8x13_211,bmpChar_8x13_212,bmpChar_8x13_213,bmpChar_8x13_214,bmpChar_8x13_215,bmpChar_8x13_216,bmpChar_8x13_217,bmpChar_8x13_218,bmpChar_8x13_219,bmpChar_8x13_220,bmpChar_8x13_221,bmpChar_8x13_222,bmpChar_8x13_223,
													bmpChar_8x13_224,bmpChar_8x13_225,bmpChar_8x13_226,bmpChar_8x13_227,bmpChar_8x13_228,bmpChar_8x13_229,bmpChar_8x13_230,bmpChar_8x13_231,bmpChar_8x13_232,bmpChar_8x13_233,bmpChar_8x13_234,bmpChar_8x13_235,bmpChar_8x13_236,bmpChar_8x13_237,bmpChar_8x13_238,bmpChar_8x13_239,
													bmpChar_8x13_240,bmpChar_8x13_241,bmpChar_8x13_242,bmpChar_8x13_243,bmpChar_8x13_244,bmpChar_8x13_245,bmpChar_8x13_246,bmpChar_8x13_247,bmpChar_8x13_248,bmpChar_8x13_249,bmpChar_8x13_250,bmpChar_8x13_251,bmpChar_8x13_252,bmpChar_8x13_253,bmpChar_8x13_254,bmpChar_8x13_255,NULL};



#include "ofTexture.h"
#include "ofGLProgrammableRenderer.h"

static bool				bBitmapTexturePrepared = false;
static ofTexture		bitmappedFontTexture;

#ifdef TARGET_OPENGLES
//---------------------------------------------------------------------
// tig: does this actually do anything?
void ofUpdateBitmapCharacterTexture(){
	bBitmapTexturePrepared = false;
}
#endif

static ofPixels myLetterPixels;
static float widthTex = 8.0f/256.0f;
static float heightTex = 14.0f/256.0f;
static ofMesh charMesh;
static int vC = 0;

//---------------------------------------------------------------------
static void prepareBitmapTexture(){

			
	
	if (!bBitmapTexturePrepared){
		myLetterPixels.allocate(16*16, 16*16, 4); // letter size:8x14pixels, texture size:16x8letters, gl_rgba: 4bytes/1pixel
        myLetterPixels.set(0);

		bitmappedFontTexture.allocate(16*16, 16*16, GL_RGBA, false);
		
		bBitmapTexturePrepared = true;
		
		for (int i = 0; i < 256; i++) {
			
			const unsigned char * face = bmpChar_8x13_Map[i];
			
			for (int j = 1; j < 15; j++){
				for (int k = 0; k < 8; k++){
					if ( ((face[15-j] << k) & (128)) > 0 ){
						myLetterPixels[(((int)(i/16))*16*16*16+(i%16)*16 + (j-1)*16*16 + k)*4] = 255;
						myLetterPixels[(((int)(i/16))*16*16*16+(i%16)*16 + (j-1)*16*16 + k)*4+1] = 255;
						myLetterPixels[(((int)(i/16))*16*16*16+(i%16)*16 + (j-1)*16*16 + k)*4+2] = 255;
						myLetterPixels[(((int)(i/16))*16*16*16+(i%16)*16 + (j-1)*16*16 + k)*4+3] = 255;
					}else{
						myLetterPixels[(((int)(i/16))*16*16*16+(i%16)*16 + (j-1)*16*16 + k)*4] = 0;
						myLetterPixels[(((int)(i/16))*16*16*16+(i%16)*16 + (j-1)*16*16 + k)*4+1] = 0;
						myLetterPixels[(((int)(i/16))*16*16*16+(i%16)*16 + (j-1)*16*16 + k)*4+2] = 0;
						myLetterPixels[(((int)(i/16))*16*16*16+(i%16)*16 + (j-1)*16*16 + k)*4+3] = 0;
					}
				}
			}
		}
		
		bitmappedFontTexture.loadData(myLetterPixels);
		bitmappedFontTexture.setTextureMinMagFilter(GL_LINEAR,GL_NEAREST);

		charMesh.setMode(OF_PRIMITIVE_TRIANGLES);
		
	}

}
		
//---------------------------------------------------------------------
void  ofDrawBitmapCharacter(int character, int x , int y){

	if(!bBitmapTexturePrepared){
		prepareBitmapTexture();
	}
		
	if (character < 128) {		

		float posTexW = (float)(character % 16)/16.0f;
		float posTexH = ((int)(character / 16.0f))/16.0f;

		float texY1 = posTexH;
		float texY2 = posTexH+heightTex;

		//TODO: look into a better fix.
		//old ofDrawBitmapString was 3 pixels higher, so this version renders text in a different position.
		//3 pixel adjustment corrects that when y is flpped 5 when it's not.
		int yOffset = 14;
		if(!ofIsVFlipped()){
			y += 5;
			y += yOffset;
			yOffset *= -1;
		}else{
			y -= 3;
		}


		charMesh.getTexCoords()[vC].set(posTexW,texY1);
		charMesh.getTexCoords()[vC+1].set(posTexW + widthTex,texY1);
		charMesh.getTexCoords()[vC+2].set(posTexW+widthTex,texY2);

		charMesh.getTexCoords()[vC+3].set(posTexW + widthTex,texY2);
		charMesh.getTexCoords()[vC+4].set(posTexW,texY2);
		charMesh.getTexCoords()[vC+5].set(posTexW,texY1);

		charMesh.getVertices()[vC].set(x,y);
		charMesh.getVertices()[vC+1].set(x+8,y);
		charMesh.getVertices()[vC+2].set(x+8,y+yOffset);

		charMesh.getVertices()[vC+3].set(x+8,y+yOffset);
		charMesh.getVertices()[vC+4].set(x,y+yOffset);
		charMesh.getVertices()[vC+5].set(x,y);
			
		vC += 6;
	}	
}

//---------------------------------------------------------------------
void ofDrawBitmapCharacterStart(int stringLength){
	charMesh.getVertices().resize(6 * stringLength);
	charMesh.getTexCoords().resize(6 * stringLength);

	if(!bBitmapTexturePrepared){
		prepareBitmapTexture();
	}
	
	vC = 0;
}

//---------------------------------------------------------------------
void ofDrawBitmapCharacterEnd(){
	if( vC > 0 ){
		charMesh.getVertices().resize(vC);
		charMesh.getTexCoords().resize(vC);
		bitmappedFontTexture.bind();

		ofPtr<ofGLProgrammableRenderer> programmableRenderer = ofGetGLProgrammableRenderer();

		if (!programmableRenderer){
			#ifndef TARGET_OPENGLES
				// this temporarily enables alpha testing,
				// which discards pixels unless their alpha is 1.0f
				glPushAttrib(GL_ENABLE_BIT | GL_COLOR_BUFFER_BIT);
				glEnable(GL_ALPHA_TEST);
				glAlphaFunc(GL_GREATER, 0);
			#endif
		}else{
			// glPush/PopAttrib is deprecated + we are doing the alpha test through a shader
			programmableRenderer->setAlphaBitmapText(true);
		}

		charMesh.draw();

		if (!programmableRenderer){
			#ifndef TARGET_OPENGLES
				glPopAttrib();
			#endif
		}else{
			programmableRenderer->setAlphaBitmapText(false);
		}

		bitmappedFontTexture.unbind();
	}

}

ofMesh & ofBitmapStringGetMesh(const string & text, int x, int y){

	int len = (int)text.length();
	//float yOffset = 0;
	float fontSize = 8.0f;
	bool bOrigin = false;

	float sx = x;
	float sy = y-fontSize;

	ofDrawBitmapCharacterStart(text.size());

	for(int c = 0; c < len; c++){
		if(text[c] == '\n'){

			sy += bOrigin ? -1 : 1 * (fontSize*1.7);
			sx = x;

			//glRasterPos2f(x,y + (int)yOffset);
		} else if (text[c] >= 32){
			// < 32 = control characters - don't draw
			// solves a bug with control characters
			// getting drawn when they ought to not be
			ofDrawBitmapCharacter(text[c], (int)sx, (int)sy);

			sx += fontSize;
		}
	}
	//We do this because its way faster
	charMesh.getVertices().resize(vC);
	charMesh.getTexCoords().resize(vC);
	return charMesh;

}

ofTexture & ofBitmapStringGetTextureRef(){
	if(!bBitmapTexturePrepared){
		prepareBitmapTexture();
	}
	return bitmappedFontTexture;
}


ofRectangle ofBitmapStringGetBoundingBox(const string & text, int x, int y){
	const ofMesh & mesh = ofBitmapStringGetMesh(text,x,y);
	ofVec2f max(numeric_limits<float>::min(),numeric_limits<float>::min());
	ofVec2f min(numeric_limits<float>::max(),numeric_limits<float>::max());
	for(int i=0;i< mesh.getNumVertices(); i++){
		const ofVec3f & p = mesh.getVertex(i);
		if(p.x<min.x) min.x = p.x;
		if(p.y<min.y) min.y = p.y;
		if(p.x>max.x) max.x = p.x;
		if(p.y>max.y) max.y = p.y;
	}
	return ofRectangle(min.x,min.y,max.x-min.x,max.y-min.y);
}
