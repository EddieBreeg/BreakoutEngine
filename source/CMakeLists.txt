include(GenerateExportHeader)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

function(CreateExports LIBNAME)
	target_compile_definitions(${LIBNAME} PRIVATE ${LIBNAME}_EXPORTS=1)
	generate_export_header(${LIBNAME}
		EXPORT_MACRO_NAME ${LIBNAME}_API
	)
endfunction(CreateExports LIBNAME)

add_library(BRK_INCLUDES INTERFACE)
target_include_directories(BRK_INCLUDES INTERFACE
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
)
target_compile_definitions(BRK_INCLUDES INTERFACE
	_BRK_EDITOR_ENABLED=$<CONFIG:Debug>
	_BRK_DEBUG_ENABLED=$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>
	GLM_FORCE_DEPTH_ZERO_TO_ONE=1,
)
add_library(Breakout::Includes ALIAS BRK_INCLUDES)

add_subdirectory(math)
add_subdirectory(core)

set(CMAKE_FOLDER "Engine/Tools")
add_subdirectory(tools)

function(EmbedAsset AssetFile AssetName NamespaceName)
	set(OutputFile ${CMAKE_CURRENT_BINARY_DIR}/${AssetName}.hpp)
	set(Flags USE_BASE85)
	set(Args)
	set(ListArgs)
	cmake_parse_arguments(arg "${Flags}" "${Args}" "${ListArgs}" ${ARGN})
	set(CommandArgs "${AssetFile}" "${AssetName}" "${NamespaceName}")

	if (${arg_USE_BASE85})
		list(APPEND CommandArgs "-b")
	endif(${arg_USE_BASE85})
	
	add_custom_command(OUTPUT ${OutputFile} 
		COMMAND $<TARGET_FILE:AssetEmbedder> ${CommandArgs} > "${OutputFile}"
		DEPENDS AssetEmbedder ${AssetFile}
	)
endfunction(EmbedAsset AssetFile AssetName NamespaceName)

set(CMAKE_FOLDER "Engine")

add_subdirectory(rendering)
add_subdirectory(ecs)
add_subdirectory(managers)
add_subdirectory(imgui)
add_subdirectory(systems)
add_subdirectory(editor)
add_subdirectory(app)

add_library(${PROJECT_NAME} INTERFACE)

target_link_libraries(${PROJECT_NAME} INTERFACE
	BRK_INCLUDES
	Breakout::Core
	Breakout::DevUi
	Breakout::ECS
	Breakout::Managers
	Breakout::Systems
	Breakout::Math
	Breakout::Editor
	Breakout::Rendering
	Breakout::App
)

target_precompile_headers(${PROJECT_NAME} INTERFACE PCH.hpp)
add_library(Breakout::Breakout ALIAS ${PROJECT_NAME})