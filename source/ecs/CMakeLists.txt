
file(GLOB ECS_HEADERS *.hpp)

add_library(BRK_ECS 
	System.cpp
	ECSManager.cpp
	${ECS_HEADERS}
)

add_library(Breakout::ECS ALIAS BRK_ECS)

target_link_libraries(BRK_ECS PUBLIC
	BRK_INCLUDES
	Breakout::Core
	EnTT::EnTT
)