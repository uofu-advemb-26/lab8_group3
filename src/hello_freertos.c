/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/cyw43_arch.h"

#include "can.h"

int count = 0;
bool on = false;

#define TX_TASK_PRIORITY      ( tskIDLE_PRIORITY + 1UL )
#define RX_TASK_PRIORITY      ( tskIDLE_PRIORITY + 1UL )
#define BLINK_TASK_PRIORITY     ( tskIDLE_PRIORITY + 2UL )
#define TX_TASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define RX_TASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define BLINK_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

void blink_task(__unused void *params) {
    hard_assert(cyw43_arch_init() == PICO_OK);
    while (true) {
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, on);
        if (count++ % 11) on = !on;
        vTaskDelay(500);
    }
}

void tx_task(__unused void *params) {

    uint8_t tx_data[8] = "A";
    uint32_t id = 0x1FF;

    while (1) {
        can_tx(&cbus, id, 8, tx_data);
        vTaskDelay(1000);
    }
}

void rx_task(__unused void *params) {
    struct can2040_msg msg = {0};
    struct can2040_stats can_stats = {0};

    uint a = 0;
    uint b = 0;

    while (1) {
        if (xQueueReceive(rx_queue, &msg, portMAX_DELAY) == pdPASS) {
            if (msg.data[0] == 'A')
                a++;
            else
                b++;

            printf("Message: %c, A: %d, B:%d\n", msg.data[0], a, b);
        }
    }
}

int main( void )
{
    stdio_init_all();
    const char *rtos_name;
    rtos_name = "FreeRTOS";
    TaskHandle_t r_task;
    TaskHandle_t t_task;

    canbus_setup();

    xTaskCreate(blink_task, "BlinkThread",
                BLINK_TASK_STACK_SIZE, NULL, BLINK_TASK_PRIORITY, NULL);
    xTaskCreate(tx_task, "TXThread",
                TX_TASK_STACK_SIZE, NULL, TX_TASK_PRIORITY, &r_task);
    xTaskCreate(rx_task, "RXThread",
                RX_TASK_STACK_SIZE, NULL, RX_TASK_PRIORITY, &t_task);

    vTaskStartScheduler();
    return 0;
}
