if(${BUILD_TESTING})
	add_executable(CoreTests CoreTests.cpp)
	add_executable(MathTests MathTests.cpp)

	target_link_libraries(CoreTests PRIVATE
		Breakout::Core
	)
	target_link_libraries(MathTests PRIVATE
	Breakout::Math
	)
	
	add_test(NAME CoreTests COMMAND CoreTests)
	add_test(NAME MathTests COMMAND MathTests)
	add_subdirectory(ecs)
endif(${BUILD_TESTING})
