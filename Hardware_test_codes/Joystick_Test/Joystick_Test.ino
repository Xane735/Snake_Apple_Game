void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP); // SW button with internal pull-up
}

void loop() {
  int x = analogRead(A0);  // Horizontal
  int y = analogRead(A1);  // Vertical
  int sw = digitalRead(2); // Button (LOW when pressed)

  Serial.print("X: ");
  Serial.print(x);
  Serial.print(" | Y: ");
  Serial.print(y);
  Serial.print(" | SW: ");
  Serial.println(sw);

  delay(500);
}
