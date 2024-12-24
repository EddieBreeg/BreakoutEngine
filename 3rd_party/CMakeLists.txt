if(${BRK_INSTALL} AND ${BRK_STANDALONE})
	set(SDL_DISABLE_INSTALL OFF)
endif(${BRK_INSTALL} AND ${BRK_STANDALONE})

set(CMAKE_FOLDER "3rd_party/SDL3")
add_subdirectory(SDL)

set(CMAKE_FOLDER "3rd_party/ImGUI")
add_subdirectory(imgui)

set(CMAKE_FOLDER "3rd_party")

if(NOT ${BRK_INSTALL})
	add_subdirectory(entt EXCLUDE_FROM_ALL)
else()
	add_subdirectory(entt)
endif(NOT ${BRK_INSTALL})

if(${BRK_STANDALONE})
	set(FMT_INSTALL ${BRK_INSTALL})
	set(JSON_Install ${BRK_INSTALL})
	set(GLM_BUILD_INSTALL OFF)	
endif(${BRK_STANDALONE})

add_subdirectory(fmt)
add_subdirectory(json)
add_subdirectory(glm)
add_subdirectory(stb)
add_subdirectory(bzip2)

# We need to install glm manually, because its uninstall target conflicts with SDL's...
if(${BRK_INSTALL})
	install(FILES entt/LICENSE DESTINATION licenses/EnTT)
	install(FILES fmt/LICENSE DESTINATION licenses/fmt)
	install(FILES glm/copying.txt DESTINATION licenses/glm)
	install(FILES imgui/LICENSE.txt DESTINATION licenses/imgui)
	install(FILES json/LICENSE.mit DESTINATION licenses/json)

	install(TARGETS glm-header-only glm EXPORT glm)
	install(
		DIRECTORY glm/glm
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
		PATTERN "CMakeLists.txt" EXCLUDE
	)
	install(
		EXPORT glm
		NAMESPACE glm::
		DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/glm
		FILE glmConfig.cmake
	)
endif(${BRK_INSTALL})