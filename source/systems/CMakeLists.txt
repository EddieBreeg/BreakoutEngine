
file(GLOB SYSTEMS_H *.h *.hpp *.inl)

add_library(BRK_SYSTEMS
	ResourceLoadingSystem.cpp
	VisualSystem.cpp
	WindowSystem.cpp
	${SYSTEMS_H}
)

target_link_libraries(BRK_SYSTEMS PUBLIC
	Breakout::ECS
)

target_link_libraries(BRK_SYSTEMS PRIVATE
	Breakout::Core
	Breakout::Rendering
	Breakout::App
	Breakout::Debug
	Breakout::Managers
	Breakout::Editor
	ImGui::ImGui
	SDL2::SDL2
)

add_library(Breakout::Systems ALIAS BRK_SYSTEMS)