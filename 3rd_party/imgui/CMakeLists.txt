file(GLOB IMGUI_HEADERS *.h)
add_library(IMGUI STATIC
	imgui_draw.cpp
	imgui_tables.cpp
	imgui_widgets.cpp
	imgui.cpp
	misc/cpp/imgui_stdlib.cpp
	${IMGUI_HEADERS}
)
target_include_directories(IMGUI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/misc/cpp)

add_subdirectory(backends)
target_link_libraries(IMGUI PUBLIC IMGUI_SDL3)

if(${BRK_BUILD_TESTING})
	target_sources(IMGUI PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/imgui_demo.cpp)
endif(${BRK_BUILD_TESTING})

add_library(ImGui::ImGui ALIAS IMGUI)