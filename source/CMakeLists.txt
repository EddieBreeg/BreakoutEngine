add_subdirectory(math)
add_subdirectory(core)
add_subdirectory(ecs)
add_subdirectory(systems)

add_library(BREAKOUT_INCLUDES INTERFACE)
target_include_directories(BREAKOUT_INCLUDES INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE
	Breakout::Core
	Breakout::ECS
	Breakout::Systems
	Breakout::Math
)