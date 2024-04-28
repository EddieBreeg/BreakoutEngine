add_library(BRK_APP STATIC App.cpp App.hpp)
target_link_libraries(BRK_APP PUBLIC
	Breakout::Core
	Breakout::Editor
	Breakout::ECS
)
target_link_libraries(BRK_APP PRIVATE
	SDL2::SDL2
)
add_library(Breakout::App ALIAS BRK_APP)