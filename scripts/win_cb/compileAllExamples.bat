

for /d %%X in (..\apps\*) do ( 
cd %%X

	for /d %%Y in (*) do ( 
		cd %%Y
		
		for %%Z in (*.cbp) do (
		
			
			"C:\Program Files\CodeBlocks\codeblocks.exe" /na /nd --build %%Z
		)
		cd ../
	)


cd ../
)

cd ../scripts