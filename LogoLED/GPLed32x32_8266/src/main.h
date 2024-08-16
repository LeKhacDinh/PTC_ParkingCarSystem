#ifndef main_application_h
#define main_application_h

#include <Arduino.h>
#include <HardwareSerial.h>
#include "led32x32/matrix.h"

#define CMD_SET_COLOR       '!'
#define CMD_CLEAR           '@'
#define CMD_MOVE_TO_00      '#'
#define CMD_MOVE_TO_XY      '&'
#define CMD_DATA_BEGIN      '$'
#define CMD_DATA_END        '%'
#define CMD_SET_DIRECTION   '^'
#define CMD_SET_FONT_1      '*'
#define CMD_SET_FONT_2      '('
#define CMD_SET_FONT_3      ')'
#define CMD_CLEAR_REC       '['

void receiveUART();
#endif  // main_application_h