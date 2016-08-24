######################################################################################
# Build for main library
######################################################################################

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${LIBUSB_1_INCLUDE_DIRS})

# Audio Firmware
include(FindPythonInterp)
IF(BUILD_REDIST_PACKAGE)
  # If this build is intended for a redistributable package, we can't include audios.bin, so we should include fwfetcher.py
  # and the package should run "python fwfetcher.py $INSTALL_PREFIX/share" as a postinst hook
  install (FILES "fwfetcher.py" DESTINATION "${CMAKE_INSTALL_PREFIX}/share")
ELSE(BUILD_REDIST_PACKAGE)
  # If the install is local only, we can just run fwfetcher.py and install the audios.bin firmware to the system folder
  add_custom_target(firmware ALL
      COMMAND ${PYTHON_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/fwfetcher.py" "../audios.bin"
      DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/fwfetcher.py")
  install (FILES "${CMAKE_CURRENT_BINARY_DIR}/../audios.bin" DESTINATION "${CMAKE_INSTALL_PREFIX}/share/libfreenect")
ENDIF()

LIST(APPEND SRC core.c tilt.c cameras.c flags.c usb_libusb10.c registration.c audio.c loader.c)

add_library (freenect SHARED ${SRC})
set_target_properties ( freenect PROPERTIES
  VERSION ${PROJECT_VER}
  SOVERSION ${PROJECT_APIVER})	
install (TARGETS freenect
  DESTINATION "${PROJECT_LIBRARY_INSTALL_DIR}")

add_library (freenectstatic STATIC ${SRC})
set_target_properties (freenectstatic PROPERTIES OUTPUT_NAME freenect)
IF(UNIX AND NOT APPLE)
  SET_TARGET_PROPERTIES (freenectstatic PROPERTIES COMPILE_FLAGS "-fPIC")
ENDIF()
install (TARGETS freenectstatic
  DESTINATION "${PROJECT_LIBRARY_INSTALL_DIR}")

target_link_libraries (freenect ${LIBUSB_1_LIBRARIES})
target_link_libraries (freenectstatic ${LIBUSB_1_LIBRARIES})

# Install the header files
install (FILES "../include/libfreenect.h" "../include/libfreenect_registration.h" "../include/libfreenect_audio.h"
  DESTINATION ${PROJECT_INCLUDE_INSTALL_DIR})

IF(UNIX)
  # Produce a pkg-config file for linking against the shared lib
  configure_file ("libfreenect.pc.in" "libfreenect.pc" @ONLY)
  install (FILES "${CMAKE_CURRENT_BINARY_DIR}/libfreenect.pc"
    DESTINATION "${PROJECT_LIBRARY_INSTALL_DIR}/pkgconfig")
ENDIF()
