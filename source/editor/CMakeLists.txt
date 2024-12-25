EmbedAsset(${BRK_ROOT}/data/fonts/Montserrat-Regular.ttf Montserrat brk::editor::fonts)

AddBrkModule(EDITOR ALIAS EditorLib SOURCES
	Editor.cpp
	ui/ImportFonts.cpp
	ui/Inspector.cpp
	ui/MenuBar.cpp
	ui/Outliner.cpp
	ui/ResourceExplorer.cpp
	ui/StartupWindow.cpp
	ui/SceneSelection.cpp
	ui/UiData.cpp
	${CMAKE_CURRENT_BINARY_DIR}/Montserrat.hpp
)

target_compile_definitions(BRK_EDITOR PUBLIC
	BRK_EDITOR=1
	BRK_LOGGING_ENABLED=1
)

target_link_libraries(BRK_EDITOR PRIVATE
	Breakout::Core
	Breakout::ECS
	Breakout::DevUi
	Breakout::EmbedLib
	Breakout::Managers
	ImGui::ImGui
	BRK_INCLUDES
	nlohmann_json::nlohmann_json
	SDL3::SDL3
)