#include <iostream>
#include <experimental/filesystem>

using std::cout;
using std::endl;

int main() {
	cout << "cplusplus version : " << __cplusplus << endl;
	cout << "__cpp_lib_experimental_filesystem " << __cpp_lib_experimental_filesystem << endl;
	cout << "__cpp_lib_filesystem " << __cpp_lib_filesystem << endl;
	return 0;
}