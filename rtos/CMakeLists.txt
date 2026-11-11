add_library(freertos INTERFACE)
target_include_directories(freertos INTERFACE
	${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(freertos INTERFACE
	FreeRTOS-Kernel
	FreeRTOS-Kernel-Heap4
)
