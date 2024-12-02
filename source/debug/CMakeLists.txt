file(GLOB DEBUG_HEADERS *.h *.hpp)

add_library(BRK_DEBUG ${BRK_LIBTYPE}
	DebugOverlay.cpp
	GameObjectInfoComponent.cpp
	${DEBUG_HEADERS}
)

CreateExports(BRK_DEBUG)

target_link_libraries(BRK_DEBUG PRIVATE
	BRK_INCLUDES
	Breakout::Core
	ImGui::ImGui)

add_library(Breakout::Debug ALIAS BRK_DEBUG)