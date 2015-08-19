# CI testing OF on Travis

This folder contains the scripts necessary for the Continuous Integration tests running on travis-ci.org.

The main CI steps are divided into two steps:
* A build step living in `/scripts/ci/*/build.sh`, which builds the openFrameworks library, the emptyExample where appropriate and the allAddonsExample (on Linux), and, in the future, e.g. the Project generator.
* In a second step, tests are run where available, using a script in `/scripts/ci/*/run_tests.sh`.

This achieves a bit of separation between the different CI tasks, so it is easier to tell which part failed, as the steps in the `script:` section of `.travis.yml` are all checked for exit status independently.

## Documented preconditions
* Currently, scripts expect the `$TRAVIS_BUILD_DIR` environment variable to exist, but falls back to automatically getting this from the script location with [a nice trick](http://stackoverflow.com/a/16879155/599884) and some help from [SO](http://stackoverflow.com/a/4774063/599884).
* The docs target relies on `$TRAVIS_PR`.
* The Android build downloads a stripped NDK from our server. The list of stripped things is at scripts/ci/android/NDK_excludes.txt for documentation.
* The build cache for Android builds currently has to be manually cleared when changing the NDK/toolchain version, and a new stripped NDK probably has to be uploaded to our server. A more granular cache test in scripts/ci/android/install.sh could make this unnecessary.

## Future fixes/improvements
* (If on newer Ubuntu) remove the -no-install-recommends from the docs before_install script.
* Within `linux/build.sh`, exporting `$CXX` does not currently make Make see it, so a workaround was implemented by export.
* Android install uses a customized NDK as Travis can't extract the "normal" way due to memory constraints.
* A helper script in /ci would be nice to enable people to easily run a whole build locally.
* The `docs` build does not work for some reason, has been reported [upstream](https://github.com/HalfdanJ/ofDocGenerator/issues/3).
* Currently, a `no-op` build is added to the build matrix to work around [a Travis issue](https://github.com/travis-ci/travis-ci/issues/1228).
* Transitioning the Linux build to the new Docker infrastructure works in principle (see b03d1a323cfc1ed284ff1c275a76e5bd75d62b30 and a7cba47dcd1d89724d3871f5e5ca8841cf4a8340), but a large list of packages we need are not [whitelisted](https://github.com/travis-ci/apt-package-whitelist/blob/master/ubuntu-precise) for use, so this has been postponed:
    * libraw1394-dev
    * libassimp-dev
    * librtaudio-dev
    * libgstreamer-plugins-base1.0-dev
    * gstreamer1.0-libav
    * gstreamer1.0-pulseaudio
    * gstreamer1.0-x
    * gstreamer1.0-plugins-bad
    * gstreamer1.0-alsa
    * gstreamer1.0-plugins-base
    * gstreamer1.0-plugins-good
