add_executable(ex_md30c ex_md30c.c)
target_link_libraries(ex_md30c rp2350_drivers md30c)
pico_add_extra_outputs(ex_md30c)
