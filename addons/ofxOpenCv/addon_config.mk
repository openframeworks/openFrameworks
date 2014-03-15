# All variables and this file are optional, if they are not present the PG and the
# makefiles will try to parse the correct values from the file system.
#
# Variables that specify exclusions can use % as a wildcard to specify that anything in
# that position will match. A partial path can also be specified to, for example, exclude
# a whole folder from the parsed paths from the file system
#
# Variables can be specified using = or +=
# = will clear the contents of that variable both specified from the file or the ones parsed
# from the file system
# += will add the values to the previous ones in the file or the ones parsed from the file 
# system
# 
# The PG can be used to detect errors in this file, just create a new project with this addon 
# and the PG will write to the console the kind of error and in which line it is

meta:
	ADDON_NAME = ofxOpenCv
	ADDON_DESCRIPTION = Addon for computer vision using the open source library openCv
	ADDON_AUTHOR = OF Team
	ADDON_TAGS = "computer vision" "opencv" "image processing"
	ADDON_URL = http://github.com/openframeworks/openFrameworks

common:
	# dependencies with other addons, a list of them separated by spaces 
	# or use += in several lines
	# ADDON_DEPENDENCIES =
	
	# include search paths, this will be usually parsed from the file system
	# but if the addon or addon libraries need special search paths they can be
	# specified here separated by spaces or one per line using +=
	# ADDON_INCLUDES =
	
	# any special flag that should be passed to the compiler when using this
	# addon
	# ADDON_CFLAGS =
	
	# any special flag that should be passed to the linker when using this
	# addon, also used for system libraries with -lname
	# ADDON_LDFLAGS =
	
	# linux only, any library that should be included in the project using
	# pkg-config
	# ADDON_PKG_CONFIG_LIBRARIES =
	
	# osx/iOS only, any framework that should be included in the project
	# ADDON_FRAMEWORKS =
	
	# source files, these will be usually parsed from the file system looking
	# in the src folders in libs and the root of the addon. if your addon needs
	# to include files in different places or a different set of files per platform
	# they can be specified here
	# ADDON_SOURCES =
	
	# some addons need resources to be copied to the bin/data folder of the project
	# specify here any files that need to be copied, you can use wildcards like * and ?
	# ADDON_DATA = 
	
	# when parsing the file system looking for libraries exclude this for all or
	# a specific platform
	# ADDON_LIBS_EXCLUDE =
	
linux64:
	# binary libraries, these will be usually parsed from the file system but some 
	# libraries need to passed to the linker in a specific order 
	ADDON_LIBS =
	ADDON_LIBS += libs/opencv/lib/linux64/libopencv_legacy.a
	ADDON_LIBS += libs/opencv/lib/linux64/libopencv_haartraining_engine.a
	ADDON_LIBS += libs/opencv/lib/linux64/libopencv_calib3d.a
	ADDON_LIBS += libs/opencv/lib/linux64/libopencv_objdetect.a 
	ADDON_LIBS += libs/opencv/lib/linux64/libopencv_features2d.a
	ADDON_LIBS += libs/opencv/lib/linux64/libopencv_video.a
	ADDON_LIBS += libs/opencv/lib/linux64/libopencv_imgproc.a
	ADDON_LIBS += libs/opencv/lib/linux64/libopencv_highgui.a 
	ADDON_LIBS += libs/opencv/lib/linux64/libopencv_ml.a
	ADDON_LIBS += libs/opencv/lib/linux64/libopencv_legacy.a 
	ADDON_LIBS += libs/opencv/lib/linux64/libopencv_core.a
	ADDON_LIBS += libs/opencv/lib/linux64/libopencv_flann.a
	ADDON_LIBS += libs/opencv/lib/linux64/libopencv_contrib.a
	ADDON_LIBS += libs/opencv/lib/linux64/libopencv_gpu.a
	
linux:
	ADDON_LIBS =
	ADDON_LIBS += libs/opencv/lib/linux/libopencv_legacy.a
	ADDON_LIBS += libs/opencv/lib/linux/libopencv_haartraining_engine.a
	ADDON_LIBS += libs/opencv/lib/linux/libopencv_calib3d.a
	ADDON_LIBS += libs/opencv/lib/linux/libopencv_objdetect.a 
	ADDON_LIBS += libs/opencv/lib/linux/libopencv_features2d.a
	ADDON_LIBS += libs/opencv/lib/linux/libopencv_video.a
	ADDON_LIBS += libs/opencv/lib/linux/libopencv_imgproc.a
	ADDON_LIBS += libs/opencv/lib/linux/libopencv_highgui.a 
	ADDON_LIBS += libs/opencv/lib/linux/libopencv_ml.a
	ADDON_LIBS += libs/opencv/lib/linux/libopencv_legacy.a 
	ADDON_LIBS += libs/opencv/lib/linux/libopencv_core.a
	ADDON_LIBS += libs/opencv/lib/linux/libopencv_flann.a
	ADDON_LIBS += libs/opencv/lib/linux/libopencv_contrib.a
	ADDON_LIBS += libs/opencv/lib/linux/libopencv_gpu.a
	
win_cb:
	ADDON_LIBS =
	ADDON_LIBS += libs/opencv/lib/win_cb/libopencv_legacy231.a
	ADDON_LIBS += libs/opencv/lib/win_cb/libopencv_haartraining_engine.a
	ADDON_LIBS += libs/opencv/lib/win_cb/libopencv_calib3d231.a
	ADDON_LIBS += libs/opencv/lib/win_cb/libopencv_features2d231.a
	ADDON_LIBS += libs/opencv/lib/win_cb/libopencv_objdetect231.a
	ADDON_LIBS += libs/opencv/lib/win_cb/libopencv_video231.a
	ADDON_LIBS += libs/opencv/lib/win_cb/libopencv_imgproc231.a
	ADDON_LIBS += libs/opencv/lib/win_cb/libopencv_highgui231.a
	ADDON_LIBS += libs/opencv/lib/win_cb/libopencv_ml231.a
	ADDON_LIBS += libs/opencv/lib/win_cb/libopencv_core231.a
	ADDON_LIBS += libs/opencv/lib/win_cb/libopencv_flann231.a
	ADDON_LIBS += libs/opencv/lib/win_cb/libopencv_contrib231.a
	ADDON_LIBS += libs/opencv/lib/win_cb/libopencv_gpu231.a
	ADDON_LIBS += libs/opencv/lib/win_cb/libopencv_ts231.a
	ADDON_LIBS += libs/opencv/lib/win_cb/libzlib.a

linuxarmv6l:
	ADDON_LIBS =
	ADDON_LIBS += libs/opencv/lib/linuxarmv6l/libopencv_legacy.a
	ADDON_LIBS += libs/opencv/lib/linuxarmv6l/libopencv_calib3d.a
	ADDON_LIBS += libs/opencv/lib/linuxarmv6l/libopencv_objdetect.a 
	ADDON_LIBS += libs/opencv/lib/linuxarmv6l/libopencv_features2d.a
	ADDON_LIBS += libs/opencv/lib/linuxarmv6l/libopencv_video.a
	ADDON_LIBS += libs/opencv/lib/linuxarmv6l/libopencv_imgproc.a
	ADDON_LIBS += libs/opencv/lib/linuxarmv6l/libopencv_highgui.a 
	ADDON_LIBS += libs/opencv/lib/linuxarmv6l/libopencv_ml.a
	ADDON_LIBS += libs/opencv/lib/linuxarmv6l/libopencv_legacy.a 
	ADDON_LIBS += libs/opencv/lib/linuxarmv6l/libopencv_core.a
	ADDON_LIBS += libs/opencv/lib/linuxarmv6l/libopencv_flann.a
	ADDON_LIBS += libs/opencv/lib/linuxarmv6l/libopencv_contrib.a
	ADDON_LIBS += libs/opencv/lib/linuxarmv6l/libopencv_gpu.a
	
	
linuxarmv7l:
	ADDON_LIBS =
	ADDON_LIBS += libs/opencv/lib/linuxarmv7l/libopencv_legacy.a
	ADDON_LIBS += libs/opencv/lib/linuxarmv7l/libopencv_calib3d.a
	ADDON_LIBS += libs/opencv/lib/linuxarmv7l/libopencv_objdetect.a 
	ADDON_LIBS += libs/opencv/lib/linuxarmv7l/libopencv_features2d.a
	ADDON_LIBS += libs/opencv/lib/linuxarmv7l/libopencv_video.a
	ADDON_LIBS += libs/opencv/lib/linuxarmv7l/libopencv_imgproc.a
	ADDON_LIBS += libs/opencv/lib/linuxarmv7l/libopencv_highgui.a 
	ADDON_LIBS += libs/opencv/lib/linuxarmv7l/libopencv_ml.a
	ADDON_LIBS += libs/opencv/lib/linuxarmv7l/libopencv_legacy.a 
	ADDON_LIBS += libs/opencv/lib/linuxarmv7l/libopencv_core.a
	ADDON_LIBS += libs/opencv/lib/linuxarmv7l/libopencv_flann.a
	ADDON_LIBS += libs/opencv/lib/linuxarmv7l/libopencv_contrib.a
	ADDON_LIBS += libs/opencv/lib/linuxarmv7l/libopencv_gpu.a

android/armeabi:	
	ADDON_LIBS =
	ADDON_LIBS += libs/opencv/lib/android/armeabi/libopencv_legacy.a 
	ADDON_LIBS += libs/opencv/lib/android/armeabi/libopencv_calib3d.a
	ADDON_LIBS += libs/opencv/lib/android/armeabi/libopencv_features2d.a 
	ADDON_LIBS += libs/opencv/lib/android/armeabi/libopencv_objdetect.a 
	ADDON_LIBS += libs/opencv/lib/android/armeabi/libopencv_video.a  
	ADDON_LIBS += libs/opencv/lib/android/armeabi/libopencv_imgproc.a
	ADDON_LIBS += libs/opencv/lib/android/armeabi/libopencv_highgui.a 
	ADDON_LIBS += libs/opencv/lib/android/armeabi/libopencv_ml.a 
	ADDON_LIBS += libs/opencv/lib/android/armeabi/libopencv_core.a 
	ADDON_LIBS += libs/opencv/lib/android/armeabi/libopencv_flann.a
	ADDON_LIBS += libs/opencv/lib/android/armeabi/libopencv_contrib.a
	
android/armeabi-v7a:	
	ADDON_LIBS =
	ADDON_LIBS += libs/opencv/lib/android/armeabi-v7a/libopencv_legacy.a 
	ADDON_LIBS += libs/opencv/lib/android/armeabi-v7a/libopencv_calib3d.a
	ADDON_LIBS += libs/opencv/lib/android/armeabi-v7a/libopencv_features2d.a 
	ADDON_LIBS += libs/opencv/lib/android/armeabi-v7a/libopencv_objdetect.a 
	ADDON_LIBS += libs/opencv/lib/android/armeabi-v7a/libopencv_video.a  
	ADDON_LIBS += libs/opencv/lib/android/armeabi-v7a/libopencv_imgproc.a
	ADDON_LIBS += libs/opencv/lib/android/armeabi-v7a/libopencv_highgui.a 
	ADDON_LIBS += libs/opencv/lib/android/armeabi-v7a/libopencv_ml.a 
	ADDON_LIBS += libs/opencv/lib/android/armeabi-v7a/libopencv_core.a 
	ADDON_LIBS += libs/opencv/lib/android/armeabi-v7a/libopencv_flann.a
	ADDON_LIBS += libs/opencv/lib/android/armeabi-v7a/libopencv_contrib.a
