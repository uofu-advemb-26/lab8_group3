#ifndef _CAN_H
#include <can2040.h>

static struct can2040 cbus;

static void can2040_cb(struct can2040 *cd, uint32_t notify, struct can2040_msg *msg);
static void PIOx_IRQHandler(void);
void canbus_setup(void);

#endif	// _CAN_H
