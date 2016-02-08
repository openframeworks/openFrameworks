import qbs
import qbs.Process
import qbs.File
import qbs.FileInfo
import qbs.TextFile
import "helpers.js" as Helpers

Module{
    name: "ofCore"
    property string msys2root: "c:/msys64"

    property string ofRoot: {
        if(FileInfo.isAbsolutePath(project.of_root)){
            return project.of_root;
        }else{
            return FileInfo.joinPaths(project.sourceDirectory, project.of_root);
        }
    }

    property string platform: {
        if(qbs.targetOS.contains("linux")){
            if(qbs.architecture==="x86_64"){
                return "linux64";
            }else if(qbs.architecture==="x86"){
                return "linux";
            }else{
                throw(qbs.architecture + " not supported yet");
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
        if(qbs.targetOS.indexOf("linux")!=-1){
            return [
                "glew",
                "cairo",
                "glu",
                "poco",
                "quicktime",
                "videoInput",
                "freetype",
                "FreeImage",
                "assimp",
                "glut",
                "rtAudio",
                "openssl",
                "boost",
                "openFrameworksCompiled"
            ];
        }else if(platform==="msys2"){
            return [
                "glew",
                "cairo",
                "poco",
                "freetype",
                "FreeImage",
                "assimp",
                "glut",
                "rtAudio",
                "openssl",
                "boost",
                "openFrameworksCompiled"
            ];
        }else if(platform==="osx"){
            return [
                "poco",
                "quicktime",
                "glut",
                "openFrameworksCompiled",
            ];

        }
    }

    readonly property stringList PKG_CONFIGS: {
        if(qbs.targetOS.indexOf("linux")!=-1){
            return [
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
                "openssl",
                "libpulse-simple",
                "alsa",
                "gl",
                "glu",
                "glew",
                "gtk+-3.0",
                "libmpg123",
            ].concat(pkgConfigs)
        }else if(qbs.targetOS.indexOf("windows")!=-1){
            return [
                "zlib",
                "openssl",
                "glew",
            ].concat(pkgConfigs)
        }else{
            return [];
        }
    }

    readonly property stringList ADDITIONAL_LIBS: {
        if(qbs.targetOS.contains("linux")){
            return [
                "glut",
                "X11",
                "Xrandr",
                "Xxf86vm",
                "Xi",
                "Xcursor",
                "dl",
                "pthread",
                "freeimage",
                "rtaudio",
                "boost_filesystem",
                "boost_system",
            ];
        }else if(platform === "msys2"){
            return [
                'opengl32', 'gdi32', 'msimg32', 'glu32', 'dsound', 'winmm', 'strmiids',
                'uuid', 'ole32', 'oleaut32', 'setupapi', 'wsock32', 'ws2_32', 'Iphlpapi', 'Comdlg32',
                'freeimage', 'boost_filesystem-mt', 'boost_system-mt', 'freetype', 'cairo','pthread'
            ];
        }
    }

    readonly property stringList PKG_CONFIG_INCLUDES: {
        if(platform.contains("linux") || platform === "msys2"){
            return Helpers.pkgconfig(PKG_CONFIGS,["--cflags-only-I"]).map(function(element){
                return element.substr(2).trim()
            });
        }else{
            return [];
        }
    }

    readonly property stringList PKG_CONFIG_CFLAGS: {
        if(platform.contains("linux") || platform === "msys2"){
            return (Helpers.pkgconfig(PKG_CONFIGS,["--cflags-only-other"]));
        }else{
            return [];
        }
    }

    readonly property stringList PKG_CONFIG_LDFLAGS: {
        if(platform.contains("linux") || platform === "msys2"){
            return (Helpers.pkgconfig(PKG_CONFIGS,["--libs"]));
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
        includes.push(ofRoot+'/libs/poco/include');
        includes = includes.concat(PKG_CONFIG_INCLUDES);
        if(platform === "msys2"){
            includes.push(FileInfo.joinPaths(msys2root,'mingw32/include'));
            includes.push(FileInfo.joinPaths(msys2root,'mingw32/include/cairo'));
            includes.push(FileInfo.joinPaths(msys2root,'mingw32/include/glib-2.0'));
            includes.push(FileInfo.joinPaths(msys2root,'mingw32/lib/glib-2.0/include'));
            includes.push(FileInfo.joinPaths(msys2root,'mingw32/include/pixman-1'));
            includes.push(FileInfo.joinPaths(msys2root,'mingw32/include/freetype2'));
            includes.push(FileInfo.joinPaths(msys2root,'mingw32/include/harfbuzz'));
            includes.push(FileInfo.joinPaths(msys2root,'mingw32/include/libpng16'));
        }

        return includes;
    }

    readonly property pathList STATIC_LIBS: {
        var staticLibraries = Helpers.findLibsRecursive(ofRoot + "/libs",platform,LIBS_EXCEPTIONS);
        if(platform === "osx"){
            staticLibraries.push(ofRoot + '/libs/poco/lib/' + platform + '/PocoNetSSL.a');
            staticLibraries.push(ofRoot + '/libs/poco/lib/' + platform + '/PocoNet.a');
            staticLibraries.push(ofRoot + '/libs/poco/lib/' + platform + '/PocoCrypto.a');
            staticLibraries.push(ofRoot + '/libs/poco/lib/' + platform + '/PocoUtil.a');
            staticLibraries.push(ofRoot + '/libs/poco/lib/' + platform + '/PocoJSON.a');
            staticLibraries.push(ofRoot + '/libs/poco/lib/' + platform + '/PocoXML.a');
            staticLibraries.push(ofRoot + '/libs/poco/lib/' + platform + '/PocoFoundation.a');
        }else{
            staticLibraries.push(ofRoot + '/libs/poco/lib/' + platform + '/libPocoNetSSL.a');
            staticLibraries.push(ofRoot + '/libs/poco/lib/' + platform + '/libPocoNet.a');
            staticLibraries.push(ofRoot + '/libs/poco/lib/' + platform + '/libPocoCrypto.a');
            staticLibraries.push(ofRoot + '/libs/poco/lib/' + platform + '/libPocoUtil.a');
            staticLibraries.push(ofRoot + '/libs/poco/lib/' + platform + '/libPocoJSON.a');
            staticLibraries.push(ofRoot + '/libs/poco/lib/' + platform + '/libPocoXML.a');
            staticLibraries.push(ofRoot + '/libs/poco/lib/' + platform + '/libPocoFoundation.a');
        }
        return(staticLibraries)
    }

    readonly property stringList LDFLAGS: {
        var ret = PKG_CONFIG_LDFLAGS;
        for(lib in ADDITIONAL_LIBS){
            var libname = ADDITIONAL_LIBS[lib].trim();
            if(libname!=""){
                ret.push("-l" + libname);
            }
        }
        if(qbs.targetOS.contains("windows")){
            ret.push("-L"+FileInfo.joinPaths(msys2root,"mingw32/lib"));
            //ret.push("-fuse-ld=gold");
        }

        return STATIC_LIBS.concat(ret);
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
        return Helpers.pkgconfig(ADDON_PKG_CONFIGS,["--libs"])
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
            defines.concat([ 'OF_USING_GTK', 'OF_USING_MPG123']);
        }

        if(qbs.targetOS.indexOf("windows")>-1){
            defines.concat(['UNICODE','_UNICODE','POCO_STATIC']);
        }
        return defines;
    }

    property stringList pkgConfigs: []
    property pathList includePaths: []
    property stringList cFlags: []
    property stringList cxxFlags: []
    property stringList linkerFlags: []
    property stringList defines: []
    property stringList frameworks: []

    Depends{
        name: "cpp"
    }

    Depends{
        condition: platform==="osx"
        name: "bundle"
    }

    //cpp.cxxLanguageVersion: "c++14"
    cpp.warningLevel: 'default'
    cpp.cFlags: PKG_CONFIG_CFLAGS
        .concat(['-Wno-unused-parameter'])
        .concat(ADDON_PKG_CONFIG_CFLAGS)
        .concat(ADDON_CFLAGS)
        .concat(cFlags)

    Properties{
        condition: qbs.targetOS.contains("linux") || platform === "msys2"
        cpp.cxxFlags: PKG_CONFIG_CFLAGS
            .concat(['-Wno-unused-parameter','-std=gnu++14'])
            .concat(ADDON_PKG_CONFIG_CFLAGS)
            .concat(ADDON_CFLAGS)
            .concat(cxxFlags)
    }

    Properties{
        condition: platform === "osx"
        cpp.cxxLanguageVersion: "c++11"
        cpp.cxxStandardLibrary: "libc++"

        cpp.cxxFlags: PKG_CONFIG_CFLAGS
            .concat(['-Wno-unused-parameter'])
            .concat(ADDON_PKG_CONFIG_CFLAGS)
            .concat(ADDON_CFLAGS)
            .concat(cxxFlags)

        cpp.frameworks: [
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
    }

    Properties{
        condition: platform === "msys2"
        cpp.cxxStandardLibrary: ""
    }

    Properties{
        condition: qbs.buildVariant.contains("debug") && platform === "osx"
        bundle.infoPlist: ({"CFBundleIconFile":"icon-debug.icns"})
    }

    Properties{
        condition: qbs.buildVariant.contains("release") && platform === "osx"
        bundle.infoPlist: ({"CFBundleIconFile":"icon.icns"})
    }

    cpp.includePaths: INCLUDE_PATHS
        .concat(ADDON_INCLUDES)
        .concat(ADDON_PKG_CONFIG_INCLUDES)
        .concat(includePaths)

    cpp.linkerFlags:
        LDFLAGS
        .concat(ADDON_LIBS)
        .concat(ADDON_PKG_CONFIG_LDFLAGS)
        .concat(ADDON_LDFLAGS)
        .concat(linkerFlags)

    Properties{
        condition: qbs.buildVariant.contains("debug")
        cpp.defines: ['DEBUG'].concat(DEFINES).concat(defines)
    }

    Properties{
        condition: qbs.buildVariant.contains("release")
        cpp.defines: ['NDEBUG'].concat(DEFINES).concat(defines)
    }

    Group{
        name: "addons"
        files: of.ADDONS_SOURCES
    }
}
