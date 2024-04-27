add_subdirectory(math)
add_subdirectory(core)
add_subdirectory(ecs)
add_subdirectory(debug)
add_subdirectory(systems)

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
)

target_precompile_headers(${PROJECT_NAME} INTERFACE PCH.hpp)