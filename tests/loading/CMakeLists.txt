add_executable(LoadingTests main.cpp
	ComponentInfoTests.cpp
	ProjectLoadingTests.cpp
	SceneLoadingTests.cpp
)
target_compile_definitions(LoadingTests PRIVATE
	DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data"
)

target_link_libraries(LoadingTests PRIVATE
	Breakout::Core
	Breakout::ECS
	Breakout::EditorLib
	Breakout::Managers
	Breakout::Systems
	ImGui::ImGui
)
add_test(NAME LoadingTests COMMAND LoadingTests)