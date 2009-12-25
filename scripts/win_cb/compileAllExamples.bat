@echo off


set OLDDIR=%CD%

cd ..\..\libs\openFrameworksCompiled\project\win_cb\
"C:\Program Files\CodeBlocks\codeblocks.exe" /na /nd --target="release" --build "openFrameworksLib.cbp"
cd ../../../../scripts/win_cb


for /d %%X in (..\..\apps\*) do ( 
cd %%X
echo compiling all folders in: %%X
	for /d %%Y in (*) do ( 
		cd %%Y

		for %%Z in (*.cbp) do (
		
			"C:\Program Files\CodeBlocks\codeblocks.exe" /na /nd --target= "release" --build %%Z	
			
		)
		cd ../
	)


)

chdir /d %OLDDIR%