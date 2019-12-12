cmake_minimum_required(VERSION 3.5)
project(CMakePythonExtensionsWith2And3)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I/usr/include/python3.5 -shared -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/include/python3.5 -shared -fPIC")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L-L/usr/lib/python3.5")

add_executable(libgreet
        cmake/FindPython.cmake
        cmake/FindPythonInterp.cmake
        cmake/FindPythonLibs.cmake

        py2/greet2.py
        py3/greet3.py
        CMakeLists.txt
        greet.c
        README.md)

set_target_properties(libgreet PROPERTIES SUFFIX ".cpython-35m-x86_64-linux-gnu.so")