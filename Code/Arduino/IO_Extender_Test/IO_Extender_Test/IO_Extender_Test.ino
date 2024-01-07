#include <Wire.h>

#define uint8_t unsigned char
#define PCF8574Add (0x40 >> 1)

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
  Serial.println("I2C Scanner started");
}

void loop() {
  // put your main code here, to run repeatedly:
  Wire.beginTransmission(PCF8574Add);   //Address of sending slave
  Wire.write(0X0F);                     //Controls 8 IO outputs
  Wire.endTransmission();               //ending signal
  
  Wire.requestFrom(PCF8574Add, 1);      //Request to read one byte
  Serial.print("I/O:");
  Serial.println(Wire.read());          //Output the real time value of the current IO through the serial port
  delay(100);
}

Motor motors[] = {
    {false, 0, {8, 9, 10, 11}}, // Connect the motor directly to Arduino
    {true, PCF8574_ADDRESS, {0, 1, 2, 3}}, // Connect the motor to I/O Extender
    {true, PCF8574_ADDRESS, {4, 5, 6, 7}}
};

struct Motor {
    bool useExtender;  // Use the I/O Extender or not
    uint8_t extenderAddress;  // I/O Extender Address
    uint8_t pins[4];  // Connect the motor to these pins
};
