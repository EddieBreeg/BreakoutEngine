add_executable(CoreTests main.cpp
	Bitset.cpp
	Enumerator.cpp
	EnumFlags.cpp
	EventTests.cpp
	FunctionTests.cpp
	HashTests.cpp
	IO.cpp
	RetainPtr.cpp
	SignalTests.cpp
	StringViewTests.cpp
	ULIDTests.cpp
	Utf8.cpp
)
	
add_executable(ThreadingTests thread_main.cpp	
	ThreadPoolTests.cpp
	ResourceLoaderTests.cpp
)

target_link_libraries(CoreTests PRIVATE
	Breakout::Core
	nlohmann_json::nlohmann_json
)

target_link_libraries(ThreadingTests PRIVATE
	Breakout::Core
)

add_test(NAME CoreTests COMMAND CoreTests)
add_test(NAME ThreadingTests COMMAND ThreadingTests)