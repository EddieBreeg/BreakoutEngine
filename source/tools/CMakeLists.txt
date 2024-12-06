add_executable(ULIDGen ULIDGen.cpp)
target_link_libraries(ULIDGen PRIVATE Breakout::Core)

add_library(Base85 INTERFACE)
target_include_directories(Base85 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Base85 INTERFACE Breakout::Core)