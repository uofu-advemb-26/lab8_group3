#ifndef _CAN_H
#include <can2040.h>
#include "FreeRTOS.h"
#include "queue.h"

// Raspberry Pi Pico GPIO pins (USB facing forward)
#define CAN_TX_PIN 1        // Top right
#define CAN_RX_PIN 2        // 2nd top right
#define CAN_STANDBY_PIN 16  // Bottom right

#define HIGH_PRIORITY_MSG "A"
#define HIGH_PRIORITY_ID 0x00

#define LOW_PRIORITY_MSG "B"
#define LOW_PRIORITY_ID 0xFF

#define CAN_MESSAGE HIGH_PRIORITY_MSG
#define CAN_ID HIGH_PRIORITY_ID

extern struct can2040 cbus;
extern QueueHandle_t rx_queue;

static void can2040_cb(struct can2040 *cd, uint32_t notify, struct can2040_msg *msg);
static void PIOx_IRQHandler(void);
void canbus_setup(void);
int can_tx(struct can2040 *can_bus, uint32_t id, uint32_t dlc, uint8_t data[]);

#endif	// _CAN_H
