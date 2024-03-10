
file(GLOB ECS_HEADERS *.hpp)

add_library(BREAKOUT_ECS 
	System.cpp
	${ECS_HEADERS}
)

add_library(Breakout::ECS ALIAS BREAKOUT_ECS)

target_link_libraries(BREAKOUT_ECS PUBLIC
	BREAKOUT_INCLUDES
	Breakout::Core
	EnTT::EnTT
)