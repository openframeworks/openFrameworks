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

    Depends{
        name: "of"
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
    Transformer {
        condition: qbs.targetOS.contains("windows")
        inputs: []
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
                File.copy(FileInfo.joinPaths(exportDir,"qtmlClient.dll"), project.path+"/bin/qtmlClient.dll");
            }

            return [cpLibsCmd];
        }
    }

    // Copy osx dylibs into bundle and run install_name_tool on the binary
    Transformer {
        condition: qbs.targetOS.contains("osx")
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
                var exportDir = Helpers.normalize(FileInfo.joinPaths(project.path, project.of_root, "libs/fmodex/lib", product.platform));
                File.copy(FileInfo.joinPaths(exportDir,"libfmodex.dylib"), FileInfo.joinPaths(product.destinationDirectory, product.targetName + ".app", "Contents/MacOS/libfmodex.dylib"));
            }
            return [cpLibsCmd];

        }
    }
}
