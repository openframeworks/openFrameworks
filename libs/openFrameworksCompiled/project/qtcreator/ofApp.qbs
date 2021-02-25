import qbs
import qbs.FileInfo;
import qbs.Process
import qbs.File
import qbs.TextFile
import "modules/of/helpers.js" as Helpers

CppApplication{
    name: "ofApp"
    type: ["application", "exportdylib", "exporticon"]
    destinationDirectory: Helpers.normalize(FileInfo.joinPaths(project.sourceDirectory,"bin"))
    qbsSearchPaths: "."
    consoleApplication: false
    readonly property string appname: name
    readonly property string platform: of.platform

    Probe{
        id: include
        property stringList paths
        property string sourceDirectory: project.sourceDirectory
        configure: {
            paths = Helpers.listDirsRecursive(sourceDirectory + '/src');
            found = true;
        }
    }

    Depends{
        name: "of"
    }

    Depends{
        condition: platform==="osx"
        name: "bundle"
    }

    cpp.includePaths: of.coreIncludePaths.concat(include.paths)
    cpp.linkerFlags: of.coreLinkerFlags
    cpp.defines: of.coreDefines.concat(of.ADDONS_DEFINES)
    cpp.cxxStandardLibrary: of.coreCxxStandardLibrary
    cpp.cxxLanguageVersion: of.coreCxxLanguageVersion
    cpp.frameworks: of.coreFrameworks
    cpp.cxxFlags: of.coreCxxFlags
    cpp.cFlags: of.coreCFlags
    cpp.warningLevel: of.coreWarningLevel
    // TODO: system libs should go as dynamic?
    cpp.staticLibraries: of.staticLibraries.concat(of.coreStaticLibs.concat(of.coreSystemLibs))
    cpp.dynamicLibraries: of.dynamicLibraries

    Properties{
        condition: of.platform === "osx"
        cpp.minimumOsxVersion: "10.9"
    }

    Probe{
        id: targetDebug
        property string name: parent.name+"_debug"
        property string sourceDirectory: project.sourceDirectory
        property string appname: parent.appname
        configure: {
            name = Helpers.parseConfig(sourceDirectory + "/config.make", "APPNAME", appname, "all") + "_debug";
            found = true;
        }
    }

    Probe{
        id: targetRelease
        property string name: parent.name
        property string sourceDirectory: project.sourceDirectory;
        property string appname: parent.name
        configure: {
            name = Helpers.parseConfig(sourceDirectory + "/config.make", "APPNAME", appname, "all") + ""; // NOTE: we must add an empty String so that the result will be converted to String
            found = true;
        }
    }

    Properties{
        condition: qbs.buildVariant.contains("debug")
        targetName: targetDebug.name
    }

    Properties{
        condition: qbs.buildVariant.contains("release")
        targetName: targetRelease.name
    }

    Group{
        name: "addons"
        files: of.ADDONS_SOURCES
    }

    Group {
        condition: product.platform === "osx" || product.platform === "msys2"
        name: "dynamic libraries"
        prefix: {
            var srcDir = project.of_root;
            if(FileInfo.isAbsolutePath(project.of_root) == false){
                srcDir = FileInfo.joinPaths(project.sourceDirectory, srcDir);
            }
            srcDir = FileInfo.joinPaths(srcDir, "libs/*/lib/", product.platform, "/");
            return srcDir;
        }

        files: {
            if( product.platform == "msys2" ){
                return ["*.dll"];
            }
            if( product.platform == "osx" ){
                return ["*.dylib"];
            }
            /*if( product.platform == "linux" ||  product.platform == "linux64" ){
                return ["*.so"];
            }*/
        }
        fileTags: ["dynamic libraries"]
    }

    // Copy dynamic libraries
    Rule {
        condition: product.platform === "osx" || product.platform === "msys2"
        inputs: ["dynamic libraries"]
        Artifact {
            filePath: {
                if( product.platform == "msys2" ){
                    return FileInfo.joinPaths(product.destinationDirectory, input.fileName)
                }
                if( product.platform == "osx" ){
                    if( product.consoleApplication ){
                        return FileInfo.joinPaths(product.destinationDirectory, input.fileName);
                    }else{
                        return FileInfo.joinPaths(product.destinationDirectory, product.targetName + ".app", "Contents/Frameworks", input.fileName);
                    }
                }

            }

            fileTags: ["exportdylib"]
        }

        prepare: {
            var cpLibsCmd = new JavaScriptCommand();
            cpLibsCmd.description = "copying dynamic libraries " +  input.fileName +" to " + output.filePath;
            cpLibsCmd.silent = false;
            cpLibsCmd.highlight = 'filegen';
            cpLibsCmd.sourceCode = function(){
                File.copy(input.filePath, output.filePath);
            }
            return [cpLibsCmd];

        }
    }


    Group {
        name: "icons"
        condition: qbs.targetOS.contains("osx")
        files: {
            var icons = [];
            var srcDir = FileInfo.joinPaths(path, '..');

            if( qbs.buildVariant.contains("release") ){
                icons.push("osx/icon.icns");
            }

            if( qbs.buildVariant.contains("debug") ){
                icons.push("osx/icon-debug.icns");
            }

            for (i in icons){
                icons[i] = FileInfo.joinPaths(srcDir, icons[i]);
            }

            return icons;
        }
        fileTags: ["icons"]
    }

    Group {
        name: "precompiled headers"
        condition: project.precompileOfMain === true
        files: [
            FileInfo.joinPaths(parent.of_root, '/openFrameworks/ofMain.h'),
        ]
        fileTags: ["cpp_pch_src"]
    }


    Rule {
        condition: qbs.targetOS.contains("osx")
        inputs: ["icons"]
        Artifact {
            filePath: [FileInfo.joinPaths(product.destinationDirectory, product.targetName + ".app", "Contents/Resources/", input.fileName)]
            fileTags: ["exporticon"]
        }
        prepare: {
            var cmd = new JavaScriptCommand();
            cmd.description = "copying icon " +  input.fileName +" to build directory " + output.filePath;
            cmd.silent = false;
            cmd.highlight = "codegen";
            cmd.sourceCode = function() {
                File.copy(input.filePath, output.filePath);
            }
            return cmd;
        }
    }

    Properties{
        condition: qbs.buildVariant.contains("debug") && of.platform === "osx"
        bundle.infoPlist: ({"CFBundleIconFile":"icon-debug.icns"})
    }

    Properties{
        condition: qbs.buildVariant.contains("release") && of.platform === "osx"
        bundle.infoPlist: ({"CFBundleIconFile":"icon.icns"})
    }
}
