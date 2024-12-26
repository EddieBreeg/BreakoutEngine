AddBrkModule(DEV_UI ALIAS DevUi SOURCES
	ComponentWidgets.cpp
	DebugOverlay.cpp
	DevUiContext.cpp
	GameObjectInfoComponent.cpp
	MaterialWidgets.cpp
	MeshWidget.cpp
	MiscWidgets.cpp
	TextureWidget.cpp
)

target_link_libraries(BRK_DEV_UI PRIVATE
	BRK_INCLUDES
	Breakout::Core
	Breakout::Managers
	Breakout::Rendering
	ImGui::ImGui
	SDL3::SDL3)