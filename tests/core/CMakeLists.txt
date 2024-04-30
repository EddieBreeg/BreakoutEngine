add_executable(CoreTests main.cpp
	EventTests.cpp
	ULIDTests.cpp
	Utf8.cpp
)

target_link_libraries(CoreTests PRIVATE
	Breakout::Core
	nlohmann_json::nlohmann_json
)

add_test(NAME CoreTests COMMAND CoreTests)