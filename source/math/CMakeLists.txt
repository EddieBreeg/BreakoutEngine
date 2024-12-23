file(GLOB MATH_HEADERS *.hpp)

add_library(BRK_MATH INTERFACE)
add_library(Breakout::Math ALIAS BRK_MATH)

target_sources(BRK_MATH INTERFACE
	FILE_SET HEADERS
	FILES ${MATH_HEADERS}
)

target_link_libraries(BRK_MATH INTERFACE BRK_INCLUDES glm::glm)