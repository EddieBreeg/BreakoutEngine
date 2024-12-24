cmake_minimum_required(VERSION 3.20)
project(BZip2)

add_library(BZLib2 STATIC 
	blocksort.c
	bzlib.c
	compress.c
	crctable.c
	decompress.c
	huffman.c
	randtable.c
)
target_sources(BZLib2 PUBLIC
	FILE_SET HEADERS
	FILES bzlib.h
)
set_target_properties(BZLib2 PROPERTIES
	EXPORT_NAME BZLib
	OUTPUT_NAME BZLib
	OUTPUT_NAME_RELWITHDEBINFO BZLib_debug
)
target_include_directories(BZLib2 PUBLIC 
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
	$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
add_library(BZip2::BZLib ALIAS BZLib2)
if(${BRK_INSTALL})
	install(TARGETS BZLib2 EXPORT BZip2Exports
		CONFIGURATIONS RelWithDebInfo Release
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
		FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	)
	install(FILES LICENSE DESTINATION licenses/bzip2)
	install(EXPORT BZip2Exports 
		DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/BZip2
		NAMESPACE BZip2::
		FILE BZip2Config.cmake
	)
endif(${BRK_INSTALL})
