AddBrkModule(DEV_UI ALIAS DevUi SOURCES
	DebugOverlay.cpp
	DevUiContext.cpp
	GameObjectInfoComponent.cpp
	MiscWidgets.cpp
)

target_link_libraries(BRK_DEV_UI PRIVATE
	BRK_INCLUDES
	Breakout::Core
	ImGui::ImGui
	SDL3::SDL3)