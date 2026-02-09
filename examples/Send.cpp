/*

    Example code to transmit CAN messages using the Pico-CAN HAT

*/

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "mcp2515/mcp2515.h"

// Waveshare Pico-CAN-B Pinout
#define SPI_PORT spi0
#define PIN_SCK 6
#define PIN_MOSI 7
#define PIN_MISO 4
#define PIN_CS 5
#define PIN_INT 21

MCP2515 can0(SPI_PORT, PIN_CS, PIN_INT);
struct can_frame tx;

int main() {
    stdio_init_all();
    sleep_ms(2000); //Small delay so the USB shows up

    spi_init(SPI_PORT, 10 * 1000 * 1000); // 10 MHz SPI

    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);  
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    //Set CS high all the time (not good for using with others)
    gpio_init(PIN_CS);
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1);

    printf("SPI initialized\n");

    if (can0.reset() != MCP2515::ERROR_OK) {
        printf("MCP2515 reset failed\n");
        while (1);
    }

    if (can0.setBitrate(CAN_1000KBPS, MCP_16MHZ) != MCP2515::ERROR_OK) {
        printf("Bitrate set failed\n");
        while (1);
    }

    if (can0.setNormalMode() != MCP2515::ERROR_OK) {
        printf("Normal mode failed\n");
        while (1);
    }
    
    printf("MCP2515 ready, transmitting...\n");

    tx.can_id = 0x123;  // standard 11-bit ID
    tx.can_dlc = 8;     //Sending 8 Byte of data

    tx.data[0] = 0x11;
    tx.data[1] = 0x22;
    tx.data[2] = 0x33;
    tx.data[3] = 0x44;
    tx.data[4] = 0x55;
    tx.data[5] = 0x66;
    tx.data[6] = 0x77;
    tx.data[7] = 0x88;
  
    while (1) {
        MCP2515::ERROR err = can0.sendMessage(&tx);

        if (err == MCP2515::ERROR_OK) {
            printf("TX OK: ID=0x%03X\n", tx.can_id);
        } else {
            printf("TX ERROR: %d\n", err);
        }
        sleep_ms(500);
    }
} // END of main()