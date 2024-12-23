file(GLOB_RECURSE EDITOR_HEADERS *.h *.hpp)

add_library(BRK_EDITOR ${BRK_LIBTYPE}
	${EDITOR_HEADERS}
	Editor.cpp
	ui/Inspector.cpp
	ui/MenuBar.cpp
	ui/Outliner.cpp
	ui/ResourceExplorer.cpp
	ui/StartupWindow.cpp
	ui/SceneSelection.cpp
	ui/UiData.cpp
)
CreateExports(BRK_EDITOR)

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

add_library(Breakout::Editor ALIAS BRK_EDITOR)