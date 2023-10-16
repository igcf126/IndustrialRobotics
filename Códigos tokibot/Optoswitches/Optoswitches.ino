const int H1_PIN = 25;
const int H2_PIN = 24;
const int H3_PIN = 23;

void setup() {
  Serial.begin(9600);
  pinMode(H1_PIN, INPUT_PULLUP);
  pinMode(H2_PIN, INPUT_PULLUP);
  pinMode(H3_PIN, INPUT_PULLUP);
}

void loop() {
  if (digitalRead(H1_PIN) == LOW) {
    Serial.println("H1 activated");
  }
  if (digitalRead(H2_PIN) == LOW) {
    Serial.println("H2 activated");
  }
  if (digitalRead(H3_PIN) == LOW) {
    Serial.println("H3 activated");
  }
  delay(100);
}
