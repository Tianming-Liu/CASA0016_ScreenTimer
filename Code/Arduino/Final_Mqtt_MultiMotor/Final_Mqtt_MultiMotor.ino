#include <WiFiS3.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <wifiConfig.h> // You need to create this file and define your own SSID and password

#define uint8_t unsigned char
#define PCF8574_ADD (0x40 >> 1)
#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11

// Setting up the WiFi and MQTT
const char* mqtt_server = "mqtt.cetools.org";
const int mqtt_port = 1883;

// Setting up the Stepper Motor
const int stepsPerRevolution = 4096; // Step count of the motor
const int stepsPerDigit = stepsPerRevolution / 10; // Step count of each digit
int Steps = 0;
int lastInputNumbers[3] = {0, 0, 0}; // Store the last input number of each motor
bool Direction = true; // true: clockwise, false: counterclockwise

char receivedValue[50];  // Store the received message
int durationSeconds = -1;     // Store the duration in seconds

WiFiClient wifiClient;
PubSubClient client(wifiClient);

// 电机结构体
struct Motor {
    bool useExtender;        // Use IO Extender or not
    uint8_t extenderAddress; // I/O Extender Address
    uint8_t pins[4];         // Arduino Pin Number or I/O Extender Pin Number
    int hallPin;             // Hall Effect Sensor Pin Number
};

Motor motors[] = {
    {false, 0, {8, 9, 10, 11}, 13},
    {true, PCF8574_ADD, {0, 1, 2, 3}, 12},
    {true, PCF8574_ADD, {4, 5, 6, 7}, 6}
};

void setup() {
    Serial.begin(9600);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(motors[0].hallPin, INPUT);
    pinMode(motors[1].hallPin, INPUT);
    pinMode(motors[2].hallPin, INPUT);

    resetPosition(motors[0]); // Reset the Angle of the Motor
    resetPosition(motors[1]); 
    resetPosition(motors[2]); 

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback(callback);
    reconnectMQTT();
}

void loop() {
    if (!client.connected()) {
        reconnectMQTT();
    }
    client.loop();
}

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");

    // Empty the receivedValue
    memset(receivedValue, 0, sizeof(receivedValue));

    // Copy the payload to receivedValue
    if(length > sizeof(receivedValue) - 1) {
        length = sizeof(receivedValue) - 1;
    }

    // Copy the payload to receivedValue
    strncpy(receivedValue, (char*)payload, length);
    receivedValue[length] = '\0';  // 确保字符串以空字符结束

    // Parse the receivedValue and get the duration in seconds
    if(sscanf(receivedValue, "Total screen usage duration: %d seconds", &durationSeconds) == 1){
        // Parse the duration in seconds to hours and minutes
        int hours = durationSeconds / 3600;
        int minutes = (durationSeconds % 3600) / 60;

        // Split the hours and minutes to digits
        rotateToNumber(2, hours % 10);             // Motor 2 controls the hour's digit
        rotateToNumber(0, minutes / 10);           // Motor 0 controls the minute's tens digit
        rotateToNumber(1, minutes % 10);           // Motor 1 controls the minute's ones digit
    } else {
        // Parse failed, set the durationSeconds to -1
        durationSeconds = -1;
    }
}

void reconnectMQTT() {
    while (!client.connected()) {
        if (client.connect("arduinoClient")) {
            client.subscribe("student/CASA0016/ucfntl0/ScreenTimer");
        } else {
            delay(2000);
        }
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
        // Update the data to be sent
        bitWrite(data, motor.pins[0], in1);
        bitWrite(data, motor.pins[1], in2);
        bitWrite(data, motor.pins[2], in3);
        bitWrite(data, motor.pins[3], in4);

        // Send the data to IO Extender
        Wire.beginTransmission(motor.extenderAddress);
        Wire.write(data);
        Wire.endTransmission();
    } else {
        // Write the data into arduino Pin
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

void rotateToNumber(int motorIndex, int number) {
    // Calculate the step count
    int stepsToMove = (number - lastInputNumbers[motorIndex - 1]) * stepsPerDigit;
    if (stepsToMove < 0) {
        stepsToMove += stepsPerRevolution; // Make sure the rotation direction
    }

    // Rotate the corresponding Motor
    for (int i = 0; i < stepsToMove; i++) {
        stepper(motors[motorIndex - 1], 1);
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