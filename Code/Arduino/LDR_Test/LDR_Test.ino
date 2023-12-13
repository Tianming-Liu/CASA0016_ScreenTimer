// Set the middle port of the slider pin
const int potPin = A0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  int potValue = analogRead(potPin); // Read the pin voltage
  Serial.println(potValue);          // Print to the serial monitor
  delay(500);                        // delay 500 millesecond
}
