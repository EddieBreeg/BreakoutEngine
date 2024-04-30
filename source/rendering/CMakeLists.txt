file(GLOB RENDERING_HEADERS *.h *.hpp)

add_library(BRK_RENDERING STATIC
	${RENDERING_HEADERS}
	Renderer.cpp
)

if(${BRK_RENDER_BACKEND} STREQUAL "SDL2")
	set(IMPL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sdl2)
else()
	message(FATAL_ERROR "Unsupported render backend: ${BRK_RENDER_BACKEND}")
endif()

target_sources(BRK_RENDERING PUBLIC
	${IMPL_DIR}/Renderer.cpp
)

target_compile_features(BRK_RENDERING PUBLIC cxx_std_17)
target_link_libraries(BRK_RENDERING PRIVATE
	BRK_INCLUDES
	Breakout::Core
	ImGui::ImGui
	SDL2::SDL2
)

add_library(Breakout::Rendering ALIAS BRK_RENDERING)