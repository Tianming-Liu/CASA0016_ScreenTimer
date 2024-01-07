#define IN1 8
#define IN2 9
#define IN3 10
#define IN4 11
#define HALL_SENSOR_PIN 6

const int stepsPerRevolution = 4096; // Step Count Per Revolution
const int stepsPerDigit = stepsPerRevolution / 10; // Step Count Per Number
int Steps = 0;
int lastInputNumber = 0; // Last Number
boolean Direction = true; // Rotate Direction

void setup() {
     Serial.begin(9600);
     pinMode(IN1, OUTPUT);
     pinMode(IN2, OUTPUT);
     pinMode(IN3, OUTPUT);
     pinMode(IN4, OUTPUT);
     pinMode(HALL_SENSOR_PIN, INPUT);

     resetPosition(); // Reset the Angle of the Motor
}
void loop() {
  if (Serial.available()) {
      int number = Serial.parseInt(); // Read the INPUT Number
      if (number >= 1 && number <= 9) {
          rotateToNumber(number);
          lastInputNumber = number;
      }
  }
}

void stepper(int xw) {
     for (int x = 0; x < xw; x++) {
        switch (Steps) {
             case 0:
                  digitalWrite(IN1, LOW);
                  digitalWrite(IN2, LOW);
                  digitalWrite(IN3, LOW);
                  digitalWrite(IN4, HIGH);
                  break;
             case 1:
                  digitalWrite(IN1, LOW);
                  digitalWrite(IN2, LOW);
                  digitalWrite(IN3, HIGH);
                  digitalWrite(IN4, HIGH);
                  break;
             case 2:
                  digitalWrite(IN1, LOW);
                  digitalWrite(IN2, LOW);
                  digitalWrite(IN3, HIGH);
                  digitalWrite(IN4, LOW);
                  break;
             case 3:
                  digitalWrite(IN1, LOW);
                  digitalWrite(IN2, HIGH);
                  digitalWrite(IN3, HIGH);
                  digitalWrite(IN4, LOW);
                  break;
             case 4:
                  digitalWrite(IN1, LOW);
                  digitalWrite(IN2, HIGH);
                  digitalWrite(IN3, LOW);
                  digitalWrite(IN4, LOW);
                  break;
             case 5:
                  digitalWrite(IN1, HIGH);
                  digitalWrite(IN2, HIGH);
                  digitalWrite(IN3, LOW);
                  digitalWrite(IN4, LOW);
                  break;
             case 6:
                  digitalWrite(IN1, HIGH);
                  digitalWrite(IN2, LOW);
                  digitalWrite(IN3, LOW);
                  digitalWrite(IN4, LOW);
                  break;
             case 7:
                  digitalWrite(IN1, HIGH);
                  digitalWrite(IN2, LOW);
                  digitalWrite(IN3, LOW);
                  digitalWrite(IN4, HIGH);
                  break;
             default:
                  digitalWrite(IN1, LOW);
                  digitalWrite(IN2, LOW);
                  digitalWrite(IN3, LOW);
                  digitalWrite(IN4, LOW);
                  break;
                  }
       SetDirection();
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

void rotateToNumber(int number) {
    int stepsToMove = (number - lastInputNumber) * stepsPerDigit;
    if (stepsToMove < 0) {
        stepsToMove += stepsPerRevolution; // Make sure the single rotation direction
    }

    for (int i = 0; i < stepsToMove; i++) {
        stepper(1);
        delayMicroseconds(2000);
    }
}

void resetPosition() {
    delay(2000);
    Serial.println("Resetting position...");

    while (digitalRead(HALL_SENSOR_PIN) == HIGH) {
        stepper(1);
        delayMicroseconds(2000);
    }

    Serial.println("Sensor is LOW, position reset.");
    Steps = 0; // Reset the step
}
