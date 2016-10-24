echo OFF
set CURRENT_DIR=%cd%
set SCRIPT_DIR=%~dp0
set OF_ROOT=%SCRIPT_DIR%\..\..
cd %OF_ROOT%\libs
if not exist openFrameworksCompiled (
  mkdir openFrameworksCompiled
)
cd openFrameworksCompiled
if not exist project (
    mklink /J project ..\..\build\projects
)
if not exist lib (
    mklink /J lib ..\..\build\lib
)

cd ..
if not exist openFrameworks (
    mklink /J openFrameworks ..\src
)

cd %CURRENT_DIR%
