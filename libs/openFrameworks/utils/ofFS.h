#pragma once

//------------------------------------------------ c++11
// check if the compiler supports c++11. vs hasn't updated the value
// of __cplusplus so we need to check for vs >= 2012 (1700)
#if __cplusplus>=201103 || _MSC_VER >= 1700
	#define OF_HAS_CPP11 1
#endif

// If you are building with c++17 or newer std filesystem will be enabled by default
#if __cplusplus >= 201703L
    #define OF_HAS_CPP17 1
#else
    #define OF_HAS_CPP17 0
#endif


#ifndef OF_USING_STD_FS
	#if OF_HAS_CPP17
		#define OF_USING_STD_FS 1
	#else
		// Set to 1 to force std filesystem instead of boost's
		#define OF_USING_STD_FS 0
	#endif
#endif

// Some projects will specify OF_USING_STD_FS even if the compiler isn't newer than 201703L
// This may be okay but we need to test for the way C++17 is including the filesystem

#if  OF_USING_STD_FS && !defined(OF_USE_EXPERIMENTAL_FS)
    #if defined(__cpp_lib_filesystem)
        #define OF_USE_EXPERIMENTAL_FS 0
    #elif defined(__cpp_lib_experimental_filesystem)
        #define OF_USE_EXPERIMENTAL_FS 1
    #elif !defined(__has_include)
        #define OF_USE_EXPERIMENTAL_FS 1
    #elif __has_include(<filesystem>)
        // If we're compiling on Visual Studio and are not compiling with C++17, we need to use experimental
        #ifdef _MSC_VER
        
            // Check and include header that defines "_HAS_CXX17"
            #if __has_include(<yvals_core.h>)
                #include <yvals_core.h>
                
                // Check for enabled C++17 support
                #if defined(_HAS_CXX17) && _HAS_CXX17
                // We're using C++17, so let's use the normal version
                    #define OF_USE_EXPERIMENTAL_FS 0
                #endif
            #endif

			// FIXME: this was causing error in Windows. INCLUDE_STD_FILESYSTEM_EXPERIMENTAL is not defined anywhere else in OF
            // If the macro isn't defined yet, that means any of the other VS specific checks failed, so we need to use experimental
//            #ifndef INCLUDE_STD_FILESYSTEM_EXPERIMENTAL
//                #define OF_USE_EXPERIMENTAL_FS 1
//            #endif

        // Not on Visual Studio. Let's use the normal version
        #else // #ifdef _MSC_VER
            #define OF_USE_EXPERIMENTAL_FS 0
        #endif
    #else
        #define OF_USE_EXPERIMENTAL_FS 0
    #endif
#endif

#if OF_USING_STD_FS
    #if OF_USE_EXPERIMENTAL_FS
        // C++17 experimental fs support
        #include <experimental/filesystem>
        
        #if OF_HAS_CPP17
            namespace std {
                namespace experimental{
                    namespace filesystem {
                        using path = v1::path;
                    }
                }
            }
        #else
            namespace std {
                namespace experimental{
                    namespace filesystem {
                        using path = v1::__cxx11::path;
                    }
                }
            }
        #endif
        
		namespace of {
			namespace filesystem = std::experimental::filesystem;
		}
    #else
		#include <filesystem>
		#if OF_HAS_CPP17
			// Regular C++17 fs support
			namespace of {
				namespace filesystem = std::filesystem;
			}
		#else
			namespace of {
				namespace filesystem = std::filesystem;
			}
		#endif
    #endif
#else //not OF_USING_STD_FS
    // No experimental or c++17 filesytem support use boost
    #if !_MSC_VER
        #define BOOST_NO_CXX11_SCOPED_ENUMS
        #define BOOST_NO_SCOPED_ENUMS
    #endif

    #include <boost/filesystem.hpp>
	namespace of {
		namespace filesystem = boost::filesystem;
	}

#endif
