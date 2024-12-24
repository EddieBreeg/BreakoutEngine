file(GLOB MATH_HEADERS *.hpp)

add_library(BRK_MATH INTERFACE)
add_library(Breakout::Math ALIAS BRK_MATH)

set_target_properties(BRK_MATH PROPERTIES
	EXPORT_NAME Math
)

if(${BRK_INSTALL})
	install(TARGETS BRK_MATH EXPORT BreakoutExports)
endif(${BRK_INSTALL})

target_link_libraries(BRK_MATH INTERFACE BRK_INCLUDES glm::glm)