add_executable(LoadingTests main.cpp
	ComponentInfoTests.cpp
)

target_link_libraries(LoadingTests PRIVATE
	Breakout::Core
	Breakout::ECS
)
add_test(NAME LoadingTests COMMAND LoadingTests)