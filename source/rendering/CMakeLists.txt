file(GLOB RENDERING_HEADERS *.h *.hpp)

add_library(BRK_RENDERING STATIC
	${RENDERING_HEADERS}
)

if(${BRK_RENDER_BACKEND} STREQUAL "SDL2")
	set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sdl2)
else()
	message(FATAL_ERROR "Unsupported render backend: ${BRK_RENDER_BACKEND}")
endif()

target_sources(BRK_RENDERING PUBLIC
	${SRC_DIR}/Renderer.cpp
)

target_compile_features(BRK_RENDERING PUBLIC cxx_std_17)
target_link_libraries(BRK_RENDERING PRIVATE
BRK_INCLUDES)