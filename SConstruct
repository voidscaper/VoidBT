#!/usr/bin/env python
import os
import sys

# You can find documentation for SCons and SConstruct files at:
# https://scons.org/documentation.html

# This lets SCons know that we're using godot-cpp, from the godot-cpp folder.
env = SConscript("godot-cpp/SConstruct")

# Configures the 'src' directory as a source for header files.
INCLUDE_PATHS = [
    ".",
    "src/",
    "src/base/core/",
    "src/base/blackboard/",
    "src/base/nexus",
    "src/base/factory/",
    "src/base/data/",
    "src/implementation/core/",
    "src/implementation/blackboard",
    "src/implementation/nexus",
    "src/implementation/factory",
    "src/implementation/data"
]
env.Append(CPPPATH=INCLUDE_PATHS)

# Collects all .cpp files in the 'src' folder as compile targets.
SOURCE_PATHS = [
    "src/base/core/*.cpp"
    "src/base/blackboard/*.cpp",
    "src/base/nexus/*.cpp",
    "src/base/factory/*.cpp",
    "src/base/data/*.cpp",
    "src/implementation/core/*.cpp",
    "src/implementation/blackboard/*.cpp",
    "src/implementation/nexus/*.cpp",
    "src/implementation/factory/*.cpp",
    "src/implementation/data/*.cpp"
]
sources = Glob("src/*.cpp")
for source in SOURCE_PATHS:
    sources += Glob(source)

#sources += Glob("src/implementation/nexus/*.cpp")
# The filename for the dynamic library for this GDExtension.
# $SHLIBPREFIX is a platform specific prefix for the dynamic library ('lib' on Unix, '' on Windows).
# $SHLIBSUFFIX is the platform specific suffix for the dynamic library (for example '.dll' on Windows).
# env["suffix"] includes the build's feature tags (e.g. '.windows.template_debug.x86_64')
# (see https://docs.godotengine.org/en/stable/tutorials/export/feature_tags.html).
# The final path should match a path in the '.gdextension' file.
lib_filename = "{}voidbt{}{}".format(env.subst('$SHLIBPREFIX'), env["suffix"], env.subst('$SHLIBSUFFIX'))

# Creates a SCons target for the path with our sources.
#library = env.SharedLibrary(
#    "project/bin/{}".format(lib_filename),
#    source=sources,
#)
library = env.SharedLibrary(
    "C:/Forge/Godot/Rime/rime/addons/VoidBT/{}".format(lib_filename),
    source=sources,
)
# Selects the shared library as the default target.
Default(library)
