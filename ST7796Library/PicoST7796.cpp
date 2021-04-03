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

#include <iostream>





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

//Touch
#define TP_IRQ    13  
#define TP_CS     14  
#define TP_CLK    15  
#define TP_MISO    17  
#define TP_MOSI    16 

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
  H_LineFast(x0  , y0  , width, color);
  H_LineFast(x0  , y0+height, width, color);
  V_LineFast(x0  , y0  , height, color);
  V_LineFast(x0+width, y0  , height, color);
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
  
  int16_t x0 = x;
  for(int i=0; i<charCount;i++)
  {
    unsigned char c=text[charCount - i- 1];
    Draw_Char(x0 , y, c, color, bg, size_x, size_y);

    x0 = x0 + (size_x * 6);

  }
}

void PicoST7796::Draw_Bitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h) 
{
  uint8_t byte1 = 0;
  uint8_t byte2 = 0;
  int16_t color=0xF800;


  int pixel_index=0;
  Lcd_StartWrite();
  for (int16_t j = 0; j < h; j++) {
    for (int16_t i = 0; i < w; i++) {

      byte1=bitmap[pixel_index];
      pixel_index++;

      byte2=bitmap[pixel_index];  
      pixel_index++;

      color = byte1 | byte2<<8;
      Draw_Pixel(x + (w- i), y + j, color);
    }
  }
   
  Lcd_EndWrite();

  printf("pixel_index %d\n", pixel_index);
}


//touch


void TCLK_HIGH(){
   gpio_put(TP_CLK, 1);
}
void TCLK_LOW(){
   gpio_put(TP_CLK, 0);
}

void TCS_HIGH(){
   gpio_put(TP_CS, 1);
}
void TCS_LOW(){
   gpio_put(TP_CS, 0);
}

void TDIN_HIGH(){
   gpio_put(TP_MISO, 1);
}
void TDIN_LOW(){
   gpio_put(TP_MISO, 0);
}

void TDOUT_HIGH(){
   gpio_put(TP_MOSI, 1);
}
void TDOUT_LOW(){
   gpio_put(TP_MOSI, 0);
}

void PicoST7796::TP_Write_Byte(uint8_t data)
{
	uint8_t val = 0x80;
	while(val)
	{
		if(data&val)
		{
			TDIN_HIGH(); 
		}
		else
		{
			TDIN_LOW();
		}
		TCLK_LOW();
		TCLK_HIGH();
		val >>= 1;
	}
}

uint16_t PicoST7796::TP_Read_ADC(uint8_t cmd)
{
	uint16_t num=0; 
	uint8_t count=0; 
	TCS_LOW(); 
	TCLK_LOW();           
	TDIN_LOW();
	TP_Write_Byte(cmd);
	//delay_us(6);              
	TCLK_LOW(); 
	//delay_us(1); 
  sleep_ms(1);
	TCLK_HIGH();
	TCLK_LOW();
	for(count=0;count<16;count++)  
	{   
	    num<<=1;          
	    TCLK_LOW();                         
	    TCLK_HIGH();
	    if(gpio_get(TP_MOSI))
	    {
	         num += 1;                  
	    } 
		else
		{
			num += 0; 
		}
	}
	num>>=4;   // the high 12 bits is valid
    TCS_HIGH();   
   	return num; 
}

uint16_t PicoST7796::TP_Read_XY(uint8_t xy)
{
 	uint16_t i, j;
 	uint16_t buf[READ_TIMES];
 	uint16_t sum=0;
 	uint16_t temp;    
 	for(i=0;i<READ_TIMES;i++)
 	{                                 
  		buf[i]=TP_Read_ADC(xy);            
    }                                    
	for(i=0;i<READ_TIMES-1; i++)//Sort in ascending order
 	{
  		for(j=i+1;j<READ_TIMES;j++)
       	{
        	if(buf[i]>buf[j])
            {
             	temp=buf[i];
             	buf[i]=buf[j];
             	buf[j]=temp;
          	}
      	}
  	}          
	for(i=LOST_VAL;i<READ_TIMES-LOST_VAL;i++) //Remove maximum and minimum values
	{
		sum+=buf[i];
	}
	temp=sum/(READ_TIMES-2*LOST_VAL);
	return temp;   
} 

uint8_t PicoST7796::TP_Read_XY(uint16_t *x,uint16_t *y)
{
        uint16_t xtemp,ytemp;                                                    
        xtemp=TP_Read_XY(CMD_RDX);
        ytemp=TP_Read_XY(CMD_RDY);
        if(xtemp<100||ytemp<100)return 0;//Reading failed
        *x=xtemp;
        *y=ytemp;
        return 1;//Reading success
}

#define ERR_RANGE 50 //error range

#define XFAC      852  
#define XOFFSET   (10)  
#define YFAC      1350 
#define YOFFSET   (25) 

uint8_t PicoST7796::LCD_TP_Read_Coordinate(uint16_t *x,uint16_t *y) 
{
  	uint16_t x1,y1;
  	uint16_t x2,y2;
  	uint8_t flag;    
    flag=TP_Read_XY(&x1,&y1);  
    if(flag==0)
    {
		  return 0;
    }

	  flag=TP_Read_XY(&x2,&y2);
    if(flag==0)
    {
		return 0;   
    }
	  
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//Before and after the two samples are within +- ERR_RANGE.
      &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
      uint16_t rx=(x1+x2)/2;
      uint16_t ry=(y1+y2)/2;

      *x=320-((long)XFAC*rx)/10000+XOFFSET;
      *y=480-((long)YFAC*ry)/10000+YOFFSET;

      return 1;
    }
	  else
	  {
      	return 0;          
  	}        
} 

void PicoST7796::LCD_TP_Init()
{ 
    gpio_init(TP_CS);
    gpio_set_dir(TP_CS, GPIO_OUT);
    
    gpio_init(TP_CLK);
    gpio_set_dir(TP_CLK, GPIO_OUT);
    
    gpio_init(TP_MISO);
    gpio_set_dir(TP_MISO, GPIO_OUT);

    gpio_init(TP_MOSI);
    gpio_set_dir(TP_MOSI, GPIO_IN);

    gpio_init(TP_IRQ);
    gpio_set_dir(TP_IRQ,  GPIO_OUT);

    TDIN_HIGH();
	  TCLK_HIGH(); 
	  TCS_HIGH();
}

bool PicoST7796::LCD_TP_IsTouched()
{
  return !gpio_get(TP_IRQ);
}


void PicoST7796::LCD_AddButton(uint16_t x,uint16_t y, uint16_t width, uint16_t height, const char *label, void (*f)())
{ 
  DrawButton( x, y,  width,  height, label);

  Button b = Button();

  b.label=label;
  b.x=x;
  b.y=y,
  b.width=width;
  b.height=height;
  b.f=f;
  
  buttons.push_back(b);
  sleep_ms(10);
}

void PicoST7796:: DrawButton(uint16_t x,uint16_t y, uint16_t width, uint16_t height, const char *label)
{
  uint16_t textSize=2;

  Draw_RectF(x, y, width, height, RGB(255,0,0));
  Draw_Rect(x, y, width, height, RGB(0,255,0));
  
  uint16_t textX = x +((width - ((strlen(label) * (textSize * 5)))) /2) ;
  uint16_t textY = y + (height/2) - (textSize * 7)/2;
  Draw_Text(textX,textY,label,RGB(255,255,255),RGB(255,0,0),textSize,textSize);

}

void PicoST7796::RedrawButtons(){
  for (auto & btn : buttons) {
    DrawButton(btn.x,btn.y,btn.width,btn.height,btn.label);
  }
}

void PicoST7796::WaitForClick()
{
  while(!LCD_TP_IsTouched()){
    sleep_ms(10);
  }

  uint16_t x,y;
  LCD_TP_Read_Coordinate(&x,&y);
  
  for (auto & btn : buttons) {
    if( x> btn.x && x< (btn.x + btn.width) && y>btn.y && y< (btn.y+btn.height)){
      
      btn.f();
      break;
    }
  }


}


