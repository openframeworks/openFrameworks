@echo off

set VS_64_BIT_ENV=VC\bin\amd64\vcvars64.bat

if "%1"=="Win32" (
	call "%VS140COMNTOOLS%vsvars32.bat"
	perl Configure VC-WIN32 --prefix=%2\ms\Win32 no-asm
	
	echo +++++++++++++++++++++++++++++++++++++++++
	
	ms\do_ms
	 
	echo +++++++++++++++++++++++++++++++++++++++++
	echo +++++++++        Make      ++++++++++++++
	echo +++++++++++++++++++++++++++++++++++++++++
	
	nmake -f ms\nt.mak
	 
	echo +++++++++++++++++++++++++++++++++++++++++
	echo +++++++++      Install     ++++++++++++++
	echo +++++++++++++++++++++++++++++++++++++++++
	  
	nmake -f ms\nt.mak install

) else (
	if "%1"=="x64" (
		call "%VS140COMNTOOLS%..\..\%VS_64_BIT_ENV%"
		perl Configure VC-WIN64A --prefix=%2\ms\x64 no-asm
		
		echo +++++++++++++++++++++++++++++++++++++++++
		
		ms\do_win64a
		
		echo +++++++++++++++++++++++++++++++++++++++++
		echo +++++++++        Make      ++++++++++++++
		echo +++++++++++++++++++++++++++++++++++++++++

		nmake -f ms\nt.mak
		
		echo +++++++++++++++++++++++++++++++++++++++++
		echo +++++++++      Install     ++++++++++++++
		echo +++++++++++++++++++++++++++++++++++++++++
		
		nmake -f ms\nt.mak install
	)
)