import qbs
import qbs.Process
import qbs.File
import qbs.FileInfo
import qbs.TextFile
import qbs.Probes
import "helpers.js" as Helpers

Module{
    name: "of"

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
            // For now, we hard-code linux64 as our architecture of choice, since
            // qbs doesn't appear to set the architecture property autimatically
            // anymore starting with qt 4.4.
            //
            // If you wanted to compile for 386, uncomment these lines, and
            // add this property in Build Settings: "qbs.architecture:x86"
            //
//            if(qbs.architecture==="x86_64"){
                return "linux64";
//            }else if(qbs.architecture==="x86"){
//                return "linux";
//            }else{
//                throw("qbs error: Target architecture: '" + qbs.architecture + "' not supported yet on target OS: '" + qbs.targetOS + "'" +
//                      "Check if the project's build settings ");
//            }
        }else if(qbs.targetOS.contains("windows")){
            return "msys2";
        }else if(qbs.targetOS.contains("osx")){
            return "osx";
        }else if(qbs.targetOS.contains("macos")){
            return "osx";
        }else{
            throw("Target architecture: '" + qbs.targetOS + "' not supported yet");
        }
    }

    Depends{
        condition: platform === "android"
        name: "Android.ndk"
    }

    property string sourceDirectory: project.sourceDirectory
    property bool isCoreLibrary: product.isCoreLibrary


    Probe{
        id: CORE
        property stringList includes
        property stringList cflags
        property stringList ldflags
        property stringList system_libs
        property stringList static_libs
        property string platform: parent.platform;
        property string ofRoot: parent.ofRoot;
        property stringList pkgConfigs: parent.pkgConfigs
        configure: {
            includes = [];
            cflags = [];
            ldflags = [];
            system_libs = [];
            static_libs = [];

            // pkgconfig packages
            var configs = [];
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

                configs = pkgs;
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

                configs = pkgs;
            }

            // library exceptions
            var libsexceptions = [];
            if(platform === "linux"  || platform === "linux64"){
                libsexceptions = [
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
                libsexceptions = [
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
                    libsexceptions.push("rtAudio")
                }
            }else if(platform==="osx"){
                libsexceptions = [
                    "openFrameworksCompiled",
                    "videoInput"
                ];
            }else if(platform==="android"){
               libsexceptions =  [
                    "glfw",
                    "fmod",
                    "glew",
                    "kiss",
                    "rtAudio",
                    "videoInput",
                    "openFrameworksCompiled",
                ];
            }

            // parse include search paths from core libraries
            var coreincludes = Helpers.listDirsRecursive(ofRoot + "/libs/openFrameworks");
            var corelibs = Helpers.listDir(ofRoot + '/libs/');
            for(var lib in corelibs){
                if(libsexceptions.indexOf(corelibs[lib])==-1){
                    var libpath = ofRoot + '/libs/' + corelibs[lib];
                    var include_path = libpath + "/include"
                    var include_paths = Helpers.listDirsRecursive(include_path);
                    coreincludes = coreincludes.concat(include_paths);
                }
            }
            includes = coreincludes;

            // add search paths from pkgconfigs;
            if(platform === "linux"  || platform === "linux64" || platform === "msys2"){
                includes = includes.concat(Helpers.pkgconfig(configs, ["--cflags-only-I"]).map(function(element){
                    return element.substr(2).trim()
                }));
            }

            // cflags from pkgconfigs
            if(platform === "linux"  || platform === "linux64" || platform === "msys2"){
                cflags = Helpers.pkgconfig(configs, ["--cflags-only-other"]);
            }else{
                cflags = [];
            }

            // ldflags from pkgconfigs
            if(platform === "linux"  || platform === "linux64" || platform === "msys2"){
                ldflags = Helpers.pkgconfig(configs, ["--libs-only-L"]);
                if(platform === "msys2"){
                    ldflags.push("-L"+FileInfo.joinPaths(Helpers.msys2root(),"mingw32/lib"));
                }else{
                    ldflags.push("-fuse-ld=gold");
                }
            }else{
                ldflags = [];
            }

            // libraries from pkgconfigs
            if(platform === "linux"  || platform === "linux64" || platform === "msys2"){
                var pkgconfiglibs = Helpers.pkgconfig(configs, ["--libs-only-l"]);
                system_libs = pkgconfiglibs.map(function(lib){
                    return lib.substr(2);
                });
            }else{
                system_libs = [];
            }

            // add static libraries from core directories
            static_libs = Helpers.findLibsRecursive(ofRoot + "/libs", platform, libsexceptions);

            found = true;
        }
    }

    Probe {
        id: ADDITIONAL_LIBS
        property bool useStdFs: project.useStdFs
        property stringList libs
        property string platform: parent.platform
        property string compilerName: cpp.compilerName
        property int compilerVersionMajor: cpp.compilerVersionMajor
        configure: {
            if(platform === "linux"  || platform === "linux64"){
                var libslist = [
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
                    "pugixml",
                ];

                if(!Helpers.pkgExists("rtaudio")){
                    libslist.push("rtaudio");
                }

                if(useStdFs && compilerName=='g++' && compilerVersionMajor>=6){
                    libslist.push('stdc++fs');
                }else{
                    libslist.push("boost_filesystem");
                    libslist.push("boost_system");
                }
//                libslist.push("boost_filesystem");
//                libslist.push("boost_system");

                libs = libslist;
            }else if(platform === "msys2"){
                libs = [
                    'opengl32', 'gdi32', 'msimg32', 'glu32', 'winmm', 'strmiids',
                    'uuid', 'oleaut32', 'setupapi', 'wsock32', 'ws2_32', 'Iphlpapi', 'Comdlg32',
                    'freeimage', 'boost_filesystem-mt', 'boost_system-mt', 'freetype', 'pthread',
                    'ksuser', 'ole32', 'dsound',
                ];
            }else if(platform === "android"){
                libs = [
                    'OpenSLES', 'z', 'GLESv1_CM', 'GLESv2', 'log'
                ];
            }else{
                libs = [];
            }
            found = true;
        }
    }

    Probe{
        condition: !of.isCoreLibrary
        id: ADDONS
        property stringList includes
        property pathList sources
        property stringList static_libs
        property stringList system_libs
        property stringList frameworks
        property stringList cflags
        property stringList ldflags
        property stringList defines;
        property bool isCoreLibrary: parent.isCoreLibrary
        property stringList addons: parent.addons
        property string sourceDirectory: parent.sourceDirectory
        property string ofRoot: parent.ofRoot;
        property string platform: parent.platform;

        configure: {
            includes = [];
            sources = [];
            static_libs = [];
            system_libs = [];
            frameworks = [];
            cflags = [];
            ldflags = [];
            defines = [];

            if(isCoreLibrary){
                found = false;
                return;
            }

            // parse list of addons from qbs or addons.make
            var allAddons = [];
            if(addons===undefined){
                // Project addons list is undefined parse addons.make instead
                try{
                    var addonsmake = new TextFile(sourceDirectory + "/addons.make");
                    while(!addonsmake.atEof()){
                        var line = addonsmake.readLine().trim();
                        if(line){
                            allAddons.push(line);
//                            var addonPath = ofRoot + '/addons/' + line;
                        }
                    }
                }catch(e){}
            }else{
                allAddons = addons;
            }

            if(allAddons === undefined || allAddons.length === 0){
                found = false;
                return;
            }

            // map addons list to addons paths
            var addonsPaths = Helpers.removeDuplicates(allAddons.map(function(addon){
                var addonPath = Helpers.normalize(FileInfo.joinPaths(sourceDirectory, addon))
                if(File.exists(addonPath)){
                    return addonPath;
                }else{
                    return Helpers.normalize(FileInfo.joinPaths(ofRoot, '/addons/', addon));
                }
            }));

            // Look for dependencies
            Object.defineProperties(Array.prototype, {
                'flatMap': {
                    value: function (lambda) {
                        return Array.prototype.concat.apply([], this.map(lambda));
                    },
                    writeable: false,
                    enumerable: false
                }
            });
            var dependencies = addonsPaths.flatMap(function(addonPath){
                var dependencies = Helpers.parseAddonConfig(addonPath, "ADDON_DEPENDENCIES", [], platform);
                if(addonPath.startsWith(ofRoot + "/addons")){
                    return dependencies;
                }else{
                    // If it's a local addon try to find dependencies in the same folder
                    var parentAddonFolder = FileInfo.path(addonPath);
                    return dependencies.map(function(dependency){
                        var local = FileInfo.joinPaths(parentAddonFolder, dependency);
                        if(File.exists(local)){
                            return local;
                        }else{
                            return dependency;
                        }
                    })
                }
            });

            allAddons = allAddons.concat(dependencies);

            // Finally map all addons and dependencies to paths
            allAddons = Helpers.removeDuplicates(allAddons.map(function(addon){
                if(FileInfo.isAbsolutePath(addon) && File.exists(addon)){
                    return addon;
                }

                var addonPath = Helpers.normalize(FileInfo.joinPaths(sourceDirectory, addon))
                if(File.exists(addonPath)){
                    return addonPath;
                }else{
                    return Helpers.normalize(FileInfo.joinPaths(ofRoot, '/addons/', addon));
                }
            }));

            // includes
            for(var addon in allAddons){
                var addonPath = allAddons[addon];
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

            // sources
            for(var addon in allAddons){
                var addonPath = allAddons[addon];
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
                addonSources = addonSources.map(function(element){
                    return element.replace("%","*");
                });
                sources = sources.concat(addonSources);
                var addon_config = FileInfo.joinPaths(addonPath,"addon_config.mk");
                if(File.exists(addon_config)){
                    sources.push(addon_config);
                }
            }

            // config cflags
            var config_cflags = [];
            for(var addon in allAddons){
                var addonPath = allAddons[addon];
                config_cflags = config_cflags.concat(Helpers.parseAddonConfig(addonPath, "ADDON_CFLAGS", [], platform))
            }

            // config ldflags
            var config_ldflags = [];
            for(var addon in allAddons){
                var addonPath = allAddons[addon];
                config_ldflags = config_ldflags.concat(Helpers.parseAddonConfig(addonPath, "ADDON_LDFLAGS", [], platform))

                // Remove linker escapes https://doc.qt.io/qbs/qml-qbsmodules-cpp.html#linkerFlags-prop
                config_ldflags = config_ldflags.map(function(element){ return element.replace("-Wl,","") })
                config_ldflags = config_ldflags.map(function(element){ return element.replace("-Xlinker,","") })
            }

            // libs
            for(var addon in allAddons){
                var addonPath = allAddons[addon];
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
                static_libs = static_libs.concat(addonLibs);
            }

            // separate config flags in libs and flags
            var config_libs = config_ldflags
                .filter(function(flag){ return flag.startsWith("-l"); })
                .map(function(flag){ return flag.substr(2); });

            config_ldflags = config_ldflags
                .filter(function(flag){ return !flag.startsWith("-l"); })

            // frameworks
            for(var addon in allAddons){
                var addonPath = allAddons[addon];
                var addonFrameworks = [];
                addonFrameworks = Helpers.parseAddonConfig(addonPath, "ADDON_FRAMEWORKS", addonFrameworks, platform);
                frameworks = frameworks.concat(addonFrameworks);
            }

            // pkg_configs
            var pkgconfigs = [];
            for(var addon in allAddons){
                var addonPath = allAddons[addon];
                pkgconfigs = pkgconfigs.concat(Helpers.parseAddonConfig(addonPath, "ADDON_PKG_CONFIG_LIBRARIES", [], platform))
            }

            // addon defines
            for(var addon in allAddons){
                var addonPath = allAddons[addon];
                defines = defines.concat(Helpers.parseAddonConfig(addonPath, "ADDON_DEFINES", [], platform))
            }

            // pkg includes
            includes = includes.concat(Helpers.pkgconfig(pkgconfigs, ["--cflags-only-I"]).map(function(element){
                return element.substr(2).trim()
            }))

            // pkg cflags
            cflags = Helpers.pkgconfig(pkgconfigs, ["--cflags-only-other"])
                .concat(config_cflags);

            // pkg ldflags
            ldflags = Helpers.pkgconfig(pkgconfigs, ["--libs-only-L"])
                .concat(config_ldflags)

            // pkg config libs
            system_libs = Helpers.pkgconfig(pkgconfigs, ["--libs-only-l"])
                .map(function(flag){ return flag.substr(2); })

            system_libs = system_libs.concat(config_libs)

            found = true;
        }
    }

    Properties{
        condition: !of.isCoreLibrary
        property stringList ADDONS_SOURCES: ADDONS.sources
    }

    Properties{
        condition: !of.isCoreLibrary
        property stringList ADDONS_DEFINES: ADDONS.defines
    }

    Probe{
        id: DEFINES_LINUX
        property stringList list
        property bool useStdFs: project.useStdFs
        property string compilerName: cpp.compilerName
        property int compilerVersionMajor: cpp.compilerVersionMajor
        configure:{
            list = ['GCC_HAS_REGEX'];
            if(Helpers.pkgExists("gtk+-3.0")){
                list.push("OF_USING_GTK=1")
            }
            if(Helpers.pkgExists("libmpg123")){
                list.push("OF_USING_MPG123=1");
            }
            if(useStdFs && compilerName=='g++' && compilerVersionMajor>=6){
                list.push('OF_USING_STD_FS=1');
            }
            found = true;
        }
    }
    Properties{
        condition: qbs.targetOS.contains("linux")
        DEFINES: DEFINES_LINUX.list
    }

    Properties{
        condition: qbs.targetOS.contains("windows")
        DEFINES: ['GCC_HAS_REGEX','UNICODE','_UNICODE']
    }

    Properties{
        condition: !qbs.targetOS.contains("linux") && !qbs.targetOS.contains("windows")
        DEFINES: ['GCC_HAS_REGEX']
    }


    Depends{
        name: "cpp"
    }

    //cpp.cxxLanguageVersion: "c++14"

    property string coreWarningLevel: 'default'
    property stringList coreCFlags: {
        var flags = CORE.cflags
            .concat(['-Wno-unused-parameter','-Werror=return-type'])
            .concat(cFlags);

        if(of.isCoreLibrary){
            return flags
        }else{
            return flags.concat(ADDONS.cflags)
        }

    }

    Properties{
        condition: of.platform === "linux" || of.platform === "linux64" || of.platform === "msys2"
        coreCxxFlags: {
            var flags = CORE.cflags
                .concat(['-Wno-unused-parameter','-Werror=return-type','-std=gnu++14'])
                .concat(cxxFlags);
            if(of.isCoreLibrary){
                return flags
            }else{
                return flags.concat(ADDONS.cflags)
            }
        }

        coreLinkerFlags: {
            var flags = CORE.ldflags
                .concat(linkerFlags);

            if(of.isCoreLibrary){
                return flags;
            }else{
                return flags.concat(ADDONS.ldflags)
            }
        }
    }

    Properties{
        condition: of.platform === "osx"
        coreCxxLanguageVersion: "c++11"
        coreCxxStandardLibrary: "libc++"

        coreCxxFlags: {
            var flags = CORE.cflags
               .concat(['-Wno-unused-parameter','-Werror=return-type'])
                .concat(cxxFlags);

            if(of.isCoreLibrary){
                return flags
            }else{
                return flags.concat(ADDONS.cflags)
            }
        }

        coreFrameworks: {
            var flags = [
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
                'Security',
                'LDAP',
            ].concat(frameworks);

            if(of.isCoreLibrary){
                return flags;
            }else{
                return flags.concat(ADDONS.frameworks);
            }
        }

        coreLinkerFlags:{
            var flags = CORE.ldflags
                .concat(linkerFlags)
            if(of.isCoreLibrary){
                return flags;
            }else{
                return flags.concat(ADDONS.ldflags)
            }
        }
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
        coreCxxFlags: {
            var flags = ['-Wno-unused-parameter','-Werror=return-type','-std=gnu++14']
                .concat('-I'+coreSysroot+'/usr/include')
                .concat('-I'+ndk_root+'/sources/android/support/include')
                .concat('-I'+ndk_root+'/sources/cxx-stl/llvm-libc++/libcxx/include')
                .concat('-I'+ndk_root+'/libs/glu/include_android')
                .concat('-I'+of_root+'/addons/ofxAndroid/src')
                .concat('-Wformat')
                .concat(['-target','armv7-none-linux-androideabi'])
                .concat(['-gcc-toolchain',ndk_root+'/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64'])
                .concat('-DANDROID_NDK')
                .concat(cxxFlags);
            if(of.isCoreLibrary){
                return flags;
            }else{
                return flags.concat(ADDONS.cflags);
            }
        }

        coreLinkerFlags:{
            var flags = CORE.ldflags
                .concat('-L"'+ndk_root+'/sources/cxx-stl/llvm-libc++/libs/'+abiPath+'"')
                .concat(['-target', 'armv7-none-linux-androideabi'])
                .concat(['-gcc-toolchain', ndk_root+'/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64'])
                //.concat('-Wl,--as-needed -Wl,--gc-sections -Wl,--exclude-libs,ALL')
                .concat(linkerFlags);

            if(of.isCoreLibrary){
                return flags;
            }else{
                return flags.concat(ADDONS.ldflags);
            }
        }
    }

    property stringList pkgConfigs: []
    property pathList   includePaths: []
    property stringList cFlags: []
    property stringList cxxFlags: []
    property stringList linkerFlags: []
    property stringList defines: []
    property stringList frameworks: []
    property stringList staticLibraries: []
    property stringList dynamicLibraries: []
    property stringList addons

    property stringList coreIncludePaths: {
        var flags = CORE.includes
            .concat(includePaths);
        if(of.isCoreLibrary){
            return flags;
        }else{
            return flags.concat(ADDONS.includes)
        }
    }

    property stringList coreStaticLibs: {
        if(of.isCoreLibrary){
            return CORE.static_libs;
        }else{
            return ADDONS.static_libs.concat(CORE.static_libs);
        }
    }

    property stringList coreSystemLibs: {
        if(of.isCoreLibrary){
            return CORE.system_libs
                .concat(ADDITIONAL_LIBS.libs);
        }else{
            return ADDONS.system_libs
                .concat(CORE.system_libs)
                .concat(ADDITIONAL_LIBS.libs);
        }
    }

    Properties{
        condition: qbs.buildVariant.contains("debug")
        coreDefines: ['DEBUG'].concat(DEFINES).concat(defines)
    }

    Properties{
        condition: qbs.buildVariant.contains("release")
        coreDefines: ['NDEBUG'].concat(DEFINES).concat(defines)
    }
}
