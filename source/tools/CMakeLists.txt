add_executable(ULIDGen ULIDGen.cpp)
target_link_libraries(ULIDGen PRIVATE Breakout::Core)

add_library(Base85 INTERFACE)
target_link_libraries(Base85 INTERFACE Breakout::Core)

add_library(DataEmbedLib STATIC DataEmbed.cpp)

target_link_libraries(DataEmbedLib 
	PRIVATE Breakout::Core
	Base85 BZip2::BZLib)
set_target_properties(DataEmbedLib PROPERTIES
	EXPORT_NAME EmbedLib
	OUTPUT_NAME_DEBUG DataEmbedLib_debug
)

target_compile_features(DataEmbedLib PUBLIC cxx_std_17)

add_executable(AssetEmbedder EmbedAsset.cpp)
target_link_libraries(AssetEmbedder PRIVATE Breakout::Core DataEmbedLib)
set_target_properties(AssetEmbedder PROPERTIES
	OUTPUT_NAME EmbedAsset
)

add_library(Breakout::EmbedLib ALIAS DataEmbedLib)

if(${BRK_INSTALL})
	install(TARGETS Base85 DataEmbedLib EXPORT BreakoutExports
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	)
	
	install(TARGETS ULIDGen AssetEmbedder EXPORT BreakoutExports
		CONFIGURATIONS Release
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/tools
	)
endif(${BRK_INSTALL})
