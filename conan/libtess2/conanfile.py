from conan import ConanFile
from conan.tools.scm import Git
from conan.tools.files import copy
from conan.tools.cmake import CMake
from os.path import join

class LibTess2Conan(ConanFile):
    name = "libtess2"
    version = "1.0.2"
    package_type = "python-require"
    exports = "*"
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def export_sources(self):
        copy(self, "CMakeLists.txt", self.recipe_folder, self.export_sources_folder)

    def source(self):
        git = Git(self)
        git.clone(url="https://github.com/memononen/libtess2.git", target="source")
        git.folder = "source"
        git.checkout("v1.0.2")

    def build(self):
        cmake = CMake(self)
        cmake.configure(build_script_folder=self.export_sources_folder)
        cmake.build()

    def package(self):
        copy(self, "*.h", join(self.source_folder, "source", "Include"), join(self.package_folder, "include"))
        copy(self, "*.lib", join(self.source_folder, "lib"), join(self.package_folder, "lib"))
        copy(self, "*.pdb", join(self.source_folder, "lib"), join(self.package_folder, "lib"))
        copy(self, "*.a", join(self.source_folder, "lib"), join(self.package_folder, "lib"))
