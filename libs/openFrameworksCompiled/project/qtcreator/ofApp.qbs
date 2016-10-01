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


}