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
    [IO.Compression.ZipFile]::ExtractToDirectory($pkg, "$libsDir\$arch")

    Remove-Item $pkg
}


$libsExists = Test-Path $libsDir
If(-Not $libsExists) {
    echo "Creating libs directory"
    new-item $libsDir -itemtype directory
}

$libs = @("32", "64", "boost", "cairo", "curl", "FreeImage", "freetype", "glew", "glfw", "json", "libpng", "openssl", "pixman", "poco", "rtAudio", "tess2", "uriparser", "utf8", "videoInput", "zlib", "opencv", "ippicv", "assimp", "README.md", ".appveyor.yml", ".travis.yml")
echo "Deleting existing libs"
ForEach ($lib in $libs){
    if(Test-Path "..\..\libs\$lib"){
        Remove-Item "..\..\libs\$lib" -Force -Recurse
    }
}

DownloadLibs 32
DownloadLibs 64

robocopy.exe ..\..\libs\32\opencv ..\..\addons\ofxOpenCv\libs\ /MOVE /NFL /R:5 /S
robocopy.exe ..\..\libs\64\opencv\lib\vs\x64 ..\..\addons\ofxOpenCv\libs\opencv\lib\vs\ /MOVE /NFL /R:5 /S
Remove-Item "..\..\libs\64\opencv" -Force -Recurse

robocopy.exe ..\..\libs\32\ippicv ..\..\addons\ofxOpenCv\libs\ /MOVE /NFL /R:5 /S
robocopy.exe ..\..\libs\64\ippicv\lib\vs\x64 ..\..\addons\ofxOpenCv\libs\ippicv\lib\vs\ /MOVE /NFL /R:5 /S
Remove-Item "..\..\libs\64\ippicv" -Force -Recurse

robocopy.exe ..\..\libs\32\assimp ..\..\addons\ofxAssimpModelLoader\libs\ /MOVE /NFL /R:5 /S
robocopy.exe ..\..\libs\64\assimp\lib\vs\x64 ..\..\addons\ofxAssimpModelLoader\libs\assimp\lib\vs\ /MOVE /NFL /R:5 /S
Remove-Item "..\..\libs\64\assimp" -Force -Recurse

robocopy.exe ..\..\libs\32\libxml2 ..\..\addons\ofxSvg\libs\ /MOVE /NFL /R:5 /S
robocopy.exe ..\..\libs\64\libxml2\lib\vs\x64 ..\..\addons\ofxSvg\libs\libxml2\lib\vs\ /MOVE /NFL /R:5 /S
Remove-Item "..\..\libs\64\libxml2" -Force -Recurse

robocopy.exe ..\..\libs\32\svgtiny ..\..\addons\ofxSvg\libs\ /MOVE /NFL /R:5 /S
robocopy.exe ..\..\libs\64\svgtiny\lib\vs\x64 ..\..\addons\ofxSvg\libs\svgtiny\lib\vs\ /MOVE /NFL /R:5 /S
Remove-Item "..\..\libs\64\svgtiny" -Force -Recurse

robocopy.exe ..\..\libs\32\poco ..\..\addons\ofxPoco\libs\ /MOVE /NFL /R:5 /S
robocopy.exe ..\..\libs\64\poco\lib\vs\x64 ..\..\addons\ofxPoco\libs\poco\lib\vs\ /MOVE /NFL /R:5 /S
Remove-Item "..\..\libs\64\poco" -Force -Recurse

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
