#include <iostream>
#ifdef __cpp_lib_filesystem
	#include <filesystem>
	namespace fs = std::filesystem;
#else
	#ifdef __cpp_lib_experimental_filesystem
		#include <experimental/filesystem>
		namespace fs = std::experimental::filesystem;
	#endif
#endif


using std::cout;
using std::endl;

int main() {
	cout << "cplusplus version : " << __cplusplus << endl;
	#ifdef __cpp_lib_experimental_filesystem
		cout << "__cpp_lib_experimental_filesystem "  << endl;
	#endif		
	#ifdef __cpp_lib_filesystem
		cout << "__cpp_lib_filesystem " << endl;
	#endif

	// cout << fs::current_path() << endl;
	// fs::path here { "folder" };
	// cout << here << endl;
	// here = fs::current_path();
	// cout << here << endl;
	return 0;
}