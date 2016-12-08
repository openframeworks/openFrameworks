import qbs
import qbs.Process
import qbs.File
import qbs.FileInfo
import qbs.TextFile
import "helpers.js" as Helpers

Module{
    name: "ofCore"

    property string ofRoot: {
        if(FileInfo.isAbsolutePath(project.of_root)){
            return project.of_root;
        }else{
            return FileInfo.joinPaths(project.sourceDirectory, project.of_root);
        }
    }

    property string platform: {
        if(qbs.targetOS.contains("android")){
            return "android";
        }else if(qbs.targetOS.contains("linux")){
            if(qbs.architecture==="x86_64"){
                return "linux64";
            }else if(qbs.architecture==="x86"){
                return "linux";
            }else{
                throw(qbs.architecture + " not supported yet on " + qbs.targetOS);
            }
        }else if(qbs.targetOS.contains("windows")){
            return "msys2";
        }else if(qbs.targetOS.contains("osx")){
            return "osx";
        }else{
            throw(qbs.targetOS + " not supported yet");
        }
    }

    property stringList addons

    readonly property stringList LIBS_EXCEPTIONS: {
        if(platform === "linux"  || platform === "linux64"){
            return [
                "glew",
                "cairo",
                "videoInput",
                "freetype",
                "FreeImage",
                "assimp",
                "rtAudio",
                "openssl",
                "boost",
                "glfw",
                "openFrameworksCompiled",
            ];
        }else if(platform==="msys2"){
            var exceptions = [
                "glew",
                "cairo",
                "freetype",
                "FreeImage",
                "assimp",
                "openssl",
                "boost",
                "glfw",
                "openFrameworksCompiled"
            ];
            if(Helpers.pkgExists("rtaudio")){
                exceptions.push("rtAudio")
            }
            return exceptions;
        }else if(platform==="osx"){
            var exceptions = [
                "openFrameworksCompiled",
                "videoInput"
            ];
            return exceptions;
        }else if(platform==="android"){
            var exceptions =  [
                "glfw",
                "fmodex",
                "glew",
                "kiss",
                "rtAudio",
                "videoInput",
                "openFrameworksCompiled",
            ];
            return exceptions;
        }
    }

    readonly property stringList PKG_CONFIGS: {
        if(platform === "linux"  || platform === "linux64"){
            var pkgs = [
                "cairo",
                "gstreamer-1.0",
                "zlib",
                "gstreamer-app-1.0",
                "gstreamer-video-1.0",
                "gstreamer-base-1.0",
                "libudev",
                "freetype2",
                "fontconfig",
                "sndfile",
                "openal",
                "libpulse-simple",
                "alsa",
                "gl",
                "glu",
                "glew",
                "gtk+-3.0",
                "libmpg123",
                "glfw3",
                "liburiparser",
                "libcurl",
            ].concat(pkgConfigs);


            if(Helpers.pkgExists("rtaudio")){
                pkgs.push("rtaudio");
            }
            if(Helpers.pkgExists("libmpg123")){
                pkgs.push("libmpg123");
            }
            if(Helpers.pkgExists("gtk+-3.0")){
                pkgs.push("gtk+-3.0")
            }

            return pkgs;
        }else if(platform === "msys2"){
            var pkgs = [
                "cairo",
                "zlib",
                "glew",
                "glfw3",
                "libcurl",
                "openssl",
            ].concat(pkgConfigs);

            if(Helpers.pkgExists("rtaudio")){
                pkgs.push("rtaudio");
            }

            return pkgs;
        }else{
            return [];
        }
    }

    readonly property stringList ADDITIONAL_LIBS: {
        if(platform === "linux"  || platform === "linux64"){
            var libs = [
                "glut",
                "X11",
                "Xrandr",
                "Xxf86vm",
                "Xi",
                "Xcursor",
                "Xinerama",
                "dl",
                "pthread",
                "freeimage",
                "boost_filesystem",
                "boost_system",
                "pugixml",
            ];

            if(!Helpers.pkgExists("rtaudio")){
                libs.push("rtaudio");
            }

            return libs;
        }else if(platform === "msys2"){
            return [
                'opengl32', 'gdi32', 'msimg32', 'glu32', 'winmm', 'strmiids',
                'uuid', 'oleaut32', 'setupapi', 'wsock32', 'ws2_32', 'Iphlpapi', 'Comdlg32',
                'freeimage', 'boost_filesystem-mt', 'boost_system-mt', 'freetype', 'pthread',
                'ksuser', 'ole32', 'dsound',
            ];
        }else if(platform === "android"){
            return [
                'OpenSLES', 'z', 'GLESv1_CM', 'GLESv2', 'log'
            ];
        }else return [];
    }

    readonly property stringList PKG_CONFIG_INCLUDES: {
        if(platform === "linux"  || platform === "linux64" || platform === "msys2"){
            return Helpers.pkgconfig(PKG_CONFIGS,["--cflags-only-I"]).map(function(element){
                return element.substr(2).trim()
            });
        }else{
            return [];
        }
    }

    readonly property stringList PKG_CONFIG_CFLAGS: {
        if(platform === "linux"  || platform === "linux64" || platform === "msys2"){
            return (Helpers.pkgconfig(PKG_CONFIGS,["--cflags-only-other"]));
        }else{
            return [];
        }
    }

    readonly property stringList PKG_CONFIG_LDFLAGS: {
        if(platform === "linux"  || platform === "linux64" || platform === "msys2"){
            return (Helpers.pkgconfig(PKG_CONFIGS,["--libs-only-L"]));
        }else{
            return [];
        }
    }

    readonly property stringList PKG_CONFIG_LIBS: {
        if(platform === "linux"  || platform === "linux64" || platform === "msys2"){
            var pkgconfiglibs = Helpers.pkgconfig(PKG_CONFIGS,["--libs-only-l"]);
            return pkgconfiglibs.map(function(lib){
                return lib.substr(2);
            });
        }else{
            return [];
        }
    }

    readonly property pathList INCLUDE_PATHS: {
        var includes = Helpers.listDirsRecursive(ofRoot + "/libs/openFrameworks");
        var libs = Helpers.listDir(ofRoot + '/libs/');
        for(var lib in libs){
            if(LIBS_EXCEPTIONS.indexOf(libs[lib])==-1){
                var libpath = ofRoot + '/libs/' + libs[lib];
                var include_path = libpath + "/include"
                var include_paths = Helpers.listDirsRecursive(include_path);
                includes = includes.concat(include_paths);
            }
        }
        includes = includes.concat(PKG_CONFIG_INCLUDES);

        return includes;
    }

    Depends{
        condition: platform === "android"
        name: "Android.ndk"
    }

    readonly property pathList STATIC_LIBS: {
        var staticLibraries = Helpers.findLibsRecursive(ofRoot + "/libs",platform,LIBS_EXCEPTIONS);
        return staticLibraries
    }

    readonly property stringList LDFLAGS: {
        var ret = PKG_CONFIG_LDFLAGS;
        if(platform === "msys2"){
            ret.push("-L"+FileInfo.joinPaths(Helpers.msys2root(),"mingw32/lib"));
            //ret.push("-fuse-ld=gold");
        }
        return ret;
    }

    readonly property stringList addonsMake: {
        var allAddons = [];
        try{
            var addonsmake = new TextFile(project.sourceDirectory + "/addons.make");
            while(!addonsmake.atEof()){
                var line = addonsmake.readLine().trim();
                allAddons.push(line);
                var addonPath = ofRoot + '/addons/' + line;
                var dependencies = Helpers.parseAddonConfig(addonPath, "ADDON_DEPENDENCIES", [], platform);
                allAddons = allAddons.concat(dependencies);
            }
        }catch(e){}
        return allAddons;
    }

    readonly property stringList ADDONS: {
        var allAddons = [];
        if(addons===undefined){
            allAddons = addonsMake;
        }else{
            allAddons = addons;
        }

        return Helpers.removeDuplicates(allAddons.map(function(addon){
            var addonPath = Helpers.normalize(FileInfo.joinPaths(project.sourceDirectory, addon))
            if(File.exists(addonPath)){
                return addonPath;
            }else{
                return Helpers.normalize(FileInfo.joinPaths(ofRoot, '/addons/', addon));
            }
        }));
    }

    readonly property stringList ADDON_INCLUDES: {
        var includes = [];
        for(var addon in ADDONS){
            var addonPath = ADDONS[addon];
            var addonIncludes = Helpers.addonIncludes(addonPath);
            addonIncludes = Helpers.parseAddonConfig(addonPath, "ADDON_INCLUDES", addonIncludes, platform, addonPath+"/");
            var addonIncludesExcludes = Helpers.parseAddonConfig(addonPath, "ADDON_INCLUDES_EXCLUDE", [], platform, addonPath+"/");
            if(addonIncludesExcludes.length>0){
                addonIncludes = addonIncludes.filter(function(element){
                    for(var exclude in addonIncludesExcludes){
                        var exclude = addonIncludesExcludes[exclude].replace("%",".*");
                        var patt = new RegExp(exclude);
                        var match = patt.exec(element);
                        if(match!=null){
                            return false;
                        }
                    }
                    return true;
                });
            }
            includes = includes.concat(addonIncludes);
        }
        return includes;
    }

    readonly property pathList ADDONS_SOURCES: {
        var sources = [];
        for(var addon in ADDONS){
            var addonPath = ADDONS[addon];
            var addonSources = Helpers.addonSources(addonPath);
            addonSources = Helpers.parseAddonConfig(addonPath, "ADDON_SOURCES", addonSources, platform, addonPath+"/");
            var addonSourcesExcludes = Helpers.parseAddonConfig(addonPath, "ADDON_SOURCES_EXCLUDE", [], platform, addonPath+"/");
            if(addonSourcesExcludes.length>0){
                addonSources = addonSources.filter(function(element){
                    for(var exclude in addonSourcesExcludes){
                        var exclude = addonSourcesExcludes[exclude].replace("%",".*");
                        var patt = new RegExp(exclude);
                        var match = patt.exec(element);
                        if(match!=null){
                            return false;
                        }
                    }
                    return true;
                });
            }
            sources = sources.concat(addonSources);
            var addon_config = FileInfo.joinPaths(addonPath,"addon_config.mk");
            if(File.exists(addon_config)){
                sources.push(addon_config);
            }
        }
        return sources;
    }

    readonly property stringList ADDON_LIBS: {
        var libs = [];
        for(var addon in ADDONS){
            var addonPath = ADDONS[addon];
            var addonLibs = Helpers.findLibsRecursive(addonPath + "/libs", platform, []);
            addonLibs = Helpers.parseAddonConfig(addonPath, "ADDON_LIBS", addonLibs, platform, addonPath+"/");
            var addonLibsExcludes = Helpers.parseAddonConfig(addonPath, "ADDON_LIBS_EXCLUDE", [], platform, addonPath+"/");
            if(addonLibsExcludes.length>0){
                addonLibs = addonLibs.filter(function(element){
                    for(var exclude in addonLibsExcludes){
                        var exclude = addonLibsExcludes[exclude].replace("%",".*");
                        var patt = new RegExp(exclude);
                        var match = patt.exec(element);
                        if(match!=null){
                            return false;
                        }
                    }
                    return true;
                });
            }
            libs = libs.concat(addonLibs);
        }

        libs = libs.concat(ADDON_LDFLAGS
            .filter(function(flag){ return flag.startsWith("-l"); })
            .map(function(flag){ return flag.substr(2); }))

        return libs;
    }

    readonly property stringList ADDON_FRAMEWORKS: {
        var frameworks = [];
        for(var addon in ADDONS){
            var addonPath = ADDONS[addon];
            var addonFrameworks = [];
            addonFrameworks = Helpers.parseAddonConfig(addonPath, "ADDON_FRAMEWORKS", addonFrameworks, platform, addonPath+"/");
            frameworks = frameworks.concat(addonFrameworks);
        }
        return frameworks;
    }

    readonly property stringList ADDON_PKG_CONFIGS: {
        var pkgconfigs = [];
        for(var addon in ADDONS){
            var addonPath = ADDONS[addon];
            pkgconfigs = pkgconfigs.concat(Helpers.parseAddonConfig(addonPath, "ADDON_PKG_CONFIG_LIBRARIES", [], platform))
        }
        return pkgconfigs;
    }

    readonly property stringList ADDON_PKG_CONFIG_INCLUDES: {
        return Helpers.pkgconfig(ADDON_PKG_CONFIGS,["--cflags-only-I"]).map(function(element){
            return element.substr(2).trim()
        })
    }

    readonly property stringList ADDON_PKG_CONFIG_CFLAGS: {
        return Helpers.pkgconfig(ADDON_PKG_CONFIGS,["--cflags-only-other"])
    }

    readonly property stringList ADDON_PKG_CONFIG_LDFLAGS: {
        return Helpers.pkgconfig(ADDON_PKG_CONFIGS,["--libs-only-L"])
    }

    readonly property stringList ADDON_PKG_CONFIG_LIBS: {
        return Helpers.pkgconfig(ADDON_PKG_CONFIGS,["--libs-only-l"])
            .map(function(flag){ return flag.substr(2); })
    }

    readonly property stringList ADDON_CFLAGS: {
        var cflags = [];
        for(var addon in ADDONS){
            var addonPath = ADDONS[addon];
            cflags = cflags.concat(Helpers.parseAddonConfig(addonPath, "ADDON_CFLAGS", [], platform))
        }
        return cflags;
    }

    readonly property stringList ADDON_LDFLAGS: {
        var ldflags = [];
        for(var addon in ADDONS){
            var addonPath = ADDONS[addon];
            ldflags = ldflags.concat(Helpers.parseAddonConfig(addonPath, "ADDON_LDFLAGS", [], platform))
        }
        return ldflags;
    }

    readonly property stringList DEFINES: {
        var defines = ['GCC_HAS_REGEX'];

        if(qbs.targetOS.contains("linux")){
            if(Helpers.pkgExists("gtk+-3.0")){
                defines.push("OF_USING_GTK")
            }
            if(Helpers.pkgExists("libmpg123")){
                defines.push("OF_USING_MPG123");
            }
        }

        if(qbs.targetOS.indexOf("windows")>-1){
            defines = defines.concat(['UNICODE','_UNICODE']);
        }
        
        return defines;
    }


    Depends{
        name: "cpp"
    }

    //cpp.cxxLanguageVersion: "c++14"
    coreWarningLevel: 'default'
    coreCFlags: PKG_CONFIG_CFLAGS
        .concat(['-Wno-unused-parameter'])
        .concat(ADDON_PKG_CONFIG_CFLAGS)
        .concat(ADDON_CFLAGS)
        .concat(cFlags)

    Properties{
        condition: of.platform === "linux" || of.platform === "linux64" || of.platform === "msys2"
        coreCxxFlags: PKG_CONFIG_CFLAGS
            .concat(['-Wno-unused-parameter','-std=gnu++14'])
            .concat(ADDON_PKG_CONFIG_CFLAGS)
            .concat(ADDON_CFLAGS)
            .concat(cxxFlags)

        coreLinkerFlags:
            LDFLAGS
            .concat(ADDON_PKG_CONFIG_LDFLAGS)
            .concat(ADDON_LDFLAGS)
            .concat(linkerFlags)
    }

    Properties{
        condition: of.platform === "osx"
        coreCxxLanguageVersion: "c++11"
        coreCxxStandardLibrary: "libc++"

        coreCxxFlags: PKG_CONFIG_CFLAGS
            .concat(['-Wno-unused-parameter'])
            .concat(ADDON_PKG_CONFIG_CFLAGS)
            .concat(ADDON_CFLAGS)
            .concat(cxxFlags)

        coreFrameworks: [
                'Accelerate',
                'AGL',
                'AppKit',
                'ApplicationServices',
                'AudioToolbox',
                'AVFoundation',
                'Cocoa',
                'CoreAudio',
                'CoreFoundation',
                'CoreMedia',
                'CoreServices',
                'CoreVideo',
                'IOKit',
                'OpenGL',
                'QuartzCore',
        ].concat(frameworks)
         .concat(ADDON_FRAMEWORKS)

        coreLinkerFlags:
            LDFLAGS
            .concat(ADDON_LDFLAGS)
            .concat(linkerFlags)
    }

    Properties{
        condition: of.platform === "msys2"
        coreCxxStandardLibrary: ""
    }

    Properties{
        condition: qbs.targetOS.contains("android")
        readonly property string ndk_root: Android.ndk.ndkDir
        //readonly property string toolchainVersion: '4.9'
        readonly property string abiPath: Android.ndk.abi
        coreSysroot: ndk_root + '/platforms/android-19/arch-arm'
        coreCxxFlags: ['-Wno-unused-parameter','-std=gnu++14']
            .concat(ADDON_CFLAGS)
            .concat('-I'+coreSysroot+'/usr/include')
            .concat('-I'+ndk_root+'/sources/android/support/include')
            .concat('-I'+ndk_root+'/sources/cxx-stl/llvm-libc++/libcxx/include')
            .concat('-I'+ndk_root+'/libs/glu/include_android')
            .concat('-I'+of_root+'/addons/ofxAndroid/src')
            .concat('-Wformat')
            .concat(['-target','armv7-none-linux-androideabi'])
            .concat(['-gcc-toolchain',ndk_root+'/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64'])
            .concat('-DANDROID_NDK')
            .concat(cxxFlags)

        coreLinkerFlags:
            LDFLAGS
            .concat(ADDON_LDFLAGS)
            .concat('-L"'+ndk_root+'/sources/cxx-stl/llvm-libc++/libs/'+abiPath+'"')
            .concat(['-target', 'armv7-none-linux-androideabi'])
            .concat(['-gcc-toolchain', ndk_root+'/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64'])
            //.concat('-Wl,--as-needed -Wl,--gc-sections -Wl,--exclude-libs,ALL')
            .concat(linkerFlags)
    }

    property stringList pkgConfigs: []
    property pathList includePaths: []
    property stringList cFlags: []
    property stringList cxxFlags: []
    property stringList linkerFlags: []
    property stringList defines: []
    property stringList frameworks: []

    coreIncludePaths: INCLUDE_PATHS
        .concat(ADDON_INCLUDES)
        .concat(ADDON_PKG_CONFIG_INCLUDES)
        .concat(includePaths)

    coreStaticLibs: ADDON_LIBS
        .concat(ADDON_PKG_CONFIG_LIBS)
        .concat(STATIC_LIBS)
        .concat(PKG_CONFIG_LIBS)
        .concat(ADDITIONAL_LIBS)


    Properties{
        condition: qbs.buildVariant.contains("debug")
        coreDefines: ['DEBUG'].concat(DEFINES).concat(defines)
    }

    Properties{
        condition: qbs.buildVariant.contains("release")
        coreDefines: ['NDEBUG'].concat(DEFINES).concat(defines)
    }
}
