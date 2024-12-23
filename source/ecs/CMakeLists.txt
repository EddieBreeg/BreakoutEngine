AddBrkModule(ECS SOURCES
	ComponentRegistry.cpp
	System.cpp
)

target_link_libraries(BRK_ECS
PUBLIC
	EnTT::EnTT
PRIVATE
	BRK_INCLUDES
	Breakout::Core
)