
file(GLOB SYSTEMS_H *.h *.hpp)

add_library(BRK_SYSTEMS
	ECSManager.cpp
	VisualSystem.cpp
	WindowSystem.cpp
	InputSystem.cpp
	${SYSTEMS_H}
)

target_link_libraries(BRK_SYSTEMS PRIVATE
	Breakout::Core
	Breakout::ECS
	Breakout::App
	Breakout::Debug
	Breakout::Editor
	ImGui::ImGui
)

add_library(Breakout::Systems ALIAS BRK_SYSTEMS)