add_executable(Base85Tests Base85Tests.cpp)
target_link_libraries(Base85Tests PRIVATE Base85)

EmbedAsset(${CMAKE_CURRENT_SOURCE_DIR}/Lorem.txt Lorem brk::asset_embed::ut)
EmbedAsset(${BRK_ROOT}/example/Icon.ico BrkIcon brk::asset_embed::ut)

add_executable(AssetEmbeddingTests 
	AssetEmbedding.cpp
	${CMAKE_CURRENT_BINARY_DIR}/Lorem.hpp
	${CMAKE_CURRENT_BINARY_DIR}/BrkIcon.hpp
)
target_include_directories(AssetEmbeddingTests PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(AssetEmbeddingTests PRIVATE Breakout::Core Breakout::EmbedLib BRK_INCLUDES)

add_test(NAME Base85Tests COMMAND Base85Tests)
add_test(NAME AssetEmbedding COMMAND AssetEmbeddingTests)