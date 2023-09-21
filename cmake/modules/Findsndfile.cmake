#
# Try to find libsndfile
# Once done this will define
#
#  SNDFILE_FOUND - libsndfile was found
#  SNDFILE_INCLUDE_DIR - the libsndfile include directory
#  SNDFILE_LIBRARIES - libsndfile libraries to link to
#
# SPDX-FileCopyrightText: 2008 Sebastian Trueg <trueg@kde.org>
# SPDX-License-Identifier: BSD-3-Clause

if ( SNDFILE_INCLUDE_DIR AND SNDFILE_LIBRARIES )
   # in cache already
   SET(Sndfile_FIND_QUIETLY TRUE)
endif ( SNDFILE_INCLUDE_DIR AND SNDFILE_LIBRARIES )

IF (NOT WIN32)
  pkg_check_modules(_pc_SNDFILE sndfile)
ENDIF (NOT WIN32)


FIND_PATH(SNDFILE_INCLUDE_DIR
  NAMES sndfile.h
  HINTS ${_pc_SNDFILE_INCLUDE_DIRS}
)

FIND_LIBRARY(SNDFILE_LIBRARIES 
  NAMES sndfile
  HINTS ${_pc_SNDFILE_LIBRARY_DIRS}
)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(sndfile DEFAULT_MSG SNDFILE_INCLUDE_DIR SNDFILE_LIBRARIES )

# show the SNDFILE_INCLUDE_DIR and SNDFILE_LIBRARIES variables only in the advanced view
MARK_AS_ADVANCED(SNDFILE_INCLUDE_DIR SNDFILE_LIBRARIES )