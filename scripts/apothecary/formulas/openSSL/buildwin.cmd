@echo off

set VS_64_BIT_ENV=VC\bin\amd64\vcvars64.bat

if "%1"=="Win32" (
	call "%VS140COMNTOOLS%vsvars32.bat"
	perl Configure VC-WIN32 enable-static-engine no-asm --prefix=%2\ms\Win32 
rem	perl util\mkfiles.pl >MINFO
rem	perl util\mk1mf.pl static_lib nasm VC-WIN32 >ms\nt-32.mak
rem	perl util\mkdef.pl static_lib 32 libeay > ms\libeay32MT.def
rem	perl util\mkdef.pl static_lib 32 ssleay > ms\ssleay32MT.def

	echo +++++++++++++++++++++++++++++++++++++++++
	
	ms\do_ms
	 
	echo +++++++++++++++++++++++++++++++++++++++++
	echo +++++++++        Make      ++++++++++++++
	echo +++++++++++++++++++++++++++++++++++++++++
	
	nmake -f ms\nt.mak
rem	nmake -f ms\nt-32.mak
	 
	echo +++++++++++++++++++++++++++++++++++++++++
	echo +++++++++      Install     ++++++++++++++
	echo +++++++++++++++++++++++++++++++++++++++++
	  
	nmake -f ms\nt.mak install
rem	nmake -f ms\nt-32.mak install

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