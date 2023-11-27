from conan import ConanFile
from conan.tools.scm import Git
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout

class FrugallyDeepRecipe(ConanFile):
    name = "frugally_deep"
    version = "1.0"

    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def source(self):
        git = Git(self)
        git.clone(url="https://github.com/Dobiasd/frugally-deep", target=".")

    def requirements(self):
        self.requires("eigen/3.4.0", transitive_headers=True)
        self.requires("functionalplus/0.2.18-p0", transitive_headers=True)
        self.requires("nlohmann_json/3.10.5")

    def layout(self):
        cmake_layout(self)

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()
