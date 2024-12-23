include(GenerateExportHeader)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

function(AddBrkModule MOD_NAME)
	cmake_parse_arguments(MOD "" "ALIAS;LIB_TYPE" "SOURCES" ${ARGN})
	set(MOD_LIB_NAME BRK_${MOD_NAME})
	if(NOT DEFINED MOD_LIB_TYPE)
		set(MOD_LIB_TYPE ${BRK_LIBTYPE})
	endif(NOT DEFINED MOD_LIB_TYPE)

	add_library(${MOD_LIB_NAME} ${MOD_LIB_TYPE} ${MOD_SOURCES})
	file(GLOB_RECURSE MOD_HEADERS *.h *.hpp)
	target_sources(${MOD_LIB_NAME} PUBLIC
		FILE_SET HEADERS
		TYPE HEADERS
		FILES ${MOD_HEADERS}
	)
	target_compile_features(${MOD_LIB_NAME} PUBLIC cxx_std_17)
	target_compile_definitions(${MOD_LIB_NAME} PRIVATE ${MOD_LIB_NAME}_EXPORTS=1)
	generate_export_header(${MOD_LIB_NAME}
		EXPORT_MACRO_NAME ${MOD_LIB_NAME}_API
	)

	if(DEFINED MOD_ALIAS)
		add_library(Breakout::${MOD_ALIAS} ALIAS ${MOD_LIB_NAME})
	else()
		add_library(Breakout::${MOD_NAME} ALIAS ${MOD_LIB_NAME})
	endif(DEFINED MOD_ALIAS)
endfunction(AddBrkModule MOD_NAME)

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

set(CMAKE_FOLDER "Breakout Engine/Tools")
add_subdirectory(tools)

function(EmbedAsset AssetFile AssetName NamespaceName)
	set(OutputFile ${CMAKE_CURRENT_BINARY_DIR}/${AssetName}.hpp)
	set(Flags USE_BASE85)
	set(Args)
	set(ListArgs)
	cmake_parse_arguments(EMBED "${Flags}" "${Args}" "${ListArgs}" ${ARGN})
	set(CommandArgs "${AssetFile}" "${AssetName}" "${NamespaceName}")

	if (${EMBED_USE_BASE85})
		list(APPEND CommandArgs "-b")
	endif(${EMBED_USE_BASE85})
	
	add_custom_command(OUTPUT ${OutputFile} 
		COMMAND $<TARGET_FILE:AssetEmbedder> ${CommandArgs} > "${OutputFile}"
		DEPENDS AssetEmbedder ${AssetFile}
	)
endfunction(EmbedAsset AssetFile AssetName NamespaceName)

set(CMAKE_FOLDER "Breakout Engine")

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