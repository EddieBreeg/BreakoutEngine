
file(GLOB SYSTEMS_H *.h *.hpp)

add_library(BREAKOUT_SYSTEMS
	VisualSystem.cpp
	WindowSystem.cpp
	InputSystem.cpp
	${SYSTEMS_H}
)

target_link_libraries(BREAKOUT_SYSTEMS PUBLIC
	Breakout::Core
	Breakout::ECS
)

add_library(Breakout::Systems ALIAS BREAKOUT_SYSTEMS)