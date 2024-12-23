AddBrkModule(CORE ALIAS Core SOURCES
	AllocTracker.cpp
	Assert.cpp
	DebugBreak.cpp
	IO.cpp
	LogManager.cpp
	MemoryPool.cpp
	Resource.cpp
	ResourceFormatter.cpp
	ResourceLoader.cpp
	ResourceRef.cpp
	RNG.cpp
	Scene.cpp
	StringView.cpp
	StringViewFormatter.cpp
	ThreadPool.cpp
	TimeInfo.cpp
	ULID.cpp
	ULIDFormatter.cpp
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

target_link_libraries(BRK_CORE
PUBLIC 
	BRK_INCLUDES
	nlohmann_json::nlohmann_json
	fmt::fmt
	BRK_MATH
PRIVATE
	EnTT::EnTT
	SDL3::SDL3
)