[openFrameworks](http://openframeworks.cc/) | [Documentation table of contents](table_of_contents.md)

Linux install
==========

Before you can use OF in linux you'll need to run a couple of scripts to install some dependencies and compile the OF library itself and the project generator.

First of all you'll need to execute the install_dependencies.sh script for your distribution which is located in `scripts/linux/dsitro_name/install_dependencies.sh`

For example if you are using ubuntu, from a console:

```
cd OF/scripts/linux/ubuntu
sudo ./install_dependencies.sh
```

Next if you want to use some formats like mp3 you'll need to run the install_codecs script, it's in the same folder and is run the same way as the install_dependencies one. We separate them cause mp3 and some other formats installed by the script might be protected by patents in some countries and could be problematic to use in comercial work.

The OF source code comes uncompiled so the first time you compile an example it would need to compile it to, if you go and compile any example it would compile OF as a dependency but to make that easier and catch any potential problems let's do that as a separate step:

```
cd OF/scritps/linux
./compileOF.sh -j3
```

`-j3` tells the script to use 3 cpus to compile, you can specify as many as you want but it's recommended to use the number of cores in your computer or less.

With this you can already go into any of the examples folders and compile the examples using make:

```
cd OF/examples/graphics/polygonExample
make
make Run
```

Or use any of the officially supported IDEs: [qtcreator](../qtcreator/) or [eclipse](../eclipse/) both IDEs have plugins that allow to create new projects, import existing ones, add addons to them.

If you want to install the project generator, a tool that allows to create and update an OF project you'll need to run a last script:

```
cd OF/scritps/linux
./compilePG.sh
```

which will compile the command line version of the project generator and install it in your system so you can create projects from anywhere, it'll also make it possible to use the gui tool which is present in the root of the OF folder.
