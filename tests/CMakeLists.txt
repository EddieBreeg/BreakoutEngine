if(${BUILD_TESTING})
	add_executable(MathTests MathTests.cpp)

	target_link_libraries(MathTests PRIVATE
		Breakout::Math
	)
	
	add_test(NAME MathTests COMMAND MathTests)
	add_subdirectory(systems)
	add_subdirectory(core)
endif(${BUILD_TESTING})
