import qbs
import qbs.FileInfo;
import qbs.Process
import qbs.File
import qbs.TextFile
import "modules/of/helpers.js" as Helpers

CppApplication{
    name: "ofApp"
    consoleApplication: false
    destinationDirectory: Helpers.normalize(FileInfo.joinPaths(project.sourceDirectory,"bin"))
    qbsSearchPaths: "."
    readonly property string platform: of.platform
    readonly property stringList ofAppIncludePaths: Helpers.listDirsRecursive(project.sourceDirectory + '/src')

    Depends{
        name: "of"
    }

    cpp.includePaths: of.coreIncludePaths.concat(ofAppIncludePaths)
    cpp.linkerFlags: of.coreLinkerFlags
    cpp.defines: of.coreDefines
    cpp.cxxStandardLibrary: of.coreCxxStandardLibrary
    cpp.cxxLanguageVersion: of.coreCxxLanguageVersion
    cpp.frameworks: of.coreFrameworks
    cpp.cxxFlags: of.coreCxxFlags
    cpp.cFlags: of.coreCFlags
    cpp.warningLevel: of.coreWarningLevel
    cpp.staticLibraries: of.coreStaticLibs
    cpp.architecture: qbs.architecture

    Properties{
        condition: of.platform === "osx"
        cpp.minimumOsxVersion: 10.8
    }

    Properties{
        condition: qbs.buildVariant.contains("debug")
        targetName: Helpers.parseConfig(project.sourceDirectory + "/config.make","APPNAME",name,"all") + "_debug"
    }

    Properties{
        condition: qbs.buildVariant.contains("release")
        targetName: Helpers.parseConfig(project.sourceDirectory + "/config.make","APPNAME",name,"all")
    }

    // Copy windows dlls from export to bin folder
    Rule {
        condition: qbs.targetOS.contains("windows")
        multiplex : true
        Artifact {
            filePath: "bin/fmodex.dll"
            fileTags: "processed_file"
        }
        Artifact {
            filePath: "bin/qtmlClient.dll"
            fileTags: "processed_file"
        }
        prepare: {
            var cpLibsCmd = new JavaScriptCommand();
            cpLibsCmd.description = "copying dynamic libraries";
            cpLibsCmd.silent = false;
            cpLibsCmd.highlight = 'filegen';
            cpLibsCmd.sourceCode = function(){
                var exportDir = FileInfo.joinPaths(project.path, project.of_root, "export", product.platform);
                File.copy(FileInfo.joinPaths(exportDir,"fmodex.dll"), project.path+"/bin/fmodex.dll");
            }

            return [cpLibsCmd];
        }
    }

    // Copy osx dylibs into bundle and run install_name_tool on the binary
    Rule {
        condition: qbs.targetOS.contains("osx")
        multiplex : true
        Artifact {
            filePath: FileInfo.joinPaths(parent.destinationDirectory, parent.targetName + ".app", "Contents/MacOS/libfmodex.dylib")
            fileTags: "preprocessed_file"
        }
        prepare: {
            var cpLibsCmd = new JavaScriptCommand();
            cpLibsCmd.description = "copying dynamic libraries";
            cpLibsCmd.silent = false;
            cpLibsCmd.highlight = 'filegen';
            cpLibsCmd.sourceCode = function(){
                var exportDir;
                if(FileInfo.isAbsolutePath(project.of_root)){
                    exportDir = Helpers.normalize(FileInfo.joinPaths(project.of_root, "libs/fmodex/lib", product.platform));
                }else{
                    exportDir = Helpers.normalize(FileInfo.joinPaths(project.path, project.of_root, "libs/fmodex/lib", product.platform));
                }

                File.copy(FileInfo.joinPaths(exportDir,"libfmodex.dylib"), FileInfo.joinPaths(product.destinationDirectory, product.targetName + ".app", "Contents/MacOS/libfmodex.dylib"));
            }
            return [cpLibsCmd];

        }
    }

    // Copy osx icon release
    Rule {
        condition: qbs.targetOS.contains("osx") && qbs.buildVariant.contains("release")
        multiplex: true
        Artifact {
            filePath: FileInfo.joinPaths(parent.destinationDirectory, parent.targetName + ".app", "Contents/Resources/icon.icns")
            fileTags: "preprocessed_file"
        }
        prepare: {
            var cpCmd = new JavaScriptCommand();
            cpCmd.description = "copying icon";
            cpCmd.silent = false;
            cpCmd.highlight = 'filegen';
            cpCmd.sourceCode = function(){
                var src;
                if(FileInfo.isAbsolutePath(project.of_root)){
                    src = FileInfo.joinPaths(project.of_root,'libs/openFrameworksCompiled/project/osx/icon.icns');
                }else{
                    src = FileInfo.joinPaths(project.path, project.of_root,'libs/openFrameworksCompiled/project/osx/icon.icns');
                }

                var dst = FileInfo.joinPaths(product.destinationDirectory, product.targetName + ".app", "Contents/Resources/icon.icns");
                File.copy(src, dst);
            }
            return [cpCmd];
        }
    }

    // Copy osx icon debug
    Rule {
        condition: qbs.targetOS.contains("osx") && qbs.buildVariant.contains("debug")
        multiplex: true
        Artifact {
            filePath: FileInfo.joinPaths(product.destinationDirectory, product.targetName + ".app", "Contents/Resources/icon-debug.icns")
            fileTags: "preprocessed_file"
        }
        prepare: {
            var cpCmd = new JavaScriptCommand();
            cpCmd.description = "copying icon";
            cpCmd.silent = false;
            cpCmd.highlight = 'filegen';
            cpCmd.sourceCode = function(){
                var src;
                if(FileInfo.isAbsolutePath(project.of_root)){
                    src = FileInfo.joinPaths(project.of_root,'libs/openFrameworksCompiled/project/osx/icon-debug.icns');
                }else{
                    src = FileInfo.joinPaths(project.path, project.of_root,'libs/openFrameworksCompiled/project/osx/icon-debug.icns');
                }
                var dst = FileInfo.joinPaths(product.destinationDirectory, product.targetName + ".app", "Contents/Resources/icon-debug.icns");
                File.copy(src, dst);
            }
            return [cpCmd];
        }
    }
}
