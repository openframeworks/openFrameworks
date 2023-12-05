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

cur_root=$(pwd);
cd cur_root;
mkdir out
out_folder="$OF_ROOT/out"

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
			cp -r "bin/em/$folder_name" "$out_folder/"
		fi
		
		cd $cur_root
    else
        echo "Folder does not exist: $folder"
    fi
done

cd cur_root;
DO_UPLOAD="false"

#if [[ "$GITHUB_ACTIONS" = true && "${GITHUB_REF##*/}" == "master" && -z "${GITHUB_HEAD_REF}" ]]; then
if [[ "$GITHUB_ACTIONS" = true && -z "${GITHUB_HEAD_REF}" ]]; then
    # Temporary file to store the private key
	key_file=$(mktemp)
	echo -e "$GA_EXAMPLES_KEY" > "$key_file"
	chmod 600 "$key_file"
    DO_UPLOAD="true";
fi

if [ "$DO_UPLOAD" = "true" ]; then
    if [ "$TARGET" = "emscripten" ]; then
		remote_path="/home/ofadmin/openFrameworks.cc/examples/"
		rsync -avz -e "ssh -i $key_file" "$out_folder/" "$GA_EXAMPLES_USER@$GA_EXAMPLES_SERVER:$remote_path"
    fi
    rm -f "$key_file"
fi
