# [openFrameworks](http://openframeworks.cc/) is a C++ toolkit for creative coding

To grab a copy of openFrameworks for your platform, check the [download page](http://openframeworks.cc/download) on the main site. The `master` branch of this repository corresponds to the most recent release, with a few differences:

1. The release includes a simple openFrameworks project generator.
2. This GitHub repository contains code and libs for all the platforms, but the releases are done on a per-platform basis.
3. This GitHub repository has no project files for the different examples. They are generated automatically for each release using a tool in `apps/devApps/projectGenerator/`.

If you want to work with the openFrameworks GitHub repository, you should use the project generator to create project files for all the code in `examples/`.

For per-platform readmes, see `readme.osx`, `readme.linux`, etc.

openFrameworks uses [Semantic Versioning](http://semver.org/), although strict adherence will only come into effect at version 1.0.0.