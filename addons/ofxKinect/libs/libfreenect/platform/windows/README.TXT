======================================================================================

SUMMARY
*******

1) libfreenect: incompatibilities with Visual C++
   This section is here merely for historical reasons. All of the issues documented
   here were already fixed in the libfreenect repository and CMake should be able to
   produce a project that is ready to build libfreenect in Windows with Visual Studio.
   Consider browsing this section if experiencing compilation issues under different
   platforms, compilers and/or IDEs.

2) libusb-1.0 vs. libusbemu: Issues and Concept
   The current port of libfreenect for Windows uses a libusb-1.0 emulation layer since
   a proper port of libusb-1.0 for Windows is not yet available. Such emulation layer
   allows Windows development to keep in sync with the official development branch of
   libfreenect, without the need of dedicated drivers/implementations. This section
   discusses why and how the current libfreenect Windows port moved in this direction.

3) libusbemu: Tips, Hints and Best Practices
   The current status of libusbemu is quite reliable under normal usage circumstances,
   but by no means stable: caution is advised. This section provides some guidelines
   to avoid potential pitfalls and keep the application running safely under Windows.
   They are simple and natural to follow, so they should not impose any special design
   considerations for the application.

4) Overall performance of libfreenect in Windows
   The current Windows port of libfreenect has some performance overhead over other
   platforms and dedicated Win32 driver implementations due to the libusbemu module.
   This section contains a benchmark scenario and a also a discussion on the results.
   In short, the overhead is negligible and should not prevent anyone from using it.

======================================================================================

1) libfreenect source code: incompatibilities with Visual C++
   **********************************************************

----------------------------------------------------------------------------------
Language issues: The Microsoft C compiler does not implement all the C99 standard.
----------------------------------------------------------------------------------

An attempt to compile the current libfreenect with Visual C++ will trigger a lot
of errors. A simple workaround is to tell Visual Studio to force compilation all the
".c" files within the project using the C++ compiler:
  Project >> Properties >> C/C++ >> Advanced >> Compile As: Compile as C++ Code (/TP)

This will get rid of most errors, except those regarding implicit pointer casts.
Here are a few examples of such implicit pointer casts from within libfreenect:
  tilt.c      dev->raw_state.tilt_status = buf[9];
  core.c      *ctx = malloc(sizeof(freenect_context));
  cameras.c   strm->raw_buf = strm->proc_buf;
It seems that it is not possible to force Visual C++ to perform such implicit casts
(if anyone knows how, please share! :-)

Such implicit casts then have to be made explicit:
              dev->raw_state.tilt_status = (freenect_tilt_status_code)buf[9];
              *ctx = (freenect_context*)malloc(sizeof(freenect_context));
              strm->raw_buf = (uint8_t*)strm->proc_buf;
Fortunately, they are not many, and it can be done in a couple of minutes.
This should impose a minimal burden to the Win32 repository maintainers.

NOTE: Sometimes, even when "Compile as C++ Code" is specified, the build will insist
on using the C-compiler to compile .c files (this happens with MSVC versions prior to
2010). To work around this, just set the "Compile As" to "Default", click "Apply" and
then set it back to "Compile as C++ Code (/TP)", click "Apply" and then "OK".

Another problem is that Visual C++ does not offer the <unistd.h> and <stdbool.h>
headers. However, they are pretty simple to be emulated; the <stdbool.h> will
actually be a dummy header because C++ already defines the "bool" type semantics,
while the <unistd.h> has to simply #include <stdint.h> and define the "ssize_t"
type. The implementation of these headers are located at:
  "libfreenect\platform\windows"

NOTE: MSVC versions prior to 2010 do not provide the <stdint.h> header. An ISO C9x
compilant <stdint.h> header for such MSVC versions can be found at the wikipedia entry
of stdint.h (at the "External links" section of the page):
  http://en.wikipedia.org/wiki/Stdint.h
or directly through this link:
  http://msinttypes.googlecode.com/svn/trunk/stdint.h
A copy of such header will be also provided within libfreenect in the future.

The "freenect_internal.h" makes use of GCC's "__attribute__" keyword, and there is no
such a thing in Visual C++. Fortunately, this header is not exposed for the library
user and is just required during the library build. There are a few simple solutions
for this issue: a) remove this keyword or b) define a dummy macro for it.

Another issue is that since all .c files were forced to be compiled as C++ code,
the "libfreenect.h" header no longer requires the "#ifdef __cpluscplus extern C"
idiom. Commenting out this guard will do the trick, but better checking is possible.

-----------------------------------------------------------------------------------
Library issues: libfreenect uses libusb-1.0 which is not yet available for Windows.
-----------------------------------------------------------------------------------

The final issue is regarding the default USB back-end used by libfreenect, libusb-1.0,
which is not yet available for Windows. This restriction forces the Windows port of
libfreenect to implement its own back-end, which then splits the Windows port from the
main development branch of libfreenect as new device features are reverse-engineered
and added to the library. Fortunately, such restriction can be alleviated through the
libusb-1.0 API "emulator" and keep the Windows port in sync with the current status of
libfreenect. More on this subject in the following section.

======================================================================================

2) libusb-1.0 vs. libusbemu: Issues and Concept
   ********************************************

The libfreenect uses libusb-1.0 as its default USB back-end to communicate with Kinect
but libusb-1.0 is not yet available for Windows. The current libusb-1.0 implementation
for Windows is experimental and uses WinUSB as its USB back-end. Unfortunately, WinUSB
does not support isochronous transfers, rendering it useless for Kinect purposes.

However, all is not lost since the latest version of libusb-win32 has support for the
isochronous transfers imposed by Kinect. There are issues too: libusb-win32 is based
on the old libusb-0.1 API which is incompatible with the libusb-1.0 API. Some of the
initial efforts to port libfreenect to Windows were based on the libusb-win32, being
Zephod's Win32 Kinect driver prototype a well-known instance:
   http://ajaxorg.posterous.com/kinect-driver-for-windows-prototype

The problem with a dedicated libusb-win32 driver implementation is that it has to be
maintained separately from the current libfreenect development. As new Kinect features
are reverse-engineered and implemented in the official libfreenect branch, the Windows
port requires additional maintenance to keep it in sync with the newest updates, even
when such updates don't involve USB communication at all.

One could argue that libfreenect should abandon the use of libusb-1.0 and just adopt
the old libusb-0.1 instead, since it is has support in a wider range of platforms...
This is out of question! First of all, libusb-0.1 still exists for legacy reasons, and
it is highly recommended to move to the new API if possible. Moreover, libusb-0.1 does
not have built-in support for asynchronous transfers, which would force libfreenect to
hold and manage threads internally, which would then lead to a whole set of new issues
that are prone to hurt performance, maintainability and portability.

Fortunately, libfreenect only requires a small portion of the libusb-1.0 API, and such
subset can be emulated, at some extent, through what is provided from libusb-win32. As
a result, the burden of maintaining dedicated development branches for a Windows port
is eliminated and the port can keep in synch with updates made in the libfreenect. One
may now ask: how can libusb-win32 be of any help if it is based on the libusb-0.1 API,
which has no support for asynchronous transfers? Well, that's because it happens that
libusb-win32 is more than just a Win32 build of libusb-0.1: it is a special branch of
the 0.1 API that extends it to allow asynchronous transfers.

The normal execution flow of libfreenect is something like this:
  Application <-> libfreenect <-> libusb-1.0 <-> Kinect
In Windows, the flow is as follows:
  Application <-> libfreenect <-> libusb-1.0-emu <-> libusb-win32 <-> Kinect

The source code of the emulation layer is available at:
  libfreenect\platform\windows\libusb10emu\libusb-1.0
Keep in mind that libusb-win32 is still required, and can be downloaded from:
  http://sourceforge.net/apps/trac/libusb-win32/wiki
The latest snapshots are recommended, obtained directly from this URL:
  http://sourceforge.net/projects/libusb-win32/files/libusb-win32-snapshots

Emulation of libusb-1.0 on top of libusb-win32 is not trivial, in special because of
some Windows-specific issues. There will be performance overhead, but as discussed in
the "Overall Performance" section, this should not hurt the application performance at
significant levels. Moreover, although the libusbemu is currently quite reliable, it
is by no means stable and on par to the real libusb-1.0 semantics. Caution is advised,
so be sure to read the following section for some usage considerations.

The libusbemu sits completely hidden behind libfreenect: the application does not need
to worry or call any special functions. In fact the application is not even aware that
libusbemu exists.

======================================================================================

3) libusbemu: Tips, Hints and Best Practices
   *****************************************

---------------------------------------------------------------
TIP: Trigger the Fail Guard if the system becomes unresponsive.
---------------------------------------------------------------

Since libusbemu is quite experimental, there is a fail guard within it. If for some
reason your system renders unresponsive, try focusing any window of the application
and hold [CTRL] + [ALT] for a while.

This will trigger a special synchronization event within the libusbemu which will
interrupt the execution of any internal thread of libusbemu and prompt a message box
to the user asking for action. You can either resume execution (if you unintentionally
pressed [CTRL] + [ALT] in the console window) or abort libusbemu. The fail guard will
never trigger if there is no incoming video or depth streams.

The Fail Guard is important since in such situations one would most likely be forced
to shutdown the computer (in a not so graceful way, by holding the power button!).

----------------------------------------
TIP: Only use a single freenect context.
----------------------------------------

Multiple freenect contexts should be no problem in the future. Having more than one
device attached to the same context should work fine, but no tests were made so far.

-----------------------------------------------------
TIP: Yield the rendering thread after each iteration.
-----------------------------------------------------

In case your application performs direct rendering (OpenGL, Direct3D), it is highly
recommended to yield the rendering thread after finishing rendering each frame. This
can alleviate a lot of CPU usage. The ideal place for this yield is right after the
swap-buffers call (SwapBuffers(), glutSwapBuffers(), Present(), etc). The best way to
yield is by calling Sleep(1). Note that Sleep(0) is also possible, but the former is
more "democratic".

Note that some graphics drivers or platforms may already yield after swap-buffers.
This seems to be the case with OpenGL NVIDIA drivers for Linux. In Windows, however,
the same driver (version) does not yield. Maybe in Linux the "yielder" is not the
driver itself, but the underlying implementation of glXSwapBuffers()... Anyway, when
in doubt, it will not hurt to explicitly yield again.

--------------------------------------------------------------------------------------
TIP: Perform stream operations only in the thread that calls freenect_process_events()
--------------------------------------------------------------------------------------

By stream operations I mean these:
  freenect_start_video()
  freenect_start_depth()
  freenect_stop_video()
  freenect_stop_depth()

This tip seems to hold for other platforms as well, and it is respected through all of
the official libfreenect examples and should be of no burden for the application. This
may be due to the underlying semantics of the real libusb-1.0. Unfortunately, although
libusb-1.0 has an excellent API documentation it lacks on a proper specification, thus
difficulting the task of checking if this is indeed a restriction.

In summary, a typical libfreenect application (check the official examples) will have
a dedicated thread that calls freenect_process_events(). This function is responsible
for querying and dispatching incoming streams (a.k.a. video and depth frames) from the
Kinect device to the application (behind the scenes is a call the libusb-1.0 function
libusb_handle_events()) and it is advised that any stream operation intended by the
application (like switching to some different video format) should happen within the
thread that calls the freenect_process_events(). The official libfreenect examples do
this, so be sure to check their source code if confused. It is also advised to have
only one thread calling freenect_handle_events().

Interestingly, some tests were made with both the libusb-1.0 and libusbemu and mixing
stream operations in different threads seem to work well. However, no guarantees are
given if executed in such a fashion. Again, libusb-1.0 has no specification document
and if such behavior is really to be expected is a hard task to determine.

======================================================================================

4) Overall performance of libfreenect in Windows
   *********************************************

---------------------------------------------
THIS SECTION IS OUTDATED, GOTTA REDO IT SOON!
---------------------------------------------

Hardware:
* Notebook 
* CPU: Intel Core2 Duo 32bit [T7250] @ 2.0GHz
* RAM: 4GB RAM
* GPU: GeForce 8600M GT 256MB VRAM



Task: display of simultaneous RGB (Bayer-to-RGB) and depth streams (16bit unpadded) on
the screen through OpenGL textures. Application source code is identical for all tests
(except for the Zephod's version that required some interface adaptation).



Results: performance measurements refer to the average frame time (one loop iteration).

Linux: Ubuntu Notebook 10.10 -- gcc 4.4.5
* Debug:   1.22ms | CPU @ 77% | video @ 30Hz | depth @ 30Hz
* Release: 1.15ms | CPU @ 72% | video @ 30Hz | depth @ 30Hz

Win32: Windows 7 Enterprise 32bit -- VC++ (Professional) 2010
1) libfreenect with libusbemu:
   * Debug:   2.44ms | CPU @ 75% | video @ 30Hz | depth @ 30Hz
   * Release: 1.93ms | CPU @ 63% | video @ 30Hz | depth @ 30Hz
2) Zephod's dedicated driver (V16):
   * Debug:   2.87ms | CPU @ 82% | video @ 30Hz | depth @ 30Hz
   * Release: 2.55ms | CPU @ 77% | video @ 30Hz | depth @ 30Hz



Remarks:

Debug builds account for the library itself being built in Debug mode (libfreenect or
Zaphod's driver, whichever applies). Release builds also imply that the program was
started without any debug information embedded.

In Windows, time was measured through the Win32 exclusive QueryPerformanceFrequency()
and QueryPerformanceCounter() routines. In Linux, the measurement was performed via
POSIX gettime() function.

A Win32 MinGW-based (gcc/g++ 4.5.0) build of libfreenect with libusbemu using POSIX
gettime() also yielded to nearly identical performance results than VC++ 2010 with
Performance Counters.

All of the Win32 performance results were also double-checked with Fraps.

In Windows, a single thread yield call was placed after rendering each screen frame
(as recommended in Item 3-3). In Linux, the graphics driver - possibly glXSwapBuffers
itself - seems to be already yielding the rendering thread, and forcing it in the code
did not incur into any extra impact on performance.



Discussion:

Even though there are no streaming frequency discrepancies between platforms, one may
infer, just from the frame times, that Windows clearly has an overhead disadvantage.
However, this does not hold true: there is still plenty of time for the application
logic to run.

For a steady 60FPS (~16.66ms per frame) real-time performance, a Release build using
libusbemu in Windows would still have about 14.70ms per frame available for the client
code, while in Linux the available time would be around 15.50ms, a 0.8ms overhead
between the platforms. Such a small difference should not impose any special design
considerations for the client code.

Furthermore, note that the CPU usage in Windows tend to be lower than in Linux. The
reason behind this difference is quite difficult to summarize here, but it is probably
related to the way Windows and Linux perform asynchronous I/O in USB-mapped files.
The interested reader is encouraged to refer to the following links:
  > http://www.unwesen.de/articles/waitformultipleobjects_considered_expensive
  > http://softwarecommunity.intel.com/articles/eng/2807.htm
  > http://software.intel.com/en-us/blogs/2006/10/19/why-windows-threads-are-better-than-posix-threads/
The memory consumption overhead of libusbemu in Windows is negligible, and as far as
the VC++ memory leak detection goes, libusbemu has no memory leaks.

======================================================================================
