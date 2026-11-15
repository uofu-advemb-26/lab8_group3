#include "can.h"

#include <hardware/regs/intctrl.h>
#include <pico/stdlib.h>
#include <stdio.h>
#include <string.h>

struct can2040 cbus = {0};
QueueHandle_t rx_queue;

static void can2040_cb(struct can2040 *cd, uint32_t notify, struct can2040_msg *msg)
{
    if (!xQueueIsQueueFullFromISR(rx_queue))
        xQueueSendFromISR(rx_queue, msg, NULL);
}

static void PIOx_IRQHandler(void)
{
    can2040_pio_irq_handler(&cbus);
}

int can_tx(struct can2040 *can_bus, uint32_t id, uint32_t dlc, uint8_t data[])
{
    struct can2040_msg msg = {0};
    msg.id = id;
    msg.dlc = dlc;
    memcpy(msg.data, data, 8);

    // int result = can2040_check_transmit(&cbus);
    // if (result) {
    //     printf("Check transmit succeeded\n");
    // }
    // else {
    //     printf("Check transmit failed %d\n", result);
    // }

    int result = can2040_transmit(&cbus, &msg);

    // if (result)
    //     printf("Failed to transmit (%d)\n", result);

    return result;
}

void canbus_setup(void)
{
    uint32_t pio_num = 0;
    uint32_t sys_clock = 125000000, bitrate = 500000;
    uint32_t gpio_rx = CAN_RX_PIN, gpio_tx = CAN_TX_PIN;

    rx_queue = xQueueCreate(100, sizeof(struct can2040_msg));

    // Setup canbus
    can2040_setup(&cbus, pio_num);
    can2040_callback_config(&cbus, can2040_cb);

    // Enable irqs
    irq_set_exclusive_handler(PIO0_IRQ_0, PIOx_IRQHandler);
    irq_set_priority(PIO0_IRQ_0, PICO_DEFAULT_IRQ_PRIORITY - 1);
    irq_set_enabled(PIO0_IRQ_0, 1);

    // Start canbus
    can2040_start(&cbus, sys_clock, bitrate, gpio_rx, gpio_tx);
}
