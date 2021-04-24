
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/spi.h"
#include "pico/binary_info.h"
#include "math.h"
#include "hardware/pio.h"
#include "blinkPio.pio.h"


int main() {

    stdio_init_all();
    

    PIO pio = pio0;
    uint offset = pio_add_program(pio, &blinkPio_program);

    uint sm = pio_claim_unused_sm(pio, true);

    //pio->sm[0].clkdiv = (uint32_t) (2.5f * (1 << 16)) * 100;
    

    hello_program_init(pio, sm, offset, 0);


    while (true) {
        
        pio_sm_put_blocking(pio, sm, 0b11111111111111111111111111111111);

        sleep_ms(5000);

        pio_sm_put_blocking(pio, sm, 0b00000);
        sleep_ms(5000);
    }
   
   
}

