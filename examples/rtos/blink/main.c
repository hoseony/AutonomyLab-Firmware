#include "FreeRTOS.h"
#include "task.h"
#include "pico/stdlib.h"
#include "stdbool.h"
#include "gpio.h"

const uint LED_PIN = 25;

static void blink_task(void *param) {
	gpio_init(LED_PIN);
	gpio_set_dir(LED_PIN, GPIO_OUT);

	bool on = false;

    while(1) {
        on = !on;
        rp_gpio_write(LED_PIN, on);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

int main(void) {
    // create a task
    // task, pcName, stackDepth?, priority, CreatedTask
    xTaskCreate(blink_task, "blink", 256, NULL, 1, NULL);
    // start scheduler
    vTaskStartScheduler();

    while(1) {
        ;
    }
}
