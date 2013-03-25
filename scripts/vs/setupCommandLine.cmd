@echo off

rem setupCommandLine.cmd
rem
rem openFrameworks C++ Libraries command-line build script
rem for MS Visual Studio 2012
rem
rem Usage
rem -----
rem setupCommandLine VS_VERSION [ACTION]
rem VS_VERSION:		100|110
rem ACTION:			build


rem ///////////////////////////////
rem / Load Visual C++ Environment /
rem ///////////////////////////////
:loader

echo ///////////////////////////////
echo / openFrameworks build script /
echo ///////////////////////////////
echo.
echo ++++ Loading variables
if "%1"=="" (set VS_VERSION=110) else (set VS_VERSION=%1)
if not defined VCINSTALLDIR (
	if %VS_VERSION%==100 (call "%VS100COMNTOOLS%vsvars32.bat") else (
	if %VS_VERSION%==110 (call "%VS110COMNTOOLS%vsvars32.bat")
	))

	if not defined VSINSTALLDIR (
		echo Error: No Visual C++ environment found.
		echo Please run this script from a Visual Studio Command Prompt
		echo or run "%%VSnnCOMNTOOLS%%\vsvars32.bat" first.
	) else (echo ++++ Variables loaded)
)


rem ///////////////
rem / Builder C++ /
rem ///////////////
:builder

set OLDDIR=%CD%
if "%2"=="build" (
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

				msbuild /nologo /noautoresponse %%Z /p:Configuration=Debug
				msbuild /nologo /noautoresponse %%Z /p:Configuration=Release
			)
			cd ../
		)
	)
	cd /d %OLDDIR%
)