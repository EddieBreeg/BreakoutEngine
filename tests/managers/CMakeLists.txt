add_executable(ManagerTests main.cpp
	ResourceManagementTests.cpp
)

add_test(NAME ManagerTests COMMAND ManagerTests)

target_link_libraries(ManagerTests PRIVATE
	Breakout::Core
	Breakout::Systems
	Breakout::Managers)