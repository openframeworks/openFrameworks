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
        if(qbs.targetOS.indexOf("linux")>-1){
            if(qbs.architecture==="x86_64"){
                return "linux64";
            }else if(qbs.architecture==="x86"){
                return "linux";
            }else{
                throw(qbs.architecture + " not supported yet")
            }
        }else{
            throw(qbs.targetOS + " not supported yet")
        }
    }
    property stringList addons

    readonly property stringList LIBS_EXCEPTIONS: [
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
    ]

    readonly property stringList PKG_CONFIGS: {
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
    }

    readonly property stringList ADDITIONAL_LIBS: [
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
    ]

    readonly property stringList PKG_CONFIG_INCLUDES: {
        return Helpers.pkgconfig(PKG_CONFIGS,["--cflags-only-I"]).map(function(element){
            return element.substr(2).trim()
        })
    }

    readonly property stringList PKG_CONFIG_CFLAGS: {
        return (Helpers.pkgconfig(PKG_CONFIGS,["--cflags-only-other"]))
    }

    readonly property stringList PKG_CONFIG_LDFLAGS: {
        return (Helpers.pkgconfig(PKG_CONFIGS,["--libs"]))
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
        return includes;
    }

    readonly property pathList STATIC_LIBS: {
        var staticLibraries = Helpers.findLibsRecursive(ofRoot + "/libs",platform,LIBS_EXCEPTIONS);
        staticLibraries.push(ofRoot + '/libs/poco/lib/' + platform + '/libPocoNetSSL.a');
        staticLibraries.push(ofRoot + '/libs/poco/lib/' + platform + '/libPocoNet.a');
        staticLibraries.push(ofRoot + '/libs/poco/lib/' + platform + '/libPocoCrypto.a');
        staticLibraries.push(ofRoot + '/libs/poco/lib/' + platform + '/libPocoUtil.a');
        staticLibraries.push(ofRoot + '/libs/poco/lib/' + platform + '/libPocoJSON.a');
        staticLibraries.push(ofRoot + '/libs/poco/lib/' + platform + '/libPocoXML.a');
        staticLibraries.push(ofRoot + '/libs/poco/lib/' + platform + '/libPocoFoundation.a');
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
        return STATIC_LIBS.concat(ret);
    }

    readonly property stringList ADDONS: {
        var allAddons = [];
        if(addons===undefined){
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

    readonly property stringList DEFINES: ['GCC_HAS_REGEX', 'OF_USING_GTK', 'OF_USING_MPG123']

    property stringList pkgConfigs: []
    property pathList includePaths: []
    property stringList cFlags: []
    property stringList cxxFlags: []
    property stringList linkerFlags: []
    property stringList defines: []

    Depends{
        name: "cpp"
    }

    //cpp.cxxLanguageVersion: "c++14"
    cpp.warningLevel: 'default'
    cpp.cFlags: PKG_CONFIG_CFLAGS
        .concat(['-Wno-unused-parameter', '-std=c++14'])
        .concat(ADDON_PKG_CONFIG_CFLAGS)
        .concat(ADDON_CFLAGS)
        .concat(cFlags)

    cpp.cxxFlags: PKG_CONFIG_CFLAGS
        .concat(['-Wno-unused-parameter', '-std=c++14'])
        .concat(ADDON_PKG_CONFIG_CFLAGS)
        .concat(ADDON_CFLAGS)
        .concat(cxxFlags)

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
        files: ADDONS_SOURCES
    }
}
