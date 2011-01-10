AM_CPPFLAGS= ${AM_CPPFLAGS} -I${srcdir} -I${builddir}
ofx3DModelLoaderdir=$(includedir)/openFrameworks/addons/ofx3DModelLoader
nobase_ofx3DModelLoader_HEADERS=\
				3DS/Vector3DS.h
				3DS/model3DS.h
				3DS/texture3DS.h
				ofx3DBaseLoader.h
				ofx3DModelLoader.h


lib_LTLIBRARIES=libofx3DModelLoader.la
libofx3DModelLoader_la_LDFLAGS= \
				-version-info ${OFX_LIB_VERSION}
libofx3DModelLoader_la_SOURCES= \
				3DS/model3DS.cpp \
				3DS/texture3DS.cpp \
				ofx3dModelLoader.cpp
