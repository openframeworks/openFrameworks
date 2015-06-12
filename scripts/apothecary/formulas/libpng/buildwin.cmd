@echo off

set VS_64_BIT_ENV=VC\bin\amd64\vcvars64.bat

if "%1"=="Win32" (
	call "%VS140COMNTOOLS%vsvars32.bat"
	 
	echo +++++++++++++++++++++++++++++++++++++++++
	echo +++++++++      Upgrade     ++++++++++++++
	echo +++++++++++++++++++++++++++++++++++++++++
	 
	devenv libpng.sln /Upgrade
	
	echo +++++++++++++++++++++++++++++++++++++++++
	echo +++++++++       Build      ++++++++++++++
	echo +++++++++++++++++++++++++++++++++++++++++
	
	devenv libpng.sln /Build "LIB Release"

) else (
	if "%1"=="x64" (
		call "%VS140COMNTOOLS%..\..\%VS_64_BIT_ENV%"
		
		echo +++++++++++++++++++++++++++++++++++++++++
		echo +++++++++      Upgrade     ++++++++++++++
		echo +++++++++++++++++++++++++++++++++++++++++
		 
		devenv libpng.sln /Upgrade
		
		echo +++++++++++++++++++++++++++++++++++++++++
		echo +++++++++       Build      ++++++++++++++
		echo +++++++++++++++++++++++++++++++++++++++++
		
		devenv libpng.sln /Build "LIB Release"
	)
)