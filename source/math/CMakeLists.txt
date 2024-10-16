file(GLOB MATH_H *.h *.hpp)

add_library(BRK_MATH INTERFACE)

target_sources(BRK_MATH INTERFACE
	${MATH_H}
)
target_compile_features(BRK_MATH INTERFACE cxx_std_17)
add_library(Breakout::Math ALIAS BRK_MATH)
target_link_libraries(BRK_MATH INTERFACE BRK_INCLUDES glm::glm)