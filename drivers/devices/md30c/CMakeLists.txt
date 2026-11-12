add_library(md30c STATIC
	md30c.c
)

target_include_directories(md30c PUBLIC include)
target_link_libraries(md30c PUBLIC rp2350_drivers)
