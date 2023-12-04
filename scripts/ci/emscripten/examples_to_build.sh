#!/bin/bash

#!/bin/bash

# List of folder paths to iterate through make sure there is no trailing slash
folders=(
    "examples/3d/3DPrimitivesExample"
    "examples/3d/ofxAssimpBoneControlExample"
    "examples/3d/ofxAssimpAdvancedExample"
    "examples/3d/pointCloudExample"
    # "examples/3d/ofxAssimpAdvancedExample"
    # Add more paths as needed
)

mkdir out
cur_root=$(pwd);

# Iterate through the folder paths
for folder in "${folders[@]}"; do
    # Check if the folder exists
    if [ -d "$folder" ]; then

        # Change to the directory
        cd $folder
        emmake make Release
        
        errorcode=$?
		if [[ $errorcode -ne 0 ]]; then
			echo "Couldn't build emscripten example: $folder"
		else
			folder_name=$(basename "$folder")
			cp -r "bin/em/$folder_name" "$cur_root/out/"
		fi
		
		cd $cur_root
    else
        echo "Folder does not exist: $folder"
    fi
done

