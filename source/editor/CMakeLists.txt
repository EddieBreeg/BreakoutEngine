AddBrkModule(EDITOR ALIAS Editor SOURCES
	Editor.cpp
	ui/Inspector.cpp
	ui/MenuBar.cpp
	ui/Outliner.cpp
	ui/ResourceExplorer.cpp
	ui/StartupWindow.cpp
	ui/SceneSelection.cpp
	ui/UiData.cpp
)

target_link_libraries(BRK_EDITOR PRIVATE
	Breakout::Core
	Breakout::ECS
	Breakout::DevUi
	Breakout::Managers
	ImGui::ImGui
	BRK_INCLUDES
	nlohmann_json::nlohmann_json
	SDL3::SDL3
)