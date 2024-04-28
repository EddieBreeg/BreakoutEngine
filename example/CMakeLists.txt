add_executable(Sandbox main.cpp)
target_link_libraries(Sandbox PRIVATE ${PROJECT_NAME})

if(${WIN32})
	target_sources(Sandbox PRIVATE Icon.rc)
endif()