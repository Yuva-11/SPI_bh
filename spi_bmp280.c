// bmp280_spi.c
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

#define SPI_PORT spi0
#define PIN_CS   17
#define PIN_MISO 16
#define PIN_SCK  18
#define PIN_MOSI 19

#define BMP280_CHIP_ID_REG 0xD0
#define BMP280_CHIP_ID     0x58

uint8_t bmp280_read_reg(uint8_t reg) {
    uint8_t tx[] = { reg | 0x80, 0x00 }; 
    uint8_t rx[2];

    gpio_put(PIN_CS, 0);
    spi_write_read_blocking(SPI_PORT, tx, rx, 2);
    gpio_put(PIN_CS, 1);

    return rx[1];
}

int main() {
    stdio_init_all();

    spi_init(SPI_PORT, 1 * 1000 * 1000); // 1 MHz
    gpio_set_function(PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(PIN_SCK, GPIO_FUNC_SPI);
    gpio_set_function(PIN_MOSI, GPIO_FUNC_SPI);

    gpio_init(PIN_CS);
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1);

    sleep_ms(500);

    uint8_t chip_id = bmp280_read_reg(BMP280_CHIP_ID_REG);
    if (chip_id == BMP280_CHIP_ID) {
        printf("BMP280 detected! Chip ID: 0x%X\n", chip_id);
    } else {
        printf("BMP280 not detected. Read ID: 0x%X\n", chip_id);
    }

    // You can add full temperature/pressure reading logic later
    while (1) {
        sleep_ms(1000);
    }
}
