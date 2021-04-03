
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/spi.h"
#include "pico/binary_info.h"
#include "math.h"

#define LCD_CS    8 //chip selection CSX
#define LCD_WR    9 //write
#define LCR_RST   10 //reset
#define LCD_RS    11 //command 0 data 1 selection D/CX
#define LCD_RD    12 //read

//Touch

#define TP_IRQ    13  
#define TP_CS     14  
#define TP_CLK    15  
#define TP_MISO    17  
#define TP_MOSI    16  


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

void H_LineFast(unsigned int x0, unsigned int y0, unsigned int lenght, unsigned int c)                   
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

void V_LineFast(unsigned int x0, unsigned int y0, unsigned int lenght, unsigned int c)                   
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
  H_LineFast(x0,y0,lenght,c);
  Lcd_EndWrite();
}

void V_line(unsigned int x0, unsigned int y0, unsigned int lenght, unsigned int c)                   
{
  Lcd_StartWrite();
  V_LineFast(x0,y0,lenght,c);
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

void TP_Write_Byte(uint8_t data)
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

uint16_t TP_Read_ADC(uint8_t cmd)
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


#define READ_TIMES 3 //the times for read
#define LOST_VAL 1          //
#define CMD_RDX 0XD0
#define CMD_RDY 0X90

uint16_t TP_Read_XY(uint8_t xy)
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

uint8_t TP_Read_Coordinate(uint16_t *x,uint16_t *y)
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

uint8_t TP_Read_Coordinate2(uint16_t *x,uint16_t *y) 
{
  	uint16_t x1,y1;
  	uint16_t x2,y2;
  	uint8_t flag;    
    flag=TP_Read_Coordinate(&x1,&y1);  
    if(flag==0)
    {
		  return 0;
    }

	  flag=TP_Read_Coordinate(&x2,&y2);
    if(flag==0)
    {
		return 0;   
    }
	  
    if(((x2<=x1&&x1<x2+ERR_RANGE)||(x1<=x2&&x2<x1+ERR_RANGE))//Before and after the two samples are within +- ERR_RANGE.
      &&((y2<=y1&&y1<y2+ERR_RANGE)||(y1<=y2&&y2<y1+ERR_RANGE)))
    {
        *x=(x1+x2)/2;
        *y=(y1+y2)/2;
        return 1;
    }
	  else
	  {
      	return 0;          
  	}        
} 

uint16_t x,y;
long myX,myY;
double factX=0;
double factY=0;

//for resolution 320x480,the calibration parameter is 852,-14,1284,-30
#define XFAC      3785// 852 //663 
#define XOFFSET   (-258)//(-14) //(-13) 
#define YFAC      3819// 1284 //894 
#define YOFFSET   (-352)//(-30) 

uint16_t calUpperX=0;
uint16_t calBottomX=0;
uint16_t calUpperY=0;
uint16_t calBottomY=0;

void ReadTouch()
{
  x=0;
  y=0;
  if(!gpio_get(TP_IRQ)){
    
    TP_Read_Coordinate2(&x,&y);
    
    printf("X,Y: %d , %d \n",x , y);
    factX =(double)(XFAC +XOFFSET ) / (double)x;
    factY =(double)(YFAC +YOFFSET ) / (double)y;
    
    printf("myX,myY: %d , %d \n",myX , myY);
    myX = (320 - (double)320/factX) + 40;
    myY = (480 - (double)480/factY) /*+ 35*/;
    
    
    if(myX>=0 && myY>=0 && myX<=320 && myY<=480)
    {
      //Lcd_StartWrite();
      //Write_Pixel(myX,myY, 0x1F);
      //Lcd_EndWrite();

      Rectf(myX, myY, 3, 3, 0x1F);
    }    

    //printf("myX,myY: %d , %d \n",myX , myY);

  }else{
    //LCD_Clear(0x0);
  }
  
}

#define SPI_PORT spi0
void touch_Init()
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
     
    while(1)
    {
      ReadTouch();  
    }

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


    //Touch
    /*
    gpio_init(TP_CS);
    gpio_set_dir(TP_CS, GPIO_OUT);

    gpio_init(TP_CLK);
    gpio_set_dir(TP_CLK, GPIO_OUT);

    gpio_init(TP_MISO);
    gpio_set_dir(TP_MISO, GPIO_OUT);

    gpio_init(TP_IRQ);
    gpio_set_dir(TP_IRQ, GPIO_OUT);

    gpio_init(TP_MOSI);
    gpio_set_dir(TP_MOSI, GPIO_IN);
    */

    printf("init...\n");
    Lcd_Init();
    printf("OK\n");


  LCD_Clear(0xFFFF);

  touch_Init();
 

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

