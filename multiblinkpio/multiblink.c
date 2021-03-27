
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/spi.h"
#include "pico/binary_info.h"
#include "hardware/pio.h"
#include "hello.pio.h"

const uint LED_0 = 0;
const uint LED_1 = 1;
const uint LED_2 = 2;
const uint LED_3 = 3;
const uint LED_4 = 4;
const uint LED_5 = 5;
const uint LED_6 = 6;
const uint LED_7 = 7;

int main() {

    stdio_init_all();
    

    
    PIO pio = pio0;
    uint offset = pio_add_program(pio, &hello_program);

    uint sm = pio_claim_unused_sm(pio, true);
    hello_program_init(pio, sm, offset, 
        LED_0, 
        LED_1, 
        LED_2, 
        LED_3,
        LED_4,
        LED_5,
        LED_6,
        LED_7
    );

    while (true) {

        pio_sm_put_blocking(pio, sm, 0b00000000);
        sleep_ms(500);

        pio_sm_put_blocking(pio, sm, 0xFFFF);
        sleep_ms(500);


        pio_sm_put_blocking(pio, sm, 1);
        sleep_ms(500);

        pio_sm_put_blocking(pio, sm, 0b00000001);
        sleep_ms(500);
        
        pio_sm_put_blocking(pio, sm, 0b00000010);
        sleep_ms(500);

        pio_sm_put_blocking(pio, sm, 0b00000100);
        sleep_ms(500);

        pio_sm_put_blocking(pio, sm, 0b00001000);
        sleep_ms(500);

        pio_sm_put_blocking(pio, sm, 0b00010000);
        sleep_ms(500);

        pio_sm_put_blocking(pio, sm, 0b00100000);
        sleep_ms(500);

        pio_sm_put_blocking(pio, sm, 0b01000000);
        sleep_ms(500);

        pio_sm_put_blocking(pio, sm, 0b10000000);
        sleep_ms(500);

        pio_sm_put_blocking(pio, sm, 0b10000000);
        sleep_ms(500);
    
    }
}