#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "mcp2515/mcp2515.h"
#include "hardware/adc.h" // temp for adc_init();


/*****************************************/
// CAN
// Waveshare Pico-CAN-B Pinout
#define SPI_PORT spi0
#define PIN_SCK 6
#define PIN_MOSI 7
#define PIN_MISO 4
#define PIN_CS 5
#define PIN_INT 21
MCP2515 can0(SPI_PORT, PIN_CS, PIN_INT);
struct can_frame tx;
/*****************************************/

//Declaring Functions
int16_t measure_pico_temperature(void);
void Telemetry(void);

struct Telemetry{
    // __uint16_t pico_temperature;
    int16_t pico_temperature = 0;
    // unsigned int msSinceBoot = 0;
    int16_t time_since_boot = 0;
};

struct Telemetry data;

// Varibales for tracking the time
int prevMillis = 0;
int curentMillis = 0;
const int interval = 1000; // ms to sleep
unsigned int msSinceBoot = 0;


int main(){
    msSinceBoot = to_ms_since_boot(get_absolute_time());
    //currentMillis = msSinceBoot;

    stdio_init_all();
    sleep_ms(2000); //Small delay so the USB shows up

    // Initialize ADC hardware
    adc_init();


    spi_init(SPI_PORT, 10 * 1000 * 1000); // 10 MHz SPI

    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);  
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    //Set CS high all the time (not good for using with others)
    gpio_init(PIN_CS);
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1);

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


    //Infinite Loop
    while(1){
        msSinceBoot = to_ms_since_boot(get_absolute_time());

        if(msSinceBoot - prevMillis >= interval){
            prevMillis = msSinceBoot;
            Telemetry();
            MCP2515::ERROR err = can0.sendMessage(&tx);

            if (err == MCP2515::ERROR_OK) {
                printf("TX OK: ID=0x%03X\n", tx.can_id);
            } else {
                printf("TX ERROR: %d\n", err);
            }
        }
    }
    
    return 0;

}

void Telemetry(void){
    data.time_since_boot = (float)msSinceBoot/100.0f;
    data.pico_temperature = measure_pico_temperature();

    printf("%d,", data.time_since_boot);
    printf("%d", (data.pico_temperature));
    printf("\n");

    tx.can_id = 0x123;  // standard 11-bit ID
    tx.can_dlc = 5;     //Sending 5 Bytes of data

    tx.data[0] = 0x02;  //Packet Type ID
    tx.data[1] = (uint8_t)((data.time_since_boot >> 8) & 0xFF);  //Time since boot
    tx.data[2] = (uint8_t)(data.time_since_boot & 0xFF);
    tx.data[3] = (uint8_t)((data.pico_temperature >> 8) & 0xFF);
    tx.data[4] = (uint8_t)(data.pico_temperature & 0xFF);

}

int16_t measure_pico_temperature(void){
    adc_set_temp_sensor_enabled(true); //changes the internal circuitry to turn on the internl analogue temperature sensor and toute the adc channel
    adc_gpio_init(26);
    //Connected to ADC4
    adc_select_input(4);
    
    // 12-bit conversion, assume max value == ADC_VREF == 3.3 V
    uint16_t raw = adc_read();
    float voltage = raw * 3.3f/(1<<12);

    // float measure_pico_temperature = 27f - (voltage - 0.706f)/0.001721f;
    int16_t measure_pico_temperature = 100.0f * (27.0f - (voltage - 0.706f)/0.001721f);

    return measure_pico_temperature;
}