#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/spi.h"
#include "pico/binary_info.h"
#include "hardware/pio.h"
#include "PicoST7796.h"
#include "math.h"
#include "gfxfont.h"


// allow clean compilation with [-Wunused-const-variable=] and [-Wall]
static inline void avoid_unused_const_variable_compiler_warning(void) {
  (void)font;
}


#define SWAP(T, a, b) do { T tmp = a; a = b; b = tmp; } while (0)
#ifndef pgm_read_byte
#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif

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
#define LCR_RST  10 //reset
#define LCD_RS    11 //chip selection CSX
#define LCD_RD   12 //read

  PicoST7796::PicoST7796()
  {
   
  }

int PicoST7796::RGB(uint16_t r,uint16_t g,uint16_t b)
{
  return r << 11 | g << 5 | b;
}

  void PicoST7796::Lcd_Writ_Bus(uint16_t command)
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

  void PicoST7796::Lcd_Write_Com(uint16_t command)
  {
      gpio_put(LCD_RS, 0); 
      Lcd_Writ_Bus(command);
  }

  void PicoST7796::Lcd_Write_Data(uint16_t data)
  {
      gpio_put(LCD_RS, 1); 
      Lcd_Writ_Bus(data);
  }

  void PicoST7796::Lcd_Write_Com_Data(uint16_t com,uint16_t dat)
  {
    Lcd_Write_Com(com);
    Lcd_Write_Data(dat);
  }

  void PicoST7796::Address_set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
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

  void PicoST7796::Lcd_StartWrite()
  {
      Lcd_Write_Com(0x02c); //Memory write
      gpio_put(LCD_RS,1);
      gpio_put(LCD_CS,0);
  }

  void PicoST7796::Lcd_EndWrite()
  {
      gpio_put(LCD_CS,1); 
  }

void PicoST7796::H_LineFast(uint16_t x0, uint16_t y0, uint16_t lenght, uint16_t c)                   
{
  uint16_t i,j;
  lenght=lenght+x0;
  Address_set(x0,y0,lenght,y0);
  j=lenght*2;
  for(i=1;i<=j;i++)
  {
    Lcd_Write_Data(c>>8);
    Lcd_Write_Data(c);
  }
}

void PicoST7796::V_LineFast(uint16_t x0, uint16_t y0, uint16_t lenght, uint16_t c)                   
{
  uint16_t i,j;
  lenght=lenght+y0;
  Address_set(x0,y0,x0,lenght);
  j=lenght*2;
  for(i=1;i<=j;i++)
  { 
    Lcd_Write_Data(c>>8);
    Lcd_Write_Data(c);
  }
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

  gpio_init(LCR_RST);
  gpio_set_dir(LCR_RST, GPIO_OUT);

  gpio_init(LCD_RD);
  gpio_set_dir(LCD_RD, GPIO_OUT);

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


void PicoST7796::LCD_Clear(uint16_t color)                   
{
  uint16_t i,m;
  Address_set(0,0,320,480);
  
  Lcd_StartWrite();
  
  for(i=0;i<320;i++)
    for(m=0;m<480;m++)
    {
      Lcd_Write_Data(color);
      Lcd_Write_Data(color>>8);
    }
 
  Lcd_EndWrite();
}


void PicoST7796::Draw_Pixel(uint16_t x,uint16_t y,uint16_t color)
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

void PicoST7796::Draw_LineH(uint16_t x0, uint16_t y0, uint16_t lenght, uint16_t color)                   
{	
  Lcd_StartWrite();
  H_LineFast(x0,y0,lenght,color);
  Lcd_EndWrite();
}

void PicoST7796::Draw_LineV(uint16_t x0, uint16_t y0, uint16_t lenght, uint16_t color)                   
{
  Lcd_StartWrite();
  V_LineFast(x0,y0,lenght,color);
  Lcd_EndWrite();  
}

void PicoST7796::Draw_Line(uint16_t  x0, uint16_t y0,  uint16_t x1,  uint16_t y1, uint16_t color)
{
  if(y0==y1){

    if (x0 > x1) {
      SWAP(uint16_t, x0, x1);
    }

    Draw_LineH(x0,y0,x1-x0,color);
    return;
  }

  if(x0==x1){

    if (y0 > y1) {
      SWAP(uint16_t, y0, y1);
    }

    Draw_LineV(x0,y0,y1-y0,color);
    return;
  }

  Lcd_StartWrite();

  uint16_t steep = abs(y1 - y0) > abs(x1 - x0);

  if (steep) {
    SWAP(uint16_t, x0, y0);
    SWAP(uint16_t, x1, y1);
  }

  if (x0 > x1) {
    SWAP(uint16_t, x0, x1);
    SWAP(uint16_t, y0, y1);
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
      Draw_Pixel(y0, x0, color);
    } else {
      Draw_Pixel(x0, y0, color);
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }

  Lcd_EndWrite(); 
}

void PicoST7796::Write_Rect(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, uint16_t color)
{
  V_LineFast(x0  , y0  , width, color);
  V_LineFast(x0  , y0+height, width, color);
  H_LineFast(x0  , y0  , height, color);
  H_LineFast(x0+width, y0  , height, color);
}

void PicoST7796::Draw_Rect(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, uint16_t color)
{
  Lcd_StartWrite();
  Write_Rect(x0,y0,width,height,color);
  Lcd_EndWrite();
}

void PicoST7796::Write_RectF(uint16_t x0,uint16_t y0,uint16_t width,uint16_t height,uint16_t color)
{
  unsigned int i;
  for(i=0;i<height;i++)
  {
    H_LineFast(x0  , y0  , width, color);
    H_LineFast(x0  , y0+i, width, color);
  }
}

void PicoST7796::Draw_RectF(uint16_t x0,uint16_t y0,uint16_t width,uint16_t height,uint16_t color)
{
  Lcd_StartWrite();
  Write_RectF(x0, y0, width, height, color);
  Lcd_EndWrite();
}

void PicoST7796::Draw_Triangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
  Draw_Line(x0, y0, x1, y1, color);
  Draw_Line(x1, y1, x2, y2, color);
  Draw_Line(x2, y2, x0, y0, color);
}

void PicoST7796::Draw_TriangleF(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
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
    H_LineFast(a, y0, b - a + 1, color);
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
    H_LineFast(a, y, b - a + 1, color);
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
    H_LineFast(a, y, b - a + 1, color);
  }
  Lcd_EndWrite();
}

void PicoST7796::Draw_Circle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t color)
{
  int16_t f = 1 - radius;
  int16_t ddF_x = 1;
  int16_t ddF_y = -2 * radius;
  int16_t x1= 0;
  int16_t y1= radius;

  Draw_Pixel(x0, y0+radius,color);
  Draw_Pixel(x0, y0-radius,color);
  Draw_Pixel(x0+radius, y0,color);
  Draw_Pixel(x0-radius, y0,color);

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
  
    Draw_Pixel(x0 + x1, y0 + y1,color);
    Draw_Pixel(x0 - x1, y0 + y1,color);
    Draw_Pixel(x0 + x1, y0 - y1,color);
    Draw_Pixel(x0 - x1, y0 - y1,color);
    Draw_Pixel(x0 + y1, y0 + x1,color);
    Draw_Pixel(x0 - y1, y0 + x1,color);
    Draw_Pixel(x0 + y1, y0 - x1,color);
    Draw_Pixel(x0 - y1, y0 - x1,color);
  }

}

void PicoST7796::Draw_CircleF(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t color){
  if(x0-radius <0)
    return;

  if(y0-radius <0)
      return;

  Draw_LineV(x0,y0-radius,2*radius+1,color);
      
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

      Draw_LineV(x0+x, y0-y, 2*y+1,color);
      Draw_LineV(x0+y, y0-x, 2*x+1,color);

      Draw_LineV(x0-x, y0-y, 2*y+1,color);
      Draw_LineV(x0-y, y0-x, 2*x+1,color);   
    }
}


void PicoST7796::Draw_Char(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size_x, uint8_t size_y)
{
  
  int16_t _width =320;
  int16_t _height=480;
  int16_t _cp437=0;

  if ((x >= _width) ||              // Clip right
      (y >= _height) ||             // Clip bottom
      ((x + 6 * size_x - 1) < 0) || // Clip left
      ((y + 8 * size_y - 1) < 0))   // Clip top
      return;

  if (!_cp437 && (c >= 176))
    c++; // Handle 'classic' charset behavior

    Lcd_StartWrite();

    for (int8_t i = 0; i < 5; i++) { // Char bitmap = 5 columns
      uint8_t line = pgm_read_byte(&font[c * 5 + (4-i)]);

      for (int8_t j = 0; j < 8; j++, line >>= 1) {
        if (line & 1) {
          if (size_x == 1 && size_y == 1)
            Draw_Pixel(x + j, y + i, color);
          else
            Write_RectF(x + i * size_x, y + j * size_y, size_x, size_y,
                            color);
          } else if (bg != color) {
            if (size_x == 1 && size_y == 1)
              Draw_Pixel(x + i, y + j, bg);
            else
              Write_RectF(x + i * size_x, y + j * size_y, size_x, size_y, bg);
          }
        }
      }
      if (bg != color) { // If opaque, draw vertical line for last column
        if (size_x == 1 && size_y == 1)
          V_LineFast(x + 5, y, 8, bg);
        else
          Write_RectF(x + 5 * size_x, y, size_x, 8 * size_y, bg);
      }

    Lcd_EndWrite();
      
}

void PicoST7796::Draw_Text(int16_t x, int16_t y, const char *text, uint16_t color, uint16_t bg, uint8_t size_x, uint8_t size_y)
{
  uint16_t charCount=strlen(text);
  
  printf("charCount %d\n", charCount);

  int16_t x0 = x;
  for(int i=0; i<charCount;i++)
  {
    unsigned char c=text[charCount - i- 1];
    Draw_Char(x0 , y, c, color, bg, size_x, size_y);

    x0 = x0 + (size_x * 6);

  }
}

