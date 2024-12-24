function(ImGuiBackend backendName)
	string(TOUPPER "IMGUI_${backendName}" LIBNAME)
	add_library(${LIBNAME} STATIC 
		"imgui_impl_${backendName}.cpp"
		"imgui_impl_${backendName}.h"
	)
	install(FILES "imgui_impl_${backendName}.h" 
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/imgui/backends
	)
	target_include_directories(${LIBNAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)
	if(${BRK_INSTALL})
		install(TARGETS ${LIBNAME} EXPORT ImGuiExports
			ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
		)
	endif(${BRK_INSTALL})
	
endfunction(ImGuiBackend)

set(SDL_TARGET SDL3::SDL3)
set(SDL_BACKEND_NAME sdl3)

string(TOUPPER "IMGUI_${SDL_BACKEND_NAME}" SDL_BACKEND_LIB)

ImGuiBackend(${SDL_BACKEND_NAME})
target_link_libraries(${SDL_BACKEND_LIB} PRIVATE ${SDL_TARGET})

if(${BRK_RENDER_BACKEND} STREQUAL "DX11")
	ImGuiBackend(dx11)
	target_link_libraries(${SDL_BACKEND_LIB} PRIVATE IMGUI_DX11)
elseif(${BRK_RENDER_BACKEND} STREQUAL "Vulkan")
	ImGuiBackend(vulkan)
	target_link_libraries(${SDL_BACKEND_LIB} PRIVATE IMGUI_VULKAN)
elseif(${BRK_RENDER_BACKEND} STREQUAL "OpenGL")
	ImGuiBackend(opengl3)
	target_link_libraries(${SDL_BACKEND_LIB} PRIVATE IMGUI_OPENGL)
elseif(${BRK_RENDER_BACKEND} STREQUAL "SDL3")
	ImGuiBackend(sdlrenderer3)
	target_link_libraries(IMGUI_SDLRENDERER3 PRIVATE ${SDL_TARGET})
	target_link_libraries(${SDL_BACKEND_LIB} PRIVATE IMGUI_SDLRENDERER3)
endif()