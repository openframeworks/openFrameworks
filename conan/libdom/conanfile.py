from conan import ConanFile
from conan.tools.scm import Git
from conan.tools.files import copy
from conan.tools.cmake import CMake
import os

class LibDomConan(ConanFile):
    name = "libdom"
    version = "0.4.1"
    package_type = "python-require"
    exports = "*"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def export_sources(self):
        copy(self, "CMakeLists.txt", self.recipe_folder, self.export_sources_folder)

    def source(self):
        git = Git(self)
        git.clone(url="git://git.netsurf-browser.org/libdom.git", target="source")
        git.folder = "source"
        git.checkout("release/0.4.1")
        
    def requirements(self):
        self.requires("libparserutils/0.2.4@user/stable")

    def build(self):
        cmake = CMake(self)
        cmake.configure(build_script_folder=self.export_sources_folder)
        cmake.build()

