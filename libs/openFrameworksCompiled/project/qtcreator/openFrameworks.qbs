import qbs
import qbs.Process
import qbs.File
import qbs.FileInfo
import qbs.TextFile
import "modules/of/helpers.js" as Helpers

Product{
    of.ofRoot: Helpers.normalize(FileInfo.joinPaths(path, "../../../.."))
    name: "openFrameworks"
    qbsSearchPaths: "."

    readonly property string projectDir: of.ofRoot + "/libs/openFrameworksCompiled/project"
    readonly property string libDir: of.ofRoot + "/libs/openFrameworksCompiled/lib/" + of.platform

    // setting this variable to true will build OF using
    // qbs instead of makefiles which helps catching errors...
    // but will build on each application rebuild instead of in
    // a common directory
    readonly property bool qbsBuild: project.makeOF !== undefined ? !project.makeOF : false
    readonly property bool usePoco: project.usePoco !== undefined ? project.usePoco : true

    Properties{
        condition: qbsBuild
        type: "staticlibrary"
        destinationDirectory: Helpers.normalize(FileInfo.joinPaths(path, "../../lib", project.platform))
    }

    Depends {
        name: "of"
    }

    cpp.includePaths: of.coreIncludePaths
    cpp.defines: of.coreDefines
    cpp.cxxStandardLibrary: of.coreCxxStandardLibrary
    cpp.cxxLanguageVersion: of.coreCxxLanguageVersion
    cpp.frameworks: of.coreFrameworks
    cpp.cxxFlags: of.coreCxxFlags
    cpp.cFlags: of.coreCFlags
    cpp.warningLevel: of.coreWarningLevel
    cpp.architecture: qbs.architecture

    Properties{
        condition: qbs.targetOS.contains("android")
        cpp.sysroot: of.coreSysroot
    }

    property stringList FILES_EXCLUDE: {

        if(qbs.targetOS.indexOf("linux")>-1){
            return [
                "video/ofDirectShowPlayer.*",
                "video/ofDirectShowGrabber.*",
                "video/ofAVFoundationVideoPlayer.*",
                "video/ofAVFoundationVideoGrabber.*",
                "video/ofQuickTimePlayer.*",
                "video/ofQuickTimeGrabber.*",
                "video/ofQtUtils.*",
                "video/ofQTKit.*",
                "app/ofAppEGLWindow.*",
            ];
        }else if(qbs.targetOS.indexOf("windows")>-1){
            return [
                "video/ofGstVideoPlayer.*",
                "video/ofGstVideoGrabber.*",
                "video/ofGstUtils.*",
                "video/ofAVFoundationVideoPlayer.*",
                "video/ofAVFoundationVideoGrabber.*",
                "video/ofQuickTimePlayer.*",
                "video/ofQuickTimeGrabber.*",
                "video/ofQtUtils.*",
                "video/ofQTKit.*",
                "app/ofAppEGLWindow.*",
            ];
        }
    }

    files: {
        var source = Helpers.findSourceRecursive(FileInfo.joinPaths(of.ofRoot, '/libs/openFrameworks'));
        var filteredSource = source.filter(function filterExcludes(path){
            for(exclude in FILES_EXCLUDE){
                var patt = new RegExp(FILES_EXCLUDE[exclude]);
                var match = patt.exec(path);
                if(match!=null){
                    return false;
                }
            }
            return true;
        });
        return filteredSource;
    }

    readonly property string make: {
        if(qbs.targetOS.contains("windows")){
            return FileInfo.joinPaths(of.msys2root,"usr/bin/make");
        }else{
            return "make";
        }
    }

    Transformer {
        condition: qbs.buildVariant.contains('debug') && !product.qbsBuild
        inputs: files
        Artifact {
             filePath: Helpers.normalize(product.libDir + "/libopenFrameworksDebug.a")
             fileTags: "staticlibrary"
        }
        prepare: {
            var parameters = ['-j4', 'Debug'];
            if(!product.usePoco){
                parameters.push('OF_USE_POCO=0');
            }

            var qbsCmd = new Command(product.make, parameters);
            qbsCmd.description = "building openFrameworks library";
            qbsCmd.workingDirectory = product.projectDir;
            qbsCmd.silent = false;
            qbsCmd.highlight = 'compiler';
            return [qbsCmd];
        }
    }

    Transformer {
        condition: qbs.buildVariant.contains('release') && !product.qbsBuild
        inputs: files
        Artifact {
             filePath: Helpers.normalize(product.libDir + "/libopenFrameworks.a")
             fileTags: "staticlibrary"
        }
        prepare: {
            var parameters = ['-j4', 'Release'];
            if(!product.usePoco){
                parameters.push('OF_USE_POCO=0');
            }

            var qbsCmd = new Command(product.make, parameters);
            qbsCmd.description = "building openFrameworks library";
            qbsCmd.workingDirectory = product.projectDir;
            qbsCmd.silent = false;
            qbsCmd.highlight = 'compiler';
            return [qbsCmd];
        }
    }
}
