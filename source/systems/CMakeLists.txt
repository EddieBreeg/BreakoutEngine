AddBrkModule(SYSTEMS ALIAS Systems SOURCES
	TransformComponent.cpp
	VisualComponents.cpp
	VisualSystem.cpp
)
target_include_directories(BRK_SYSTEMS PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(BRK_SYSTEMS
PUBLIC
	Breakout::ECS
PRIVATE
	Breakout::Core
	Breakout::DevUi
	Breakout::Managers
	Breakout::Rendering
	ImGui::ImGui
	SDL3::SDL3
)