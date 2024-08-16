#include "main.h"

HardwareSerial mySerial(1);

#define FW_VISION "CP_ESP8266.1.0.1" //phiên bản của code

Matrix matrix;

void setup() {
  Serial.begin(115200);

  matrix.begin();
  matrix.clearBuffer();
  matrix.setPrintDirection(CONST_PRINT_DIRECTION_TOP);
  matrix.moveTo(0, 0);
  matrix.setTextColor(GREEN);
  matrix.setTextSize(3);
  matrix.print("G");
  matrix.moveTo(15, 0);
  matrix.setTextColor(RED);
  matrix.print("M");
}

void loop() {
  matrix.redraw();
  receiveUART();
}

int direction = 0;
char cmd = CMD_DATA_BEGIN;
unsigned char clear = 0;
unsigned char moveTo00 = 0;
unsigned char moveToXY = 0;
unsigned char move_x = 0;
unsigned char move_y = 0;
unsigned char fontSize = 3;

unsigned char x = 0;
unsigned char y = 0;
unsigned char w = 32;
unsigned char h = 32;
unsigned char cursor = 0;
String data = "";
// Function to receive UART data
void receiveUART() {
    // Check if there is data available to read
    if (Serial.available() > 0) {
        // Read the incoming byte
        char incomingByte = Serial.read();
        // Print the incoming byte to the Serial Monitor
        if(incomingByte == CMD_SET_COLOR){
          //color
          cmd = incomingByte;
          Serial.print("Color #: ");
        }else if(incomingByte==CMD_MOVE_TO_00){
          cmd = incomingByte;
          moveTo00 = 1;
          Serial.println("Move to 0x0");
        }else if(incomingByte==CMD_MOVE_TO_XY){
          cmd = incomingByte;
          moveToXY = 1;
          cursor = 0;
          Serial.println("Move to XY");
        }else if(incomingByte==CMD_SET_FONT_1){
          cmd = incomingByte;
          fontSize = 1;
          Serial.println("CMD_SET_FONT_1");
        }else if(incomingByte==CMD_SET_FONT_2){
          cmd = incomingByte;
          fontSize = 2;
          Serial.println("CMD_SET_FONT_2");
        }else if(incomingByte==CMD_SET_FONT_3){
          cmd = incomingByte;
          fontSize = 3;
          Serial.println("CMD_SET_FONT_3");
        }else if(incomingByte==CMD_CLEAR){
          cmd = incomingByte;
          clear = 1;
          Serial.println("Clear");
        }else if(incomingByte==CMD_CLEAR_REC){
          cmd = incomingByte;
          clear = 2;
          cursor = 0;
          Serial.println(", Clear rec: 0");
        }else if(incomingByte==CMD_DATA_BEGIN){
          cmd = incomingByte;
          data = "";
          Serial.print("Data #:");
        }else if(incomingByte==CMD_DATA_END){
          cmd = incomingByte;
          if(clear == 1){
            Serial.print(", clear");
            matrix.clearBuffer();
            clear = 0;
          }
          if(clear == 2){
            Serial.print(", clear rec");
            matrix.clearBuffer(x, y, w, h);
            clear = 0;
          }
          if(moveTo00 == 1){
            Serial.print(", move to 0x0");
            matrix.moveTo(0, 0);
            moveTo00 = 0;
          }
          if(moveToXY == 1){
            Serial.print(", move to XY");
            matrix.moveTo(move_x, move_y);
            moveToXY = 0;
          }
          Serial.print(", fontSize: ");
          Serial.print(fontSize);
          matrix.setTextSize(fontSize);

          Serial.print(",data: ");
          Serial.println(data);
          matrix.print(data);
        }else if(incomingByte==CMD_SET_DIRECTION){
          cmd = incomingByte;
          Serial.print("Direction: ");
        }else{
          String tmp = "";
          switch(cmd){
            case CMD_SET_COLOR:
              //color
              tmp = String(incomingByte);
              matrix.setTextColor(tmp.toInt());
              Serial.println(tmp.toInt());
              break;
            case CMD_SET_DIRECTION:
              //color
              tmp = String(incomingByte);
              matrix.setPrintDirection(tmp.toInt());
              Serial.println(tmp.toInt());
              break;
            case CMD_DATA_BEGIN:
              data = data + incomingByte;
              break;
            case CMD_CLEAR_REC:
              switch(cursor){
                case 0:
                  x = (int)incomingByte;
                  Serial.print(", ");
                  Serial.print(x);
                  break;
                case 1:
                  y = (int)incomingByte;
                  Serial.print(", ");
                  Serial.print(y);
                  break;
                case 2:
                  w = (int)incomingByte;
                  Serial.print(", ");
                  Serial.print(w);
                  break;
                case 3:
                  h = (int)incomingByte;
                  Serial.print(", ");
                  Serial.print(h);
                  break;
              }
              cursor++;
              break;
            case CMD_MOVE_TO_XY:
              switch(cursor){
                case 0:
                  move_x = (int)incomingByte-65;
                  Serial.print(", ");
                  Serial.print(move_x);
                  break;
                case 1:
                  move_y = (int)incomingByte-65;
                  Serial.print(", ");
                  Serial.print(move_y);
                  break;
              }
              cursor++;
              break;
          }
        }
    }
}