
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/spi.h"
#include "pico/binary_info.h"
#include "hardware/pio.h"
#include "time.h"
#include "../ST7796Library/PicoST7796.h"
#include "image1.h"
#include "image2.h"

PicoST7796 picoST7796;
 
void Button1Clicked(){
    picoST7796.Draw_Bitmap(0,0,myImage1,320,480);
    sleep_ms(5000);
}

void Button2Clicked(){
    picoST7796.Draw_Bitmap(0,0,myImage2,320,480);
    sleep_ms(5000);
}

void Button3Clicked(){
    uint color=picoST7796.RGB(0xFF,0,0);
    //red
        picoST7796.LCD_Clear(0xF800);
        sleep_ms(1000);
            
        picoST7796.LCD_Clear(picoST7796.RGB(0xFF,0,0));
        sleep_ms(1000);

        //GREEN
        picoST7796.LCD_Clear(0x7E0);
        sleep_ms(1000);

        picoST7796.LCD_Clear(picoST7796.RGB(0,0xFF,0));
        sleep_ms(1000);

        //BLUE
        picoST7796.LCD_Clear(0x1F);
        sleep_ms(1000);

        picoST7796.LCD_Clear(picoST7796.RGB(0,0,0));
        sleep_ms(1000);

        //Vertical lines
        for (int i=1; i<320; i++)
        {
            if(i>106){
                color=picoST7796.RGB(0,0xFF,0);
            } 
            if(i>212){
                color=picoST7796.RGB(0,0,0xFF);
            } 

            picoST7796.Draw_LineV(i,0,480,color);
        }

        sleep_ms(1000);
        picoST7796.LCD_Clear(picoST7796.RGB(0,0,0));
        sleep_ms(500);

        //Horizontal lines
        color=picoST7796.RGB(0xFF,0,0);
        for (int i=1; i<480; i++)
        {
            if(i>160){
                color=picoST7796.RGB(0,0xFF,0);
            } 
            if(i>320){
                color=picoST7796.RGB(0,0,0xFF);
            } 

            picoST7796.Draw_LineH(1,i,320,color);
        }

        sleep_ms(1000);
        picoST7796.LCD_Clear(picoST7796.RGB(0,0,0));
        sleep_ms(500);



        //Oblique lines 1
        for (int i = 1; i<320; i++){
            picoST7796.Draw_Line(160,1,i,479, color);
        }      
        sleep_ms(1000);
        picoST7796.LCD_Clear(picoST7796.RGB(0,0,0));
        sleep_ms(500);

        //Oblique lines 2
        for (int i = 1; i<320; i++){
            picoST7796.Draw_Line(160,480,i,1, color);
        }      
        sleep_ms(1000);
        picoST7796.LCD_Clear(picoST7796.RGB(0,0,0));
        sleep_ms(500);

        //Oblique lines 3
        for (int i = 1; i<480; i++){
            picoST7796.Draw_Line(0,240,320,i, color);
        } 
        sleep_ms(1000);
        picoST7796.LCD_Clear(picoST7796.RGB(0,0,0));
        sleep_ms(500);

        //Oblique lines 4
        for (int i = 1; i<480; i++){
            picoST7796.Draw_Line(320,240,0,i, color);
        }        
        sleep_ms(1000);
        picoST7796.LCD_Clear(picoST7796.RGB(0,0,0));
        sleep_ms(500);

        //Empty rectangles
        color=picoST7796.RGB(0xFF,0,0);
        for (int i = 0; i<320; i+=10){

            if(i>106){
                color=picoST7796.RGB(0,0xFF,0);
            } 
            if(i>212){
                color=picoST7796.RGB(0,0,0xFF);
            }        

            for(int j=0; j<480; j+=10){
                picoST7796.Draw_Rect(i+1, j+1, 8, 8 , color); 
            }
        }

        sleep_ms(1000);
        picoST7796.LCD_Clear(picoST7796.RGB(0,0,0));
        sleep_ms(500);

        //Filled rectangles
        color=picoST7796.RGB(0xFF,0,0);
        for (int i = 0; i<320; i+=10){

            if(i>106){
                color=picoST7796.RGB(0,0xFF,0);
            } 
            if(i>212){
                color=picoST7796.RGB(0,0,0xFF);
            }        

            for(int j=0; j<480; j+=10){
                picoST7796.Draw_RectF(i+1, j+1, 8, 8 , color); 
            }
        }
        sleep_ms(1000);
        picoST7796.LCD_Clear(picoST7796.RGB(0,0,0));
        sleep_ms(500);

        //Empty triangles
        color=picoST7796.RGB(0xFF,0,0);
        for (int i = 150; i>0; i-=10){
            if(i<100){
                color=picoST7796.RGB(0,0xFF,0);
            }
            if(i<50){
               color=picoST7796.RGB(0,0,0xFF);
            } 
            picoST7796.Draw_Triangle(160,1+i, 320-i, 480-i, 1+i, 480-i , color);        
        }
        sleep_ms(1000);
        picoST7796.LCD_Clear(picoST7796.RGB(0,0,0));
        sleep_ms(500);

        //Filled triangles
        color=picoST7796.RGB(0xFF,0,0);
        for (int i = 150; i>0; i-=10){
            if(i<100){
                color=picoST7796.RGB(0,0xFF,0);
            }
            if(i<50){
                color=picoST7796.RGB(0,0,0xFF);
            } 

            picoST7796.Draw_TriangleF(160,1+i, 320-i, 480-i, 1+i, 480-i , color);        
        }

        sleep_ms(1000);
        picoST7796.LCD_Clear(picoST7796.RGB(0,0,0));
        sleep_ms(500);

        //Empty Circles
        color=picoST7796.RGB(0xFF,0,0);
        for (int i = 4; i<320; i+=10){

            if(i>106){
                 color=picoST7796.RGB(0,0xFF,0);
            } 
            if(i>212){
                 color=picoST7796.RGB(0,0,0xFF);
            }          

            for(int j=4; j<480; j+=10){
                picoST7796.Draw_Circle(i+1, j+1,4,color);
            }
        }

        sleep_ms(1000);
        picoST7796.LCD_Clear(picoST7796.RGB(0,0,0));
        sleep_ms(500);

        color=picoST7796.RGB(0xFF,0,0);
        for (int i = 4; i<320; i+=10){

            if(i>106){
                color=picoST7796.RGB(0,0xFF,0);
            } 
            if(i>212){
                 color=picoST7796.RGB(0,0,0xFF);
            }          

            for(int j=4; j<480; j+=10){
                picoST7796.Draw_CircleF(i+1, j+1,4,color);
            }
        }

        sleep_ms(1000);
        picoST7796.LCD_Clear(picoST7796.RGB(0xFF,0xFF,0xFF));
        sleep_ms(500);

        //Text         
        picoST7796.Draw_Text( 1,1, "Hello world!", picoST7796.RGB(0xFF,0,0), picoST7796.RGB(0,0xFF,0),4,4);
        
        picoST7796.Draw_Text( 1,50, "This is a line", picoST7796.RGB(0xFF,0,0), picoST7796.RGB(0,0xFF,0),2,2);
        picoST7796.Draw_Text( 1,70, "This is a line", picoST7796.RGB(0,0,0), picoST7796.RGB(0xFF,0xFF,0xFF),2,2);
        picoST7796.Draw_Text( 1,90, "This is a line", picoST7796.RGB(0xFF,0xFF,0xFF), picoST7796.RGB(0,0,0),2,2);
        picoST7796.Draw_Text( 1,110, "This is a line", picoST7796.RGB(0xFF,0,0), picoST7796.RGB(0xFF,0xFF,0xFF),2,2);
        picoST7796.Draw_Text( 1,130, "This is a line", picoST7796.RGB(0,0xFF,0), picoST7796.RGB(0xFF,0xFF,0xFF),2,2);
        picoST7796.Draw_Text( 1,150, "This is a line", picoST7796.RGB(0,0,0xFF), picoST7796.RGB(0xFF,0xFF,0xFF),2,2);
       

        sleep_ms(2000);
        picoST7796.LCD_Clear(picoST7796.RGB(0,0,0));
        sleep_ms(500);
}

int main() {

    stdio_init_all();      
    
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    gpio_put(PICO_DEFAULT_LED_PIN, 1);
    sleep_ms(5000);
    gpio_put(PICO_DEFAULT_LED_PIN, 0);
    sleep_ms(1000); 

    picoST7796.Lcd_Init();
    
    
    picoST7796.LCD_TP_Init();
    
    picoST7796.LCD_Clear(picoST7796.RGB(0xFF,0xFF,0xFF));

    picoST7796.LCD_AddButton(200,100,100,70,"Image1", *Button1Clicked); 
    picoST7796.LCD_AddButton(200,200,100,70,"Image2", *Button2Clicked);
    picoST7796.LCD_AddButton(100,300,200,70,"Start test", *Button3Clicked);

 while(1){
    
    picoST7796.WaitForClick();   
    picoST7796.LCD_Clear(picoST7796.RGB(0xFF,0xFF,0xFF));
    picoST7796.RedrawButtons();
    
          
  }
}