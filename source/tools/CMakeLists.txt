add_executable(ULIDGen ULIDGen.cpp)
target_link_libraries(ULIDGen PRIVATE Breakout::Core)

add_library(Base85 INTERFACE)
target_include_directories(Base85 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Base85 INTERFACE Breakout::Core)

add_library(DataEmbedLib STATIC DataEmbed.cpp)
target_link_libraries(DataEmbedLib 
	PUBLIC Breakout::Core
	PRIVATE Base85 BZip2::BZLib)
target_compile_features(DataEmbedLib PUBLIC cxx_std_17)

add_executable(AssetEmbedder EmbedAsset.cpp)
target_link_libraries(AssetEmbedder PRIVATE DataEmbedLib)
set_target_properties(AssetEmbedder PROPERTIES
	OUTPUT_NAME EmbedAsset
)

add_library(Breakout::EmbedLib ALIAS DataEmbedLib)