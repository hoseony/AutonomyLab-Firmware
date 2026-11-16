# Add freeRTOS as interface library.
# It doesn't build anything itself.
add_library(freertos INTERFACE)

# include this folder (for FreeRTOSConfig.h)
target_include_directories(freertos INTERFACE .)

# actually include freeRTOS library
target_link_libraries(freertos INTERFACE
	FreeRTOS-Kernel
	FreeRTOS-Kernel-Heap4
)
