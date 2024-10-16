file(GLOB DEBUG_HEADERS *.h *.hpp)

add_library(BRK_DEBUG STATIC
	DebugOverlay.cpp
	GameObjectInfoComponent.cpp
	${DEBUG_HEADERS}
)

target_link_libraries(BRK_DEBUG PRIVATE
	BRK_INCLUDES
	Breakout::Core
	ImGui::ImGui)

add_library(Breakout::Debug ALIAS BRK_DEBUG)