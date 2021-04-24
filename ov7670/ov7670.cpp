
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/spi.h"
#include "pico/binary_info.h"
#include "math.h"

#include <stdio.h>

static int I2C_ADD = 0x50;

int main() {
 
    stdio_init_all();
    

    
    i2c_init(i2c_default,  100000); //100kHZ
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));

    i2c_set_slave_mode(i2c_default,true,I2C_ADD);
    

    uint8_t buffer[1];   

    while(1){
        
        int readResult = i2c_read_blocking(i2c_default, I2C_ADD, buffer, 1, false);
        
        if(readResult>0){
            printf("Lenght= %d. Received: %d\n",readResult, buffer[0]);
        }
       
    }
}

