get_filename_component(openFrameworksRoot "${CMAKE_CURRENT_LIST_DIR}/../" ABSOLUTE)

# Having to make the following distinction is a result of
# openFrameworks being not properly deployed into standard lib/include
# paths as of yet, but this is subject to change. This does not
# support 32 bit linux as of now.
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
        set(PLATFORM "linux64")
    else()
        set(PLATFORM "linux")
    endif()
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(PLATFORM "osx")
elseif(${WIN32})
    set(PLATFORM "vs")
endif()

set(openFrameworks_INCLUDES
    ${openFrameworksRoot}/libs/openFrameworks)
set(openFrameworks_LIBRARIES
    ${openFrameworksRoot}/libs/openFrameworksCompiled/lib/${PLATFORM}/libopenFrameworks.a)

message("-- Found openFrameworks in ${openFrameworksRoot}")
