add_executable(ex_freertos_blink main.c)

target_link_libraries(ex_freertos_blink
	freertos
	pico_stdlib
	rp2350_drivers
)



pico_add_extra_outputs(ex_freertos_blink)
