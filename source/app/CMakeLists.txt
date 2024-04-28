add_library(BRK_APP STATIC App.cpp App.hpp)
target_link_libraries(BRK_APP PUBLIC
	Breakout::Core)
add_library(Breakout::App ALIAS BRK_APP)