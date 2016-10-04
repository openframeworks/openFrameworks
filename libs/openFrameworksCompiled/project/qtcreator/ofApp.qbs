import qbs
import qbs.FileInfo;
import qbs.Process
import qbs.File
import qbs.TextFile
import "modules/of/helpers.js" as Helpers

Product{
    name: "ofApp"
    type: ["application", "exportdylib"]
    destinationDirectory: Helpers.normalize(FileInfo.joinPaths(project.sourceDirectory,"bin"))
    qbsSearchPaths: "."
    readonly property string platform: of.platform
    readonly property stringList ofAppIncludePaths: Helpers.listDirsRecursive(project.sourceDirectory + '/src')

    Depends{
        name: "of"
    }

    Depends{
        condition: platform==="osx"
        name: "bundle"
    }

    Depends{
        name: "cpp"
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
        consoleApplication: false
    }

    Properties{
        condition: of.platform === "windows"
        consoleApplication: false
    }

    Properties{
        condition: qbs.buildVariant.contains("debug")
        targetName: Helpers.parseConfig(project.sourceDirectory + "/config.make","APPNAME",name,"all") + "_debug"
    }

    Properties{
        condition: qbs.buildVariant.contains("release")
        targetName: Helpers.parseConfig(project.sourceDirectory + "/config.make","APPNAME",name,"all")
    }

    Group{
        name: "addons"
        files: of.ADDONS_SOURCES
    }


    Group {
        condition: platform === "osx" || product.platform === "msys2"
        name: "dynamic libraries"
        files: {
            var libs = [];
            var srcDir = project.of_root;
            if(FileInfo.isAbsolutePath(project.of_root) == false){
                srcDir = FileInfo.joinPaths(project.path, srcDir);
            }

            if( product.platform == "msys2" ){
                srcDir = FileInfo.joinPaths(srcDir, "export", "msys2");
                libs.push("fmodex.dll")
                for (lib in libs){
                    libs[lib] = FileInfo.joinPaths(srcDir,libs[lib]);
                }
            }
            if( product.platform == "osx" ){
                srcDir = FileInfo.joinPaths(srcDir, "libs");
                libs.push(FileInfo.joinPaths(srcDir, "fmodex/lib", of.platform, "libfmodex.dylib"));
            }

            return libs;
        }
        fileTags: ["dynamic libraries"]
    }


    // Copy dynamic libraries
    Rule {
        inputs: ["dynamic libraries"]
        Artifact {
            filePath: {
                if( product.platform == "msys2" ){
                    return FileInfo.joinPaths(product.destinationDirectory, input.fileName)
                }
                if( product.platform == "osx" ){
                    return FileInfo.joinPaths(product.destinationDirectory, product.targetName + ".app", "Contents/MacOS", input.fileName);
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

            var srcDir = FileInfo.joinPaths(project.of_root,'libs/openFrameworksCompiled/project');
            if(FileInfo.isAbsolutePath(project.of_root) == false){
                srcDir = FileInfo.joinPaths(project.path, srcDir);
            }

            if( qbs.buildVariant.contains("release") ){
                icons.push("osx/icon.icns");
            }

            if( qbs.buildVariant.contains("debug") ){
                icons.push("osx/icon-debug.icns");
            }

            for (i in icons){
                icons[i] = FileInfo.joinPaths(srcDir,icons[i]);
            }
            return icons;
        }
        fileTags: ["icons"]
    }



    Rule {
        inputs: ["icons"]
        Artifact {
            filePath: [FileInfo.joinPaths(product.destinationDirectory, product.targetName + ".app", "Contents/Resources/", input.fileName)]
            fileTags: ["application"]
        }
        prepare: {
            var cmd = new JavaScriptCommand();
            cmd.description = "Copying icon " +  input.fileName +" to build directory " + output.filePath;
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
