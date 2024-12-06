add_executable(Base85Tests Base85Tests.cpp)
target_link_libraries(Base85Tests PRIVATE Base85)

add_test(NAME Base85Tests COMMAND Base85Tests)