#ifndef Matric_class_h
#define Matric_class_h
#include <Arduino.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "font/font.h"

#define CLK_P 14
#define OE_P  16
#define LAT_P 2
#define A_P   15
#define B_P   13
#define C_P   12

#define R_P 0          // R1 Pin
#define G_P 5           // G1 Pin

#define Graphic_x 32
#define Graphic_y 32

#define BLACK     0x00
#define WHITE     0x07
#define RED       0x01
#define GREEN     0x02
#define BLUE      0x04
#define MAGENTA   0x05
#define YELLOW    0x03
#define CYAN      0x06

#define R_MASK      0x01
#define G_MASK      0x02
#define B_MASK      0x04
#define CONST_DEFAULT_TIMER_INTERRUPTS_IN_S 1
#define CONST_ROWS      4

#define CONST_PRINT_DIRECTION_NORMAL    0
#define CONST_PRINT_DIRECTION_LEFT      0
#define CONST_PRINT_DIRECTION_RIGHT     1
#define CONST_PRINT_DIRECTION_TOP       2
#define CONST_PRINT_DIRECTION_BOTTOM    3

class Matrix
{
public:
    //for hub12
    Matrix();
    Matrix(unsigned char AP, unsigned char BP, unsigned char CP, unsigned char CLKP, unsigned char LATP, unsigned char OEP, unsigned char RP, unsigned char GP);

    void fillScreen(unsigned char color);
    void setRowActive(unsigned char row);

    void drawPoint(unsigned char x, unsigned char y, unsigned char color);
    void moveTo(unsigned char x, unsigned char y);
    void setCursor(unsigned char x, unsigned char y);
    void drawChar(unsigned char txt,unsigned char color, unsigned char font_size);
    void drawCharRight(unsigned char txt,unsigned char color, unsigned char font_size);
    void drawCharTop(unsigned char txt,unsigned char color, unsigned char font_size);
    void drawCharBottom(unsigned char txt,unsigned char color, unsigned char font_size);
    void drawString(String s, unsigned char color, unsigned char fontSize);
    void drawStringUp(String s, unsigned char color, unsigned char fontSize);
    void redraw(void);
    void clearBuffer();
    void clearBuffer(unsigned char x, unsigned char y, unsigned char w, unsigned char h);
    void setTextColor(unsigned char color);
    void setTextSize(unsigned char size);
    void setPrintDirection(unsigned char direction);
    void print(unsigned char txt);
    void print(String txt);
    void begin();
private:
    unsigned char m_A = A_P;
    unsigned char m_B = B_P;
    unsigned char m_C = C_P;
    unsigned char m_CLK = CLK_P;
    unsigned char m_LAT = LAT_P;
    unsigned char m_OE = OE_P;
    unsigned char m_R = R_P;
    unsigned char m_G = G_P;
    unsigned char m_cursor_x = 0;
    unsigned char m_cursor_y = 0;

    unsigned char Buffer_display[2][CONST_ROWS][32];
    unsigned char m_text_color = BLACK;
    unsigned char m_text_size = 3;
    unsigned char m_direction = CONST_PRINT_DIRECTION_NORMAL;

};

#endif  // Matric_class_h