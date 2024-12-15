
file(GLOB SYSTEMS_H *.h *.hpp *.inl)
EmbedAsset(${BRK_ROOT}/data/fonts/Montserrat-Regular.ttf Montserrat brk::editor::fonts)

add_library(BRK_SYSTEMS ${BRK_LIBTYPE}
	ImportFonts.cpp
	TransformComponent.cpp
	VisualComponents.cpp
	VisualSystem.cpp
	WindowSystem.cpp
	${CMAKE_CURRENT_BINARY_DIR}/Montserrat.hpp
	${SYSTEMS_H}
)
CreateExports(BRK_SYSTEMS)
target_include_directories(BRK_SYSTEMS PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(BRK_SYSTEMS PUBLIC
	Breakout::ECS
)

target_link_libraries(BRK_SYSTEMS PRIVATE
	Breakout::Core
	Breakout::DevUi
	Breakout::EmbedLib
	Breakout::Editor
	Breakout::Managers
	Breakout::Rendering
	ImGui::ImGui
	SDL3::SDL3
)

add_library(Breakout::Systems ALIAS BRK_SYSTEMS)