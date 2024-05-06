add_executable(CoreTests main.cpp
	EventTests.cpp
	FunctionTests.cpp
	HashTests.cpp
	SignalTests.cpp
	StringViewTests.cpp
	ULIDTests.cpp
	Utf8.cpp
)
	
add_executable(ThreadingTests thread_main.cpp	
	ThreadPoolTests.cpp
)

target_link_libraries(CoreTests PRIVATE
	Breakout::Core
	nlohmann_json::nlohmann_json
)

target_link_libraries(ThreadingTests PRIVATE
	Breakout::Core
)

add_test(NAME CoreTests COMMAND CoreTests)