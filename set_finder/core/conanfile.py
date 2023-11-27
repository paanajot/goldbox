from conan import ConanFile
from conan.tools.scm import Git
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout

import os


class SetFinderCoreRecipe(ConanFile):
    name = "set_finder_core"
    version = "1.0"

    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    options = {"build_test": [True, False],
               "disable_tools": [True, False]}
    default_options = {"build_test": False, "disable_tools": True}

    def requirements(self):
        self.requires("frugally_deep/1.0")

    def layout(self):
        cmake_layout(self, build_folder='build/' + self.settings.get_safe("arch"))

    def build(self):
        cmake = CMake(self)
        arch = self.settings.get_safe("arch")
        build_test = self.options.get_safe("build_test")
        disable_tools = self.options.get_safe("disable_tools")

        arch_map = {"armv8":"arm64-v8a", "armv7":"armeabi-v7a", "x86_64":"x86_64", "x86":"x86"}
        cmake_vars = {
            "BUILD_TESTS": build_test,
            "DISABLE_TOOLS": disable_tools,
            "CMAKE_INSTALL_PREFIX": os.environ['SET_FINDER_CORE'] + '/' + arch_map[arch]
        }
        cmake.configure(cmake_vars)
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()
