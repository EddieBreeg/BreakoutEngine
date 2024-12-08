file(GLOB_RECURSE EDITOR_HEADERS *.h *.hpp)

add_library(BRK_EDITOR ${BRK_LIBTYPE}
	${EDITOR_HEADERS}
	Editor.cpp
	ui/ComponentWidgets.cpp
	ui/Inspector.cpp
	ui/MenuBar.cpp
	ui/MiscWidgets.cpp
	ui/Outliner.cpp
	ui/ResourceExplorer.cpp
	ui/StartupWindow.cpp
	ui/SceneCreation.cpp
	ui/SceneSelection.cpp
	ui/UiData.cpp
)
CreateExports(BRK_EDITOR)

target_link_libraries(BRK_EDITOR PRIVATE
	Breakout::Core
	Breakout::ECS
	Breakout::Managers
	ImGui::ImGui
	TinyFileDialogs
	BRK_INCLUDES
	nlohmann_json::nlohmann_json
)

add_library(Breakout::Editor ALIAS BRK_EDITOR)