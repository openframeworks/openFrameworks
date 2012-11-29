# -DRAFT IN PROGRESS-
---
# Building openFrameworks using GNU Make

## Usage

To build the default core openFrameworks libraries on the terminal, navigate to the `$(OF_ROOT)/libs/openFrameworksCompiled/Projects` folder and run:

```
make
```

The `makefile` there will then examine your system, determine the appropriate platform and architecture, and then choose the appropriate `config.core.XXX.default.make` file.  An example of a platform-specific core configuration file can be found here:

```
$(OF_ROOT)/libs/openFrameworksCompiled/Projects/linux64/configure.core.linux64.default.make
```

Platform specific configuration filenames take the form:

```
configure.core.PLATFORM_ARCH.PLATFORM_VARANT.make
```

The makefile system also supports special platform variants.  Platform variants are useful when a developer wants to build the openFrameworks core with non-standard system libraries or needs to develop platform variants for a standard OS and archiecture (e.g. linux and armv6l) that may be unique to a specific embedded system (e.g. Raspberry Pi).  To build platform variant core openFrameworks libraries on the terminal navigate to the `$(OF_ROOT)/libs/openFrameworksCompiled/Projects` folder and run:

```
make PLATFORM_VARIANT=raspberrypi
```

If if you are executing the command on a `linuxarmv6l` machine, the `makefile` will configure your settings based on:

```
$(OF_ROOT)/libs/openFrameworksCompiled/Projects/linuxarmv6l/configure.core.linuxarmv6l.raspberrypi.make
```

In the case of `linuxarmv6l`, there is currently no `default` configuration file, so `make` must be run with the appropirate `PLATFORM_VARIANT` set.

## Features

### Core Makefiles
- openFrameworks core libraries can now be compiled in a platform specific way, with support for easily specifying platform variants.
- openFrameworks core libraries now now supported with libsorder.make (useful for Poco especially)
- libsorder.make supports comments via #

### Project Makefiles
- Fully compliant with ofxAddonTemplate
- Addons are supported via addons.make
- addons.make supports comments via #


## Supported Systems

### Linux (64-bit)
- Tested on Ubuntu 12.04.01 64-bit

## Under Development

- Linux (32-bit)
- OSX
- Android
- Raspberry Pi

## Legacy Makefiles


# TODO
- make sure that all `find` commands that may not return results dump errors in to /dev/null
- make sure that PLATFORM_REQUIRED_ADDONS are being integrated correctly
- check for platform required system libs using pkg-config -- if they do not exist, throw an error that instructs the user how to install them using their package management system.
- .

- Priority Low: Makefiles need to be consolidated (there are a lot of files in the makefilesCommon directory and most of them can be merged together -- they are currently seprated by function -- so makefiles dont have to reimplement common features).  This is low priority.
- Priority Medium: Makefiles currently work with linux and linux64, but are not yet configured to replace the android builds.  This should be straightforward using the platform-specific configuration files.  This will need to be done before any kind of oF core integration will be a possibility.
Testing!
- Makefile should support cross-compiling. Not sure if they do already, I haven't tried. But we shouldn't have to compile on the board. Normally I compile on my host machine in an smb-share with a board. (need cross compiling workflow tut :)