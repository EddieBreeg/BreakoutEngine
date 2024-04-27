
file(GLOB SYSTEMS_H *.h *.hpp)

add_library(BRK_SYSTEMS
	VisualSystem.cpp
	WindowSystem.cpp
	InputSystem.cpp
	${SYSTEMS_H}
)

target_link_libraries(BRK_SYSTEMS PUBLIC
	Breakout::Core
	Breakout::ECS
	Breakout::Debug
)

target_link_libraries(BRK_SYSTEMS PRIVATE
	ImGui::ImGui
)

add_library(Breakout::Systems ALIAS BRK_SYSTEMS)