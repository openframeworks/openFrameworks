################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ofxOpenCv/src/ofxCvColorImage.cpp \
../ofxOpenCv/src/ofxCvContourFinder.cpp \
../ofxOpenCv/src/ofxCvFloatImage.cpp \
../ofxOpenCv/src/ofxCvGrayscaleImage.cpp \
../ofxOpenCv/src/ofxCvImage.cpp \
../ofxOpenCv/src/ofxCvShortImage.cpp 

OBJS += \
./ofxOpenCv/src/ofxCvColorImage.o \
./ofxOpenCv/src/ofxCvContourFinder.o \
./ofxOpenCv/src/ofxCvFloatImage.o \
./ofxOpenCv/src/ofxCvGrayscaleImage.o \
./ofxOpenCv/src/ofxCvImage.o \
./ofxOpenCv/src/ofxCvShortImage.o 

CPP_DEPS += \
./ofxOpenCv/src/ofxCvColorImage.d \
./ofxOpenCv/src/ofxCvContourFinder.d \
./ofxOpenCv/src/ofxCvFloatImage.d \
./ofxOpenCv/src/ofxCvGrayscaleImage.d \
./ofxOpenCv/src/ofxCvImage.d \
./ofxOpenCv/src/ofxCvShortImage.d 


# Each subdirectory must supply rules for building sources it contributes
ofxOpenCv/src/%.o: ../ofxOpenCv/src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/arturo/Escritorio/openFrameworks/libs/openFrameworks/app" -I"/home/arturo/Escritorio/openFrameworks/libs/openFrameworks/communication" -I"/home/arturo/Escritorio/openFrameworks/libs/openFrameworks/events" -I"/home/arturo/Escritorio/openFrameworks/libs/openFrameworks/graphics" -I"/home/arturo/Escritorio/openFrameworks/libs/openFrameworks/sound" -I"/home/arturo/Escritorio/openFrameworks/libs/openFrameworks/utils" -I"/home/arturo/Escritorio/openFrameworks/libs/openFrameworks/video" -I"/home/arturo/Escritorio/openFrameworks/libs/openFrameworks" -I"/home/arturo/Escritorio/openFrameworks/addons/ofx3DModelLoader/src/3DS" -I"/home/arturo/Escritorio/openFrameworks/addons/ofxOpenCv/libs/opencv/include" -I"/home/arturo/Escritorio/openFrameworks/addons/ofxOsc/libs/oscpack/include/ip" -I"/home/arturo/Escritorio/openFrameworks/addons/ofxOsc/libs/oscpack/include/osc" -I"/home/arturo/Escritorio/openFrameworks/addons/ofxVectorGraphics/libs" -I"/home/arturo/Escritorio/openFrameworks/addons/ofxXmlSettings/libs" -I"/home/arturo/Escritorio/openFrameworks/addons/ofxThread/src" -I"/home/arturo/Escritorio/openFrameworks/libs/FreeImage/include" -I"/home/arturo/Escritorio/openFrameworks/libs/glee/include" -I"/home/arturo/Escritorio/openFrameworks/libs/poco/include" -I"/home/arturo/Escritorio/openFrameworks/libs/rtAudio/include" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


