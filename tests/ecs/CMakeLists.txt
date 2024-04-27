add_executable(ECSTests
	ManagerTests.cpp
	main.cpp)

target_link_libraries(ECSTests PRIVATE Breakout::ECS)