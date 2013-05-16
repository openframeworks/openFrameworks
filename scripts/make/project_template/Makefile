# Attempt to load a config.make file. If none is found, project defaults in 
# config.make will be used.
ifneq ($(wildcard config.make),)
	include config.make
endif

# Make sure the the OF_ROOT location is defined.  It may not have been if there
# was no config.make.
ifndef OF_ROOT
    OF_ROOT=../../..
endif

# Call the project makefile!
include $(OF_ROOT)/libs/openFrameworksCompiled/project/makefileCommon/compile.project.mk
