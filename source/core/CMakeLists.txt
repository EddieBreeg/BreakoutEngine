file(GLOB CORE_HEADERS *.hpp)

add_library(BRK_CORE STATIC
	Assert.cpp
	DebugBreak.cpp
	LogManager.cpp
	TimeInfo.cpp
	${CORE_HEADERS}
)

if(${BRK_RENDER_BACKEND} STREQUAL "DX11")
	target_compile_definitions(BRK_CORE PUBLIC BRK_DX11)
	target_link_libraries(BRK_CORE PRIVATE d3d11)
elseif(${BRK_RENDER_BACKEND} STREQUAL "Vulkan")
	target_compile_definitions(BRK_CORE PUBLIC BRK_VULKAN)
elseif(${BRK_RENDER_BACKEND} STREQUAL "OpenGL")
	target_compile_definitions(BRK_CORE PUBLIC BRK_OPENGL)
elseif(${BRK_RENDER_BACKEND} STREQUAL "SDL2")
	target_compile_definitions(BRK_CORE PUBLIC BRK_SDL2_RENDERER)
endif(${BRK_RENDER_BACKEND} STREQUAL "DX11")

target_compile_features(BRK_CORE PUBLIC cxx_std_17)
add_library(Breakout::Core ALIAS BRK_CORE)

target_link_libraries(BRK_CORE PUBLIC 
	BRK_INCLUDES
	fmt::fmt
	EnTT::EnTT
	SDL2::SDL2
)