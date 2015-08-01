As of 0.9, the following platforms are supported:

# Android #

versions: ?

architectures:
  * armeabi-v7a
  * i386


# Emscripten #

notes: VM


# iOS #

versions: iOS 5 - 8 (FAT Bundle with libc++ for all archs)

architectures:
  * armv7
  * arm64
  * i386
  * x86_64

notes:
  * armv7s removed since potential performance gains not worth the binary size increase, see #3423 for more discussion
  * arm64 supported until Xcode 5 (OS X 10.8+)
  * Apple iOS store requires 64bit support


# Linux #

versions: Debian, Arch, Ubuntu, Fedora

architectures:
  * i386
  * x86_64
  * armv6l
  * armv7l
	
# Microsoft Windows #

versions: Win7 - Win8

architectures: ?

notes:
  * Visual Studio 2012 for i386
  * MinGW / Codeblocks 12.11 i386


# OS X #

versions: 10.7 - 10.10 (FAT Bundle)

architectures:
  * i386 (with libc++)
  * x86_64 (with libc++)

notes:
  * QTKitVideo pipeline will be deprecated, but essential for 10.7 support.
  * 10.8 recommended minimum (to use Xcode 5)
