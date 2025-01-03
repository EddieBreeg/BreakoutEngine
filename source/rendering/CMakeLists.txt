AddBrkModule(RENDERING ALIAS Rendering SOURCES
	Material.cpp
	Mesh.cpp
	Renderer.cpp
	Texture.cpp
)

if(${BRK_RENDER_BACKEND} STREQUAL "DX11")
	set(IMPL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/dx11)
	target_compile_definitions(BRK_RENDERING PUBLIC BRK_DX11=1)
	target_link_libraries(BRK_RENDERING PRIVATE d3d11 dxgi d3dcompiler)
elseif(${BRK_RENDER_BACKEND} STREQUAL "OpenGL")
	message(FATAL_ERROR "OpenGL isn't supported right now")
else()
	message(FATAL_ERROR "Unsupported render backend: ${BRK_RENDER_BACKEND}")
endif()

target_sources(BRK_RENDERING PRIVATE
	${IMPL_DIR}/Buffer.cpp
	${IMPL_DIR}/Renderer.cpp
	${IMPL_DIR}/ResourceHandles.cpp
	${IMPL_DIR}/Shaders.cpp
	${IMPL_DIR}/Texture.cpp
)

target_link_libraries(BRK_RENDERING PRIVATE
	BRK_INCLUDES
	Breakout::Core
	Breakout::Managers
	SDL3::SDL3
	STB::Image
)