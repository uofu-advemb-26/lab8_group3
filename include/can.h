#ifndef _CAN_H
#include <can2040.h>

// Raspberry Pi Pico GPIO pins (USB facing forward)
#define CAN_TX_PIN 1        // Top right
#define CAN_RX_PIN 2        // 2nd top right
#define CAN_STANDBY_PIN 16  // Bottom right

static struct can2040 cbus;

static void can2040_cb(struct can2040 *cd, uint32_t notify, struct can2040_msg *msg);
static void PIOx_IRQHandler(void);
void canbus_setup(void);

#endif	// _CAN_H
