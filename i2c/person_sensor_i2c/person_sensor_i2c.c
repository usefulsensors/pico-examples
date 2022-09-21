/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"

/* Example code to talk to a MCP9808 ±0.5°C Digital temperature Sensor
   
   This reads and writes to registers on the board. 

   Connections on Raspberry Pi Pico board, other boards may vary.

   GPIO PICO_DEFAULT_I2C_SDA_PIN (On Pico this is GP4 (physical pin 6)) -> SDA on MCP9808 board
   GPIO PICO_DEFAULT_I2C_SCK_PIN (On Pico this is GP5 (physcial pin 7)) -> SCL on MCP9808 board
   Vsys (physical pin 39) -> VDD on MCP9808 board
   GND (physical pin 38)  -> GND on MCP9808 board

*/
//The bus address is determined by the state of pins A0, A1 and A2 on the MCP9808 board
static uint8_t ADDRESS = 0x62;

//hardware registers

const uint8_t REG_MODE = 0x01;
const uint8_t REG_SINGLE_SHOT = 0x02;
const uint8_t REG_ENABLE_ID = 0x03;
const uint8_t REG_LABEL_ID = 0x04;
const uint8_t REG_ENABLE_SMOOTHING = 0x05;

typedef struct {
  float confidence;
  float id_confidence;
  uint8_t x1; 
  uint8_t y1; 
  uint8_t x2; 
  uint8_t y2; 
  int8_t identity;
} inference_results_t;

int main() {
    stdio_init_all();
    //setup_default_uart();

#if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN) || !defined(PICO_DEFAULT_I2C_SCL_PIN)
#warning i2c/mcp9808_i2c example requires a board with I2C pins
    puts("Default I2C pins were not defined");
#else
    printf("Setting up i2c\n");

    // This example will use I2C0 on the default SDA and SCL pins (4, 5 on a Pico)
    i2c_init(i2c_default, 400 * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    // Make the I2C pins available to picotool
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));

    printf("setup done\n");
    
    inference_results_t results;
    
    while (1) {
        // Start reading ambient temperature register for 2 bytes
        i2c_read_blocking(i2c_default, ADDRESS, &results, sizeof(inference_results_t), false);

        printf("conf %f\% bbox [%d %d %d %d], id conf %f\% id %d\n", results.confidence, results.x1, results.y1, results.x2, results.y2, results.id_confidence, results.identity);

        sleep_ms(200);
    }
#endif
}
