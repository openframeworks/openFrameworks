ofxXmlSettingsdir=$(includedir)/openFrameworks/addons/ofxXmlSettings
ofxXmlSettings_HEADERS=\
		       libs/tinyxml.h \
		       src/ofxXmlSettings.h

lib_LTLIBRARIES=libofxXmlSettings.la
libofxXmlSettings_la_CPPFLAGS=${AM_CPPFLAGS} \
			      -I${srcdir}/libs \
			      -I${srcdir}/src
libofxXmlSettings_la_LDFLAGS=-version-info ${OFX_LIB_VERSION}
libofxXmlSettings_la_SOURCES=\
			     libs/tinyxml.cpp \
			     libs/tinyxmlerror.cpp \
			     libs/tinyxmlparser.cpp \
			     src/ofxXmlSettings.cpp
