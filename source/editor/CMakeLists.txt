file(GLOB_RECURSE EDITOR_HEADERS *.h *.hpp)

add_library(BRK_EDITOR STATIC
	${EDITOR_HEADERS}
	Editor.cpp
	ui/MenuBar.cpp
)

target_link_libraries(BRK_EDITOR PRIVATE
	Breakout::Core
	ImGui::ImGui
	BRK_INCLUDES
)

add_library(Breakout::Editor ALIAS BRK_EDITOR)