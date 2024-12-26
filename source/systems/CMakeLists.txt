AddBrkModule(SYSTEMS ALIAS Systems SOURCES
	TransformComponent.cpp
	VisualSystem.cpp
)
target_include_directories(BRK_SYSTEMS PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(BRK_SYSTEMS
PUBLIC
	Breakout::ECS
PRIVATE
	Breakout::Core
	Breakout::Managers
	Breakout::Rendering
	SDL3::SDL3
)