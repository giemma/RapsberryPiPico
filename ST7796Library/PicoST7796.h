
  //#define USE_PIO =1


  class PicoST7796 {
    private :
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
    protected:
     
  public:
    PicoST7796();

    void Lcd_Init(void);
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
    //utility
    int RGB(uint16_t r,uint16_t g,uint16_t b);
    

  };



