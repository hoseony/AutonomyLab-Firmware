add_library(ems22a STATIC
    ems22a.c
)

target_include_directories(ems22a
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(ems22a
    PUBLIC
        hardware_pio
    PRIVATE
        rp2350_drivers
        pico_stdlib
        hardware_clocks
)

pico_generate_pio_header(ems22a
    ${CMAKE_CURRENT_LIST_DIR}/ems22a.pio
)
