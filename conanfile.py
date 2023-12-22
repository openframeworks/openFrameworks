from conan import ConanFile
from conan.tools.cmake import CMakeToolchain
from conan.tools.cmake import CMake
import os

class OFConan(ConanFile):
    name = "openFrameworks"
    version = "0.11.2"
    license = "MIT"
    url = "https://openframeworks.cc/"
    generators = "CMakeDeps", "CMakeToolchain"
    settings = "os", "compiler", "build_type", "arch"
    
    def include_package(self, name):
        thisfilepath = os.path.dirname(os.path.realpath(__file__))
        recipes_path = os.path.join(thisfilepath, "conan", name)
        self.run("conan export --user=user --channel=stable conanfile.py", cwd=recipes_path)

    def configure(self):
        self.settings.compiler.cppstd = "17"

        self.options["poco/*"].enable_data_sqlite = False
        self.options["poco/*"].enable_data_postgresql = False
        self.options["poco/*"].enable_data_mysql = False

    def requirements(self):
        self.requires("libtiff/4.6.0", force=True) # Unfortunately we have to override these versions and hope they work
        self.requires("libwebp/1.3.2", force=True) # This is a conflict between OpenCV and FreeImage
        self.requires("freeimage/3.18.0")
        self.requires("opencv/3.4.17")

        self.requires("glm/cci.20230113")
        self.requires("utfcpp/3.2.3")
        self.requires("uriparser/0.9.7")
        self.requires("libcurl/8.5.0")
        self.requires("freetype/2.13.2")
        self.requires("pugixml/1.13")
        self.requires("nlohmann_json/3.11.3")
        self.requires("assimp/5.2.2")
        self.requires("libusb/1.0.26")
        self.requires("poco/1.12.5p2")
        self.requires("libxml2/2.12.2")
        self.requires("glew/2.2.0")
        self.requires("glfw/3.3.8")
        self.requires("cairo/1.18.0")

        # self.include_package("svgtiny")
        # self.include_package("libdom")
        # self.include_package("libparserutils")
        self.include_package("libtess2")
        self.requires("libtess2/1.0.2@user/stable")
        #self.requires("svgtiny/0.1.7@user/stable")
