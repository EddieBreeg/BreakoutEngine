file(GLOB CORE_HEADERS *.hpp)

add_library(BREAKOUT_CORE STATIC
	App.cpp
	Assert.cpp
	LogManager.cpp
	TimeInfo.cpp
	${CORE_HEADERS}
)

target_compile_features(BREAKOUT_CORE PUBLIC cxx_std_17)
add_library(Breakout::Core ALIAS BREAKOUT_CORE)

target_link_libraries(BREAKOUT_CORE PUBLIC 
	BREAKOUT_INCLUDES
	fmt::fmt
	EnTT::EnTT
	SDL2::SDL2
)