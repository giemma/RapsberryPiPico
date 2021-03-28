#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/spi.h"
#include "pico/binary_info.h"
#include "hardware/pio.h"
#include "PicoST7796.h"


#ifdef USE_PIO 
#include "pioST7796.pio.h"
#endif


const uint LED_0 = 0;
const uint LED_1 = 1;
const uint LED_2 = 2;
const uint LED_3 = 3;
const uint LED_4 = 4;
const uint LED_5 = 5;
const uint LED_6 = 6;
const uint LED_7 = 7;


#define LCD_CS    8 //command 0 data 1 selection D/CX
#define LCD_WR     9 //write
#define LCD_REST  10 //reset
#define LCD_RS    11 //chip selection CSX
#define LCD_RD   12 //read

  PicoST7796::PicoST7796()
  {
   
  }


  void PicoST7796::Lcd_Writ_Bus(uint command)
  {    
      gpio_put(LCD_WR,1);
      gpio_put(LCD_CS,0);
      
      #ifdef USE_PIO 
      pio_sm_put_blocking(pio, sm, command);
      #else
      gpio_put_masked (0b00000000000000000000011111111,command);
      #endif

      gpio_put(LCD_WR,0);
      gpio_put(LCD_CS,1); 
  }

  void PicoST7796::Lcd_Write_Com(uint command)
  {
      gpio_put(LCD_RS, 0); 
      Lcd_Writ_Bus(command);
  }

  void PicoST7796::Lcd_Write_Data(uint data)
  {
      gpio_put(LCD_RS, 1); 
      Lcd_Writ_Bus(data);
  }

  void PicoST7796::Lcd_Write_Com_Data(unsigned int com,unsigned int dat)
  {
    Lcd_Write_Com(com);
    Lcd_Write_Data(dat);
  }

  void PicoST7796::Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
  {
      Lcd_Write_Com(0x2a);
      Lcd_Write_Data(x1>>8);
      Lcd_Write_Data(x1);
      Lcd_Write_Data(x2>>8);
      Lcd_Write_Data(x2);
      Lcd_Write_Com(0x2b);
      Lcd_Write_Data(y1>>8);
      Lcd_Write_Data(y1);
      Lcd_Write_Data(y2>>8);
      Lcd_Write_Data(y2);
      Lcd_Write_Com(0x2c); 							 
  }

  void PicoST7796::Lcd_Init(void)
  {
    #ifdef USE_PIO 
      offset = pio_add_program(pio, &pioST7796_program);
      sm = pio_claim_unused_sm(pio, true);
      pioST7796_program_init(pio, sm, offset, 
          LED_0, 
          LED_1, 
          LED_2, 
          LED_3,
          LED_4,
          LED_5,
          LED_6,
          LED_7
      );
    #else
      gpio_init(LED_0);
      gpio_set_dir(LED_0, GPIO_OUT);
      gpio_init(LED_1);
      gpio_set_dir(LED_1, GPIO_OUT);
      gpio_init(LED_2);
      gpio_set_dir(LED_2, GPIO_OUT);
      gpio_init(LED_3);
      gpio_set_dir(LED_3, GPIO_OUT);
      gpio_init(LED_4);
      gpio_set_dir(LED_4, GPIO_OUT);
      gpio_init(LED_5);
      gpio_set_dir(LED_5, GPIO_OUT);
      gpio_init(LED_6);
      gpio_set_dir(LED_6, GPIO_OUT);
      gpio_init(LED_7);
      gpio_set_dir(LED_7, GPIO_OUT);
    #endif


      gpio_init(LCD_RS);
      gpio_set_dir(LCD_RS, GPIO_OUT);

      gpio_init(LCD_WR);
      gpio_set_dir(LCD_WR, GPIO_OUT);

      gpio_init(LCD_CS);
      gpio_set_dir(LCD_CS, GPIO_OUT);

      gpio_init(LCD_REST);
      gpio_set_dir(LCD_REST, GPIO_OUT);

      gpio_init(LCD_RD);
      gpio_set_dir(LCD_RD, GPIO_OUT);

      gpio_put(LCD_RS,1); //command 0 data 1 selection D/CX
      gpio_put(LCD_WR,1); //write
      gpio_put(LCD_CS,1); //chip selection CSX
      gpio_put(LCD_REST,1); //reset
      gpio_put(LCD_RD,1); //read

      gpio_put(LCD_REST,1);
      sleep_ms(5); 
      gpio_put(LCD_REST,0);
      sleep_ms(15);
      gpio_put(LCD_REST,1);
      sleep_ms(15);

      gpio_put(LCD_CS,1);
      gpio_put(LCD_WR,1);
      gpio_put(LCD_CS,0);  //CS

      Lcd_Write_Com(0xF0);
      Lcd_Write_Data(0xC3);
      Lcd_Write_Com(0xF0);
      Lcd_Write_Data(0x96);
      Lcd_Write_Com(0x36);
      Lcd_Write_Data(0x68);  
      Lcd_Write_Com(0x3A);
      Lcd_Write_Data(0x05);  
      Lcd_Write_Com(0xB0);
      Lcd_Write_Data(0x80);  
      Lcd_Write_Com(0xB6);
      Lcd_Write_Data(0x20);
      Lcd_Write_Data(0x02);  
      Lcd_Write_Com(0xB5);
      Lcd_Write_Data(0x02);
      Lcd_Write_Data(0x03);
      Lcd_Write_Data(0x00);
      Lcd_Write_Data(0x04);
      Lcd_Write_Com(0xB1);
      Lcd_Write_Data(0x80);  
      Lcd_Write_Data(0x10);  
      Lcd_Write_Com(0xB4);
      Lcd_Write_Data(0x00);
      Lcd_Write_Com(0xB7);
      Lcd_Write_Data(0xC6);
      Lcd_Write_Com(0xC5);
      Lcd_Write_Data(0x24);
      Lcd_Write_Com(0xE4);
      Lcd_Write_Data(0x31);
      Lcd_Write_Com(0xE8);
      Lcd_Write_Data(0x40);
      Lcd_Write_Data(0x8A);
      Lcd_Write_Data(0x00);
      Lcd_Write_Data(0x00);
      Lcd_Write_Data(0x29);
      Lcd_Write_Data(0x19);
      Lcd_Write_Data(0xA5);
      Lcd_Write_Data(0x33);
      Lcd_Write_Com(0xC2);
      Lcd_Write_Com(0xA7);

      Lcd_Write_Com(0xE0);
      Lcd_Write_Data(0xF0);
      Lcd_Write_Data(0x09);
      Lcd_Write_Data(0x13);
      Lcd_Write_Data(0x12);
      Lcd_Write_Data(0x12);
      Lcd_Write_Data(0x2B);
      Lcd_Write_Data(0x3C);
      Lcd_Write_Data(0x44);
      Lcd_Write_Data(0x4B);
      Lcd_Write_Data(0x1B);
      Lcd_Write_Data(0x18);
      Lcd_Write_Data(0x17);
      Lcd_Write_Data(0x1D);
      Lcd_Write_Data(0x21);

      Lcd_Write_Com(0XE1);
      Lcd_Write_Data(0xF0);
      Lcd_Write_Data(0x09);
      Lcd_Write_Data(0x13);
      Lcd_Write_Data(0x0C);
      Lcd_Write_Data(0x0D);
      Lcd_Write_Data(0x27);
      Lcd_Write_Data(0x3B);
      Lcd_Write_Data(0x44);
      Lcd_Write_Data(0x4D);
      Lcd_Write_Data(0x0B);
      Lcd_Write_Data(0x17);
      Lcd_Write_Data(0x17);
      Lcd_Write_Data(0x1D);
      Lcd_Write_Data(0x21);

      Lcd_Write_Com(0X36);
      Lcd_Write_Data(0x08);
      Lcd_Write_Com(0xF0);
      Lcd_Write_Data(0xC3);
      Lcd_Write_Com(0xF0);
      Lcd_Write_Data(0x69);
      Lcd_Write_Com(0X13);
      Lcd_Write_Com(0X11);
      Lcd_Write_Com(0X29);
  }

  void PicoST7796::DrawPixel(unsigned int x, unsigned int y, unsigned int c)
  {    
      Lcd_Write_Com(0x02c); //write_memory_start
      gpio_put(LCD_RS,1);
      gpio_put(LCD_CS,0);
      Address_set(x,y,x+1,y+1);

      Lcd_Write_Data(c);
      Lcd_Write_Data(c>>8);

      gpio_put(LCD_CS,1);   
  }

  void PicoST7796::H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c)                   
  {	
    unsigned int i,j;
    Lcd_Write_Com(0x02c); //write_memory_start
    gpio_put(LCD_RS,1);
    gpio_put(LCD_CS,0);
    l=l+x;
    Address_set(x,y,l,y);
    j=l*2;

    for(i=1;i<=j;i++)
    {
      Lcd_Write_Data(c);
      Lcd_Write_Data(c>>8);
    }
    gpio_put(LCD_CS,1);   
  }

  void PicoST7796::V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c)                   
  {	
    unsigned int i,j;
    Lcd_Write_Com(0x02c); //write_memory_start
    gpio_put(LCD_RS,1);
    gpio_put(LCD_CS,0);
    l=l+y;
    Address_set(x,y,x,l);
    j=l*2;
    for(i=1;i<=j;i++)
    { 
      Lcd_Write_Data(c);
      Lcd_Write_Data(c>>8);
    }
    gpio_put(LCD_CS,1);   
  }

  void PicoST7796::DrawEmptyCircle(unsigned int x, unsigned int y, unsigned int radius, unsigned int c)
  {
      int16_t f = 1 - radius;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * radius;
    int16_t x1= 0;
    int16_t y1= radius;

    DrawPixel(x, y+radius,c);
    DrawPixel(x, y-radius,c);
    DrawPixel(x+radius, y,c);
    DrawPixel(x-radius, y,c);

    while (x1<y1) 
    {
        if (f >= 0) 
      {
            y1--;
            ddF_y += 2;
            f += ddF_y;
        }
        x1++;
        ddF_x += 2;
        f += ddF_x;
    
      DrawPixel(x + x1, y + y1,c);
        DrawPixel(x - x1, y + y1,c);
      DrawPixel(x + x1, y - y1,c);
      DrawPixel(x - x1, y - y1,c);
      DrawPixel(x + y1, y + x1,c);
      DrawPixel(x - y1, y + x1,c);
      DrawPixel(x + y1, y - x1,c);
      DrawPixel(x - y1, y - x1,c);
    }
  }

  void PicoST7796::DrawFilledCircle(unsigned int x0, unsigned int y0, unsigned int radius, unsigned int c)
  {
      if(x0-radius <0)
          return;

      if(y0-radius <0)
          return;

      V_line(x0,y0-radius,2*radius+1,c);
      
      int16_t f     = 1 - radius;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * radius;
    int16_t x     = 0;
    int16_t y     = radius;

    while (x<y) 
    {
        if (f >= 0) 
      {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        V_line(x0+x, y0-y, 2*y+1,c);
          V_line(x0+y, y0-x, 2*x+1,c);

          V_line(x0-x, y0-y, 2*y+1,c);
          V_line(x0-y, y0-x, 2*x+1,c);   
      }
  }

  void PicoST7796::Rect(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int c)
  {
    H_line(x  , y  , w, c);
    H_line(x  , y+h, w, c);
    V_line(x  , y  , h, c);
    V_line(x+w, y  , h, c);
  }

  void PicoST7796::Rectf(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int c)
  {
    unsigned int i;
    for(i=0;i<h;i++)
    {
      H_line(x  , y  , w, c);
      H_line(x  , y+i, w, c);
    }
  }

  int PicoST7796::RGB(int r,int g,int b)
  {
      return r << 16 | g << 8 | b;
  }

  void PicoST7796::startWrite()
  {
    Lcd_Write_Com(0x02c); //write_memory_start
  }

  void PicoST7796::endWrite(){
    gpio_put(LCD_CS,0); 
  }

  void PicoST7796::LCD_Clear(unsigned int j)                   
  {	
    unsigned int i,m;
    Address_set(0,0,320,480);
    Lcd_Write_Com(0x02c); //write_memory_start
    gpio_put(LCD_RS,1);
    gpio_put(LCD_CS,0);


    for(i=0;i<320;i++)
      for(m=0;m<480;m++)
      {
        Lcd_Write_Data(j>>8);
        Lcd_Write_Data(j);
      }
    gpio_put(LCD_CS,0);   
  }



