[openFrameworks](http://openframeworks.cc/)
================

openFrameworks is a C++ toolkit for creative coding.  Welcome to our community!

[![Slack Status](https://ofslack.herokuapp.com/badge.svg)](https://ofslack.herokuapp.com)

##Build status
================

Linux, OSX, iOS and Android     [![Build Status](https://travis-ci.org/openframeworks/openFrameworks.svg?branch=master)](https://travis-ci.org/openframeworks/openFrameworks)

Visual studio [![Build status](https://ci.appveyor.com/api/projects/status/sm9jxy0u56bl8syi/branch/master?svg=true)](https://ci.appveyor.com/project/arturoc/openframeworks/branch/master)

folder structure
--------

This release includes:

* **addons**
* **apps**
* **docs**
* **examples**
* **export (on some systems)**
* **libs**
* **other**
* **scripts**
* **project generator**


`docs` has some documentation, things to consider for each plattform, etc. You should take a look in there; if you are on OSX, read the osx.md.   Projects go into `apps` and `examples` -- `examples` contains a projects that show you how to use OF, and `apps` is where your projects go.  `libs` contains the OF libraries, including the openframeworks core.  `addons` aim at additional functionality that's not part of core.  `export` is for DLLs and dylibs that need to be put in each project.  The `scripts` folder has the tools for automating OF per platform. `project generator` is a GUI based tool for making new projects - this folder is only included in packaged releases.  

OF releases are designed to be self-contained.  You can place them anywhere. Tt's not possible to mix different releases of OF together, so keep each release (0.8.0, 0.8.1) separate.  Projects work from release to release, but this is not guaranteed.  Because OF is self-contained, there's extensive use of local file paths (ie, ../../../) thoughout OF.  It's important how directories are structured.  A common error is to take a project and move it.  This means that links such as ../../../libs will break.  


Get involved
--------

The openframeworks forum:

[http://forum.openframeworks.cc/](http://forum.openframeworks.cc/)

is a warm and friendly place.  Ask or answer a question.  This project is a community, more than just a tool, so join us!  Also, this is free software, and we learn so much about what is hard, what doesn't make sense, what is useful, etc. The most basic questions are acceptable!  Don't worry, just join the conversation.  Learning in OF is social, it's hard to do it alone, but together we can get far!

Our github site is active:

[https://github.com/openframeworks/openFrameworks](https://github.com/openframeworks/openFrameworks)

if you have bugs or feature requests, open an issue.  If you are a developer and want to help, pull requests are warmly welcome.  Please read the contributing guide for guidelines:

[https://github.com/openframeworks/openFrameworks/blob/master/CONTRIBUTING.md](https://github.com/openframeworks/openFrameworks/blob/master/CONTRIBUTING.md
)

We also have a developer's mailing list, which is useful for discussing issues around the development and future of OF.

Developers
------

To grab a copy of openFrameworks for your platform, check the [download page](http://openframeworks.cc/download) on the main site.  

If you are working with the Git repository, the `stable` branch of the OF repository corresponds to the most recent release, with a few important differences:  

1. The release includes a openFrameworks project generator.
2. This GitHub repository contains code and libs for all the platforms; releases are done on a per-platform basis.
3. This GitHub repository has no project files for the different examples. They are generated automatically for each release using a tool in `apps/projectGenerator/`.
4. There's no external dependencies in the repository, you can download them using the download_libs.sh script for each platform in the particular platform folder inside scripts.

If you want to work with the openFrameworks GitHub repository, you need to download the external dependencies and you should use the project generator to create project files for all the code in `examples/`.  To generate the project files with the project generator enable the 'Advanced Options' in the settings tab, then use 'Update Multiple' to update the projects for the `examples/` folder path in the repo.

To set up the project generator submodule within the OF repo, use the command `git submodule init` then `git submodule update` whilst inside the openFrameworks repo.

For info on working with the Project Generator, for per-platform readmes, and more information, see the [documentation](docs/table_of_contents.md).

Versioning
----------
openFrameworks uses [Semantic Versioning](http://semver.org/), although strict adherence will only come into effect at version 1.0.0.
