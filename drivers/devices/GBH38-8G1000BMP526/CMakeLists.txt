
add_library(gbh38_driver STATIC
    gbh38.c
)

target_include_directories(gbh38_driver PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(gbh38_driver PUBLIC
    hardware_pio
)

pico_generate_pio_header(
    gbh38_driver
    ${CMAKE_CURRENT_LIST_DIR}/gbh38.pio
)
