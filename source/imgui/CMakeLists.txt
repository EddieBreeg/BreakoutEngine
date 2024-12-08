file(GLOB DEBUG_HEADERS *.h *.hpp)

add_library(BRK_DEV_UI ${BRK_LIBTYPE}
	DebugOverlay.cpp
	DevUiContext.cpp
	GameObjectInfoComponent.cpp
	MiscWidgets.cpp
	${DEBUG_HEADERS}
)

CreateExports(BRK_DEV_UI)

target_link_libraries(BRK_DEV_UI PRIVATE
	BRK_INCLUDES
	Breakout::Core
	ImGui::ImGui
	SDL3::SDL3)

add_library(Breakout::DevUi ALIAS BRK_DEV_UI)