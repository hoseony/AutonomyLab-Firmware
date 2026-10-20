add_library(rp2350_drivers STATIC
    resets.c
)

target_include_directories(rp2350_drivers PUBLIC include)
