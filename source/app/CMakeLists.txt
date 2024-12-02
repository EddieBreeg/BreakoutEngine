add_library(BRK_APP STATIC 
	App.cpp 
	Entry.cpp 
	TypeRegistration.cpp
	App.hpp 
	Entry.hpp)

target_link_libraries(BRK_APP PRIVATE
	Breakout::Core
	Breakout::Editor
	Breakout::ECS
	Breakout::Managers
)
target_link_libraries(BRK_APP PRIVATE
	SDL3::SDL3
	nlohmann_json::nlohmann_json
)
add_library(Breakout::App ALIAS BRK_APP)