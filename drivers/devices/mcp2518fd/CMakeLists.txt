add_library(mcp2518fd STATIC
	mcp2518fd.c
)

target_include_directories(mcp2518fd PUBLIC include)
target_link_libraries(mcp2518fd PUBLIC
	pico_stdlib
	hardware_spi
)
