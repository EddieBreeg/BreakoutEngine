cmake_minimum_required(VERSION 3.20.0)
project(BreakoutEngine VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(BRK_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(BRK_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})
string(COMPARE EQUAL ${BRK_ROOT} ${CMAKE_SOURCE_DIR} BRK_STANDALONE)

option(BRK_BUILD_TESTING "Build test apps" ${BRK_STANDALONE})
set(BUILD_TESTING ${BRK_BUILD_TESTING})
include(CTest)
message("-- BRK_BUILD_TESTING: ${BRK_BUILD_TESTING}")

option(BRK_BUILD_SHARED "Build shared libs" OFF)
message("-- BRK_BUILD_SHARED: ${BRK_BUILD_SHARED}")
option(BRK_BUILD_EXAMPLE "Build example project" ${BRK_STANDALONE})
message("-- BRK_BUILD_EXAMPLE: ${BRK_BUILD_EXAMPLE}")

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
message("-- BRK_RENDER_BACKEND: ${BRK_RENDER_BACKEND}")

set(CMAKE_FOLDER "3rd_party")
add_subdirectory(3rd_party)

set(CMAKE_FOLDER "Breakout Engine")
add_subdirectory(source)

if(${BRK_BUILD_EXAMPLE})
	set(CMAKE_FOLDER "Breakout Engine/Example")
	add_subdirectory(example)
endif(${BRK_BUILD_EXAMPLE})

if(${BRK_STANDALONE})
	set(CMAKE_FOLDER "Breakout Engine/Tests")
	add_subdirectory(tests)
endif(${BRK_STANDALONE})