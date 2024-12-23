
EmbedAsset(${BRK_ROOT}/data/fonts/Montserrat-Regular.ttf Montserrat brk::editor::fonts)

AddBrkModule(SYSTEMS ALIAS Systems SOURCES
	ImportFonts.cpp
	TransformComponent.cpp
	VisualComponents.cpp
	VisualSystem.cpp
	WindowSystem.cpp
	${CMAKE_CURRENT_BINARY_DIR}/Montserrat.hpp
)
target_include_directories(BRK_SYSTEMS PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(BRK_SYSTEMS
PUBLIC
	Breakout::ECS
PRIVATE
	Breakout::Core
	Breakout::DevUi
	Breakout::EmbedLib
	Breakout::Editor
	Breakout::Managers
	Breakout::Rendering
	ImGui::ImGui
	SDL3::SDL3
)