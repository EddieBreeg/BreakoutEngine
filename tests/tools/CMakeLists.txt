add_executable(Base85Tests Base85Tests.cpp)
target_link_libraries(Base85Tests PRIVATE Base85)

add_executable(AssetEmbeddingTests AssetEmbedding.cpp)
target_link_libraries(AssetEmbeddingTests PRIVATE Breakout::Core Breakout::EmbedLib BRK_INCLUDES)

target_compile_definitions(AssetEmbeddingTests PRIVATE DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")

add_test(NAME Base85Tests COMMAND Base85Tests)
add_test(NAME AssetEmbedding COMMAND AssetEmbeddingTests)