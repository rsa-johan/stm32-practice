#include "signals.h"
#include "xmcu_tasks.h"

#include "base/common.h"
#include "base/clock.h"
#include "base/gpio.h"

#include "functions/led.h"

void initialize() {
    /* System level */
    SystemInit();
    HAL_Init();
    clock_init();

    /* Base level */
    gpio_init();

    /* Function level */
    led_setup();

    /* Tasks */
    tasks_init();
}

void run() {
   vTaskStartScheduler();
}

int main(void) {
    initialize();
    run();
    return 0;
}