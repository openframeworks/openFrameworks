# CI testing OF on Travis

This folder contains the scripts necessary for the Continuous Integration tests running on travis-ci.org.

## Documented preconditions
* Currently, scripts expect the `$TRAVIS_BUILD_DIR` environment variable to exist, but falls back to automatically getting this from the script location with [a nice trick](http://stackoverflow.com/a/16879155/599884) and some help from [SO](http://stackoverflow.com/a/4774063/599884).
* The docs target relies on `$TRAVIS_PR`
* The Android build downloads a stripped NDK from our server. The list of stripped things is at scripts/ci/android/NDK_excludes.txt for documentation.
* The build cache for Android builds currently has to be manually cleared when changing the NDK/toolchain version, and a new stripped NDK probably has to be uploaded to our server. A more granular cache test in scripts/ci/android/install.sh could make this unnecessary.

## Future fixes/improvements
* (New Ubuntu) remove the -no-install-recommends from the docs before_install script
* (New Ubuntu) probably remove the gcc export from .travis.yml
* In `linux/build.sh`, exporting `$CXX` does not currently make Make see it, so a workaround was implemented
* Android install uses a customized NDK as Travis can't extract the "normal" way due to memory constraints
* A helper script in /ci would be nice to enable people to easily run a whole build locally
