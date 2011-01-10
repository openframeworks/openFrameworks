ofxVectorGraphicsdir=$(includedir)/openFrameworks/addons/ofxVectorGraphics
ofxVectorGraphics_HEADERS=\
			  src/ofxVectorGraphics.h 
			  libs/CreEPS.hpp \
			  libs/CreEPS_Types.hpp
lib_LTLIBRARIES=libofxVectorGraphics.la
libofxVectorGraphics_la_CPPFLAGS=${AM_CPPFLAGS} -I${srcdir}/libs -I${srcdir}/src
libofxVectorGraphics_la_LDFLAGS=-version-info ${OFX_LIB_VERSION}
libofxVectorGraphics_la_SOURCES=\
				libs/CreEPS.cpp \
				src/ofxVectorGraphics.cpp 

