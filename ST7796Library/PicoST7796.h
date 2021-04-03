#include <vector>

#define READ_TIMES 3 //the times for read
#define LOST_VAL 1          //
#define CMD_RDX 0XD0
#define CMD_RDY 0X90

  //#define USE_PIO =1

struct Button
{
  const char *label;
  uint16_t x;
  uint16_t y;
  uint16_t width;
  uint16_t height;
  void (*f)();
};


  class PicoST7796 {
    private :
    
    std::vector<Button> buttons;
   
    #ifdef USE_PIO 
    PIO pio = pio0;
    uint sm ;
    uint offset;
    #endif
        
    void Lcd_Writ_Bus(uint16_t command);
    void Lcd_Write_Com(uint16_t command);
    void Lcd_Write_Data(uint16_t data);
    void Lcd_Write_Com_Data(uint16_t com,uint16_t dat);
    void Address_set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);
    void Lcd_StartWrite();
    void Lcd_EndWrite();

    void H_LineFast(uint16_t x, uint16_t y, uint16_t l, uint16_t c);
    void V_LineFast(uint16_t x, uint16_t y, uint16_t l, uint16_t c);

    void Write_Rect(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height, uint16_t color);
    void Write_RectF(uint16_t x0,uint16_t y0,uint16_t width,uint16_t height,uint16_t color);

    //touch
    uint8_t TP_Read_XY(uint16_t *x,uint16_t *y);
    uint16_t TP_Read_XY(uint8_t xy);
    uint16_t TP_Read_ADC(uint8_t cmd);
    void TP_Write_Byte(uint8_t data);

    void DrawButton(uint16_t x,uint16_t y, uint16_t width, uint16_t height, const char *label);

    protected:
     
  public:
    PicoST7796();

    void Lcd_Init(void);

    //Drawing
    void LCD_Clear(uint16_t color);

    void Draw_Pixel(uint16_t x, uint16_t y, uint16_t color);

    void Draw_LineH(uint16_t x0, uint16_t y0, uint16_t lenght, uint16_t color);
    void Draw_LineV(uint16_t x0, uint16_t y0, uint16_t lenght, uint16_t color);
    void Draw_Line(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t color);
    
    void Draw_Rect(uint16_t x0,uint16_t y0,uint16_t width,uint16_t height,uint16_t color);
    void Draw_RectF(uint16_t x0,uint16_t y0,uint16_t width,uint16_t height,uint16_t color);

    void Draw_Triangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
    void Draw_TriangleF(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);

    void Draw_Circle(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t color);
    void Draw_CircleF(uint16_t x0, uint16_t y0, uint16_t radius, uint16_t color);

    void Draw_Char(int16_t x, int16_t y, unsigned char c, uint16_t color, uint16_t bg, uint8_t size_x, uint8_t size_y);
    void Draw_Text(int16_t x, int16_t y, const char *text, uint16_t color, uint16_t bg, uint8_t size_x, uint8_t size_y);
    
    void Draw_Bitmap(int16_t x, int16_t y, const uint8_t bitmap[],int16_t w, int16_t h);

    //Touch
    void LCD_TP_Init();
    bool LCD_TP_IsTouched();
    uint8_t LCD_TP_Read_Coordinate(uint16_t *x,uint16_t *y) ;


    void LCD_AddButton(uint16_t x,uint16_t y, uint16_t width, uint16_t height, const char *label, void (*f)());
    void RedrawButtons();

    void WaitForClick();

    //utility
    int RGB(uint16_t r,uint16_t g,uint16_t b);
    

  };



