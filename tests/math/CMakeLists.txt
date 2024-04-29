add_executable(MathTests
	VectorTests.cpp
	main.cpp)

target_link_libraries(MathTests PRIVATE
	Breakout::Math
)

add_test(NAME MathTests COMMAND MathTests)