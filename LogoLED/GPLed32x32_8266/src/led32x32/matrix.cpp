#include "matrix.h"

Matrix::Matrix()
{
}

Matrix::Matrix(unsigned char A, unsigned char B, unsigned char C, unsigned char CLK, unsigned char LAT, unsigned char OE, unsigned char R, unsigned char G)
{
  m_A = A;
  m_B = B;
  m_C = C;
  m_CLK = CLK;
  m_LAT = LAT;
  m_OE = OE;
  m_R = R;
  m_G = G;
}

void Matrix::begin()
{
  pinMode(m_R, OUTPUT);
  pinMode(m_G, OUTPUT);

  pinMode(m_CLK, OUTPUT);
  pinMode(m_OE, OUTPUT);
  pinMode(m_LAT, OUTPUT);
  pinMode(m_A, OUTPUT);
  pinMode(m_B, OUTPUT);
  pinMode(m_C, OUTPUT);

  digitalWrite(m_R, LOW);
  digitalWrite(m_G, LOW);

  digitalWrite(m_CLK, LOW);
  digitalWrite(m_OE, HIGH);
  digitalWrite(m_LAT, LOW);
  digitalWrite(m_A, LOW);
  digitalWrite(m_B, LOW);
  digitalWrite(m_C, LOW);

  clearBuffer();
}

void Matrix::fillScreen(unsigned char color)
{
  unsigned char r = !!(color & RED);
  unsigned char g = !!(color & GREEN);
  for (int i = 0; i < CONST_ROWS; i++)
  {
    for (int j = 0; j < 32; j++)
    {
      Buffer_display[0][i][j] = r;
      Buffer_display[1][i][j] = g;
    }
  }
}

void Matrix::setRowActive(unsigned char row)
{
  // 00 -> row 0
  digitalWrite(m_A, (row == 1 || row == 3) ? HIGH : LOW); // 1
  digitalWrite(m_B, (row == 2 || row == 3) ? HIGH : LOW); // 2
  digitalWrite(m_C, row == 3 ? HIGH : LOW);               // 3
                                                          // 11 -> row 4
}

void Matrix::drawPoint(unsigned char x, unsigned char y, unsigned char color)
{
  if (x > Graphic_x || y > Graphic_y || x < 0 || y < 0)
  {
    return;
  }
  unsigned char row;   // 0-3
  unsigned char block; // 0-7
  unsigned char id;    // 0-3
  unsigned char value;
  if (y > 15)
  {
    // bottom panel
    row = y % 4;
    block = (x / 8) * 4 + (31 - y) / 4 + 16;

    value = (31 - x) % 8;
    if ((color & R_MASK) != 0)
    {
      Buffer_display[0][row][block] |= (0x01 << value);
    }
    else
    {
      Buffer_display[0][row][block] &= ~(0x01 << value);
    }
    if ((color & G_MASK) != 0)
    {
      Buffer_display[1][row][block] |= (0x01 << value);
    }
    else
    {
      Buffer_display[1][row][block] &= ~(0x01 << value);
    }
  }
  else
  {
    // top panel
    row = (15 - y) % 4;
    block = ((31 - x) / 8) * 4 + y / 4;

    value = (31 - x) % 8;
    if ((color & R_MASK) != 0)
    {
      Buffer_display[0][row][block] |= (0x80 >> value);
    }
    else
    {
      Buffer_display[0][row][block] &= ~(0x80 >> value);
    }
    if ((color & G_MASK) != 0)
    {
      Buffer_display[1][row][block] |= (0x80 >> value);
    }
    else
    {
      Buffer_display[1][row][block] &= ~(0x80 >> value);
    }
  }
}

void Matrix::moveTo(unsigned char x, unsigned char y)
{
  m_cursor_x = x;
  m_cursor_y = y;
}

void Matrix::setCursor(unsigned char x, unsigned char y)
{
  moveTo(x, y);
}

void Matrix::drawLogo(unsigned char color)
{
  int x = 0;
  int y = 0;
  unsigned char fontW = 32;
  clearBuffer();
  for(int b = 0; b<128; b++){
    int byte = font_logo[0][b];
    for(int i = 0; i<8; i++){
      if ((byte & (0x80 >> i)) != 0){
        switch(m_direction){
          case CONST_PRINT_DIRECTION_RIGHT:
            drawPoint(31 - x, 31 - y, color);
            break;
          case CONST_PRINT_DIRECTION_TOP:
            drawPoint(31 - y, x, color);
            break;
          case CONST_PRINT_DIRECTION_BOTTOM:
            drawPoint(y, 31 - x, color);
            break;
          default:
            drawPoint(x, y, color);
        }
      }
      x++;
    }
    if(x>=fontW){
      x = 0;
      y++;
    }
  }
}

void Matrix::drawChar(unsigned char txt, unsigned char color, unsigned char font_size)
{
  int x, y;
  unsigned char fontW, fontH;
  if (font_size == 3)
  {
    fontW = 16;
    fontH = 32;
  }
  else if (font_size == 2)
  {
    fontW = 8;
    fontH = 16;
  }
  else
  {
    fontW = 6;
    fontH = 7;
  }

  for (y = m_cursor_y; y < fontH + m_cursor_y; y++)
  {
    for (x = m_cursor_x; x < fontW + m_cursor_x; x++)
    {
      if (font_size == 3)
      {
        if (x < (m_cursor_x + 8) && (font16x32[txt - 32][(y - m_cursor_y) * 2] & (0x80 >> (x - m_cursor_x))) != 0)
        {
          drawPoint(x, y, color);
        }
        else if (x >= (m_cursor_x + 8) && (font16x32[txt - 32][(y - m_cursor_y) * 2 + 1] & (0x80 >> (x - m_cursor_x - 8))) != 0)
        {
          drawPoint(x, y, color);
        }
        else
        {
          drawPoint(x, y, 0);
        }
      }
      else if (font_size == 2 && (font8x16_basic[txt - 32][y - m_cursor_y] & (0x80 >> (x - m_cursor_x))) != 0)
      {
        drawPoint(x, y, color);
      }
      else if (font_size != 2 && font_size != 3 && (font6x7[txt - 32][y - m_cursor_y] & (0x80 >> (x - m_cursor_x))) != 0)
      {
        drawPoint(x, y, color);
      }
      else
      {
        drawPoint(x, y, 0);
      }
    }
  }
  m_cursor_x += fontW;
}

void Matrix::drawCharRight(unsigned char txt, unsigned char color, unsigned char font_size)
{
  int x, y;
  unsigned char fontW, fontH;
  if (font_size == 3)
  {
    fontW = 16;
    fontH = 32;
  }
  else if (font_size == 2)
  {
    fontW = 8;
    fontH = 16;
  }
  else
  {
    fontW = 6;
    fontH = 7;
  }

  for (y = m_cursor_y; y < fontH + m_cursor_y; y++)
  {
    for (x = m_cursor_x; x < fontW + m_cursor_x; x++)
    {
      if (font_size == 3)
      {
        // 1 row 16bit = 2byte
        if (x < (m_cursor_x + 8) && (font16x32[txt - 32][(y - m_cursor_y) * 2] & (0x80 >> (x - m_cursor_x))) != 0)
        {
          //byte 1
          drawPoint(31 - x, 31 - y, color);
        }
        else if (x >= (m_cursor_x + 8) && (font16x32[txt - 32][(y - m_cursor_y) * 2 + 1] & (0x80 >> (x - m_cursor_x - 8))) != 0)
        {
          //byte 2
          drawPoint(31 - x, 31 - y, color);
        }
        else
        {
          drawPoint(31 - x, 31 - y, 0);
        }
      }
      else if (font_size == 2 && (font8x16_basic[txt - 32][y - m_cursor_y] & (0x80 >> (x - m_cursor_x))) != 0)
      {
        // 1 row 8bit = 1byte
        drawPoint(31 - x, 31 - y, color);
      }
      else if (font_size != 2 && font_size != 3 && (font6x7[txt - 32][y - m_cursor_y] & (0x80 >> (x - m_cursor_x))) != 0)
      {
        // 1 row 6bit hight of 1byte
        drawPoint(31 - x, 31 - y, color);
      }
      else
      {
        drawPoint(31 - x, 31 - y, 0);
      }
    }
  }
  m_cursor_x += fontW;
}

void Matrix::drawCharTop(unsigned char txt, unsigned char color, unsigned char font_size)
{
  int x, y;
  unsigned char fontW, fontH;
  if (font_size == 3)
  {
    fontW = 16;
    fontH = 32;
  }
  else if (font_size == 2)
  {
    fontW = 8;
    fontH = 16;
  }
  else
  {
    fontW = 6;
    fontH = 7;
  }

  for (y = m_cursor_y; y < fontH + m_cursor_y; y++)
  {
    for (x = m_cursor_x; x < fontW + m_cursor_x; x++)
    {
      if (font_size == 3)
      {
        if (x < (m_cursor_x + 8) && (font16x32[txt - 32][(y - m_cursor_y) * 2] & (0x80 >> (x - m_cursor_x))) != 0)
        {
          drawPoint(31 - y, x, color);
        }
        else if (x >= (m_cursor_x + 8) && (font16x32[txt - 32][(y - m_cursor_y) * 2 + 1] & (0x80 >> (x - m_cursor_x - 8))) != 0)
        {
          drawPoint(31 - y, x, color);
        }
        else
        {
          drawPoint(31 - y, x, 0);
        }
      }
      else if (font_size == 2 && (font8x16_basic[txt - 32][y - m_cursor_y] & (0x80 >> (x - m_cursor_x))) != 0)
      {
        drawPoint(31 - y, x, color);
      }
      else if (font_size != 2 && font_size != 3 && (font6x7[txt - 32][y - m_cursor_y] & (0x80 >> (x - m_cursor_x))) != 0)
      {
        drawPoint(31 - y, x, color);
      }
      else
      {
        drawPoint(31 - y, x, 0);
      }
    }
  }
  m_cursor_x += fontW;
}

void Matrix::drawCharBottom(unsigned char txt, unsigned char color, unsigned char font_size)
{
  int x, y;
  unsigned char fontW, fontH;
  if (font_size == 3)
  {
    fontW = 16;
    fontH = 32;
  }
  else if (font_size == 2)
  {
    fontW = 8;
    fontH = 16;
  }
  else
  {
    fontW = 6;
    fontH = 7;
  }

  for (y = m_cursor_y; y < fontH + m_cursor_y; y++)
  {
    for (x = m_cursor_x; x < fontW + m_cursor_x; x++)
    {
      if (font_size == 3)
      {
        if (x < (m_cursor_x + 8) && (font16x32[txt - 32][(y - m_cursor_y) * 2] & (0x80 >> (x - m_cursor_x))) != 0)
        {
          drawPoint(y, 31 - x, color);
        }
        else if (x >= (m_cursor_x + 8) && (font16x32[txt - 32][(y - m_cursor_y) * 2 + 1] & (0x80 >> (x - m_cursor_x - 8))) != 0)
        {
          drawPoint(y, 31 - x, color);
        }
        else
        {
          drawPoint(y, 31 - x, 0);
        }
      }
      else if (font_size == 2 && (font8x16_basic[txt - 32][y - m_cursor_y] & (0x80 >> (x - m_cursor_x))) != 0)
      {
        drawPoint(y, 31 - x, color);
      }
      else if (font_size != 2 && font_size != 3 && (font6x7[txt - 32][y - m_cursor_y] & (0x80 >> (x - m_cursor_x))) != 0)
      {
        drawPoint(y, 31 - x, color);
      }
      else
      {
        drawPoint(y, 31 - x, 0);
      }
    }
  }
  m_cursor_x += fontW;
}

void Matrix::drawString(String s, unsigned char color, unsigned char fontSize)
{
  unsigned char ind = 0;
  while (ind < s.length())
  {
    drawChar(s.charAt(ind), color, fontSize);
    ind++;
  }
}

void Matrix::drawStringUp(String s, unsigned char color, unsigned char fontSize)
{
  unsigned char ind = 0;
  while (ind < s.length())
  {
    drawCharBottom(s.charAt(ind), color, fontSize);
    ind++;
  }
}

void Matrix::redraw(void)
{
  for (unsigned char row = 0; row < 4; row++)
  {
    // Select the row
    digitalWrite(m_OE, HIGH);
    digitalWrite(m_LAT, LOW);
    setRowActive(row);
    // Update the display for the selected row
    digitalWrite(m_OE, HIGH);
    for (unsigned char i = 0; i < 32; i++)
    {
      unsigned char mask = 0x80;
      for (unsigned char j = 0; j < 8; j++)
      {
        mask = 0x80 >> j;
        digitalWrite(m_R, !!(Buffer_display[0][row][i] & mask));
        digitalWrite(m_G, !!(Buffer_display[1][row][i] & mask));

        digitalWrite(m_CLK, HIGH);
        digitalWrite(m_CLK, LOW);
      }
    }
    digitalWrite(m_LAT, HIGH);
    digitalWrite(m_LAT, LOW);

    delayMicroseconds(300);
    digitalWrite(m_OE, LOW);
    delayMicroseconds(1000 - 300);
  }
}

void Matrix::clearBuffer()
{
  for (int i = 0; i < CONST_ROWS; i++)
  {
    for (int j = 0; j < 32; j++)
    {
      Buffer_display[0][i][j] = 0x00;
      Buffer_display[1][i][j] = 0x00;
    }
  }
}

void Matrix::clearBuffer(unsigned char x, unsigned char y, unsigned char w, unsigned char h)
{
  for (int i = x; i < w; i++)
  {
    for (int j = y; j < h; j++)
    {
      drawPoint(i, j, 0x00);
    }
  }
}

void Matrix::setTextSize(unsigned char size)
{
  m_text_size = size;
}

void Matrix::setTextColor(unsigned char color)
{
  m_text_color = color;
}

void Matrix::setPrintDirection(unsigned char direction)
{
  m_direction = direction;
}

void Matrix::print(unsigned char txt)
{
  Serial.print("direction:");
  Serial.println(m_direction % 4);
  switch (m_direction % 4)
  {
    case CONST_PRINT_DIRECTION_RIGHT:
      drawCharRight(txt, m_text_color, m_text_size);
      break;
    case CONST_PRINT_DIRECTION_TOP:
      drawCharTop(txt, m_text_color, m_text_size);
      break;
    case CONST_PRINT_DIRECTION_BOTTOM:
      drawCharBottom(txt, m_text_color, m_text_size);
      break;
    default:
      drawChar(txt, m_text_color, m_text_size);
  }
}

void Matrix::print(String txt)
{
  unsigned char ind = 0;
  while (ind < txt.length())
  {
    print(txt.charAt(ind));
    ind++;
  }
}