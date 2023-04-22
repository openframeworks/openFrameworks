#include <iostream>
#include <experimental/filesystem>

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
	return 0;
}