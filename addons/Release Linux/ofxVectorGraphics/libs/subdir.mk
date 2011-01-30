################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ofxVectorGraphics/libs/CreEPS.cpp 

OBJS += \
./ofxVectorGraphics/libs/CreEPS.o 

CPP_DEPS += \
./ofxVectorGraphics/libs/CreEPS.d 


# Each subdirectory must supply rules for building sources it contributes
ofxVectorGraphics/libs/%.o: ../ofxVectorGraphics/libs/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/arturo/Escritorio/openFrameworks/libs/openFrameworks/app" -I"/home/arturo/Escritorio/openFrameworks/libs/openFrameworks/communication" -I"/home/arturo/Escritorio/openFrameworks/libs/openFrameworks/events" -I"/home/arturo/Escritorio/openFrameworks/libs/openFrameworks/graphics" -I"/home/arturo/Escritorio/openFrameworks/libs/openFrameworks/sound" -I"/home/arturo/Escritorio/openFrameworks/libs/openFrameworks/utils" -I"/home/arturo/Escritorio/openFrameworks/libs/openFrameworks/video" -I"/home/arturo/Escritorio/openFrameworks/libs/openFrameworks" -I"/home/arturo/Escritorio/openFrameworks/addons/ofx3DModelLoader/src/3DS" -I"/home/arturo/Escritorio/openFrameworks/addons/ofxOpenCv/libs/opencv/include" -I"/home/arturo/Escritorio/openFrameworks/addons/ofxOsc/libs/oscpack/include/ip" -I"/home/arturo/Escritorio/openFrameworks/addons/ofxOsc/libs/oscpack/include/osc" -I"/home/arturo/Escritorio/openFrameworks/addons/ofxVectorGraphics/libs" -I"/home/arturo/Escritorio/openFrameworks/addons/ofxXmlSettings/libs" -I"/home/arturo/Escritorio/openFrameworks/addons/ofxThread/src" -I"/home/arturo/Escritorio/openFrameworks/libs/fmodex/include" -I"/home/arturo/Escritorio/openFrameworks/libs/FreeImage/include" -I"/home/arturo/Escritorio/openFrameworks/libs/freetype/include/freetype2" -I"/home/arturo/Escritorio/openFrameworks/libs/poco/include" -I"/home/arturo/Escritorio/openFrameworks/libs/rtAudio/include" -O3 -march=native -mtune=native -mmmx -msse -msse2 -msse3 -fopenmp -Wall -c -fmessage-length=0 `pkg-config gstreamer-0.10 --cflags` -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


