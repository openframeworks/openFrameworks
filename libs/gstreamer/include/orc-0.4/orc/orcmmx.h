
#ifndef _ORC_MMX_H_
#define _ORC_MMX_H_

#include <orc/orcx86.h>

typedef enum {
  ORC_TARGET_MMX_MMX = (1<<0),
  ORC_TARGET_MMX_MMXEXT = (1<<1),
  ORC_TARGET_MMX_3DNOW = (1<<2),
  ORC_TARGET_MMX_3DNOWEXT = (1<<3),
  ORC_TARGET_MMX_SSSE3 = (1<<4),
  ORC_TARGET_MMX_SSE4_1 = (1<<5),
  ORC_TARGET_MMX_SSE4_2 = (1<<6),
  ORC_TARGET_MMX_FRAME_POINTER = (1<<7),
  ORC_TARGET_MMX_SHORT_JUMPS = (1<<8),
  ORC_TARGET_MMX_64BIT = (1<<9)
} OrcTargetMMXFlags;

typedef enum {
  X86_MM0 = ORC_VEC_REG_BASE,
  X86_MM1,
  X86_MM2,
  X86_MM3,
  X86_MM4,
  X86_MM5,
  X86_MM6,
  X86_MM7
} OrcMMXRegister;

#define ORC_MMX_SHUF(a,b,c,d) ((((a)&3)<<6)|(((b)&3)<<4)|(((c)&3)<<2)|(((d)&3)<<0))

const char * orc_x86_get_regname_mmx(int i);
void orc_x86_emit_mov_memoffset_mmx (OrcCompiler *compiler, int size, int offset,
    int reg1, int reg2, int is_aligned);
void orc_x86_emit_mov_memindex_mmx (OrcCompiler *compiler, int size, int offset,
    int reg1, int regindex, int shift, int reg2, int is_aligned);
void orc_x86_emit_mov_mmx_memoffset (OrcCompiler *compiler, int size, int reg1, int offset,
    int reg2, int aligned, int uncached);
void orc_x86_emit_mov_mmx_reg_reg (OrcCompiler *compiler, int reg1, int reg2);
void orc_x86_emit_mov_reg_mmx (OrcCompiler *compiler, int reg1, int reg2);
void orc_x86_emit_mov_mmx_reg (OrcCompiler *compiler, int reg1, int reg2);
void orc_mmx_emit_loadib (OrcCompiler *p, int reg, int value);
void orc_mmx_emit_loadiw (OrcCompiler *p, int reg, int value);
void orc_mmx_emit_loadil (OrcCompiler *p, int reg, int value);
void orc_mmx_emit_loadpb (OrcCompiler *p, int reg, int value);
void orc_mmx_emit_loadpw (OrcCompiler *p, int reg, int value);
void orc_mmx_emit_loadpl (OrcCompiler *p, int reg, int value);
void orc_mmx_emit_loadpq (OrcCompiler *p, int reg, int value);

void orc_mmx_emit_660f (OrcCompiler *p, const char *insn_name, int code,
    int src, int dest);
void orc_mmx_emit_f20f (OrcCompiler *p, const char *insn_name, int code,
    int src, int dest);
void orc_mmx_emit_f30f (OrcCompiler *p, const char *insn_name, int code,
    int src, int dest);
void orc_mmx_emit_0f (OrcCompiler *p, const char *insn_name, int code,
    int src, int dest);
void orc_mmx_emit_pshufw (OrcCompiler *p, int shuf, int src, int dest);
void orc_mmx_emit_palignr (OrcCompiler *p, int align, int src, int dest);
void orc_mmx_emit_pinsrw_memoffset (OrcCompiler *p, int imm, int offset,
    int src, int dest);
void orc_mmx_emit_pextrw_memoffset (OrcCompiler *p, int imm, int src,
    int offset, int dest);
void orc_mmx_emit_shiftimm (OrcCompiler *p, const char *insn_name,
    int code, int modrm_code, int shift, int reg);
unsigned int orc_mmx_get_cpu_flags (void);

void orc_mmx_load_constant (OrcCompiler *compiler, int reg, int size,
    orc_uint64 value);

/* MMX instructions */
#define orc_mmx_emit_punpcklbw(p,a,b)  orc_mmx_emit_660f (p, "punpcklbw", 0x60, a, b)
#define orc_mmx_emit_punpcklwd(p,a,b)  orc_mmx_emit_660f (p, "punpcklwd", 0x61, a, b)
#define orc_mmx_emit_punpckldq(p,a,b)  orc_mmx_emit_660f (p, "punpckldq", 0x62, a, b)
#define orc_mmx_emit_packsswb(p,a,b)   orc_mmx_emit_660f (p, "packsswb", 0x63, a, b)
#define orc_mmx_emit_pcmpgtb(p,a,b)    orc_mmx_emit_660f (p, "pcmpgtb", 0x64, a, b)
#define orc_mmx_emit_pcmpgtw(p,a,b)    orc_mmx_emit_660f (p, "pcmpgtw", 0x65, a, b)
#define orc_mmx_emit_pcmpgtd(p,a,b)    orc_mmx_emit_660f (p, "pcmpgtd", 0x66, a, b)
#define orc_mmx_emit_packuswb(p,a,b)   orc_mmx_emit_660f (p, "packuswb", 0x67, a, b)
#define orc_mmx_emit_punpckhbw(p,a,b)  orc_mmx_emit_660f (p, "punpckhbw", 0x68, a, b)
#define orc_mmx_emit_punpckhwd(p,a,b)  orc_mmx_emit_660f (p, "punpckhwd", 0x69, a, b)
#define orc_mmx_emit_punpckhdq(p,a,b)  orc_mmx_emit_660f (p, "punpckhdq", 0x6a, a, b)
#define orc_mmx_emit_packssdw(p,a,b)   orc_mmx_emit_660f (p, "packssdw", 0x6b, a, b)

#define orc_mmx_emit_movq(p,a,b)       orc_mmx_emit_660f (p, "movq", 0x6f, a, b)

#define orc_mmx_emit_psraw(p,a,b)      orc_mmx_emit_shiftimm (p, "psraw", 0x71, 4, a, b)
#define orc_mmx_emit_psrlw(p,a,b)      orc_mmx_emit_shiftimm (p, "psrlw", 0x71, 2, a, b)
#define orc_mmx_emit_psllw(p,a,b)      orc_mmx_emit_shiftimm (p, "psllw", 0x71, 6, a, b)

#define orc_mmx_emit_psrad(p,a,b)      orc_mmx_emit_shiftimm (p, "psrad", 0x72, 4, a, b)
#define orc_mmx_emit_psrld(p,a,b)      orc_mmx_emit_shiftimm (p, "psrld", 0x72, 2, a, b)
#define orc_mmx_emit_pslld(p,a,b)      orc_mmx_emit_shiftimm (p, "pslld", 0x72, 6, a, b)

#define orc_mmx_emit_psrlq(p,a,b)      orc_mmx_emit_shiftimm (p, "psrlq", 0x73, 2, a, b)
#define orc_mmx_emit_psllq(p,a,b)      orc_mmx_emit_shiftimm (p, "psllq", 0x73, 6, a, b)

#define orc_mmx_emit_pcmpeqb(p,a,b)    orc_mmx_emit_660f (p, "pcmpeqb", 0x74, a, b)
#define orc_mmx_emit_pcmpeqw(p,a,b)    orc_mmx_emit_660f (p, "pcmpeqw", 0x75, a, b)
#define orc_mmx_emit_pcmpeqd(p,a,b)    orc_mmx_emit_660f (p, "pcmpeqd", 0x76, a, b)
#define orc_mmx_emit_emms(p)           orc_x86_emit_emms (p)

#define orc_mmx_emit_pmullw(p,a,b)     orc_mmx_emit_660f (p, "pmullw", 0xd5, a, b)

#define orc_mmx_emit_psubusb(p,a,b)    orc_mmx_emit_660f (p, "psubusb", 0xd8, a, b)
#define orc_mmx_emit_psubusw(p,a,b)    orc_mmx_emit_660f (p, "psubusw", 0xd9, a, b)
#define orc_mmx_emit_pand(p,a,b)       orc_mmx_emit_660f (p, "pand", 0xdb, a, b)
#define orc_mmx_emit_paddusb(p,a,b)    orc_mmx_emit_660f (p, "paddusb", 0xdc, a, b)
#define orc_mmx_emit_paddusw(p,a,b)    orc_mmx_emit_660f (p, "paddusw", 0xdd, a, b)
#define orc_mmx_emit_pandn(p,a,b)      orc_mmx_emit_660f (p, "pandn", 0xdf, a, b)

#define orc_mmx_emit_pmulhw(p,a,b)     orc_mmx_emit_660f (p, "pmulhw", 0xe5, a, b)

#define orc_mmx_emit_psubsb(p,a,b)     orc_mmx_emit_660f (p, "psubsb", 0xe8, a, b)
#define orc_mmx_emit_psubsw(p,a,b)     orc_mmx_emit_660f (p, "psubsw", 0xe9, a, b)
#define orc_mmx_emit_por(p,a,b)        orc_mmx_emit_660f (p, "por", 0xeb, a, b)
#define orc_mmx_emit_paddsb(p,a,b)     orc_mmx_emit_660f (p, "paddsb", 0xec, a, b)
#define orc_mmx_emit_paddsw(p,a,b)     orc_mmx_emit_660f (p, "paddsw", 0xed, a, b)
#define orc_mmx_emit_pxor(p,a,b)       orc_mmx_emit_660f (p, "pxor", 0xef, a, b)

#define orc_mmx_emit_pmaddwd(p,a,b)    orc_mmx_emit_660f (p, "pmaddwd", 0xf5, a, b)

#define orc_mmx_emit_psubb(p,a,b)      orc_mmx_emit_660f (p, "psubb", 0xf8, a, b)
#define orc_mmx_emit_psubw(p,a,b)      orc_mmx_emit_660f (p, "psubw", 0xf9, a, b)
#define orc_mmx_emit_psubd(p,a,b)      orc_mmx_emit_660f (p, "psubd", 0xfa, a, b)

#define orc_mmx_emit_paddb(p,a,b)      orc_mmx_emit_660f (p, "paddb", 0xfc, a, b)
#define orc_mmx_emit_paddw(p,a,b)      orc_mmx_emit_660f (p, "paddw", 0xfd, a, b)
#define orc_mmx_emit_paddd(p,a,b)      orc_mmx_emit_660f (p, "paddd", 0xfe, a, b)

/* MMX EXT instructions */
/* MMX SSE instructions */
#define orc_mmx_emit_pinsrw(p,a,b)     orc_mmx_emit_660f (p, "pinsrw", 0xc4, a, b)
#define orc_mmx_emit_pextrw(p,a,b)     orc_mmx_emit_660f (p, "pextrw", 0xc5, a, b)
#define orc_mmx_emit_pminub(p,a,b)     orc_mmx_emit_660f (p, "pminub", 0xda, a, b)
#define orc_mmx_emit_pmaxub(p,a,b)     orc_mmx_emit_660f (p, "pmaxub", 0xde, a, b)
#define orc_mmx_emit_pavgb(p,a,b)      orc_mmx_emit_660f (p, "pavgb", 0xe0, a, b)
#define orc_mmx_emit_pavgw(p,a,b)      orc_mmx_emit_660f (p, "pavgw", 0xe3, a, b)
#define orc_mmx_emit_pmulhuw(p,a,b)    orc_mmx_emit_660f (p, "pmulhuw", 0xe4, a, b)
#define orc_mmx_emit_pminsw(p,a,b)     orc_mmx_emit_660f (p, "pminsw", 0xea, a, b)
#define orc_mmx_emit_pmaxsw(p,a,b)     orc_mmx_emit_660f (p, "pmaxsw", 0xee, a, b)
#define orc_mmx_emit_psadbw(p,a,b)     orc_mmx_emit_660f (p, "psadbw", 0xf6, a, b)

/* 3DNow! instrunctions, FIXME, not correct */
#define orc_mmx_emit_pmulhrw(p,a,b)    orc_mmx_emit_660f (p, "pmulhrw", 0xb7, a, b)
#define orc_mmx_emit_pavgusb(p,a,b)    orc_mmx_emit_660f (p, "pavgusb", 0xbf, a, b)

/* MMX SSE2 instructions */
#define orc_mmx_emit_paddq(p,a,b)      orc_mmx_emit_660f (p, "paddq", 0xd4, a, b)
#define orc_mmx_emit_psubq(p,a,b)      orc_mmx_emit_660f (p, "psubq", 0xfb, a, b)
#define orc_mmx_emit_pmuludq(p,a,b)    orc_mmx_emit_660f (p, "pmuludq", 0xf4, a, b)

/* MMX SSE3 instructions */

/* MMX SSSE3 instructions */
#define orc_mmx_emit_pshufb(p,a,b)     orc_mmx_emit_660f (p, "pshufb", 0x3800, a, b)
#define orc_mmx_emit_phaddw(p,a,b)     orc_mmx_emit_660f (p, "phaddw", 0x3801, a, b)
#define orc_mmx_emit_phaddd(p,a,b)     orc_mmx_emit_660f (p, "phaddd", 0x3802, a, b)
#define orc_mmx_emit_phaddsw(p,a,b)    orc_mmx_emit_660f (p, "phaddsw", 0x3803, a, b)
#define orc_mmx_emit_pmaddubsw(p,a,b)  orc_mmx_emit_660f (p, "pmaddubsw", 0x3804, a, b)
#define orc_mmx_emit_phsubw(p,a,b)     orc_mmx_emit_660f (p, "phsubw", 0x3805, a, b)
#define orc_mmx_emit_phsubd(p,a,b)     orc_mmx_emit_660f (p, "phsubd", 0x3806, a, b)
#define orc_mmx_emit_phsubsw(p,a,b)    orc_mmx_emit_660f (p, "phsubsw", 0x3807, a, b)
#define orc_mmx_emit_psignb(p,a,b)     orc_mmx_emit_660f (p, "psignb", 0x3808, a, b)
#define orc_mmx_emit_psignw(p,a,b)     orc_mmx_emit_660f (p, "psignw", 0x3809, a, b)
#define orc_mmx_emit_psignd(p,a,b)     orc_mmx_emit_660f (p, "psignd", 0x380a, a, b)
#define orc_mmx_emit_pmulhrsw(p,a,b)   orc_mmx_emit_660f (p, "pmulhrsw", 0x380b, a, b)

#define orc_mmx_emit_pabsb(p,a,b)      orc_mmx_emit_660f (p, "pabsb", 0x381c, a, b)
#define orc_mmx_emit_pabsw(p,a,b)      orc_mmx_emit_660f (p, "pabsw", 0x381d, a, b)
#define orc_mmx_emit_pabsd(p,a,b)      orc_mmx_emit_660f (p, "pabsd", 0x381e, a, b)

/* MMX SSE4.1 instructions */
#define orc_mmx_emit_pmovsxbw(p,a,b)   orc_mmx_emit_660f (p, "pmovsxbw", 0x3820, a, b)
#define orc_mmx_emit_pmovsxbd(p,a,b)   orc_mmx_emit_660f (p, "pmovsxbd", 0x3821, a, b)
#define orc_mmx_emit_pmovsxbq(p,a,b)   orc_mmx_emit_660f (p, "pmovsxbq", 0x3822, a, b)
#define orc_mmx_emit_pmovsxwd(p,a,b)   orc_mmx_emit_660f (p, "pmovsxwd", 0x3823, a, b)
#define orc_mmx_emit_pmovsxwq(p,a,b)   orc_mmx_emit_660f (p, "pmovsxwq", 0x3824, a, b)
#define orc_mmx_emit_pmovsxdq(p,a,b)   orc_mmx_emit_660f (p, "pmovsxdq", 0x3825, a, b)

#define orc_mmx_emit_pmuldq(p,a,b)     orc_mmx_emit_660f (p, "pmuldq", 0x3828, a, b)
#define orc_mmx_emit_pcmpeqq(p,a,b)    orc_mmx_emit_660f (p, "pcmpeqq", 0x3829, a, b)

#define orc_mmx_emit_packusdw(p,a,b)   orc_mmx_emit_660f (p, "packusdw", 0x382b, a, b)

#define orc_mmx_emit_pmovzxbw(p,a,b)   orc_mmx_emit_660f (p, "pmovzxbw", 0x3830, a, b)
#define orc_mmx_emit_pmovzxbd(p,a,b)   orc_mmx_emit_660f (p, "pmovzxbd", 0x3831, a, b)
#define orc_mmx_emit_pmovzxbq(p,a,b)   orc_mmx_emit_660f (p, "pmovzxbq", 0x3832, a, b)
#define orc_mmx_emit_pmovzxwd(p,a,b)   orc_mmx_emit_660f (p, "pmovzxwd", 0x3833, a, b)
#define orc_mmx_emit_pmovzxwq(p,a,b)   orc_mmx_emit_660f (p, "pmovzxwq", 0x3834, a, b)
#define orc_mmx_emit_pmovzxdq(p,a,b)   orc_mmx_emit_660f (p, "pmovzxdq", 0x3835, a, b)

#define orc_mmx_emit_pmulld(p,a,b)     orc_mmx_emit_660f (p, "pmuldq", 0x3840, a, b)
#define orc_mmx_emit_phminposuw(p,a,b) orc_mmx_emit_660f (p, "phminposuw", 0x3841, a, b)

#define orc_mmx_emit_pminsb(p,a,b)     orc_mmx_emit_660f (p, "pminsb", 0x3838, a, b)
#define orc_mmx_emit_pminsd(p,a,b)     orc_mmx_emit_660f (p, "pminsd", 0x3839, a, b)
#define orc_mmx_emit_pminuw(p,a,b)     orc_mmx_emit_660f (p, "pminuw", 0x383a, a, b)
#define orc_mmx_emit_pminud(p,a,b)     orc_mmx_emit_660f (p, "pminud", 0x383b, a, b)
#define orc_mmx_emit_pmaxsb(p,a,b)     orc_mmx_emit_660f (p, "pmaxsb", 0x383c, a, b)
#define orc_mmx_emit_pmaxsd(p,a,b)     orc_mmx_emit_660f (p, "pmaxsd", 0x383d, a, b)
#define orc_mmx_emit_pmaxuw(p,a,b)     orc_mmx_emit_660f (p, "pmaxuw", 0x383e, a, b)
#define orc_mmx_emit_pmaxud(p,a,b)     orc_mmx_emit_660f (p, "pmaxud", 0x383f, a, b)

/* SSE4.2 instructions */
#define orc_mmx_emit_pcmpgtq(p,a,b)    orc_mmx_emit_660f (p, "pcmpgtq", 0x3837, a, b)


#endif

