/*
     File:       Traps.h
 
     Contains:   A-Trap constants.
 
     Version:    Technology: Mac OS 9
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1985-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __TRAPS__
#define __TRAPS__

#ifndef __CONDITIONALMACROS__
#include <ConditionalMacros.h>
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
    #pragma options align=mac68k
#elif PRAGMA_STRUCT_PACKPUSH
    #pragma pack(push, 2)
#elif PRAGMA_STRUCT_PACK
    #pragma pack(2)
#endif

/*  QuickDraw */
enum {
    _NQDMisc                    = 0xABC3,
    _CopyMask                   = 0xA817,
    _MeasureText                = 0xA837,
    _GetMaskTable               = 0xA836,
    _CalcMask                   = 0xA838,
    _SeedFill                   = 0xA839,
    _InitCursor                 = 0xA850,
    _SetCursor                  = 0xA851,
    _HideCursor                 = 0xA852,
    _ShowCursor                 = 0xA853,
    _ShieldCursor               = 0xA855,
    _ObscureCursor              = 0xA856,
    _BitAnd                     = 0xA858,
    _BitXOr                     = 0xA859,
    _BitNot                     = 0xA85A,
    _BitOr                      = 0xA85B,
    _BitShift                   = 0xA85C,
    _BitTst                     = 0xA85D,
    _BitSet                     = 0xA85E,
    _BitClr                     = 0xA85F,
    _Random                     = 0xA861,
    _ForeColor                  = 0xA862,
    _BackColor                  = 0xA863,
    _ColorBit                   = 0xA864,
    _GetPixel                   = 0xA865,
    _StuffHex                   = 0xA866,
    _LongMul                    = 0xA867,
    _FixMul                     = 0xA868,
    _FixRatio                   = 0xA869,
    _HiWord                     = 0xA86A,
    _LoWord                     = 0xA86B,
    _FixRound                   = 0xA86C,
    _InitPort                   = 0xA86D,
    _InitGraf                   = 0xA86E,
    _OpenPort                   = 0xA86F,
    _LocalToGlobal              = 0xA870,
    _GlobalToLocal              = 0xA871,
    _GrafDevice                 = 0xA872,
    _SetPort                    = 0xA873,
    _GetPort                    = 0xA874,
    _SetPBits                   = 0xA875,
    _PortSize                   = 0xA876,
    _MovePortTo                 = 0xA877,
    _SetOrigin                  = 0xA878,
    _SetClip                    = 0xA879,
    _GetClip                    = 0xA87A,
    _ClipRect                   = 0xA87B,
    _BackPat                    = 0xA87C,
    _ClosePort                  = 0xA87D,
    _AddPt                      = 0xA87E,
    _SubPt                      = 0xA87F,
    _SetPt                      = 0xA880,
    _EqualPt                    = 0xA881,
    _StdText                    = 0xA882,
    _DrawChar                   = 0xA883,
    _DrawString                 = 0xA884,
    _DrawText                   = 0xA885,
    _TextWidth                  = 0xA886,
    _TextFont                   = 0xA887,
    _TextFace                   = 0xA888,
    _TextMode                   = 0xA889,
    _TextSize                   = 0xA88A,
    _GetFontInfo                = 0xA88B,
    _StringWidth                = 0xA88C,
    _CharWidth                  = 0xA88D,
    _SpaceExtra                 = 0xA88E,
    _StdLine                    = 0xA890,
    _LineTo                     = 0xA891,
    _Line                       = 0xA892
};

enum {
    _MoveTo                     = 0xA893,
    _Move                       = 0xA894,
    _ShutDown                   = 0xA895,
    _HidePen                    = 0xA896,
    _ShowPen                    = 0xA897,
    _GetPenState                = 0xA898,
    _SetPenState                = 0xA899,
    _GetPen                     = 0xA89A,
    _PenSize                    = 0xA89B,
    _PenMode                    = 0xA89C,
    _PenPat                     = 0xA89D,
    _PenNormal                  = 0xA89E,
    _Unimplemented              = 0xA89F,
    _StdRect                    = 0xA8A0,
    _FrameRect                  = 0xA8A1,
    _PaintRect                  = 0xA8A2,
    _EraseRect                  = 0xA8A3,
    _InverRect                  = 0xA8A4,
    _FillRect                   = 0xA8A5,
    _EqualRect                  = 0xA8A6,
    _SetRect                    = 0xA8A7,
    _OffsetRect                 = 0xA8A8,
    _InsetRect                  = 0xA8A9,
    _SectRect                   = 0xA8AA,
    _UnionRect                  = 0xA8AB,
    _Pt2Rect                    = 0xA8AC,
    _PtInRect                   = 0xA8AD,
    _EmptyRect                  = 0xA8AE,
    _StdRRect                   = 0xA8AF,
    _FrameRoundRect             = 0xA8B0,
    _PaintRoundRect             = 0xA8B1,
    _EraseRoundRect             = 0xA8B2,
    _InverRoundRect             = 0xA8B3,
    _FillRoundRect              = 0xA8B4,
    _StdOval                    = 0xA8B6,
    _FrameOval                  = 0xA8B7,
    _PaintOval                  = 0xA8B8,
    _EraseOval                  = 0xA8B9,
    _InvertOval                 = 0xA8BA,
    _FillOval                   = 0xA8BB,
    _SlopeFromAngle             = 0xA8BC,
    _StdArc                     = 0xA8BD,
    _FrameArc                   = 0xA8BE,
    _PaintArc                   = 0xA8BF,
    _EraseArc                   = 0xA8C0,
    _InvertArc                  = 0xA8C1,
    _FillArc                    = 0xA8C2,
    _PtToAngle                  = 0xA8C3,
    _AngleFromSlope             = 0xA8C4,
    _StdPoly                    = 0xA8C5,
    _FramePoly                  = 0xA8C6,
    _PaintPoly                  = 0xA8C7,
    _ErasePoly                  = 0xA8C8,
    _InvertPoly                 = 0xA8C9,
    _FillPoly                   = 0xA8CA,
    _OpenPoly                   = 0xA8CB,
    _ClosePgon                  = 0xA8CC,
    _ClosePoly                  = 0xA8CC,
    _KillPoly                   = 0xA8CD,
    _OffsetPoly                 = 0xA8CE,
    _PackBits                   = 0xA8CF,
    _UnpackBits                 = 0xA8D0,
    _StdRgn                     = 0xA8D1,
    _FrameRgn                   = 0xA8D2,
    _PaintRgn                   = 0xA8D3,
    _EraseRgn                   = 0xA8D4,
    _InverRgn                   = 0xA8D5,
    _FillRgn                    = 0xA8D6,
    _BitMapRgn                  = 0xA8D7,
    _BitMapToRegion             = 0xA8D7,
    _NewRgn                     = 0xA8D8,
    _DisposRgn                  = 0xA8D9,
    _DisposeRgn                 = 0xA8D9,
    _OpenRgn                    = 0xA8DA,
    _CloseRgn                   = 0xA8DB,
    _CopyRgn                    = 0xA8DC,
    _SetEmptyRgn                = 0xA8DD,
    _SetRecRgn                  = 0xA8DE,
    _RectRgn                    = 0xA8DF,
    _OffsetRgn                  = 0xA8E0,
    _InsetRgn                   = 0xA8E1,
    _EmptyRgn                   = 0xA8E2,
    _EqualRgn                   = 0xA8E3,
    _SectRgn                    = 0xA8E4,
    _UnionRgn                   = 0xA8E5,
    _DiffRgn                    = 0xA8E6,
    _XOrRgn                     = 0xA8E7,
    _PtInRgn                    = 0xA8E8,
    _RectInRgn                  = 0xA8E9,
    _SetStdProcs                = 0xA8EA,
    _StdBits                    = 0xA8EB,
    _CopyBits                   = 0xA8EC,
    _StdTxMeas                  = 0xA8ED,
    _StdGetPic                  = 0xA8EE,
    _ScrollRect                 = 0xA8EF,
    _StdPutPic                  = 0xA8F0,
    _StdComment                 = 0xA8F1,
    _PicComment                 = 0xA8F2,
    _OpenPicture                = 0xA8F3,
    _ClosePicture               = 0xA8F4,
    _KillPicture                = 0xA8F5,
    _DrawPicture                = 0xA8F6,
    _Layout                     = 0xA8F7,
    _ScalePt                    = 0xA8F8,
    _MapPt                      = 0xA8F9,
    _MapRect                    = 0xA8FA,
    _MapRgn                     = 0xA8FB,
    _MapPoly                    = 0xA8FC
};

#if OLDROUTINENAMES
/* Some names had wrong case in C, long ago*/
enum {
    _OffSetRect                 = _OffsetRect,
    _InSetRect                  = _InsetRect,
    _OffSetPoly                 = _OffsetPoly,
    _OfSetRgn                   = _OffsetRgn,
    _OfsetRgn                   = _OffsetRgn,
    _InSetRgn                   = _InsetRgn
};

#endif  /* OLDROUTINENAMES */


/* Toolbox */
enum {
    _Count1Resources            = 0xA80D,
    _Get1IxResource             = 0xA80E,
    _Get1IxType                 = 0xA80F,
    _Unique1ID                  = 0xA810,
    _TESelView                  = 0xA811,
    _TEPinScroll                = 0xA812,
    _TEAutoView                 = 0xA813,
    _Pack8                      = 0xA816,
    _FixATan2                   = 0xA818,
    _XMunger                    = 0xA819,
    _HOpenResFile               = 0xA81A,
    _HCreateResFile             = 0xA81B,
    _Count1Types                = 0xA81C,
    _Get1Resource               = 0xA81F,
    _Get1NamedResource          = 0xA820,
    _ResourceDispatch           = 0xA822,
    _MaxSizeRsrc                = 0xA821,
    _InsMenuItem                = 0xA826,
    _InsertMenuItem             = 0xA826,
    _HideDItem                  = 0xA827,
    _HideDialogItem             = 0xA827,
    _ShowDItem                  = 0xA828,
    _ShowDialogItem             = 0xA828,
    _Pack9                      = 0xA82B,
    _Pack10                     = 0xA82C,
    _Pack11                     = 0xA82D,
    _Pack12                     = 0xA82E,
    _Pack13                     = 0xA82F,
    _Pack14                     = 0xA830,
    _Pack15                     = 0xA831,
    _ScrnBitMap                 = 0xA833,
    _SetFScaleDisable           = 0xA834,
    _FontMetrics                = 0xA835,
    _ZoomWindow                 = 0xA83A,
    _TrackBox                   = 0xA83B,
    _PrGlue                     = 0xA8FD,
    _InitFonts                  = 0xA8FE,
    _GetFName                   = 0xA8FF,
    _GetFNum                    = 0xA900,
    _FMSwapFont                 = 0xA901,
    _RealFont                   = 0xA902,
    _SetFontLock                = 0xA903,
    _DrawGrowIcon               = 0xA904,
    _DragGrayRgn                = 0xA905,
    _NewString                  = 0xA906,
    _SetString                  = 0xA907,
    _ShowHide                   = 0xA908,
    _CalcVis                    = 0xA909,
    _CalcVBehind                = 0xA90A,
    _ClipAbove                  = 0xA90B,
    _PaintOne                   = 0xA90C,
    _PaintBehind                = 0xA90D,
    _SaveOld                    = 0xA90E,
    _DrawNew                    = 0xA90F,
    _GetWMgrPort                = 0xA910,
    _CheckUpDate                = 0xA911,
    _InitWindows                = 0xA912,
    _NewWindow                  = 0xA913,
    _DisposWindow               = 0xA914,
    _DisposeWindow              = 0xA914,
    _ShowWindow                 = 0xA915,
    _HideWindow                 = 0xA916,
    _GetWRefCon                 = 0xA917,
    _SetWRefCon                 = 0xA918,
    _GetWTitle                  = 0xA919,
    _SetWTitle                  = 0xA91A,
    _MoveWindow                 = 0xA91B,
    _HiliteWindow               = 0xA91C,
    _SizeWindow                 = 0xA91D,
    _TrackGoAway                = 0xA91E,
    _SelectWindow               = 0xA91F,
    _BringToFront               = 0xA920,
    _SendBehind                 = 0xA921,
    _BeginUpDate                = 0xA922,
    _EndUpDate                  = 0xA923,
    _FrontWindow                = 0xA924,
    _DragWindow                 = 0xA925,
    _DragTheRgn                 = 0xA926,
    _InvalRgn                   = 0xA927,
    _InvalRect                  = 0xA928,
    _ValidRgn                   = 0xA929,
    _ValidRect                  = 0xA92A,
    _GrowWindow                 = 0xA92B,
    _FindWindow                 = 0xA92C,
    _CloseWindow                = 0xA92D,
    _SetWindowPic               = 0xA92E,
    _GetWindowPic               = 0xA92F
};

enum {
    _InitMenus                  = 0xA930,
    _NewMenu                    = 0xA931,
    _DisposMenu                 = 0xA932,
    _DisposeMenu                = 0xA932,
    _AppendMenu                 = 0xA933,
    _ClearMenuBar               = 0xA934,
    _InsertMenu                 = 0xA935,
    _DeleteMenu                 = 0xA936,
    _DrawMenuBar                = 0xA937,
    _InvalMenuBar               = 0xA81D,
    _HiliteMenu                 = 0xA938,
    _EnableItem                 = 0xA939,
    _DisableItem                = 0xA93A,
    _GetMenuBar                 = 0xA93B,
    _SetMenuBar                 = 0xA93C,
    _MenuSelect                 = 0xA93D,
    _MenuKey                    = 0xA93E,
    _GetItmIcon                 = 0xA93F,
    _SetItmIcon                 = 0xA940,
    _GetItmStyle                = 0xA941,
    _SetItmStyle                = 0xA942,
    _GetItmMark                 = 0xA943,
    _SetItmMark                 = 0xA944,
    _CheckItem                  = 0xA945,
    _GetItem                    = 0xA946,
    _GetMenuItemText            = 0xA946,
    _SetItem                    = 0xA947,
    _SetMenuItemText            = 0xA947,
    _CalcMenuSize               = 0xA948,
    _GetMHandle                 = 0xA949,
    _GetMenuHandle              = 0xA949,
    _SetMFlash                  = 0xA94A,
    _PlotIcon                   = 0xA94B,
    _FlashMenuBar               = 0xA94C,
    _AddResMenu                 = 0xA94D,
    _AppendResMenu              = 0xA94D,
    _PinRect                    = 0xA94E,
    _DeltaPoint                 = 0xA94F,
    _CountMItems                = 0xA950,
    _InsertResMenu              = 0xA951,
    _DelMenuItem                = 0xA952,
    _DeleteMenuItem             = 0xA952,
    _UpdtControl                = 0xA953,
    _NewControl                 = 0xA954,
    _DisposControl              = 0xA955,
    _DisposeControl             = 0xA955,
    _KillControls               = 0xA956,
    _ShowControl                = 0xA957,
    _HideControl                = 0xA958,
    _MoveControl                = 0xA959,
    _GetCRefCon                 = 0xA95A,
    _GetControlReference        = 0xA95A,
    _SetCRefCon                 = 0xA95B,
    _SetControlReference        = 0xA95B,
    _SizeControl                = 0xA95C,
    _HiliteControl              = 0xA95D,
    _GetCTitle                  = 0xA95E,
    _GetControlTitle            = 0xA95E,
    _SetCTitle                  = 0xA95F,
    _SetControlTitle            = 0xA95F,
    _GetCtlValue                = 0xA960,
    _GetControlValue            = 0xA960,
    _GetMinCtl                  = 0xA961,
    _GetControlMinimum          = 0xA961,
    _GetMaxCtl                  = 0xA962,
    _GetControlMaximum          = 0xA962,
    _SetCtlValue                = 0xA963,
    _SetControlValue            = 0xA963,
    _SetMinCtl                  = 0xA964,
    _SetControlMinimum          = 0xA964,
    _SetMaxCtl                  = 0xA965,
    _SetControlMaximum          = 0xA965,
    _TestControl                = 0xA966,
    _DragControl                = 0xA967,
    _TrackControl               = 0xA968,
    _DrawControls               = 0xA969,
    _GetCtlAction               = 0xA96A,
    _GetControlAction           = 0xA96A,
    _SetCtlAction               = 0xA96B,
    _SetControlAction           = 0xA96B,
    _FindControl                = 0xA96C,
    _Draw1Control               = 0xA96D
};

enum {
    _Dequeue                    = 0xA96E,
    _Enqueue                    = 0xA96F,
    _WaitNextEvent              = 0xA860,
    _GetNextEvent               = 0xA970,
    _EventAvail                 = 0xA971,
    _GetMouse                   = 0xA972,
    _StillDown                  = 0xA973,
    _Button                     = 0xA974,
    _TickCount                  = 0xA975,
    _GetKeys                    = 0xA976,
    _WaitMouseUp                = 0xA977,
    _UpdtDialog                 = 0xA978,
    _InitDialogs                = 0xA97B,
    _GetNewDialog               = 0xA97C,
    _NewDialog                  = 0xA97D,
    _SelIText                   = 0xA97E,
    _SelectDialogItemText       = 0xA97E,
    _IsDialogEvent              = 0xA97F,
    _DialogSelect               = 0xA980,
    _DrawDialog                 = 0xA981,
    _CloseDialog                = 0xA982,
    _DisposDialog               = 0xA983,
    _DisposeDialog              = 0xA983,
    _FindDItem                  = 0xA984,
    _FindDialogItem             = 0xA984,
    _Alert                      = 0xA985,
    _StopAlert                  = 0xA986,
    _NoteAlert                  = 0xA987,
    _CautionAlert               = 0xA988,
    _ParamText                  = 0xA98B,
    _ErrorSound                 = 0xA98C,
    _GetDItem                   = 0xA98D,
    _GetDialogItem              = 0xA98D,
    _SetDItem                   = 0xA98E,
    _SetDialogItem              = 0xA98E,
    _SetIText                   = 0xA98F,
    _SetDialogItemText          = 0xA98F,
    _GetIText                   = 0xA990,
    _GetDialogItemText          = 0xA990,
    _ModalDialog                = 0xA991,
    _DetachResource             = 0xA992,
    _SetResPurge                = 0xA993,
    _CurResFile                 = 0xA994,
    _InitResources              = 0xA995,
    _RsrcZoneInit               = 0xA996,
    _OpenResFile                = 0xA997,
    _UseResFile                 = 0xA998,
    _UpdateResFile              = 0xA999,
    _CloseResFile               = 0xA99A,
    _SetResLoad                 = 0xA99B,
    _CountResources             = 0xA99C,
    _GetIndResource             = 0xA99D,
    _CountTypes                 = 0xA99E,
    _GetIndType                 = 0xA99F,
    _GetResource                = 0xA9A0,
    _GetNamedResource           = 0xA9A1,
    _LoadResource               = 0xA9A2,
    _ReleaseResource            = 0xA9A3,
    _HomeResFile                = 0xA9A4,
    _SizeRsrc                   = 0xA9A5,
    _GetResAttrs                = 0xA9A6,
    _SetResAttrs                = 0xA9A7,
    _GetResInfo                 = 0xA9A8,
    _SetResInfo                 = 0xA9A9,
    _ChangedResource            = 0xA9AA,
    _AddResource                = 0xA9AB,
    _AddReference               = 0xA9AC,
    _RmveResource               = 0xA9AD,
    _RmveReference              = 0xA9AE,
    _ResError                   = 0xA9AF,
    _WriteResource              = 0xA9B0,
    _CreateResFile              = 0xA9B1,
    _SystemEvent                = 0xA9B2,
    _SystemClick                = 0xA9B3,
    _SystemTask                 = 0xA9B4,
    _SystemMenu                 = 0xA9B5,
    _OpenDeskAcc                = 0xA9B6,
    _CloseDeskAcc               = 0xA9B7,
    _GetPattern                 = 0xA9B8,
    _GetCursor                  = 0xA9B9,
    _GetString                  = 0xA9BA,
    _GetIcon                    = 0xA9BB,
    _GetPicture                 = 0xA9BC,
    _GetNewWindow               = 0xA9BD,
    _GetNewControl              = 0xA9BE,
    _GetRMenu                   = 0xA9BF,
    _GetNewMBar                 = 0xA9C0,
    _UniqueID                   = 0xA9C1,
    _SysEdit                    = 0xA9C2,
    _OpenRFPerm                 = 0xA9C4,
    _RsrcMapEntry               = 0xA9C5,
    _Secs2Date                  = 0xA9C6,
    _SecondsToDate              = 0xA9C6,
    _Date2Secs                  = 0xA9C7,
    _DateToSeconds              = 0xA9C7,
    _SysBeep                    = 0xA9C8,
    _SysError                   = 0xA9C9,
    _PutIcon                    = 0xA9CA,
    _Munger                     = 0xA9E0,
    _HandToHand                 = 0xA9E1,
    _PtrToXHand                 = 0xA9E2,
    _PtrToHand                  = 0xA9E3,
    _HandAndHand                = 0xA9E4,
    _InitPack                   = 0xA9E5,
    _InitAllPacks               = 0xA9E6,
    _Pack0                      = 0xA9E7,
    _Pack1                      = 0xA9E8,
    _Pack2                      = 0xA9E9,
    _Pack3                      = 0xA9EA,
    _FP68K                      = 0xA9EB,
    _Pack4                      = 0xA9EB,
    _Elems68K                   = 0xA9EC,
    _Pack5                      = 0xA9EC,
    _Pack6                      = 0xA9ED,
    _DECSTR68K                  = 0xA9EE,
    _Pack7                      = 0xA9EE,
    _PtrAndHand                 = 0xA9EF,
    _LoadSeg                    = 0xA9F0,
    _UnLoadSeg                  = 0xA9F1,
    _Launch                     = 0xA9F2,
    _Chain                      = 0xA9F3,
    _ExitToShell                = 0xA9F4,
    _GetAppParms                = 0xA9F5,
    _GetResFileAttrs            = 0xA9F6,
    _SetResFileAttrs            = 0xA9F7,
    _MethodDispatch             = 0xA9F8,
    _InfoScrap                  = 0xA9F9,
    _UnlodeScrap                = 0xA9FA,
    _UnloadScrap                = 0xA9FA,
    _LodeScrap                  = 0xA9FB,
    _LoadScrap                  = 0xA9FB,
    _ZeroScrap                  = 0xA9FC,
    _GetScrap                   = 0xA9FD,
    _PutScrap                   = 0xA9FE,
    _Debugger                   = 0xA9FF,
    _DisplayDispatch            = 0xABEB,
    _IconDispatch               = 0xABC9,
    _ThreadDispatch             = 0xABF2,
    _DictionaryDispatch         = 0xAA53,
    _DebugStr                   = 0xABFF
};


enum {
                                                                /* PPCToolbox */
    _PPC                        = 0xA0DD,                       /* Alias Manager */
    _AliasDispatch              = 0xA823,                       /* Device Manager (some shared by the File Manager) */
    _Open                       = 0xA000,
    _Close                      = 0xA001,
    _Read                       = 0xA002,
    _Write                      = 0xA003,
    _Control                    = 0xA004,
    _Status                     = 0xA005,
    _KillIO                     = 0xA006,                       /* File Manager */
    _GetVolInfo                 = 0xA007,
    _Create                     = 0xA008,
    _Delete                     = 0xA009,
    _OpenRF                     = 0xA00A,
    _Rename                     = 0xA00B,
    _GetFileInfo                = 0xA00C,
    _SetFileInfo                = 0xA00D,
    _UnmountVol                 = 0xA00E,
    _HUnmountVol                = 0xA20E,
    _MountVol                   = 0xA00F,
    _Allocate                   = 0xA010,
    _GetEOF                     = 0xA011,
    _SetEOF                     = 0xA012,
    _FlushVol                   = 0xA013,
    _GetVol                     = 0xA014,
    _SetVol                     = 0xA015,
    _FInitQueue                 = 0xA016,
    _Eject                      = 0xA017,
    _GetFPos                    = 0xA018,
    _SetFilLock                 = 0xA041,
    _RstFilLock                 = 0xA042,
    _SetFilType                 = 0xA043,
    _SetFPos                    = 0xA044,
    _FlushFile                  = 0xA045,
    _HOpen                      = 0xA200,
    _HGetVInfo                  = 0xA207,
    _HCreate                    = 0xA208,
    _HDelete                    = 0xA209,
    _HOpenRF                    = 0xA20A,
    _HRename                    = 0xA20B,
    _HGetFileInfo               = 0xA20C,
    _HSetFileInfo               = 0xA20D,
    _AllocContig                = 0xA210,
    _HSetVol                    = 0xA215,
    _HGetVol                    = 0xA214,
    _HSetFLock                  = 0xA241,
    _HRstFLock                  = 0xA242,                       /* dispatch trap for remaining File Manager (and Desktop Manager) calls*/
    _FSDispatch                 = 0xA060,
    _HFSDispatch                = 0xA260,                       /* High level FSSpec calls */
    _HighLevelFSDispatch        = 0xAA52
};

enum {
                                                                /* Memory Manager */
    _InitZone                   = 0xA019,
    _GetZone                    = 0xA11A,
    _SetZone                    = 0xA01B,
    _FreeMem                    = 0xA01C,
    _MaxMem                     = 0xA11D,
    _NewPtr                     = 0xA11E,
    _NewPtrSys                  = 0xA51E,
    _NewPtrClear                = 0xA31E,
    _NewPtrSysClear             = 0xA71E,
    _DisposPtr                  = 0xA01F,
    _DisposePtr                 = 0xA01F,
    _SetPtrSize                 = 0xA020,
    _GetPtrSize                 = 0xA021,
    _NewHandle                  = 0xA122,
    _NewHandleSys               = 0xA522,
    _NewHandleClear             = 0xA322,
    _NewHandleSysClear          = 0xA722,
    _DisposHandle               = 0xA023,
    _DisposeHandle              = 0xA023,
    _SetHandleSize              = 0xA024,
    _GetHandleSize              = 0xA025,
    _HandleZone                 = 0xA126,
    _ReallocHandle              = 0xA027,
    _RecoverHandle              = 0xA128,
    _HLock                      = 0xA029,
    _HUnlock                    = 0xA02A,
    _EmptyHandle                = 0xA02B,
    _InitApplZone               = 0xA02C,
    _SetApplLimit               = 0xA02D,
    _BlockMove                  = 0xA02E,
    _BlockMoveData              = 0xA22E,
    _MemoryDispatch             = 0xA05C,
    _MemoryDispatchA0Result     = 0xA15C,
    _DeferUserFn                = 0xA08F,
    _DebugUtil                  = 0xA08D,
    _HeapDispatch               = 0xA0A4
};

enum {
                                                                /* Event Manager */
    _PostEvent                  = 0xA02F,
    _PPostEvent                 = 0xA12F,
    _OSEventAvail               = 0xA030,
    _GetOSEvent                 = 0xA031,
    _FlushEvents                = 0xA032,
    _VInstall                   = 0xA033,
    _VRemove                    = 0xA034,
    _OffLine                    = 0xA035,
    _MoreMasters                = 0xA036,
    _WriteParam                 = 0xA038,
    _ReadDateTime               = 0xA039,
    _SetDateTime                = 0xA03A,
    _Delay                      = 0xA03B,
    _CmpString                  = 0xA03C,
    _DrvrInstall                = 0xA03D,
    _DrvrRemove                 = 0xA03E,
    _InitUtil                   = 0xA03F,
    _ResrvMem                   = 0xA040,
    _GetTrapAddress             = 0xA146,
    _SetTrapAddress             = 0xA047,
    _GetOSTrapAddress           = 0xA346,
    _SetOSTrapAddress           = 0xA247,
    _GetToolTrapAddress         = 0xA746,
    _SetToolTrapAddress         = 0xA647,
    _GetToolBoxTrapAddress      = 0xA746,
    _SetToolBoxTrapAddress      = 0xA647,
    _PtrZone                    = 0xA148,
    _HPurge                     = 0xA049,
    _HNoPurge                   = 0xA04A,
    _SetGrowZone                = 0xA04B,
    _CompactMem                 = 0xA04C,
    _PurgeMem                   = 0xA04D,
    _AddDrive                   = 0xA04E,
    _RDrvrInstall               = 0xA04F,
    _LwrString                  = 0xA056,
    _UprString                  = 0xA054,
    _SetApplBase                = 0xA057,
    _HWPriv                     = 0xA198
};

enum {
    _LowerText                  = 0xA056,
    _StripText                  = 0xA256,
    _UpperText                  = 0xA456,
    _StripUpperText             = 0xA656,
    _OSDispatch                 = 0xA88F,
    _RelString                  = 0xA050,
    _CompareString              = 0xA050,
    _ReadXPRam                  = 0xA051,
    _WriteXPRam                 = 0xA052,
    _InsTime                    = 0xA058,
    _InsXTime                   = 0xA458,
    _RmvTime                    = 0xA059,
    _PrimeTime                  = 0xA05A,
    _Microseconds               = 0xA193,
    _PowerOff                   = 0xA05B,
    _MaxBlock                   = 0xA061,
    _PurgeSpace                 = 0xA162,
    _PurgeSpaceSys              = 0xA562,
    _MaxApplZone                = 0xA063,
    _MoveHHi                    = 0xA064,
    _StackSpace                 = 0xA065,
    _NewEmptyHandle             = 0xA166,
    _HSetRBit                   = 0xA067,
    _HClrRBit                   = 0xA068,
    _HGetState                  = 0xA069,
    _HSetState                  = 0xA06A,
    _InitFS                     = 0xA06C,
    _InitEvents                 = 0xA06D,
    _StripAddress               = 0xA055,
    _Translate24To32            = 0xA091,
    _SetAppBase                 = 0xA057,
    _SwapMMUMode                = 0xA05D,
    _SlotVInstall               = 0xA06F,
    _SlotVRemove                = 0xA070,
    _AttachVBL                  = 0xA071,
    _DoVBLTask                  = 0xA072,
    _SIntInstall                = 0xA075,
    _SIntRemove                 = 0xA076,
    _CountADBs                  = 0xA077,
    _GetIndADB                  = 0xA078,
    _GetADBInfo                 = 0xA079,
    _SetADBInfo                 = 0xA07A,
    _ADBReInit                  = 0xA07B,
    _ADBOp                      = 0xA07C,
    _VADBProc                   = 0xA0AE,
    _GetDefaultStartup          = 0xA07D,
    _SetDefaultStartup          = 0xA07E,
    _InternalWait               = 0xA07F,
    _RGetResource               = 0xA80C,
    _GetVideoDefault            = 0xA080,
    _SetVideoDefault            = 0xA081,
    _DTInstall                  = 0xA082,
    _SetOSDefault               = 0xA083,
    _GetOSDefault               = 0xA084,
    _IOPInfoAccess              = 0xA086,
    _IOPMsgRequest              = 0xA087,
    _IOPMoveData                = 0xA088
};

enum {
                                                                /* Power Manager */
    _PowerDispatch              = 0xA09F,
    _PMgrOp                     = 0xA085,
    _IdleUpdate                 = 0xA285,
    _IdleState                  = 0xA485,
    _SerialPower                = 0xA685,
    _Sleep                      = 0xA08A,
    _SleepQInstall              = 0xA28A,
    _SlpQInstall                = 0xA28A,
    _SleepQRemove               = 0xA48A,
    _SlpQRemove                 = 0xA48A,
    _CommToolboxDispatch        = 0xA08B,
    _SysEnvirons                = 0xA090,
    _Gestalt                    = 0xA1AD,
    _NewGestalt                 = 0xA3AD,
    _ReplaceGestalt             = 0xA5AD,
    _GetGestaltProcPtr          = 0xA7AD,
    _InitProcMenu               = 0xA808,
    _GetItemCmd                 = 0xA84E,
    _SetItemCmd                 = 0xA84F,
    _PopUpMenuSelect            = 0xA80B,
    _KeyTrans                   = 0xA9C3,
    _KeyTranslate               = 0xA9C3
};

enum {
                                                                /* TextEdit */
    _TEGetText                  = 0xA9CB,
    _TEInit                     = 0xA9CC,
    _TEDispose                  = 0xA9CD,
    _TextBox                    = 0xA9CE,
    _TETextBox                  = 0xA9CE,
    _TESetText                  = 0xA9CF,
    _TECalText                  = 0xA9D0,
    _TESetSelect                = 0xA9D1,
    _TENew                      = 0xA9D2,
    _TEUpdate                   = 0xA9D3,
    _TEClick                    = 0xA9D4,
    _TECopy                     = 0xA9D5,
    _TECut                      = 0xA9D6,
    _TEDelete                   = 0xA9D7,
    _TEActivate                 = 0xA9D8,
    _TEDeactivate               = 0xA9D9,
    _TEIdle                     = 0xA9DA,
    _TEPaste                    = 0xA9DB,
    _TEKey                      = 0xA9DC,
    _TEScroll                   = 0xA9DD,
    _TEInsert                   = 0xA9DE,
    _TESetJust                  = 0xA9DF,
    _TESetAlignment             = 0xA9DF,
    _TEGetOffset                = 0xA83C,
    _TEDispatch                 = 0xA83D,
    _TEStyleNew                 = 0xA83E,
    _TEFindWord                 = 0xA0FE,
    _TEFindLine                 = 0xA0FF
};

enum {
                                                                /* Color Quickdraw */
    _OpenCPort                  = 0xAA00,
    _InitCPort                  = 0xAA01,
    _CloseCPort                 = 0xAA02,
    _NewPixMap                  = 0xAA03,
    _DisposPixMap               = 0xAA04,
    _DisposePixMap              = 0xAA04,
    _CopyPixMap                 = 0xAA05,
    _SetPortPix                 = 0xAA06,
    _NewPixPat                  = 0xAA07,
    _DisposPixPat               = 0xAA08,
    _DisposePixPat              = 0xAA08,
    _CopyPixPat                 = 0xAA09,
    _PenPixPat                  = 0xAA0A,
    _BackPixPat                 = 0xAA0B,
    _GetPixPat                  = 0xAA0C,
    _MakeRGBPat                 = 0xAA0D,
    _FillCRect                  = 0xAA0E,
    _FillCOval                  = 0xAA0F,
    _FillCRoundRect             = 0xAA10,
    _FillCArc                   = 0xAA11,
    _FillCRgn                   = 0xAA12,
    _FillCPoly                  = 0xAA13,
    _RGBForeColor               = 0xAA14,
    _RGBBackColor               = 0xAA15,
    _SetCPixel                  = 0xAA16,
    _GetCPixel                  = 0xAA17,
    _GetCTable                  = 0xAA18,
    _GetForeColor               = 0xAA19,
    _GetBackColor               = 0xAA1A,
    _GetCCursor                 = 0xAA1B,
    _SetCCursor                 = 0xAA1C,
    _AllocCursor                = 0xAA1D,
    _GetCIcon                   = 0xAA1E,
    _PlotCIcon                  = 0xAA1F,
    _OpenCPicture               = 0xAA20,
    _OpColor                    = 0xAA21,
    _HiliteColor                = 0xAA22,
    _CharExtra                  = 0xAA23,
    _DisposCTable               = 0xAA24,
    _DisposeCTable              = 0xAA24,
    _DisposCIcon                = 0xAA25,
    _DisposeCIcon               = 0xAA25,
    _DisposCCursor              = 0xAA26,
    _DisposeCCursor             = 0xAA26,
    _SeedCFill                  = 0xAA50,
    _CalcCMask                  = 0xAA4F,
    _CopyDeepMask               = 0xAA51
};

enum {
                                                                /* Routines for video devices */
    _GetMaxDevice               = 0xAA27,
    _GetCTSeed                  = 0xAA28,
    _GetDeviceList              = 0xAA29,
    _GetMainDevice              = 0xAA2A,
    _GetNextDevice              = 0xAA2B,
    _TestDeviceAttribute        = 0xAA2C,
    _SetDeviceAttribute         = 0xAA2D,
    _InitGDevice                = 0xAA2E,
    _NewGDevice                 = 0xAA2F,
    _DisposGDevice              = 0xAA30,
    _DisposeGDevice             = 0xAA30,
    _SetGDevice                 = 0xAA31,
    _GetGDevice                 = 0xAA32,
    _DeviceLoop                 = 0xABCA,                       /* Color Manager */
    _Color2Index                = 0xAA33,
    _Index2Color                = 0xAA34,
    _InvertColor                = 0xAA35,
    _RealColor                  = 0xAA36,
    _GetSubTable                = 0xAA37,
    _UpdatePixMap               = 0xAA38
};

enum {
                                                                /* Dialog Manager */
    _NewCDialog                 = 0xAA4B,
    _NewColorDialog             = 0xAA4B,
    _MakeITable                 = 0xAA39,
    _AddSearch                  = 0xAA3A,
    _AddComp                    = 0xAA3B,
    _SetClientID                = 0xAA3C,
    _ProtectEntry               = 0xAA3D,
    _ReserveEntry               = 0xAA3E,
    _SetEntries                 = 0xAA3F,
    _QDError                    = 0xAA40,
    _SaveEntries                = 0xAA49,
    _RestoreEntries             = 0xAA4A,
    _DelSearch                  = 0xAA4C,
    _DelComp                    = 0xAA4D,
    _SetStdCProcs               = 0xAA4E,
    _StdOpcodeProc              = 0xABF8,                       /* Added to Toolbox for color */
    _SetWinColor                = 0xAA41,
    _GetAuxWin                  = 0xAA42,
    _SetCtlColor                = 0xAA43,
    _SetControlColor            = 0xAA43,
    _GetAuxCtl                  = 0xAA44,
    _GetAuxiliaryControlRecord  = 0xAA44,
    _NewCWindow                 = 0xAA45,
    _GetNewCWindow              = 0xAA46,
    _SetDeskCPat                = 0xAA47,
    _GetCWMgrPort               = 0xAA48,
    _GetCVariant                = 0xA809,
    _GetControlVariant          = 0xA809,
    _GetWVariant                = 0xA80A,                       /* Added to Menu Manager for color */
    _DelMCEntries               = 0xAA60,
    _DeleteMCEntries            = 0xAA60,
    _GetMCInfo                  = 0xAA61,
    _SetMCInfo                  = 0xAA62,
    _DispMCInfo                 = 0xAA63,
    _DisposeMCInfo              = 0xAA63,
    _GetMCEntry                 = 0xAA64,
    _SetMCEntries               = 0xAA65,
    _MenuChoice                 = 0xAA66,                       /* Dialog Manager */
    _DialogDispatch             = 0xAA68,                       /* Font Manager */
    _SetFractEnable             = 0xA814,
    _FontDispatch               = 0xA854
};

enum {
                                                                /* Palette Manager */
    _InitPalettes               = 0xAA90,
    _NewPalette                 = 0xAA91,
    _GetNewPalette              = 0xAA92,
    _DisposePalette             = 0xAA93,
    _ActivatePalette            = 0xAA94,
    _SetPalette                 = 0xAA95,
    _NSetPalette                = 0xAA95,
    _GetPalette                 = 0xAA96,
    _PmForeColor                = 0xAA97,
    _PmBackColor                = 0xAA98,
    _AnimateEntry               = 0xAA99,
    _AnimatePalette             = 0xAA9A,
    _GetEntryColor              = 0xAA9B,
    _SetEntryColor              = 0xAA9C,
    _GetEntryUsage              = 0xAA9D,
    _SetEntryUsage              = 0xAA9E,
    _CTab2Palette               = 0xAA9F,
    _Palette2CTab               = 0xAAA0,
    _CopyPalette                = 0xAAA1,
    _PaletteDispatch            = 0xAAA2
};

enum {
                                                                /* Sound Manager */
    _EgretDispatch              = 0xA092,
    _SoundDispatch              = 0xA800,
    _SndDisposeChannel          = 0xA801,
    _SndAddModifier             = 0xA802,
    _SndDoCommand               = 0xA803,
    _SndDoImmediate             = 0xA804,
    _SndPlay                    = 0xA805,
    _SndControl                 = 0xA806,
    _SndNewChannel              = 0xA807
};

enum {
    _SlotManager                = 0xA06E,
    _ScriptUtil                 = 0xA8B5,
    _SCSIAtomic                 = 0xA089,
    _SCSIDispatch               = 0xA815,
    _Long2Fix                   = 0xA83F,
    _Fix2Long                   = 0xA840,
    _Fix2Frac                   = 0xA841,
    _Frac2Fix                   = 0xA842,
    _Fix2X                      = 0xA843,
    _X2Fix                      = 0xA844,
    _Frac2X                     = 0xA845,
    _X2Frac                     = 0xA846,
    _FracCos                    = 0xA847,
    _FracSin                    = 0xA848,
    _FracSqrt                   = 0xA849,
    _FracMul                    = 0xA84A,
    _FracDiv                    = 0xA84B,
    _FixDiv                     = 0xA84D,
    _NMInstall                  = 0xA05E,
    _NMRemove                   = 0xA05F,
    _QDExtensions               = 0xAB1D,
    _ComponentDispatch          = 0xA82A
};

enum {
    _DockingDispatch            = 0xAA57,
    _MixedModeDispatch          = 0xAA59,                       /* PowerPC Mac */
    _CodeFragmentDispatch       = 0xAA5A,                       /* PowerPC Mac */
    _TranslationDispatch        = 0xABFC,                       /* Translation Manager */
    _TextServicesDispatch       = 0xAA54,                       /* Text Services Manager */
    _CursorDeviceDispatch       = 0xAADB,                       /* Cursor Device Manager */
                                                                /* Miscellaneous Traps */
    _InitDogCow                 = 0xA89F,
    _EnableDogCow               = 0xA89F,
    _DisableDogCow              = 0xA89F,
    _Moof                       = 0xA89F,
    _HFSPinaforeDispatch        = 0xAA52
};

enum {
    _OCEUtils                   = 0xAA5C,                       /* PowerTalk (AOCE)*/
    _DigitalSignature           = 0xAA5D,                       /* PowerTalk (AOCE)*/
    _TBDispatch                 = 0xAA5E                        /* PowerTalk (AOCE)*/
};

enum {
    _CollectionMgr              = 0xABF6                        /* Collection Manager*/
};

enum {
    _ControlStripDispatch       = 0xAAF2,                       /* see TechNote OS 06*/
    _PowerMgrDispatch           = 0xA09E
};

enum {
    _ALMDispatch                = 0xAAA4                        /* Apple Location Manger*/
};

enum {
    _ServerDispatch             = 0xA094                        /* AppleShare, Macintosh File Sharing */
};

enum {
    _FSMDispatch                = 0xA0AC,                       /* File System Manager*/
    _HFSUtilDispatch            = 0xA824                        /* HFS Utility routines in the File System Manager*/
};

enum {
    _ControlDispatch            = 0xAA73,
    _AppearanceDispatch         = 0xAA74                        /*  Appearance Trap */
};


enum {
    _AVLTreeDispatch            = 0xAA80
};






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

#endif /* __TRAPS__ */

