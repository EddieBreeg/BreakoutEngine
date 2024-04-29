add_executable(CoreTests main.cpp
	EventTests.cpp
	Utf8.cpp
)
target_link_libraries(CoreTests PRIVATE
	Breakout::Core
)
add_test(NAME CoreTests COMMAND CoreTests)