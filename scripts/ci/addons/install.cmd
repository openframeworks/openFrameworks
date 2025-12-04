%MSYS2_PATH%\usr\bin\bash -lc "pacman --noconfirm -S --needed unzip rsync"

move %APPVEYOR_PROJECT_NAME% openFrameworks\addons\%APPVEYOR_PROJECT_NAME%
cd openFrameworks

if "%BUILDER%"=="VS" (
    rem set PATH="C:\Program Files (x86)\MSBuild\14.0\Bin;%PATH%"
    set OF_PATH=%APPVEYOR_BUILD_FOLDER%\..\openFrameworks
    set PG_OF_PATH=%APPVEYOR_BUILD_FOLDER%\..\openFrameworks
    %MSYS2_PATH%\usr\bin\bash -lc "scripts/vs/download_libs.sh --silent"
    echo "Downloading projectGenerator from ci server"
    %MSYS2_PATH%\usr\bin\bash -lc "downloader() { if command -v wget 2>/dev/null; then wget $@; else curl -LO --retry 20 -O --progress-bar $@; fi; }"
    %MSYS2_PATH%\usr\bin\bash -lc "downloader http://ci.openframeworks.cc/projectGenerator/projectGenerator-vs.zip -nv"
    %MSYS2_PATH%\usr\bin\bash -lc "unzip -qq projectGenerator-vs.zip"
    rm projectGenerator-vs.zip
    if exist addons\%APPVEYOR_PROJECT_NAME%\scripts\ci\vs\install.sh call %MSYS2_PATH%\usr\bin\bash -lc "source addons/%APPVEYOR_PROJECT_NAME%/scripts/ci/vs/install.sh"
    if exist addons\%APPVEYOR_PROJECT_NAME%\scripts\ci\vs\install.bat call addons\%APPVEYOR_PROJECT_NAME%\scripts\ci\vs\install.bat
    if exist addons\%APPVEYOR_PROJECT_NAME%\scripts\ci\vs\install.ps call addons\%APPVEYOR_PROJECT_NAME%\scripts\ci\vs\install.ps
    for /D %%e in (addons\%APPVEYOR_PROJECT_NAME%\example*) do (
        projectGenerator.exe %%e
    )
)

if "%BUILDER%"=="MSYS2" (
    %MSYS2_PATH%\usr\bin\bash -lc "scripts/msys2/install_dependencies.sh --noconfirm"
    %MSYS2_PATH%\usr\bin\bash -lc "scripts/msys2/download_libs.sh --silent"
    for /D %%e in (addons\%APPVEYOR_PROJECT_NAME%\example*) do (
        copy scripts\templates\msys2\Makefile %%e\Makefile
        copy scripts\templates\msys2\config.make %%e\config.make
    )
    if exist addons\%APPVEYOR_PROJECT_NAME%\scripts\ci\msys2\install.sh ( 
        %MSYS2_PATH%\usr\bin\bash -lc "addons/%APPVEYOR_PROJECT_NAME%/scripts/ci/msys2/install.sh" 
    ) 
)
