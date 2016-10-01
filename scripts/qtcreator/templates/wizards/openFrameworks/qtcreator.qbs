import qbs
import qbs.Process
import qbs.File
import qbs.FileInfo
import qbs.TextFile
import "%{JS: %{CorrectInitialOFPath}?'../../..':'%{OFPath}'}/libs/openFrameworksCompiled/project/qtcreator/ofApp.qbs" as ofApp

Project{
    property string of_root: %{JS: %{CorrectInitialOFPath}?'\'../../..\'':'\'%{OFPath}\''}

    ofApp {
        name: { return FileInfo.baseName(path) }

        files: [
            'src/main.cpp',
            'src/ofApp.cpp',
            'src/ofApp.h',
        ]

        of.addons: [
            %{JS: 
                [].concat(%{ofx3DModelLoader}       ? ['\'ofx3DModelLoader\'']       : [])
                .concat(%{ofxAssimpModelLoader}     ? ['\'ofxAssimpModelLoader\'']   : [])
                .concat(%{ofxGui}                   ? ['\'ofxGui\'']                 : [])
                .concat(%{ofxKinect}                ? ['\'ofxKinect\'']              : [])
                .concat(%{ofxNetwork}               ? ['\'ofxNetwork\'']             : [])
                .concat(%{ofxOpenCv}                ? ['\'ofxOpenCv\'']              : [])
                .concat(%{ofxOsc}                   ? ['\'ofxOsc\'']                 : [])
                .concat(%{ofxSvg}                   ? ['\'ofxSvg\'']                 : [])
                .concat(%{ofxTween}                 ? ['\'ofxTween\'']               : [])
                .concat(%{ofxVectorGraphics}        ? ['\'ofxVectorGraphics\'']      : [])
                .concat(%{ofxXmlSettings}           ? ['\'ofxXmlSettings\'']         : []).toString()
            }
        ]

        // additional flags for the project. the of module sets some
        // flags by default to add the core libraries, search paths...
        // this flags can be augmented through the following properties:
        of.pkgConfigs: []       // list of additional system pkgs to include
        of.includePaths: []     // include search paths
        of.cFlags: []           // flags passed to the c compiler
        of.cxxFlags: []         // flags passed to the c++ compiler
        of.linkerFlags: []      // flags passed to the linker
        of.defines: []          // defines are passed as -D to the compiler
                                // and can be checked with #ifdef or #if in the code
        of.frameworks: []       // osx only, additional frameworks to link with the project

        // other flags can be set through the cpp module: http://doc.qt.io/qbs/cpp-module.html
        // eg: this will enable ccache when compiling
        //
        // cpp.compilerWrapper: 'ccache'

        Depends{
            name: "cpp"
        }

        // common rules that parse the include search paths, core libraries...
        Depends{
            name: "of"
        }

        // dependency with the OF library
        Depends{
            name: "openFrameworks"
        }
    }

    references: [FileInfo.joinPaths(of_root, "/libs/openFrameworksCompiled/project/qtcreator/openFrameworks.qbs")]
}
