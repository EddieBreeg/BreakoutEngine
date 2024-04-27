add_executable(CoreTests CoreTests.cpp)
add_executable(MathTests MathTests.cpp)

target_link_libraries(CoreTests PRIVATE
	Breakout::Core
)
target_link_libraries(MathTests PRIVATE
	Breakout::Math
)

add_subdirectory(ecs)