# This is a sample cmake file to add to newly added OF addons
# simply copy this file to addon directory and customize lib
# finding and include finding.
#
# An addon include cmake file *MUST* set the following cmake variables:
# 
# - ADDONS_SOURCES: list of addon source files
# - ADDONS_INCLUDE_DIRS: list of directories to add to addon include path
# - ADDONS_LIBRARIES: list of library the addon depend on and should be linked with
# 

get_filename_component(ADDON_ROOT ${CMAKE_CURRENT_LIST_FILE} PATH)
get_filename_component(ADDON_NAME ${ADDON_ROOT} NAME)

message(STATUS "-------------------------------- Configuring OF Addon '${ADDON_NAME}'")

#########################################################################
# Add here your cmake code to fill-in described variables
# 
# this populate ADDON_SRC with the list of source files in src/
# this should work in most cases

file( GLOB_RECURSE ADDON_SRC ${ADDON_ROOT}/src/*.cpp ${ADDON_ROOT}/libs/*.cpp)
#########################################################################


set(ADDONS_SOURCES ${ADDONS_SOURCES} ${ADDON_SRC})
set(ADDONS_INCLUDE_DIRS ${ADDONS_INCLUDE_DIRS} "${ADDON_ROOT}/src/" "${ADDON_ROOT}/libs")
