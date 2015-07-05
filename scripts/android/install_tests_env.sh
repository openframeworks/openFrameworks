curl -Lk http://dl.google.com/android/ndk/android-ndk-r10e-darwin-x86_64.bin -o ndk.bin
chmod a+x ndk.bin
./ndk.bin -y | grep -v Extracting
cat libs/openFrameworksCompiled/project/android/paths.default.make | sed s/path_to/${PWD}/ > libs/openFrameworksCompiled/project/android/paths.make
