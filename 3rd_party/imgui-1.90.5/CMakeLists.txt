add_library(IMGUI STATIC
	imgui_draw.cpp
	imgui_tables.cpp
	imgui_widgets.cpp
	imgui.cpp
)
target_include_directories(IMGUI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(backends)
target_link_libraries(IMGUI PUBLIC IMGUI_SDL2)

if(${BRK_BUILD_TESTING})
	target_sources(IMGUI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui_demo.cpp)
endif(${BRK_BUILD_TESTING})

add_library(ImGui::ImGui ALIAS IMGUI)