
#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/spi.h"
#include "pico/binary_info.h"

#define LCD_RS     8 //command 0 data 1 selection D/CX
#define LCD_WR     9 //write
#define LCD_REST  10 //reset
#define LCD_CS    11 //chip selection CSX
#define LCD_RD   12 //read
   
void Lcd_Writ_Bus(uint command)
{
    gpio_put(LCD_WR,1);
    gpio_put(LCD_CS,0);
    gpio_put_masked (0b00000000000000000000011111111,command);
    gpio_put(LCD_WR,0);
    gpio_put(LCD_CS,1); 
}

void Lcd_Write_Com(uint command)
{
    gpio_put(LCD_RS, 0); 
    Lcd_Writ_Bus(command);
}

void Lcd_Write_Data(uint data)
{
    gpio_put(LCD_RS, 1); 
    Lcd_Writ_Bus(data);
}

void Lcd_Write_Com_Data(unsigned int com,unsigned int dat)
{
  Lcd_Write_Com(com);
  Lcd_Write_Data(dat);
}

void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
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

void Lcd_Init(void)
{
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

void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c)                   
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
    Lcd_Write_Data(c>>8);
    Lcd_Write_Data(c);
  }
  gpio_put(LCD_CS,1);   
}

void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c)                   
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
    Lcd_Write_Data(c>>8);
    Lcd_Write_Data(c);
  }
  gpio_put(LCD_CS,1);   
}

void Rect(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int c)
{
  H_line(x  , y  , w, c);
  H_line(x  , y+h, w, c);
  V_line(x  , y  , h, c);
  V_line(x+w, y  , h, c);
}

void Rectf(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int c)
{
  unsigned int i;
  for(i=0;i<h;i++)
  {
    H_line(x  , y  , w, c);
    H_line(x  , y+i, w, c);
  }
}

int RGB(int r,int g,int b)
{
    return r << 16 | g << 8 | b;
}


void LCD_Clear(unsigned int j)                   
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

int main() {

    stdio_init_all();
    
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    gpio_init(0);
    gpio_set_dir(0, GPIO_OUT);
    gpio_init(1);
    gpio_set_dir(1, GPIO_OUT);
    gpio_init(2);
    gpio_set_dir(2, GPIO_OUT);
    gpio_init(3);
    gpio_set_dir(3, GPIO_OUT);
    gpio_init(4);
    gpio_set_dir(4, GPIO_OUT);
    gpio_init(5);
    gpio_set_dir(5, GPIO_OUT);
    gpio_init(6);
    gpio_set_dir(6, GPIO_OUT);
    gpio_init(7);
    gpio_set_dir(7, GPIO_OUT);

    gpio_put(LED_PIN, 1);
    sleep_ms(250);
    gpio_put(LED_PIN, 0);
    sleep_ms(250);
    gpio_put(LED_PIN, 1);
    sleep_ms(250);
    gpio_put(LED_PIN, 0);
    sleep_ms(250);
    gpio_put(LED_PIN, 1);
    sleep_ms(250);
    gpio_put(LED_PIN, 0);
    sleep_ms(250);

    printf("Inizio tra 5 secondi\n");
    sleep_ms(1000);

    printf("Inizio tra 4 secondi\n");
    sleep_ms(1000);

    printf("Inizio tra 3 secondi\n");
    sleep_ms(1000);

    printf("Inizio tra 2 secondi\n");
    sleep_ms(1000);

    printf("Inizio tra 1 secondi\n");
    sleep_ms(1000);
     
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



    printf("init...\n");
    Lcd_Init();
    printf("OK\n");


gpio_put(0U, 1);
gpio_put(1U, 1);
gpio_put(2U, 1);
gpio_put(3U, 1);
gpio_put(4U, 1);
gpio_put(5U, 1);
gpio_put(6U, 1);
gpio_put(7U, 1);
sleep_ms(1000);
gpio_put(0, 0);
gpio_put(1, 0);
gpio_put(2, 0);
gpio_put(3, 0);
gpio_put(4, 0);
gpio_put(5, 0);
gpio_put(6, 0);
gpio_put(7, 0);
sleep_ms(1000);

  while(1){      
      
      gpio_put(LED_PIN, 1);

      printf("blu\n");
      LCD_Clear(0xf800);
      printf("OK\n");
      sleep_ms(1000);
      
      printf("red\n");
      LCD_Clear(0x07E0);
      printf("OK\n");
      sleep_ms(1000);
      
      printf("green\n");
      LCD_Clear(0x001F);
      printf("OK\n");
      sleep_ms(1000);
      
      printf("blank\n"); 
      LCD_Clear(0x0);
      printf("OK\n");
      sleep_ms(1000);
      
      printf("white\n");
      LCD_Clear(0xFFFFFFFF);
      printf("OK\n");
      sleep_ms(1000);

      printf("rect\n");
      
      int color=RGB(0XF8,0,0);
      for (int i = 0; i<320; i+=10){

      if(i>106){
        color=RGB(0,0X7E,0);
      } 
      if(i>212){
        color=RGB(0,0,0X1F);
      }          

        for(int j=0; j<480; j+=10){
          Rectf(i+1, j+1, 8, 8 , color); 
          //sleep_ms(10);
        }
      }
      
      printf("OK\n");
      sleep_ms(10000);

      gpio_put(LED_PIN, 0);   
      printf("END\n"); 
      
  }
   
}

