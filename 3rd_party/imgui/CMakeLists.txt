file(GLOB IMGUI_HEADERS *.h)
add_library(IMGUI STATIC
	imgui_draw.cpp
	imgui_tables.cpp
	imgui_widgets.cpp
	imgui.cpp
	misc/cpp/imgui_stdlib.cpp
)
set_target_properties(IMGUI PROPERTIES
	EXPORT_NAME ImGui
)
target_sources(IMGUI PUBLIC
	FILE_SET HEADERS
	FILES ${IMGUI_HEADERS}
)
target_include_directories(IMGUI PUBLIC 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/misc/cpp>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/imgui>
)

add_subdirectory(backends)
target_link_libraries(IMGUI PUBLIC IMGUI_SDL3)

if(${BRK_BUILD_TESTING})
	target_sources(IMGUI PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/imgui_demo.cpp)
endif(${BRK_BUILD_TESTING})

add_library(ImGui::ImGui ALIAS IMGUI)

if(${BRK_INSTALL})
	install(TARGETS IMGUI EXPORT ImGuiExports
		CONFIGURATIONS RelWithDebInfo
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
		FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/imgui
	)
	install(EXPORT ImGuiExports
		DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/ImGui
		NAMESPACE ImGui
		FILE ImGuiConfig.cmake
	)
endif(${BRK_INSTALL})