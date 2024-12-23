
AddBrkModule(MANAGERS ALIAS Managers SOURCES
	ECSManager.cpp
	ResourceManager.cpp
	SceneManager.cpp
)

target_link_libraries(BRK_MANAGERS
PUBLIC
	EnTT::EnTT
PRIVATE
	BRK_INCLUDES
	Breakout::Core
	Breakout::ECS
)