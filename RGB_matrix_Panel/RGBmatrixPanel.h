#ifndef RGB_MATRIX_PANEL_H
#define RGB_MATRIX_PANEL_H

#include "mbed.h"
#include "Adafruit_GFX.h"

#ifndef _BV
#define _BV(bit) (1<<(bit))
#endif

#ifdef DEBUG
#define log_debug(format,...)   std::printf(format,##__VA_ARGS__)
#else
#define log_debug(...)
#endif

static GPIO_Type * const gpio_addrs[] = GPIO_BASE_PTRS;

static void setPin(PinName p, int value)
{
    uint32_t port = p >> GPIO_PORT_SHIFT;
    uint32_t pin = p & 0xFF;
    uint8_t val;
    if(value == 0) {
        gpio_addrs[port]->PCOR = 1U << pin;
    } else {
        gpio_addrs[port]->PSOR = 1U << pin;
    }
}

class RGBmatrixPanel : public Adafruit_GFX
{

public:
    // Constructor for 16x32 panel:
    //RGBmatrixPanel(PinName r1,PinName g1,PinName b1,PinName r2,PinName g2,PinName b2,PinName a,PinName b,PinName c,PinName sclk,PinName latch,PinName oe, bool dbuf);

    // Constructor for 32x32 panel (adds 'd' pin):
    //RGBmatrixPanel(PinName r1,PinName r2,PinName g1,PinName g2,PinName b1,PinName b2,PinName a,PinName b,PinName c,PinName d,PinName sclk, PinName latch,PinName oe,bool dbuf);

    RGBmatrixPanel(PinName Pin_R1, PinName Pin_R2, PinName Pin_G1, PinName Pin_G2, 
         PinName Pin_B1, PinName Pin_B2, 
         PinName Pin_A, PinName Pin_B, PinName Pin_C, PinName Pin_D,
         PinName Pin_CLK, PinName Pin_LAT, PinName Pin_OE);

    int numRefreshes;
    void printRefreshes(void) {
        printf("Refreshes: %06dd\n", numRefreshes);
    }
        
    /** @fn void RGBmatrixPanel::begin(void)
    *   @bref Attach a updateDisplay() to be called by the Ticker(every 100us)
    */
    void begin(void);
    /** @fn void RGBmatrixPanel::drawPixel(int16_t x, int16_t y, uint16_t c)
    *   @bref drawPixel
    */
    virtual void drawPixel(int16_t x,int16_t y,uint16_t c);
    /** @fn void RGBmatrixPanel::fillScreen(uint16_t c)
    *   @bref fillScreen
    *   @param  c   fill screen 16bit color 0x0000 ~ 0xFFFF
    */
    virtual void fillScreen(uint16_t c);
    /** @fn void RGBmatrixPanel::updateDisplay(void)
    *   @param c    updateDisplay\n
    *               This method is called by the interrupt start at begin().
    */
    void updateDisplay(void);
    /** @fn void RGBmatrixPanel::swapBuffers(bool copy)
    *   @param  copy swap buffer (if you use double-buffer)
    */
    void swapBuffers(bool copy);
    /** @fn void RGBmatrixPanel::dumpMatrix(void)
    *   @bref dump to default USB Serial\n
    *         Declaration is required to use.(#define DEBUG)
    */
    void dumpMatrix(void);

    uint8_t *backBuffer(void);

    /** @fn uint16_t RGBmatrixPanel::Color333(uint8_t r, uint8_t g, uint8_t b)
    *   @bref up convert to 16bit color from 9bit color.
    *   @return 16bit(uint16_t) color value
    */
    uint16_t Color333(uint8_t r, uint8_t g, uint8_t b);
    /** @fn uint16_t RGBmatrixPanel::Color444(uint8_t r, uint8_t g, uint8_t b)
    *   @bref up convert to 16bit color from 12bit color.
    *   @param r 0~7
    *   @param g 0~7
    *   @param b 0~7
    *   @return 16bit(uint16_t) color value
    */
    uint16_t Color444(uint8_t r, uint8_t g, uint8_t b);
    /** @fn uint16_t RGBmatrixPanel::Color888(uint8_t r, uint8_t g, uint8_t b)
    *   @bref down convert to 16bit color from 24bit color.
    *   @return 16bit(uint16_t) color value
    */
    uint16_t Color888(uint8_t r, uint8_t g, uint8_t b);
    /** @fn uint16_t RGBmatrixPanel::Color888(uint8_t r, uint8_t g, uint8_t b, bool gflag)
    *   @bref down convert to 16bit color from 24bit color using the gamma value table.
    *   @return 16bit(uint16_t) color value
    */
    uint16_t Color888(uint8_t r, uint8_t g, uint8_t b, bool gflag);
    /** @fn uint16_t RGBmatrixPanel::ColorHSV(long hue, uint8_t sat, uint8_t val, bool gflag)
    *   @bref convert to 16bit color from (unsigned integer)HSV color using the gamma value table.
    *   @param hue 0~1536(decimal value)
    *   @param sat 0~255(decimal value) Does not make sense that it is not a multiple of 32.
    *   @param val 0~255(decimal value) Does not make sense that it is not a multiple of 32.
    *   @return 16bit(uint16_t) color value
    */
    uint16_t ColorHSV(long hue, uint8_t sat, uint8_t val, bool gflag)
    ;
    /** @fn uint16_t RGBmatrixPanel::ColorHSV(float hue, float sat, float val, bool gflag)
    *   @bref convert to 16bit color from (float)HSV color using the gamma value table.
    *   @param hue Normalized value from 0.0 to 1.0
    *   @param sat Normalized value from 0.0 to 1.0
    *   @param val Normalized value from 0.0 to 1.0
    *   @return 16bit(uint16_t) color value
    */
    uint16_t ColorHSV(float hue, float sat, float val, bool gflag);
private:
    uint8_t *matrixbuff[2];
    uint8_t nRows;
    uint8_t backindex;
    bool swapflag;

    // Init/alloc code common to both constructors:
    void init(uint8_t rows, bool dbuf);

    BusOut _dataBus;
    BusOut _rowBus;
    DigitalOut _d,_sclk, _latch, _oe;
    Ticker _refresh;
    
    DigitalOut _Pin_R1; 
    DigitalOut _Pin_R2; 
    DigitalOut _Pin_G1; 
    DigitalOut _Pin_G2; 
    DigitalOut _Pin_B1; 
    DigitalOut _Pin_B2; 
    DigitalOut _Pin_CLK; 
    DigitalOut _Pin_LAT; 
    DigitalOut _Pin_OE; 
    DigitalOut _Pin_A; 
    DigitalOut _Pin_B; 
    DigitalOut _Pin_C; 
    DigitalOut _Pin_D; 
    
    PinName Pin_R1; 
    PinName Pin_R2; 
    PinName Pin_G1; 
    PinName Pin_G2; 
    PinName Pin_B1; 
    PinName Pin_B2; 
    PinName Pin_CLK; 
    PinName Pin_LAT; 
    PinName Pin_OE; 
    PinName Pin_A; 
    PinName Pin_B; 
    PinName Pin_C; 
    PinName Pin_D; 
    
    
    
    // Counters/pointers for interrupt handler:
    uint8_t row, plane;
    uint8_t *buffptr;
};

#endif
