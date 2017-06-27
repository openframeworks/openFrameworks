## How to compile openFrameworks from github

Follow these instructions only if you want to submit a pull request or if you want to always use the most recent version of openFrameworks. In other cases you will not need to clone the entire openFrameworks repository.

If you are a developer, or if you want to submit a pull request, [read this first](https://github.com/openframeworks/openFrameworks/blob/master/README.md#developers)

### Clone the master branch

Let's start by cloning the last `master` branch of openFrameworks and its submodules.

```bash
git clone --recursive git@github.com:openframeworks/openFrameworks.git
```

_Before continuing make sure your new openFrameworks path **has no spaces**. Many of the shell scripts below will fail on paths that include spaces._

### Download dependencies

As the external dependencies are not found in the repository, you need to download them. To make things simpler, use the bash script called `download_libs.sh` which can be found in the `scripts` folder. In this folder, there are several subfolders, one for each platform. Assuming you are, for example, using OSX, you need to run `/bin/bash scripts/osx/download_libs.sh`.

### Get the Project Generator

Now that you have all the required files, you need to compile them and set up all the projects contained in the `examples` folder. The easiest way to do this is by using the Project Generator to compile all the examples.
The Project Generator app facilitates creating new openFrameworks projects and adding or removing addons to existing projects. As it is a separate project with a well defined scope, the openFrameworks developers decided to put it into its own repository. You can get the project generator in two different ways:
* Initialize and update all the git submodules (including the Project Generator) by first running `git submodule init` and then `git submodule update`.
* Or, easier, download an already packaged openFrameworks installation from the [download page](http://openframeworks.cc/download/), and copy the `projectGenerator` folder into your openFrameworks installation.

As the first option can lead to [unexpected results](https://forum.openframeworks.cc/t/how-to-build-project-generator-from-of-git-repo/26232), we suggest you use the second approach.

### Compile all the examples

Now that you have the Project Generator, you are ready to compile all the examples:

#### Option 1 - With the Project Generator app
* Start the Project Generator application by double clicking on it.
* Hit the cog wheel on the right side of the Project Generator window and select `advanced options`.
* Select the `update multiple` tab, enter the path to the examples folder inside the current OpenFrameworks repo folder.
* Make sure to select your platform in the platforms field below.
* Click on `update multiple` and wait. This operation can take a while depending on your hardware.

Once the success message appears you are done.

#### Option 2 - With the command line Project Generator
* from the OF root directory, run `projectGenerator -r -o"." examples`

## How to submit your pull requests

If you plan to submit pull requests to openFrameworks, the procedure is almost identical to what we described above, with one little difference: you should clone your own fork of openFrameworks:

### A. Fork and clone openFrameworks

* Visit https://github.com/openframeworks/openFrameworks and click Fork to create your own copy of the project.
* Clone your newly created repository:

```bash
git clone --recursive git@github.com:YOUR-USER-NAME/openFrameworks.git
```

Replace YOUR-USER-NAME with your GitHub user name in the previous command.

* Add the external dependencies and the project generator as explained in the top section.
* Follow the Next steps below.

### B. If you cloned openFrameworks without forking

If you followed the top section of this document and cloned openFrameworks from `git@github.com:openframeworks/openFrameworks.git`, downloaded the dependencies and compiled the examples and then decided you want to submit a pull request, you don't need to start from scratch. You only need to fork openFrameworks, and then point your local repository to your new fork:

* Visit https://github.com/openframeworks/openFrameworks and click Fork to create your own copy of the project.
* Edit the `.git/config` file inside your local openFrameworks folder and update the `url` line so it points to your own fork. Do this by substituting `openframeworks/openFrameworks.git` by `YOUR-USER-NAME/openFrameworks.git` (where YOUR-USER-NAME is your GitHub user name).

### Next steps

[This help page](https://help.github.com/articles/creating-a-pull-request-from-a-fork/) by GitHub explains the pull request process. Please read it before starting.

In the openFrameworks repository you can find instructions about [how to contribute](https://github.com/openframeworks/openFrameworks/blob/master/CONTRIBUTING.md).
