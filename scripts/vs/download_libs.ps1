param([String]$ver="master")
$currentPath = $pwd
$scriptPath = split-path -parent $MyInvocation.MyCommand.Definition
$libsDir = $scriptPath + "\..\..\libs"

function DownloadLibs{
    cd $scriptPath
    $client = new-object System.Net.WebClient
    $arch = $args[0]
    $pkg = "openFrameworksLibs_"+$ver+"_vs"+$arch+".zip"
    $url = "http://ci.openframeworks.cc/libs/$pkg"
    If(Test-Path "$pkg") {
        echo "Deleting old package"
        Remove-Item $pkg
    }

    echo "Downloading $url to $scriptPath\$pkg"
    $client.DownloadFile($url, "$scriptPath\$pkg")

    If(Test-Path "$libsDir\$arch") {
        Remove-Item "$libsDir\$arch" -Force -Recurse
    }
    new-item "$libsDir\$arch" -itemtype directory

    echo "Uncompressing downloaded libs to $libsDir\$arch"
    Add-Type -A System.IO.Compression.FileSystem
    [IO.Compression.ZipFile]::ExtractToDirectory("$scriptPath\$pkg", "$libsDir\$arch")

    Remove-Item $pkg
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
