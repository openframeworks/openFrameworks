#!/bin/bash

# List of examples to build - add emscripten compatible examples to this list
# NOTE: make sure there is no trailing slash!!!
folders=(
##3d
#    "examples/3d/pointCloudExample"
#    "examples/3d/3DPrimitivesExample"
#    "examples/3d/ofxAssimpBoneControlExample"
##   "examples/3d/ofxAssimpAdvancedExample" #broken currently
#    "examples/3d/ofNodeExample"
#    "examples/3d/multiTexture3dExample"
#    "examples/3d/modelNoiseExample"
#    "examples/3d/pathsToMeshesExample"
#    "examples/3d/quaternionLatLongExample"
##gl
#    "examples/gl/shadowsExample"
#    "examples/gl/materialPBRExample"
#    "examples/gl/materialPBRAdvancedExample"
#	"examples/gl/transformFeedbackAnimatedExample"
#    "examples/gl/pointLightsExample"
#    "examples/gl/vboMeshDrawInstancedExample"
##math
#    "examples/math/noise1dOctaveExample"
#    "examples/math/particlesExample"
#    "examples/math/periodicSignalsExample"
#    "examples/math/trigonometryExample"
#    "examples/math/trigonometricMotionExample"
#    
#    # Add more paths as needed
"examples/3d/3DPrimitivesExample/"
"examples/3d/advanced3dExample/"
"examples/3d/assimp3DModelLoaderExample/"
"examples/3d/cameraLensOffsetExample/"
"examples/3d/cameraParentingExample/"
"examples/3d/cameraRibbonExample/"
"examples/3d/easyCamExample/"
"examples/3d/meshFromCameraExample/"
"examples/3d/modelNoiseExample/"
"examples/3d/normalsExample/"
"examples/3d/ofBoxExample/"
"examples/3d/ofNodeExample/"
"examples/3d/ofxAssimpAdvancedExample/"
"examples/3d/ofxAssimpBoneControlExample/"
"examples/3d/ofxAssimpExample/"
"examples/3d/orientationExample/"
"examples/3d/pathsToMeshesExample/"
"examples/3d/pointCloudExample/"
"examples/3d/pointPickerExample/"
"examples/3d/quaternionArcballExample/"
"examples/3d/quaternionLatLongExample/"
"examples/communication/oscChatSystemExample/"
"examples/communication/oscOptionalExample/"
"examples/communication/oscReceiveExample/"
"examples/communication/oscSenderExample/"
"examples/computer_vision/opencvExample/"
"examples/computer_vision/opencvHaarFinderExample/"
"examples/computer_vision/opencvImageClassification/"
"examples/computer_vision/opencvOpticalFlowExample/"
"examples/computer_vision/opencvPeopleDetection/"
"examples/events/advancedEventsExample/"
"examples/events/allEventsExample/"
"examples/events/customEventExample/"
"examples/events/simpleEventsExample/"
"examples/events/simpleTimerEventExample/"
"examples/gl/areaLightExample/"
"examples/gl/billboardExample/"
"examples/gl/billboardRotationExample/"
"examples/gl/computeShaderParticlesExample/"
"examples/gl/computeShaderTextureExample/"
"examples/gl/fboHighResOutputExample/"
"examples/gl/fboTrailsExample/"
"examples/gl/geometryShaderExample/"
"examples/gl/glInfoExample/"
"examples/gl/gpuParticleSystemExample/"
"examples/gl/materialPBR/"
"examples/gl/materialPBRAdvanced/"
"examples/gl/multiLightExample/"
"examples/gl/multiTexture3dExample/"
"examples/gl/multiTextureShaderExample/"
"examples/gl/pixelBufferExample/"
"examples/gl/pointLightsExample/"
"examples/gl/pointsAsTexturesExample/"
"examples/gl/shaderExample/"
"examples/gl/shadowsExample/"
"examples/gl/slowFastRenderingExample/"
"examples/gl/textureBufferInstancedExample/"
"examples/gl/textureExample/"
"examples/gl/textureScreengrabExample/"
"examples/gl/threadedPixelBufferExample/"
"examples/gl/transformFeedbackAnimatedExample/"
"examples/gl/transformFeedbackExample/"
"examples/gl/vboExample/"
"examples/gl/vboMeshDrawInstancedExample/"
"examples/gl/viewportExample/"
"examples/graphics/blendingExample/"
"examples/graphics/colorExample/"
"examples/graphics/colorsExtendedExample/"
"examples/graphics/floatingPointImageExample/"
"examples/graphics/fontShapesExample/"
"examples/graphics/fontsExample/"
"examples/graphics/graphicsExample/"
"examples/graphics/imageSubsectionExample/"
"examples/graphics/lutFilterExample/"
"examples/graphics/polygonExample/"
"examples/graphics/polylineBlobsExample/"
"examples/graphics/rectangleAlignmentAndScalingExample/"
"examples/graphics/simpleColorKeyExample/"
"examples/graphics/vectorGraphicsExample/"
"examples/gui/guiFromParametersExample/"
"examples/gui/oscParametersReceiverExample/"
"examples/gui/oscParametersSenderExample/"
"examples/gui/parameterEdgeCasesExample/"
"examples/gui/parameterGroupExample/"
"examples/input_output/clipboardExample/"
"examples/input_output/dirListExample/"
"examples/input_output/dragDropExample/"
"examples/input_output/fileBufferLoadingCSVExample/"
"examples/input_output/fileOpenSaveDialogExample/"
"examples/input_output/imageCompressionExample/"
"examples/input_output/imageLoaderExample/"
"examples/input_output/imageLoaderWebExample/"
"examples/input_output/imageSaverExample/"
"examples/input_output/imageSequenceExample/"
"examples/input_output/jsonExample/"
"examples/input_output/loadTextFileExample/"
"examples/input_output/pdfExample/"
"examples/input_output/pugiXmlExample/"
"examples/input_output/svgExample/"
"examples/input_output/systemSpeakExample/"
"examples/input_output/xmlExample/"
"examples/input_output/xmlSettingsExample/"
"examples/math/noise1dExample/"
"examples/math/noise1dOctaveExample/"
"examples/math/noiseField2dExample/"
"examples/math/particlesExample/"
"examples/math/periodicSignalsExample/"
"examples/math/randomExample/"
"examples/math/trigonometricMotionExample/"
"examples/math/trigonometryExample/"
"examples/math/vectorMathExample/"
"examples/shader/01_simpleColorQuad/"
"examples/shader/02_simpleVertexDisplacement/"
"examples/shader/03_simpleShaderInteraction/"
"examples/shader/04_simpleTexturing/"
"examples/shader/05_alphaMasking/"
"examples/shader/06_multiTexture/"
"examples/shader/07_fboAlphaMask/"
"examples/shader/08_displacementMap/"
"examples/shader/09_gaussianBlurFilter/"
"examples/sound/audioInputExample/"
"examples/sound/audioOutputExample/"
"examples/sound/soundBufferExample/"
"examples/sound/soundPlayerExample/"
"examples/sound/soundPlayerFFTExample/"
"examples/strings/conversionExample/"
"examples/strings/ofLogExample/"
"examples/strings/regularExpressionExample/"
"examples/strings/sortingExample/"
#"examples/threads/threadChannelExample/"
#"examples/threads/threadExample/"
#"examples/threads/threadedImageLoaderExample/"
"examples/video/asciiVideoExample/"
"examples/video/slitscanRadialClockExample/"
"examples/video/videoGrabberExample/"
"examples/video/videoPlayerExample/"
)

cur_root=$(pwd);
script_path="$(cd "$(dirname "$0")" && pwd)"

cd $cur_root;
mkdir -p out
out_folder="$cur_root/out"

outPaths=""
outThumbs=""

code_file_path="$script_path/example-code-preview.html"
code_preview=$(cat "$code_file_path")

# Iterate through the folder paths
for folder in "${folders[@]}"; do
    # Check if the folder exists
    if [ -d "$folder" ]; then
    
		parent_folder=$(dirname "$folder")
		parent_folder_name=$(basename "$parent_folder")

        # Change to the directory
        cd $folder
        cp ../../../scripts/templates/emscripten/Makefile .
		cp ../../../scripts/templates/emscripten/config.make .
        emmake make -j Release
        
        errorcode=$?
		if [[ $errorcode -ne 0 ]]; then
			echo "Couldn't build emscripten example: $folder"
		else
			folder_name=$(basename "$folder")
			mkdir -p "$out_folder/$parent_folder_name"
			cp -r "bin/em/$folder_name" "$out_folder/$parent_folder_name/"
			
			example_index="$out_folder/$parent_folder_name/$folder_name/index.html"
			
			#get the src/ file list so we can show the code
			file_list=""
			for file in "src"/*; do
				# Check if the file is a regular file
				if [ -f "$file" ]; then
					# Append the filename with single quotes to the string
					file_list+="'$(basename "$file")',"
				fi
			done
			file_list=${file_list%,}
			echo $file_list
			
			#append the code preview to the index.html with src/main.cpp / src/ofApp.cpp etc
			sed -i  -e "s#</body></html>##" "$example_index"
			echo "$code_preview" >> "$example_index"
			echo "</body></html>" >> "$example_index"

			cp -r src/* "$out_folder/$parent_folder_name/$folder_name/"
			
			#Github Link and file src replacement
			sed -i  "s|EXAMPLE_PARENT_FOLDER|$parent_folder_name|g" "$example_index"
			sed -i  "s|EXAMPLE_FOLDER|$folder_name|g" "$example_index"
			sed -i  "s|EXAMPLE_FILES|$file_list|g" "$example_index"


			thumb_png="$folder_name.png"
			thumb_gif="$folder_name.gif"
			thumb_jpg="$folder_name.jpg"
			
			if [ -e "$thumb_png" ]; then
				cp -r $thumb_png "$out_folder/$parent_folder_name/$folder_name/"
				outThumbs+="$thumb_png,"
			elif [ -e "$thumb_gif" ]; then
				cp -r $thumb_gif "$out_folder/$parent_folder_name/$folder_name/"
				outThumbs+="$thumb_gif,"
			elif [ -e "$thumb_jpg" ]; then
				cp -r $thumb_jpg "$out_folder/$parent_folder_name/$folder_name/"
				outThumbs+="$thumb_jpg,"
			else
				#try any image file
				first_file=$(find . -maxdepth 1 \( -name "*.gif" -o -name "*.png" -o -name "*.jpg" \) -print -quit)
				# Check if a file was found
				if [ -n "$first_file" ]; then
					cp -r $first_file "$out_folder/$parent_folder_name/$folder_name/"
					outThumbs+="$first_file,"
				else
					#default file
					outThumbs+="of.png,"
				fi
			fi
			
			outPaths+="$parent_folder_name/$folder_name,"
		fi
		
		cd $cur_root
    else
        echo "Folder does not exist: $folder"
    fi
done

cd $cur_root;

# Remove the trailing comma
outPaths=${outPaths%,}
outThumbs=${outThumbs%,}

htmlFile="$out_folder/index.html"

echo "outThumbs is $outThumbs"
echo "outPaths is $outPaths"
echo "html is $htmlFile"

# Replace the placeholder in the template file
cp -r $script_path/index.html $htmlFile
sed -i  "s|REPLACE_ME|$outPaths|g" $htmlFile
sed -i  "s|REPLACE_FILES|$outThumbs|g" $htmlFile

# Grab the code snippet js / css
cp -r $script_path/prism.* $out_folder

DO_UPLOAD="false"

if [[ "$GH_ACTIONS" = true && "${GH_BRANCH}" == "master" && -z "${GH_HEAD_REF}" ]]; then
	echo "upload 1/2 - make key file"
    # Temporary file to store the private key
	key_file=$(mktemp)
	echo -e "$GA_EXAMPLES_KEY" > "$key_file"
	chmod 600 "$key_file"
    DO_UPLOAD="true";
fi

if [ "$DO_UPLOAD" = "true" ]; then
	echo "upload 2/2 - time for rsync"
	remote_path="/home/$GA_EXAMPLES_USER/examples/"
	rsync -avz -e "ssh -o 'StrictHostKeyChecking no' -i $key_file" "$out_folder/" "$GA_EXAMPLES_USER@$GA_EXAMPLES_SERVER:$remote_path"
    rm -f "$key_file"
fi
