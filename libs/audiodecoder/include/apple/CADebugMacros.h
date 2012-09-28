/*	Copyright © 2007 Apple Inc. All Rights Reserved.
	
	Disclaimer: IMPORTANT:  This Apple software is supplied to you by 
			Apple Inc. ("Apple") in consideration of your agreement to the
			following terms, and your use, installation, modification or
			redistribution of this Apple software constitutes acceptance of these
			terms.  If you do not agree with these terms, please do not use,
			install, modify or redistribute this Apple software.
			
			In consideration of your agreement to abide by the following terms, and
			subject to these terms, Apple grants you a personal, non-exclusive
			license, under Apple's copyrights in this original Apple software (the
			"Apple Software"), to use, reproduce, modify and redistribute the Apple
			Software, with or without modifications, in source and/or binary forms;
			provided that if you redistribute the Apple Software in its entirety and
			without modifications, you must retain this notice and the following
			text and disclaimers in all such redistributions of the Apple Software. 
			Neither the name, trademarks, service marks or logos of Apple Inc. 
			may be used to endorse or promote products derived from the Apple
			Software without specific prior written permission from Apple.  Except
			as expressly stated in this notice, no other rights or licenses, express
			or implied, are granted by Apple herein, including but not limited to
			any patent rights that may be infringed by your derivative works or by
			other works in which the Apple Software may be incorporated.
			
			The Apple Software is provided by Apple on an "AS IS" basis.  APPLE
			MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION
			THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS
			FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND
			OPERATION ALONE OR IN COMBINATION WITH YOUR PRODUCTS.
			
			IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL
			OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
			SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
			INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION,
			MODIFICATION AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED
			AND WHETHER UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE),
			STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE
			POSSIBILITY OF SUCH DAMAGE.
*/
/*=============================================================================
	CADebugMacros.h

=============================================================================*/
#if !defined(__CADebugMacros_h__)
#define __CADebugMacros_h__

//=============================================================================
//	Includes
//=============================================================================

#if !defined(__COREAUDIO_USE_FLAT_INCLUDES__)
	#include <CoreAudio/CoreAudioTypes.h>
#else
	#include "CoreAudioTypes.h"
#endif

//=============================================================================
//	CADebugMacros
//=============================================================================

//#define	CoreAudio_StopOnFailure			1
//#define	CoreAudio_TimeStampMessages		1
//#define	CoreAudio_ThreadStampMessages	1
//#define	CoreAudio_FlushDebugMessages	1

#if TARGET_RT_BIG_ENDIAN
	#define	CA4CCToCString(the4CC)					{ ((char*)&the4CC)[0], ((char*)&the4CC)[1], ((char*)&the4CC)[2], ((char*)&the4CC)[3], 0 }
	#define CACopy4CCToCString(theCString, the4CC)	{ theCString[0] = ((char*)&the4CC)[0]; theCString[1] = ((char*)&the4CC)[1]; theCString[2] = ((char*)&the4CC)[2]; theCString[3] = ((char*)&the4CC)[3]; theCString[4] = 0; }
#else
	#define	CA4CCToCString(the4CC)					{ ((char*)&the4CC)[3], ((char*)&the4CC)[2], ((char*)&the4CC)[1], ((char*)&the4CC)[0], 0 }
	#define CACopy4CCToCString(theCString, the4CC)	{ theCString[0] = ((char*)&the4CC)[3]; theCString[1] = ((char*)&the4CC)[2]; theCString[2] = ((char*)&the4CC)[1]; theCString[3] = ((char*)&the4CC)[0]; theCString[4] = 0; }
#endif

#pragma mark	Basic Definitions

#if	DEBUG || CoreAudio_Debug
	
	// can be used to break into debugger immediately, also see CADebugger
	#define BusError()		(*(long *)0 = 0)
	
	//	basic debugging print routines
	#if	TARGET_OS_MAC && !TARGET_API_MAC_CARBON
		extern pascal void DebugStr(const unsigned char* debuggerMsg);
		#define	DebugMessage(msg)	DebugStr("\p"msg)
		#define DebugMessageN1(msg, N1)
		#define DebugMessageN2(msg, N1, N2)
		#define DebugMessageN3(msg, N1, N2, N3)
	#else
		#include "CADebugPrintf.h"
		
		#if	(CoreAudio_FlushDebugMessages && !CoreAudio_UseSysLog) || defined(CoreAudio_UseSideFile)
			#define	FlushRtn	;fflush(DebugPrintfFile)
		#else
			#define	FlushRtn
		#endif
		
		#if		CoreAudio_ThreadStampMessages
			#include <pthread.h>
			#include "CAHostTimeBase.h"
			#define	DebugMessage(msg)										DebugPrintfRtn(DebugPrintfFileComma "%p %.4f: %s"DebugPrintfLineEnding, pthread_self(), ((Float64)(CAHostTimeBase::GetCurrentTimeInNanos()) / 1000000.0), msg) FlushRtn
			#define DebugMessageN1(msg, N1)									DebugPrintfRtn(DebugPrintfFileComma "%p %.4f: "msg"\n", pthread_self(), ((Float64)(CAHostTimeBase::GetCurrentTimeInNanos()) / 1000000.0), N1) FlushRtn
			#define DebugMessageN2(msg, N1, N2)								DebugPrintfRtn(DebugPrintfFileComma "%p %.4f: "msg"\n", pthread_self(), ((Float64)(CAHostTimeBase::GetCurrentTimeInNanos()) / 1000000.0), N1, N2) FlushRtn
			#define DebugMessageN3(msg, N1, N2, N3)							DebugPrintfRtn(DebugPrintfFileComma "%p %.4f: "msg"\n", pthread_self(), ((Float64)(CAHostTimeBase::GetCurrentTimeInNanos()) / 1000000.0), N1, N2, N3) FlushRtn
			#define DebugMessageN4(msg, N1, N2, N3, N4)						DebugPrintfRtn(DebugPrintfFileComma "%p %.4f: "msg"\n", pthread_self(), ((Float64)(CAHostTimeBase::GetCurrentTimeInNanos()) / 1000000.0), N1, N2, N3, N4) FlushRtn
			#define DebugMessageN5(msg, N1, N2, N3, N4, N5)					DebugPrintfRtn(DebugPrintfFileComma "%p %.4f: "msg"\n", pthread_self(), ((Float64)(CAHostTimeBase::GetCurrentTimeInNanos()) / 1000000.0), N1, N2, N3, N4, N5) FlushRtn
			#define DebugMessageN6(msg, N1, N2, N3, N4, N5, N6)				DebugPrintfRtn(DebugPrintfFileComma "%p %.4f: "msg"\n", pthread_self(), ((Float64)(CAHostTimeBase::GetCurrentTimeInNanos()) / 1000000.0), N1, N2, N3, N4, N5, N6) FlushRtn
			#define DebugMessageN7(msg, N1, N2, N3, N4, N5, N6, N7)			DebugPrintfRtn(DebugPrintfFileComma "%p %.4f: "msg"\n", pthread_self(), ((Float64)(CAHostTimeBase::GetCurrentTimeInNanos()) / 1000000.0), N1, N2, N3, N4, N5, N6, N7) FlushRtn
			#define DebugMessageN8(msg, N1, N2, N3, N4, N5, N6, N7, N8)		DebugPrintfRtn(DebugPrintfFileComma "%p %.4f: "msg"\n", pthread_self(), ((Float64)(CAHostTimeBase::GetCurrentTimeInNanos()) / 1000000.0), N1, N2, N3, N4, N5, N6, N7, N8) FlushRtn
			#define DebugMessageN9(msg, N1, N2, N3, N4, N5, N6, N7, N8, N9)	DebugPrintfRtn(DebugPrintfFileComma "%p %.4f: "msg"\n", pthread_self(), ((Float64)(CAHostTimeBase::GetCurrentTimeInNanos()) / 1000000.0), N1, N2, N3, N4, N5, N6, N7, N8, N9) FlushRtn
		#elif	CoreAudio_TimeStampMessages
			#include "CAHostTimeBase.h"
			#define	DebugMessage(msg)										DebugPrintfRtn(DebugPrintfFileComma "%.4f: %s"DebugPrintfLineEnding, pthread_self(), ((Float64)(CAHostTimeBase::GetCurrentTimeInNanos()) / 1000000.0), msg) FlushRtn
			#define DebugMessageN1(msg, N1)									DebugPrintfRtn(DebugPrintfFileComma "%.4f: "msg DebugPrintfLineEnding, ((Float64)(CAHostTimeBase::GetCurrentTimeInNanos()) / 1000000.0), N1) FlushRtn
			#define DebugMessageN2(msg, N1, N2)								DebugPrintfRtn(DebugPrintfFileComma "%.4f: "msg DebugPrintfLineEnding, ((Float64)(CAHostTimeBase::GetCurrentTimeInNanos()) / 1000000.0), N1, N2) FlushRtn
			#define DebugMessageN3(msg, N1, N2, N3)							DebugPrintfRtn(DebugPrintfFileComma "%.4f: "msg DebugPrintfLineEnding, ((Float64)(CAHostTimeBase::GetCurrentTimeInNanos()) / 1000000.0), N1, N2, N3) FlushRtn
			#define DebugMessageN4(msg, N1, N2, N3, N4)						DebugPrintfRtn(DebugPrintfFileComma "%.4f: "msg DebugPrintfLineEnding, ((Float64)(CAHostTimeBase::GetCurrentTimeInNanos()) / 1000000.0), N1, N2, N3, N4) FlushRtn
			#define DebugMessageN5(msg, N1, N2, N3, N4, N5)					DebugPrintfRtn(DebugPrintfFileComma "%.4f: "msg DebugPrintfLineEnding, ((Float64)(CAHostTimeBase::GetCurrentTimeInNanos()) / 1000000.0), N1, N2, N3, N4, N5) FlushRtn
			#define DebugMessageN6(msg, N1, N2, N3, N4, N5, N6)				DebugPrintfRtn(DebugPrintfFileComma "%.4f: "msg DebugPrintfLineEnding, ((Float64)(CAHostTimeBase::GetCurrentTimeInNanos()) / 1000000.0), N1, N2, N3, N4, N5, N6) FlushRtn
			#define DebugMessageN7(msg, N1, N2, N3, N4, N5, N6, N7)			DebugPrintfRtn(DebugPrintfFileComma "%.4f: "msg DebugPrintfLineEnding, ((Float64)(CAHostTimeBase::GetCurrentTimeInNanos()) / 1000000.0), N1, N2, N3, N4, N5, N6, N7) FlushRtn
			#define DebugMessageN8(msg, N1, N2, N3, N4, N5, N6, N7, N8)		DebugPrintfRtn(DebugPrintfFileComma "%.4f: "msg DebugPrintfLineEnding, ((Float64)(CAHostTimeBase::GetCurrentTimeInNanos()) / 1000000.0), N1, N2, N3, N4, N5, N6, N7, N8) FlushRtn
			#define DebugMessageN9(msg, N1, N2, N3, N4, N5, N6, N7, N8, N9)	DebugPrintfRtn(DebugPrintfFileComma "%.4f: "msg DebugPrintfLineEnding, ((Float64)(CAHostTimeBase::GetCurrentTimeInNanos()) / 1000000.0), N1, N2, N3, N4, N5, N6, N7, N8, N9) FlushRtn
		#else
			#define	DebugMessage(msg)										DebugPrintfRtn(DebugPrintfFileComma "%s"DebugPrintfLineEnding, msg) FlushRtn
			#define DebugMessageN1(msg, N1)									DebugPrintfRtn(DebugPrintfFileComma msg DebugPrintfLineEnding, N1) FlushRtn
			#define DebugMessageN2(msg, N1, N2)								DebugPrintfRtn(DebugPrintfFileComma msg DebugPrintfLineEnding, N1, N2) FlushRtn
			#define DebugMessageN3(msg, N1, N2, N3)							DebugPrintfRtn(DebugPrintfFileComma msg DebugPrintfLineEnding, N1, N2, N3) FlushRtn
			#define DebugMessageN4(msg, N1, N2, N3, N4)						DebugPrintfRtn(DebugPrintfFileComma msg DebugPrintfLineEnding, N1, N2, N3, N4) FlushRtn
			#define DebugMessageN5(msg, N1, N2, N3, N4, N5)					DebugPrintfRtn(DebugPrintfFileComma msg DebugPrintfLineEnding, N1, N2, N3, N4, N5) FlushRtn
			#define DebugMessageN6(msg, N1, N2, N3, N4, N5, N6)				DebugPrintfRtn(DebugPrintfFileComma msg DebugPrintfLineEnding, N1, N2, N3, N4, N5, N6) FlushRtn
			#define DebugMessageN7(msg, N1, N2, N3, N4, N5, N6, N7)			DebugPrintfRtn(DebugPrintfFileComma msg DebugPrintfLineEnding, N1, N2, N3, N4, N5, N6, N7) FlushRtn
			#define DebugMessageN8(msg, N1, N2, N3, N4, N5, N6, N7, N8)		DebugPrintfRtn(DebugPrintfFileComma msg DebugPrintfLineEnding, N1, N2, N3, N4, N5, N6, N7, N8) FlushRtn
			#define DebugMessageN9(msg, N1, N2, N3, N4, N5, N6, N7, N8, N9)	DebugPrintfRtn(DebugPrintfFileComma msg DebugPrintfLineEnding, N1, N2, N3, N4, N5, N6, N7, N8, N9) FlushRtn
		#endif
	#endif
	void	DebugPrint(const char *fmt, ...);	// can be used like printf
	#define DEBUGPRINT(msg) DebugPrint msg		// have to double-parenthesize arglist (see Debugging.h)
	#if VERBOSE
		#define vprint(msg) DEBUGPRINT(msg)
	#else
		#define vprint(msg)
	#endif
	
	#if	CoreAudio_StopOnFailure
		#include "CADebugger.h"
		#define STOP	CADebuggerStop()
	#else
		#define	STOP
	#endif

#else
	#define	DebugMessage(msg)
	#define DebugMessageN1(msg, N1)
	#define DebugMessageN2(msg, N1, N2)
	#define DebugMessageN3(msg, N1, N2, N3)
	#define DebugMessageN4(msg, N1, N2, N3, N4)
	#define DebugMessageN5(msg, N1, N2, N3, N4, N5)
	#define DebugMessageN6(msg, N1, N2, N3, N4, N5, N6)
	#define DebugMessageN7(msg, N1, N2, N3, N4, N5, N6, N7)
	#define DebugMessageN8(msg, N1, N2, N3, N4, N5, N6, N7, N8)
	#define DebugMessageN9(msg, N1, N2, N3, N4, N5, N6, N7, N8, N9)
	#define DEBUGPRINT(msg)
	#define vprint(msg)
	#define	STOP
#endif

void	LogError(const char *fmt, ...);			// writes to syslog (and stderr if debugging)
void	LogWarning(const char *fmt, ...);		// writes to syslog (and stderr if debugging)

#if	DEBUG || CoreAudio_Debug

#pragma mark	Debug Macros

#define	Assert(inCondition, inMessage)													\
			if(!(inCondition))															\
			{																			\
				DebugMessage(inMessage);												\
				STOP;																	\
			}

#define	AssertNoError(inError, inMessage)												\
			{																			\
				SInt32 __Err = (inError);												\
				if(__Err != 0)															\
				{																		\
					char __4CC[5] = CA4CCToCString(__Err);								\
					DebugMessageN2(inMessage ", Error: %d (%s)", (int)__Err, __4CC);		\
					STOP;																\
				}																		\
			}

#define	AssertNoKernelError(inError, inMessage)											\
			{																			\
				unsigned int __Err = (unsigned int)(inError);							\
				if(__Err != 0)															\
				{																		\
					DebugMessageN1(inMessage ", Error: 0x%X", __Err);					\
					STOP;																\
				}																		\
			}

#define	FailIf(inCondition, inHandler, inMessage)										\
			if(inCondition)																\
			{																			\
				DebugMessage(inMessage);												\
				STOP;																	\
				goto inHandler;															\
			}

#define	FailWithAction(inCondition, inAction, inHandler, inMessage)						\
			if(inCondition)																\
			{																			\
				DebugMessage(inMessage);												\
				STOP;																	\
				{ inAction; }															\
				goto inHandler;															\
			}

#define	FailIfNULL(inPointer, inAction, inHandler, inMessage)							\
			if((inPointer) == NULL)														\
			{																			\
				DebugMessage(inMessage);												\
				STOP;																	\
				{ inAction; }															\
				goto inHandler;															\
			}

#define	FailIfKernelError(inKernelError, inException, inMessage)						\
			{																			\
				kern_return_t __Err = (inKernelError);									\
				if(__Err != 0)															\
				{																		\
					DebugMessageN1(inMessage ", Error: 0x%X", __Err);					\
					STOP;																\
					{ inAction; }														\
					goto inHandler;														\
				}																		\
			}

#define	FailIfError(inError, inException, inMessage)									\
			{																			\
				SInt32 __Err = (inError);												\
				if(__Err != 0)															\
				{																		\
					char __4CC[5] = CA4CCToCString(__Err);								\
					DebugMessageN2(inMessage ", Error: %ld (%s)", __Err, __4CC);		\
					STOP;																\
					{ inAction; }														\
					goto inHandler;														\
				}																		\
			}

#if defined(__cplusplus)

#define Throw(inException)  STOP; throw (inException)

#define	ThrowIf(inCondition, inException, inMessage)									\
			if(inCondition)																\
			{																			\
				DebugMessage(inMessage);												\
				Throw(inException);														\
			}

#define	ThrowIfNULL(inPointer, inException, inMessage)									\
			if((inPointer) == NULL)														\
			{																			\
				DebugMessage(inMessage);												\
				Throw(inException);														\
			}

#define	ThrowIfKernelError(inKernelError, inException, inMessage)						\
			{																			\
				kern_return_t __Err = (inKernelError);									\
				if(__Err != 0)															\
				{																		\
					DebugMessageN1(inMessage ", Error: 0x%X", __Err);					\
					Throw(inException);													\
				}																		\
			}

#define	ThrowIfError(inError, inException, inMessage)									\
			{																			\
				SInt32 __Err = (inError);												\
				if(__Err != 0)															\
				{																		\
					char __4CC[5] = CA4CCToCString(__Err);								\
					DebugMessageN2(inMessage ", Error: %d (%s)", (int)__Err, __4CC);		\
					Throw(inException);													\
				}																		\
			}

#if TARGET_OS_WIN32
#define	ThrowIfWinError(inError, inException, inMessage)								\
			{																			\
				HRESULT __Err = (inError);												\
				if(FAILED(__Err))														\
				{																		\
					DebugMessageN2(inMessage ", Code: %d, Facility: 0x%X", HRESULT_CODE(__Err), HRESULT_FACILITY(__Err));			\
					Throw(inException);													\
				}																		\
			}
#endif

#define	SubclassResponsibility(inMethodName, inException)								\
			{																			\
				DebugMessage(inMethodName": Subclasses must implement this method");	\
				Throw(inException);														\
			}

#endif	//	defined(__cplusplus)

#else

#pragma mark	Release Macros

#define	Assert(inCondition, inMessage)													\
			if(!(inCondition))															\
			{																			\
				STOP;																	\
			}

#define	AssertNoError(inError, inMessage)												\
			{																			\
				SInt32 __Err = (inError);												\
				if(__Err != 0)															\
				{																		\
					STOP;																\
				}																		\
			}

#define	AssertNoKernelError(inError, inMessage)											\
			{																			\
				unsigned int __Err = (unsigned int)(inError);							\
				if(__Err != 0)															\
				{																		\
					STOP;																\
				}																		\
			}

#define	FailIf(inCondition, inHandler, inMessage)										\
			if(inCondition)																\
			{																			\
				STOP;																	\
				goto inHandler;															\
			}

#define	FailWithAction(inCondition, inAction, inHandler, inMessage)						\
			if(inCondition)																\
			{																			\
				STOP;																	\
				{ inAction; }															\
				goto inHandler;															\
			}

#define	FailIfNULL(inPointer, inAction, inHandler, inMessage)							\
			if((inPointer) == NULL)														\
			{																			\
				STOP;																	\
				{ inAction; }															\
				goto inHandler;															\
			}

#define	FailIfKernelError(inKernelError, inException, inMessage)						\
			if((inKernelError) != 0)													\
			{																			\
				STOP;																	\
				{ inAction; }															\
				goto inHandler;															\
			}

#define	FailIfError(inError, inException, inMessage)									\
			if((inError) != 0)															\
			{																			\
				STOP;																	\
				{ inAction; }															\
				goto inHandler;															\
			}

#if defined(__cplusplus)

#define Throw(inException)  STOP; throw (inException)

#define	ThrowIf(inCondition, inException, inMessage)									\
			if(inCondition)																\
			{																			\
				Throw(inException);														\
			}

#define	ThrowIfNULL(inPointer, inException, inMessage)									\
			if((inPointer) == NULL)														\
			{																			\
				Throw(inException);														\
			}

#define	ThrowIfKernelError(inKernelError, inException, inMessage)						\
			{																			\
				kern_return_t __Err = (inKernelError);									\
				if(__Err != 0)															\
				{																		\
					Throw(inException);													\
				}																		\
			}

#define	ThrowIfError(inError, inException, inMessage)									\
			{																			\
				SInt32 __Err = (inError);												\
				if(__Err != 0)															\
				{																		\
					Throw(inException);													\
				}																		\
			}

#if TARGET_OS_WIN32
#define	ThrowIfWinError(inError, inException, inMessage)								\
			{																			\
				HRESULT __Err = (inError);												\
				if(FAILED(__Err))														\
				{																		\
					Throw(inException);													\
				}																		\
			}
#endif

#define	SubclassResponsibility(inMethodName, inException)								\
			{																			\
				Throw(inException);														\
			}

#endif	//	defined(__cplusplus)

#endif  //  DEBUG || CoreAudio_Debug

#endif
