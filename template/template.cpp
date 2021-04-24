
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/spi.h"
#include "pico/binary_info.h"
#include "math.h"



int main() {

    stdio_init_all();
    
    gpio_init(25);
    gpio_set_dir(25, GPIO_OUT);
    
    sleep_ms(5000);

    printf("Started\n");
   
	while(1){  
    printf("ON\n");
	  gpio_put(25,1); 

    sleep_ms(1000);

    printf("OFF\n");
	  gpio_put(25,0); 
	  sleep_ms(1000);      
  }
   
}

