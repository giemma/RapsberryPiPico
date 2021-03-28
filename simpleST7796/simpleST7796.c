
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/spi.h"
#include "pico/binary_info.h"
#include "math.h"

#define LCD_CS    8 //chip selection CSX
#define LCD_WR     9 //write
#define LCR_RST  10 //reset
#define LCD_RS     11 //command 0 data 1 selection D/CX
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

void Lcd_StartWrite()
{
    Lcd_Write_Com(0x02c); //Memory write
    gpio_put(LCD_RS,1);
    gpio_put(LCD_CS,0);
}

void Lcd_EndWrite()
{
    gpio_put(LCD_CS,1); 
}

void Write_Pixel(unsigned int x,unsigned int y,unsigned int color)
{
    Lcd_Write_Com(0x2a); //Column address set
    Lcd_Write_Data(x>>8);
    Lcd_Write_Data(x);
    
    Lcd_Write_Com(0x2b); //Row address set
    Lcd_Write_Data(y>>8);
    Lcd_Write_Data(y);
    
    Lcd_Write_Com(0x2c); //Memory write	

    Lcd_Write_Data(color>>8);
    Lcd_Write_Data(color);				 
}

void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2)
{
    Lcd_Write_Com(0x2a); //Column address set
    Lcd_Write_Data(x1>>8);
    Lcd_Write_Data(x1);
    Lcd_Write_Data(x2>>8);
    Lcd_Write_Data(x2);
    Lcd_Write_Com(0x2b); //Row address set
    Lcd_Write_Data(y1>>8);
    Lcd_Write_Data(y1);
    Lcd_Write_Data(y2>>8);
    Lcd_Write_Data(y2);
    Lcd_Write_Com(0x2c); //Memory write					 
}

void Lcd_Init(void)
{
    gpio_put(LCR_RST,1);
    sleep_ms(5); 
    gpio_put(LCR_RST,0);
    sleep_ms(15);
    gpio_put(LCR_RST,1);
    sleep_ms(15);

    gpio_put(LCD_CS,1);
    gpio_put(LCD_WR,1);
    gpio_put(LCD_CS,0);  //CS

    Lcd_Write_Com(0xF0); //NO OPERATION
    Lcd_Write_Data(0xC3);
    Lcd_Write_Com(0xF0); //NO OPERATION
    Lcd_Write_Data(0x96);
    Lcd_Write_Com(0x36); // Memory data access control
    Lcd_Write_Data(0x68);  
    Lcd_Write_Com(0x3A); //nterface pixel format
    Lcd_Write_Data(0x05);  
    Lcd_Write_Com(0xB0); //Interface Mode Control
    Lcd_Write_Data(0x80);  
    Lcd_Write_Com(0xB6); //Display Function Control 
    Lcd_Write_Data(0x20);
    Lcd_Write_Data(0x02);  
    Lcd_Write_Com(0xB5); //Blanking Porch Control
    Lcd_Write_Data(0x02);
    Lcd_Write_Data(0x03);
    Lcd_Write_Data(0x00);
    Lcd_Write_Data(0x04);
    Lcd_Write_Com(0xB1); //Frame Rate Control （In Normal Mode/Full Colors）
    Lcd_Write_Data(0x80);  
    Lcd_Write_Data(0x10);  
    Lcd_Write_Com(0xB4); //Display Inversion Control 
    Lcd_Write_Data(0x00);
    Lcd_Write_Com(0xB7); //Entry Mode Set
    Lcd_Write_Data(0xC6);
    Lcd_Write_Com(0xC5); //Vcom Control 
    Lcd_Write_Data(0x24);
    Lcd_Write_Com(0xE4); //?
    Lcd_Write_Data(0x31);
    Lcd_Write_Com(0xE8); //Display Output 
    Lcd_Write_Data(0x40);
    Lcd_Write_Data(0x8A);
    Lcd_Write_Data(0x00);
    Lcd_Write_Data(0x00);
    Lcd_Write_Data(0x29);
    Lcd_Write_Data(0x19);
    Lcd_Write_Data(0xA5);
    Lcd_Write_Data(0x33);
    Lcd_Write_Com(0xC2); //Power Control 3 
    Lcd_Write_Com(0xA7);

    Lcd_Write_Com(0xE0); //Positive Gamma Control 
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

    Lcd_Write_Com(0XE1); //Negative Gamma Control 
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

    Lcd_Write_Com(0X36); //Memory data access control
    Lcd_Write_Data(0x08);
    Lcd_Write_Com(0xF0); //NO OPERATION
    Lcd_Write_Data(0xC3);
    Lcd_Write_Com(0xF0); //NO OPERATION
    Lcd_Write_Data(0x69);
    Lcd_Write_Com(0X13); //Partial off
    Lcd_Write_Com(0X11); //Sleep out
    Lcd_Write_Com(0X29); //Display ON
}


void H_lineFast(unsigned int x0, unsigned int y0, unsigned int lenght, unsigned int c)                   
{
  unsigned int i,j;
  lenght=lenght+x0;
  Address_set(x0,y0,lenght,y0);
  j=lenght*2;
  for(i=1;i<=j;i++)
  {
    Lcd_Write_Data(c>>8);
    Lcd_Write_Data(c);
  }
}

void V_lineFast(unsigned int x0, unsigned int y0, unsigned int lenght, unsigned int c)                   
{
  unsigned int i,j;
  lenght=lenght+y0;
  Address_set(x0,y0,x0,lenght);
  j=lenght*2;
  for(i=1;i<=j;i++)
  { 
    Lcd_Write_Data(c>>8);
    Lcd_Write_Data(c);
  }
}

void H_line(unsigned int x0, unsigned int y0, unsigned int lenght, unsigned int c)                   
{	
  Lcd_StartWrite();
  H_lineFast(x0,y0,lenght,c);
  Lcd_EndWrite();
}

void V_line(unsigned int x0, unsigned int y0, unsigned int lenght, unsigned int c)                   
{
  Lcd_StartWrite();
  V_lineFast(x0,y0,lenght,c);
  Lcd_EndWrite();  
}




#define SWAP(T, a, b) do { T tmp = a; a = b; b = tmp; } while (0)

void Draw_Line(int x0,  int y0, int x1,  int y1,  int color)
{
  Lcd_StartWrite();

  int steep = abs(y1 - y0) > abs(x1 - x0);

  if (steep) {
    SWAP(int, x0, y0);
    SWAP(int, x1, y1);
  }

  if (x0 > x1) {
    SWAP(int, x0, x1);
    SWAP(int, y0, y1);
  }

  int16_t dx, dy;
  dx = x1 - x0;
  dy = abs(y1 - y0);

  int16_t err = dx / 2;
  int16_t ystep;

  if (y0 < y1) {
    ystep = 1;
  } else {
    ystep = -1;
  }

  for (; x0 <= x1; x0++) {
    if (steep) {
      Write_Pixel(y0, x0, color);
    } else {
      Write_Pixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }

  Lcd_EndWrite(); 
}

void Rect(unsigned int x0,unsigned int y0,unsigned int width,unsigned int height,unsigned int color)
{
  V_line(x0  , y0  , width, color);
  V_line(x0  , y0+height, width, color);
  H_line(x0  , y0  , height, color);
  H_line(x0+width, y0  , height, color);
}

void Rectf(unsigned int x0,unsigned int y0,unsigned int width,unsigned int height,unsigned int color)
{
  unsigned int i;
  for(i=0;i<height;i++)
  {
    H_line(x0  , y0  , width, color);
    H_line(x0  , y0+i, width, color);
  }
}

void Draw_Triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
                                int16_t x2, int16_t y2, uint16_t color) {
  Draw_Line(x0, y0, x1, y1, color);
  Draw_Line(x1, y1, x2, y2, color);
  Draw_Line(x2, y2, x0, y0, color);
}

void Draw_TriangleF(int16_t x0, int16_t y0, int16_t x1, int16_t y1,int16_t x2, int16_t y2, uint16_t color)
{
  int16_t a, b, y, last;

  if (y0 > y1) {
    SWAP(int16_t,y0, y1);
    SWAP(int16_t,x0, x1);
  }
  if (y1 > y2) {
    SWAP(int16_t,y2, y1);
    SWAP(int16_t,x2, x1);
  }
  if (y0 > y1) {
    SWAP(int16_t,y0, y1);
    SWAP(int16_t,x0, x1);
  }

  Lcd_StartWrite();
  if (y0 == y2) {
    a = b = x0;
    if (x1 < a)
      a = x1;
    else if (x1 > b)
      b = x1;
    if (x2 < a)
      a = x2;
    else if (x2 > b)
      b = x2;
    H_lineFast(a, y0, b - a + 1, color);
    Lcd_EndWrite();
    return;
  }

  int16_t dx01 = x1 - x0, dy01 = y1 - y0, dx02 = x2 - x0, dy02 = y2 - y0,
          dx12 = x2 - x1, dy12 = y2 - y1;
  int32_t sa = 0, sb = 0;

  if (y1 == y2)
    last = y1;
  else
    last = y1 - 1;

  for (y = y0; y <= last; y++) {
    a = x0 + sa / dy01;
    b = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;

    if (a > b)
      SWAP(int16_t,a, b);
    H_lineFast(a, y, b - a + 1, color);
  }

  sa = (int32_t)dx12 * (y - y1);
  sb = (int32_t)dx02 * (y - y0);
  for (; y <= y2; y++) {
    a = x1 + sa / dy12;
    b = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;

    if (a > b)
      SWAP(int16_t,a, b);
    H_lineFast(a, y, b - a + 1, color);
  }
  Lcd_EndWrite();
}

void LCD_Clear(unsigned int j)                   
{
  unsigned int i,m;
  Address_set(0,0,320,480);
  
  Lcd_StartWrite();
  
  for(i=0;i<320;i++)
    for(m=0;m<480;m++)
    {
      Lcd_Write_Data(j);
      Lcd_Write_Data(j>>8);
    }
 
  Lcd_EndWrite();
}

 int RGB(int r,int g,int b)
  {
      return r << 11 | g << 5 | b;
  }

int main() {

    stdio_init_all();
    

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

    gpio_init(LCR_RST);
    gpio_set_dir(LCR_RST, GPIO_OUT);

    gpio_init(LCD_RD);
    gpio_set_dir(LCD_RD, GPIO_OUT);

    gpio_put(LCD_RS,1); 
    gpio_put(LCD_WR,1); 
    gpio_put(LCD_CS,1); 
    gpio_put(LCR_RST,1); 
    gpio_put(LCD_RD,1); 

    printf("init...\n");
    Lcd_Init();
    printf("OK\n");

  while(1){      
      
      //red
      LCD_Clear(0xF800);
      sleep_ms(1000);
      
      LCD_Clear(RGB(0xFF,0,0));
      sleep_ms(1000);

      //GREEN
      LCD_Clear(0x7E0);
      sleep_ms(1000);

      LCD_Clear(RGB(0,0xFF,0));
      sleep_ms(1000);

      //BLUE
      LCD_Clear(0x1F);
      sleep_ms(1000);
      
      LCD_Clear(RGB(0,0,0));
      sleep_ms(1000);

      uint color=RGB(0xFF,0,0);
      
      //Vertical lines
      for (int i=1; i<320; i++)
      {
        if(i>106){
          color=RGB(0,0xFF,0);
         
        } 
        if(i>212){
          color=RGB(0,0,0xFF);
        } 

        V_line(i,0,480,color);
      }

      sleep_ms(1000);
      LCD_Clear(RGB(0,0,0));
      sleep_ms(500);

      //Horizontal lines
      color=RGB(0xFF,0,0);
      for (int i=1; i<480; i++)
      {
        if(i>160){
          color=RGB(0,0xFF,0);
        } 
        if(i>320){
          color=RGB(0,0,0xFF);
        } 

        H_line(1,i,320,color);
      }
      
      sleep_ms(1000);
      LCD_Clear(RGB(0,0,0));
      sleep_ms(500);



      //Oblique lines 1
      for (int i = 1; i<320; i++){
        Draw_Line(160,1,i,479, color);
      }      
      sleep_ms(1000);
      LCD_Clear(RGB(0,0,0));
      sleep_ms(500);

      //Oblique lines 2
      for (int i = 1; i<320; i++){
        Draw_Line(160,480,i,1, color);
      }      
      sleep_ms(1000);
      LCD_Clear(RGB(0,0,0));
      sleep_ms(500);

      //Oblique lines 3
      for (int i = 1; i<480; i++){
        Draw_Line(0,240,320,i, color);
      } 
      sleep_ms(1000);
      LCD_Clear(RGB(0,0,0));
      sleep_ms(500);

      //Oblique lines 4
      for (int i = 1; i<480; i++){
        Draw_Line(320,240,0,i, color);
      }
      sleep_ms(1000);
      LCD_Clear(RGB(0,0,0));
      sleep_ms(500);

      //Empty rectangles
      color=0xf800;
      for (int i = 0; i<320; i+=10){

        if(i>106){
          color=0x07E0;
        } 
        if(i>212){
          color=0x001F;
        }        

          for(int j=0; j<480; j+=10){
            Rect(i+1, j+1, 8, 8 , color); 
          }
      }

      sleep_ms(1000);
      LCD_Clear(RGB(0,0,0));
      sleep_ms(500);

      //Filled rectangles
      color=0xf800;
      for (int i = 0; i<320; i+=10){

        if(i>106){
          color=0x07E0;
        } 
        if(i>212){
          color=0x001F;
        }        

          for(int j=0; j<480; j+=10){
            Rectf(i+1, j+1, 8, 8 , color); 
          }
      }
      sleep_ms(1000);
      LCD_Clear(RGB(0,0,0));
      sleep_ms(500);

      //Empty triangles
      color=0xf800;
      for (int i = 150; i>0; i-=10){
        if(i<100){
          color=0x001F;
        }
        if(i<50){
          color=0x07E0;
        } 

        Draw_Triangle(160,1+i, 320-i, 480-i, 1+i, 480-i , color);        
      }
      sleep_ms(1000);
      LCD_Clear(RGB(0,0,0));
      sleep_ms(500);

     //Filled triangles
      color=0xf800;
      for (int i = 150; i>0; i-=10){
        if(i<100){
          color=0x001F;
        }
        if(i<50){
          color=0x07E0;
        } 

        Draw_TriangleF(160,1+i, 320-i, 480-i, 1+i, 480-i , color);        
      }
      
      printf("OK\n");
      sleep_ms(2000);
 
      printf("END\n"); 
      
  }
   
}

