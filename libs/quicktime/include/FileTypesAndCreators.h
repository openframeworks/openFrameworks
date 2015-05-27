/*
     File:       FileTypesAndCreators.h
 
     Contains:   Symbolic constants for FileTypes and signatures of popular documents.
 
     Version:    Technology: Macintosh Easy Open 1.1
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1992-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __FILETYPESANDCREATORS__
#define __FILETYPESANDCREATORS__

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

/*

Motivation for FileTypesAndCreators.h

    The TranslationMgr (Macintosh Easy Open) uses "FileTypes" to express and manage
    translation capabilities.  This works well for most Macintosh applications.  It
    simply uses the file type stored in the catalog info by HFS.  But, the TranslationMgr
    needs to work with non-Macintosh files.  These do not have file types (or creators).
    Another problem is some Macintosh applications keep the same file type for a file
    after changing the file format from one version to the next.  Thus, the HFS file type
    cannot be used to distinguish between formats.

    This file is an effort to define FileTypes and creator codes for non-Macintosh file
    formats and define FileTypes that distinguish between formats.


There are two main categories of constants defined in FileTypesAndCreators.h.  One is
creator signatures of applications the other is file types.  To make this easier to use,
some style guidelines are provided.

Creators:

    The symbolic constant for all creator codes begin with "sig". Following "sig" is the
    application name.  Example:

        sigSurfWriter = 'SURF'

    If the application name is localized for different markets, the name of the U.S.
    version is always used. It is optional to make symbolic constants for localized names.
    If a U.S. is not available, another Roman name is used (this is for ease of editing
    in MPW.)  Example:

        sigSurfWriter = 'SURF',
        sigSkiWriter =  'SURF'  // optional definition of Canadian name of app

    If multiple versions of an application exist, and the creator codes are different,
    then the version number is added to the end of the symbolic name, for the first version
    in which the signature changed.  Example:

        sigDirtWriter =     'DIRT', // version 1 and 2 used same signature
        sigDirtWriter3 =    'DRT3'

    If a developer has the same application on another platform as is on the Mac, then the
    creator for a file from the other platform should be the same as the creator of the
    Macintosh application.   Example:

        sigMicrosoftWord =  'MSWD'  // used for Mac and Windows documents

    If there is no similar Macintosh product, then the signature of the platform is used.
    Example:

        sigPCDOS =          'mdos', // for DOS and Windows
        sigAppleProDos =    'pdos'


FileTypes:

    The symbolic constant for all file types begin with "ft". Following "ft" is the ideal
    kind for that document.  Example:

        ftSurfWriterDocument =  'SRFD'

    If the application name is localized for different markets, the kind string of the U.S.
    version is always used. It is optional to make symbolic constants for localized kind
    strings.  If a U.S. is not available, another Roman kind string is used (this is for
    ease of editing in MPW.)  Example:

        ftSurfWriterDocument =  'SRFD,
        ftSkiWriterDocument =   'SRFD'  // optional Canadian name

    If multiple versions of an application exist, and the document for files created by each
    are different (meaning one version of the app can not read the documents of a newer
    version), then the version number is added to the end of the symbolic name, for the
    first version in which the format changed.  Example:

       // version 1 and 2 docs have same format, version 3 has a different format
        ftDirtWriterDocument =          'DDRT',
        ftDirtWriter3Document =         '3DRT'

    If multiple versions of an application exist, and the document for files created by each
    are different, but the catInfo file type has always been the same, then constants are
    made for each unique format and one is made for the catInfo type actually used.

        ftWordDocument =        'WDBN',
        ftWord3Document =       'WDB3',
        ftWord4Document =       'WDB4',
        ftWord5Document =       'WDB5'

    If a developer has the same application on another platform as is on the Mac and the file
    formats are the same (binary interchangeable), then the file type for a file from the other
    platform should be the same as the file type of the Macintosh document.   Example:

        ftPageMakerPublication = 'ALB4' // used for Mac and PC documents

    If there is no similar Macintosh document format, then a file type is made up.  If the
    program is used on two different non-Mac platforms and their file formats are not the
    same, the platform name is added to the name.  Typically after the program name and before
    document.  Example:

        ftXyWriteIIIDocument =  'XyWr'  // XyWriteIII only has a PC format
        ftSurfWindowsDocument = 'SRFW'  // Surf Windows file format
        ftSurfPCDocument =      'SRFP'  // Surf PC file format
*/

/************** Macintosh applications and FileTypes **************/

enum {
                                                                /* Mac word processors */
    sigWord                     = FOUR_CHAR_CODE('MSWD'),
    ftWord3Document             = FOUR_CHAR_CODE('MSW3'),
    ftWord4Document             = FOUR_CHAR_CODE('MSW4'),
    ftWord5Document             = FOUR_CHAR_CODE('MSW5'),
    ftWordDocument              = FOUR_CHAR_CODE('WDBN'),
    ftWordDocumentPC            = FOUR_CHAR_CODE('MWPC'),       /* not registered */
    ftWord1DocumentWindows      = FOUR_CHAR_CODE('WW1 '),       /* not registered */
    ftWord2DocumentWindows      = FOUR_CHAR_CODE('WW2 '),       /* not registered */
    ftRTFDocument               = FOUR_CHAR_CODE('RTF '),       /* not registered */
    sigWordPerfect              = FOUR_CHAR_CODE('SSIW'),
    ftWordPerfectDocument       = FOUR_CHAR_CODE('WPD0'),
    sigWordPerfect2             = FOUR_CHAR_CODE('WPC2'),
    ftWordPerfect2Document      = FOUR_CHAR_CODE('WPD1'),
    ftWordPerfect21Document     = FOUR_CHAR_CODE('WPD2'),
    ftWordPerfect42DocumentPC   = FOUR_CHAR_CODE('.WP4'),       /* not registered */
    ftWordPerfect50DocumentPC   = FOUR_CHAR_CODE('.WP5'),       /* not registered */
    ftWordPerfect51DocumentPC   = FOUR_CHAR_CODE('WP51'),       /* not registered */
    ftWordPerfectGraphicsPC     = FOUR_CHAR_CODE('WPGf'),       /* not registered */
    sigMacWriteII               = FOUR_CHAR_CODE('MWII'),
    ftMacWriteIIDocument        = FOUR_CHAR_CODE('MW2D'),
    sigWriteNow                 = FOUR_CHAR_CODE('nX^n'),
    ftWriteNow2Document         = FOUR_CHAR_CODE('nX^d'),
    ftWriteNow3Document         = FOUR_CHAR_CODE('nX^2'),
    sigMacWrite                 = FOUR_CHAR_CODE('MACA'),
    ftMacWrite5Document         = FOUR_CHAR_CODE('WORD'),
    sigFrameMaker               = FOUR_CHAR_CODE('Fram'),
    ftFrameMakerDocument        = FOUR_CHAR_CODE('FASL'),
    ftFrameMakerMIFDocument     = FOUR_CHAR_CODE('MIF '),
    ftFrameMakerMIF2Document    = FOUR_CHAR_CODE('MIF2'),
    ftFrameMakerMIF3Document    = FOUR_CHAR_CODE('MIF3'),
    sigMSWrite                  = FOUR_CHAR_CODE('MSWT'),
    sigActa                     = FOUR_CHAR_CODE('ACTA'),
    sigTHINKPascal              = FOUR_CHAR_CODE('PJMM'),
    sigTHINKC                   = FOUR_CHAR_CODE('KAHL'),
    sigFullWrite                = FOUR_CHAR_CODE('FWRT'),
    sigTeachText                = FOUR_CHAR_CODE('ttxt'),
    ftTeachTextDocument         = FOUR_CHAR_CODE('ttro'),
    sigSimpleText               = FOUR_CHAR_CODE('ttxt'),
    ftSimpleTextDocument        = FOUR_CHAR_CODE('ttro'),
    sigMPWShell                 = FOUR_CHAR_CODE('MPS '),
    sigQuarkXPress              = FOUR_CHAR_CODE('XPR3'),
    sigNisus                    = FOUR_CHAR_CODE('NISI'),
    sigOmniPage                 = FOUR_CHAR_CODE('PRTC'),
    sigPersonalPress            = FOUR_CHAR_CODE('SCPG'),
    sigPublishItEZ              = FOUR_CHAR_CODE('2CTY'),
    sigReadySetGo               = FOUR_CHAR_CODE('MEMR'),
    sigRagTime                  = FOUR_CHAR_CODE('R#+A'),
    sigLetraStudio              = FOUR_CHAR_CODE('LSTP'),
    sigLetterPerfect            = FOUR_CHAR_CODE('WPCI'),
    sigTheWritingCenter         = 0x0A1A5750,                   /* this 'unprintable unprintable WP' One of the unprintables is a line feed.  */
    sigInstantUpdate            = FOUR_CHAR_CODE('IUA0')
};

enum {
                                                                /* databases */
    sig4thDimension             = FOUR_CHAR_CODE('4D03'),
    ft4thDimensionDB            = FOUR_CHAR_CODE('BAS3'),
    sigFileMakerPro             = FOUR_CHAR_CODE('FMPR'),
    ftFileMakerProDatabase      = FOUR_CHAR_CODE('FMPR'),
    sigHyperCard                = FOUR_CHAR_CODE('WILD'),
    ftHyperCard                 = FOUR_CHAR_CODE('STAK'),
    sigSmartFormAsst            = FOUR_CHAR_CODE('KCFM'),
    ftSmartFormAsst             = FOUR_CHAR_CODE('STCK'),
    sigSmartFormDesign          = FOUR_CHAR_CODE('KCFD'),
    ftSmartFormDesign           = FOUR_CHAR_CODE('CFRM'),
    sigFileForce                = FOUR_CHAR_CODE('4D93'),
    ftFileForceDatabase         = FOUR_CHAR_CODE('FIL3'),
    sigFileMaker2               = FOUR_CHAR_CODE('FMK4'),
    ftFileMaker2Database        = FOUR_CHAR_CODE('FMK$'),
    sigSuperCard                = FOUR_CHAR_CODE('RUNT'),
    sigDoubleHelix              = FOUR_CHAR_CODE('HELX'),
    sigGeoQuery                 = FOUR_CHAR_CODE('RGgq'),
    sigFoxBASE                  = FOUR_CHAR_CODE('FOX+'),
    sigINSPIRATION              = FOUR_CHAR_CODE('CER3'),
    sigPanorama                 = FOUR_CHAR_CODE('KAS1'),
    sigSilverrunLDM             = FOUR_CHAR_CODE('CDML'),
    sigSilverrunDFD             = FOUR_CHAR_CODE('CDDF'),       /* finance */
    sigQuicken                  = FOUR_CHAR_CODE('INTU'),
    sigMacInTax91               = FOUR_CHAR_CODE('MIT1'),
    ftMacInTax91                = FOUR_CHAR_CODE('MITF'),
    sigAccountantInc            = FOUR_CHAR_CODE('APRO'),
    sigAtOnce                   = FOUR_CHAR_CODE('KISS'),
    sigCAT3                     = FOUR_CHAR_CODE('tCat'),
    sigDollarsNSense            = FOUR_CHAR_CODE('EAGP'),
    sigInsightExpert            = FOUR_CHAR_CODE('LSGL'),
    sigMYOB                     = FOUR_CHAR_CODE('MYOB'),
    sigMacMoney                 = FOUR_CHAR_CODE('SSLA'),
    sigManagingYourMoney        = FOUR_CHAR_CODE('MYMC'),
    sigPlainsAndSimple          = FOUR_CHAR_CODE('PEGG'),       /* scheduling */
    sigMacProject2              = FOUR_CHAR_CODE('MPRX'),
    ftMacProject                = FOUR_CHAR_CODE('MPRD'),
    sigMSProject                = FOUR_CHAR_CODE('MSPJ'),
    sigMacProjectPro            = FOUR_CHAR_CODE('MPRP'),       /* utilities */
    sigStuffIt                  = FOUR_CHAR_CODE('SIT!'),
    ftStuffItArchive            = FOUR_CHAR_CODE('SIT!'),
    sigCompactPro               = FOUR_CHAR_CODE('CPCT'),
    ftCompactProArchive         = FOUR_CHAR_CODE('PACT'),
    sigFontographer             = FOUR_CHAR_CODE('aCa2'),
    sigMetamorphosis            = FOUR_CHAR_CODE('MEtP'),
    sigCorrectGrammar           = FOUR_CHAR_CODE('LsCG'),
    sigDynodex                  = FOUR_CHAR_CODE('DYNO'),
    sigMariah                   = FOUR_CHAR_CODE('MarH'),
    sigAddressBook              = FOUR_CHAR_CODE('AdBk'),
    sigThePrintShop             = FOUR_CHAR_CODE('PSHP'),
    sigQuicKeys2                = FOUR_CHAR_CODE('Qky2'),
    sigReadStar2Plus            = FOUR_CHAR_CODE('INOV'),
    sigSoftPC                   = FOUR_CHAR_CODE('PCXT'),
    sigMacMenlo                 = FOUR_CHAR_CODE('MNLO'),
    sigDisinfectant             = FOUR_CHAR_CODE('D2CT'),       /* communications */
    sigSmartcom2                = FOUR_CHAR_CODE('SCOM'),
    sigVersaTermPRO             = FOUR_CHAR_CODE('VPRO'),
    sigVersaTerm                = FOUR_CHAR_CODE('VATM'),
    sigWhiteKnight              = FOUR_CHAR_CODE('WK11'),
    sigNCSATelnet               = FOUR_CHAR_CODE('NCSA'),
    sigDynaComm                 = FOUR_CHAR_CODE('PAR2'),
    sigQMForms                  = FOUR_CHAR_CODE('MLTM'),       /* math and statistics */
    sigMathematica              = FOUR_CHAR_CODE('OMEG'),
    sigMathCAD                  = FOUR_CHAR_CODE('MCAD'),
    sigStatView2                = FOUR_CHAR_CODE('STAT'),
    sigDataDesk                 = FOUR_CHAR_CODE('DDSK'),
    sigPowerMath2               = FOUR_CHAR_CODE('MATH'),
    sigSuperANOVA               = FOUR_CHAR_CODE('SupA'),
    sigSystat                   = FOUR_CHAR_CODE('SYT1'),
    sigTheorist                 = FOUR_CHAR_CODE('Theo')
};

enum {
                                                                /* spreadsheets */
    sigExcel                    = FOUR_CHAR_CODE('XCEL'),
    ftExcel2Spreadsheet         = FOUR_CHAR_CODE('XLS '),
    ftExcel2Macro               = FOUR_CHAR_CODE('XLM '),
    ftExcel2Chart               = FOUR_CHAR_CODE('XLC '),
    ftExcel3Spreadsheet         = FOUR_CHAR_CODE('XLS3'),
    ftExcel3Macro               = FOUR_CHAR_CODE('XLM3'),
    ftExcel3Chart               = FOUR_CHAR_CODE('XLC3'),
    ftExcel4Spreadsheet         = FOUR_CHAR_CODE('XLS4'),
    ftExcel4Macro               = FOUR_CHAR_CODE('XLM4'),
    ftSYLKSpreadsheet           = FOUR_CHAR_CODE('SYLK'),
    sigLotus123                 = FOUR_CHAR_CODE('L123'),
    ft123Spreadsheet            = FOUR_CHAR_CODE('LWKS'),
    sigWingz                    = FOUR_CHAR_CODE('WNGZ'),
    ftWingzSpreadsheet          = FOUR_CHAR_CODE('WZSS'),
    ftWingzScript               = FOUR_CHAR_CODE('WZSC'),
    sigResolve                  = FOUR_CHAR_CODE('Rslv'),
    ftResolve                   = FOUR_CHAR_CODE('RsWs'),
    ftResolveScript             = FOUR_CHAR_CODE('RsWc'),
    sigFullImpact2              = FOUR_CHAR_CODE('Flv2')
};

enum {
                                                                /* graphics */
    sigIllustrator              = FOUR_CHAR_CODE('ART3'),
    ftPostScriptMac             = FOUR_CHAR_CODE('EPSF'),
    sigMacPaint                 = FOUR_CHAR_CODE('MPNT'),
    ftMacPaintGraphic           = FOUR_CHAR_CODE('PNTG'),
    sigSuperPaint               = FOUR_CHAR_CODE('SPNT'),
    ftSuperPaintGraphic         = FOUR_CHAR_CODE('SPTG'),
    sigCanvas                   = FOUR_CHAR_CODE('DAD2'),
    ftCanvasGraphic             = FOUR_CHAR_CODE('drw2'),
    sigUltraPaint               = FOUR_CHAR_CODE('ULTR'),
    ftUltraPaint                = FOUR_CHAR_CODE('UPNT'),
    sigPhotoshop                = FOUR_CHAR_CODE('8BIM'),
    ftPhotoshopGraphic          = FOUR_CHAR_CODE('8BIM'),
    sigMacDrawPro               = FOUR_CHAR_CODE('dPro'),
    ftMacDrawProDrawing         = FOUR_CHAR_CODE('dDoc'),
    sigPageMaker                = FOUR_CHAR_CODE('ALD4'),
    ftPageMakerPublication      = FOUR_CHAR_CODE('ALB4'),
    sigFreeHand                 = FOUR_CHAR_CODE('FHA3'),
    ftFreeHandDrawing           = FOUR_CHAR_CODE('FHD3'),
    sigClarisCAD                = FOUR_CHAR_CODE('CCAD'),
    ftClarisCAD                 = FOUR_CHAR_CODE('CAD2'),
    sigMacDrawII                = FOUR_CHAR_CODE('MDPL'),
    ftMacDrawIIDrawing          = FOUR_CHAR_CODE('DRWG'),
    sigMacroMindDirector        = FOUR_CHAR_CODE('MMDR'),
    ftMMDirectorMovie           = FOUR_CHAR_CODE('VWMD'),
    ftMMDirectorSound           = FOUR_CHAR_CODE('MMSD'),
    sigOptix                    = FOUR_CHAR_CODE('PIXL'),       /* was previously PixelPerfect */
    sigPixelPaint               = FOUR_CHAR_CODE('PIXR'),
    ftPixelPaint                = FOUR_CHAR_CODE('PX01'),
    sigAldusSuper3D             = FOUR_CHAR_CODE('SP3D'),
    ftSuper3DDrawing            = FOUR_CHAR_CODE('3DBX'),
    sigSwivel3D                 = FOUR_CHAR_CODE('SWVL'),
    ftSwivel3DDrawing           = FOUR_CHAR_CODE('SMDL'),
    sigCricketDraw              = FOUR_CHAR_CODE('CRDW'),
    ftCricketDrawing            = FOUR_CHAR_CODE('CKDT'),
    sigCricketGraph             = FOUR_CHAR_CODE('CGRF'),
    ftCricketChart              = FOUR_CHAR_CODE('CGPC'),
    sigDesignCAD                = FOUR_CHAR_CODE('ASBC'),
    ftDesignCADDrawing          = FOUR_CHAR_CODE('DCAD'),
    sigImageStudio              = FOUR_CHAR_CODE('FSPE'),
    ftImageStudioGraphic        = FOUR_CHAR_CODE('RIFF'),
    sigVersaCad                 = FOUR_CHAR_CODE('VCAD'),
    ftVersaCADDrawing           = FOUR_CHAR_CODE('2D  '),
    sigAdobePremier             = FOUR_CHAR_CODE('PrMr'),
    ftAdobePremierMovie         = FOUR_CHAR_CODE('MooV'),
    sigAfterDark                = FOUR_CHAR_CODE('ADrk'),
    ftAfterDarkModule           = FOUR_CHAR_CODE('ADgm'),
    sigClip3D                   = FOUR_CHAR_CODE('EZ3E'),
    ftClip3Dgraphic             = FOUR_CHAR_CODE('EZ3D'),
    sigKaleidaGraph             = FOUR_CHAR_CODE('QKPT'),
    ftKaleidaGraphGraphic       = FOUR_CHAR_CODE('QPCT'),
    sigMacFlow                  = FOUR_CHAR_CODE('MCFL'),
    ftMacFlowChart              = FOUR_CHAR_CODE('FLCH'),
    sigMoviePlayer              = FOUR_CHAR_CODE('TVOD'),
    ftMoviePlayerMovie          = FOUR_CHAR_CODE('MooV'),
    sigMacSpin                  = FOUR_CHAR_CODE('D2SP'),
    ftMacSpinDataSet            = FOUR_CHAR_CODE('D2BN'),
    sigAutoCAD                  = FOUR_CHAR_CODE('ACAD'),
    sigLabVIEW                  = FOUR_CHAR_CODE('LBVW'),
    sigColorMacCheese           = FOUR_CHAR_CODE('CMC '),
    sigMiniCad                  = FOUR_CHAR_CODE('CDP3'),
    sigDreams                   = FOUR_CHAR_CODE('PHNX'),
    sigOmnis5                   = FOUR_CHAR_CODE('Q2$$'),
    sigPhotoMac                 = FOUR_CHAR_CODE('PMAC'),
    sigGraphMaster              = FOUR_CHAR_CODE('GRAM'),
    sigInfiniD                  = FOUR_CHAR_CODE('SI D'),
    sigOfoto                    = FOUR_CHAR_CODE('APLS'),
    sigMacDraw                  = FOUR_CHAR_CODE('MDRW'),
    sigDeltagraphPro            = FOUR_CHAR_CODE('DGRH'),
    sigDesign2                  = FOUR_CHAR_CODE('DESG'),
    sigDesignStudio             = FOUR_CHAR_CODE('MRJN'),
    sigDynaperspective          = FOUR_CHAR_CODE('PERS'),
    sigGenericCADD              = FOUR_CHAR_CODE('CAD3'),
    sigMacDraft                 = FOUR_CHAR_CODE('MD20'),
    sigModelShop                = FOUR_CHAR_CODE('MDSP'),
    sigOasis                    = FOUR_CHAR_CODE('TAOA'),
    sigOBJECTMASTER             = FOUR_CHAR_CODE('BROW'),
    sigMovieRecorder            = FOUR_CHAR_CODE('mrcr'),
    sigPictureCompressor        = FOUR_CHAR_CODE('ppxi'),
    sigPICTViewer               = FOUR_CHAR_CODE('MDTS'),
    sigSmoothie                 = FOUR_CHAR_CODE('Smoo'),
    sigScreenPlay               = FOUR_CHAR_CODE('SPLY'),
    sigStudio1                  = FOUR_CHAR_CODE('ST/1'),
    sigStudio32                 = FOUR_CHAR_CODE('ST32'),
    sigStudio8                  = FOUR_CHAR_CODE('ST/8'),
    sigKidPix                   = FOUR_CHAR_CODE('Kid2'),
    sigDigDarkroom              = FOUR_CHAR_CODE('DIDR')
};

enum {
                                                                /* presentations */
    sigMore                     = FOUR_CHAR_CODE('MOR2'),
    ftMore3Document             = FOUR_CHAR_CODE('MOR3'),
    ftMore2Document             = FOUR_CHAR_CODE('MOR2'),
    sigPersuasion               = FOUR_CHAR_CODE('PLP2'),
    ftPersuasion1Presentation   = FOUR_CHAR_CODE('PRS1'),
    ftPersuasion2Presentation   = FOUR_CHAR_CODE('PRS2'),
    sigPowerPoint               = FOUR_CHAR_CODE('PPNT'),
    ftPowerPointPresentation    = FOUR_CHAR_CODE('SLDS'),
    sigCricketPresents          = FOUR_CHAR_CODE('CRPR'),
    ftCricketPresentation       = FOUR_CHAR_CODE('PRDF'),       /* works */
    sigMSWorks                  = FOUR_CHAR_CODE('PSI2'),
    sigMSWorks3                 = FOUR_CHAR_CODE('MSWK'),
    ftMSWorksWordProcessor      = FOUR_CHAR_CODE('AWWP'),
    ftMSWorksSpreadsheet        = FOUR_CHAR_CODE('AWSS'),
    ftMSWorksDataBase           = FOUR_CHAR_CODE('AWDB'),
    ftMSWorksComm               = FOUR_CHAR_CODE('AWDC'),
    ftMSWorksMacros             = FOUR_CHAR_CODE('AWMC'),
    ftMSWorks1WordProcessor     = FOUR_CHAR_CODE('AWW1'),       /* not registered */
    ftMSWorks1Spreadsheet       = FOUR_CHAR_CODE('AWS1'),       /* not registered */
    ftMSWorks1DataBase          = FOUR_CHAR_CODE('AWD1'),       /* not registered */
    ftMSWorks2WordProcessor     = FOUR_CHAR_CODE('AWW2'),       /* not registered */
    ftMSWorks2Spreadsheet       = FOUR_CHAR_CODE('AWS2'),       /* not registered */
    ftMSWorks2DataBase          = FOUR_CHAR_CODE('AWD2'),       /* not registered */
    ftMSWorks3WordProcessor     = FOUR_CHAR_CODE('AWW3'),       /* not registered */
    ftMSWorks3Spreadsheet       = FOUR_CHAR_CODE('AWS3'),       /* not registered */
    ftMSWorks3DataBase          = FOUR_CHAR_CODE('AWD3'),       /* not registered */
    ftMSWorks3Comm              = FOUR_CHAR_CODE('AWC3'),       /* not registered */
    ftMSWorks3Macro             = FOUR_CHAR_CODE('AWM3'),       /* not registered */
    ftMSWorks3Draw              = FOUR_CHAR_CODE('AWR3'),       /* not registered */
    ftMSWorks2WordProcessorPC   = FOUR_CHAR_CODE('PWW2'),       /* not registered */
    ftMSWorks2DatabasePC        = FOUR_CHAR_CODE('PWDB'),       /* not registered */
    sigGreatWorks               = FOUR_CHAR_CODE('ZEBR'),
    ftGreatWorksWordProcessor   = FOUR_CHAR_CODE('ZWRT'),
    ftGreatWorksSpreadsheet     = FOUR_CHAR_CODE('ZCAL'),
    ftGreatWorksPaint           = FOUR_CHAR_CODE('ZPNT'),
    sigClarisWorks              = FOUR_CHAR_CODE('BOBO'),
    ftClarisWorksWordProcessor  = FOUR_CHAR_CODE('CWWP'),
    ftClarisWorksSpreadsheet    = FOUR_CHAR_CODE('CWSS'),
    ftClarisWorksGraphics       = FOUR_CHAR_CODE('CWGR'),
    sigBeagleWorks              = FOUR_CHAR_CODE('BWks'),
    ftBeagleWorksWordProcessor  = FOUR_CHAR_CODE('BWwp'),
    ftBeagleWorksDatabase       = FOUR_CHAR_CODE('BWdb'),
    ftBeagleWorksSpreadsheet    = FOUR_CHAR_CODE('BWss'),
    ftBeagleWorksComm           = FOUR_CHAR_CODE('BWcm'),
    ftBeagleWorksDrawing        = FOUR_CHAR_CODE('BWdr'),
    ftBeagleWorksGraphic        = FOUR_CHAR_CODE('BWpt'),
    ftPICTFile                  = FOUR_CHAR_CODE('PICT')
};

enum {
                                                                /* entertainment */
    sigPGATourGolf              = FOUR_CHAR_CODE('gOLF'),
    sigSimCity                  = FOUR_CHAR_CODE('MCRP'),
    sigHellCats                 = FOUR_CHAR_CODE('HELL'),       /* education */
    sigReaderRabbit3            = FOUR_CHAR_CODE('RDR3'),       /* Translation applications */
    sigDataVizDesktop           = FOUR_CHAR_CODE('DVDT'),
    sigSotwareBridge            = FOUR_CHAR_CODE('mdos'),
    sigWordForWord              = FOUR_CHAR_CODE('MSTR'),
    sigAppleFileExchange        = FOUR_CHAR_CODE('PSPT'),       /* Apple software */
    sigAppleLink                = FOUR_CHAR_CODE('GEOL'),
    ftAppleLinkAddressBook      = FOUR_CHAR_CODE('ADRS'),
    ftAppleLinkImageFile        = FOUR_CHAR_CODE('SIMA'),
    ftAppleLinkPackage          = FOUR_CHAR_CODE('HBSF'),
    ftAppleLinkConnFile         = FOUR_CHAR_CODE('PETE'),
    ftAppleLinkHelp             = FOUR_CHAR_CODE('HLPF'),
    sigInstaller                = FOUR_CHAR_CODE('bjbc'),
    ftInstallerScript           = FOUR_CHAR_CODE('bjbc'),
    sigDiskCopy                 = FOUR_CHAR_CODE('dCpy'),
    ftDiskCopyImage             = FOUR_CHAR_CODE('dImg'),
    sigResEdit                  = FOUR_CHAR_CODE('RSED'),
    ftResEditResourceFile       = FOUR_CHAR_CODE('rsrc'),
    sigAardvark                 = FOUR_CHAR_CODE('AARD'),
    sigCompatibilityChkr        = FOUR_CHAR_CODE('wkrp'),
    sigMacTerminal              = FOUR_CHAR_CODE('Term'),
    sigSADE                     = FOUR_CHAR_CODE('sade'),
    sigCurare                   = FOUR_CHAR_CODE('Cura'),
    sigPCXChange                = FOUR_CHAR_CODE('dosa'),
    sigAtEase                   = FOUR_CHAR_CODE('mfdr'),
    sigStockItToMe              = FOUR_CHAR_CODE('SITM'),
    sigAppleSearch              = FOUR_CHAR_CODE('asis'),
    sigAppleSearchToo           = FOUR_CHAR_CODE('hobs'),       /* the following are files types for system files */
    ftScriptSystemResourceCollection = FOUR_CHAR_CODE('ifil'),
    ftSoundFile                 = FOUR_CHAR_CODE('sfil'),
    ftFontFile                  = FOUR_CHAR_CODE('ffil'),
    ftTrueTypeFontFile          = FOUR_CHAR_CODE('tfil'),
    ftKeyboardLayout            = FOUR_CHAR_CODE('kfil'),
    ftFontSuitcase              = FOUR_CHAR_CODE('FFIL'),
    ftDASuitcase                = FOUR_CHAR_CODE('DFIL'),
    ftSystemExtension           = FOUR_CHAR_CODE('INIT'),
    ftDAMQueryDocument          = FOUR_CHAR_CODE('qery')
};



/************** Special FileTypes and creators **************/
enum {
    ftApplicationName           = FOUR_CHAR_CODE('apnm'),       /* this is the type used to define the application name in a kind resource */
    sigIndustryStandard         = FOUR_CHAR_CODE('istd'),       /* this is the creator used to define a kind string in a kind resource for a FileType that has many creators  */
    ftXTND13TextImport          = FOUR_CHAR_CODE('xt13')        /* this is a pseduo-format used by "XTND for Apps". The taDstIsAppTranslation bit is set */
};


/************** Apple][ applications and FileTypes **************/
enum {
    sigAppleProDOS              = FOUR_CHAR_CODE('pdos'),       /* not registered */
    ftAppleWorksWordProcessor   = FOUR_CHAR_CODE('1A  '),       /* not registered */
    ftAppleWorks1WordProcessor  = FOUR_CHAR_CODE('1A1 '),       /* not registered */
    ftAppleWorks2WordProcessor  = FOUR_CHAR_CODE('1A2 '),       /* not registered */
    ftAppleWorks3WordProcessor  = FOUR_CHAR_CODE('1A3 '),       /* not registered */
    ftAppleWorksDataBase        = FOUR_CHAR_CODE('19  '),       /* not registered */
    ftAppleWorks1DataBase       = FOUR_CHAR_CODE('191 '),       /* not registered */
    ftAppleWorks2DataBase       = FOUR_CHAR_CODE('192 '),       /* not registered */
    ftAppleWorks3DataBase       = FOUR_CHAR_CODE('193 '),       /* not registered */
    ftAppleWorksSpreadsheet     = FOUR_CHAR_CODE('1B  '),       /* not registered */
    ftAppleWorks1Spreadsheet    = FOUR_CHAR_CODE('1B1 '),       /* not registered */
    ftAppleWorks2Spreadsheet    = FOUR_CHAR_CODE('1B2 '),       /* not registered */
    ftAppleWorks3Spreadsheet    = FOUR_CHAR_CODE('1B3 '),       /* not registered */
    ftAppleWorksWordProcessorGS = FOUR_CHAR_CODE('50  '),       /* not registered */
    ftApple2GS_SuperHiRes       = FOUR_CHAR_CODE('A2SU'),       /* not registered */
    ftApple2GS_SuperHiResPacked = FOUR_CHAR_CODE('A2SP'),       /* not registered */
    ftApple2GS_PaintWorks       = FOUR_CHAR_CODE('A2PW'),       /* not registered */
    ftApple2_DoubleHiRes        = FOUR_CHAR_CODE('A2DU'),       /* not registered */
    ftApple2_DoubleHiResPacked  = FOUR_CHAR_CODE('A2DP'),       /* not registered */
    ftApple2_DoubleHiRes16colors = FOUR_CHAR_CODE('A2DC'),      /* not registered */
    ftApple2_SingleHiRes        = FOUR_CHAR_CODE('A2HU'),       /* not registered */
    ftApple2_SingleHiResPacked  = FOUR_CHAR_CODE('A2HP'),       /* not registered */
    ftApple2_SingleHiRes8colors = FOUR_CHAR_CODE('A2HC')        /* not registered */
};


/************** PC-DOS applications and FileTypes **************/
enum {
    sigPCDOS                    = FOUR_CHAR_CODE('mdos'),       /* not registered */
    ftGenericDocumentPC         = FOUR_CHAR_CODE('TEXT'),       /*    word processor formats */
    ftWordStarDocumentPC        = FOUR_CHAR_CODE('WStr'),       /* not registered */
    ftWordStar4DocumentPC       = FOUR_CHAR_CODE('WSt4'),       /* not registered */
    ftWordStar5DocumentPC       = FOUR_CHAR_CODE('WSt5'),       /* not registered */
    ftWordStar55DocumentPC      = FOUR_CHAR_CODE('WS55'),       /* not registered */
    ftWordStar6DocumentPC       = FOUR_CHAR_CODE('WSt6'),       /* not registered */
    ftWordStar2000DocumentPC    = FOUR_CHAR_CODE('WS20'),       /* not registered */
    ftXyWriteIIIDocumentPC      = FOUR_CHAR_CODE('XyWr'),       /* registered??? */
    ftDecDXDocumentPC           = FOUR_CHAR_CODE('DX  '),       /* registered??? */
    ftDecWPSPlusDocumentPC      = FOUR_CHAR_CODE('WPS+'),       /* registered??? */
    ftDisplayWrite3DocumentPC   = FOUR_CHAR_CODE('DW3 '),       /* registered??? */
    ftDisplayWrite4DocumentPC   = FOUR_CHAR_CODE('DW4 '),       /* registered??? */
    ftDisplayWrite5DocumentPC   = FOUR_CHAR_CODE('DW5 '),       /* registered??? */
    ftIBMWritingAsstDocumentPC  = FOUR_CHAR_CODE('ASST'),       /* registered??? */
    ftManuscript1DocumentPC     = FOUR_CHAR_CODE('MAN1'),       /* registered??? */
    ftManuscript2DocumentPC     = FOUR_CHAR_CODE('MAN2'),       /* registered??? */
    ftMass11PCDocumentPC        = FOUR_CHAR_CODE('M11P'),       /* registered??? */
    ftMass11VaxDocumentPC       = FOUR_CHAR_CODE('M11V'),       /* registered??? */
    ftMultiMateDocumentPC       = FOUR_CHAR_CODE('MMAT'),       /* registered??? */
    ftMultiMate36DocumentPC     = FOUR_CHAR_CODE('MM36'),       /* registered??? */
    ftMultiMate40DocumentPC     = FOUR_CHAR_CODE('MM40'),       /* registered??? */
    ftMultiMateAdvDocumentPC    = FOUR_CHAR_CODE('MMAD'),       /* registered??? */
    ftMultiMateNoteDocumentPC   = FOUR_CHAR_CODE('MMNT'),       /* registered??? */
    ftOfficeWriterDocumentPC    = FOUR_CHAR_CODE('OFFW'),       /* registered??? */
    ftPCFileLetterDocumentPC    = FOUR_CHAR_CODE('PCFL'),       /* registered??? */
    ftPFSWriteADocumentPC       = FOUR_CHAR_CODE('PFSA'),       /* registered??? */
    ftPFSWriteBDocumentPC       = FOUR_CHAR_CODE('PFSB'),       /* registered??? */
    ftPFSPlanDocumentPC         = FOUR_CHAR_CODE('PFSP'),       /* registered??? */
    ftProWrite1DocumentPC       = FOUR_CHAR_CODE('PW1 '),       /* registered??? */
    ftProWrite2DocumentPC       = FOUR_CHAR_CODE('PW2 '),       /* registered??? */
    ftProWritePlusDocumentPC    = FOUR_CHAR_CODE('PW+ '),       /* registered??? */
    ftFirstChoiceDocumentPC     = FOUR_CHAR_CODE('FCH '),       /* registered??? */
    ftFirstChoice3DocumentPC    = FOUR_CHAR_CODE('FCH3'),       /* registered??? */
    ftDCARFTDocumentPC          = FOUR_CHAR_CODE('RFT '),       /* registered??? */
    ftSamnaDocumentPC           = FOUR_CHAR_CODE('SAMN'),       /* registered??? */
    ftSmartDocumentPC           = FOUR_CHAR_CODE('SMRT'),       /* registered??? */
    ftSprintDocumentPC          = FOUR_CHAR_CODE('SPRT'),       /* registered??? */
    ftTotalWordDocumentPC       = FOUR_CHAR_CODE('TOTL'),       /* registered??? */
    ftVolksWriterDocumentPC     = FOUR_CHAR_CODE('VOLK'),       /* registered??? */
    ftWangWPSDocumentPC         = FOUR_CHAR_CODE('WPS '),       /* registered??? */
    ftWordMarcDocumentPC        = FOUR_CHAR_CODE('MARC'),       /* registered??? */
    ftAmiDocumentPC             = FOUR_CHAR_CODE('AMI '),       /* registered??? */
    ftAmiProDocumentPC          = FOUR_CHAR_CODE('APRO'),       /* registered??? */
    ftAmiPro2DocumentPC         = FOUR_CHAR_CODE('APR2'),       /* registered??? */
    ftEnableDocumentPC          = FOUR_CHAR_CODE('ENWP'),       /* registered??? */
                                                                /*    data base formats */
    ftdBaseDatabasePC           = FOUR_CHAR_CODE('DBF '),       /* registered??? */
    ftdBase3DatabasePC          = FOUR_CHAR_CODE('DB3 '),       /* registered??? */
    ftdBase4DatabasePC          = FOUR_CHAR_CODE('DB4 '),       /* registered??? */
    ftDataEaseDatabasePC        = FOUR_CHAR_CODE('DTEZ'),       /* registered??? */
    ftFrameWorkIIIDatabasePC    = FOUR_CHAR_CODE('FWK3'),       /* registered??? */
    ftRBaseVDatabasePC          = FOUR_CHAR_CODE('RBsV'),       /* registered??? */
    ftRBase5000DatabasePC       = FOUR_CHAR_CODE('RB50'),       /* registered??? */
    ftRBaseFile1DatabasePC      = FOUR_CHAR_CODE('RBs1'),       /* registered??? */
    ftRBaseFile3DatabasePC      = FOUR_CHAR_CODE('RBs3'),       /* registered??? */
    ftReflexDatabasePC          = FOUR_CHAR_CODE('RFLX'),       /* registered??? */
    ftQAWriteDatabasePC         = FOUR_CHAR_CODE('QAWT'),       /* registered??? */
    ftQADBaseDatabasePC         = FOUR_CHAR_CODE('QADB'),       /* registered??? */
    ftSmartDataBasePC           = FOUR_CHAR_CODE('SMTD'),       /* registered??? */
    ftFirstChoiceDataBasePC     = FOUR_CHAR_CODE('FCDB')        /* registered??? */
};

enum {
                                                                /*    spread sheet formats */
    ftDIFSpreadsheetPC          = FOUR_CHAR_CODE('DIF '),       /* registered??? */
    ftEnableSpreadsheetPC       = FOUR_CHAR_CODE('ENAB'),       /* registered??? */
    ft123R1SpreadsheetPC        = FOUR_CHAR_CODE('WKS1'),       /* registered??? */
    ft123R2SpreadsheetPC        = FOUR_CHAR_CODE('WKS2'),       /* registered??? */
    ft123R3SpreadsheetPC        = FOUR_CHAR_CODE('WKS3'),       /* registered??? */
    ftParadox3SpreadsheetPC     = FOUR_CHAR_CODE('PDX3'),       /* registered??? */
    ftParadox35SpreadsheetPC    = FOUR_CHAR_CODE('PD35'),       /* registered??? */
    ftQuattroSpreadsheetPC      = FOUR_CHAR_CODE('QTRO'),       /* registered??? */
    ftQuattroProSpreadsheetPC   = FOUR_CHAR_CODE('QTR5'),       /* registered??? */
    ftSuperCalc5SpreadsheetPC   = FOUR_CHAR_CODE('SPC5'),       /* registered??? */
    ftSymphony1SpreadsheetPC    = FOUR_CHAR_CODE('SYM1'),       /* registered??? */
    ftTwinSpreadsheetPC         = FOUR_CHAR_CODE('TWIN'),       /* registered??? */
    ftVPPlannerSpreadsheetPC    = FOUR_CHAR_CODE('VPPL'),       /* registered??? */
    ftSmartSpeadsheetPC         = FOUR_CHAR_CODE('SMSH'),       /* registered??? */
    ftFirstChoiceSpeadsheetPC   = FOUR_CHAR_CODE('FCSS'),       /* registered??? */
                                                                /*    graphics formats */
    ftPCPaintBrushGraphicPC     = FOUR_CHAR_CODE('PCX '),       /* not registered */
    ftLotusPICGraphicPC         = FOUR_CHAR_CODE('.PIC'),       /* not registered */
    ftCGMGraphicPC              = FOUR_CHAR_CODE('.CGM'),       /* not registered */
    ftGEMGraphicPC              = FOUR_CHAR_CODE('.GEM'),       /* not registered */
    ftIMGGraphicPC              = FOUR_CHAR_CODE('.IMG'),       /* not registered */
    ftDXFGraphicPC              = FOUR_CHAR_CODE('.DXF'),       /* not registered */
    ftBitmapWindows             = FOUR_CHAR_CODE('.BMP'),       /* not registered */
    ftMetaFileWindows           = FOUR_CHAR_CODE('.WMF'),       /* not registered */
    ftTIFFGraphic               = FOUR_CHAR_CODE('TIFF'),       /* not registered */
    ftPostScriptPC              = FOUR_CHAR_CODE('EPSP'),
    ftPostScriptWindows         = FOUR_CHAR_CODE('EPSW'),       /* not registered */
    ftDigitalFX_TitleMan        = FOUR_CHAR_CODE('TDIM'),       /* registered??? */
    ftDigitalFX_VideoFX         = FOUR_CHAR_CODE('GRAF'),       /* registered??? */
    ftAutodeskFLIandFLC         = FOUR_CHAR_CODE('FLIC'),       /* registered??? */
    ftGIF                       = FOUR_CHAR_CODE('GIFf'),       /* registered??? */
    ftIFF                       = FOUR_CHAR_CODE('ILBM'),       /* registered??? */
    ftMicrosoftPaint            = FOUR_CHAR_CODE('.MSP'),       /* registered??? */
    ftPixar                     = FOUR_CHAR_CODE('PXAR'),       /* registered??? */
    ftQDV                       = FOUR_CHAR_CODE('.QDV'),       /* registered??? */
    ftRLE_Compuserve            = FOUR_CHAR_CODE('RLEC'),       /* registered??? */
                                                                /*    Generic vector formats */
    ftIGESGraphicPC             = FOUR_CHAR_CODE('IGES'),       /* not registered */
    ftDDES2GraphicPC            = FOUR_CHAR_CODE('DDES'),       /* not registered */
    ft3DGFGraphicPC             = FOUR_CHAR_CODE('3DGF'),       /* not registered */
                                                                /*    Plotter formats */
    ftHPGLGraphicPC             = FOUR_CHAR_CODE('HPGL'),       /* not registered */
    ftDMPLGraphicPC             = FOUR_CHAR_CODE('DMPL'),       /* not registered */
    ftCalComp906GraphicPC       = FOUR_CHAR_CODE('C906'),       /* not registered */
    ftCalComp907GraphicPC       = FOUR_CHAR_CODE('C907'),       /* not registered */
                                                                /*    Vendor-specific formats */
    ftStereoLithographyGraphicPC = FOUR_CHAR_CODE('STL '),      /*    3D Systems     - not registered */
    ftZoomGraphicPC             = FOUR_CHAR_CODE('ZOOM'),       /*    Abvent          - not registered */
    ftFocusGraphicPC            = FOUR_CHAR_CODE('FOCS'),       /*    Abvent          - not registered */
    ftWaveFrontGraphicPC        = FOUR_CHAR_CODE('WOBJ'),       /*    WaveFront      - not registered */
    ftSculpt4DGraphicPC         = FOUR_CHAR_CODE('Scn2'),       /*    Byte By Byte   - not registered */
    ftMiniPascal3GraphicPC      = FOUR_CHAR_CODE('MPT3'),       /*    Graphsoft      - not registered */
    ftMiniPascal4GraphicPC      = FOUR_CHAR_CODE('MPT4'),       /*    Graphsoft      - not registered */
    ftWalkThroughGraphicPC      = FOUR_CHAR_CODE('VWLK'),       /*    Virtus          - not registered */
    ftSiliconGraphics           = FOUR_CHAR_CODE('.SGI'),       /* registered??? */
    ftSunRaster                 = FOUR_CHAR_CODE('.SUN'),       /* registered??? */
    ftTarga                     = FOUR_CHAR_CODE('TPIC'),       /* registered??? */
                                                                /* misc DOS  */
    ftDOSComPC                  = FOUR_CHAR_CODE('.COM'),       /* registered??? */
    ftDOSExecutablePC           = FOUR_CHAR_CODE('.EXE'),       /* registered??? */
    ftDOSArcPC                  = FOUR_CHAR_CODE('.ARC'),       /* registered??? */
    ftAbekas                    = FOUR_CHAR_CODE('ABEK'),       /* registered??? */
    ftDrHaloCUT                 = FOUR_CHAR_CODE('.CUT'),       /* registered??? */
                                                                /* misc Atari */
    ftDegas                     = FOUR_CHAR_CODE('DEGA'),       /* not registered */
    ftNEO                       = FOUR_CHAR_CODE('.NEO')        /* not registered */
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

#endif /* __FILETYPESANDCREATORS__ */

