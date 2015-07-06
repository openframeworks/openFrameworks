curl -Lk http://dl.google.com/android/ndk/android-ndk-r10e-darwin-x86_64.bin -o ndk.bin
chmod a+x ndk.bin
./ndk.bin -y | grep -v Extracting
NDK_ROOT=$(echo ${PWD} | sed "s/\//\\\\\//g")
cat libs/openFrameworksCompiled/project/android/paths.default.make | sed s/path_to/${NDK_ROOT}/ > libs/openFrameworksCompiled/project/android/paths.make
