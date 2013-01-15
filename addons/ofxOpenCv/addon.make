meta:
	ADDON_NAME = ofxOpenCv
	ADDON_DESCRIPTION = Addon for computer vision using the open source library openCv
	ADDON_AUTHOR = OF Team
	ADDON_TAGS = "computer vision" "opencv" "image processing"

common:
	ADDON_DEPENDENCY =
	
	ADDON_INCLUDES =
	
	ADDON_CFLAGS =
	
	ADDON_LDFLAGS =
	
	ADDON_PKG_CONFIG_LIBRARIES =
	
	ADDON_FRAMEWORKS =
	
	ADDON_SOURCES =
	
	
linux64:
	#ADDON_PKG_CONFIG_LIBRARIES = opencv
	ADDON_LIBS =
	ADDON_LIBS += libs/opencv/lib/linux64/libopencv_legacy.a
	ADDON_LIBS += libs/opencv/lib/linux64/libopencv_haartraining_engine.a
	ADDON_LIBS += libs/opencv/lib/linux64/libopencv_calib3d.a
	ADDON_LIBS += libs/opencv/lib/linux64/libopencv_objdetect.a 
	ADDON_LIBS += libs/opencv/lib/linux64/libopencv_features2d.a
	ADDON_LIBS += libs/opencv/lib/linux64/libopencv_imgproc.a
	ADDON_LIBS += libs/opencv/lib/linux64/libopencv_video.a
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
	ADDON_LIBS += libs/opencv/lib/linux/libopencv_imgproc.a
	ADDON_LIBS += libs/opencv/lib/linux/libopencv_video.a
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
	ADDON_LIBS += libs/opencv/lib/win_cb/libopencv_imgproc231.a
	ADDON_LIBS += libs/opencv/lib/win_cb/libopencv_video231.a
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
	ADDON_LIBS += libs/opencv/lib/linuxarmv6l/libopencv_imgproc.a
	ADDON_LIBS += libs/opencv/lib/linuxarmv6l/libopencv_video.a
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
	ADDON_LIBS += libs/opencv/lib/linuxarmv7l/libopencv_imgproc.a
	ADDON_LIBS += libs/opencv/lib/linuxarmv7l/libopencv_video.a
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
	ADDON_LIBS += libs/opencv/lib/android/armeabi/libopencv_imgproc.a
	ADDON_LIBS += libs/opencv/lib/android/armeabi/libopencv_video.a  
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
	ADDON_LIBS += libs/opencv/lib/android/armeabi-v7a/libopencv_imgproc.a
	ADDON_LIBS += libs/opencv/lib/android/armeabi-v7a/libopencv_video.a  
	ADDON_LIBS += libs/opencv/lib/android/armeabi-v7a/libopencv_highgui.a 
	ADDON_LIBS += libs/opencv/lib/android/armeabi-v7a/libopencv_ml.a 
	ADDON_LIBS += libs/opencv/lib/android/armeabi-v7a/libopencv_core.a 
	ADDON_LIBS += libs/opencv/lib/android/armeabi-v7a/libopencv_flann.a
	ADDON_LIBS += libs/opencv/lib/android/armeabi-v7a/libopencv_contrib.a
