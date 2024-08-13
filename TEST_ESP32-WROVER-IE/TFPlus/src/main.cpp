#include <HardwareSerial.h>
#define SDA 19
#define SCL 21
#define DS 22
#define RX_DIS  4
#define TX_DIS  5
HardwareSerial mySerial(1);
int dist;      //actual distance measurements of LiDAR
int strength;  //signal strength of LiDAR
float temprature;
int check;  //save check value
int i;
int uart[9];              //save data measured by LiDAR
const int HEADER = 0x59;  //frame header of data package
unsigned int time1;

void setup() {
  Serial.begin(115200);                      // Giao tiếp serial với máy tính
  mySerial.begin(115200, SERIAL_8N1, RX_DIS, TX_DIS);  // Giao tiếp với TFmini Plus, dùng GPIO26 (RX) và GPIO27 (TX)
  time1 = millis();
}

void loop() {
  if (mySerial.available()) {         //check if serial port has data input
    if (mySerial.read() == HEADER) {  //assess data package frame header 0x59
      uart[0] = HEADER;
      if (mySerial.read() == HEADER) {  //assess data package frame header 0x59
        uart[1] = HEADER;
        for (i = 2; i < 9; i++) {  //save data in array
          uart[i] = mySerial.read();
        }
        check = uart[0] + uart[1] + uart[2] + uart[3] + uart[4] + uart[5] + uart[6] + uart[7];
        if (uart[8] == (check & 0xff)) {         //verify the received data as per protocol
          dist = uart[2] + uart[3] * 256;        //calculate distance value
          strength = uart[4] + uart[5] * 256;    //calculate signal strength value
          temprature = uart[6] + uart[7] * 256;  //calculate chip temprature
          temprature = temprature / 8 - 256;
        }
      }
    }
  }
  if (millis() - time1 > 1000) {
    Serial.print("dist = ");
    Serial.print(dist);  //output measure distance value of LiDAR
    Serial.print('\t');
    Serial.print("strength = ");
    Serial.print(strength);  //output signal strength value

    Serial.print("\t Chip Temprature = ");
    Serial.print(temprature);
    Serial.println(" celcius degree");  //output chip temperature of Lidar
    time1 = millis();
  }
}