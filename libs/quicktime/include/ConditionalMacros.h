/*
     File:       ConditionalMacros.h
 
     Contains:   Set up for compiler independent conditionals
 
     Version:    Technology: Universal Interface Files 3.3
                 Release:    QuickTime 6.0.2
 
     Copyright:  (c) 1993-2001 by Apple Computer, Inc., all rights reserved
 
     Bugs?:      For bug reports, consult the following page on
                 the World Wide Web:
 
                     http://developer.apple.com/bugreporter/
 
*/
#ifndef __CONDITIONALMACROS__
#define __CONDITIONALMACROS__

/****************************************************************************************************
    UNIVERSAL_INTERFACES_VERSION
    
        0x0330 => version 3.3
        0x0320 => version 3.2
        0x0310 => version 3.1
        0x0301 => version 3.0.1
        0x0300 => version 3.0
        0x0210 => version 2.1
        This conditional did not exist prior to version 2.1
****************************************************************************************************/
#define UNIVERSAL_INTERFACES_VERSION 0x0330

/****************************************************************************************************

    TARGET_CPU_*    
    These conditionals specify which microprocessor instruction set is being
    generated.  At most one of these is true, the rest are false.

        TARGET_CPU_PPC          - Compiler is generating PowerPC instructions
        TARGET_CPU_68K          - Compiler is generating 680x0 instructions
        TARGET_CPU_X86          - Compiler is generating x86 instructions
        TARGET_CPU_MIPS         - Compiler is generating MIPS instructions
        TARGET_CPU_SPARC        - Compiler is generating Sparc instructions
        TARGET_CPU_ALPHA        - Compiler is generating Dec Alpha instructions


    TARGET_OS_* 
    These conditionals specify in which Operating System the generated code will
    run. At most one of the these is true, the rest are false.

        TARGET_OS_MAC           - Generate code will run under Mac OS
        TARGET_OS_WIN32         - Generate code will run under 32-bit Windows
        TARGET_OS_UNIX          - Generate code will run under some unix 


    TARGET_RT_* 
    These conditionals specify in which runtime the generated code will
    run. This is needed when the OS and CPU support more than one runtime
    (e.g. MacOS on 68K supports CFM68K and Classic 68k).

        TARGET_RT_LITTLE_ENDIAN - Generated code uses little endian format for integers
        TARGET_RT_BIG_ENDIAN    - Generated code uses big endian format for integers    
        TARGET_RT_MAC_CFM       - TARGET_OS_MAC is true and CFM68K or PowerPC CFM (TVectors) are used
        TARGET_RT_MAC_MACHO     - TARGET_OS_MAC is true and Mach-O style runtime
        TARGET_RT_MAC_68881     - TARGET_OS_MAC is true and 68881 floating point instructions used  


    TARGET__API_*_* 
    These conditionals are used to differentiate between sets of API's on the same
    processor under the same OS.  The first section after _API_ is the OS.  The
    second section is the API set.  Unlike TARGET_OS_ and TARGET_CPU_, these
    conditionals are not mutally exclusive. This file will attempt to auto-configure
    all TARGET_API_*_* values, but will often need a TARGET_API_*_* value predefined
    in order to disambiguate.
    
        TARGET_API_MAC_OS8      - Code is being compiled to run on System 7 through Mac OS 8.x
        TARGET_API_MAC_CARBON   - Code is being compiled to run on Mac OS 8 and Mac OS X via CarbonLib
        TARGET_API_MAC_OSX      - Code is being compiled to run on Mac OS X


    PRAGMA_*
    These conditionals specify whether the compiler supports particular #pragma's
    
        PRAGMA_IMPORT           - Compiler supports: #pragma import on/off/reset
        PRAGMA_ONCE             - Compiler supports: #pragma once
        PRAGMA_STRUCT_ALIGN     - Compiler supports: #pragma options align=mac68k/power/reset
        PRAGMA_STRUCT_PACK      - Compiler supports: #pragma pack(n)
        PRAGMA_STRUCT_PACKPUSH  - Compiler supports: #pragma pack(push, n)/pack(pop)
        PRAGMA_ENUM_PACK        - Compiler supports: #pragma options(!pack_enums)
        PRAGMA_ENUM_ALWAYSINT   - Compiler supports: #pragma enumsalwaysint on/off/reset
        PRAGMA_ENUM_OPTIONS     - Compiler supports: #pragma options enum=int/small/reset


    FOUR_CHAR_CODE
    This conditional does the proper byte swapping to assue that a four character code (e.g. 'TEXT')
    is compiled down to the correct value on all compilers.

        FOUR_CHAR_CODE('abcd')  - Convert a four-char-code to the correct 32-bit value


    TYPE_*
    These conditionals specify whether the compiler supports particular types.

        TYPE_LONGLONG           - Compiler supports "long long" 64-bit integers
        TYPE_BOOL               - Compiler supports "bool"
        TYPE_EXTENDED           - Compiler supports "extended" 80/96 bit floating point


    FUNCTION_*
    These conditionals specify whether the compiler supports particular language extensions
    to function prototypes and definitions.

        FUNCTION_PASCAL         - Compiler supports "pascal void Foo()"
        FUNCTION_DECLSPEC       - Compiler supports "__declspec(xxx) void Foo()"
        FUNCTION_WIN32CC        - Compiler supports "void __cdecl Foo()" and "void __stdcall Foo()"

****************************************************************************************************/

#if defined(__MRC__)
  /*
     MrC[pp] compiler from Apple Computer, Inc.
 */
 #define TARGET_CPU_PPC              1
  #define TARGET_CPU_68K              0
  #define TARGET_CPU_X86              0
  #define TARGET_CPU_MIPS             0
  #define TARGET_CPU_SPARC            0       
   #define TARGET_CPU_ALPHA            0
  #define TARGET_OS_MAC               1
  #define TARGET_OS_WIN32             0
  #define TARGET_OS_UNIX              0
  #define TARGET_RT_LITTLE_ENDIAN     0
  #define TARGET_RT_BIG_ENDIAN        1
  #define TARGET_RT_MAC_CFM           1
  #define TARGET_RT_MAC_MACHO         0
  #define TARGET_RT_MAC_68881         0
  #if (__MRC__  > 0x0200) && (__MRC__ < 0x0700)
      #define PRAGMA_IMPORT           1
  #else
      #define PRAGMA_IMPORT           0
  #endif
 #define PRAGMA_STRUCT_ALIGN         1
  #define PRAGMA_ONCE                 1
  #define PRAGMA_STRUCT_PACK          0
  #define PRAGMA_STRUCT_PACKPUSH      0
  #define PRAGMA_ENUM_PACK            1
  #define PRAGMA_ENUM_ALWAYSINT       0
  #define PRAGMA_ENUM_OPTIONS         0
  #define FOUR_CHAR_CODE(x)           (x)
        
   #if (__MRC__  > 0x0300) && (__MRC__ < 0x0700)
      #if __option(longlong)


           #define TYPE_LONGLONG       1


       #else
          #define TYPE_LONGLONG       0
      #endif
     #if __option(bool)
         #define TYPE_BOOL           1
      #else
          #define TYPE_BOOL           0
      #endif
     #define SLASH_INCLUDES_UNSUPPORTED      !__option(unix_includes)
   #else
      #define TYPE_LONGLONG           0
      #define TYPE_BOOL               0
      #define SLASH_INCLUDES_UNSUPPORTED      1
  #endif
 #define TYPE_EXTENDED               0
  
   #define FUNCTION_PASCAL             1
  #define FUNCTION_DECLSPEC           0
  #define FUNCTION_WIN32CC            0
  


#elif defined(__SC__) && (defined(MPW_CPLUS) || defined(MPW_C))
   /*
     SC[pp] compiler from Apple Computer, Inc.
  */
 #define TARGET_CPU_PPC              0
  #define TARGET_CPU_68K              1
  #define TARGET_CPU_X86              0
  #define TARGET_CPU_MIPS             0
  #define TARGET_CPU_SPARC            0       
   #define TARGET_CPU_ALPHA            0
  #define TARGET_OS_MAC               1
  #define TARGET_OS_WIN32             0
  #define TARGET_OS_UNIX              0
  #define TARGET_RT_LITTLE_ENDIAN     0
  #define TARGET_RT_BIG_ENDIAN        1
  #if defined(__CFM68K__)
        #define TARGET_RT_MAC_CFM       1
  #else
      #define TARGET_RT_MAC_CFM       0
  #endif
 #define TARGET_RT_MAC_MACHO         0
  #if defined(mc68881)
       #define TARGET_RT_MAC_68881     1
  #else
      #define TARGET_RT_MAC_68881     0
  #endif
 #if TARGET_RT_MAC_CFM 
     #define PRAGMA_IMPORT           1
      #if (__SC__ <= 0x0810)
         /* old versions of SC don't support "#pragma import reset" */
          #define PRAGMA_IMPORT_OFF   1
      #endif
 #else
      #define PRAGMA_IMPORT           0
  #endif
 #if (__SC__  >= 0x0801)
        #define PRAGMA_STRUCT_ALIGN     1
  #else
      #define PRAGMA_STRUCT_ALIGN     0
  #endif
 #define PRAGMA_ONCE                 0
  #define PRAGMA_STRUCT_PACK          0
  #define PRAGMA_STRUCT_PACKPUSH      0
  #define PRAGMA_ENUM_PACK            1
  #define PRAGMA_ENUM_ALWAYSINT       0
  #define PRAGMA_ENUM_OPTIONS         0
  #define FOUR_CHAR_CODE(x)           (x)
    
   #define TYPE_LONGLONG               0
  #define TYPE_EXTENDED               1
  #if (__SC__  > 0x0810)
     #if __option(bool)
         #define TYPE_BOOL           1
      #else
          #define TYPE_BOOL           0
      #endif
 #else
      #define TYPE_BOOL               0
  #endif
 #if TARGET_RT_MAC_CFM
      #define FUNCTION_PASCAL         0
  #else
      #define FUNCTION_PASCAL         1
  #endif
 #define FUNCTION_DECLSPEC           0
  #define FUNCTION_WIN32CC            0       
   #define SLASH_INCLUDES_UNSUPPORTED  !__option(unix_includes)


#elif defined(__MWERKS__)
 /*
     CodeWarrior compiler from Metrowerks, Inc.
 */
 #if (__MWERKS__ < 0x0900) || macintosh
     #define TARGET_OS_MAC               1
      #define TARGET_OS_WIN32             0
      #define TARGET_OS_UNIX              0
      #define TARGET_CPU_X86              0
      #define TARGET_CPU_MIPS             0
      #define TARGET_CPU_SPARC            0       
       #define TARGET_CPU_ALPHA            0
      #define TARGET_RT_LITTLE_ENDIAN     0
      #define TARGET_RT_BIG_ENDIAN        1
      #if powerc
         #define TARGET_CPU_PPC          1
          #define TARGET_CPU_68K          0
          #define TARGET_RT_MAC_CFM       1
          #define TARGET_RT_MAC_MACHO     0
          #define TARGET_RT_MAC_68881     0
      #else
          #define TARGET_CPU_PPC          0
          #define TARGET_CPU_68K          1
          #if defined(__CFM68K__)
                #define TARGET_RT_MAC_CFM   1
          #else
              #define TARGET_RT_MAC_CFM   0
          #endif
         #define TARGET_RT_MAC_MACHO     0
          #if __MC68881__
                #define TARGET_RT_MAC_68881 1
          #else
              #define TARGET_RT_MAC_68881 0
          #endif
     #endif
     #define PRAGMA_ONCE                 1
      #if (__MWERKS__ >= 0x0700)
         #define PRAGMA_IMPORT           TARGET_RT_MAC_CFM
      #else
          #define PRAGMA_IMPORT           0
      #endif
     #define PRAGMA_STRUCT_ALIGN         1
      #define PRAGMA_STRUCT_PACK          0
      #define PRAGMA_STRUCT_PACKPUSH      0
      #define PRAGMA_ENUM_PACK            0
      #define PRAGMA_ENUM_ALWAYSINT       1
      #define PRAGMA_ENUM_OPTIONS         0
      #if __option(enumsalwaysint) && __option(ANSI_strict)
          #define FOUR_CHAR_CODE(x)       ((long)(x)) /* otherwise compiler will complain about values with high bit set */
      #else
          #define FOUR_CHAR_CODE(x)       (x)
        #endif
     #if TARGET_CPU_68K && !TARGET_RT_MAC_CFM
           #define FUNCTION_PASCAL         1
      #else
          #define FUNCTION_PASCAL         0
      #endif
     #if (__MWERKS__ >= 0x2000)
         #define FUNCTION_DECLSPEC       0
      #else
          #define FUNCTION_DECLSPEC       0
      #endif
     #define FUNCTION_WIN32CC            0       

    
   #elif (__MWERKS__ >= 0x0900) && __INTEL__
      #define TARGET_CPU_PPC              0
      #define TARGET_CPU_68K              0
      #define TARGET_CPU_X86              1
      #define TARGET_CPU_MIPS             0
      #define TARGET_CPU_SPARC            0       
       #define TARGET_CPU_ALPHA            0
      #define TARGET_OS_MAC               0
      #define TARGET_OS_WIN32             1
      #define TARGET_OS_UNIX              0
      #define TARGET_RT_LITTLE_ENDIAN     1
      #define TARGET_RT_BIG_ENDIAN        0
      #define PRAGMA_ONCE                 1
      #define PRAGMA_IMPORT               0
      #define PRAGMA_STRUCT_ALIGN         0
      #define PRAGMA_STRUCT_PACK          1
      #define PRAGMA_STRUCT_PACKPUSH      1
      #define PRAGMA_ENUM_PACK            0
      #define PRAGMA_ENUM_ALWAYSINT       1
      #define PRAGMA_ENUM_OPTIONS         0
      #define FOUR_CHAR_CODE(x)           (x)
        #define FUNCTION_PASCAL             0
      #ifndef FUNCTION_DECLSPEC                   /* allow use of __declspec(dllimport) to be enabled */
         #define FUNCTION_DECLSPEC       0       /* QuickTime for Windows cannot use dllimport */
       #endif
     #ifndef FUNCTION_WIN32CC                    /* allow calling convention to be overriddden */
           #define FUNCTION_WIN32CC        1   
       #endif


   #elif (__MWERKS__ >= 0x1900) && __MIPS__
       #define TARGET_CPU_PPC              0
      #define TARGET_CPU_68K              0
      #define TARGET_CPU_X86              0
      #define TARGET_CPU_MIPS             1
      #define TARGET_CPU_SPARC            0       
       #define TARGET_CPU_ALPHA            0
      #define TARGET_OS_MAC               0
      #define TARGET_OS_WIN32             0
      #define TARGET_OS_UNIX              1
      #if __option(little_endian)
            #define TARGET_RT_LITTLE_ENDIAN 1
          #define TARGET_RT_BIG_ENDIAN    0
      #else
          #define TARGET_RT_LITTLE_ENDIAN 0
          #define TARGET_RT_BIG_ENDIAN    1
      #endif
     #define PRAGMA_ONCE                 1
      #define PRAGMA_IMPORT               0
      #define PRAGMA_STRUCT_ALIGN         0
      #define PRAGMA_STRUCT_PACK          1
      #define PRAGMA_STRUCT_PACKPUSH      0
      #define PRAGMA_ENUM_PACK            0
      #define PRAGMA_ENUM_ALWAYSINT       1
      #define PRAGMA_ENUM_OPTIONS         0
      #define FOUR_CHAR_CODE(x)           (x)
        #define FUNCTION_PASCAL             0
      #define FUNCTION_DECLSPEC           0
      #define FUNCTION_WIN32CC            0           


   #elif (__MWERKS__ >= 0x2110) && __MACH__
       #define TARGET_CPU_PPC              1
      #define TARGET_CPU_68K              0
      #define TARGET_CPU_X86              0
      #define TARGET_CPU_MIPS             0
      #define TARGET_CPU_SPARC            0       
       #define TARGET_CPU_ALPHA            0
      #define TARGET_OS_MAC               1   
       #define TARGET_OS_WIN32             0
      #define TARGET_OS_UNIX              0
      #if __option(little_endian)
            #define TARGET_RT_LITTLE_ENDIAN 1
          #define TARGET_RT_BIG_ENDIAN    0
      #else
          #define TARGET_RT_LITTLE_ENDIAN 0
          #define TARGET_RT_BIG_ENDIAN    1
      #endif
     #define TARGET_RT_MAC_CFM           0
      #define TARGET_RT_MAC_MACHO         1
      #define TARGET_RT_MAC_68881         0
      #define PRAGMA_ONCE                 1
      #define PRAGMA_IMPORT               0
      #define PRAGMA_STRUCT_ALIGN         1
      #define PRAGMA_STRUCT_PACK          1
      #define PRAGMA_STRUCT_PACKPUSH      0
      #define PRAGMA_ENUM_PACK            0
      #define PRAGMA_ENUM_ALWAYSINT       1
      #define PRAGMA_ENUM_OPTIONS         0
      #define FOUR_CHAR_CODE(x)           (x)
        #define FUNCTION_PASCAL             0
      #define FUNCTION_DECLSPEC           0
      #define FUNCTION_WIN32CC            0           
   #else   
       #error unknown Metrowerks compiler
 #endif


   #if (__MWERKS__ >= 0x1100)
     #if __option(longlong)
         #define TYPE_LONGLONG       1
      #else
          #define TYPE_LONGLONG       0
      #endif
 #else
      #define TYPE_LONGLONG           0
  #endif
 #if (__MWERKS__ >= 0x1000)
     #if __option(bool)
         #define TYPE_BOOL           1
      #else
          #define TYPE_BOOL           0
      #endif
 #else
      #define TYPE_BOOL               0
  #endif
 #define TYPE_EXTENDED               0
  #define SLASH_INCLUDES_UNSUPPORTED  1


#elif defined(SYMANTEC_CPLUS) || defined(SYMANTEC_C)
  /*
     C and C++ compiler from Symantec, Inc.
 */
 #define TARGET_OS_MAC               1
  #define TARGET_OS_WIN32             0
  #define TARGET_OS_UNIX              0
  #define TARGET_CPU_X86              0
  #define TARGET_CPU_MIPS             0
  #define TARGET_CPU_SPARC            0       
   #define TARGET_CPU_ALPHA            0
  #define TARGET_RT_LITTLE_ENDIAN     0
  #define TARGET_RT_BIG_ENDIAN        1
  #if powerc
     #define TARGET_CPU_PPC          1
      #define TARGET_CPU_68K          0
      #define TARGET_RT_MAC_CFM       1
      #define TARGET_RT_MAC_MACHO     0
      #define TARGET_RT_MAC_68881     0
  #else
      #define TARGET_CPU_PPC          0
      #define TARGET_CPU_68K          1
      #if defined(__CFM68K)
          #define TARGET_RT_MAC_CFM   1
      #else
          #define TARGET_RT_MAC_CFM   0
      #endif
     #define TARGET_RT_MAC_MACHO     0
      #if mc68881
            #define TARGET_RT_MAC_68881 1
      #else
          #define TARGET_RT_MAC_68881 0
      #endif
 #endif
 #define PRAGMA_IMPORT               0
  #define PRAGMA_ONCE                 1
  #define PRAGMA_STRUCT_ALIGN         1
  #define PRAGMA_STRUCT_PACK          0
  #define PRAGMA_STRUCT_PACKPUSH      0
  #define PRAGMA_ENUM_PACK            1
  #define PRAGMA_ENUM_ALWAYSINT       0
  #define PRAGMA_ENUM_OPTIONS         0
  #define FOUR_CHAR_CODE(x)           (x)
    
   #if __useAppleExts__
       #define TYPE_EXTENDED           1
  #else
      #define TYPE_EXTENDED           0
  #endif
 #define TYPE_LONGLONG               0
  #define TYPE_BOOL                   0
  
   #define FUNCTION_PASCAL             0
  #define FUNCTION_DECLSPEC           0
  #define FUNCTION_WIN32CC            0           
   #define SLASH_INCLUDES_UNSUPPORTED  1


#elif defined(THINK_C)
    /*
     THINK C compiler from Symantec, Inc.        << WARNING: Unsupported Compiler >>
    */
 #define TARGET_CPU_PPC              0
  #define TARGET_CPU_68K              1
  #define TARGET_CPU_X86              0
  #define TARGET_CPU_MIPS             0
  #define TARGET_CPU_SPARC            0       
   #define TARGET_CPU_ALPHA            0
  #define TARGET_OS_MAC               1
  #define TARGET_OS_WIN32             0
  #define TARGET_OS_UNIX              0
  #define TARGET_RT_LITTLE_ENDIAN     0
  #define TARGET_RT_BIG_ENDIAN        1
  #define TARGET_RT_MAC_CFM           0
  #define TARGET_RT_MAC_MACHO         0
  #if defined(mc68881)
       #define TARGET_RT_MAC_68881     1
  #else
      #define TARGET_RT_MAC_68881     0
  #endif
 #define PRAGMA_IMPORT               0
  #define PRAGMA_STRUCT_ALIGN         0
  #define PRAGMA_ONCE                 1
  #define PRAGMA_STRUCT_PACK          0
  #define PRAGMA_STRUCT_PACKPUSH      0
  #define PRAGMA_ENUM_PACK            1
  #define PRAGMA_ENUM_ALWAYSINT       0
  #define PRAGMA_ENUM_OPTIONS         0
  #define FOUR_CHAR_CODE(x)           (x)
    
   #define TYPE_EXTENDED               1
  #define TYPE_LONGLONG               0
  #define TYPE_BOOL                   0
  
   #define FUNCTION_PASCAL             1
  #define FUNCTION_DECLSPEC           0
  #define FUNCTION_WIN32CC            0
  #define SLASH_INCLUDES_UNSUPPORTED  1


#elif defined(__PPCC__)
   /*
     PPCC compiler from Apple Computer, Inc.     << WARNING: Unsupported Compiler >>
    */
 #define TARGET_CPU_PPC              1
  #define TARGET_CPU_68K              0
  #define TARGET_CPU_X86              0
  #define TARGET_CPU_MIPS             0
  #define TARGET_CPU_SPARC            0       
   #define TARGET_CPU_ALPHA            0
  #define TARGET_OS_MAC               1
  #define TARGET_OS_WIN32             0
  #define TARGET_OS_UNIX              0
  #define TARGET_RT_LITTLE_ENDIAN     0
  #define TARGET_RT_BIG_ENDIAN        1
  #define TARGET_RT_MAC_CFM           1
  #define TARGET_RT_MAC_MACHO         0
  #define TARGET_RT_MAC_68881         0
  #define PRAGMA_IMPORT               0
  #define PRAGMA_STRUCT_ALIGN         1
  #define PRAGMA_ONCE                 0
  #define PRAGMA_STRUCT_PACK          0
  #define PRAGMA_STRUCT_PACKPUSH      0
  #define PRAGMA_ENUM_PACK            0
  #define PRAGMA_ENUM_ALWAYSINT       0
  #define PRAGMA_ENUM_OPTIONS         0
  #define FOUR_CHAR_CODE(x)           (x)
    
   #define TYPE_EXTENDED               0
  #define TYPE_LONGLONG               0
  #define TYPE_BOOL                   0
  
   #define FUNCTION_PASCAL             0
  #define FUNCTION_DECLSPEC           0
  #define FUNCTION_WIN32CC            0           
   #define SLASH_INCLUDES_UNSUPPORTED  1


#elif defined(applec) && !defined(__SC__)
 /*
     MPW C compiler from Apple Computer, Inc.    << WARNING: Unsupported Compiler >>
    */
 #define TARGET_CPU_PPC              0
  #define TARGET_CPU_68K              1
  #define TARGET_CPU_X86              0
  #define TARGET_CPU_MIPS             0
  #define TARGET_CPU_SPARC            0       
   #define TARGET_CPU_ALPHA            0
  #define TARGET_OS_MAC               1
  #define TARGET_OS_WIN32             0
  #define TARGET_OS_UNIX              0
  #define TARGET_RT_LITTLE_ENDIAN     0
  #define TARGET_RT_BIG_ENDIAN        1
  #define TARGET_RT_MAC_CFM           0
  #define TARGET_RT_MAC_MACHO         0
  #if defined(mc68881)
       #define TARGET_RT_MAC_68881     1
  #else
      #define TARGET_RT_MAC_68881     0
  #endif
 #define PRAGMA_IMPORT               0
  #define PRAGMA_STRUCT_ALIGN         0
  #define PRAGMA_ONCE                 0
  #define PRAGMA_STRUCT_PACK          0
  #define PRAGMA_STRUCT_PACKPUSH      0
  #define PRAGMA_ENUM_PACK            0
  #define PRAGMA_ENUM_ALWAYSINT       0
  #define PRAGMA_ENUM_OPTIONS         0
  /* Note: MPW C 3.2 had a bug where MACRO('xx   ') would cause 'xx  ' to be misevaluated */
 #define FOUR_CHAR_CODE              

  #define TYPE_EXTENDED               1
  #define TYPE_LONGLONG               0
  #define TYPE_BOOL                   0

 #define FUNCTION_PASCAL             1
  #define FUNCTION_DECLSPEC           0
  #define FUNCTION_WIN32CC            0           
   #define SLASH_INCLUDES_UNSUPPORTED  1


#elif defined(__GNUC__) && (defined(__APPLE_CPP__) || defined(__APPLE_CC__) || defined(__NEXT_CPP__))
 /*
     gcc based compilers used on OpenStep -> Rhapsody -> Mac OS X
   */
 #if defined(__ppc__) || defined(powerpc) || defined(ppc)
       #define TARGET_CPU_PPC          1
      #define TARGET_CPU_68K          0
      #define TARGET_CPU_X86          0
      #define TARGET_CPU_MIPS         0
      #define TARGET_CPU_SPARC        0   
       #define TARGET_CPU_ALPHA        0
      #define TARGET_RT_MAC_CFM       0
      #define TARGET_RT_MAC_MACHO     1
      #define TARGET_RT_MAC_68881     0
      #define TARGET_RT_LITTLE_ENDIAN 0
      #define TARGET_RT_BIG_ENDIAN    1
  #elif defined(m68k)
        #define TARGET_CPU_PPC          0
      #define TARGET_CPU_68K          1
      #define TARGET_CPU_X86          0
      #define TARGET_CPU_MIPS         0
      #define TARGET_CPU_SPARC        0   
       #define TARGET_CPU_ALPHA        0
      #define TARGET_RT_MAC_CFM       0
      #define TARGET_RT_MAC_MACHO     1
      #define TARGET_RT_MAC_68881     0
      #define TARGET_RT_LITTLE_ENDIAN 0
      #define TARGET_RT_BIG_ENDIAN    1
  #elif defined(sparc)
       #define TARGET_CPU_PPC          0
      #define TARGET_CPU_68K          0
      #define TARGET_CPU_X86          0
      #define TARGET_CPU_MIPS         0
      #define TARGET_CPU_SPARC        1
      #define TARGET_CPU_ALPHA        0
      #define TARGET_RT_MAC_CFM       0
      #define TARGET_RT_MAC_MACHO     1
      #define TARGET_RT_MAC_68881     0
      #define TARGET_RT_LITTLE_ENDIAN 0
      #define TARGET_RT_BIG_ENDIAN    1
  #elif defined(__i386__) || defined(i386) || defined(intel)
     #define TARGET_CPU_PPC          0
      #define TARGET_CPU_68K          0
      #define TARGET_CPU_X86          1
      #define TARGET_CPU_MIPS         0
      #define TARGET_CPU_SPARC        0
      #define TARGET_CPU_ALPHA        0
      #define TARGET_RT_MAC_CFM       0
      #define TARGET_RT_MAC_MACHO     1
      #define TARGET_RT_MAC_68881     0
      #define TARGET_RT_LITTLE_ENDIAN 1
      #define TARGET_RT_BIG_ENDIAN    0
  #else
      #error unrecognized GNU C compiler
 #endif


#ifndef TARGET_OS_MAC
 #define TARGET_OS_MAC               1
#endif
#ifndef TARGET_OS_WIN32
   #define TARGET_OS_WIN32             0
#endif
#ifndef TARGET_OS_UNIX
    #define TARGET_OS_UNIX              0
#endif

  #define PRAGMA_IMPORT               0
  #define PRAGMA_STRUCT_ALIGN         1
  #define PRAGMA_ONCE                 0
  #define PRAGMA_STRUCT_PACK          0
  #define PRAGMA_STRUCT_PACKPUSH      0
  #define PRAGMA_ENUM_PACK            0
  #define PRAGMA_ENUM_ALWAYSINT       0
  #define PRAGMA_ENUM_OPTIONS         0
  #define FOUR_CHAR_CODE(x)           (x)

   #define TYPE_EXTENDED               0
  #if __GNUC__ >= 2
      #define TYPE_LONGLONG           1
  #else
      #define TYPE_LONGLONG           0
  #endif
 #ifdef __cplusplus
     #define TYPE_BOOL               1
  #else
      #define TYPE_BOOL               0
  #endif
 
   #define FUNCTION_PASCAL             0
  #define FUNCTION_DECLSPEC           0
  #define FUNCTION_WIN32CC            0           


#elif defined(__GNUC__) && defined(__linux__)
 /*
     gcc (egcs, really) for MkLinux.             << WARNING: Unsupported Compiler >>
    */
 #if #cpu(powerpc)
      #define TARGET_CPU_PPC          1
      #define TARGET_CPU_68K          0
      #define TARGET_CPU_X86          0
      #define TARGET_CPU_MIPS         0
      #define TARGET_CPU_SPARC        0   
       #define TARGET_CPU_ALPHA        0
      #define TARGET_RT_MAC_CFM       1
      #define TARGET_RT_MAC_MACHO     0
      #define TARGET_RT_MAC_68881     0
  #elif #cpu(m68k)
       #define TARGET_CPU_PPC          0
      #define TARGET_CPU_68K          1
      #define TARGET_CPU_X86          0
      #define TARGET_CPU_MIPS         0
      #define TARGET_CPU_SPARC        0   
       #define TARGET_CPU_ALPHA        0
      #define TARGET_RT_MAC_CFM       0
      #define TARGET_RT_MAC_MACHO     0
      #define TARGET_RT_MAC_68881     0
  #else
      #error unsupported GNU C compiler
  #endif
 
   #if #system(macos)
     #define TARGET_OS_MAC           1
      #define TARGET_OS_WIN32         0
      #define TARGET_OS_UNIX          0
  #elif #system(unix)
        #define TARGET_OS_MAC           0
      #define TARGET_OS_WIN32         0
      #define TARGET_OS_UNIX          1
  #else
      #error unsupported GNU C compiler
  #endif


   
   #define TARGET_RT_LITTLE_ENDIAN     0
  #define TARGET_RT_BIG_ENDIAN        1
  #define PRAGMA_IMPORT               0
  #define PRAGMA_STRUCT_ALIGN         0
  #define PRAGMA_ONCE                 0
  #define PRAGMA_STRUCT_PACK          1
  #define PRAGMA_STRUCT_PACKPUSH      0
  #define PRAGMA_ENUM_PACK            0
  #define PRAGMA_ENUM_ALWAYSINT       0
  #define PRAGMA_ENUM_OPTIONS         0
  #define FOUR_CHAR_CODE(x)           (x)

   #define TYPE_EXTENDED               0
  #ifdef _LONG_LONG
      #define TYPE_LONGLONG           1
  #else
      #define TYPE_LONGLONG           0
  #endif
 #define TYPE_BOOL                   0
  
   #define FUNCTION_PASCAL             0
  #define FUNCTION_DECLSPEC           0
  #define FUNCTION_WIN32CC            0           


#elif defined(__GNUC__) && defined(__MINGW32__)
   /*
     Mingw gnu gcc/egcs compiler for Win32 systems (http://www.mingw.org).
  */
 #define TARGET_CPU_PPC              0
  #define TARGET_CPU_68K              0
  #define TARGET_CPU_X86              1
  #define TARGET_CPU_MIPS             0
  #define TARGET_CPU_SPARC            0   
   #define TARGET_CPU_ALPHA            0
  #define TARGET_RT_MAC_CFM           0
  #define TARGET_RT_MAC_MACHO         0
  #define TARGET_RT_MAC_68881         0
  #define TARGET_OS_MAC               0
  #define TARGET_OS_WIN32             1
  #define TARGET_OS_UNIX              0
  #define TARGET_RT_LITTLE_ENDIAN     1
  #define TARGET_RT_BIG_ENDIAN        0
  #define PRAGMA_IMPORT               0
  #define PRAGMA_STRUCT_ALIGN         0
  #define PRAGMA_ONCE                 0
  #define PRAGMA_STRUCT_PACK          1
  #define PRAGMA_STRUCT_PACKPUSH      0
  #define PRAGMA_ENUM_PACK            0
  #define PRAGMA_ENUM_ALWAYSINT       0
  #define PRAGMA_ENUM_OPTIONS         0
  #define FOUR_CHAR_CODE(x)           (x)
    #define TYPE_EXTENDED               0
  #define TYPE_LONGLONG               1
  #define TYPE_BOOL                   1
  #define FUNCTION_PASCAL             0
  #define FUNCTION_DECLSPEC           0
  #define FUNCTION_WIN32CC            0           


#elif defined(__GNUC__)
   /*
     gC for MPW from Free Software Foundation, Inc.
 */
 #if #cpu(powerpc)
      #define TARGET_CPU_PPC          1
      #define TARGET_CPU_68K          0
      #define TARGET_CPU_X86          0
      #define TARGET_CPU_MIPS         0
      #define TARGET_CPU_SPARC        0   
       #define TARGET_CPU_ALPHA        0
      #define TARGET_RT_MAC_CFM       1
      #define TARGET_RT_MAC_MACHO     0
      #define TARGET_RT_MAC_68881     0
  #elif #cpu(m68k)
       #define TARGET_CPU_PPC          0
      #define TARGET_CPU_68K          1
      #define TARGET_CPU_X86          0
      #define TARGET_CPU_MIPS         0
      #define TARGET_CPU_SPARC        0   
       #define TARGET_CPU_ALPHA        0
      #define TARGET_RT_MAC_CFM       0
      #define TARGET_RT_MAC_MACHO     0
      #define TARGET_RT_MAC_68881     0
  #else
      #error unsupported GNU C compiler
  #endif
 
   #if #system(macos)
     #define TARGET_OS_MAC           1
      #define TARGET_OS_WIN32         0
      #define TARGET_OS_UNIX          0
  #elif #system(unix)
        #define TARGET_OS_MAC           0
      #define TARGET_OS_WIN32         0
      #define TARGET_OS_UNIX          1
  #else
      #error unsupported GNU C compiler
  #endif
 
   #define TARGET_RT_LITTLE_ENDIAN     0
  #define TARGET_RT_BIG_ENDIAN        1
  #define PRAGMA_IMPORT               0
  #define PRAGMA_STRUCT_ALIGN         1
  #define PRAGMA_ONCE                 0
  #define PRAGMA_STRUCT_PACK          0
  #define PRAGMA_STRUCT_PACKPUSH      0
  #define PRAGMA_ENUM_PACK            0
  #define PRAGMA_ENUM_ALWAYSINT       0
  #define PRAGMA_ENUM_OPTIONS         0
  #define FOUR_CHAR_CODE(x)           (x)

   #define TYPE_EXTENDED               0
  #ifdef _LONG_LONG
      #define TYPE_LONGLONG           1
  #else
      #define TYPE_LONGLONG           0
  #endif
 #define TYPE_BOOL                   0

 #define FUNCTION_PASCAL             0
  #define FUNCTION_DECLSPEC           0
  #define FUNCTION_WIN32CC            0           


#elif defined(__xlc) || defined(__xlC) || defined(__xlC__) || defined(__XLC121__)
 /*
     xlc and xlC on RS/6000 from IBM, Inc.
  */
 #define TARGET_CPU_PPC              1
  #define TARGET_CPU_68K              0
  #define TARGET_CPU_X86              0
  #define TARGET_CPU_MIPS             0
  #define TARGET_CPU_SPARC            0       
   #define TARGET_CPU_ALPHA            0
  #if defined(_AIX)
      #define TARGET_OS_MAC           0
      #define TARGET_OS_UNIX          1
  #else
      #define TARGET_OS_MAC           1
      #define TARGET_OS_UNIX          0
  #endif
 #define TARGET_OS_WIN32             0
  #define TARGET_RT_LITTLE_ENDIAN     0
  #define TARGET_RT_BIG_ENDIAN        1
  #define TARGET_RT_MAC_CFM           1
  #define TARGET_RT_MAC_MACHO         0
  #define TARGET_RT_MAC_68881         0
  #define PRAGMA_IMPORT               0
  #define PRAGMA_STRUCT_ALIGN         1
  #define PRAGMA_ONCE                 0
  #define PRAGMA_STRUCT_PACK          0
  #define PRAGMA_STRUCT_PACKPUSH      0
  #define PRAGMA_ENUM_PACK            0
  #define PRAGMA_ENUM_ALWAYSINT       0
  #define PRAGMA_ENUM_OPTIONS         1
  #define FOUR_CHAR_CODE(x)           (x)

   #define TYPE_EXTENDED               0
  #ifdef _LONG_LONG
      #define TYPE_LONGLONG           1
  #else
      #define TYPE_LONGLONG           0
  #endif
 #define TYPE_BOOL                   0
  
   #define FUNCTION_PASCAL             0
  #define FUNCTION_DECLSPEC           0
  #define FUNCTION_WIN32CC            0           


#elif defined(_MSC_VER) && !defined(__MWERKS__) 
  /*
     Visual Studio C/C++ from Microsoft, Inc.
   */
 #if defined(_M_M68K)    /* Visual C++ with Macintosh 68K target */
     #define TARGET_CPU_PPC              0
      #define TARGET_CPU_68K              1
      #define TARGET_CPU_X86              0
      #define TARGET_CPU_MIPS             0
      #define TARGET_CPU_SPARC            0
      #define TARGET_CPU_ALPHA            0
      #define TARGET_OS_MAC               1
      #define TARGET_OS_WIN32             0
      #define TARGET_OS_UNIX              0
      #define TARGET_RT_LITTLE_ENDIAN     0
      #define TARGET_RT_BIG_ENDIAN        1
      #define TARGET_RT_MAC_CFM           0
      #define TARGET_RT_MAC_MACHO         0
      #define TARGET_RT_MAC_68881         0
      #define PRAGMA_IMPORT               0
      #define PRAGMA_STRUCT_ALIGN         1
      #define PRAGMA_ONCE                 0
      #define PRAGMA_STRUCT_PACK          1
      #define PRAGMA_STRUCT_PACKPUSH      1
      #define PRAGMA_ENUM_PACK            0
      #define PRAGMA_ENUM_ALWAYSINT       0
      #define PRAGMA_ENUM_OPTIONS         0
      #define FOUR_CHAR_CODE(x)           (x)
        #define TYPE_EXTENDED               0
      #define TYPE_LONGLONG               0
      #define TYPE_BOOL                   0
      #define FUNCTION_PASCAL             1
      #define FUNCTION_DECLSPEC           0
      #define FUNCTION_WIN32CC            0           

    #elif defined(_M_MPPC)  /* Visual C++ with Macintosh PowerPC target */
     #define TARGET_CPU_PPC              1
      #define TARGET_CPU_68K              0
      #define TARGET_CPU_X86              0
      #define TARGET_CPU_MIPS             0
      #define TARGET_CPU_SPARC            0
      #define TARGET_CPU_ALPHA            0
      #define TARGET_OS_MAC               1
      #define TARGET_OS_WIN32             0
      #define TARGET_OS_UNIX              0
      #define TARGET_RT_LITTLE_ENDIAN     0
      #define TARGET_RT_BIG_ENDIAN        1
      #define TARGET_RT_MAC_CFM           1
      #define TARGET_RT_MAC_MACHO         0
      #define TARGET_RT_MAC_68881         0
      #define PRAGMA_IMPORT               0
      #define PRAGMA_STRUCT_ALIGN         1
      #define PRAGMA_ONCE                 0
      #define PRAGMA_STRUCT_PACK          1
      #define PRAGMA_STRUCT_PACKPUSH      1
      #define PRAGMA_ENUM_PACK            0
      #define PRAGMA_ENUM_ALWAYSINT       0
      #define PRAGMA_ENUM_OPTIONS         0
      #define FOUR_CHAR_CODE(x)           (x)
        #define TYPE_EXTENDED               0
      #define TYPE_LONGLONG               0
      #define TYPE_BOOL                   0
      #define FUNCTION_PASCAL             0
      #define FUNCTION_DECLSPEC           0   
       #define FUNCTION_WIN32CC            0   

    #elif defined(_M_IX86)  /* Visual Studio with Intel x86 target */
      #define TARGET_CPU_PPC              0
      #define TARGET_CPU_68K              0
      #define TARGET_CPU_X86              1
      #define TARGET_CPU_MIPS             0
      #define TARGET_CPU_SPARC            0
      #define TARGET_CPU_ALPHA            0
      #define TARGET_OS_MAC               0
      #define TARGET_OS_WIN32             1
      #define TARGET_OS_UNIX              0
      #define TARGET_RT_LITTLE_ENDIAN     1
      #define TARGET_RT_BIG_ENDIAN        0
      #define PRAGMA_IMPORT               0
      #define PRAGMA_STRUCT_ALIGN         0
      #define PRAGMA_ONCE                 0
      #define PRAGMA_STRUCT_PACK          1
      #define PRAGMA_STRUCT_PACKPUSH      1
      #define PRAGMA_ENUM_PACK            0
      #define PRAGMA_ENUM_ALWAYSINT       0
      #define PRAGMA_ENUM_OPTIONS         0
      #define FOUR_CHAR_CODE(x)           (x) 
       #define TYPE_EXTENDED               0
      #define TYPE_LONGLONG               1   /* note: uses __int64 instead of long long */
      #define LONGLONG_TYPENAME           __int64
        #define LONGLONG_SIGNED_MAX         (9223372036854775807i64)
       #define LONGLONG_SIGNED_MIN         (-9223372036854775807i64 - 1)
      #define LONGLONG_UNSIGNED_MAX       (0xffffffffffffffffui64)
       #if defined(__cplusplus) && (_MSC_VER >= 1100)
         #define TYPE_BOOL               1
      #else
          #define TYPE_BOOL               0
      #endif
     #define FUNCTION_PASCAL             0
      #ifndef FUNCTION_DECLSPEC               /* allow use of __declspec(dllimport) to be enabled */
         #define FUNCTION_DECLSPEC       0   /* QuickTime for Windows cannot use dllimport */
       #endif
     #ifndef FUNCTION_WIN32CC                /* allow calling convention to be overriddden */
           #define FUNCTION_WIN32CC        1   
       #endif
     /* Warning:  This macros away the pascal word in source code. */
       /* Very useful for code that needs to compile on Mac 68k and Windows */
        /* but can silently change code */
     #undef pascal
      #define pascal

        #elif defined(_M_ALPHA)     /* Visual C++ with Dec Alpha target */
     #define TARGET_CPU_PPC              0
      #define TARGET_CPU_68K              0
      #define TARGET_CPU_X86              0
      #define TARGET_CPU_MIPS             0
      #define TARGET_CPU_SPARC            0
      #define TARGET_CPU_ALPHA            1
      #define TARGET_OS_MAC               0
      #define TARGET_OS_WIN32             1
      #define TARGET_OS_UNIX              0
      #define TARGET_RT_LITTLE_ENDIAN     1
      #define TARGET_RT_BIG_ENDIAN        0
      #define PRAGMA_IMPORT               0
      #define PRAGMA_STRUCT_ALIGN         0
      #define PRAGMA_ONCE                 0
      #define PRAGMA_STRUCT_PACK          1
      #define PRAGMA_STRUCT_PACKPUSH      1
      #define PRAGMA_ENUM_PACK            0
      #define PRAGMA_ENUM_ALWAYSINT       0
      #define PRAGMA_ENUM_OPTIONS         0
      #define FOUR_CHAR_CODE(x)       (((unsigned long) ((x) & 0x000000FF)) << 24) \
                                 | (((unsigned long) ((x) & 0x0000FF00)) << 8) \
                                    | (((unsigned long) ((x) & 0x00FF0000)) >> 8) \
                                    | (((unsigned long) ((x) & 0xFF000000)) >> 24)
     #define TYPE_EXTENDED               0
      #define TYPE_LONGLONG               0
      #define TYPE_BOOL                   0
      #define FUNCTION_PASCAL             0
      #define FUNCTION_DECLSPEC           0   
       #define FUNCTION_WIN32CC            0   

    #elif defined(_M_PPC)   /* Visual C++ for Windows NT on PowerPC target */
      #define TARGET_CPU_PPC              1
      #define TARGET_CPU_68K              0
      #define TARGET_CPU_X86              0
      #define TARGET_CPU_MIPS             0
      #define TARGET_CPU_SPARC            0
      #define TARGET_CPU_ALPHA            0
      #define TARGET_OS_MAC               0
      #define TARGET_OS_WIN32             1
      #define TARGET_OS_UNIX              0
      #define TARGET_RT_LITTLE_ENDIAN     1
      #define TARGET_RT_BIG_ENDIAN        0
      #define PRAGMA_IMPORT               0
      #define PRAGMA_STRUCT_ALIGN         0
      #define PRAGMA_ONCE                 0
      #define PRAGMA_STRUCT_PACK          1
      #define PRAGMA_STRUCT_PACKPUSH      1
      #define PRAGMA_ENUM_PACK            0
      #define PRAGMA_ENUM_ALWAYSINT       0
      #define PRAGMA_ENUM_OPTIONS         0
      #define FOUR_CHAR_CODE(x)       (((unsigned long) ((x) & 0x000000FF)) << 24) \
                                 | (((unsigned long) ((x) & 0x0000FF00)) << 8) \
                                    | (((unsigned long) ((x) & 0x00FF0000)) >> 8) \
                                    | (((unsigned long) ((x) & 0xFF000000)) >> 24)
     #define TYPE_EXTENDED               0
      #define TYPE_LONGLONG               0
      #define TYPE_BOOL                   0
      #define FUNCTION_PASCAL             0
      #define FUNCTION_DECLSPEC           0   
       #define FUNCTION_WIN32CC            0   

        #elif defined(_M_MRX000)    /* Visual C++ for Windows NT on MIPS target */
     #define TARGET_CPU_PPC              0
      #define TARGET_CPU_68K              0
      #define TARGET_CPU_X86              0
      #define TARGET_CPU_MIPS             1
      #define TARGET_CPU_SPARC            0
      #define TARGET_CPU_ALPHA            0
      #define TARGET_OS_MAC               0
      #define TARGET_OS_WIN32             1
      #define TARGET_OS_UNIX              0
      #define TARGET_RT_LITTLE_ENDIAN     1
      #define TARGET_RT_BIG_ENDIAN        0
      #define PRAGMA_IMPORT               0
      #define PRAGMA_STRUCT_ALIGN         0
      #define PRAGMA_ONCE                 0
      #define PRAGMA_STRUCT_PACK          1
      #define PRAGMA_STRUCT_PACKPUSH      1
      #define PRAGMA_ENUM_PACK            0
      #define PRAGMA_ENUM_ALWAYSINT       0
      #define PRAGMA_ENUM_OPTIONS         0
      #define FOUR_CHAR_CODE(x)       (((unsigned long) ((x) & 0x000000FF)) << 24) \
                                 | (((unsigned long) ((x) & 0x0000FF00)) << 8) \
                                    | (((unsigned long) ((x) & 0x00FF0000)) >> 8) \
                                    | (((unsigned long) ((x) & 0xFF000000)) >> 24)
     #define TYPE_EXTENDED               0
      #define TYPE_LONGLONG               0
      #define TYPE_BOOL                   0
      #define FUNCTION_PASCAL             0
      #define FUNCTION_DECLSPEC           0   
       #define FUNCTION_WIN32CC            0   
   #endif


#elif defined(__MOTO__)
   /*
     mcc from Motorola, Inc. 
   */
 #define TARGET_CPU_PPC              1
  #define TARGET_CPU_68K              0
  #define TARGET_CPU_X86              0
  #define TARGET_CPU_MIPS             0
  #define TARGET_CPU_SPARC            0       
   #define TARGET_CPU_ALPHA            0
  #define TARGET_OS_MAC               1
  #define TARGET_OS_WIN32             0
  #define TARGET_OS_UNIX              0
  #define TARGET_RT_LITTLE_ENDIAN     0
  #define TARGET_RT_BIG_ENDIAN        1
  #define TARGET_RT_MAC_CFM           1
  #define TARGET_RT_MAC_MACHO         0
  #define TARGET_RT_MAC_68881         0
  #define PRAGMA_IMPORT               0           /* how is this detected ?? */
  #define PRAGMA_STRUCT_ALIGN         1
  #if __MOTO__ >= 40702                           /* MCC version 4.7.2 */
      #define PRAGMA_ONCE               1
  #else
    #define PRAGMA_ONCE               0
  #endif
 #define PRAGMA_STRUCT_PACK          0
  #define PRAGMA_STRUCT_PACKPUSH      0
  #define PRAGMA_ENUM_PACK            0
  #define PRAGMA_ENUM_ALWAYSINT       0
  #define PRAGMA_ENUM_OPTIONS         0
  #define FOUR_CHAR_CODE(x)           (x)
    #define TYPE_LONGLONG               0           /* how is this detected ?? */
  #ifdef _BOOL
       #define TYPE_BOOL               1           
   #else
      #define TYPE_BOOL               0           
   #endif
 #define TYPE_EXTENDED               0
  #define FUNCTION_PASCAL             0
  #define FUNCTION_DECLSPEC           0   
   #define FUNCTION_WIN32CC            0   



#elif defined(_MIPS_ISA)
  /*
     MIPSpro compiler from Silicon Graphics Inc.
    */
 #define TARGET_CPU_PPC              0
  #define TARGET_CPU_68K              0
  #define TARGET_CPU_X86              0
  #define TARGET_CPU_MIPS             1
  #define TARGET_CPU_SPARC            0       
   #define TARGET_CPU_ALPHA            0
  #define TARGET_OS_MAC               0
  #define TARGET_OS_WIN32             0
  #define TARGET_OS_UNIX              1
  #define TARGET_RT_LITTLE_ENDIAN     0
  #define TARGET_RT_BIG_ENDIAN        1
  #define PRAGMA_IMPORT               0
  #define PRAGMA_STRUCT_ALIGN         0
  #define PRAGMA_ONCE                 0
  #define PRAGMA_STRUCT_PACK          1
  #define PRAGMA_STRUCT_PACKPUSH      0
  #define PRAGMA_ENUM_PACK            0
  #define PRAGMA_ENUM_ALWAYSINT       0
  #define PRAGMA_ENUM_OPTIONS         0
  #define FOUR_CHAR_CODE(x)           (x)
    #define TYPE_EXTENDED               0
  #define TYPE_LONGLONG               0
  #define TYPE_BOOL                   0
  #define FUNCTION_PASCAL             0
  #define FUNCTION_DECLSPEC           0   
   #define FUNCTION_WIN32CC            0   


#elif defined(__sparc)
    /*
     SPARCompiler compiler from Sun Microsystems Inc.
   */
 #define TARGET_CPU_PPC              0
  #define TARGET_CPU_68K              0
  #define TARGET_CPU_X86              0
  #define TARGET_CPU_MIPS             0
  #define TARGET_CPU_SPARC            1       
   #define TARGET_CPU_ALPHA            0
  #define TARGET_OS_MAC               0
  #define TARGET_OS_WIN32             0
  #define TARGET_OS_UNIX              1
  #define TARGET_RT_LITTLE_ENDIAN     0
  #define TARGET_RT_BIG_ENDIAN        1
  #define PRAGMA_IMPORT               0
  #define PRAGMA_STRUCT_ALIGN         0
  #define PRAGMA_ONCE                 0
  #define PRAGMA_STRUCT_PACK          1
  #define PRAGMA_STRUCT_PACKPUSH      0
  #define PRAGMA_ENUM_PACK            0
  #define PRAGMA_ENUM_ALWAYSINT       0
  #define PRAGMA_ENUM_OPTIONS         0
  #define FOUR_CHAR_CODE(x)       (((unsigned long) ((x) & 0x000000FF)) << 24) \
                                 | (((unsigned long) ((x) & 0x0000FF00)) << 8) \
                                    | (((unsigned long) ((x) & 0x00FF0000)) >> 8) \
                                    | (((unsigned long) ((x) & 0xFF000000)) >> 24)
 #define TYPE_EXTENDED               0
  #define TYPE_LONGLONG               0
  #define TYPE_BOOL                   0
  #define FUNCTION_PASCAL             0
  #define FUNCTION_DECLSPEC           0   
   #define FUNCTION_WIN32CC            0   


#else
 /*
     Unknown compiler, perhaps set up from the command line (e.g. -d TARGET_CPU_MIPS , etc.)
    */
 #if defined(TARGET_CPU_PPC) && TARGET_CPU_PPC
      #define TARGET_CPU_68K   0
     #define TARGET_CPU_X86   0
     #define TARGET_CPU_MIPS  0
     #define TARGET_CPU_SPARC 0
     #define TARGET_CPU_ALPHA 0
 #elif defined(TARGET_CPU_68K) && TARGET_CPU_68K
        #define TARGET_CPU_PPC   0
     #define TARGET_CPU_X86   0
     #define TARGET_CPU_MIPS  0
     #define TARGET_CPU_SPARC 0
     #define TARGET_CPU_ALPHA 0
 #elif defined(TARGET_CPU_X86) && TARGET_CPU_X86
        #define TARGET_CPU_PPC   0
     #define TARGET_CPU_68K   0
     #define TARGET_CPU_MIPS  0
     #define TARGET_CPU_SPARC 0
     #define TARGET_CPU_ALPHA 0
 #elif defined(TARGET_CPU_MIPS) && TARGET_CPU_MIPS
      #define TARGET_CPU_PPC   0
     #define TARGET_CPU_68K   0
     #define TARGET_CPU_X86   0
     #define TARGET_CPU_SPARC 0
     #define TARGET_CPU_ALPHA 0
 #elif defined(TARGET_CPU_SPARC) && TARGET_CPU_SPARC
        #define TARGET_CPU_PPC   0
     #define TARGET_CPU_68K   0
     #define TARGET_CPU_X86   0
     #define TARGET_CPU_MIPS  0
     #define TARGET_CPU_ALPHA 0
 #elif defined(TARGET_CPU_ALPHA) && TARGET_CPU_ALPHA
        #define TARGET_CPU_PPC   0
     #define TARGET_CPU_68K   0
     #define TARGET_CPU_X86   0
     #define TARGET_CPU_MIPS  0
     #define TARGET_CPU_SPARC 0

        
   #else
      /*
         NOTE:   If your compiler errors out here then support for your compiler 
                   has not yet been added to ConditionalMacros.h.  
                   
                   ConditionalMacros.h is designed to be plug-and-play.  It auto detects
                  which compiler is being run and configures the TARGET_ conditionals
                    appropriately.  
                   
                   The short term work around is to set the TARGET_CPU_ and TARGET_OS_
                    on the command line to the compiler (e.g. d TARGET_CPU_MIPS -d TARGET_OS_UNIX)
                 
                   The long term solution is to add a new case to this file which
                 auto detects your compiler and sets up the TARGET_ conditionals.
                   If you do this, send the changes you made to devsupport@apple.com
                  to get it integrated into the next release of ConditionalMacros.h.
     */
     #error ConditionalMacros.h: unknown compiler (see comment above)
       #define TARGET_CPU_PPC   0
     #define TARGET_CPU_68K   0
     #define TARGET_CPU_X86   0
     #define TARGET_CPU_MIPS  0
     #define TARGET_CPU_SPARC 0
     #define TARGET_CPU_ALPHA 0
 #endif
 


   #if defined(TARGET_OS_MAC) && TARGET_OS_MAC
        #define TARGET_OS_WIN32  0
     #define TARGET_OS_UNIX   0
 #elif defined(TARGET_OS_WIN32) && TARGET_OS_WIN32
      #define TARGET_OS_MAC    0
     #define TARGET_OS_UNIX   0
 #elif defined(TARGET_OS_UNIX) && TARGET_OS_UNIX
        #define TARGET_OS_MAC    0
     #define TARGET_OS_WIN32  0
 #elif TARGET_CPU_PPC || TARGET_CPU_68K
     #define TARGET_OS_MAC    1
     #define TARGET_OS_WIN32  0
     #define TARGET_OS_UNIX   0
 #else
      #error ConditionalMacros.h: unknown target OS (see comment above)
  #endif

    #if !defined(TARGET_RT_BIG_ENDIAN) && !defined(TARGET_RT_LITTLE_ENDIAN)
        #if TARGET_OS_MAC
          #define TARGET_RT_LITTLE_ENDIAN     0
          #define TARGET_RT_BIG_ENDIAN        1
      #elif TARGET_OS_WIN32
          #define TARGET_RT_LITTLE_ENDIAN     1
          #define TARGET_RT_BIG_ENDIAN        0
      #endif
 #endif
     
   #if defined(TARGET_RT_BIG_ENDIAN) && !defined(TARGET_RT_LITTLE_ENDIAN)
     #define TARGET_RT_LITTLE_ENDIAN     !TARGET_RT_BIG_ENDIAN
  #elif !defined(TARGET_RT_BIG_ENDIAN) && defined(TARGET_RT_LITTLE_ENDIAN)
       #define TARGET_RT_BIG_ENDIAN        !TARGET_RT_LITTLE_ENDIAN
   #endif
 
   #if !defined(TARGET_RT_BIG_ENDIAN) && !defined(TARGET_RT_LITTLE_ENDIAN)
        #error unknown endianess of target processor
   #endif

    #if TARGET_OS_MAC
  #ifndef TARGET_RT_MAC_CFM
  #define TARGET_RT_MAC_CFM           TARGET_CPU_PPC
 #endif
 #ifndef TARGET_RT_MAC_68881
    #define TARGET_RT_MAC_68881         0
  #endif
     #ifndef TARGET_RT_MAC_MACHO
        #define TARGET_RT_MAC_MACHO         !TARGET_RT_MAC_CFM
     #endif      
   #endif


        
   #ifndef PRAGMA_IMPORT
  #define PRAGMA_IMPORT               0
  #endif
 #ifndef PRAGMA_STRUCT_ALIGN
    #define PRAGMA_STRUCT_ALIGN         0
  #endif
 #ifndef PRAGMA_ONCE
    #define PRAGMA_ONCE                 0
  #endif
 #ifndef PRAGMA_STRUCT_PACK
 #define PRAGMA_STRUCT_PACK          0
  #endif
 #ifndef PRAGMA_STRUCT_PACKPUSH
 #define PRAGMA_STRUCT_PACKPUSH      0
  #endif
 #ifndef PRAGMA_ENUM_PACK
   #define PRAGMA_ENUM_PACK            0
  #endif
 #ifndef PRAGMA_ENUM_ALWAYSINT
  #define PRAGMA_ENUM_ALWAYSINT       0
  #endif
 #ifndef PRAGMA_ENUM_OPTIONS
    #define PRAGMA_ENUM_OPTIONS         0
  #endif
 #ifndef FOUR_CHAR_CODE
 #define FOUR_CHAR_CODE(x)           (x)
    #endif

    #ifndef TYPE_EXTENDED
  #define TYPE_EXTENDED               0
  #endif
 #ifndef TYPE_LONGLONG
  #define TYPE_LONGLONG               0
  #endif
 #ifndef TYPE_BOOL
  #define TYPE_BOOL                   0
  #endif
 #ifndef FUNCTION_PASCAL
    #define FUNCTION_PASCAL             0
  #endif
 #ifndef FUNCTION_DECLSPEC
  #define FUNCTION_DECLSPEC           0
  #endif
 #ifndef FUNCTION_WIN32CC
   #define FUNCTION_WIN32CC            0
  #endif
#endif




/****************************************************************************************************

    Under MacOS, the classic 68k runtime has two calling conventions: pascal or C
    Under Win32, there are two calling conventions: __cdecl or __stdcall
    Headers and implementation files can use the following macros to make their
    source more portable by hiding the calling convention details:

    EXTERN_API* 
    These macros are used to specify the calling convention on a function prototype.

        EXTERN_API              - Classic 68k: pascal, Win32: __cdecl
        EXTERN_API_C            - Classic 68k: C,      Win32: __cdecl
        EXTERN_API_STDCALL      - Classic 68k: pascal, Win32: __stdcall
        EXTERN_API_C_STDCALL    - Classic 68k: C,      Win32: __stdcall


    DEFINE_API* 
    These macros are used to specify the calling convention on a function definition.

        DEFINE_API              - Classic 68k: pascal, Win32: __cdecl
        DEFINE_API_C            - Classic 68k: C,      Win32: __cdecl
        DEFINE_API_STDCALL      - Classic 68k: pascal, Win32: __stdcall
        DEFINE_API_C_STDCALL    - Classic 68k: C,      Win32: __stdcall


    CALLBACK_API*   
    These macros are used to specify the calling convention of a function pointer.

        CALLBACK_API            - Classic 68k: pascal, Win32: __stdcall
        CALLBACK_API_C          - Classic 68k: C,      Win32: __stdcall
        CALLBACK_API_STDCALL    - Classic 68k: pascal, Win32: __cdecl
        CALLBACK_API_C_STDCALL  - Classic 68k: C,      Win32: __cdecl

****************************************************************************************************/

#if FUNCTION_PASCAL && !FUNCTION_DECLSPEC && !FUNCTION_WIN32CC
    /* compiler supports pascal keyword only  */
    #define EXTERN_API(_type)                       extern pascal _type
    #define EXTERN_API_C(_type)                     extern        _type
    #define EXTERN_API_STDCALL(_type)               extern pascal _type
    #define EXTERN_API_C_STDCALL(_type)             extern        _type
    
    #define DEFINE_API(_type)                       pascal _type
    #define DEFINE_API_C(_type)                            _type
    #define DEFINE_API_STDCALL(_type)               pascal _type
    #define DEFINE_API_C_STDCALL(_type)                    _type
    
    #define CALLBACK_API(_type, _name)              pascal _type (*_name)
    #define CALLBACK_API_C(_type, _name)                   _type (*_name)
    #define CALLBACK_API_STDCALL(_type, _name)      pascal _type (*_name)
    #define CALLBACK_API_C_STDCALL(_type, _name)           _type (*_name)

#elif FUNCTION_PASCAL && FUNCTION_DECLSPEC && !FUNCTION_WIN32CC
    /* compiler supports pascal and __declspec() */
    #define EXTERN_API(_type)                       extern pascal __declspec(dllimport) _type
    #define EXTERN_API_C(_type)                     extern        __declspec(dllimport) _type
    #define EXTERN_API_STDCALL(_type)               extern pascal __declspec(dllimport) _type
    #define EXTERN_API_C_STDCALL(_type)             extern        __declspec(dllimport) _type
    
    #define DEFINE_API(_type)                       pascal __declspec(dllexport) _type
    #define DEFINE_API_C(_type)                            __declspec(dllexport) _type
    #define DEFINE_API_STDCALL(_type)               pascal __declspec(dllexport) _type
    #define DEFINE_API_C_STDCALL(_type)                    __declspec(dllexport) _type

    #define CALLBACK_API(_type, _name)              pascal _type (*_name)
    #define CALLBACK_API_C(_type, _name)                   _type (*_name)
    #define CALLBACK_API_STDCALL(_type, _name)      pascal _type (*_name)
    #define CALLBACK_API_C_STDCALL(_type, _name)           _type (*_name)

#elif !FUNCTION_PASCAL && FUNCTION_DECLSPEC && !FUNCTION_WIN32CC
    /* compiler supports __declspec() */
    #define EXTERN_API(_type)                       extern __declspec(dllimport) _type
    #define EXTERN_API_C(_type)                     extern __declspec(dllimport) _type
    #define EXTERN_API_STDCALL(_type)               extern __declspec(dllimport) _type
    #define EXTERN_API_C_STDCALL(_type)             extern __declspec(dllimport) _type
    
    #define DEFINE_API(_type)                       __declspec(dllexport) _type 
    #define DEFINE_API_C(_type)                     __declspec(dllexport) _type
    #define DEFINE_API_STDCALL(_type)               __declspec(dllexport) _type
    #define DEFINE_API_C_STDCALL(_type)             __declspec(dllexport) _type

    #define CALLBACK_API(_type, _name)              _type ( * _name)
    #define CALLBACK_API_C(_type, _name)            _type ( * _name)
    #define CALLBACK_API_STDCALL(_type, _name)      _type ( * _name)
    #define CALLBACK_API_C_STDCALL(_type, _name)    _type ( * _name)

#elif !FUNCTION_PASCAL && FUNCTION_DECLSPEC && FUNCTION_WIN32CC
    /* compiler supports __declspec() and __cdecl */
    #define EXTERN_API(_type)                       __declspec(dllimport) _type __cdecl
    #define EXTERN_API_C(_type)                     __declspec(dllimport) _type __cdecl
    #define EXTERN_API_STDCALL(_type)               __declspec(dllimport) _type __stdcall
    #define EXTERN_API_C_STDCALL(_type)             __declspec(dllimport) _type __stdcall
    
    #define DEFINE_API(_type)                       __declspec(dllexport) _type __cdecl
    #define DEFINE_API_C(_type)                     __declspec(dllexport) _type __cdecl
    #define DEFINE_API_STDCALL(_type)               __declspec(dllexport) _type __stdcall
    #define DEFINE_API_C_STDCALL(_type)             __declspec(dllexport) _type __stdcall
    
    #define CALLBACK_API(_type, _name)              _type (__cdecl * _name)
    #define CALLBACK_API_C(_type, _name)            _type (__cdecl * _name)
    #define CALLBACK_API_STDCALL(_type, _name)      _type (__stdcall * _name)
    #define CALLBACK_API_C_STDCALL(_type, _name)    _type (__stdcall * _name)

#elif !FUNCTION_PASCAL && !FUNCTION_DECLSPEC && FUNCTION_WIN32CC
    /* compiler supports __cdecl */
    #define EXTERN_API(_type)                       _type __cdecl
    #define EXTERN_API_C(_type)                     _type __cdecl
    #define EXTERN_API_STDCALL(_type)               _type __stdcall
    #define EXTERN_API_C_STDCALL(_type)             _type __stdcall
    
    #define DEFINE_API(_type)                       _type __cdecl
    #define DEFINE_API_C(_type)                     _type __cdecl
    #define DEFINE_API_STDCALL(_type)               _type __stdcall
    #define DEFINE_API_C_STDCALL(_type)             _type __stdcall
    
    #define CALLBACK_API(_type, _name)              _type (__cdecl * _name)
    #define CALLBACK_API_C(_type, _name)            _type (__cdecl * _name)
    #define CALLBACK_API_STDCALL(_type, _name)      _type (__stdcall * _name)
    #define CALLBACK_API_C_STDCALL(_type, _name)    _type (__stdcall * _name)

#else 
    /* compiler supports no extensions */
    #define EXTERN_API(_type)                       extern _type
    #define EXTERN_API_C(_type)                     extern _type
    #define EXTERN_API_STDCALL(_type)               extern _type
    #define EXTERN_API_C_STDCALL(_type)             extern _type
    
    #define DEFINE_API(_type)                       _type
    #define DEFINE_API_C(_type)                     _type
    #define DEFINE_API_STDCALL(_type)               _type
    #define DEFINE_API_C_STDCALL(_type)             _type

    #define CALLBACK_API(_type, _name)              _type ( * _name)
    #define CALLBACK_API_C(_type, _name)            _type ( * _name)
    #define CALLBACK_API_STDCALL(_type, _name)      _type ( * _name)
    #define CALLBACK_API_C_STDCALL(_type, _name)    _type ( * _name)
    #undef pascal
    #define pascal
#endif

/****************************************************************************************************
    
    Set up TARGET_API_*_* values

****************************************************************************************************/
#if TARGET_OS_MAC
#if !defined(TARGET_API_MAC_OS8) && !defined(TARGET_API_MAC_OSX) && !defined(TARGET_API_MAC_CARBON)
/* No TARGET_API_MAC_* predefind on command line */
#if defined(TARGET_CARBON) && TARGET_CARBON
/* grandfather in use of TARGET_CARBON */
#define TARGET_API_MAC_OS8 0
#define TARGET_API_MAC_CARBON 1
#define TARGET_API_MAC_OSX 0
#elif TARGET_CPU_PPC && TARGET_RT_MAC_CFM
/* Looks like CFM style PPC compiler */
#define TARGET_API_MAC_OS8 1
#define TARGET_API_MAC_CARBON 0
#define TARGET_API_MAC_OSX 0
#elif TARGET_RT_MAC_MACHO
/* Looks like MachO style PPC compiler */
#define TARGET_API_MAC_OS8 0
#define TARGET_API_MAC_CARBON 0
#define TARGET_API_MAC_OSX 1
#else
/* 68k or some other compiler */
#define TARGET_API_MAC_OS8 1
#define TARGET_API_MAC_CARBON 0
#define TARGET_API_MAC_OSX 0
#endif  /*  */

#else
#ifndef TARGET_API_MAC_OS8
#define TARGET_API_MAC_OS8 0
#endif  /* !defined(TARGET_API_MAC_OS8) */

#ifndef TARGET_API_MAC_CARBON
#define TARGET_API_MAC_CARBON 0
#endif  /* !defined(TARGET_API_MAC_CARBON) */

#ifndef TARGET_API_MAC_OSX
#define TARGET_API_MAC_OSX 0
#endif  /* !defined(TARGET_API_MAC_OSX) */

#endif  /* !defined(TARGET_API_MAC_OS8) && !defined(TARGET_API_MAC_OSX) && !defined(TARGET_API_MAC_CARBON) */

#if TARGET_API_MAC_OS8 && TARGET_API_MAC_OSX
#error TARGET_API_MAC_OS8 and TARGET_API_MAC_OSX are mutually exclusive
#endif  /* TARGET_API_MAC_OS8 && TARGET_API_MAC_OSX */

#if !TARGET_API_MAC_OS8 && !TARGET_API_MAC_CARBON && !TARGET_API_MAC_OSX
#error At least one of TARGET_API_MAC_* must be true
#endif  /* !TARGET_API_MAC_OS8 && !TARGET_API_MAC_CARBON && !TARGET_API_MAC_OSX */

#else
#define TARGET_API_MAC_OS8 0
#define TARGET_API_MAC_CARBON 0
#define TARGET_API_MAC_OSX 0
#endif  /* TARGET_OS_MAC */

/* Support source code still using TARGET_CARBON */
#ifndef TARGET_CARBON
#if TARGET_API_MAC_CARBON && !TARGET_API_MAC_OS8
#define TARGET_CARBON 1
#else
#define TARGET_CARBON 0
#endif  /* TARGET_API_MAC_CARBON && !TARGET_API_MAC_OS8 */

#endif  /* !defined(TARGET_CARBON) */

/* Set forCarbon to 0 if it's not already defined */
#ifndef forCarbon
#define forCarbon 0
#endif  /* !defined(forCarbon) */

/****************************************************************************************************
    Backward compatibility for clients expecting 2.x version on ConditionalMacros.h

    GENERATINGPOWERPC       - Compiler is generating PowerPC instructions
    GENERATING68K           - Compiler is generating 68k family instructions
    GENERATING68881         - Compiler is generating mc68881 floating point instructions
    GENERATINGCFM           - Code being generated assumes CFM calling conventions
    CFMSYSTEMCALLS          - No A-traps.  Systems calls are made using CFM and UPP's
    PRAGMA_ALIGN_SUPPORTED  - Compiler supports: #pragma options align=mac68k/power/reset
    PRAGMA_IMPORT_SUPPORTED - Compiler supports: #pragma import on/off/reset
    CGLUESUPPORTED          - Clients can use all lowercase toolbox functions that take C strings instead of pascal strings

****************************************************************************************************/
#if TARGET_API_MAC_OS8 || !TARGET_OS_MAC
#define GENERATINGPOWERPC TARGET_CPU_PPC
#define GENERATING68K TARGET_CPU_68K
#define GENERATING68881 TARGET_RT_MAC_68881
#define GENERATINGCFM TARGET_RT_MAC_CFM
#define CFMSYSTEMCALLS TARGET_RT_MAC_CFM
#ifndef CGLUESUPPORTED
#define CGLUESUPPORTED 0
#endif  /* !defined(CGLUESUPPORTED) */

#ifndef OLDROUTINELOCATIONS
#define OLDROUTINELOCATIONS 0
#endif  /* !defined(OLDROUTINELOCATIONS) */

#define PRAGMA_ALIGN_SUPPORTED  PRAGMA_STRUCT_ALIGN
#define PRAGMA_IMPORT_SUPPORTED PRAGMA_IMPORT
#else
/* Carbon code should not use old conditionals */
#define PRAGMA_ALIGN_SUPPORTED  ..PRAGMA_ALIGN_SUPPORTED_is_obsolete..
#define GENERATINGPOWERPC       ..GENERATINGPOWERPC_is_obsolete..
#define GENERATING68K           ..GENERATING68K_is_obsolete..
#define GENERATING68881         ..GENERATING68881_is_obsolete..
#define GENERATINGCFM           ..GENERATINGCFM_is_obsolete..
#define CFMSYSTEMCALLS          ..CFMSYSTEMCALLS_is_obsolete..
#endif  /* TARGET_API_MAC_OS8 || !TARGET_OS_MAC */



/****************************************************************************************************

    OLDROUTINENAMES         - "Old" names for Macintosh system calls are allowed in source code.
                              (e.g. DisposPtr instead of DisposePtr). The names of system routine
                              are now more sensitive to change because CFM binds by name.  In the 
                              past, system routine names were compiled out to just an A-Trap.  
                              Macros have been added that each map an old name to its new name.  
                              This allows old routine names to be used in existing source files,
                              but the macros only work if OLDROUTINENAMES is true.  This support
                              will be removed in the near future.  Thus, all source code should 
                              be changed to use the new names! You can set OLDROUTINENAMES to false
                              to see if your code has any old names left in it.
    
****************************************************************************************************/
#ifndef OLDROUTINENAMES
#define OLDROUTINENAMES 0
#endif  /* !defined(OLDROUTINENAMES) */



/****************************************************************************************************
 The following macros isolate the use of 68K inlines in function prototypes.
    On the Mac OS under the Classic 68K runtime, function prototypes were followed
 by a list of 68K opcodes which the compiler inserted in the generated code instead
 of a JSR.  Under Classic 68K on the Mac OS, this macro will put the opcodes
    in the right syntax.  For all other OS's and runtimes the macro suppress the opcodes.
  Example:
   
       EXTERN_P void DrawPicture(PicHandle myPicture, const Rect *dstRect)
            ONEWORDINLINE(0xA8F6);
 
****************************************************************************************************/

#if TARGET_OS_MAC && TARGET_CPU_68K && !TARGET_RT_MAC_CFM
 #define ONEWORDINLINE(w1) = w1
 #define TWOWORDINLINE(w1,w2) = {w1,w2}
 #define THREEWORDINLINE(w1,w2,w3) = {w1,w2,w3}
 #define FOURWORDINLINE(w1,w2,w3,w4)  = {w1,w2,w3,w4}
   #define FIVEWORDINLINE(w1,w2,w3,w4,w5) = {w1,w2,w3,w4,w5}
  #define SIXWORDINLINE(w1,w2,w3,w4,w5,w6)     = {w1,w2,w3,w4,w5,w6}
 #define SEVENWORDINLINE(w1,w2,w3,w4,w5,w6,w7)    = {w1,w2,w3,w4,w5,w6,w7}
  #define EIGHTWORDINLINE(w1,w2,w3,w4,w5,w6,w7,w8)     = {w1,w2,w3,w4,w5,w6,w7,w8}
   #define NINEWORDINLINE(w1,w2,w3,w4,w5,w6,w7,w8,w9)   = {w1,w2,w3,w4,w5,w6,w7,w8,w9}
    #define TENWORDINLINE(w1,w2,w3,w4,w5,w6,w7,w8,w9,w10)  = {w1,w2,w3,w4,w5,w6,w7,w8,w9,w10}
  #define ELEVENWORDINLINE(w1,w2,w3,w4,w5,w6,w7,w8,w9,w10,w11)     = {w1,w2,w3,w4,w5,w6,w7,w8,w9,w10,w11}
    #define TWELVEWORDINLINE(w1,w2,w3,w4,w5,w6,w7,w8,w9,w10,w11,w12)     = {w1,w2,w3,w4,w5,w6,w7,w8,w9,w10,w11,w12}
#else
  #define ONEWORDINLINE(w1)
  #define TWOWORDINLINE(w1,w2)
   #define THREEWORDINLINE(w1,w2,w3)
  #define FOURWORDINLINE(w1,w2,w3,w4)
    #define FIVEWORDINLINE(w1,w2,w3,w4,w5)
 #define SIXWORDINLINE(w1,w2,w3,w4,w5,w6)
   #define SEVENWORDINLINE(w1,w2,w3,w4,w5,w6,w7)
  #define EIGHTWORDINLINE(w1,w2,w3,w4,w5,w6,w7,w8)
   #define NINEWORDINLINE(w1,w2,w3,w4,w5,w6,w7,w8,w9)
 #define TENWORDINLINE(w1,w2,w3,w4,w5,w6,w7,w8,w9,w10)
  #define ELEVENWORDINLINE(w1,w2,w3,w4,w5,w6,w7,w8,w9,w10,w11)
   #define TWELVEWORDINLINE(w1,w2,w3,w4,w5,w6,w7,w8,w9,w10,w11,w12)
#endif


/****************************************************************************************************

    TARGET_CARBON                   - default: false. Switches all of the above as described.  Overrides all others
                                    - NOTE: If you set TARGET_CARBON to 1, then the other switches will be setup by
                                            ConditionalMacros, and should not be set manually.

    If you wish to do development for pre-Carbon Systems, you can set the following:

    OPAQUE_TOOLBOX_STRUCTS          - default: false. True for Carbon builds, hides struct fields.
    OPAQUE_UPP_TYPES                - default: false. True for Carbon builds, UPP types are unique and opaque.
    ACCESSOR_CALLS_ARE_FUNCTIONS    - default: false. True for Carbon builds, enables accessor functions.
    CALL_NOT_IN_CARBON              - default: true.  False for Carbon builds, hides calls not supported in Carbon.
    
    Specifically, if you are building a non-Carbon application (one that links against InterfaceLib)
    but you wish to use some of the accessor functions, you can set ACCESSOR_CALLS_ARE_FUNCTIONS to 1
    and link with CarbonAccessors.o, which implements just the accessor functions. This will help you
    preserve source compatibility between your Carbon and non-Carbon application targets.
    
    MIXEDMODE_CALLS_ARE_FUNCTIONS   - deprecated.

****************************************************************************************************/
#if TARGET_API_MAC_CARBON && !TARGET_API_MAC_OS8
#ifndef OPAQUE_TOOLBOX_STRUCTS
#define OPAQUE_TOOLBOX_STRUCTS 1
#endif  /* !defined(OPAQUE_TOOLBOX_STRUCTS) */

#ifndef OPAQUE_UPP_TYPES
#define OPAQUE_UPP_TYPES 1
#endif  /* !defined(OPAQUE_UPP_TYPES) */

#ifndef ACCESSOR_CALLS_ARE_FUNCTIONS
#define ACCESSOR_CALLS_ARE_FUNCTIONS 1
#endif  /* !defined(ACCESSOR_CALLS_ARE_FUNCTIONS) */

#ifndef CALL_NOT_IN_CARBON
#define CALL_NOT_IN_CARBON 0
#endif  /* !defined(CALL_NOT_IN_CARBON) */

#ifndef MIXEDMODE_CALLS_ARE_FUNCTIONS
#define MIXEDMODE_CALLS_ARE_FUNCTIONS 1
#endif  /* !defined(MIXEDMODE_CALLS_ARE_FUNCTIONS) */

#else
#ifndef OPAQUE_TOOLBOX_STRUCTS
#define OPAQUE_TOOLBOX_STRUCTS 0
#endif  /* !defined(OPAQUE_TOOLBOX_STRUCTS) */

#ifndef OPAQUE_UPP_TYPES
#define OPAQUE_UPP_TYPES 0
#endif  /* !defined(OPAQUE_UPP_TYPES) */

#ifndef ACCESSOR_CALLS_ARE_FUNCTIONS
#define ACCESSOR_CALLS_ARE_FUNCTIONS 0
#endif  /* !defined(ACCESSOR_CALLS_ARE_FUNCTIONS) */

/*
     * It's possible to have ACCESSOR_CALLS_ARE_FUNCTIONS set to true and OPAQUE_TOOLBOX_STRUCTS
     * set to false, but not the other way around, so make sure the defines are not set this way.
     */
#if OPAQUE_TOOLBOX_STRUCTS && !ACCESSOR_CALLS_ARE_FUNCTIONS
#error OPAQUE_TOOLBOX_STRUCTS cannot be true when ACCESSOR_CALLS_ARE_FUNCTIONS is false
#endif  /* OPAQUE_TOOLBOX_STRUCTS && !ACCESSOR_CALLS_ARE_FUNCTIONS */

#ifndef CALL_NOT_IN_CARBON
#define CALL_NOT_IN_CARBON 1
#endif  /* !defined(CALL_NOT_IN_CARBON) */

#ifndef MIXEDMODE_CALLS_ARE_FUNCTIONS
#define MIXEDMODE_CALLS_ARE_FUNCTIONS 0
#endif  /* !defined(MIXEDMODE_CALLS_ARE_FUNCTIONS) */

#endif  /* TARGET_API_MAC_CARBON && !TARGET_API_MAC_OS8 */


#if TARGET_RT_MAC_MACHO && TARGET_RT_LITTLE_ENDIAN
#define OLD_COMPONENT_GLUE 1
#endif 




#endif /* __CONDITIONALMACROS__ */

