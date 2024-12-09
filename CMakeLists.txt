cmake_minimum_required(VERSION 3.20.0)
project(BreakoutEngine VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(BRK_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(BRK_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

option(BRK_BUILD_TESTING "Build test apps" ON)
option(BRK_BUILD_SHARED "Build shared libs" OFF)

if(${BRK_BUILD_SHARED})
	set(BRK_LIBTYPE SHARED)
else()
	set(BRK_LIBTYPE STATIC)
endif(${BRK_BUILD_SHARED})


set(BRK_RENDER_BACKEND "OpenGL" CACHE STRING "The rendering backend to use")
set(BRK_RENDER_BACKEND_OPTIONS "OpenGL")
if(${WIN32})
	list(APPEND BRK_RENDER_BACKEND_OPTIONS "DX11")
endif(${WIN32})

set_property(CACHE BRK_RENDER_BACKEND PROPERTY STRINGS ${BRK_RENDER_BACKEND_OPTIONS})

set(${BUILD_TESTING} ${BRK_BUILD_TESTING})
include(CTest)

set(CMAKE_FOLDER "3rd_party")
add_subdirectory(3rd_party)

set(CMAKE_FOLDER "Engine")
add_subdirectory(source)

set(CMAKE_FOLDER "Examples")
add_subdirectory(example)

set(CMAKE_FOLDER "Tests")
add_subdirectory(tests)

message("-- Selected ${BRK_RENDER_BACKEND} as a rendering backend")
