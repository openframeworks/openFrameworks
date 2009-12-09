/*
     File:       PEFBinaryFormat.h
 
     Contains:   PEF Types and Macros
 
     Version:    Technology: Master Interfaces
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1993-2001 by Apple Computer, Inc., all rights reserved.
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/


#ifndef __PEFBINARYFORMAT__
#define __PEFBINARYFORMAT__

#ifndef __MACTYPES__
#include <MacTypes.h>
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



/* -------------------------------------------------------------------------------------------- */
/* Almost all types are padded for natural alignment.  However the PEFExportedSymbol type is    */
/* 10 bytes long, containing two 32 bit fields and one 16 bit field.  Arrays of it must be      */
/* packed, so it requires "68K" alignment.  Setting this globally to 68K should also help       */
/* ensure consistent treatment across compilers.                                                */



/* ======================================================================================== */
/* Overall Structure */
/* ================= */



/* -------------------------------------------------------------------------------------------- */
/* This header contains a complete set of types and macros for dealing with the PEF executable  */
/* format.  While some description is provided, this header is not meant as a primary source    */
/* of documentation on PEF.  An excellent specification of PEF can be found in the Macintosh    */
/* Runtime Architectures book.  This header is primarily a physical format description.  Thus   */
/* it depends on as few other headers as possible and structure fields have obvious sizes.      */
/*                                                                                              */
/* The physical storage for a PEF executable is known as a "container".  This refers to just    */
/* the executable itself, not the file etc.  E.g. if five DLLs are packaged in a single file's  */
/* data fork, that one data fork has five containers within it.                                 */
/*                                                                                              */
/* A PEF container consists of an overall header, followed by one or more section headers,      */
/* followed by the section name table, followed by the contents for the sections.  Some kinds   */
/* of sections have specific internal representation.  The "loader" section is the most common  */
/* of these special sections.  It contains information on the exports, imports, and runtime     */
/* relocations required to prepare the executable.  PEF containers are self contained, all      */
/* portions are located via relative offsets.                                                   */
/*                                                                                              */
/*                                                                                              */
/*          +-------------------------------+                                                   */
/*          |       Container Header        |   40 bytes                                        */
/*          +-------------------------------+                                                   */
/*          |       Section 0 header        |   28 bytes each                                   */
/*          |...............................|                                                   */
/*          |           - - - -             |                                                   */
/*          |...............................|                                                   */
/*          |       Section n-1 header      |                                                   */
/*          +-------------------------------+                                                   */
/*          |       Section Name Table      |                                                   */
/*          +-------------------------------+                                                   */
/*          |       Section x raw data      |                                                   */
/*          +-------------------------------+                                                   */
/*          |           - - - -             |                                                   */
/*          +-------------------------------+                                                   */
/*          |       Section y raw data      |                                                   */
/*          +-------------------------------+                                                   */
/*                                                                                              */
/*                                                                                              */
/* The sections are implicitly numbered from 0 to n according to the order of their headers.    */
/* The headers of the instantiated sections must precede those of the non-instantiated          */
/* sections.  The ordering of the raw data is independent of the section header ordering.       */
/* Each section header contains the offset for that section's raw data.                         */



/* =========================================================================================== */
/* Container Header */
/* ================ */




struct PEFContainerHeader {
    OSType                          tag1;                       /* Must contain 'Joy!'.*/
    OSType                          tag2;                       /* Must contain 'peff'.  (Yes, with two 'f's.)*/
    OSType                          architecture;               /* The ISA for code sections.  Constants in CodeFragments.h.*/
    UInt32                          formatVersion;              /* The physical format version.*/
    UInt32                          dateTimeStamp;              /* Macintosh format creation/modification stamp.*/
    UInt32                          oldDefVersion;              /* Old definition version number for the code fragment.*/
    UInt32                          oldImpVersion;              /* Old implementation version number for the code fragment.*/
    UInt32                          currentVersion;             /* Current version number for the code fragment.*/
    UInt16                          sectionCount;               /* Total number of section headers that follow.*/
    UInt16                          instSectionCount;           /* Number of instantiated sections.*/
    UInt32                          reservedA;                  /* Reserved, must be written as zero.*/
};
typedef struct PEFContainerHeader       PEFContainerHeader;
enum {
    kPEFTag1                    = FOUR_CHAR_CODE('Joy!'),       /* For non-Apple compilers: 0x4A6F7921.*/
    kPEFTag2                    = FOUR_CHAR_CODE('peff'),       /* For non-Apple compilers: 0x70656666.*/
    kPEFVersion                 = 0x00000001
};


enum {
    kPEFFirstSectionHeaderOffset = sizeof(PEFContainerHeader)
};

#define PEFFirstSectionNameOffset(container)    \
            ( kPEFFirstSectionHeaderOffset + ((container)->sectionCount * sizeof ( PEFSectionHeader )) )



/* =========================================================================================== */
/* Section Headers */
/* =============== */




struct PEFSectionHeader {
    SInt32                          nameOffset;                 /* Offset of name within the section name table, -1 => none.*/
    UInt32                          defaultAddress;             /* Default address, affects relocations.*/
    UInt32                          totalLength;                /* Fully expanded size in bytes of the section contents.*/
    UInt32                          unpackedLength;             /* Size in bytes of the "initialized" part of the contents.*/
    UInt32                          containerLength;            /* Size in bytes of the raw data in the container.*/
    UInt32                          containerOffset;            /* Offset of section's raw data.*/
    UInt8                           sectionKind;                /* Kind of section contents/usage.*/
    UInt8                           shareKind;                  /* Sharing level, if a writeable section.*/
    UInt8                           alignment;                  /* Preferred alignment, expressed as log 2.*/
    UInt8                           reservedA;                  /* Reserved, must be zero.*/
};
typedef struct PEFSectionHeader         PEFSectionHeader;
enum {
                                                                /* Values for the sectionKind field.*/
                                                                /*    Section kind values for instantiated sections.*/
    kPEFCodeSection             = 0,                            /* Code, presumed pure & position independent.*/
    kPEFUnpackedDataSection     = 1,                            /* Unpacked writeable data.*/
    kPEFPackedDataSection       = 2,                            /* Packed writeable data.*/
    kPEFConstantSection         = 3,                            /* Read-only data.*/
    kPEFExecDataSection         = 6,                            /* Intermixed code and writeable data.*/
                                                                /* Section kind values for non-instantiated sections.*/
    kPEFLoaderSection           = 4,                            /* Loader tables.*/
    kPEFDebugSection            = 5,                            /* Reserved for future use.*/
    kPEFExceptionSection        = 7,                            /* Reserved for future use.*/
    kPEFTracebackSection        = 8                             /* Reserved for future use.*/
};


enum {
                                                                /* Values for the shareKind field.*/
    kPEFProcessShare            = 1,                            /* Shared within a single process.*/
    kPEFGlobalShare             = 4,                            /* Shared across the entire system.*/
    kPEFProtectedShare          = 5                             /* Readable across the entire system, writeable only to privileged code.*/
};




/* =========================================================================================== */
/* Packed Data Contents */
/* ==================== */



/* -------------------------------------------------------------------------------------------- */
/* The raw contents of a packed data section are a sequence of byte codes.  The basic format    */
/* has a 3 bit opcode followed by a 5 bit count.  Additional bytes might be used to contain     */
/* counts larger than 31, and to contain a second or third count.  Further additional bytes     */
/* contain actual data values to transfer.                                                      */
/*                                                                                              */
/* All counts are represented in a variable length manner.  A zero in the initial 5 bit count   */
/* indicates the actual value follows.  In this case, and for the second and third counts, the  */
/* count is represented as a variable length sequence of bytes.  The bytes are stored in big    */
/* endian manner, most significant part first.  The high order bit is set in all but the last   */
/* byte.  The value is accumulated by shifting the current value up 7 bits and adding in the    */
/* low order 7 bits of the next byte.                                                           */


enum {
                                                                /* The packed data opcodes.*/
    kPEFPkDataZero              = 0,                            /* Zero fill "count" bytes.*/
    kPEFPkDataBlock             = 1,                            /* Block copy "count" bytes.*/
    kPEFPkDataRepeat            = 2,                            /* Repeat "count" bytes "count2"+1 times.*/
    kPEFPkDataRepeatBlock       = 3,                            /* Interleaved repeated and unique data.*/
    kPEFPkDataRepeatZero        = 4                             /* Interleaved zero and unique data.*/
};


enum {
    kPEFPkDataOpcodeShift       = 5,
    kPEFPkDataCount5Mask        = 0x1F,
    kPEFPkDataMaxCount5         = 31,
    kPEFPkDataVCountShift       = 7,
    kPEFPkDataVCountMask        = 0x7F,
    kPEFPkDataVCountEndMask     = 0x80
};


#define PEFPkDataOpcode(byte) ( ((UInt8)(byte)) >> kPEFPkDataOpcodeShift )

#define PEFPkDataCount5(byte) ( ((UInt8)(byte)) & kPEFPkDataCount5Mask )

#define PEFPkDataComposeInstr(opcode,count5)        \
            ( (((UInt8)(opcode)) << kPEFPkDataOpcodeShift) | ((UInt8)(count5)) )



/* -------------------------------------------------------------------------------------------- */
/* The following code snippet can be used to input a variable length count.                     */
/*                                                                                              */
/*      count = 0;                                                                              */
/*      do {                                                                                    */
/*          byte = *bytePtr++;                                                                  */
/*          count = (count << kPEFPkDataVCountShift) | (byte & kPEFPkDataVCountMask);           */
/*      } while ( (byte & kPEFPkDataVCountEndMask) != 0 );                                      */
/*                                                                                              */
/* The following code snippet can be used to output a variable length count to a byte array.    */
/* This is more complex than the input code because the chunks are output in big endian order.  */
/* Think about handling values like 0 or 0x030000.                                              */
/*                                                                                              */
/*      count = 1;.                                                                             */
/*      tempValue = value >> kPEFPkDataCountShift;                                              */
/*      while ( tempValue != 0 ) {                                                              */
/*          count += 1;                                                                         */
/*          tempValue = tempValue >> kPEFPkDataCountShift;                                      */
/*      }                                                                                       */
/*                                                                                              */
/*      bytePtr += count;                                                                       */
/*      tempPtr = bytePtr - 1;                                                                  */
/*      *tempPtr-- = value;     // ! No need to mask, only the low order byte is stored.        */
/*      for ( count -= 1; count != 0; count -= 1 ) {                                            */
/*          value = value >> kPEFPkDataCountShift;                                              */
/*          *tempPtr-- = value | kPEFPkDataCountEndMask;                                        */
/*      }                                                                                       */



/* =========================================================================================== */
/* Loader Section */
/* ============== */



/* -------------------------------------------------------------------------------------------- */
/* The loader section contains information needed to prepare the code fragment for execution.   */
/* This includes this fragment's exports, the import libraries and the imported symbols from    */
/* each library, and the relocations for the writeable sections.                                */
/*                                                                                              */
/*          +-----------------------------------+               <-- containerOffset --------+   */
/*          |       Loader Info Header          |   56 bytes                                |   */
/*          |-----------------------------------|                                           |   */
/*          |       Imported Library 0          |   24 bytes each                           |   */
/*          |...................................|                                           |   */
/*          |           - - -                   |                                           |   */
/*          |...................................|                                           |   */
/*          |       Imported Library l-1        |                                           |   */
/*          |-----------------------------------|                                           |   */
/*          |       Imported Symbol 0           |   4 bytes each                            |   */
/*          |...................................|                                           |   */
/*          |           - - -                   |                                           |   */
/*          |...................................|                                           |   */
/*          |       Imported Symbol i-1         |                                           |   */
/*          |-----------------------------------|                                           |   */
/*          |       Relocation Header 0         |   12 bytes each                           |   */
/*          |...................................|                                           |   */
/*          |           - - -                   |                                           |   */
/*          |...................................|                                           |   */
/*          |       Relocation Header r-1       |                                           |   */
/*          |-----------------------------------|               <-- + relocInstrOffset -----|   */
/*          |       Relocation Instructions     |                                           |   */
/*          |-----------------------------------|               <-- + loaderStringsOffset --|   */
/*          |       Loader String Table         |                                           |   */
/*          |-----------------------------------|               <-- + exportHashOffset -----+   */
/*          |       Export Hash Slot 0          |   4 bytes each                                */
/*          |...................................|                                               */
/*          |           - - -                   |                                               */
/*          |...................................|                                               */
/*          |       Export Hash Slot h-1        |                                               */
/*          |-----------------------------------|                                               */
/*          |       Export Symbol Key 0         |   4 bytes each                                */
/*          |...................................|                                               */
/*          |           - - -                   |                                               */
/*          |...................................|                                               */
/*          |       Export Symbol Key e-1       |                                               */
/*          |-----------------------------------|                                               */
/*          |       Export Symbol 0             |   10 bytes each                               */
/*          |...................................|                                               */
/*          |           - - -                   |                                               */
/*          |...................................|                                               */
/*          |       Export Symbol e-1           |                                               */
/*          +-----------------------------------+                                               */




struct PEFLoaderInfoHeader {
    SInt32                          mainSection;                /* Section containing the main symbol, -1 => none.*/
    UInt32                          mainOffset;                 /* Offset of main symbol.*/
    SInt32                          initSection;                /* Section containing the init routine's TVector, -1 => none.*/
    UInt32                          initOffset;                 /* Offset of the init routine's TVector.*/
    SInt32                          termSection;                /* Section containing the term routine's TVector, -1 => none.*/
    UInt32                          termOffset;                 /* Offset of the term routine's TVector.*/
    UInt32                          importedLibraryCount;       /* Number of imported libraries.  ('l')*/
    UInt32                          totalImportedSymbolCount;   /* Total number of imported symbols.  ('i')*/
    UInt32                          relocSectionCount;          /* Number of sections with relocations.  ('r')*/
    UInt32                          relocInstrOffset;           /* Offset of the relocation instructions.*/
    UInt32                          loaderStringsOffset;        /* Offset of the loader string table.*/
    UInt32                          exportHashOffset;           /* Offset of the export hash table.*/
    UInt32                          exportHashTablePower;       /* Export hash table size as log 2.  (Log2('h'))*/
    UInt32                          exportedSymbolCount;        /* Number of exported symbols.  ('e')*/
};
typedef struct PEFLoaderInfoHeader      PEFLoaderInfoHeader;


/* =========================================================================================== */
/* Imported Libraries */
/* ------------------ */



struct PEFImportedLibrary {
    UInt32                          nameOffset;                 /* Loader string table offset of library's name.*/
    UInt32                          oldImpVersion;              /* Oldest compatible implementation version.*/
    UInt32                          currentVersion;             /* Current version at build time.*/
    UInt32                          importedSymbolCount;        /* Imported symbol count for this library.*/
    UInt32                          firstImportedSymbol;        /* Index of first imported symbol from this library.*/
    UInt8                           options;                    /* Option bits for this library.*/
    UInt8                           reservedA;                  /* Reserved, must be zero.*/
    UInt16                          reservedB;                  /* Reserved, must be zero.*/
};
typedef struct PEFImportedLibrary       PEFImportedLibrary;
enum {
                                                                /* Bits for the PEFImportedLibrary options field.*/
    kPEFWeakImportLibMask       = 0x40,                         /* The imported library is allowed to be missing.*/
    kPEFInitLibBeforeMask       = 0x80                          /* The imported library must be initialized first.*/
};




/* =========================================================================================== */
/* Imported Symbols */
/* ---------------- */



/* -------------------------------------------------------------------------------------------- */
/* The PEFImportedSymbol type has the following bit field layout.                               */
/*                                                                                              */
/*                                                                     3                        */
/*       0             7 8                                             1                        */
/*      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                       */
/*      | symbol class  | offset of symbol name in loader string table  |                       */
/*      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                       */
/*      |<-- 8 bits --->|<-- 24 bits ---------------------------------->|                       */




struct PEFImportedSymbol {
    UInt32                          classAndName;
};
typedef struct PEFImportedSymbol        PEFImportedSymbol;
enum {
    kPEFImpSymClassShift        = 24,
    kPEFImpSymNameOffsetMask    = 0x00FFFFFF,
    kPEFImpSymMaxNameOffset     = 0x00FFFFFF                    /* 16,777,215*/
};

#define PEFImportedSymbolClass(classAndName)        ((UInt8) ((classAndName) >> kPEFImpSymClassShift))
#define PEFImportedSymbolNameOffset(classAndName)   ((classAndName) & kPEFImpSymNameOffsetMask)

#define PEFComposeImportedSymbol(class,nameOffset)      \
            ( ( ((UInt32)(class)) << kPEFImpSymClassShift ) | ( (UInt32)(nameOffset) ) )

enum {
                                                                /* Imported and exported symbol classes.*/
    kPEFCodeSymbol              = 0x00,
    kPEFDataSymbol              = 0x01,
    kPEFTVectorSymbol           = 0x02,
    kPEFTOCSymbol               = 0x03,
    kPEFGlueSymbol              = 0x04,
    kPEFUndefinedSymbol         = 0x0F,
    kPEFWeakImportSymMask       = 0x80
};




/* =========================================================================================== */
/* Exported Symbol Hash Table */
/* -------------------------- */



/* -------------------------------------------------------------------------------------------- */
/* Exported symbols are described in four parts, optimized for speed of lookup.  These parts    */
/* are the "export hash table", the "export key table", the "export symbol table", and the      */
/* "export name table".  Overall they contain a flattened representation of a fairly normal     */
/* hashed symbol table.                                                                         */
/*                                                                                              */
/* The export hash table is an array of small fixed size elements.  The number of elements is   */
/* a power of 2.  A 32 bit hash word for a symbol is converted into an index into this array.   */
/* Each hash slot contains a count of the number of exported symbols that map to this slot and  */
/* the index of the first of those symbols in the key and symbol tables.  Of course some hash   */
/* slots will have a zero count.                                                                */
/*                                                                                              */
/* The key and symbol tables are also arrays of fixed size elements, one for each exported      */
/* symbol.  Their entries are grouped by hash slot, those elements mapping to the same hash     */
/* slot are contiguous.  The key table contains just the full 32 bit hash word for each         */
/* exported symbol.  The symbol table contains the offset of the symbol's name in the string    */
/* table and other information about the exported symbol.                                       */
/*                                                                                              */
/* To look up an export you take the hashword and compute the hash slot index.  You then scan   */
/* the indicated portion of the key table for matching hashwords.  If a hashword matches, you   */
/* look at the corresponding symbol table entry to find the full symbol name.  If the names     */
/* match the symbol is found.                                                                   */



/* -------------------------------------------------------------------------------------------- */
/* The following function may be used to compute the hash table size.  Signed values are used   */
/* just to avoid potential code generation overhead for unsigned division.                      */
/*                                                                                              */
/*      UInt8   PEFComputeHashTableExponent ( SInt32    exportCount )                           */
/*      {                                                                                       */
/*          SInt32  exponent;                                                                   */
/*                                                                                              */
/*          const SInt32    kExponentLimit      = 16;   // Arbitrary, but must not exceed 30.   */
/*          const SInt32    kAverageChainLimit  = 10;   // Arbitrary, for space/time tradeoff.  */
/*                                                                                              */
/*          for ( exponent = 0; exponent < kExponentLimit; exponent += 1 ) {                    */
/*              if ( (exportCount / (1 << exponent)) < kAverageChainLimit ) break;              */
/*          }                                                                                   */
/*                                                                                              */
/*          return exponent;                                                                    */
/*                                                                                              */
/*      }   // PEFComputeHashTableExponent ()                                                   */



/* -------------------------------------------------------------------------------------------- */
/* The PEFExportedSymbolHashSlot type has the following bit field layout.                       */
/*                                                                                              */
/*                                 1 1                                 3                        */
/*       0                         3 4                                 1                        */
/*      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                       */
/*      | symbol count              | index of first export key         |                       */
/*      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                       */
/*      |<-- 14 bits -------------->|<-- 18 bits ---------------------->|                       */




struct PEFExportedSymbolHashSlot {
    UInt32                          countAndStart;
};
typedef struct PEFExportedSymbolHashSlot PEFExportedSymbolHashSlot;
enum {
    kPEFHashSlotSymCountShift   = 18,
    kPEFHashSlotFirstKeyMask    = 0x0003FFFF,
    kPEFHashSlotMaxSymbolCount  = 0x00003FFF,                   /*  16,383*/
    kPEFHashSlotMaxKeyIndex     = 0x0003FFFF                    /* 262,143*/
};

#define PEFHashTableIndex(fullHashWord,hashTablePower)  \
            ( ( (fullHashWord) ^ ((fullHashWord) >> (hashTablePower)) ) & ((1 << (hashTablePower)) - 1) )

#define PEFHashSlotSymbolCount(countAndStart)   ((UInt32) ((countAndStart) >> kPEFHashSlotSymCountShift))
#define PEFHashSlotFirstKey(countAndStart)      ((countAndStart) & kPEFHashSlotFirstKeyMask)

#define PEFComposeExportedSymbolHashSlot(symbolCount,firstKey)  \
            ( ( ((UInt32)(symbolCount)) << kPEFHashSlotSymCountShift ) | ( (UInt32)(firstKey) ) )



/* =========================================================================================== */
/* Exported Symbol Hash Key */
/* ------------------------ */




struct PEFSplitHashWord {
    UInt16                          nameLength;
    UInt16                          hashValue;
};
typedef struct PEFSplitHashWord         PEFSplitHashWord;

struct PEFExportedSymbolKey {
    union {
        UInt32                          fullHashWord;
        PEFSplitHashWord                splitHashWord;
    }                                 u;
};
typedef struct PEFExportedSymbolKey     PEFExportedSymbolKey;
enum {
    kPEFHashLengthShift         = 16,
    kPEFHashValueMask           = 0x0000FFFF,
    kPEFHashMaxLength           = 0x0000FFFF                    /* 65,535*/
};

#define PEFHashNameLength(fullHashWord) ((UInt32) ((fullHashWord) >> kPEFHashLengthShift))
#define PEFHashValue(fullHashWord)  ((fullHashWord) & kPEFHashValueMask)

#define PEFComposeFullHashWord(nameLength,hashValue)    \
            ( ( ((UInt32)(nameLength)) << kPEFHashLengthShift ) | ( (UInt32)(hashValue) ) )



/* ---------------------------------------------------------------------------------------------------- */
/* The following function computes the full 32 bit hash word.                                           */
/*                                                                                                      */
/*      UInt32  PEFComputeHashWord  ( BytePtr   nameText,       // ! First "letter", not length byte.   */
/*                                    UInt32    nameLength )    // ! The text may be zero terminated.   */
/*      {                                                                                               */
/*          BytePtr charPtr     = nameText;                                                             */
/*          SInt32  hashValue   = 0;        // ! Signed to match old published algorithm.               */
/*          UInt32  length      = 0;                                                                    */
/*          UInt32  limit;                                                                              */
/*          UInt32  result;                                                                             */
/*          UInt8   currChar;                                                                           */
/*                                                                                                      */
/*          #define PseudoRotate(x)  ( ( (x) << 1 ) - ( (x) >> 16 ) )                                   */
/*                                                                                                      */
/*          for ( limit = nameLength; limit > 0; limit -= 1 ) {                                         */
/*              currChar = *charPtr++;                                                                  */
/*              if ( currChar == NULL ) break;                                                          */
/*              length += 1;                                                                            */
/*              hashValue = PseudoRotate ( hashValue ) ^ currChar;                                      */
/*          }                                                                                           */
/*                                                                                                      */
/*          result  = (length << kPEFHashLengthShift) |                                                 */
/*                    ((UInt16) ((hashValue ^ (hashValue >> 16)) & kPEFHashValueMask));                 */
/*                                                                                                      */
/*          return result;                                                                              */
/*                                                                                                      */
/*      }   // PEFComputeHashWord ()                                                                    */



/* =========================================================================================== */
/* Exported Symbols */
/* ---------------- */




struct PEFExportedSymbol {                                      /* ! This structure is 10 bytes long and arrays are packed.*/
    UInt32                          classAndName;               /* A combination of class and name offset.*/
    UInt32                          symbolValue;                /* Typically the symbol's offset within a section.*/
    SInt16                          sectionIndex;               /* The index of the section, or pseudo-section, for the symbol.*/
};
typedef struct PEFExportedSymbol        PEFExportedSymbol;

/* -------------------------------------------------------------------------------------------- */
/* The classAndName field of the PEFExportedSymbol type has the following bit field layout.     */
/*                                                                                              */
/*                                                                     3                        */
/*       0             7 8                                             1                        */
/*      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                       */
/*      | symbol class  | offset of symbol name in loader string table  |                       */
/*      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                       */
/*      |<-- 8 bits --->|<-- 24 bits ---------------------------------->|                       */


enum {
    kPEFExpSymClassShift        = 24,
    kPEFExpSymNameOffsetMask    = 0x00FFFFFF,
    kPEFExpSymMaxNameOffset     = 0x00FFFFFF                    /* 16,777,215*/
};

#define PEFExportedSymbolClass(classAndName)        ((UInt8) ((classAndName) >> kPEFExpSymClassShift))
#define PEFExportedSymbolNameOffset(classAndName)   ((classAndName) & kPEFExpSymNameOffsetMask)

#define PEFComposeExportedSymbol(class,nameOffset)      \
            ( ( ((UInt32)(class)) << kPEFExpSymClassShift ) | ( (UInt32)(nameOffset) ) )

enum {
                                                                /* Negative section indices indicate pseudo-sections.*/
    kPEFAbsoluteExport          = -2,                           /* The symbol value is an absolute address.*/
    kPEFReexportedImport        = -3                            /* The symbol value is the index of a reexported import.*/
};




/* =========================================================================================== */
/* Loader Relocations */
/* ================== */



/* -------------------------------------------------------------------------------------------- */
/* The relocations for a section are defined by a sequence of instructions for an abstract      */
/* machine that is specifically geared to performing relocations commonly needed for the "CFM"  */
/* code generation model.  These instructions occur in 16 bit chunks.  Most instructions have   */
/* just a single chunk.  Instructions that are larger than 16 bits have an opcode and some of   */
/* the operands in the first chunk, with other operands in following chunks.                    */
/*                                                                                              */
/* ! Note that the multi-chunk relocations have separate "Compose" macros for each chunk.  The  */
/* ! macros have the same basic name with a positional suffix of "_1st", "_2nd", etc.           */




typedef UInt16                          PEFRelocChunk;

struct PEFLoaderRelocationHeader {
    UInt16                          sectionIndex;               /* Index of the section to be fixed up.*/
    UInt16                          reservedA;                  /* Reserved, must be zero.*/
    UInt32                          relocCount;                 /* Number of 16 bit relocation chunks.*/
    UInt32                          firstRelocOffset;           /* Offset of first relocation instruction.*/
};
typedef struct PEFLoaderRelocationHeader PEFLoaderRelocationHeader;

/* -------------------------------------------------------------------------------------------- */
/* ! Note that the relocCount field is the number of 16 bit relocation chunks, i.e. 1/2 the     */
/* ! total number of bytes of relocation instructions.  While most relocation instructions are  */
/* ! 16 bits long, some are longer so the number of complete relocation instructions may be     */
/* ! less than the relocCount value.                                                            */



/* ------------------------------------------------------------------------------------ */
/* The PEFRelocField macro is a utility for extracting relocation instruction fields.   */


#define PEFRFShift(offset,length)   (16 - ((offset) + (length)))
#define PEFRFMask(length)           ((1 << (length)) - 1)

#define PEFRelocField(chunk,offset,length)  \
            ( ( (chunk) >> (16 - ((offset) + (length))) ) & ((1 << (length)) - 1) )



/* =========================================================================================== */
/* Basic Relocation Opcodes */
/* ------------------------ */


/* -------------------------------------------------------------------------------------------- */
/* The number of opcode bits varies from 2 to 7.  The enumeration and switch table given here   */
/* are defined in terms of the most significant 7 bits of the first instruction chunk.  An      */
/* instruction is decoded by using the most significant 7 bits as an index into the opcode      */
/* table, which in turn contains appropriately masked forms of the most significant 7 bits.     */
/* The macro PEFRelocBasicOpcode assumes a declaration of the form.                             */
/*                                                                                              */
/*      UInt8 kPEFRelocBasicOpcodes [kPEFRelocBasicOpcodeRange] = { PEFMaskedBasicOpcodes };    */


enum {
    kPEFRelocBasicOpcodeRange   = 128
};

#define PEFRelocBasicOpcode(firstChunk) (kPEFRelocBasicOpcodes[(firstChunk)>>9])



/* -------------------------------------------------------------------------------------------- */
/* The relocation opcodes, clustered by major and minor groups.  The instructions within a      */
/* cluster all have the same bit field layout.  The enumeration values use the high order 7     */
/* bits of the relocation instruction.  Unused low order bits are set to zero.                  */

enum {
    kPEFRelocBySectDWithSkip    = 0x00,                         /* Binary: 00x_xxxx*/
    kPEFRelocBySectC            = 0x20,                         /* Binary: 010_0000, group is "RelocRun"*/
    kPEFRelocBySectD            = 0x21,                         /* Binary: 010_0001*/
    kPEFRelocTVector12          = 0x22,                         /* Binary: 010_0010*/
    kPEFRelocTVector8           = 0x23,                         /* Binary: 010_0011*/
    kPEFRelocVTable8            = 0x24,                         /* Binary: 010_0100*/
    kPEFRelocImportRun          = 0x25,                         /* Binary: 010_0101*/
    kPEFRelocSmByImport         = 0x30,                         /* Binary: 011_0000, group is "RelocSmIndex"*/
    kPEFRelocSmSetSectC         = 0x31,                         /* Binary: 011_0001*/
    kPEFRelocSmSetSectD         = 0x32,                         /* Binary: 011_0010*/
    kPEFRelocSmBySection        = 0x33,                         /* Binary: 011_0011*/
    kPEFRelocIncrPosition       = 0x40,                         /* Binary: 100_0xxx*/
    kPEFRelocSmRepeat           = 0x48,                         /* Binary: 100_1xxx*/
    kPEFRelocSetPosition        = 0x50,                         /* Binary: 101_000x*/
    kPEFRelocLgByImport         = 0x52,                         /* Binary: 101_001x*/
    kPEFRelocLgRepeat           = 0x58,                         /* Binary: 101_100x*/
    kPEFRelocLgSetOrBySection   = 0x5A,                         /* Binary: 101_101x*/
    kPEFRelocUndefinedOpcode    = 0xFF                          /* Used in masking table for all undefined values.*/
};



/* ---------------------------------------------------------------------------- */
/* The RelocLgSetOrBySection instruction has an additional 4 bits of subopcode  */
/* beyond the 7 used by the dispatch table.  To be precise it has 6 plus 4 but  */
/* the dispatch table ignores the 7th bit, so the subdispatch is on all 4 extra */
/* subopcode bits.                                                              */

enum {
    kPEFRelocLgBySectionSubopcode = 0x00,                       /* Binary: 0000*/
    kPEFRelocLgSetSectCSubopcode = 0x01,                        /* Binary: 0001*/
    kPEFRelocLgSetSectDSubopcode = 0x02                         /* Binary: 0010*/
};

#define PEFRelocLgSetOrBySubopcode(chunk) (((chunk) >> 6) & 0x0F)



/* -------------------------------------------------------------------------------------------- */
/* The initial values for the opcode "masking" table.  This has the enumeration values from     */
/* above with appropriate replications for "don't care" bits.  It is almost certainly shorter   */
/* and faster to look up the masked value in a table than to use a branch tree.                 */


#define PEFMaskedBasicOpcodes                                                                                                                   \
                                                                                                                                                \
            kPEFRelocBySectDWithSkip,   kPEFRelocBySectDWithSkip,   kPEFRelocBySectDWithSkip,   kPEFRelocBySectDWithSkip,   /* 0x00 .. 0x03 */  \
            kPEFRelocBySectDWithSkip,   kPEFRelocBySectDWithSkip,   kPEFRelocBySectDWithSkip,   kPEFRelocBySectDWithSkip,   /* 0x04 .. 0x07 */  \
            kPEFRelocBySectDWithSkip,   kPEFRelocBySectDWithSkip,   kPEFRelocBySectDWithSkip,   kPEFRelocBySectDWithSkip,   /* 0x08 .. 0x0B */  \
            kPEFRelocBySectDWithSkip,   kPEFRelocBySectDWithSkip,   kPEFRelocBySectDWithSkip,   kPEFRelocBySectDWithSkip,   /* 0x0C .. 0x0F */  \
                                                                                                                                                \
            kPEFRelocBySectDWithSkip,   kPEFRelocBySectDWithSkip,   kPEFRelocBySectDWithSkip,   kPEFRelocBySectDWithSkip,   /* 0x10 .. 0x13 */  \
            kPEFRelocBySectDWithSkip,   kPEFRelocBySectDWithSkip,   kPEFRelocBySectDWithSkip,   kPEFRelocBySectDWithSkip,   /* 0x14 .. 0x17 */  \
            kPEFRelocBySectDWithSkip,   kPEFRelocBySectDWithSkip,   kPEFRelocBySectDWithSkip,   kPEFRelocBySectDWithSkip,   /* 0x18 .. 0x1B */  \
            kPEFRelocBySectDWithSkip,   kPEFRelocBySectDWithSkip,   kPEFRelocBySectDWithSkip,   kPEFRelocBySectDWithSkip,   /* 0x1C .. 0x1F */  \
                                                                                                                                                \
            kPEFRelocBySectC,           kPEFRelocBySectD,           kPEFRelocTVector12,         kPEFRelocTVector8,          /* 0x20 .. 0x23 */  \
            kPEFRelocVTable8,           kPEFRelocImportRun,         kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   /* 0x24 .. 0x27 */  \
                                                                                                                                                \
            kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   /* 0x28 .. 0x2B */  \
            kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   /* 0x2C .. 0x2F */  \
                                                                                                                                                \
            kPEFRelocSmByImport,        kPEFRelocSmSetSectC,        kPEFRelocSmSetSectD,        kPEFRelocSmBySection,       /* 0x30 .. 0x33 */  \
                                                                                                                                                \
            kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   /* 0x34 .. 0x37 */  \
            kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   /* 0x38 .. 0x3B */  \
            kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   /* 0x3C .. 0x3F */  \
                                                                                                                                                \
            kPEFRelocIncrPosition,      kPEFRelocIncrPosition,      kPEFRelocIncrPosition,      kPEFRelocIncrPosition,      /* 0x40 .. 0x43 */  \
            kPEFRelocIncrPosition,      kPEFRelocIncrPosition,      kPEFRelocIncrPosition,      kPEFRelocIncrPosition,      /* 0x44 .. 0x47 */  \
                                                                                                                                                \
            kPEFRelocSmRepeat,          kPEFRelocSmRepeat,          kPEFRelocSmRepeat,          kPEFRelocSmRepeat,          /* 0x48 .. 0x4B */  \
            kPEFRelocSmRepeat,          kPEFRelocSmRepeat,          kPEFRelocSmRepeat,          kPEFRelocSmRepeat,          /* 0x4C .. 0x4F */  \
                                                                                                                                                \
            kPEFRelocSetPosition,       kPEFRelocSetPosition,       kPEFRelocLgByImport,        kPEFRelocLgByImport,        /* 0x50 .. 0x53 */  \
                                                                                                                                                \
            kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   /* 0x54 .. 0x57 */  \
                                                                                                                                                \
            kPEFRelocLgRepeat,          kPEFRelocLgRepeat,          kPEFRelocLgSetOrBySection,  kPEFRelocLgSetOrBySection,  /* 0x58 .. 0x5B */  \
                                                                                                                                                \
            kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   /* 0x5C .. 0x5F */  \
                                                                                                                                                \
            kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   /* 0x60 .. 0x63 */  \
            kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   /* 0x64 .. 0x67 */  \
            kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   /* 0x68 .. 0x6B */  \
            kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   /* 0x6C .. 0x6F */  \
                                                                                                                                                \
            kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   /* 0x70 .. 0x73 */  \
            kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   /* 0x74 .. 0x77 */  \
            kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   /* 0x78 .. 0x7B */  \
            kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode,   kPEFRelocUndefinedOpcode    /* 0x7C .. 0x7F */



/* =========================================================================================== */
/* RelocBySectDWithSkip Instruction */
/* -------------------------------- */



/* -------------------------------------------------------------------------------------------- */
/* The "RelocBySectDWithSkip" instruction has the following bit field layout.                   */
/*                                                                                              */
/*                           1         1                                                        */
/*       0 1 2             9 0         5                                                        */
/*      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                                                       */
/*      |0 0| skip count    | rel count |                                                       */
/*      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                                                       */
/*      | 2 |<-- 8 bits --->|<--  6 --->|                                                       */
/*                                                                                              */
/* ! Note that the stored skip count and reloc count are the actual values!                     */

enum {
    kPEFRelocWithSkipMaxSkipCount = 255,
    kPEFRelocWithSkipMaxRelocCount = 63
};

#define PEFRelocWithSkipSkipCount(chunk)    PEFRelocField ( (chunk), 2, 8 )
#define PEFRelocWithSkipRelocCount(chunk)   PEFRelocField ( (chunk), 10, 6 )

#define PEFRelocComposeWithSkip(skipCount,relocCount)   \
            ( 0x0000 | (((UInt16)(skipCount)) << 6) | ((UInt16)(relocCount)) )



/* =========================================================================================== */
/* RelocRun Group */
/* -------------- */



/* -------------------------------------------------------------------------------------------- */
/* The "RelocRun" group includes the "RelocBySectC", "RelocBySectD", "RelocTVector12",          */
/* "RelocTVector8", "RelocVTable8", and "RelocImportRun" instructions.  This group has the      */
/* following bit field layout.                                                                  */
/*                                                                                              */
/*                                     1                                                        */
/*       0   2 3     6 7               5                                                        */
/*      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                                                       */
/*      |0 1 0| subop.| run length      |                                                       */
/*      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                                                       */
/*      |  3  |<- 4 ->|<-- 9 bits ----->|                                                       */
/*                                                                                              */
/* ! Note that the stored run length is the actual value minus 1, but the macros deal with the  */
/* ! actual value!                                                                              */

enum {
    kPEFRelocRunMaxRunLength    = 512
};

#define PEFRelocRunSubopcode(chunk) PEFRelocField ( (chunk), 3, 4 )
#define PEFRelocRunRunLength(chunk) (PEFRelocField ( (chunk), 7, 9 ) + 1)

#define PEFRelocComposeRun(subopcode,runLength) \
            ( 0x4000 | (((UInt16)(subopcode)) << 9) | ((UInt16)((runLength)-1)) )

#define PEFRelocComposeBySectC(runLength)       PEFRelocComposeRun ( 0, (runLength) )
#define PEFRelocComposeBySectD(runLength)       PEFRelocComposeRun ( 1, (runLength) )
#define PEFRelocComposeTVector12(runLength)     PEFRelocComposeRun ( 2, (runLength) )
#define PEFRelocComposeTVector8(runLength)      PEFRelocComposeRun ( 3, (runLength) )
#define PEFRelocComposeVTable8(runLength)       PEFRelocComposeRun ( 4, (runLength) )
#define PEFRelocComposeImportRun(runLength)     PEFRelocComposeRun ( 5, (runLength) )



/* =========================================================================================== */
/* RelocSmIndex Group */
/* ------------------ */



/* ---------------------------------------------------------------------------------------- */
/* The "RelocSmIndex" group includes the "RelocSmByImport", "RelocSmSetSectC",              */
/* "RelocSmSetSectD" and "RelocSmBySection" instructions.  This group has the following bit */
/* field layout.                                                                            */
/*                                                                                          */
/*                                     1                                                    */
/*       0   2 3     6 7               5                                                    */
/*      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                                                   */
/*      |0 1 1| subop.| index           |                                                   */
/*      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                                                   */
/*      |  3  |<- 4 ->|<-- 9 bits ----->|                                                   */
/*                                                                                          */
/* ! Note that the stored index is the actual value!                                        */

enum {
    kPEFRelocSmIndexMaxIndex    = 511
};

#define PEFRelocSmIndexSubopcode(chunk) PEFRelocField ( (chunk), 3, 4 )
#define PEFRelocSmIndexIndex(chunk)     PEFRelocField ( (chunk), 7, 9 )

#define PEFRelocComposeSmIndex(subopcode,index) \
            ( 0x6000 | (((UInt16)(subopcode)) << 9) | ((UInt16)(index)) )

#define PEFRelocComposeSmByImport(index)    PEFRelocComposeSmIndex ( 0, (index) )
#define PEFRelocComposeSmSetSectC(index)    PEFRelocComposeSmIndex ( 1, (index) )
#define PEFRelocComposeSmSetSectD(index)    PEFRelocComposeSmIndex ( 2, (index) )
#define PEFRelocComposeSmBySection(index)   PEFRelocComposeSmIndex ( 3, (index) )



/* =========================================================================================== */
/* RelocIncrPosition Instruction */
/* ----------------------------- */



/* -------------------------------------------------------------------------------------------- */
/* The "RelocIncrPosition" instruction has the following bit field layout.                      */
/*                                                                                              */
/*                                     1                                                        */
/*       0     3 4                     5                                                        */
/*      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                                                       */
/*      |1 0 0 0| offset                |                                                       */
/*      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                                                       */
/*      |<- 4 ->|<-- 12 bits ---------->|                                                       */
/*                                                                                              */
/* ! Note that the stored offset is the actual value minus 1, but the macros deal with the      */
/* ! actual value!                                                                              */

enum {
    kPEFRelocIncrPositionMaxOffset = 4096
};

#define PEFRelocIncrPositionOffset(chunk)   (PEFRelocField ( (chunk), 4, 12 ) + 1)

#define PEFRelocComposeIncrPosition(offset) \
            ( 0x8000 | ((UInt16)((offset)-1)) )



/* =========================================================================================== */
/* RelocSmRepeat Instruction */
/* ------------------------- */



/* -------------------------------------------------------------------------------------------- */
/* The "RelocSmRepeat" instruction has the following bit field layout.                          */
/*                                                                                              */
/*                                     1                                                        */
/*       0     3 4     7 8             5                                                        */
/*      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                                                       */
/*      |1 0 0 1| chnks | repeat count  |                                                       */
/*      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                                                       */
/*      |<- 4 ->|<- 4 ->|<-- 8 bits --->|                                                       */
/*                                                                                              */
/* ! Note that the stored chunk count and repeat count are the actual values minus 1, but the   */
/* ! macros deal with the actual values!                                                        */

enum {
    kPEFRelocSmRepeatMaxChunkCount = 16,
    kPEFRelocSmRepeatMaxRepeatCount = 256
};

#define PEFRelocSmRepeatChunkCount(chunk)   (PEFRelocField ( (chunk), 4, 4 ) + 1)
#define PEFRelocSmRepeatRepeatCount(chunk)  (PEFRelocField ( (chunk), 8, 8 ) + 1)

#define PEFRelocComposeSmRepeat(chunkCount,repeatCount) \
            ( 0x9000 | ((((UInt16)(chunkCount))-1) << 8) | (((UInt16)(repeatCount))-1) )



/* =========================================================================================== */
/* RelocSetPosition Instruction */
/* ---------------------------- */



/* -------------------------------------------------------------------------------------------- */
/* The "RelocSetPosition" instruction has the following bit field layout.                       */
/*                                                                                              */
/*                                     1                                   1                    */
/*       0         5 6                 5     0                             5                    */
/*      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                   */
/*      |1 0 1 0 0 0| offset (high)     |   | offset (low)                  |                   */
/*      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                   */
/*      |<-- 6 ---->|<-- 10 bits ------>|   |<-- 16 bits ------------------>|                   */
/*                                                                                              */
/* ! Note that the stored offset is the actual value!                                           */

enum {
    kPEFRelocSetPosMaxOffset    = 0x03FFFFFF                    /* 67,108,863*/
};

#define PEFRelocSetPosOffsetHigh(chunk) PEFRelocField ( (chunk), 6, 10 )

#define PEFRelocSetPosFullOffset(firstChunk,secondChunk)    \
            ( ((((UInt32)(firstChunk)) & 0x03FF) << 16) | ((UInt32)(secondChunk)) )

#define PEFRelocComposeSetPosition_1st(fullOffset)  \
            ( 0xA000 | ((UInt16) (((UInt32)(fullOffset)) >> 16) ) )
#define PEFRelocComposeSetPosition_2nd(fullOffset)  \
            ( (UInt16) ((UInt32)(fullOffset) & 0xFFFF) )



/* =========================================================================================== */
/* RelocLgByImport Instruction */
/* --------------------------- */



/* -------------------------------------------------------------------------------------------- */
/* The "RelocLgByImport" instruction has the following bit field layout.                        */
/*                                                                                              */
/*                                     1                                   1                    */
/*       0         5 6                 5     0                             5                    */
/*      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                   */
/*      |1 0 1 0 0 1| index (high)      |   | index (low)                   |                   */
/*      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                   */
/*      |<-- 6 ---->|<-- 10 bits ------>|   |<-- 16 bits ------------------>|                   */
/*                                                                                              */
/* ! Note that the stored offset is the actual value!                                           */

enum {
    kPEFRelocLgByImportMaxIndex = 0x03FFFFFF                    /* 67,108,863*/
};

#define PEFRelocLgByImportIndexHigh(chunk)  PEFRelocField ( (chunk), 6, 10 )

#define PEFRelocLgByImportFullIndex(firstChunk,secondChunk) \
            ( ((((UInt32)(firstChunk)) & 0x03FF) << 16) | ((UInt32)(secondChunk)) )

#define PEFRelocComposeLgByImport_1st(fullIndex)    \
            ( 0xA400 | ((UInt16) (((UInt32)(fullIndex)) >> 16) ) )
#define PEFRelocComposeLgByImport_2nd(fullIndex)    \
            ( (UInt16) ((UInt32)(fullIndex) & 0xFFFF) )



/* =========================================================================================== */
/* RelocLgRepeat Instruction */
/* ------------------------- */



/* -------------------------------------------------------------------------------------------- */
/* The "RelocLgRepeat" instruction has the following bit field layout.                          */
/*                                                                                              */
/*                           1         1                                   1                    */
/*       0         5 6     9 0         5     0                             5                    */
/*      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                   */
/*      |1 0 1 1 0 0| chnks | rpt (high)|   | repeat count (low)            |                   */
/*      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                   */
/*      |<--  6 --->|<- 4 ->|<--  6 --->|   |<-- 16 bits ------------------>|                   */
/*                                                                                              */
/* ! Note that the stored chunk count is the actual value minus 1, but the macros deal with     */
/* ! the actual value!  The stored repeat count is the actual value!                            */

enum {
    kPEFRelocLgRepeatMaxChunkCount = 16,
    kPEFRelocLgRepeatMaxRepeatCount = 0x003FFFFF                /* 4,194,303*/
};

#define PEFRelocLgRepeatChunkCount(chunk)       (PEFRelocField ( (chunk), 6, 4 ) + 1)
#define PEFRelocLgRepeatRepeatCountHigh(chunk)  PEFRelocField ( (chunk), 10, 6 )

#define PEFRelocLgRepeatFullRepeatCount(firstChunk,secondChunk) \
            ( ((((UInt32)(firstChunk)) & 0x003F) << 16) | ((UInt32)(secondChunk)) )

#define PEFRelocComposeLgRepeat_1st(chunkCount,fullRepeatCount) \
            ( 0xB000 | ((((UInt16)(chunkCount))-1) << 6) | ((UInt16) (((UInt32)(fullRepeatCount)) >>16 ) ) )
#define PEFRelocComposeLgRepeat_2nd(chunkCount,fullRepeatCount) \
            ( (UInt16) ((UInt32)(fullRepeatCount) & 0xFFFF) )



/* =========================================================================================== */
/* RelocLgSetOrBySection Group */
/* --------------------------- */



/* -------------------------------------------------------------------------------------------- */
/* The "RelocLgSetOrBySection" instruction is really a group including the "RelocLgBySection",  */
/* "RelocLgSetSectC" and "RelocLgSetSectD" instructions.  This group has the following bit      */
/* field layout.                                                                                */
/*                                                                                              */
/*                           1         1                                   1                    */
/*       0         5 6     9 0         5     0                             5                    */
/*      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                   */
/*      |1 0 1 1 0 1| subop | idx (high)|   | index (low)                   |                   */
/*      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                   */
/*      |<--  6 --->|<- 4 ->|<--  6 --->|   |<-- 16 bits ------------------>|                   */
/*                                                                                              */
/* ! Note that the stored index is the actual value!                                            */

enum {
    kPEFRelocLgSetOrBySectionMaxIndex = 0x003FFFFF              /* 4,194,303*/
};

#define PEFRelocLgSetOrBySectionSubopcode(chunk)    PEFRelocField ( (chunk), 6, 4 )
#define PEFRelocLgSetOrBySectionIndexHigh(chunk)    PEFRelocField ( (chunk), 10, 6 )

#define PEFRelocLgSetOrBySectionFullIndex(firstChunk,secondChunk)   \
            ( ((((UInt32)(firstChunk)) & 0x003F) << 16) | ((UInt32)(secondChunk)) )

#define PEFRelocComposeLgSetOrBySection_1st(subopcode,fullIndex)    \
            ( 0xB400 | (((UInt16)(subopcode)) << 6) | ((UInt16) (((UInt32)(fullIndex)) >> 16) ) )
#define PEFRelocComposeLgSetOrBySection_2nd(subopcode,fullIndex)    \
            ( (UInt16) ((UInt32)(fullIndex) & 0xFFFF) )

#define PEFRelocComposeLgBySection(fullIndex)   PEFRelocComposeLgSetOrBySection ( 0x00, (fullIndex) )
#define PEFRelocComposeLgSetSectC(fullIndex)    PEFRelocComposeLgSetOrBySection ( 0x01, (fullIndex) )
#define PEFRelocComposeLgSetSectD(fullIndex)    PEFRelocComposeLgSetOrBySection ( 0x02, (fullIndex) )



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

#endif /* __PEFBINARYFORMAT__ */

