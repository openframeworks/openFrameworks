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

#pragma once

// Wrappers for platform-specific thread/synchronization objects:
// * Thread
// * Mutex  (Critical Section)
// * Events (Conditional Variables)

#ifdef WIN32
  // stick to the Windows scheme for now, but this structure could be easily
  // replaced by pthread for portability; however if real-time priority turns
  // out to be a requirement on that platform, the pthread implementation may
  // not have support for such scheduling.
  // for a much more lightweight run-time, this could be replaced by dummy
  // objects, provided that the library client is careful enough to avoid any
  // sort of race-conditions or dead-locks...
  #include "libusbemu_threads_win32.h"
#else
  #error LIBUSBEMU PTHREAD WRAPPER NOT YET IMPLEMENTED!
  // #include "libusbemu_threads_pthread.h"
#endif

namespace libusbemu
{

struct RAIIMutex
{
  QuickMutex& m_mutex;
  RAIIMutex(QuickMutex& mutex) : m_mutex(mutex) { m_mutex.Enter(); }
  ~RAIIMutex() { m_mutex.Leave(); }
};

}
