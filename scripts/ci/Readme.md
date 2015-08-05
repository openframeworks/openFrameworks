# CI testing OF on Travis

This folder contains the scripts necessary for the Continuous Integration tests running on travis-ci.org.

## Documented preconditions
* Currently, scripts expect the `$TRAVIS_BUILD_DIR` environment variable to exist, but falls back to automatically getting this from the script location with [a nice trick](http://stackoverflow.com/a/16879155/599884) and some help from [SO](http://stackoverflow.com/a/4774063/599884).
* The docs target relies on `$TRAVIS_PR`

## Future fixes
* (New Ubuntu) remove the -no-install-recommends from the docs before_install script
* (New Ubuntu) probably remove the gcc export from .travis.yml
* In `linux/build.sh`, exporting `$CXX` does not currently make Make see it, so a workaround was implemented
* Android install uses a customized NDK as Travis can't extract the "normal" way due to memory constraints
* A helper script in /ci would be nice to enable people to easily run a whole build locally
