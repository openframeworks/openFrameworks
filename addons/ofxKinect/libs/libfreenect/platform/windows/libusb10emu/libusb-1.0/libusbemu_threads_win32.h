/*
* This file is part of the OpenKinect Project. http://www.openkinect.org
*
* Copyright (c) 2010 individual OpenKinect contributors. See the CONTRIB file
* for details.
*
* This code is licensed to you under the terms of the Apache License, version
* 2.0, or, at your option, the terms of the GNU General Public License,
* version 2.0. See the APACHE20 and GPL2 files for the text of the licenses,
* or the following URLs:
* http://www.apache.org/licenses/LICENSE-2.0
* http://www.gnu.org/licenses/gpl-2.0.txt
*
* If you redistribute this file in source form, modified or unmodified, you
* may:
*   1) Leave this header intact and distribute it under the same terms,
*      accompanying it with the APACHE20 and GPL20 files, or
*   2) Delete the Apache 2.0 clause and accompany it with the GPL2 file, or
*   3) Delete the GPL v2 clause and accompany it with the APACHE20 file
* In all cases you must keep the copyright notice intact and include a copy
* of the CONTRIB file.
*
* Binary distributions must follow the binary distribution requirements of
* either License.
*/

#ifndef LIBUSBEMU_THREAD_INTERFACE_WRAPPER_FOR_WIN32_THREADS_H
#define LIBUSBEMU_THREAD_INTERFACE_WRAPPER_FOR_WIN32_THREADS_H

#include <windows.h>
#include <stdio.h>
#include <vector>
#include <algorithm>

namespace libusbemu {

struct QuickEvent
{
friend struct EventList;

private:
  HANDLE hEvent;

public:
  inline QuickEvent(const bool signaled=false) : hEvent(NULL)
  {
    hEvent = CreateEvent(NULL, TRUE, (BOOL)signaled, NULL);
  }
  inline ~QuickEvent()
  {
    CloseHandle(hEvent);
  }
  inline void Signal()
  {
    SetEvent(hEvent);
  }
  inline void Reset()
  {
    ResetEvent(hEvent);
  }
  inline bool Check()
  {
    return(WAIT_OBJECT_0 == WaitForSingleObject(hEvent, 0));
  }
  inline const bool WaitUntilTimeout(const unsigned int milliseconds)
  {
    return(WAIT_OBJECT_0 == WaitForSingleObject(hEvent, (DWORD)milliseconds));
  }
  inline void Wait()
  {
    WaitUntilTimeout(INFINITE);
  }
};



struct QuickThread
{
private:
  HANDLE hThread;

  // Type-safe wrapper that converts arbitrary function signatures into the
  // required signature of Win32 Thread Procedures (LPTHREAD_START_ROUTINE).
  // Any Win32 LPTHREAD_START_ROUTINE declared routine can be wrapped as well.
  // The wrapper is also capable of cleaning up itself upon thread termination.
  // This wrapper can be extended in the future to support member-functions
  // to run as thread procedures.
  template<typename F>
  struct ThreadWrapper
  {
    struct State
    {
      F* routine;
      void* params;
      bool release;
      QuickThread* instance;
      QuickEvent* sigclone;
    };
    static DWORD WINAPI Thunk(LPVOID lpParameter)
    {
      State state = *((State*)lpParameter); // clone state (no heap alloc!)
      state.sigclone->Signal(); // done cloning, signal back to creator
      state.sigclone = NULL;

      // start wrapped thread procedure
      DWORD ret = (DWORD)state.routine(state.params);

      // release the associated QuickThread instance if requested
      if (state.release)
        delete(state.instance);

      return(ret);
    }
  };

  // allow the creation of pseudo-handles to the calling thread
  // this constructor cannot and should never be called explicitly!
  // use QuickThread::Myself() to spawn a pseudo-handle QuickThread
  inline QuickThread() : hThread(GetCurrentThread()) {}

public:
  template<typename F>
  inline QuickThread(F* proc, void* params, const bool auto_release=false) : hThread(NULL)
  {
    // the 'typename' is required here because of dependent names...
    // MSVC relaxes this constraint, but it goes against the standard.
    typename ThreadWrapper<F>::State state;
    state.routine  = proc;
    state.params   = params;
    state.release  = auto_release;
    state.instance = this;
    // in order to prevent heap allocation, an event is created so that the
    // thunk function can signal back when it is done cloning the state; this
    // may look like unnecessary overhead, but the less heap memory control,
    // the better becomes the management and maintenance of this class.
    QuickEvent hWaitThunkCloneState;
    state.sigclone = &hWaitThunkCloneState;

    // Ready to issue the thread creation:
    hThread = CreateThread(NULL, 0, &ThreadWrapper<F>::Thunk, (LPVOID)&state, 0, NULL);

    // Wait for the thread thunk to clone the state...
    hWaitThunkCloneState.Wait();
    // Event object will then be automatically released upon return
  }

  inline ~QuickThread()
  {
    // only if not a pseudo-handle...
    if (hThread == GetCurrentThread())
      return;
    CloseHandle(hThread);
  }

  static inline QuickThread Myself()
  {
    return(QuickThread());
  }

  inline void Join()
  {
    WaitForSingleObject(hThread, INFINITE);
  }

  inline bool TryJoin()
  {
    return(WAIT_OBJECT_0 == WaitForSingleObject(hThread, 0));
  }

  inline bool LowerPriority()
  {
    return(TRUE == SetThreadPriority(hThread, THREAD_PRIORITY_NORMAL));
  }

  inline bool RaisePriority()
  {
    return(TRUE == SetThreadPriority(hThread, THREAD_PRIORITY_TIME_CRITICAL));
  }

  static inline void Sleep(int milliseconds)
  {
    ::Sleep(milliseconds);
  }

// Yield is already a Win32 macro (WinBase.h)...
// http://winapi.freetechsecrets.com/win32/WIN32Yield.htm
#ifdef Yield
#undef Yield
#endif
// A pragma push/pop could be used instead, but it does not solve the issues
// http://stackoverflow.com/questions/1793800/can-i-redefine-a-c-macro-for-a-few-includes-and-then-define-it-back
//#pragma push_macro("Yield")
//#undef Yield
  static inline void Yield()
  {
    // Sleep(0) or Sleep(1) ?!
    // http://stackoverflow.com/questions/1413630/switchtothread-thread-yield-vs-thread-sleep0-vs-thead-sleep1
    ::Sleep(1);
    // could also use the following (but the semantics are quite shady...):
    // http://msdn.microsoft.com/en-us/library/ms686352(v=vs.85).aspx
    // SwitchToThread();
  }
//#pragma pop_macro("Yield")
};



struct QuickMutex
{
private:
  CRITICAL_SECTION cs;

public:
  inline QuickMutex()
  {
    InitializeCriticalSection(&cs);
  }

  inline ~QuickMutex()
  {
    DeleteCriticalSection(&cs);
  }

  inline const bool TryEnter()
  {
    return(0 != TryEnterCriticalSection(&cs));
  }

  inline void Enter()
  {
    EnterCriticalSection(&cs);
  }

  inline void Leave()
  {
    LeaveCriticalSection(&cs);
  }
};



struct EventList
{
  QuickMutex mutex;
  std::vector<QuickEvent*> m_vEvents;
  std::vector<HANDLE> m_vHandles;

  EventList() {};
  ~EventList() {};

  const bool AttachEvent(QuickEvent* poEvent)
  {
    mutex.Enter();
      m_vEvents.push_back(poEvent);
      m_vHandles.push_back(poEvent->hEvent);
    mutex.Leave();
    return(true);
  }

  const bool DetachEvent(QuickEvent* poEvent)
  {
    mutex.Enter();
      std::vector<QuickEvent*>::iterator it1 = std::find(m_vEvents.begin(), m_vEvents.end(), poEvent);
      m_vEvents.erase(it1);
      std::vector<HANDLE>::iterator it2 = std::find(m_vHandles.begin(), m_vHandles.end(), poEvent->hEvent);
      m_vHandles.erase(it2);
    mutex.Leave();
    return(true);
  }

  int WaitAnyUntilTimeout(const unsigned int milliseconds)
  {
    int index (-1);
    mutex.Enter();
      DWORD ret (WAIT_FAILED);
      const unsigned int nHandles (m_vHandles.size());
      if (nHandles > 0)
        ret = WaitForMultipleObjects(nHandles, &m_vHandles[0], FALSE, milliseconds);
      if (ret - WAIT_OBJECT_0 < nHandles)
        index = (int)(ret - WAIT_OBJECT_0);
    mutex.Leave();
    return(index);
  }

  int WaitAny()
  {
    return(WaitAnyUntilTimeout(INFINITE));
  }

  int CheckAny()
  {
    return(WaitAnyUntilTimeout(0));
  }

  const bool WaitAllUntilTimeout(const unsigned int milliseconds)
  {
    bool waited (false);
    mutex.Enter();
      const unsigned int nHandles (m_vHandles.size());
      if (nHandles > 0)
        waited = (WAIT_TIMEOUT != WaitForMultipleObjects(nHandles, &m_vHandles[0], FALSE, milliseconds));
    mutex.Leave();
    return(waited);
  }

  const bool WaitAll()
  {
    return(WaitAllUntilTimeout(INFINITE));
  }

  QuickEvent* operator [] (const unsigned int index)
  {
    QuickEvent* poEvent (NULL);
    mutex.Enter();
      poEvent = m_vEvents[index];
    mutex.Leave();
    return(poEvent);
  }

};

} //end of 'namespace libusbemu'

#endif//LIBUSBEMU_THREAD_INTERFACE_WRAPPER_FOR_WIN32_THREADS_H
