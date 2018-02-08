# You may override default parameters for this script by specifying 
#       -paramName paramValue
# When calling this script. For example: 
#       download_libs.ps1 -platform vs2015
# Which will load the visual studio 2015 libraries
param(
    [String]$ver="master",
    [String]$platform="vs2017"
    )
$currentPath = $pwd
$scriptPath = split-path -parent $MyInvocation.MyCommand.Definition
$libsDir = $scriptPath + "\..\..\libs"

function DownloadPackage{
    $pkg = $args[0]
    $url = "http://ci.openframeworks.cc/libs/$pkg"
    If(Test-Path "$pkg") {
        echo "Deleting old package"
        Remove-Item $pkg
    }

    echo "Downloading $url to $scriptPath\$pkg"
    $client.DownloadFile($url, "$scriptPath\$pkg")

    If(-Not (Test-Path "$libsDir\$arch")){
        new-item "$libsDir\$arch" -itemtype directory
    }

    echo "Uncompressing downloaded libs to $libsDir\$arch"
    Add-Type -A System.IO.Compression.FileSystem
    [IO.Compression.ZipFile]::ExtractToDirectory("$scriptPath\$pkg", "$libsDir\$arch")

    Remove-Item $pkg
}

function DownloadLibs{
    cd $scriptPath
    $client = new-object System.Net.WebClient
    $arch = $args[0]
    $pkg1 = "openFrameworksLibs_"+$ver+"_"+$platform+"_"+$arch+"_1.zip"
    $pkg2 = "openFrameworksLibs_"+$ver+"_"+$platform+"_"+$arch+"_2.zip"
    $pkg3 = "openFrameworksLibs_"+$ver+"_"+$platform+"_"+$arch+"_3.zip"
    $pkg4 = "openFrameworksLibs_"+$ver+"_"+$platform+"_"+$arch+"_4.zip"
    DownloadPackage $pkg1
    DownloadPackage $pkg2
    DownloadPackage $pkg3
    DownloadPackage $pkg4
}

echo "Installing libs for platform $platform."

if (-Not ($platform -eq "vs2015")){
   echo "You may want to call this script with parameters: 'download_libs.ps1 -platform vs2015' to install libs for Visual Studio 2015." 
}

$libsExists = Test-Path $libsDir
If(-Not $libsExists) {
    echo "Creating libs directory"
    new-item $libsDir -itemtype directory
} 

$libs = @(
    "32", 
    "64", 
    "boost", 
    "cairo", 
    "curl", 
    "FreeImage", 
    "freetype", 
    "glew", 
    "glfw", 
    "json", 
    "libpng", 
    "openssl", 
    "pixman", 
    "poco", 
    "rtAudio", 
    "tess2", 
    "uriparser", 
    "utf8", 
    "videoInput", 
    "zlib", 
    "opencv", 
    "ippicv", 
    "assimp", 
    "README.md", 
    ".appveyor.yml", 
    ".travis.yml"
    )

$addon_libs = @(
    "ofxOpenCv\libs\opencv", 
    "ofxOpenCv\libs\ippicv", 
    "ofxAssimpModelLoader\libs\assimp",
    "ofxSvg\libs\libxml2",
    "ofxSvg\libs\svgtiny",
    "ofxPoco\libs\poco"
    )



echo "Deleting existing libs"
ForEach ($lib in $libs){
    if(Test-Path "..\..\libs\$lib"){
        Remove-Item "..\..\libs\$lib" -Force -Recurse
    }
}

echo "Deleting existing addons libs"
ForEach ($lib in $addon_libs){
    if(Test-Path "..\..\addons\$lib"){
        Remove-Item "..\..\addons\$lib" -Force -Recurse
    }
}

DownloadLibs 32
DownloadLibs 64

function moveAddonLib {
    $addonLib = $args[0]

    $addon_path = split-path -parent "$addonLib"
    $lib_name   = split-path -leaf "$addonLib"

    echo "Moving addon lib: $lib_name"

    robocopy.exe "..\..\libs\32\$lib_name" "..\..\addons\$addon_path\$lib_name" /MOVE /NFL /R:5 /S
    robocopy.exe "..\..\libs\64\$lib_name\lib\vs\x64" "..\..\addons\$addon_path\$lib_name\lib\vs\x64" /MOVE /NFL /R:5 /S
    Remove-Item  "..\..\libs\64\$lib_name" -Force -Recurse

}

echo "Moving addons libs"
ForEach ($lib in $addon_libs){
   moveAddonLib $lib
}

robocopy.exe ..\..\libs\32\ ..\..\libs\ /MOVE /NFL /R:5 /S
robocopy.exe ..\..\libs\64\ ..\..\libs\ /MOVE /NFL /R:5 /S

if(Test-Path "..\..\libs\32"){
    Remove-Item "..\..\libs\32" -Force -Recurse
}

if(Test-Path "..\..\libs\64"){
    Remove-Item "..\..\libs\64" -Force -Recurse
}

if(Test-Path "..\..\libs\README.md"){
    Remove-Item "..\..\libs\README.md" -Force -Recurse
}

cd $currentPath

echo "Success."