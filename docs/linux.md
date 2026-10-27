[openFrameworks](http://openframeworks.cc/) | [Documentation table of contents](table_of_contents.md)

Linux install
==========

Before you can use OF in linux you'll need to run a couple of scripts to install some dependencies and compile the OF library itself and the project generator.

First of all you'll need to execute the install_dependencies.sh script for your distribution which is located in `scripts/linux/distro_name/install_dependencies.sh`

For example if you are using ubuntu, from a console:

```
cd OF/scripts/linux/ubuntu
sudo ./install_dependencies.sh
```

Next if you want to use some formats like mp3 you'll need to run the install_codecs script, it's in the same folder and is run the same way as the install_dependencies one. We separate them cause mp3 and some other formats installed by the script might be protected by patents in some countries and could be problematic to use in comercial work.

The OF source code comes uncompiled so the first time you compile an example it would need to compile it to, if you go and compile any example it would compile OF as a dependency but to make that easier and catch any potential problems let's do that as a separate step:

```
cd OF/scripts/linux
./compileOF.sh -j3
```

`-j3` tells the script to use 3 cpus to compile, you can specify as many as you want but it's recommended to use the number of cores in your computer or less.

With this you can already go into any of the examples folders and compile the examples using make:

```
cd OF/examples/graphics/polygonExample
make
make run
```

Or use any of the officially supported IDEs: [qtcreator](../qtcreator/) or [eclipse](../eclipse/) both IDEs have plugins that allow to create new projects, import existing ones, add addons to them.

If you want to install the project generator, a tool that allows to create and update an OF project you'll need to run a last script:

```
cd OF/scripts/linux
./compilePG.sh
```

which will compile the command line version of the project generator and install it in your system so you can create projects from anywhere, it'll also make it possible to use the gui tool which is present in the root of the OF folder.

Linking prebuilt libraries: "recompile with -fPIE"
---------------------------------------------------

Current distributions (Ubuntu, Debian, Fedora, Raspberry Pi OS...) build executables as PIE (position independent executables) by default. A static library (`.a`) that wasn't compiled as position independent code can't be linked into one, and the link fails with an error like this:

```
/usr/bin/ld: libfoo.a(foo.o): relocation R_X86_64_32S against symbol `bar' can not be used when making a PIE object; recompile with -fPIE
/usr/bin/ld: failed to set dynamic section sizes: bad value
collect2: error: ld returned 1 exit status
```

The relocation type depends on the architecture: `R_X86_64_32` or `R_X86_64_32S` on x86_64, `R_ARM_MOVW_ABS_NC` or `R_ARM_MOVT_ABS` on 32-bit ARM. This usually comes from a precompiled library in an addon (`addons/ofxSomething/libs/*/lib/linux64/*.a`) built before PIE was the default. To check a library, look for those relocations; no output means it's position independent:

```
objdump -r libfoo.a | grep -E 'R_X86_64_32S? |R_ARM_MOV[WT]_ABS'
```

**Fix: rebuild the library as position independent code** with `-fPIC`. With CMake, add `-DCMAKE_POSITION_INDEPENDENT_CODE=ON`; with autotools or plain makefiles, add `-fPIC` to `CFLAGS` and `CXXFLAGS`:

```
cmake -S . -B build -DCMAKE_POSITION_INDEPENDENT_CODE=ON -DBUILD_SHARED_LIBS=OFF
./configure --enable-static --disable-shared CFLAGS="-fPIC" CXXFLAGS="-fPIC"
```

If the library comes with an addon, please also report it to the addon so the fixed library helps everyone.

**Workaround: link your app without PIE.** If you can't rebuild the library, add this to your project's `config.make`:

```
PROJECT_LDFLAGS = -no-pie
```

The app then links as a regular executable. It works the same, but without the address randomization PIE gives, so prefer rebuilding the library.
