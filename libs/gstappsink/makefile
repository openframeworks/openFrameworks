################################################################################
# Use this makefile to rebuild oFappsink in case of problems with diferent 
# gstreamer versions
################################################################################

-include ./makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
#------------------------------------------------------------ sources.mk

O_SRCS := 
C_SRCS := 
S_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
OBJS := 
C_DEPS := 
ARCHIVES := 

# Every subdirectory with source files must be described here
SUBDIRS := \
. \

#------------------------------------------------------------


#------------------------------------------------------------ subdir.mk

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
./gstappsink.c 

OBJS += \
./gstappsink.o 

C_DEPS += \
./gstappsink.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ./%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -m32 -O3 -Wall -c -fmessage-length=0 `pkg-config --cflags gstreamer-0.10` -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

#------------------------------------------------------------


#------------------------------------------------------------ objects.mk

USER_OBJS :=

LIBS :=

#------------------------------------------------------------

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ./makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: liboFappsink.a

# Tool invocations
liboFappsink.a: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC Archiver'
	ar -r "lib/linux/liboFappsink.a" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ARCHIVES) lib/linux/liboFappsink.a
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ./makefile.targets
