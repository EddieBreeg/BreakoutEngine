add_executable(ECSTests
	ManagerTests.cpp
	main.cpp)

add_test(NAME ECSTests COMMAND ECSTests)

target_link_libraries(ECSTests PRIVATE Breakout::ECS)