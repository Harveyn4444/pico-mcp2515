/*

    Example code to receive CAN messages using the Pico-CAN HAT

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
struct can_frame rx;

struct Telemetry{
    int16_t pico_temperature = 0;
    int16_t time_since_boot = 0;
};
struct Telemetry data;


void handleTelemetry(void);

int main() {
    stdio_init_all();
    sleep_ms(2000);//Small delay so the USB shows up

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

    printf("MCP2515 ready, waiting for frames...\n");  

    while (1) {
        if (can0.readMessage(&rx) == MCP2515::ERROR_OK) {
            printf("-------------------\n");
            printf("RX: ID=0x%03X DLC=%d DATA=",rx.can_id,rx.can_dlc);

            for (int i = 0; i < rx.can_dlc; i++) {
                printf("%02X ", rx.data[i]);
            }
            printf("\n");

            handleTelemetry();
        }
    }
} // END of main

void handleTelemetry(void){

    switch(rx.data[0]){
        case 0x02:
            printf("Telemetry Packet: \n");
            printf("Time Since Boot: ");
            data.time_since_boot = (rx.data[1] << 8) | (rx.data[2]);
            printf("%d seconds\n" , data.time_since_boot);
            data.pico_temperature = (rx.data[3] << 8) | (rx.data[4]);
            printf("Temperature: ");
            printf("%.2f C\n", data.pico_temperature / 100.0f);


        default:
            break;        
    }

} // END of handleTelemetry