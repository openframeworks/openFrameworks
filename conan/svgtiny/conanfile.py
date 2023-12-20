from conan import ConanFile
from conan.tools.scm import Git
from conan.tools.cmake import CMake
from conan.tools.files import copy
import os

class SvgTinyConan(ConanFile):
    name = "svgtiny"
    version = "0.1.7"
    package_type = "python-require"
    exports = "*"

    def export_sources(self):
        copy(self, "CMakeLists.txt", self.recipe_folder, self.export_sources_folder)

    def source(self):
        git = Git(self)
        git.clone(url="git://git.netsurf-browser.org/libsvgtiny.git", target=".")
        git.checkout("release/0.1.7")
        
    def requirements(self):
        self.requires("libdom/0.4.1@user/stable")

    def build(self):
        cmake = CMake(self)
        cmake.configure(build_script_folder=os.path.join(self.source_folder, os.pardir))
        cmake.build()
