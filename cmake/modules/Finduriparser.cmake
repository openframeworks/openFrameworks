# Find the UriParser library
# Defines:

#  URIPARSER_INCLUDE_DIR - uriparser include directory
#  URIPARSER_LIBRARY     - uriparser library file
#  URIPARSER_FOUND       - TRUE if uriparser is found

if (URIPARSER_INCLUDE_DIR)
 #check cache 
  set(URIPARSER_FIND_QUIETLY TRUE)
endif ()

if (NOT URIPARSER_INCLUDE_DIR)
    find_path(URIPARSER_INCLUDE_DIR NAMES Uri.h PATH_SUFFIXES uriparser)
    set(URIPARSER_INCLUDE_DIR ${URIPARSER_INCLUDE_DIR}/uriparser CACHE PATH "uriparser includes")
endif ()

find_library(URIPARSER_LIBRARY NAMES uriparser)

if (URIPARSER_INCLUDE_DIR AND URIPARSER_LIBRARY)
  set(URIPARSER_FOUND TRUE)
  set(UriParser_FOUND TRUE)
endif ()

if (URIPARSER_FOUND)
   if (NOT URIPARSER_FIND_QUIETLY)
      message(STATUS "Found UriParser library: ${URIPARSER_LIBRARY}")
   endif ()
else ()
   if (NOT URIPARSER_FIND_QUIETLY)
      message(FATAL_ERROR "Could NOT find UriParser library")
    else ()
      message(STATUS "Could NOT find UriParser library")
    endif ()
endif ()