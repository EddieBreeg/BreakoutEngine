set(APP_SOURCES
	App.cpp 
	Entry.cpp 
	TypeRegistration.cpp
	Window.cpp
)
set(COMMON_APP_DEPS
	Breakout::Core
	Breakout::ECS
	Breakout::Managers
	Breakout::Systems
	ImGui::ImGui
	SDL3::SDL3
)

AddBrkModule(ENGINE_APP ALIAS EngineApp LIB_TYPE STATIC SOURCES ${APP_SOURCES})
AddBrkModule(EDITOR_APP ALIAS EditorApp LIB_TYPE STATIC SOURCES ${APP_SOURCES})

target_link_libraries(BRK_ENGINE_APP PRIVATE ${COMMON_APP_DEPS})
target_link_libraries(BRK_EDITOR_APP PRIVATE ${COMMON_APP_DEPS} Breakout::EditorLib)