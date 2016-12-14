move %APPVEYOR_PROJECT_NAME% openFrameworks\addons\%APPVEYOR_PROJECT_NAME%
cd openFrameworks

if "%BUILDER%"=="VS" (%MSYS2_PATH%\usr\bin\bash -lc "scripts/vs/download_libs.sh --silent")
if "%BUILDER%"=="VS" (echo "Downloading projectGenerator from ci server")
if "%BUILDER%"=="VS" (%MSYS2_PATH%\usr\bin\bash -lc "wget http://ci.openframeworks.cc/projectGenerator/projectGenerator-vs.zip -nv")
if "%BUILDER%"=="VS" (%MSYS2_PATH%\usr\bin\bash -lc "unzip -qq projectGenerator-vs.zip")
if "%BUILDER%"=="VS" (rm projectGenerator-vs.zip)
if "%BUILDER%"=="VS" if exist addons\%APPVEYOR_PROJECT_NAME%\scripts\ci\vs\install.sh call %MSYS2_PATH%\usr\bin\bash -lc "source addons/%APPVEYOR_PROJECT_NAME%/scripts/ci/vs/install.sh"
if "%BUILDER%"=="VS" if exist addons\%APPVEYOR_PROJECT_NAME%\scripts\ci\vs\install.bat call addons\%APPVEYOR_PROJECT_NAME%\scripts\ci\vs\install.bat
if "%BUILDER%"=="VS" if exist addons\%APPVEYOR_PROJECT_NAME%\scripts\ci\vs\install.ps call addons\%APPVEYOR_PROJECT_NAME%\scripts\ci\vs\install.ps
if "%BUILDER%"=="VS" (    
    for /D %%e in (addons\%APPVEYOR_PROJECT_NAME%\example*) do (
        projectGenerator.exe %%e
    )
)

if "%BUILDER%"=="MSYS2" (%MSYS2_PATH%\usr\bin\bash -lc "scripts/msys2/install_dependencies.sh --noconfirm")
if "%BUILDER%"=="MSYS2" (%MSYS2_PATH%\usr\bin\bash -lc "scripts/msys2/download_libs.sh --silent")
if "%BUILDER%"=="MSYS2" (
    for /D %%e in (addons\%APPVEYOR_PROJECT_NAME%\example*) do (
        copy scripts\templates\msys2\Makefile %%e\Makefile
        copy scripts\templates\msys2\config.make %%e\config.make
    )
)
if "%BUILDER%"=="MSYS2" ( 
    if exist addons\%APPVEYOR_PROJECT_NAME%\scripts\ci\msys2\install.sh ( 
        %MSYS2_PATH%\usr\bin\bash -lc "addons/%APPVEYOR_PROJECT_NAME%/scripts/ci/msys2/install.sh" 
    ) 
)
