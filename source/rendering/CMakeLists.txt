file(GLOB RENDERING_HEADERS *.h *.hpp)

add_library(BRK_RENDERING STATIC
	${RENDERING_HEADERS}
	Material.cpp
	Renderer.cpp
)

if(${BRK_RENDER_BACKEND} STREQUAL "SDL2")
	set(IMPL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/sdl2)
	target_compile_definitions(BRK_RENDERING PUBLIC BRK_SDL2=1)
elseif(${BRK_RENDER_BACKEND} STREQUAL "DX11")
	set(IMPL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dx11)
	target_compile_definitions(BRK_RENDERING PUBLIC BRK_DX11=1)
	target_link_libraries(BRK_RENDERING PRIVATE d3d11 dxgi)
else()
	message(FATAL_ERROR "Unsupported render backend: ${BRK_RENDER_BACKEND}")
endif()

file(GLOB BACKEND_HEADERS ${IMPL_DIR}/*.h ${IMPL_DIR}/*.hpp)

target_sources(BRK_RENDERING PUBLIC
	${IMPL_DIR}/Buffer.cpp
	${IMPL_DIR}/Renderer.cpp
	${IMPL_DIR}/Shaders.cpp
	${IMPL_DIR}/ResourceHandles.cpp
	${BACKEND_HEADERS}
)

target_compile_features(BRK_RENDERING PUBLIC cxx_std_17)
target_link_libraries(BRK_RENDERING PRIVATE
	BRK_INCLUDES
	Breakout::Core
	ImGui::ImGui
	SDL3::SDL3
)

add_library(Breakout::Rendering ALIAS BRK_RENDERING)