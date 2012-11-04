



LDFLAGS = -Wl,-rpath=./libs
SYSTEMLIBS += $(shell pkg-config jack glu glew gstreamer-0.10 gstreamer-video-0.10 gstreamer-base-0.10 gstreamer-app-0.10 libudev cairo zlib --libs)
SYSTEMLIBS += -lglut -lGL -lasound -lopenal -lsndfile -lvorbis -lFLAC -logg -lfreeimage
LIB_STATIC += $(OF_ROOT)/libs/poco/lib/$(LIBSPATH)/libPocoNet.a $(OF_ROOT)/libs/poco/lib/$(LIBSPATH)/libPocoXML.a $(OF_ROOT)/libs/poco/lib/$(LIBSPATH)/libPocoUtil.a $(OF_ROOT)/libs/poco/lib/$(LIBSPATH)/libPocoFoundation.a

ifeq ($(findstring Debug,$(MAKECMDGOALS)),Debug)
	TARGET_NAME = Debug
	BIN_NAME = $(APPNAME)_debug
	TARGET = bin/$(BIN_NAME)
endif

ifeq ($(findstring Release,$(MAKECMDGOALS)),Release)
	TARGET_NAME = Release
	BIN_NAME = $(APPNAME)
	TARGET = bin/$(BIN_NAME)
endif

ifeq ($(MAKECMDGOALS),)
	TARGET_NAME = Release
	BIN_NAME = $(APPNAME)
	TARGET = bin/$(BIN_NAME)
endif