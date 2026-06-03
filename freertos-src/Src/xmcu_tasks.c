#include "xmcu_tasks.h"


void Task_led_toggle(void *pvParameters) {
    Led led = Led_1;
    (void)pvParameters;

    for (;;) {
        led_toggle(led);
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}

void tasks_init(void) {
    xTaskCreate(Task_led_toggle, "LED toggle", 2048, NULL, 1, NULL);
}

