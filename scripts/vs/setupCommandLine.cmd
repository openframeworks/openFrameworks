@echo off

rem setupCommandLine.cmd
rem
rem openFrameworks C++ Libraries command-line build script
rem for MS Visual Studio 2012
rem
rem Usage
rem -----
rem setupCommandLine VS_VERSION [ACTION] [CONFIGURATION] [TOOL]
rem VS_VERSION:    80|90|100|110
rem ACTION:        build|rebuild|clean
rem CONFIGURATION: release|debug|both
rem TOOL:          devenv|vcexpress|wdexpress|msbuild



echo ///////////////////////////////
echo / openFrameworks build script /
echo ///////////////////////////////
echo.

rem ///////////////////////////////
rem / Load Visual C++ Environment /
rem ///////////////////////////////
:loader

echo ++++ Loading variables
if "%1"=="" (set VS_VERSION=110) else (set VS_VERSION=%1)
if not defined VCINSTALLDIR (
	if "%VS_VERSION%"=="80" (call "%VS80COMNTOOLS%vsvars32.bat") else (
		if "%VS_VERSION%"=="90" (call "%VS90COMNTOOLS%vsvars32.bat") else (
			if "%VS_VERSION%"=="100" (call "%VS100COMNTOOLS%vsvars32.bat") else (
				if "%VS_VERSION%"=="110" (call "%VS110COMNTOOLS%vsvars32.bat")
				)))

	if not defined VSINSTALLDIR (
		echo Error: No Visual C++ environment found.
		echo Please run this script from a Visual Studio Command Prompt
		echo or run "%%VSnnCOMNTOOLS%%\vsvars32.bat" first.
	) else (echo ++++ Variables loaded))

rem ////////////////////////////
rem / Init variables - options /
rem ////////////////////////////
:options

rem TOOL [devenv|vcexpress|wdexpress|msbuild]
if "%4"=="" (
	set BUILD_TOOL=devenv
	if "%VS_VERSION%"=="100" (set BUILD_TOOL=msbuild)
	if "%VS_VERSION%"=="110" (set BUILD_TOOL=msbuild)
) else (set BUILD_TOOL=%4)
if "%VS_VERSION%"==100 (goto action)
if "%VS_VERSION%"==110 (goto action)
if "%BUILD_TOOL%"=="msbuild" (
	if not "%VS_VERSION%"=="110" (
		if not "%VS_VERSION%"=="100" (
			echo "Cannot use msbuild with Visual Studio 2008 or earlier."
			exit)))

rem ACTION [build|rebuild|clean]
set ACTION=%2
if not "%ACTION%"=="build" (
	if not "%ACTION%"=="rebuild" (
		if not "%ACTION%"=="" (
			if not "%ACTION%"=="clean" exit)))
if "%ACTION%"=="" (set ACTION="build")

rem CONFIGURATION [release|debug|both]
set CONFIGURATION=%3
if not "%CONFIGURATION%"=="release" (
	if not "%CONFIGURATION%"=="debug" (
		if not "%CONFIGURATION%"=="" (
			if not "%CONFIGURATION%"=="both" exit)))

rem ///////////////
rem / Builder C++ /
rem ///////////////
:builder

for /d %%X in (..\..\examples\*) do (
	cd %%X
	for /d %%Y in (*) do (
		cd %%Y
		for %%Z in (*.sln) do (
			echo.
			echo ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			echo ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			echo ++++ Building [%%Z]
			echo ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			echo ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
			echo.

			if "%BUILD_TOOL%"=="msbuild" (
				if not "%CONFIGURATION%"=="release" (msbuild /nologo /noautoresponse %%Z /p:Configuration=Debug)
				if not "%CONFIGURATION%"=="debug" (msbuild /nologo /noautoresponse %%Z /p:Configuration=Release)
			) else (
				if not "%CONFIGURATION%"=="release" (%BUILD_TOOL% "%CD%\%%X\%%Y\%%Z" /%ACTION% Debug /nologo)
				if not "%CONFIGURATION%"=="debug" (%BUILD_TOOL% "%CD%\%%X\%%Y\%%Z" /%ACTION% Release /nologo))
		)
		cd ../
	))
