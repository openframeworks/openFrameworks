openFrameworks currently supports the following platforms:

# Android #

versions: 2.2+

architectures:
  * armeabi-v7a
  * x86


# Emscripten #

notes: VM


# iOS #

versions: iOS 5 - 8 (FAT Bundle with libc++ for all archs)

architectures:
  * ARMv7
  * ARM64
  * x86
  * x86_64

notes:
  * ARMv7s removed since potential performance gains not worth the binary size increase, see #3423 for more discussion
  * ARM64 supported until Xcode 6 (OS X 10.9+)
  * Apple iOS store requires 64bit support and submission with Xcode 6+


# Linux #

versions: Debian, Arch, Ubuntu, Fedora

architectures:
  * x86
  * x86_64
  * ARMv6l
  * ARMv7l
	
# Microsoft Windows #

versions: Win7 - Win10

architectures:
  * x86
  * x86_64

notes:
  * Visual Studio 2015
  * MSYS2


# OS X #

versions: 10.7 - 10.10 (FAT Bundle)

architectures:
  * x86 (with libc++)
  * x86_64 (with libc++)

notes:
  * QTKitVideo pipeline will be deprecated, but essential for 10.7 support.
  * 10.8 recommended minimum (to use Xcode 5)
