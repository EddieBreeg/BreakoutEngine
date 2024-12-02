
file(GLOB SYSTEMS_H *.h *.hpp *.inl)

add_library(BRK_SYSTEMS ${BRK_LIBTYPE}
	ResourceLoadingSystem.cpp
	TransformComponent.cpp
	VisualSystem.cpp
	WindowSystem.cpp
	${SYSTEMS_H}
)
CreateExports(BRK_SYSTEMS)

target_link_libraries(BRK_SYSTEMS PUBLIC
	Breakout::ECS
)

target_link_libraries(BRK_SYSTEMS PRIVATE
	Breakout::Core
	Breakout::Debug
	Breakout::Editor
	Breakout::Managers
	Breakout::Rendering
	ImGui::ImGui
	SDL3::SDL3
)

add_library(Breakout::Systems ALIAS BRK_SYSTEMS)