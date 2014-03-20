/*
     File:       AERegistry.h
 
     Contains:   AppleEvents Registry Interface.
 
     Version:    Technology: Mac OS 8.5
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1993-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __AEREGISTRY__
#define __AEREGISTRY__

#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __MACERRORS__
#include <MacErrors.h>
#endif

#ifndef __APPLEEVENTS__
#include <AppleEvents.h>
#endif

#ifndef __QUICKDRAW__
#include <Quickdraw.h>
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

enum {
    cAEList                     = FOUR_CHAR_CODE('list'),       /*  0x6c697374  */
    cApplication                = FOUR_CHAR_CODE('capp'),       /*  0x63617070  */
    cArc                        = FOUR_CHAR_CODE('carc'),       /*  0x63617263  */
    cBoolean                    = FOUR_CHAR_CODE('bool'),       /*  0x626f6f6c  */
    cCell                       = FOUR_CHAR_CODE('ccel'),       /*  0x6363656c  */
    cChar                       = FOUR_CHAR_CODE('cha '),       /*  0x63686120  */
    cColorTable                 = FOUR_CHAR_CODE('clrt'),       /*  0x636c7274  */
    cColumn                     = FOUR_CHAR_CODE('ccol'),       /*  0x63636f6c  */
    cDocument                   = FOUR_CHAR_CODE('docu'),       /*  0x646f6375  */
    cDrawingArea                = FOUR_CHAR_CODE('cdrw'),       /*  0x63647277  */
    cEnumeration                = FOUR_CHAR_CODE('enum'),       /*  0x656e756d  */
    cFile                       = FOUR_CHAR_CODE('file'),       /*  0x66696c65  */
    cFixed                      = FOUR_CHAR_CODE('fixd'),       /*  0x66697864  */
    cFixedPoint                 = FOUR_CHAR_CODE('fpnt'),       /*  0x66706e74  */
    cFixedRectangle             = FOUR_CHAR_CODE('frct'),       /*  0x66726374  */
    cGraphicLine                = FOUR_CHAR_CODE('glin'),       /*  0x676c696e  */
    cGraphicObject              = FOUR_CHAR_CODE('cgob'),       /*  0x63676f62  */
    cGraphicShape               = FOUR_CHAR_CODE('cgsh'),       /*  0x63677368  */
    cGraphicText                = FOUR_CHAR_CODE('cgtx'),       /*  0x63677478  */
    cGroupedGraphic             = FOUR_CHAR_CODE('cpic')        /*  0x63706963  */
};

enum {
    cInsertionLoc               = FOUR_CHAR_CODE('insl'),       /*  0x696e736c  */
    cInsertionPoint             = FOUR_CHAR_CODE('cins'),       /*  0x63696e73  */
    cIntlText                   = FOUR_CHAR_CODE('itxt'),       /*  0x69747874  */
    cIntlWritingCode            = FOUR_CHAR_CODE('intl'),       /*  0x696e746c  */
    cItem                       = FOUR_CHAR_CODE('citm'),       /*  0x6369746d  */
    cLine                       = FOUR_CHAR_CODE('clin'),       /*  0x636c696e  */
    cLongDateTime               = FOUR_CHAR_CODE('ldt '),       /*  0x6c647420  */
    cLongFixed                  = FOUR_CHAR_CODE('lfxd'),       /*  0x6c667864  */
    cLongFixedPoint             = FOUR_CHAR_CODE('lfpt'),       /*  0x6c667074  */
    cLongFixedRectangle         = FOUR_CHAR_CODE('lfrc'),       /*  0x6c667263  */
    cLongInteger                = FOUR_CHAR_CODE('long'),       /*  0x6c6f6e67  */
    cLongPoint                  = FOUR_CHAR_CODE('lpnt'),       /*  0x6c706e74  */
    cLongRectangle              = FOUR_CHAR_CODE('lrct'),       /*  0x6c726374  */
    cMachineLoc                 = FOUR_CHAR_CODE('mLoc'),       /*  0x6d4c6f63  */
    cMenu                       = FOUR_CHAR_CODE('cmnu'),       /*  0x636d6e75  */
    cMenuItem                   = FOUR_CHAR_CODE('cmen'),       /*  0x636d656e  */
    cObject                     = FOUR_CHAR_CODE('cobj'),       /*  0x636f626a  */
    cObjectSpecifier            = FOUR_CHAR_CODE('obj '),       /*  0x6f626a20  */
    cOpenableObject             = FOUR_CHAR_CODE('coob'),       /*  0x636f6f62  */
    cOval                       = FOUR_CHAR_CODE('covl')        /*  0x636f766c  */
};

enum {
    cParagraph                  = FOUR_CHAR_CODE('cpar'),       /*  0x63706172  */
    cPICT                       = FOUR_CHAR_CODE('PICT'),       /*  0x50494354  */
    cPixel                      = FOUR_CHAR_CODE('cpxl'),       /*  0x6370786c  */
    cPixelMap                   = FOUR_CHAR_CODE('cpix'),       /*  0x63706978  */
    cPolygon                    = FOUR_CHAR_CODE('cpgn'),       /*  0x6370676e  */
    cProperty                   = FOUR_CHAR_CODE('prop'),       /*  0x70726f70  */
    cQDPoint                    = FOUR_CHAR_CODE('QDpt'),       /*  0x51447074  */
    cQDRectangle                = FOUR_CHAR_CODE('qdrt'),       /*  0x71647274  */
    cRectangle                  = FOUR_CHAR_CODE('crec'),       /*  0x63726563  */
    cRGBColor                   = FOUR_CHAR_CODE('cRGB'),       /*  0x63524742  */
    cRotation                   = FOUR_CHAR_CODE('trot'),       /*  0x74726f74  */
    cRoundedRectangle           = FOUR_CHAR_CODE('crrc'),       /*  0x63727263  */
    cRow                        = FOUR_CHAR_CODE('crow'),       /*  0x63726f77  */
    cSelection                  = FOUR_CHAR_CODE('csel'),       /*  0x6373656c  */
    cShortInteger               = FOUR_CHAR_CODE('shor'),       /*  0x73686f72  */
    cTable                      = FOUR_CHAR_CODE('ctbl'),       /*  0x6374626c  */
    cText                       = FOUR_CHAR_CODE('ctxt'),       /*  0x63747874  */
    cTextFlow                   = FOUR_CHAR_CODE('cflo'),       /*  0x63666c6f  */
    cTextStyles                 = FOUR_CHAR_CODE('tsty'),       /*  0x74737479  */
    cType                       = FOUR_CHAR_CODE('type')        /*  0x74797065  */
};

enum {
    cVersion                    = FOUR_CHAR_CODE('vers'),       /*  0x76657273  */
    cWindow                     = FOUR_CHAR_CODE('cwin'),       /*  0x6377696e  */
    cWord                       = FOUR_CHAR_CODE('cwor'),       /*  0x63776f72  */
    enumArrows                  = FOUR_CHAR_CODE('arro'),       /*  0x6172726f  */
    enumJustification           = FOUR_CHAR_CODE('just'),       /*  0x6a757374  */
    enumKeyForm                 = FOUR_CHAR_CODE('kfrm'),       /*  0x6b66726d  */
    enumPosition                = FOUR_CHAR_CODE('posi'),       /*  0x706f7369  */
    enumProtection              = FOUR_CHAR_CODE('prtn'),       /*  0x7072746e  */
    enumQuality                 = FOUR_CHAR_CODE('qual'),       /*  0x7175616c  */
    enumSaveOptions             = FOUR_CHAR_CODE('savo'),       /*  0x7361766f  */
    enumStyle                   = FOUR_CHAR_CODE('styl'),       /*  0x7374796c  */
    enumTransferMode            = FOUR_CHAR_CODE('tran'),       /*  0x7472616e  */
    formUniqueID                = FOUR_CHAR_CODE('ID  '),       /*  0x49442020  */
    kAEAbout                    = FOUR_CHAR_CODE('abou'),       /*  0x61626f75  */
    kAEAfter                    = FOUR_CHAR_CODE('afte'),       /*  0x61667465  */
    kAEAliasSelection           = FOUR_CHAR_CODE('sali'),       /*  0x73616c69  */
    kAEAllCaps                  = FOUR_CHAR_CODE('alcp'),       /*  0x616c6370  */
    kAEArrowAtEnd               = FOUR_CHAR_CODE('aren'),       /*  0x6172656e  */
    kAEArrowAtStart             = FOUR_CHAR_CODE('arst'),       /*  0x61727374  */
    kAEArrowBothEnds            = FOUR_CHAR_CODE('arbo')        /*  0x6172626f  */
};

enum {
    kAEAsk                      = FOUR_CHAR_CODE('ask '),       /*  0x61736b20  */
    kAEBefore                   = FOUR_CHAR_CODE('befo'),       /*  0x6265666f  */
    kAEBeginning                = FOUR_CHAR_CODE('bgng'),       /*  0x62676e67  */
    kAEBeginsWith               = FOUR_CHAR_CODE('bgwt'),       /*  0x62677774  */
    kAEBeginTransaction         = FOUR_CHAR_CODE('begi'),       /*  0x62656769  */
    kAEBold                     = FOUR_CHAR_CODE('bold'),       /*  0x626f6c64  */
    kAECaseSensEquals           = FOUR_CHAR_CODE('cseq'),       /*  0x63736571  */
    kAECentered                 = FOUR_CHAR_CODE('cent'),       /*  0x63656e74  */
    kAEChangeView               = FOUR_CHAR_CODE('view'),       /*  0x76696577  */
    kAEClone                    = FOUR_CHAR_CODE('clon'),       /*  0x636c6f6e  */
    kAEClose                    = FOUR_CHAR_CODE('clos'),       /*  0x636c6f73  */
    kAECondensed                = FOUR_CHAR_CODE('cond'),       /*  0x636f6e64  */
    kAEContains                 = FOUR_CHAR_CODE('cont'),       /*  0x636f6e74  */
    kAECopy                     = FOUR_CHAR_CODE('copy'),       /*  0x636f7079  */
    kAECoreSuite                = FOUR_CHAR_CODE('core'),       /*  0x636f7265  */
    kAECountElements            = FOUR_CHAR_CODE('cnte'),       /*  0x636e7465  */
    kAECreateElement            = FOUR_CHAR_CODE('crel'),       /*  0x6372656c  */
    kAECreatePublisher          = FOUR_CHAR_CODE('cpub'),       /*  0x63707562  */
    kAECut                      = FOUR_CHAR_CODE('cut '),       /*  0x63757420  */
    kAEDelete                   = FOUR_CHAR_CODE('delo')        /*  0x64656c6f  */
};

enum {
    kAEDoObjectsExist           = FOUR_CHAR_CODE('doex'),       /*  0x646f6578  */
    kAEDoScript                 = FOUR_CHAR_CODE('dosc'),       /*  0x646f7363  */
    kAEDrag                     = FOUR_CHAR_CODE('drag'),       /*  0x64726167  */
    kAEDuplicateSelection       = FOUR_CHAR_CODE('sdup'),       /*  0x73647570  */
    kAEEditGraphic              = FOUR_CHAR_CODE('edit'),       /*  0x65646974  */
    kAEEmptyTrash               = FOUR_CHAR_CODE('empt'),       /*  0x656d7074  */
    kAEEnd                      = FOUR_CHAR_CODE('end '),       /*  0x656e6420  */
    kAEEndsWith                 = FOUR_CHAR_CODE('ends'),       /*  0x656e6473  */
    kAEEndTransaction           = FOUR_CHAR_CODE('endt'),       /*  0x656e6474  */
    kAEEquals                   = FOUR_CHAR_CODE('=   '),       /*  0x3d202020  */
    kAEExpanded                 = FOUR_CHAR_CODE('pexp'),       /*  0x70657870  */
    kAEFast                     = FOUR_CHAR_CODE('fast'),       /*  0x66617374  */
    kAEFinderEvents             = FOUR_CHAR_CODE('FNDR'),       /*  0x464e4452  */
    kAEFormulaProtect           = FOUR_CHAR_CODE('fpro'),       /*  0x6670726f  */
    kAEFullyJustified           = FOUR_CHAR_CODE('full'),       /*  0x66756c6c  */
    kAEGetClassInfo             = FOUR_CHAR_CODE('qobj'),       /*  0x716f626a  */
    kAEGetData                  = FOUR_CHAR_CODE('getd'),       /*  0x67657464  */
    kAEGetDataSize              = FOUR_CHAR_CODE('dsiz'),       /*  0x6473697a  */
    kAEGetEventInfo             = FOUR_CHAR_CODE('gtei'),       /*  0x67746569  */
    kAEGetInfoSelection         = FOUR_CHAR_CODE('sinf')        /*  0x73696e66  */
};

enum {
    kAEGetPrivilegeSelection    = FOUR_CHAR_CODE('sprv'),       /*  0x73707276  */
    kAEGetSuiteInfo             = FOUR_CHAR_CODE('gtsi'),       /*  0x67747369  */
    kAEGreaterThan              = FOUR_CHAR_CODE('>   '),       /*  0x3e202020  */
    kAEGreaterThanEquals        = FOUR_CHAR_CODE('>=  '),       /*  0x3e3d2020  */
    kAEGrow                     = FOUR_CHAR_CODE('grow'),       /*  0x67726f77  */
    kAEHidden                   = FOUR_CHAR_CODE('hidn'),       /*  0x6869646e  */
    kAEHiQuality                = FOUR_CHAR_CODE('hiqu'),       /*  0x68697175  */
    kAEImageGraphic             = FOUR_CHAR_CODE('imgr'),       /*  0x696d6772  */
    kAEIsUniform                = FOUR_CHAR_CODE('isun'),       /*  0x6973756e  */
    kAEItalic                   = FOUR_CHAR_CODE('ital'),       /*  0x6974616c  */
    kAELeftJustified            = FOUR_CHAR_CODE('left'),       /*  0x6c656674  */
    kAELessThan                 = FOUR_CHAR_CODE('<   '),       /*  0x3c202020  */
    kAELessThanEquals           = FOUR_CHAR_CODE('<=  '),       /*  0x3c3d2020  */
    kAELowercase                = FOUR_CHAR_CODE('lowc'),       /*  0x6c6f7763  */
    kAEMakeObjectsVisible       = FOUR_CHAR_CODE('mvis'),       /*  0x6d766973  */
    kAEMiscStandards            = FOUR_CHAR_CODE('misc'),       /*  0x6d697363  */
    kAEModifiable               = FOUR_CHAR_CODE('modf'),       /*  0x6d6f6466  */
    kAEMove                     = FOUR_CHAR_CODE('move'),       /*  0x6d6f7665  */
    kAENo                       = FOUR_CHAR_CODE('no  '),       /*  0x6e6f2020  */
    kAENoArrow                  = FOUR_CHAR_CODE('arno')        /*  0x61726e6f  */
};

enum {
    kAENonmodifiable            = FOUR_CHAR_CODE('nmod'),       /*  0x6e6d6f64  */
    kAEOpen                     = FOUR_CHAR_CODE('odoc'),       /*  0x6f646f63  */
    kAEOpenSelection            = FOUR_CHAR_CODE('sope'),       /*  0x736f7065  */
    kAEOutline                  = FOUR_CHAR_CODE('outl'),       /*  0x6f75746c  */
    kAEPageSetup                = FOUR_CHAR_CODE('pgsu'),       /*  0x70677375  */
    kAEPaste                    = FOUR_CHAR_CODE('past'),       /*  0x70617374  */
    kAEPlain                    = FOUR_CHAR_CODE('plan'),       /*  0x706c616e  */
    kAEPrint                    = FOUR_CHAR_CODE('pdoc'),       /*  0x70646f63  */
    kAEPrintSelection           = FOUR_CHAR_CODE('spri'),       /*  0x73707269  */
    kAEPrintWindow              = FOUR_CHAR_CODE('pwin'),       /*  0x7077696e  */
    kAEPutAwaySelection         = FOUR_CHAR_CODE('sput'),       /*  0x73707574  */
    kAEQDAddOver                = FOUR_CHAR_CODE('addo'),       /*  0x6164646f  */
    kAEQDAddPin                 = FOUR_CHAR_CODE('addp'),       /*  0x61646470  */
    kAEQDAdMax                  = FOUR_CHAR_CODE('admx'),       /*  0x61646d78  */
    kAEQDAdMin                  = FOUR_CHAR_CODE('admn'),       /*  0x61646d6e  */
    kAEQDBic                    = FOUR_CHAR_CODE('bic '),       /*  0x62696320  */
    kAEQDBlend                  = FOUR_CHAR_CODE('blnd'),       /*  0x626c6e64  */
    kAEQDCopy                   = FOUR_CHAR_CODE('cpy '),       /*  0x63707920  */
    kAEQDNotBic                 = FOUR_CHAR_CODE('nbic'),       /*  0x6e626963  */
    kAEQDNotCopy                = FOUR_CHAR_CODE('ncpy')        /*  0x6e637079  */
};

enum {
    kAEQDNotOr                  = FOUR_CHAR_CODE('ntor'),       /*  0x6e746f72  */
    kAEQDNotXor                 = FOUR_CHAR_CODE('nxor'),       /*  0x6e786f72  */
    kAEQDOr                     = FOUR_CHAR_CODE('or  '),       /*  0x6f722020  */
    kAEQDSubOver                = FOUR_CHAR_CODE('subo'),       /*  0x7375626f  */
    kAEQDSubPin                 = FOUR_CHAR_CODE('subp'),       /*  0x73756270  */
    kAEQDSupplementalSuite      = FOUR_CHAR_CODE('qdsp'),       /*  0x71647370  */
    kAEQDXor                    = FOUR_CHAR_CODE('xor '),       /*  0x786f7220  */
    kAEQuickdrawSuite           = FOUR_CHAR_CODE('qdrw'),       /*  0x71647277  */
    kAEQuitAll                  = FOUR_CHAR_CODE('quia'),       /*  0x71756961  */
    kAERedo                     = FOUR_CHAR_CODE('redo'),       /*  0x7265646f  */
    kAERegular                  = FOUR_CHAR_CODE('regl'),       /*  0x7265676c  */
    kAEReopenApplication        = FOUR_CHAR_CODE('rapp'),       /*  0x72617070  */
    kAEReplace                  = FOUR_CHAR_CODE('rplc'),       /*  0x72706c63  */
    kAERequiredSuite            = FOUR_CHAR_CODE('reqd'),       /*  0x72657164  */
    kAERestart                  = FOUR_CHAR_CODE('rest'),       /*  0x72657374  */
    kAERevealSelection          = FOUR_CHAR_CODE('srev'),       /*  0x73726576  */
    kAERevert                   = FOUR_CHAR_CODE('rvrt'),       /*  0x72767274  */
    kAERightJustified           = FOUR_CHAR_CODE('rght'),       /*  0x72676874  */
    kAESave                     = FOUR_CHAR_CODE('save'),       /*  0x73617665  */
    kAESelect                   = FOUR_CHAR_CODE('slct'),       /*  0x736c6374  */
    kAESetData                  = FOUR_CHAR_CODE('setd')        /*  0x73657464  */
};

enum {
    kAESetPosition              = FOUR_CHAR_CODE('posn'),       /*  0x706f736e  */
    kAEShadow                   = FOUR_CHAR_CODE('shad'),       /*  0x73686164  */
    kAEShowClipboard            = FOUR_CHAR_CODE('shcl'),       /*  0x7368636c  */
    kAEShutDown                 = FOUR_CHAR_CODE('shut'),       /*  0x73687574  */
    kAESleep                    = FOUR_CHAR_CODE('slep'),       /*  0x736c6570  */
    kAESmallCaps                = FOUR_CHAR_CODE('smcp'),       /*  0x736d6370  */
    kAESpecialClassProperties   = FOUR_CHAR_CODE('c@#!'),       /*  0x63402321  */
    kAEStrikethrough            = FOUR_CHAR_CODE('strk'),       /*  0x7374726b  */
    kAESubscript                = FOUR_CHAR_CODE('sbsc'),       /*  0x73627363  */
    kAESuperscript              = FOUR_CHAR_CODE('spsc'),       /*  0x73707363  */
    kAETableSuite               = FOUR_CHAR_CODE('tbls'),       /*  0x74626c73  */
    kAETextSuite                = FOUR_CHAR_CODE('TEXT'),       /*  0x54455854  */
    kAETransactionTerminated    = FOUR_CHAR_CODE('ttrm'),       /*  0x7474726d  */
    kAEUnderline                = FOUR_CHAR_CODE('undl'),       /*  0x756e646c  */
    kAEUndo                     = FOUR_CHAR_CODE('undo'),       /*  0x756e646f  */
    kAEWholeWordEquals          = FOUR_CHAR_CODE('wweq'),       /*  0x77776571  */
    kAEYes                      = FOUR_CHAR_CODE('yes '),       /*  0x79657320  */
    kAEZoom                     = FOUR_CHAR_CODE('zoom')        /*  0x7a6f6f6d  */
};

/* EventRecord Classes and EventIDs */
enum {
    kAEMouseClass               = FOUR_CHAR_CODE('mous'),
    kAEDown                     = FOUR_CHAR_CODE('down'),
    kAEUp                       = FOUR_CHAR_CODE('up  '),
    kAEMoved                    = FOUR_CHAR_CODE('move'),
    kAEStoppedMoving            = FOUR_CHAR_CODE('stop'),
    kAEWindowClass              = FOUR_CHAR_CODE('wind'),
    kAEUpdate                   = FOUR_CHAR_CODE('updt'),
    kAEActivate                 = FOUR_CHAR_CODE('actv'),
    kAEDeactivate               = FOUR_CHAR_CODE('dact'),
    kAECommandClass             = FOUR_CHAR_CODE('cmnd'),       /* Modern Command Event Class */
    kAEKeyClass                 = FOUR_CHAR_CODE('keyc'),
    kAERawKey                   = FOUR_CHAR_CODE('rkey'),       /* Modern Raw Key Event */
    kAEVirtualKey               = FOUR_CHAR_CODE('keyc'),       /* Modern Virtual Key Event */
    kAENavigationKey            = FOUR_CHAR_CODE('nave'),       /* Modern Navigation Key Event */
    kAEAutoDown                 = FOUR_CHAR_CODE('auto'),
    kAEApplicationClass         = FOUR_CHAR_CODE('appl'),
    kAESuspend                  = FOUR_CHAR_CODE('susp'),
    kAEResume                   = FOUR_CHAR_CODE('rsme'),
    kAEDiskEvent                = FOUR_CHAR_CODE('disk'),
    kAENullEvent                = FOUR_CHAR_CODE('null'),
    kAEWakeUpEvent              = FOUR_CHAR_CODE('wake'),
    kAEScrapEvent               = FOUR_CHAR_CODE('scrp'),
    kAEHighLevel                = FOUR_CHAR_CODE('high')
};

enum {
    keyAEAngle                  = FOUR_CHAR_CODE('kang'),       /*  0x6b616e67  */
    keyAEArcAngle               = FOUR_CHAR_CODE('parc')        /*  0x70617263  */
};

enum {
    keyAEBaseAddr               = FOUR_CHAR_CODE('badd'),       /*  0x62616464  */
    keyAEBestType               = FOUR_CHAR_CODE('pbst'),       /*  0x70627374  */
    keyAEBgndColor              = FOUR_CHAR_CODE('kbcl'),       /*  0x6b62636c  */
    keyAEBgndPattern            = FOUR_CHAR_CODE('kbpt'),       /*  0x6b627074  */
    keyAEBounds                 = FOUR_CHAR_CODE('pbnd'),       /*  0x70626e64  */
    keyAECellList               = FOUR_CHAR_CODE('kclt'),       /*  0x6b636c74  */
    keyAEClassID                = FOUR_CHAR_CODE('clID'),       /*  0x636c4944  */
    keyAEColor                  = FOUR_CHAR_CODE('colr'),       /*  0x636f6c72  */
    keyAEColorTable             = FOUR_CHAR_CODE('cltb'),       /*  0x636c7462  */
    keyAECurveHeight            = FOUR_CHAR_CODE('kchd'),       /*  0x6b636864  */
    keyAECurveWidth             = FOUR_CHAR_CODE('kcwd'),       /*  0x6b637764  */
    keyAEDashStyle              = FOUR_CHAR_CODE('pdst'),       /*  0x70647374  */
    keyAEData                   = FOUR_CHAR_CODE('data'),       /*  0x64617461  */
    keyAEDefaultType            = FOUR_CHAR_CODE('deft'),       /*  0x64656674  */
    keyAEDefinitionRect         = FOUR_CHAR_CODE('pdrt'),       /*  0x70647274  */
    keyAEDescType               = FOUR_CHAR_CODE('dstp'),       /*  0x64737470  */
    keyAEDestination            = FOUR_CHAR_CODE('dest'),       /*  0x64657374  */
    keyAEDoAntiAlias            = FOUR_CHAR_CODE('anta'),       /*  0x616e7461  */
    keyAEDoDithered             = FOUR_CHAR_CODE('gdit'),       /*  0x67646974  */
    keyAEDoRotate               = FOUR_CHAR_CODE('kdrt')        /*  0x6b647274  */
};

enum {
    keyAEDoScale                = FOUR_CHAR_CODE('ksca'),       /*  0x6b736361  */
    keyAEDoTranslate            = FOUR_CHAR_CODE('ktra'),       /*  0x6b747261  */
    keyAEEditionFileLoc         = FOUR_CHAR_CODE('eloc'),       /*  0x656c6f63  */
    keyAEElements               = FOUR_CHAR_CODE('elms'),       /*  0x656c6d73  */
    keyAEEndPoint               = FOUR_CHAR_CODE('pend'),       /*  0x70656e64  */
    keyAEEventClass             = FOUR_CHAR_CODE('evcl'),       /*  0x6576636c  */
    keyAEEventID                = FOUR_CHAR_CODE('evti'),       /*  0x65767469  */
    keyAEFile                   = FOUR_CHAR_CODE('kfil'),       /*  0x6b66696c  */
    keyAEFileType               = FOUR_CHAR_CODE('fltp'),       /*  0x666c7470  */
    keyAEFillColor              = FOUR_CHAR_CODE('flcl'),       /*  0x666c636c  */
    keyAEFillPattern            = FOUR_CHAR_CODE('flpt'),       /*  0x666c7074  */
    keyAEFlipHorizontal         = FOUR_CHAR_CODE('kfho'),       /*  0x6b66686f  */
    keyAEFlipVertical           = FOUR_CHAR_CODE('kfvt'),       /*  0x6b667674  */
    keyAEFont                   = FOUR_CHAR_CODE('font'),       /*  0x666f6e74  */
    keyAEFormula                = FOUR_CHAR_CODE('pfor'),       /*  0x70666f72  */
    keyAEGraphicObjects         = FOUR_CHAR_CODE('gobs'),       /*  0x676f6273  */
    keyAEID                     = FOUR_CHAR_CODE('ID  '),       /*  0x49442020  */
    keyAEImageQuality           = FOUR_CHAR_CODE('gqua'),       /*  0x67717561  */
    keyAEInsertHere             = FOUR_CHAR_CODE('insh'),       /*  0x696e7368  */
    keyAEKeyForms               = FOUR_CHAR_CODE('keyf')        /*  0x6b657966  */
};

enum {
    keyAEKeyword                = FOUR_CHAR_CODE('kywd'),       /*  0x6b797764  */
    keyAELevel                  = FOUR_CHAR_CODE('levl'),       /*  0x6c65766c  */
    keyAELineArrow              = FOUR_CHAR_CODE('arro'),       /*  0x6172726f  */
    keyAEName                   = FOUR_CHAR_CODE('pnam'),       /*  0x706e616d  */
    keyAENewElementLoc          = FOUR_CHAR_CODE('pnel'),       /*  0x706e656c  */
    keyAEObject                 = FOUR_CHAR_CODE('kobj'),       /*  0x6b6f626a  */
    keyAEObjectClass            = FOUR_CHAR_CODE('kocl'),       /*  0x6b6f636c  */
    keyAEOffStyles              = FOUR_CHAR_CODE('ofst'),       /*  0x6f667374  */
    keyAEOnStyles               = FOUR_CHAR_CODE('onst'),       /*  0x6f6e7374  */
    keyAEParameters             = FOUR_CHAR_CODE('prms'),       /*  0x70726d73  */
    keyAEParamFlags             = FOUR_CHAR_CODE('pmfg'),       /*  0x706d6667  */
    keyAEPenColor               = FOUR_CHAR_CODE('ppcl'),       /*  0x7070636c  */
    keyAEPenPattern             = FOUR_CHAR_CODE('pppa'),       /*  0x70707061  */
    keyAEPenWidth               = FOUR_CHAR_CODE('ppwd'),       /*  0x70707764  */
    keyAEPixelDepth             = FOUR_CHAR_CODE('pdpt'),       /*  0x70647074  */
    keyAEPixMapMinus            = FOUR_CHAR_CODE('kpmm'),       /*  0x6b706d6d  */
    keyAEPMTable                = FOUR_CHAR_CODE('kpmt'),       /*  0x6b706d74  */
    keyAEPointList              = FOUR_CHAR_CODE('ptlt'),       /*  0x70746c74  */
    keyAEPointSize              = FOUR_CHAR_CODE('ptsz'),       /*  0x7074737a  */
    keyAEPosition               = FOUR_CHAR_CODE('kpos')        /*  0x6b706f73  */
};

enum {
    keyAEPropData               = FOUR_CHAR_CODE('prdt'),       /*  0x70726474  */
    keyAEProperties             = FOUR_CHAR_CODE('qpro'),       /*  0x7170726f  */
    keyAEProperty               = FOUR_CHAR_CODE('kprp'),       /*  0x6b707270  */
    keyAEPropFlags              = FOUR_CHAR_CODE('prfg'),       /*  0x70726667  */
    keyAEPropID                 = FOUR_CHAR_CODE('prop'),       /*  0x70726f70  */
    keyAEProtection             = FOUR_CHAR_CODE('ppro'),       /*  0x7070726f  */
    keyAERenderAs               = FOUR_CHAR_CODE('kren'),       /*  0x6b72656e  */
    keyAERequestedType          = FOUR_CHAR_CODE('rtyp'),       /*  0x72747970  */
    keyAEResult                 = FOUR_CHAR_CODE('----'),       /*  0x2d2d2d2d  */
    keyAEResultInfo             = FOUR_CHAR_CODE('rsin'),       /*  0x7273696e  */
    keyAERotation               = FOUR_CHAR_CODE('prot'),       /*  0x70726f74  */
    keyAERotPoint               = FOUR_CHAR_CODE('krtp'),       /*  0x6b727470  */
    keyAERowList                = FOUR_CHAR_CODE('krls'),       /*  0x6b726c73  */
    keyAESaveOptions            = FOUR_CHAR_CODE('savo'),       /*  0x7361766f  */
    keyAEScale                  = FOUR_CHAR_CODE('pscl'),       /*  0x7073636c  */
    keyAEScriptTag              = FOUR_CHAR_CODE('psct'),       /*  0x70736374  */
    keyAEShowWhere              = FOUR_CHAR_CODE('show'),       /*  0x73686f77  */
    keyAEStartAngle             = FOUR_CHAR_CODE('pang'),       /*  0x70616e67  */
    keyAEStartPoint             = FOUR_CHAR_CODE('pstp'),       /*  0x70737470  */
    keyAEStyles                 = FOUR_CHAR_CODE('ksty')        /*  0x6b737479  */
};

enum {
    keyAESuiteID                = FOUR_CHAR_CODE('suit'),       /*  0x73756974  */
    keyAEText                   = FOUR_CHAR_CODE('ktxt'),       /*  0x6b747874  */
    keyAETextColor              = FOUR_CHAR_CODE('ptxc'),       /*  0x70747863  */
    keyAETextFont               = FOUR_CHAR_CODE('ptxf'),       /*  0x70747866  */
    keyAETextPointSize          = FOUR_CHAR_CODE('ptps'),       /*  0x70747073  */
    keyAETextStyles             = FOUR_CHAR_CODE('txst'),       /*  0x74787374  */
    keyAETextLineHeight         = FOUR_CHAR_CODE('ktlh'),       /* type ShortInteger */
    keyAETextLineAscent         = FOUR_CHAR_CODE('ktas'),       /* type ShortInteger */
    keyAETheText                = FOUR_CHAR_CODE('thtx'),       /*  0x74687478  */
    keyAETransferMode           = FOUR_CHAR_CODE('pptm'),       /*  0x7070746d  */
    keyAETranslation            = FOUR_CHAR_CODE('ptrs'),       /*  0x70747273  */
    keyAETryAsStructGraf        = FOUR_CHAR_CODE('toog'),       /*  0x746f6f67  */
    keyAEUniformStyles          = FOUR_CHAR_CODE('ustl'),       /*  0x7573746c  */
    keyAEUpdateOn               = FOUR_CHAR_CODE('pupd'),       /*  0x70757064  */
    keyAEUserTerm               = FOUR_CHAR_CODE('utrm'),       /*  0x7574726d  */
    keyAEWindow                 = FOUR_CHAR_CODE('wndw'),       /*  0x776e6477  */
    keyAEWritingCode            = FOUR_CHAR_CODE('wrcd')        /*  0x77726364  */
};

enum {
    keyMiscellaneous            = FOUR_CHAR_CODE('fmsc'),       /*  0x666d7363  */
    keySelection                = FOUR_CHAR_CODE('fsel'),       /*  0x6673656c  */
    keyWindow                   = FOUR_CHAR_CODE('kwnd'),       /*  0x6b776e64  */
                                                                /* EventRecord keys */
    keyWhen                     = FOUR_CHAR_CODE('when'),
    keyWhere                    = FOUR_CHAR_CODE('wher'),
    keyModifiers                = FOUR_CHAR_CODE('mods'),
    keyKey                      = FOUR_CHAR_CODE('key '),
    keyKeyCode                  = FOUR_CHAR_CODE('code'),
    keyKeyboard                 = FOUR_CHAR_CODE('keyb'),
    keyDriveNumber              = FOUR_CHAR_CODE('drv#'),
    keyErrorCode                = FOUR_CHAR_CODE('err#'),
    keyHighLevelClass           = FOUR_CHAR_CODE('hcls'),
    keyHighLevelID              = FOUR_CHAR_CODE('hid ')
};

enum {
    pArcAngle                   = FOUR_CHAR_CODE('parc'),       /*  0x70617263  */
    pBackgroundColor            = FOUR_CHAR_CODE('pbcl'),       /*  0x7062636c  */
    pBackgroundPattern          = FOUR_CHAR_CODE('pbpt'),       /*  0x70627074  */
    pBestType                   = FOUR_CHAR_CODE('pbst'),       /*  0x70627374  */
    pBounds                     = FOUR_CHAR_CODE('pbnd'),       /*  0x70626e64  */
    pClass                      = FOUR_CHAR_CODE('pcls'),       /*  0x70636c73  */
    pClipboard                  = FOUR_CHAR_CODE('pcli'),       /*  0x70636c69  */
    pColor                      = FOUR_CHAR_CODE('colr'),       /*  0x636f6c72  */
    pColorTable                 = FOUR_CHAR_CODE('cltb'),       /*  0x636c7462  */
    pContents                   = FOUR_CHAR_CODE('pcnt'),       /*  0x70636e74  */
    pCornerCurveHeight          = FOUR_CHAR_CODE('pchd'),       /*  0x70636864  */
    pCornerCurveWidth           = FOUR_CHAR_CODE('pcwd'),       /*  0x70637764  */
    pDashStyle                  = FOUR_CHAR_CODE('pdst'),       /*  0x70647374  */
    pDefaultType                = FOUR_CHAR_CODE('deft'),       /*  0x64656674  */
    pDefinitionRect             = FOUR_CHAR_CODE('pdrt'),       /*  0x70647274  */
    pEnabled                    = FOUR_CHAR_CODE('enbl'),       /*  0x656e626c  */
    pEndPoint                   = FOUR_CHAR_CODE('pend'),       /*  0x70656e64  */
    pFillColor                  = FOUR_CHAR_CODE('flcl'),       /*  0x666c636c  */
    pFillPattern                = FOUR_CHAR_CODE('flpt'),       /*  0x666c7074  */
    pFont                       = FOUR_CHAR_CODE('font')        /*  0x666f6e74  */
};

enum {
    pFormula                    = FOUR_CHAR_CODE('pfor'),       /*  0x70666f72  */
    pGraphicObjects             = FOUR_CHAR_CODE('gobs'),       /*  0x676f6273  */
    pHasCloseBox                = FOUR_CHAR_CODE('hclb'),       /*  0x68636c62  */
    pHasTitleBar                = FOUR_CHAR_CODE('ptit'),       /*  0x70746974  */
    pID                         = FOUR_CHAR_CODE('ID  '),       /*  0x49442020  */
    pIndex                      = FOUR_CHAR_CODE('pidx'),       /*  0x70696478  */
    pInsertionLoc               = FOUR_CHAR_CODE('pins'),       /*  0x70696e73  */
    pIsFloating                 = FOUR_CHAR_CODE('isfl'),       /*  0x6973666c  */
    pIsFrontProcess             = FOUR_CHAR_CODE('pisf'),       /*  0x70697366  */
    pIsModal                    = FOUR_CHAR_CODE('pmod'),       /*  0x706d6f64  */
    pIsModified                 = FOUR_CHAR_CODE('imod'),       /*  0x696d6f64  */
    pIsResizable                = FOUR_CHAR_CODE('prsz'),       /*  0x7072737a  */
    pIsStationeryPad            = FOUR_CHAR_CODE('pspd'),       /*  0x70737064  */
    pIsZoomable                 = FOUR_CHAR_CODE('iszm'),       /*  0x69737a6d  */
    pIsZoomed                   = FOUR_CHAR_CODE('pzum'),       /*  0x707a756d  */
    pItemNumber                 = FOUR_CHAR_CODE('itmn'),       /*  0x69746d6e  */
    pJustification              = FOUR_CHAR_CODE('pjst'),       /*  0x706a7374  */
    pLineArrow                  = FOUR_CHAR_CODE('arro'),       /*  0x6172726f  */
    pMenuID                     = FOUR_CHAR_CODE('mnid'),       /*  0x6d6e6964  */
    pName                       = FOUR_CHAR_CODE('pnam')        /*  0x706e616d  */
};

enum {
    pNewElementLoc              = FOUR_CHAR_CODE('pnel'),       /*  0x706e656c  */
    pPenColor                   = FOUR_CHAR_CODE('ppcl'),       /*  0x7070636c  */
    pPenPattern                 = FOUR_CHAR_CODE('pppa'),       /*  0x70707061  */
    pPenWidth                   = FOUR_CHAR_CODE('ppwd'),       /*  0x70707764  */
    pPixelDepth                 = FOUR_CHAR_CODE('pdpt'),       /*  0x70647074  */
    pPointList                  = FOUR_CHAR_CODE('ptlt'),       /*  0x70746c74  */
    pPointSize                  = FOUR_CHAR_CODE('ptsz'),       /*  0x7074737a  */
    pProtection                 = FOUR_CHAR_CODE('ppro'),       /*  0x7070726f  */
    pRotation                   = FOUR_CHAR_CODE('prot'),       /*  0x70726f74  */
    pScale                      = FOUR_CHAR_CODE('pscl'),       /*  0x7073636c  */
    pScript                     = FOUR_CHAR_CODE('scpt'),       /*  0x73637074  */
    pScriptTag                  = FOUR_CHAR_CODE('psct'),       /*  0x70736374  */
    pSelected                   = FOUR_CHAR_CODE('selc'),       /*  0x73656c63  */
    pSelection                  = FOUR_CHAR_CODE('sele'),       /*  0x73656c65  */
    pStartAngle                 = FOUR_CHAR_CODE('pang'),       /*  0x70616e67  */
    pStartPoint                 = FOUR_CHAR_CODE('pstp'),       /*  0x70737470  */
    pTextColor                  = FOUR_CHAR_CODE('ptxc'),       /*  0x70747863  */
    pTextFont                   = FOUR_CHAR_CODE('ptxf'),       /*  0x70747866  */
    pTextItemDelimiters         = FOUR_CHAR_CODE('txdl'),       /*  0x7478646c  */
    pTextPointSize              = FOUR_CHAR_CODE('ptps')        /*  0x70747073  */
};

enum {
    pTextStyles                 = FOUR_CHAR_CODE('txst'),       /*  0x74787374  */
    pTransferMode               = FOUR_CHAR_CODE('pptm'),       /*  0x7070746d  */
    pTranslation                = FOUR_CHAR_CODE('ptrs'),       /*  0x70747273  */
    pUniformStyles              = FOUR_CHAR_CODE('ustl'),       /*  0x7573746c  */
    pUpdateOn                   = FOUR_CHAR_CODE('pupd'),       /*  0x70757064  */
    pUserSelection              = FOUR_CHAR_CODE('pusl'),       /*  0x7075736c  */
    pVersion                    = FOUR_CHAR_CODE('vers'),       /*  0x76657273  */
    pVisible                    = FOUR_CHAR_CODE('pvis')        /*  0x70766973  */
};

enum {
    typeAEText                  = FOUR_CHAR_CODE('tTXT'),       /*  0x74545854  */
    typeArc                     = FOUR_CHAR_CODE('carc'),       /*  0x63617263  */
    typeBest                    = FOUR_CHAR_CODE('best'),       /*  0x62657374  */
    typeCell                    = FOUR_CHAR_CODE('ccel'),       /*  0x6363656c  */
    typeClassInfo               = FOUR_CHAR_CODE('gcli'),       /*  0x67636c69  */
    typeColorTable              = FOUR_CHAR_CODE('clrt'),       /*  0x636c7274  */
    typeColumn                  = FOUR_CHAR_CODE('ccol'),       /*  0x63636f6c  */
    typeDashStyle               = FOUR_CHAR_CODE('tdas'),       /*  0x74646173  */
    typeData                    = FOUR_CHAR_CODE('tdta'),       /*  0x74647461  */
    typeDrawingArea             = FOUR_CHAR_CODE('cdrw'),       /*  0x63647277  */
    typeElemInfo                = FOUR_CHAR_CODE('elin'),       /*  0x656c696e  */
    typeEnumeration             = FOUR_CHAR_CODE('enum'),       /*  0x656e756d  */
    typeEPS                     = FOUR_CHAR_CODE('EPS '),       /*  0x45505320  */
    typeEventInfo               = FOUR_CHAR_CODE('evin')        /*  0x6576696e  */
};

enum {
    typeFinderWindow            = FOUR_CHAR_CODE('fwin'),       /*  0x6677696e  */
    typeFixedPoint              = FOUR_CHAR_CODE('fpnt'),       /*  0x66706e74  */
    typeFixedRectangle          = FOUR_CHAR_CODE('frct'),       /*  0x66726374  */
    typeGraphicLine             = FOUR_CHAR_CODE('glin'),       /*  0x676c696e  */
    typeGraphicText             = FOUR_CHAR_CODE('cgtx'),       /*  0x63677478  */
    typeGroupedGraphic          = FOUR_CHAR_CODE('cpic'),       /*  0x63706963  */
    typeInsertionLoc            = FOUR_CHAR_CODE('insl'),       /*  0x696e736c  */
    typeIntlText                = FOUR_CHAR_CODE('itxt'),       /*  0x69747874  */
    typeIntlWritingCode         = FOUR_CHAR_CODE('intl'),       /*  0x696e746c  */
    typeLongDateTime            = FOUR_CHAR_CODE('ldt '),       /*  0x6c647420  */
    typeLongFixed               = FOUR_CHAR_CODE('lfxd'),       /*  0x6c667864  */
    typeLongFixedPoint          = FOUR_CHAR_CODE('lfpt'),       /*  0x6c667074  */
    typeLongFixedRectangle      = FOUR_CHAR_CODE('lfrc'),       /*  0x6c667263  */
    typeLongPoint               = FOUR_CHAR_CODE('lpnt'),       /*  0x6c706e74  */
    typeLongRectangle           = FOUR_CHAR_CODE('lrct'),       /*  0x6c726374  */
    typeMachineLoc              = FOUR_CHAR_CODE('mLoc'),       /*  0x6d4c6f63  */
    typeOval                    = FOUR_CHAR_CODE('covl'),       /*  0x636f766c  */
    typeParamInfo               = FOUR_CHAR_CODE('pmin'),       /*  0x706d696e  */
    typePict                    = FOUR_CHAR_CODE('PICT')        /*  0x50494354  */
};

enum {
    typePixelMap                = FOUR_CHAR_CODE('cpix'),       /*  0x63706978  */
    typePixMapMinus             = FOUR_CHAR_CODE('tpmm'),       /*  0x74706d6d  */
    typePolygon                 = FOUR_CHAR_CODE('cpgn'),       /*  0x6370676e  */
    typePropInfo                = FOUR_CHAR_CODE('pinf'),       /*  0x70696e66  */
    typeQDPoint                 = FOUR_CHAR_CODE('QDpt'),       /*  0x51447074  */
    typeQDRegion                = FOUR_CHAR_CODE('Qrgn'),       /*  0x51447074  (data is actual region data, including rectangle and size, _not_ region handle or ptr)*/
    typeRectangle               = FOUR_CHAR_CODE('crec'),       /*  0x63726563  */
    typeRGB16                   = FOUR_CHAR_CODE('tr16'),       /*  0x74723136  */
    typeRGB96                   = FOUR_CHAR_CODE('tr96'),       /*  0x74723936  */
    typeRGBColor                = FOUR_CHAR_CODE('cRGB'),       /*  0x63524742  */
    typeRotation                = FOUR_CHAR_CODE('trot'),       /*  0x74726f74  */
    typeRoundedRectangle        = FOUR_CHAR_CODE('crrc'),       /*  0x63727263  */
    typeRow                     = FOUR_CHAR_CODE('crow'),       /*  0x63726f77  */
    typeScrapStyles             = FOUR_CHAR_CODE('styl'),       /*  0x7374796c  */
    typeScript                  = FOUR_CHAR_CODE('scpt'),       /*  0x73637074  */
    typeStyledText              = FOUR_CHAR_CODE('STXT'),       /*  0x53545854  */
    typeSuiteInfo               = FOUR_CHAR_CODE('suin'),       /*  0x7375696e  */
    typeTable                   = FOUR_CHAR_CODE('ctbl'),       /*  0x6374626c  */
    typeTextStyles              = FOUR_CHAR_CODE('tsty')        /*  0x74737479  */
};

enum {
    typeTIFF                    = FOUR_CHAR_CODE('TIFF'),       /*  0x54494646  */
    typeVersion                 = FOUR_CHAR_CODE('vers')        /*  0x76657273  */
};

enum {
    kAEMenuClass                = FOUR_CHAR_CODE('menu'),
    kAEMenuSelect               = FOUR_CHAR_CODE('mhit'),
    kAEMouseDown                = FOUR_CHAR_CODE('mdwn'),
    kAEMouseDownInBack          = FOUR_CHAR_CODE('mdbk'),
    kAEKeyDown                  = FOUR_CHAR_CODE('kdwn'),
    kAEResized                  = FOUR_CHAR_CODE('rsiz'),
    kAEPromise                  = FOUR_CHAR_CODE('prom')
};

enum {
    keyMenuID                   = FOUR_CHAR_CODE('mid '),
    keyMenuItem                 = FOUR_CHAR_CODE('mitm'),
    keyCloseAllWindows          = FOUR_CHAR_CODE('caw '),
    keyOriginalBounds           = FOUR_CHAR_CODE('obnd'),
    keyNewBounds                = FOUR_CHAR_CODE('nbnd'),
    keyLocalWhere               = FOUR_CHAR_CODE('lwhr')
};

enum {
    typeHIMenu                  = FOUR_CHAR_CODE('mobj'),
    typeHIWindow                = FOUR_CHAR_CODE('wobj')
};

enum {
    kBySmallIcon                = 0,
    kByIconView                 = 1,
    kByNameView                 = 2,
    kByDateView                 = 3,
    kBySizeView                 = 4,
    kByKindView                 = 5,
    kByCommentView              = 6,
    kByLabelView                = 7,
    kByVersionView              = 8
};

enum {
    kAEInfo                     = 11,
    kAEMain                     = 0,
    kAESharing                  = 13
};

enum {
    kAEZoomIn                   = 7,
    kAEZoomOut                  = 8
};

enum {
    kTextServiceClass           = FOUR_CHAR_CODE('tsvc'),
    kUpdateActiveInputArea      = FOUR_CHAR_CODE('updt'),       /* update the active input area */
    kShowHideInputWindow        = FOUR_CHAR_CODE('shiw'),       /* show or hide the input window */
    kPos2Offset                 = FOUR_CHAR_CODE('p2st'),       /* converting global coordinates to char position */
    kOffset2Pos                 = FOUR_CHAR_CODE('st2p'),       /* converting char position to global coordinates */
    kUnicodeNotFromInputMethod  = FOUR_CHAR_CODE('unim'),       /* Unicode text when event not handled by Input Method or no Input Method */
    kGetSelectedText            = FOUR_CHAR_CODE('gtxt'),       /* Get text for current selection */
    keyAETSMDocumentRefcon      = FOUR_CHAR_CODE('refc'),       /* TSM document refcon, typeLongInteger */
    keyAEServerInstance         = FOUR_CHAR_CODE('srvi'),       /* component instance */
    keyAETheData                = FOUR_CHAR_CODE('kdat'),       /* typeText */
    keyAEFixLength              = FOUR_CHAR_CODE('fixl'),       /* fix len ?? */
    keyAEUpdateRange            = FOUR_CHAR_CODE('udng'),       /* typeTextRangeArray */
    keyAECurrentPoint           = FOUR_CHAR_CODE('cpos'),       /* current point */
    keyAEBufferSize             = FOUR_CHAR_CODE('buff'),       /* buffer size to get the text */
    keyAEMoveView               = FOUR_CHAR_CODE('mvvw'),       /* move view flag */
    keyAENextBody               = FOUR_CHAR_CODE('nxbd'),       /* next or previous body */
    keyAETSMScriptTag           = FOUR_CHAR_CODE('sclg'),
    keyAETSMTextFont            = FOUR_CHAR_CODE('ktxf'),
    keyAETSMTextPointSize       = FOUR_CHAR_CODE('ktps'),
    keyAETSMEventRecord         = FOUR_CHAR_CODE('tevt'),       /* Low level Event Record, typeLowLevelEventRecord */
    keyAETextServiceEncoding    = FOUR_CHAR_CODE('tsen'),       /* Text Service encoding, mac or Unicode in UpdateActiveInputArea or GetSelectedText events. */
    keyAETextServiceMacEncoding = FOUR_CHAR_CODE('tmen'),       /* Target mac encoding for TSM conversion of text from Unicode text service. */
    typeTextRange               = FOUR_CHAR_CODE('txrn'),       /* TextRange */
    typeComponentInstance       = FOUR_CHAR_CODE('cmpi'),       /* server instance */
    typeOffsetArray             = FOUR_CHAR_CODE('ofay'),       /* offset array */
    typeTextRangeArray          = FOUR_CHAR_CODE('tray'),
    typeLowLevelEventRecord     = FOUR_CHAR_CODE('evtr'),       /* Low Level Event Record */
    typeText                    = typeChar                      /* Plain text */
};


/* Desc type constants */
enum {
    kTSMOutsideOfBody           = 1,
    kTSMInsideOfBody            = 2,
    kTSMInsideOfActiveInputArea = 3
};

enum {
    kNextBody                   = 1,
    kPreviousBody               = 2
};


struct TextRange {
    long                            fStart;
    long                            fEnd;
    short                           fHiliteStyle;
};
typedef struct TextRange                TextRange;
typedef TextRange *                     TextRangePtr;
typedef TextRangePtr *                  TextRangeHandle;

struct TextRangeArray {
    short                           fNumOfRanges;               /* specify the size of the fRange array */
    TextRange                       fRange[1];                  /* when fNumOfRanges > 1, the size of this array has to be calculated */
};
typedef struct TextRangeArray           TextRangeArray;
typedef TextRangeArray *                TextRangeArrayPtr;
typedef TextRangeArrayPtr *             TextRangeArrayHandle;

struct OffsetArray {
    short                           fNumOfOffsets;              /* specify the size of the fOffset array */
    long                            fOffset[1];                 /* when fNumOfOffsets > 1, the size of this array has to be calculated */
};
typedef struct OffsetArray              OffsetArray;
typedef OffsetArray *                   OffsetArrayPtr;
typedef OffsetArrayPtr *                OffsetArrayHandle;

struct WritingCode {
    ScriptCode                      theScriptCode;
    LangCode                        theLangCode;
};
typedef struct WritingCode              WritingCode;

struct IntlText {
    ScriptCode                      theScriptCode;
    LangCode                        theLangCode;
    char                            theText[1];                 /* variable length data */
};
typedef struct IntlText                 IntlText;

/* Hilite styles */
enum {
    kCaretPosition              = 1,                            /* specify caret position */
    kRawText                    = 2,                            /* specify range of raw text */
    kSelectedRawText            = 3,                            /* specify range of selected raw text */
    kConvertedText              = 4,                            /* specify range of converted text */
    kSelectedConvertedText      = 5,                            /* specify range of selected converted text */
    kBlockFillText              = 6,                            /* Block Fill hilite style */
    kOutlineText                = 7,                            /* Outline hilite style */
    kSelectedText               = 8                             /* Selected hilite style */
};

enum {
    keyAEHiliteRange            = FOUR_CHAR_CODE('hrng'),       /* typeTextRangeArray for System 7, typeHiliteRangeArray for System 8 */
    keyAEPinRange               = FOUR_CHAR_CODE('pnrg'),       /* typeTextRange for System 7, typeTextRegionRange for System 8   */
    keyAEClauseOffsets          = FOUR_CHAR_CODE('clau'),       /* typeOffsetArray for System 7, typeClauseOffsetArray for System 8 */
    keyAEOffset                 = FOUR_CHAR_CODE('ofst'),       /* typeLongInteger for System 7, typeByteOffset for System 8  */
    keyAEPoint                  = FOUR_CHAR_CODE('gpos'),       /* typePoint for System 7, typeQDPoint for System 8 */
    keyAELeftSide               = FOUR_CHAR_CODE('klef'),       /* typeBoolean */
    keyAERegionClass            = FOUR_CHAR_CODE('rgnc'),       /* typeShortInteger for System 7, typeRegionClass for System 8 */
    keyAEDragging               = FOUR_CHAR_CODE('bool')        /* typeBoolean */
};


#if OLDROUTINENAMES
enum {
    keyAELeadingEdge            = keyAELeftSide
};

#endif  /* OLDROUTINENAMES */

enum {
                                                                /* AppleScript 1.3: New Text types */
    typeUnicodeText             = FOUR_CHAR_CODE('utxt'),
    typeStyledUnicodeText       = FOUR_CHAR_CODE('sutx'),
    typeEncodedString           = FOUR_CHAR_CODE('encs'),
    typeCString                 = FOUR_CHAR_CODE('cstr'),
    typePString                 = FOUR_CHAR_CODE('pstr')
};

enum {
                                                                /* AppleScript 1.3: Unit types */
    typeMeters                  = FOUR_CHAR_CODE('metr'),       /* Base Unit */
    typeInches                  = FOUR_CHAR_CODE('inch'),
    typeFeet                    = FOUR_CHAR_CODE('feet'),
    typeYards                   = FOUR_CHAR_CODE('yard'),
    typeMiles                   = FOUR_CHAR_CODE('mile'),
    typeKilometers              = FOUR_CHAR_CODE('kmtr'),
    typeCentimeters             = FOUR_CHAR_CODE('cmtr'),
    typeSquareMeters            = FOUR_CHAR_CODE('sqrm'),       /* Base Unit */
    typeSquareFeet              = FOUR_CHAR_CODE('sqft'),
    typeSquareYards             = FOUR_CHAR_CODE('sqyd'),
    typeSquareMiles             = FOUR_CHAR_CODE('sqmi'),
    typeSquareKilometers        = FOUR_CHAR_CODE('sqkm'),
    typeLiters                  = FOUR_CHAR_CODE('litr'),       /* Base Unit */
    typeQuarts                  = FOUR_CHAR_CODE('qrts'),
    typeGallons                 = FOUR_CHAR_CODE('galn'),
    typeCubicMeters             = FOUR_CHAR_CODE('cmet'),       /* Base Unit */
    typeCubicFeet               = FOUR_CHAR_CODE('cfet'),
    typeCubicInches             = FOUR_CHAR_CODE('cuin'),
    typeCubicCentimeter         = FOUR_CHAR_CODE('ccmt'),
    typeCubicYards              = FOUR_CHAR_CODE('cyrd'),
    typeKilograms               = FOUR_CHAR_CODE('kgrm'),       /* Base Unit */
    typeGrams                   = FOUR_CHAR_CODE('gram'),
    typeOunces                  = FOUR_CHAR_CODE('ozs '),
    typePounds                  = FOUR_CHAR_CODE('lbs '),
    typeDegreesC                = FOUR_CHAR_CODE('degc'),       /* Base Unit */
    typeDegreesF                = FOUR_CHAR_CODE('degf'),
    typeDegreesK                = FOUR_CHAR_CODE('degk')
};

enum {
                                                                /* AppleScript 1.3: Folder Actions */
    kFAServerApp                = FOUR_CHAR_CODE('ssrv'),       /* Creator code for Folder Actions Server*/
    kDoFolderActionEvent        = FOUR_CHAR_CODE('fola'),       /* Event the Finder sends to the Folder Actions FBA*/
    kFolderActionCode           = FOUR_CHAR_CODE('actn'),       /* Parameter that contains the Folder Action*/
    kFolderOpenedEvent          = FOUR_CHAR_CODE('fopn'),       /* Value of kFolderActionCode parameter; sent to script as event*/
    kFolderClosedEvent          = FOUR_CHAR_CODE('fclo'),
    kFolderWindowMovedEvent     = FOUR_CHAR_CODE('fsiz'),
    kFolderItemsAddedEvent      = FOUR_CHAR_CODE('fget'),
    kFolderItemsRemovedEvent    = FOUR_CHAR_CODE('flos'),
    kItemList                   = FOUR_CHAR_CODE('flst'),       /* List parameter for added and removed items*/
    kNewSizeParameter           = FOUR_CHAR_CODE('fnsz'),       /* Parameter for moved window*/
    kFASuiteCode                = FOUR_CHAR_CODE('faco'),       /* Suite code for the following events*/
    kFAAttachCommand            = FOUR_CHAR_CODE('atfa'),       /* Attach event id*/
    kFARemoveCommand            = FOUR_CHAR_CODE('rmfa'),       /* Remove event id*/
    kFAEditCommand              = FOUR_CHAR_CODE('edfa'),       /* Edit event id*/
    kFAFileParam                = FOUR_CHAR_CODE('faal'),       /* Key for file parameter for Attach*/
    kFAIndexParam               = FOUR_CHAR_CODE('indx')        /* Key for index (0-based) parameter for Remove and Edit*/
};

/* AppleScript 1.3 Internet Suite */
enum {
                                                                /* Suite code */
    kAEInternetSuite            = FOUR_CHAR_CODE('gurl'),
    kAEISWebStarSuite           = FOUR_CHAR_CODE('WWW ')
};

enum {
                                                                /* Events */
    kAEISGetURL                 = FOUR_CHAR_CODE('gurl'),
    KAEISHandleCGI              = FOUR_CHAR_CODE('sdoc')
};

enum {
                                                                /* Classes */
    cURL                        = FOUR_CHAR_CODE('url '),
    cInternetAddress            = FOUR_CHAR_CODE('IPAD'),
    cHTML                       = FOUR_CHAR_CODE('html'),
    cFTPItem                    = FOUR_CHAR_CODE('ftp ')
};

enum {
                                                                /* Parameters */
    kAEISHTTPSearchArgs         = FOUR_CHAR_CODE('kfor'),
    kAEISPostArgs               = FOUR_CHAR_CODE('post'),
    kAEISMethod                 = FOUR_CHAR_CODE('meth'),
    kAEISClientAddress          = FOUR_CHAR_CODE('addr'),
    kAEISUserName               = FOUR_CHAR_CODE('user'),
    kAEISPassword               = FOUR_CHAR_CODE('pass'),
    kAEISFromUser               = FOUR_CHAR_CODE('frmu'),
    kAEISServerName             = FOUR_CHAR_CODE('svnm'),
    kAEISServerPort             = FOUR_CHAR_CODE('svpt'),
    kAEISScriptName             = FOUR_CHAR_CODE('scnm'),
    kAEISContentType            = FOUR_CHAR_CODE('ctyp'),
    kAEISReferrer               = FOUR_CHAR_CODE('refr'),
    kAEISUserAgent              = FOUR_CHAR_CODE('Agnt'),
    kAEISAction                 = FOUR_CHAR_CODE('Kact'),
    kAEISActionPath             = FOUR_CHAR_CODE('Kapt'),
    kAEISClientIP               = FOUR_CHAR_CODE('Kcip'),
    kAEISFullRequest            = FOUR_CHAR_CODE('Kfrq')
};

enum {
                                                                /* Properties */
    pScheme                     = FOUR_CHAR_CODE('pusc'),
    pHost                       = FOUR_CHAR_CODE('HOST'),
    pPath                       = FOUR_CHAR_CODE('FTPc'),
    pUserName                   = FOUR_CHAR_CODE('RAun'),
    pUserPassword               = FOUR_CHAR_CODE('RApw'),
    pDNSForm                    = FOUR_CHAR_CODE('pDNS'),
    pURL                        = FOUR_CHAR_CODE('pURL'),
    pTextEncoding               = FOUR_CHAR_CODE('ptxe'),
    pFTPKind                    = FOUR_CHAR_CODE('kind')
};

enum {
                                                                /* Scheme enumerations */
    eScheme                     = FOUR_CHAR_CODE('esch'),
    eurlHTTP                    = FOUR_CHAR_CODE('http'),       /* RFC 2068 */
    eurlHTTPS                   = FOUR_CHAR_CODE('htps'),
    eurlFTP                     = FOUR_CHAR_CODE('ftp '),       /* RFC 1738 */
    eurlMail                    = FOUR_CHAR_CODE('mail'),       /* RFC 2638 */
    eurlFile                    = FOUR_CHAR_CODE('file'),       /* RFC 1738 */
    eurlGopher                  = FOUR_CHAR_CODE('gphr'),       /* RFC 1738 */
    eurlTelnet                  = FOUR_CHAR_CODE('tlnt'),       /* RFC 1738 */
    eurlNews                    = FOUR_CHAR_CODE('news'),       /* RFC 1738 */
    eurlSNews                   = FOUR_CHAR_CODE('snws'),
    eurlNNTP                    = FOUR_CHAR_CODE('nntp'),       /* RFC 1738 */
    eurlMessage                 = FOUR_CHAR_CODE('mess'),
    eurlMailbox                 = FOUR_CHAR_CODE('mbox'),
    eurlMulti                   = FOUR_CHAR_CODE('mult'),
    eurlLaunch                  = FOUR_CHAR_CODE('laun'),
    eurlAFP                     = FOUR_CHAR_CODE('afp '),
    eurlAT                      = FOUR_CHAR_CODE('at  '),
    eurlEPPC                    = FOUR_CHAR_CODE('eppc'),
    eurlRTSP                    = FOUR_CHAR_CODE('rtsp'),       /* RFC 2326 */
    eurlIMAP                    = FOUR_CHAR_CODE('imap'),       /* RFC 2192 */
    eurlNFS                     = FOUR_CHAR_CODE('unfs'),       /* RFC 2224 */
    eurlPOP                     = FOUR_CHAR_CODE('upop'),       /* RFC 2384 */
    eurlLDAP                    = FOUR_CHAR_CODE('uldp'),       /* RFC 2255 */
    eurlUnknown                 = FOUR_CHAR_CODE('url?')
};

enum {
                                                                /* AppleScript 1.3: Connectivity Suite in aeut */
    kConnSuite                  = FOUR_CHAR_CODE('macc'),
    cDevSpec                    = FOUR_CHAR_CODE('cdev'),
    cAddressSpec                = FOUR_CHAR_CODE('cadr'),
    cADBAddress                 = FOUR_CHAR_CODE('cadb'),
    cAppleTalkAddress           = FOUR_CHAR_CODE('cat '),
    cBusAddress                 = FOUR_CHAR_CODE('cbus'),
    cEthernetAddress            = FOUR_CHAR_CODE('cen '),
    cFireWireAddress            = FOUR_CHAR_CODE('cfw '),
    cIPAddress                  = FOUR_CHAR_CODE('cip '),
    cLocalTalkAddress           = FOUR_CHAR_CODE('clt '),
    cSCSIAddress                = FOUR_CHAR_CODE('cscs'),
    cTokenRingAddress           = FOUR_CHAR_CODE('ctok'),
    cUSBAddress                 = FOUR_CHAR_CODE('cusb'),       /* */
                                                                /* Properties */
    pDeviceType                 = FOUR_CHAR_CODE('pdvt'),
    pDeviceAddress              = FOUR_CHAR_CODE('pdva'),
    pConduit                    = FOUR_CHAR_CODE('pcon'),
    pProtocol                   = FOUR_CHAR_CODE('pprt'),       /* cde 4/27/98 was 'ppro' conflicted with DB suite */
    pATMachine                  = FOUR_CHAR_CODE('patm'),
    pATZone                     = FOUR_CHAR_CODE('patz'),
    pATType                     = FOUR_CHAR_CODE('patt'),
    pDottedDecimal              = FOUR_CHAR_CODE('pipd'),
    pDNS                        = FOUR_CHAR_CODE('pdns'),
    pPort                       = FOUR_CHAR_CODE('ppor'),
    pNetwork                    = FOUR_CHAR_CODE('pnet'),
    pNode                       = FOUR_CHAR_CODE('pnod'),
    pSocket                     = FOUR_CHAR_CODE('psoc'),
    pSCSIBus                    = FOUR_CHAR_CODE('pscb'),
    pSCSILUN                    = FOUR_CHAR_CODE('pslu'),       /* cde 5/22/98 per WWDC developer request */
                                                                /* Enumerations and enumerators */
    eDeviceType                 = FOUR_CHAR_CODE('edvt'),
    eAddressSpec                = FOUR_CHAR_CODE('eads'),
    eConduit                    = FOUR_CHAR_CODE('econ'),
    eProtocol                   = FOUR_CHAR_CODE('epro'),
    eADB                        = FOUR_CHAR_CODE('eadb'),
    eAnalogAudio                = FOUR_CHAR_CODE('epau'),
    eAppleTalk                  = FOUR_CHAR_CODE('epat'),
    eAudioLineIn                = FOUR_CHAR_CODE('ecai'),
    eAudioLineOut               = FOUR_CHAR_CODE('ecal'),       /* cde 4/24/98 changed from 'ecao' to not conflict */
    eAudioOut                   = FOUR_CHAR_CODE('ecao'),
    eBus                        = FOUR_CHAR_CODE('ebus'),
    eCDROM                      = FOUR_CHAR_CODE('ecd '),
    eCommSlot                   = FOUR_CHAR_CODE('eccm'),
    eDigitalAudio               = FOUR_CHAR_CODE('epda'),
    eDisplay                    = FOUR_CHAR_CODE('edds'),
    eDVD                        = FOUR_CHAR_CODE('edvd'),
    eEthernet                   = FOUR_CHAR_CODE('ecen'),
    eFireWire                   = FOUR_CHAR_CODE('ecfw'),
    eFloppy                     = FOUR_CHAR_CODE('efd '),
    eHD                         = FOUR_CHAR_CODE('ehd '),
    eInfrared                   = FOUR_CHAR_CODE('ecir'),
    eIP                         = FOUR_CHAR_CODE('epip'),
    eIrDA                       = FOUR_CHAR_CODE('epir'),
    eIRTalk                     = FOUR_CHAR_CODE('epit'),
    eKeyboard                   = FOUR_CHAR_CODE('ekbd'),
    eLCD                        = FOUR_CHAR_CODE('edlc'),
    eLocalTalk                  = FOUR_CHAR_CODE('eclt'),
    eMacIP                      = FOUR_CHAR_CODE('epmi'),
    eMacVideo                   = FOUR_CHAR_CODE('epmv'),
    eMicrophone                 = FOUR_CHAR_CODE('ecmi'),
    eModemPort                  = FOUR_CHAR_CODE('ecmp'),
    eModemPrinterPort           = FOUR_CHAR_CODE('empp'),
    eModem                      = FOUR_CHAR_CODE('edmm'),
    eMonitorOut                 = FOUR_CHAR_CODE('ecmn'),
    eMouse                      = FOUR_CHAR_CODE('emou'),
    eNuBusCard                  = FOUR_CHAR_CODE('ednb'),
    eNuBus                      = FOUR_CHAR_CODE('enub'),
    ePCcard                     = FOUR_CHAR_CODE('ecpc'),
    ePCIbus                     = FOUR_CHAR_CODE('ecpi'),
    ePCIcard                    = FOUR_CHAR_CODE('edpi'),
    ePDSslot                    = FOUR_CHAR_CODE('ecpd'),
    ePDScard                    = FOUR_CHAR_CODE('epds'),
    ePointingDevice             = FOUR_CHAR_CODE('edpd'),
    ePostScript                 = FOUR_CHAR_CODE('epps'),
    ePPP                        = FOUR_CHAR_CODE('eppp'),
    ePrinterPort                = FOUR_CHAR_CODE('ecpp'),
    ePrinter                    = FOUR_CHAR_CODE('edpr'),
    eSvideo                     = FOUR_CHAR_CODE('epsv'),
    eSCSI                       = FOUR_CHAR_CODE('ecsc'),
    eSerial                     = FOUR_CHAR_CODE('epsr'),
    eSpeakers                   = FOUR_CHAR_CODE('edsp'),
    eStorageDevice              = FOUR_CHAR_CODE('edst'),
    eSVGA                       = FOUR_CHAR_CODE('epsg'),
    eTokenRing                  = FOUR_CHAR_CODE('etok'),
    eTrackball                  = FOUR_CHAR_CODE('etrk'),
    eTrackpad                   = FOUR_CHAR_CODE('edtp'),
    eUSB                        = FOUR_CHAR_CODE('ecus'),
    eVideoIn                    = FOUR_CHAR_CODE('ecvi'),
    eVideoMonitor               = FOUR_CHAR_CODE('edvm'),
    eVideoOut                   = FOUR_CHAR_CODE('ecvo')
};

enum {
                                                                /* AppleScript 1.3: Keystroke class */
    cKeystroke                  = FOUR_CHAR_CODE('kprs'),
    pKeystrokeKey               = FOUR_CHAR_CODE('kMsg'),
    pModifiers                  = FOUR_CHAR_CODE('kMod'),
    pKeyKind                    = FOUR_CHAR_CODE('kknd'),
    eModifiers                  = FOUR_CHAR_CODE('eMds'),
    eOptionDown                 = FOUR_CHAR_CODE('Kopt'),
    eCommandDown                = FOUR_CHAR_CODE('Kcmd'),
    eControlDown                = FOUR_CHAR_CODE('Kctl'),
    eShiftDown                  = FOUR_CHAR_CODE('Ksft'),
    eCapsLockDown               = FOUR_CHAR_CODE('Kclk'),
    eKeyKind                    = FOUR_CHAR_CODE('ekst'),       /* */
                                                                /* Special keys all start with 'ks' */
    eEscapeKey                  = 0x6B733500,                   /* Third byte is virtual key code byte        */
    eDeleteKey                  = 0x6B733300,                   /* (see IM Mac Toolbox Essentials, pp. 2-43) */
    eTabKey                     = 0x6B733000,
    eReturnKey                  = 0x6B732400,
    eClearKey                   = 0x6B734700,
    eEnterKey                   = 0x6B734C00,
    eUpArrowKey                 = 0x6B737E00,
    eDownArrowKey               = 0x6B737D00,
    eLeftArrowKey               = 0x6B737B00,
    eRightArrowKey              = 0x6B737C00,
    eHelpKey                    = 0x6B737200,
    eHomeKey                    = 0x6B737300,
    ePageUpKey                  = 0x6B737400,
    ePageDownKey                = 0x6B737900,
    eForwardDelKey              = 0x6B737500,
    eEndKey                     = 0x6B737700,
    eF1Key                      = 0x6B737A00,
    eF2Key                      = 0x6B737800,
    eF3Key                      = 0x6B736300,
    eF4Key                      = 0x6B737600,
    eF5Key                      = 0x6B736000,
    eF6Key                      = 0x6B736100,
    eF7Key                      = 0x6B736200,
    eF8Key                      = 0x6B736400,
    eF9Key                      = 0x6B736500,
    eF10Key                     = 0x6B736D00,
    eF11Key                     = 0x6B736700,
    eF12Key                     = 0x6B736F00,
    eF13Key                     = 0x6B736900,
    eF14Key                     = 0x6B736B00,
    eF15Key                     = 0x6B737100
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

#endif /* __AEREGISTRY__ */

