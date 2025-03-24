@echo off
setlocal enabledelayedexpansion

rem setupCommandLine.cmd
rem
rem openFrameworks C++ Libraries command-line build script
rem for Microsoft Visual Studio
rem
rem Usage
rem -----
rem setupCommandLine VS_VERSION [ACTION] [CONFIGURATION] [PLATFORM] [TOOL]
rem VS_VERSION:    100|110|120|140
rem ACTION:        build|rebuild|clean
rem CONFIGURATION: release|debug|both
rem PLATFORM:      Win32|x64
rem TOOL:          devenv|vcexpress|wdexpress|msbuild

echo ///////////////////////////////
echo / openFrameworks build script /
echo ///////////////////////////////
echo.

echo Loading variables

rem VS_VERSION {100 | 110 | 120 | 140}
if "%1"=="" (
	echo VS_VERSION is required argument.
)
set VS_VERSION=vs%1
set VS_64_BIT_ENV=VC\bin\x86_amd64\vcvarsx86_amd64.bat

rem PLATFORM [Win32|x64|WinCE|WEC2013]
set PLATFORM=%4
if "%PLATFORM%"=="" (
	set PLATFORM=Win32
)
if not %PLATFORM%==Win32 (
	if not %PLATFORM%==x64 (
		echo Invalid plateform.
		goto :EOF
	)
)

rem Load Visual C++ Environment
if not defined VCINSTALLDIR (
	if %VS_VERSION%==vs100 (
		if %PLATFORM%==x64 (
			call "%VS100COMNTOOLS%..\..\%VS_64_BIT_ENV%"
		) else (
			call "%VS100COMNTOOLS%vsvars32.bat"
		)
	) else (
		if %VS_VERSION%==vs110 (
			if %PLATFORM%==x64 (
				call "%VS110COMNTOOLS%..\..\%VS_64_BIT_ENV%"
			) else (
				call "%VS110COMNTOOLS%vsvars32.bat"
			)
		) else (
			if %VS_VERSION%==vs120 (
				if %PLATFORM%==x64 (
					call "%VS120COMNTOOLS%..\..\%VS_64_BIT_ENV%"
				) else (
					call "%VS120COMNTOOLS%vsvars32.bat
				)
			) else (
				if %VS_VERSION%==vs140 (
					if %PLATFORM%==x64 (
						call "%VS140COMNTOOLS%..\..\%VS_64_BIT_ENV%"
					) else (
						call "%VS140COMNTOOLS%vsvars32.bat
					)
				)
			)
		)
	)
)
if not defined VSINSTALLDIR (
  echo Error: No Visual C++ environment found.
  echo Please run this script from a Visual Studio Command Prompt
  echo or run "%%VSnnCOMNTOOLS%%\vsvars32.bat" first.
  goto :EOF
) else (echo Variables loaded)

rem TOOL [devenv|vcexpress|wdexpress|msbuild]
if "%5"=="" (
	set BUILD_TOOL=msbuild
) else (
	set BUILD_TOOL=%5
)
if %BUILD_TOOL%==msbuild (
	if not %VS_VERSION%==vs140 (
		if not %VS_VERSION%==vs120 (
			if not %VS_VERSION%==vs110 (
				if not %VS_VERSION%==vs100 (
					echo "Cannot use msbuild with Visual Studio 2008 or earlier."
					goto :EOF
				)
			)
		)
	)
)

rem ACTION [build|rebuild|clean]
set ACTION=%2
if not %ACTION%==build (
	if not %ACTION%==rebuild (
		if not "%ACTION%"=="" (
			if not %ACTION%==clean (
				echo Invalid action.
				goto :EOF
			)
		)
	)
)
if "%ACTION%"=="" (set ACTION="build")

rem CONFIGURATION [release|debug|both]
set CONFIGURATION=%3
if not %CONFIGURATION%==release (
	if not %CONFIGURATION%==debug (
		if not "%CONFIGURATION%"=="" (
			if not %CONFIGURATION%==both (
				echo Invalid configuration.
				goto :EOF
			)
		)
	)
)

echo Environment ready

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
			echo ++++++++++++++++++++++++++++++++++++
			echo ++ Building [%%Z]
			echo ++++++++++++++++++++++++++++++++++++
			echo.

			if "%BUILD_TOOL%"=="msbuild" (
				if not "%CONFIGURATION%"=="release" (msbuild %%Z /t:%ACTION% /nologo /noautoresponse /maxcpucount /p:Configuration=Debug)
				if not "%CONFIGURATION%"=="debug" (msbuild %%Z /t:%ACTION% /nologo /noautoresponse /maxcpucount /p:Configuration=Release)
			) else (
				if not "%CONFIGURATION%"=="release" (%BUILD_TOOL% "%CD%\%%X\%%Z" /%ACTION% "Debug|%PLATFORM%" /nologo)
				if not "%CONFIGURATION%"=="debug" (%BUILD_TOOL% "%CD%\%%X\%%Z" /%ACTION% "Release|%PLATFORM%" /nologo)
			)
		)
		cd ../
	)
)
