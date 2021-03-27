
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/spi.h"
#include "pico/binary_info.h"


const uint DATA_0 = 0;
const uint DATA_1 = 1;
const uint DATA_2 = 2;
const uint DATA_3 = 3;
const uint DATA_4 = 4;
const uint DATA_5 = 5;
const uint DATA_6 = 6;
const uint DATA_7 = 7;

const uint LCR_CS = 8;
const uint LCR_WR = 9;
const uint LCR_RST = 10;
const uint LCR_RS = 11;
const uint LCR_RD = 12;

int main() {

    stdio_init_all();

    gpio_init(DATA_0);
    gpio_set_dir(DATA_0, GPIO_OUT);
    
    gpio_init(DATA_1);
    gpio_set_dir(DATA_1, GPIO_OUT);

    gpio_init(DATA_2);
    gpio_set_dir(DATA_2, GPIO_OUT);
    
    gpio_init(DATA_3);
    gpio_set_dir(DATA_3, GPIO_OUT);
    
    gpio_init(DATA_4);
    gpio_set_dir(DATA_4, GPIO_OUT);
    
    gpio_init(DATA_5);
    gpio_set_dir(DATA_5, GPIO_OUT);
    
    gpio_init(DATA_6);
    gpio_set_dir(DATA_6, GPIO_OUT);
    
    gpio_init(DATA_7);
    gpio_set_dir(DATA_7, GPIO_OUT);
    
    gpio_init(LCR_CS);
    gpio_set_dir(LCR_CS, GPIO_OUT);

    gpio_init(LCR_WR);
    gpio_set_dir(LCR_WR, GPIO_OUT);

    gpio_init(LCR_RST);
    gpio_set_dir(LCR_RST, GPIO_OUT);

    gpio_init(LCR_RS);
    gpio_set_dir(LCR_RS, GPIO_OUT);

    gpio_init(LCR_RD);
    gpio_set_dir(LCR_RD, GPIO_OUT);
    
    
    while (true) {
        uint mask=0b00000000000000001111111111111;
        uint data=0b00000000000000001111111111111;

        gpio_put_masked (mask,data); 
        sleep_ms(500);
        
        data=0b00000000000000000000000000000;
        gpio_put_masked (mask,data); 
        sleep_ms(500);

        data=0b00000000000000000000000000001;
        for (int i=0; i<13; i++)
        {
            gpio_put_masked (mask,data); 
            sleep_ms(500);
            data= data<<1;
        }
    }
}