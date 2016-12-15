rem if "%BUILDER%"=="VS" (
rem     msbuild libs/openFrameworksCompiled/project/vs/openframeworksLib.vcxproj  /verbosity:minimal /logger:"C:\Program Files\AppVeyor\BuildAgent\Appveyor.MSBuildLogger.dll" &&
rem     for /D %%e in (addons\%APPVEYOR_PROJECT_NAME%\example*) do (
        msbuild %%e/%%~ne.vcxproj  /verbosity:minimal /logger:"C:\Program Files\AppVeyor\BuildAgent\Appveyor.MSBuildLogger.dll"
rem     )
rem )
  
rem if "%BUILDER%"=="MSYS2" (    
rem     for /D %%e in (addons\%APPVEYOR_PROJECT_NAME%\example*) do (
rem         %MSYS2_PATH%\usr\bin\bash -lc "make -C addons/%APPVEYOR_PROJECT_NAME%/%%~ne Debug"
rem     )
rem )
