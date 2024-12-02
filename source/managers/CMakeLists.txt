
file(GLOB MANAGER_HEADERS *.hpp)

add_library(BRK_MANAGERS ${BRK_LIBTYPE}
	ECSManager.cpp
	ResourceManager.cpp
	SceneManager.cpp
	${MANAGER_HEADERS}
)
CreateExports(BRK_MANAGERS)

add_library(Breakout::Managers ALIAS BRK_MANAGERS)

target_link_libraries(BRK_MANAGERS PUBLIC
	EnTT::EnTT
)

target_link_libraries(BRK_MANAGERS PRIVATE
	BRK_INCLUDES
	Breakout::Core
	Breakout::ECS
)