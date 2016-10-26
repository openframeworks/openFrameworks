import qbs
import qbs.Process
import qbs.File
import qbs.FileInfo
import qbs.TextFile
import "modules/of/helpers.js" as Helpers

Product{
    of.ofRoot: Helpers.normalize(FileInfo.joinPaths(path, "../../../.."))
    name: "openFrameworks"
    type: "staticlibrary"
    qbsSearchPaths: "."


    readonly property string projectDir: of.ofRoot + "/libs/openFrameworksCompiled/project"
    readonly property string libDir: of.ofRoot + "/libs/openFrameworksCompiled/lib/" + of.platform

    // setting this variable to true will build OF using
    // qbs instead of makefiles which helps catching errors...
    // but will build on each application rebuild instead of in
    // a common directory
    readonly property bool qbsBuild: project.makeOF !== undefined ? !project.makeOF : false

    Properties{
        condition: qbsBuild
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

    Properties{
        condition: of.platform === "osx"
        cpp.minimumOsxVersion: 10.9
    }

    property stringList FILES_EXCLUDE: {
        var excludes = [];
        if(qbs.targetOS.indexOf("linux")>-1){
            excludes = [
                "app/ofAppGlutWindow\\..*",
                "video/ofDirectShowPlayer\\..*",
                "video/ofDirectShowGrabber\\..*",
                "video/ofAVFoundationVideoPlayer\\..*",
                "video/ofAVFoundationVideoGrabber\\..*",
                "video/ofQuickTimePlayer\\..*",
                "video/ofQuickTimeGrabber\\..*",
                "video/ofQtUtils\\..*",
                "video/ofQTKit\\..*",
                "app/ofAppEGLWindow\\..*",
            ];
        }else if(qbs.targetOS.indexOf("windows")>-1){
            excludes = [
                "app/ofAppGlutWindow\\..*",
                "video/ofGstVideoPlayer\\..*",
                "video/ofGstVideoGrabber\\..*",
                "video/ofGstUtils\\..*",
                "video/ofAVFoundationVideoPlayer\\..*",
                "video/ofAVFoundationVideoGrabber\\..*",
                "video/ofQuickTimePlayer\\..*",
                "video/ofQuickTimeGrabber\\..*",
                "video/ofQtUtils\\..*",
                "video/ofQTKit\\..*",
                "app/ofAppEGLWindow\\..*",
            ];
        }else if(qbs.targetOS.indexOf("osx")>-1){
            excludes = [
                "app/ofAppGlutWindow.*",
                "video/ofGstVideoPlayer\\..*",
                "video/ofGstVideoGrabber\\..*",
                "video/ofGstUtils\\..*",
                "video/ofDirectShowPlayer\\..*",
                "video/ofDirectShowGrabber\\..*",
                "video/ofQuickTimePlayer\\..*",
                "video/ofQuickTimeGrabber\\..*",
                "video/ofQtUtils\\..*",
                "video/ofQTKit\\..*",
                "app/ofAppEGLWindow\\..*",
            ];
        }

        return excludes;
    }

    Group {
        condition: !product.qbsBuild
        name: "src"
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
        fileTags: ["filtered_sources"]
    }

    files: {
        if(qbsBuild){
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
        }else{
            return [];
        }
    }


    readonly property string make: {
        return "make";
    }

    Rule {
        condition: qbs.buildVariant.contains('debug') && !product.qbsBuild
        inputs: ["filtered_sources"]
        multiplex : true
        alwaysRun: false
        Artifact {
             filePath: Helpers.normalize(product.libDir + "/libopenFrameworksDebug.a")
             fileTags: "staticlibrary"
        }
        prepare: {
            var parameters = ['-j4', 'Debug'];
            var qbsCmd = new Command(product.make, parameters);
            qbsCmd.description = "building openFrameworks library";
            qbsCmd.workingDirectory = product.projectDir;
            qbsCmd.silent = false;
            qbsCmd.highlight = 'compiler';
            return [qbsCmd];
        }
    }

    Rule {
        condition: qbs.buildVariant.contains('release') && !product.qbsBuild
        inputs: ["filtered_sources"]
        multiplex : true
        alwaysRun: false
        Artifact {
             filePath: Helpers.normalize(product.libDir + "/libopenFrameworks.a")
             fileTags: "staticlibrary"
        }
        prepare: {
            var parameters = ['-j4', 'Release'];
            var qbsCmd = new Command(product.make, parameters);
            qbsCmd.description = "building openFrameworks library";
            qbsCmd.workingDirectory = product.projectDir;
            qbsCmd.silent = false;
            qbsCmd.highlight = 'compiler';
            return [qbsCmd];
        }
    }
}
