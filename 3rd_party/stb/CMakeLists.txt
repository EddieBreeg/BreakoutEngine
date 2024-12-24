add_library(stb_image INTERFACE)
target_sources(stb_image INTERFACE
	FILE_SET HEADERS FILES stb_image.h
)
target_include_directories(stb_image INTERFACE 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

set_target_properties(stb_image PROPERTIES
	EXPORT_NAME Image
)

add_library(STB::Image ALIAS stb_image)

if(${BRK_INSTALL})
	install(TARGETS stb_image EXPORT STBExports
		FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	)
	install(FILES LICENSE DESTINATION licenses/stb)
	install(EXPORT STBExports 
		DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/STB
		NAMESPACE STB
		FILE STBConfig.cmake
	)
endif(${BRK_INSTALL})
