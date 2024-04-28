add_subdirectory(math)
add_subdirectory(core)
add_subdirectory(ecs)
add_subdirectory(debug)
add_subdirectory(systems)
add_subdirectory(editor)
add_subdirectory(app)

add_library(BRK_INCLUDES INTERFACE)
target_include_directories(BRK_INCLUDES INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(${PROJECT_NAME} INTERFACE)

target_link_libraries(${PROJECT_NAME} INTERFACE
	Breakout::Core
	Breakout::ECS
	Breakout::Systems
	Breakout::Math
	Breakout::Editor
	Breakout::App
)

target_precompile_headers(${PROJECT_NAME} INTERFACE PCH.hpp)
add_library(Breakout::Breakout ALIAS ${PROJECT_NAME})