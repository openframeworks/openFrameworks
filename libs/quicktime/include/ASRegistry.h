/*
     File:       ASRegistry.h
 
     Contains:   AppleScript Registry constants.
 
     Version:    Technology: AppleScript 1.3
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1991-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __ASREGISTRY__
#define __ASREGISTRY__

#ifndef __AEREGISTRY__
#include <AERegistry.h>
#endif

#ifndef __AEOBJECTS__
#include <AEObjects.h>
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
    keyAETarget                 = FOUR_CHAR_CODE('targ'),       /*  0x74617267  */
    keySubjectAttr              = FOUR_CHAR_CODE('subj'),       /*  0x7375626a  */
                                                                /* Magic 'returning' parameter: */
    keyASReturning              = FOUR_CHAR_CODE('Krtn'),       /*  0x4b72746e  */
                                                                /* AppleScript Specific Codes: */
    kASAppleScriptSuite         = FOUR_CHAR_CODE('ascr'),       /*  0x61736372  */
    kASScriptEditorSuite        = FOUR_CHAR_CODE('ToyS'),       /* AppleScript 1.3 added from private headers */
    kASTypeNamesSuite           = FOUR_CHAR_CODE('tpnm'),       /*  0x74706e6d  */
                                                                /* dynamic terminologies */
    typeAETE                    = FOUR_CHAR_CODE('aete'),       /*  0x61657465  */
    typeAEUT                    = FOUR_CHAR_CODE('aeut'),       /*  0x61657574  */
    kGetAETE                    = FOUR_CHAR_CODE('gdte'),       /*  0x67647465  */
    kGetAEUT                    = FOUR_CHAR_CODE('gdut'),       /*  0x67647574  */
    kUpdateAEUT                 = FOUR_CHAR_CODE('udut'),       /*  0x75647574  */
    kUpdateAETE                 = FOUR_CHAR_CODE('udte'),       /*  0x75647465  */
    kCleanUpAEUT                = FOUR_CHAR_CODE('cdut'),       /*  0x63647574  */
    kASComment                  = FOUR_CHAR_CODE('cmnt'),       /*  0x636d6e74  */
    kASLaunchEvent              = FOUR_CHAR_CODE('noop'),       /*  0x6e6f6f70  */
    keyScszResource             = FOUR_CHAR_CODE('scsz'),       /*  0x7363737A  */
    typeScszResource            = FOUR_CHAR_CODE('scsz'),       /*  0x7363737A  */
                                                                /* subroutine calls */
    kASSubroutineEvent          = FOUR_CHAR_CODE('psbr'),       /*  0x70736272  */
    keyASSubroutineName         = FOUR_CHAR_CODE('snam'),       /*  0x736e616d  */
    kASPrepositionalSubroutine  = FOUR_CHAR_CODE('psbr'),       /* AppleScript 1.3 added from private headers */
    keyASPositionalArgs         = FOUR_CHAR_CODE('parg')        /* AppleScript 1.3 added from private headers */
};

enum {
                                                                /* Miscellaneous AppleScript commands */
    kASStartLogEvent            = FOUR_CHAR_CODE('log1'),       /* AppleScript 1.3 Script Editor Start Log */
    kASStopLogEvent             = FOUR_CHAR_CODE('log0'),       /* AppleScript 1.3 Script Editor Stop Log */
    kASCommentEvent             = FOUR_CHAR_CODE('cmnt')        /* AppleScript 1.3 magic "comment" event */
};


/* Operator Events: */
enum {
                                                                /* Binary: */
    kASAdd                      = FOUR_CHAR_CODE('+   '),       /*  0x2b202020  */
    kASSubtract                 = FOUR_CHAR_CODE('-   '),       /*  0x2d202020  */
    kASMultiply                 = FOUR_CHAR_CODE('*   '),       /*  0x2a202020  */
    kASDivide                   = FOUR_CHAR_CODE('/   '),       /*  0x2f202020  */
    kASQuotient                 = FOUR_CHAR_CODE('div '),       /*  0x64697620  */
    kASRemainder                = FOUR_CHAR_CODE('mod '),       /*  0x6d6f6420  */
    kASPower                    = FOUR_CHAR_CODE('^   '),       /*  0x5e202020  */
    kASEqual                    = kAEEquals,
    kASNotEqual                 = 0xad202020,       /*  0xad202020  */
    kASGreaterThan              = kAEGreaterThan,
    kASGreaterThanOrEqual       = kAEGreaterThanEquals,
    kASLessThan                 = kAELessThan,
    kASLessThanOrEqual          = kAELessThanEquals,
    kASComesBefore              = FOUR_CHAR_CODE('cbfr'),       /*  0x63626672  */
    kASComesAfter               = FOUR_CHAR_CODE('cafr'),       /*  0x63616672  */
    kASConcatenate              = FOUR_CHAR_CODE('ccat'),       /*  0x63636174  */
    kASStartsWith               = kAEBeginsWith,
    kASEndsWith                 = kAEEndsWith,
    kASContains                 = kAEContains
};

enum {
    kASAnd                      = kAEAND,
    kASOr                       = kAEOR,                        /* Unary: */
    kASNot                      = kAENOT,
    kASNegate                   = FOUR_CHAR_CODE('neg '),       /*  0x6e656720  */
    keyASArg                    = FOUR_CHAR_CODE('arg ')        /*  0x61726720  */
};

enum {
                                                                /* event code for the 'error' statement */
    kASErrorEventCode           = FOUR_CHAR_CODE('err '),       /*  0x65727220  */
    kOSAErrorArgs               = FOUR_CHAR_CODE('erra'),       /*  0x65727261  */
    keyAEErrorObject            = FOUR_CHAR_CODE('erob'),       /*     Added in AppleScript 1.3 from AppleScript private headers */
                                                                /* Properties: */
    pLength                     = FOUR_CHAR_CODE('leng'),       /*  0x6c656e67  */
    pReverse                    = FOUR_CHAR_CODE('rvse'),       /*  0x72767365  */
    pRest                       = FOUR_CHAR_CODE('rest'),       /*  0x72657374  */
    pInherits                   = FOUR_CHAR_CODE('c@#^'),       /*  0x6340235e  */
    pProperties                 = FOUR_CHAR_CODE('pALL'),       /* User-Defined Record Fields: */
    keyASUserRecordFields       = FOUR_CHAR_CODE('usrf'),       /*  0x75737266  */
    typeUserRecordFields        = typeAEList
};

/* Prepositions: */
enum {
    keyASPrepositionAt          = FOUR_CHAR_CODE('at  '),       /*  0x61742020  */
    keyASPrepositionIn          = FOUR_CHAR_CODE('in  '),       /*  0x696e2020  */
    keyASPrepositionFrom        = FOUR_CHAR_CODE('from'),       /*  0x66726f6d  */
    keyASPrepositionFor         = FOUR_CHAR_CODE('for '),       /*  0x666f7220  */
    keyASPrepositionTo          = FOUR_CHAR_CODE('to  '),       /*  0x746f2020  */
    keyASPrepositionThru        = FOUR_CHAR_CODE('thru'),       /*  0x74687275  */
    keyASPrepositionThrough     = FOUR_CHAR_CODE('thgh'),       /*  0x74686768  */
    keyASPrepositionBy          = FOUR_CHAR_CODE('by  '),       /*  0x62792020  */
    keyASPrepositionOn          = FOUR_CHAR_CODE('on  '),       /*  0x6f6e2020  */
    keyASPrepositionInto        = FOUR_CHAR_CODE('into'),       /*  0x696e746f  */
    keyASPrepositionOnto        = FOUR_CHAR_CODE('onto'),       /*  0x6f6e746f  */
    keyASPrepositionBetween     = FOUR_CHAR_CODE('btwn'),       /*  0x6274776e  */
    keyASPrepositionAgainst     = FOUR_CHAR_CODE('agst'),       /*  0x61677374  */
    keyASPrepositionOutOf       = FOUR_CHAR_CODE('outo'),       /*  0x6f75746f  */
    keyASPrepositionInsteadOf   = FOUR_CHAR_CODE('isto'),       /*  0x6973746f  */
    keyASPrepositionAsideFrom   = FOUR_CHAR_CODE('asdf'),       /*  0x61736466  */
    keyASPrepositionAround      = FOUR_CHAR_CODE('arnd'),       /*  0x61726e64  */
    keyASPrepositionBeside      = FOUR_CHAR_CODE('bsid'),       /*  0x62736964  */
    keyASPrepositionBeneath     = FOUR_CHAR_CODE('bnth'),       /*  0x626e7468  */
    keyASPrepositionUnder       = FOUR_CHAR_CODE('undr')        /*  0x756e6472  */
};

enum {
    keyASPrepositionOver        = FOUR_CHAR_CODE('over'),       /*  0x6f766572  */
    keyASPrepositionAbove       = FOUR_CHAR_CODE('abve'),       /*  0x61627665  */
    keyASPrepositionBelow       = FOUR_CHAR_CODE('belw'),       /*  0x62656c77  */
    keyASPrepositionApartFrom   = FOUR_CHAR_CODE('aprt'),       /*  0x61707274  */
    keyASPrepositionGiven       = FOUR_CHAR_CODE('givn'),       /*  0x6769766e  */
    keyASPrepositionWith        = FOUR_CHAR_CODE('with'),       /*  0x77697468  */
    keyASPrepositionWithout     = FOUR_CHAR_CODE('wout'),       /*  0x776f7574  */
    keyASPrepositionAbout       = FOUR_CHAR_CODE('abou'),       /*  0x61626f75  */
    keyASPrepositionSince       = FOUR_CHAR_CODE('snce'),       /*  0x736e6365  */
    keyASPrepositionUntil       = FOUR_CHAR_CODE('till')        /*  0x74696c6c  */
};

enum {
                                                                /* Terminology & Dialect things: */
    kDialectBundleResType       = FOUR_CHAR_CODE('Dbdl'),       /*  0x4462646c  */
                                                                /* AppleScript Classes and Enums: */
    cConstant                   = typeEnumerated,
    cClassIdentifier            = pClass,
    cObjectBeingExamined        = typeObjectBeingExamined,
    cList                       = typeAEList,
    cSmallReal                  = typeSMFloat,
    cReal                       = typeFloat,
    cRecord                     = typeAERecord,
    cReference                  = cObjectSpecifier,
    cUndefined                  = FOUR_CHAR_CODE('undf'),       /*  0x756e6466  */
    cMissingValue               = FOUR_CHAR_CODE('msng'),       /*  AppleScript 1.3 newly created*/
    cSymbol                     = FOUR_CHAR_CODE('symb'),       /*  0x73796d62  */
    cLinkedList                 = FOUR_CHAR_CODE('llst'),       /*  0x6c6c7374  */
    cVector                     = FOUR_CHAR_CODE('vect'),       /*  0x76656374  */
    cEventIdentifier            = FOUR_CHAR_CODE('evnt'),       /*  0x65766e74  */
    cKeyIdentifier              = FOUR_CHAR_CODE('kyid'),       /*  0x6b796964  */
    cUserIdentifier             = FOUR_CHAR_CODE('uid '),       /*  0x75696420  */
    cPreposition                = FOUR_CHAR_CODE('prep'),       /*  0x70726570  */
    cKeyForm                    = enumKeyForm,
    cScript                     = FOUR_CHAR_CODE('scpt'),       /*  0x73637074  */
    cHandler                    = FOUR_CHAR_CODE('hand'),       /*  0x68616e64  */
    cProcedure                  = FOUR_CHAR_CODE('proc')        /*  0x70726f63  */
};

enum {
    cClosure                    = FOUR_CHAR_CODE('clsr'),       /*  0x636c7372  */
    cRawData                    = FOUR_CHAR_CODE('rdat'),       /*  0x72646174  */
    cStringClass                = typeChar,
    cNumber                     = FOUR_CHAR_CODE('nmbr'),       /*  0x6e6d6272  */
    cListElement                = FOUR_CHAR_CODE('celm'),       /* AppleScript 1.3 added from private headers */
    cListOrRecord               = FOUR_CHAR_CODE('lr  '),       /*  0x6c722020  */
    cListOrString               = FOUR_CHAR_CODE('ls  '),       /*  0x6c732020  */
    cListRecordOrString         = FOUR_CHAR_CODE('lrs '),       /*  0x6c727320  */
    cNumberOrString             = FOUR_CHAR_CODE('ns  '),       /* AppleScript 1.3 for Display Dialog */
    cNumberOrDateTime           = FOUR_CHAR_CODE('nd  '),       /*  0x6e642020  */
    cNumberDateTimeOrString     = FOUR_CHAR_CODE('nds '),       /*  0x6e647320  */
    cAliasOrString              = FOUR_CHAR_CODE('sf  '),
    cSeconds                    = FOUR_CHAR_CODE('scnd'),       /*  0x73636e64  */
    typeSound                   = FOUR_CHAR_CODE('snd '),
    enumBooleanValues           = FOUR_CHAR_CODE('boov'),       /*  Use this instead of typeBoolean to avoid with/without conversion  */
    kAETrue                     = typeTrue,
    kAEFalse                    = typeFalse,
    enumMiscValues              = FOUR_CHAR_CODE('misc'),       /*  0x6d697363  */
    kASCurrentApplication       = FOUR_CHAR_CODE('cura'),       /*  0x63757261  */
                                                                /* User-defined property ospecs: */
    formUserPropertyID          = FOUR_CHAR_CODE('usrp')        /*  0x75737270  */
};

enum {
    cString                     = cStringClass                  /* old name for cStringClass - can't be used in .r files*/
};

enum {
                                                                /* Global properties: */
    pASIt                       = FOUR_CHAR_CODE('it  '),       /*  0x69742020  */
    pASMe                       = FOUR_CHAR_CODE('me  '),       /*  0x6d652020  */
    pASResult                   = FOUR_CHAR_CODE('rslt'),       /*  0x72736c74  */
    pASSpace                    = FOUR_CHAR_CODE('spac'),       /*  0x73706163  */
    pASReturn                   = FOUR_CHAR_CODE('ret '),       /*  0x72657420  */
    pASTab                      = FOUR_CHAR_CODE('tab '),       /*  0x74616220  */
    pASPi                       = FOUR_CHAR_CODE('pi  '),       /*  0x70692020  */
    pASParent                   = FOUR_CHAR_CODE('pare'),       /*  0x70617265  */
    kASInitializeEventCode      = FOUR_CHAR_CODE('init'),       /*  0x696e6974  */
    pASPrintLength              = FOUR_CHAR_CODE('prln'),       /*  0x70726c6e  */
    pASPrintDepth               = FOUR_CHAR_CODE('prdp'),       /*  0x70726470  */
    pASTopLevelScript           = FOUR_CHAR_CODE('ascr')        /*  0x61736372  */
};

enum {
                                                                /* Considerations */
    kAECase                     = FOUR_CHAR_CODE('case'),       /*  0x63617365  */
    kAEDiacritic                = FOUR_CHAR_CODE('diac'),       /*  0x64696163  */
    kAEWhiteSpace               = FOUR_CHAR_CODE('whit'),       /*  0x77686974  */
    kAEHyphens                  = FOUR_CHAR_CODE('hyph'),       /*  0x68797068  */
    kAEExpansion                = FOUR_CHAR_CODE('expa'),       /*  0x65787061  */
    kAEPunctuation              = FOUR_CHAR_CODE('punc'),       /*  0x70756e63  */
    kAEZenkakuHankaku           = FOUR_CHAR_CODE('zkhk'),       /*  0x7a6b686b  */
    kAESmallKana                = FOUR_CHAR_CODE('skna'),       /*  0x736b6e61  */
    kAEKataHiragana             = FOUR_CHAR_CODE('hika'),       /*  0x68696b61  */
                                                                /* AppleScript considerations: */
    kASConsiderReplies          = FOUR_CHAR_CODE('rmte'),       /*  0x726d7465  */
    enumConsiderations          = FOUR_CHAR_CODE('cons')        /*  0x636f6e73  */
};

enum {
    cCoercion                   = FOUR_CHAR_CODE('coec'),       /*  0x636f6563  */
    cCoerceUpperCase            = FOUR_CHAR_CODE('txup'),       /*  0x74787570  */
    cCoerceLowerCase            = FOUR_CHAR_CODE('txlo'),       /*  0x74786c6f  */
    cCoerceRemoveDiacriticals   = FOUR_CHAR_CODE('txdc'),       /*  0x74786463  */
    cCoerceRemovePunctuation    = FOUR_CHAR_CODE('txpc'),       /*  0x74787063  */
    cCoerceRemoveHyphens        = FOUR_CHAR_CODE('txhy'),       /*  0x74786879  */
    cCoerceOneByteToTwoByte     = FOUR_CHAR_CODE('txex'),       /*  0x74786578  */
    cCoerceRemoveWhiteSpace     = FOUR_CHAR_CODE('txws'),       /*  0x74787773  */
    cCoerceSmallKana            = FOUR_CHAR_CODE('txsk'),       /*  0x7478736b  */
    cCoerceZenkakuhankaku       = FOUR_CHAR_CODE('txze'),       /*  0x74787a65  */
    cCoerceKataHiragana         = FOUR_CHAR_CODE('txkh'),       /*  0x74786b68  */
                                                                /* Lorax things: */
    cZone                       = FOUR_CHAR_CODE('zone'),       /*  0x7a6f6e65  */
    cMachine                    = FOUR_CHAR_CODE('mach'),       /*  0x6d616368  */
    cAddress                    = FOUR_CHAR_CODE('addr'),       /*  0x61646472  */
    cRunningAddress             = FOUR_CHAR_CODE('radd'),       /*  0x72616464  */
    cStorage                    = FOUR_CHAR_CODE('stor')        /*  0x73746f72  */
};

enum {
                                                                /* DateTime things: */
    pASWeekday                  = FOUR_CHAR_CODE('wkdy'),       /*  0x776b6479  */
    pASMonth                    = FOUR_CHAR_CODE('mnth'),       /*  0x6d6e7468  */
    pASDay                      = FOUR_CHAR_CODE('day '),       /*  0x64617920  */
    pASYear                     = FOUR_CHAR_CODE('year'),       /*  0x79656172  */
    pASTime                     = FOUR_CHAR_CODE('time'),       /*  0x74696d65  */
    pASDateString               = FOUR_CHAR_CODE('dstr'),       /*  0x64737472  */
    pASTimeString               = FOUR_CHAR_CODE('tstr'),       /*  0x74737472  */
                                                                /* Months */
    cMonth                      = pASMonth,
    cJanuary                    = FOUR_CHAR_CODE('jan '),       /*  0x6a616e20  */
    cFebruary                   = FOUR_CHAR_CODE('feb '),       /*  0x66656220  */
    cMarch                      = FOUR_CHAR_CODE('mar '),       /*  0x6d617220  */
    cApril                      = FOUR_CHAR_CODE('apr '),       /*  0x61707220  */
    cMay                        = FOUR_CHAR_CODE('may '),       /*  0x6d617920  */
    cJune                       = FOUR_CHAR_CODE('jun '),       /*  0x6a756e20  */
    cJuly                       = FOUR_CHAR_CODE('jul '),       /*  0x6a756c20  */
    cAugust                     = FOUR_CHAR_CODE('aug '),       /*  0x61756720  */
    cSeptember                  = FOUR_CHAR_CODE('sep '),       /*  0x73657020  */
    cOctober                    = FOUR_CHAR_CODE('oct '),       /*  0x6f637420  */
    cNovember                   = FOUR_CHAR_CODE('nov '),       /*  0x6e6f7620  */
    cDecember                   = FOUR_CHAR_CODE('dec ')        /*  0x64656320  */
};

enum {
                                                                /* Weekdays */
    cWeekday                    = pASWeekday,
    cSunday                     = FOUR_CHAR_CODE('sun '),       /*  0x73756e20  */
    cMonday                     = FOUR_CHAR_CODE('mon '),       /*  0x6d6f6e20  */
    cTuesday                    = FOUR_CHAR_CODE('tue '),       /*  0x74756520  */
    cWednesday                  = FOUR_CHAR_CODE('wed '),       /*  0x77656420  */
    cThursday                   = FOUR_CHAR_CODE('thu '),       /*  0x74687520  */
    cFriday                     = FOUR_CHAR_CODE('fri '),       /*  0x66726920  */
    cSaturday                   = FOUR_CHAR_CODE('sat '),       /*  0x73617420  */
                                                                /* AS 1.1 Globals: */
    pASQuote                    = FOUR_CHAR_CODE('quot'),       /*  0x71756f74  */
    pASSeconds                  = FOUR_CHAR_CODE('secs'),       /*  0x73656373  */
    pASMinutes                  = FOUR_CHAR_CODE('min '),       /*  0x6d696e20  */
    pASHours                    = FOUR_CHAR_CODE('hour'),       /*  0x686f7572  */
    pASDays                     = FOUR_CHAR_CODE('days'),       /*  0x64617973  */
    pASWeeks                    = FOUR_CHAR_CODE('week'),       /*  0x7765656b  */
                                                                /* Writing Code things: */
    cWritingCodeInfo            = FOUR_CHAR_CODE('citl'),       /*  0x6369746c  */
    pScriptCode                 = FOUR_CHAR_CODE('pscd'),       /*  0x70736364  */
    pLangCode                   = FOUR_CHAR_CODE('plcd'),       /*  0x706c6364  */
                                                                /* Magic Tell and End Tell events for logging: */
    kASMagicTellEvent           = FOUR_CHAR_CODE('tell'),       /*  0x74656c6c  */
    kASMagicEndTellEvent        = FOUR_CHAR_CODE('tend')        /*  0x74656e64  */
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

#endif /* __ASREGISTRY__ */

