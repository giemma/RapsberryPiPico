
  //#define USE_PIO =1
  class PicoST7796 {
    private :
    #ifdef USE_PIO 
    PIO pio = pio0;
    uint sm ;
    uint offset;
    #endif
    void startWrite();
    void endWrite();

    protected:
     
  public:
    PicoST7796();

    void Lcd_Writ_Bus(uint command);
    void Lcd_Write_Com(uint command);
    void Lcd_Write_Data(uint data);
    void Lcd_Write_Com_Data(unsigned int com,unsigned int dat);
    void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
    void Lcd_Init(void);
    void DrawPixel(unsigned int x, unsigned int y, unsigned int c);
    void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
    void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
    void DrawEmptyCircle(unsigned int x, unsigned int y, unsigned int radius, unsigned int c);
    void DrawFilledCircle(unsigned int x0, unsigned int y0, unsigned int radius, unsigned int c);
    void Rect(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int c);
    void Rectf(unsigned int x,unsigned int y,unsigned int w,unsigned int h,unsigned int c);
    int RGB(int r,int g,int b);
    void LCD_Clear(unsigned int j) ;

  };



