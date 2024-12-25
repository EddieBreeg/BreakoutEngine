AddBrkModule(APP ALIAS App LIB_TYPE STATIC
SOURCES 
	App.cpp 
	Entry.cpp 
	TypeRegistration.cpp
	Window.cpp
)

target_link_libraries(BRK_APP PRIVATE
	Breakout::Core
	Breakout::Editor
	Breakout::ECS
	Breakout::Managers
	Breakout::Systems
	ImGui::ImGui
	SDL3::SDL3
)