# Example Code


## Simple Send
The example `Send.cpp` initialises the correct pins for `SPI0` for the CAN HAT and sends on loop a CAN frame with ID `0x123` and 8 Bytes of Data `11 22 33 44 55 66 77 88`.

## Simple Receive
The example `receive.cpp` initialises the correct pins for `SPI0` for the CAN HAT and prints all complete CAN frames it receives.

## Send Telemetry