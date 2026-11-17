# Observations
For osciloscope images, channel 1 shows the CANH or TX signals and channel 2 shows the CANL or RX signals.

## Activity 1
The [CAN signals](images/activity_1-can_signals.png) and [Pico signals](images/activity_1-pico_signals.png) images show the waveforms of the CAN bus and the TX/RX signals from the transmitting pico, respectively.

## Activity 2
The [CAN high-priority](images/activity_2-high_priority.png) image shows the waveforms of the CAN bus with a babbling node with no delay.

No delay for node that is sending high priority message:
- The oscilloscope is flooded with high priority messages.
- Low priority message rarely is transmitted
- 0.029% of transmitted messages received are low priority

100 ms delay on babbling node:
- 10 high priority messages sent for every low priority message

500 ms delay on babbling node:
- Twice as many high priority messages are sent as low priority
