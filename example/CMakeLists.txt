add_executable(SandboxGame
	main.cpp
	TestSystem.cpp
)
add_executable(SandboxEditor
	main.cpp
	TestSystem.cpp
)

target_link_libraries(SandboxGame PRIVATE Breakout::Engine)
target_link_libraries(SandboxEditor PRIVATE Breakout::Editor)

if(${WIN32})
	target_sources(SandboxGame PRIVATE Icon.rc)
	target_sources(SandboxEditor PRIVATE Icon.rc)
endif()