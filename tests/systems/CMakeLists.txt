add_executable(SystemTests
	ECSManagerTests.cpp
	main.cpp)

add_test(NAME SystemTests COMMAND SystemTests)

target_link_libraries(SystemTests PRIVATE
	Breakout::Core
	Breakout::Managers
	Breakout::Systems)