file(GLOB MATH_H *.h *.hpp)

add_library(BREAKOUT_MATH INTERFACE)

target_sources(BREAKOUT_MATH INTERFACE
	${MATH_H}
)
target_compile_features(BREAKOUT_MATH INTERFACE cxx_std_17)
add_library(Breakout::Math ALIAS BREAKOUT_MATH)
target_link_libraries(BREAKOUT_MATH INTERFACE BREAKOUT_INCLUDES)