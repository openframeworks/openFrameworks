param([String]$ver="master")
$currentPath = $pwd
$scriptPath = split-path -parent $MyInvocation.MyCommand.Definition
$libsDir = "..\..\libs"

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
    $client.DownloadFile($url, '.\'+$pkg)

    If(Test-Path "$libsDir\$arch") {
        Remove-Item "$libsDir\$arch" -Force -Recurse
    }
    new-item "$libsDir\$arch" -itemtype directory

    echo "Uncompressing downloaded libs"
    Add-Type -A System.IO.Compression.FileSystem
    [IO.Compression.ZipFile]::ExtractToDirectory($pkg, "..\..\libs\$arch")

    Remove-Item $pkg
}


$libsExists = Test-Path $libsDir
If(-Not $libsExists) {
    echo "Creating libs directory"
    new-item $libsDir -itemtype directory
}

$libs = @("32", "64", "boost", "cairo", "curl", "FreeImage", "freetype", "glew", "glfw", "json", "libpng", "openssl", "pixman", "poco", "rtAudio", "tess2", "uriparser", "utf8", "videoInput", "zlib", "README.md", ".appveyor.yml", ".travis.yml")
echo "Deleting existing libs"
ForEach ($lib in $libs){
    if(Test-Path "..\..\libs\$lib"){
        Remove-Item "..\..\libs\$lib" -Force -Recurse
    }
}

DownloadLibs 32
DownloadLibs 64

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
