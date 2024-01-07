#include <Wire.h>

#define uint8_t unsigned char
#define PCF8574_ADD (0x40 >> 1)
#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11
// #define HALL_SENSOR_PIN 6

const int stepsPerRevolution = 4096; // Step Count Per Revolution
const int stepsPerDigit = stepsPerRevolution / 10; // Step Count Per Number
int Steps = 0;
int lastInputNumber = 0; // Last Number
boolean Direction = true; // Rotate Direction

struct Motor {
    bool useExtender;  // 是否使用I/O扩展器
    uint8_t extenderAddress;  // I/O扩展器的地址
    uint8_t pins[4];  // 直接连接到Arduino的引脚或在I/O扩展器上的引脚位置
    int hallPin;
};

// Motor motors[] = {
//     {false, 0, {8, 9, 10, 11}, 13}, // 直接连接到Arduino的电机
//     {true, PCF8574_ADD, {0, 1, 2, 3}, 12}, // 连接到I/O扩展器的电机
//     {true, PCF8574_ADD, {4, 5, 6, 7}, 6}
// };
Motor motor = {true, PCF8574_ADD, {0,1,2,3}, 3};

void setup() {
     Serial.begin(9600);
     pinMode(motor.hallPin, INPUT);

     resetPosition(motor);
}

void loop() {
  for(int i=0; i<4096; i++){
      stepper(motor, 1);
      delayMicroseconds(2000);
      }
}

void stepper(Motor motor, int xw) {
     for (int x = 0; x < xw; x++) {

        switch (Steps) {
             case 0:
                  setMotorPins(motor, LOW, LOW, LOW, HIGH);
                  break;
             case 1:
                  setMotorPins(motor, LOW, LOW, HIGH, HIGH);
                  break;
             case 2:
                  setMotorPins(motor, LOW, LOW, HIGH, LOW);
                  break;
             case 3:
                  setMotorPins(motor, LOW, HIGH, HIGH, LOW);
                  break;
             case 4:
                  setMotorPins(motor, LOW, HIGH, LOW, LOW);
                  break;
             case 5:
                  setMotorPins(motor, HIGH, HIGH, LOW, LOW);
                  break;
             case 6:
                  setMotorPins(motor, HIGH, LOW, LOW, LOW);
                  break;
             case 7:
                  setMotorPins(motor, HIGH, LOW, LOW, HIGH);
                  break;
             default:
                  setMotorPins(motor, LOW, LOW, LOW, LOW);
                  break;
                  }
        SetDirection();
      }
}

void setMotorPins(Motor motor, bool in1, bool in2, bool in3, bool in4) {
    uint8_t data = 0;
    if (motor.useExtender) {
        // 更新data字节
        bitWrite(data, motor.pins[0], in1);
        bitWrite(data, motor.pins[1], in2);
        bitWrite(data, motor.pins[2], in3);
        bitWrite(data, motor.pins[3], in4);

        // 发送更新后的数据
        Wire.beginTransmission(motor.extenderAddress);
        Wire.write(data);
        byte errorCode = Wire.endTransmission();

        if (errorCode == 0) {
            Serial.println("Data sent successfully");
        } else {
            Serial.print("Error in I2C transmission: ");
            Serial.println(errorCode);
        };

    } else {
        // 直接控制Arduino引脚
        digitalWrite(motor.pins[0], in1);
        digitalWrite(motor.pins[1], in2);
        digitalWrite(motor.pins[2], in3);
        digitalWrite(motor.pins[3], in4);
    }
}

void SetDirection() {
    // Rotate clockwise
    if (Direction == 1) {
        Steps++;
    }
    // Loop in the step case
    if (Steps > 7) {
        Steps = 0;
    }
}

void rotateToNumber(Motor motor, int number) {
    int stepsToMove = (number - lastInputNumber) * stepsPerDigit;
    if (stepsToMove < 0) {
        stepsToMove += stepsPerRevolution; // Make sure the single rotation direction
    }

    for (int i = 0; i < stepsToMove; i++) {
        stepper(motor,1);
        delayMicroseconds(2000);
    }
}

void resetPosition(Motor motor) {
    delay(2000);
    Serial.println(motor.pins[0]);
    Serial.println("Resetting position...");

    while (digitalRead(motor.hallPin) == HIGH) {
        stepper(motor,1);
        delayMicroseconds(2000);
    }

    Serial.println("Sensor is LOW, position reset.");
    Steps = 0; // Reset the step
}
