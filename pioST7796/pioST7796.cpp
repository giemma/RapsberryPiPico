
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/spi.h"
#include "pico/binary_info.h"
#include "hardware/pio.h"
#include "time.h"
#include "../ST7796Library/PicoST7796.h"




PicoST7796 p;
 
int main() {

    stdio_init_all();
    
    
    
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 1);
    sleep_ms(5000);
    gpio_put(PICO_DEFAULT_LED_PIN, 0);
    sleep_ms(1000);

 

    p.Lcd_Init();

    while(1){      
            
        printf("blu\n");
        p.LCD_Clear(0xf800);
        printf("OK\n");
        sleep_ms(1000);

        printf("red\n");
        p.LCD_Clear(0x07E0);
        printf("OK\n");
        sleep_ms(1000);

        printf("green\n");
        p.LCD_Clear(0x001F);
        printf("OK\n");
        sleep_ms(1000);

        printf("blank\n"); 
        p.LCD_Clear(0x0);
        printf("OK\n");
        sleep_ms(1000);

        printf("white\n");
        p.LCD_Clear(0xFFFFFFFF);
        printf("OK\n");
        sleep_ms(1000);

        p.DrawEmptyCircle(100,100,20,0x07E0);
        p.DrawFilledCircle(200,200,20,0x001F);

        sleep_ms(5000);

        printf("rect\n");
      
        clock_t t; t = clock();

        p.LCD_Clear(0xFFFFFFFF);
        int color=0x07E0; //red
        for (int i = 4; i<320; i+=10){

            if(i>106){
                color=0x001F; //green
            } 
            if(i>212){
                color=0xf800; //blu
            }          

            for(int j=4; j<480; j+=10){
            //Rectf(i+1, j+1, 8, 8 , color);
            p.DrawFilledCircle(i+1, j+1,4,color);
            //sleep_ms(10);
            }
        }

        p.LCD_Clear(0xFFFFFFFF);
        color=0x07E0; //red
        for (int i = 0; i<320; i+=10){

             if(i>106){
                color=0x001F; //green
            } 
            if(i>212){
                color=0xf800; //blu
            }         

            for(int j=0; j<480; j+=10){
            p.Rectf(i+1, j+1, 8, 8 , color);
           
            //sleep_ms(10);
            }
        }


        t = clock() - t;
        double elapsed = ((double)t) / CLOCKS_PER_SEC ;

        #ifdef USE_PIO 
        printf("%s(%d) = %d in %f seconds\n", "PIO", "", "rects", elapsed);
        #else
        printf("%s(%d) = %d in %f seconds\n", "WITHOUT PIO", "", "rects", elapsed);
        #endif
        
        
        printf("OK\n");
        sleep_ms(10000);
  
        printf("END\n"); 
      
  }

    /*
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
    */
}