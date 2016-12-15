if "%BUILDER%"=="VS" (
    set PATH="C:\Program Files (x86)\MSBuild\14.0\Bin;%PATH%"
    msbuild libs/openFrameworksCompiled/project/vs/openframeworksLib.vcxproj  /verbosity:minimal /logger:"C:\Program Files\AppVeyor\BuildAgent\Appveyor.MSBuildLogger.dll"
    for /D %%e in (addons\%APPVEYOR_PROJECT_NAME%\example*) do (
        msbuild %%e/%%~ne.vcxproj  /verbosity:minimal /logger:"C:\Program Files\AppVeyor\BuildAgent\Appveyor.MSBuildLogger.dll"
    )
)
  
if "%BUILDER%"=="MSYS2" (    
     for /D %%e in (addons\%APPVEYOR_PROJECT_NAME%\example*) do (
         %MSYS2_PATH%\usr\bin\bash -lc "make -C addons/%APPVEYOR_PROJECT_NAME%/%%~ne Debug"
     )
)
