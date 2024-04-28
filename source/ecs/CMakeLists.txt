
file(GLOB ECS_HEADERS *.hpp)

add_library(BRK_ECS 
	System.cpp
	${ECS_HEADERS}
)

add_library(Breakout::ECS ALIAS BRK_ECS)

target_link_libraries(BRK_ECS PUBLIC
	EnTT::EnTT
)

target_link_libraries(BRK_ECS PRIVATE
	BRK_INCLUDES
	Breakout::Core
)