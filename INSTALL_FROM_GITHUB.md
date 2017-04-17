## How to compile openFrameworks from github

This instructions are useful just in case you want to submit a pull request or you want to have always the last openFrameworks version. In any other case, most probably, you don't need to clone the entire openFrameworks repository.

If you are a developer, or if you want to submit a pull request, [read this first](https://github.com/openframeworks/openFrameworks/blob/master/README.md#developers)

Let's start cloning the last `master` branch of openframeworks and its submodules.

```bash
git clone --recursive git@github.com:openframeworks/openFrameworks.git
```

As the external dependencies are not in the repository, you need to download them. To make the things simplier, there is a bash script for this goal called `download_lib.sh`. This bash script is located in the `scripts` folder. In this folder, there are several subfolders, one for each paltform. Assuming you are, for example, using OSX, you need to run `/bin/bash scripts/osx/download_libs.sh`.

Now you have all the files that you need, you just need to compile them and to setup all the projects contained in the `examples` folder. The easiest way to do this is to use the projectGenerator to compile all the examples.
The projectGenerator is useful app to create new project and to add and remove addons. As it is a separate project, with a well defined scope, the openFrameworks developer decided to put it in a separate repository. You have 2 ways to get the project generator in your openFrameworks installation:
* You can initialize and update all the git submodules (including the projectGenerator) running first `git submodule init` and then `git submodule update`
* Or, more easily, you can download an already packaged openFrameworks installation from the [download page](http://openframeworks.cc/download/), and copy the `projectGenerator` folder in your openFrameworks installation.

As the first option can lead to [unexpected results](https://forum.openframeworks.cc/t/how-to-build-project-generator-from-of-git-repo/26232), we suggest you the second one.

Now that you have the project generator, you are ready to compile all the examples. Do as follow:

* Then run the project generator application double clicking on it.
* Hit the cog wheel on the right of the Project Generator and select `advanced options`.
* Then select the `update multiple` tab, put in the path to the examples folder in the current OpenFrameworks repo folder.
* Make sure to select your platform in the platforms field below.
* Hit `update multiple` and wait. This operation can take a while depending on your hardware.

Once the success message appears you are done.

## How to submit you pull requests

The procedure to install openFrameworks locally is almost the same, but you need first to fork the openFrameworks repository from github, and clone on your local machine your fork of the openFrameworks repository instead `git@github.com:openframeworks/openFrameworks.git`. Please be sure to read [this](https://help.github.com/articles/creating-a-pull-request-from-a-fork/) before starting.

Once you have your forked repo locally, add the project generator as explained in the previous section.

In the openFrameworks repository you can find the instruction about [how to contribute](https://github.com/openframeworks/openFrameworks/blob/master/CONTRIBUTING.md)

