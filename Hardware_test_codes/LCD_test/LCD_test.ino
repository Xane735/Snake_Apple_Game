#include <LiquidCrystal.h>

// RS, E, D4, D5, D6, D7
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

void setup() {
  lcd.begin(20, 4);  // Set dimensions for 20x4 display
  lcd.print("LCD is working!");
  lcd.setCursor(0, 1);
  lcd.print("Line 2");
  lcd.setCursor(0, 2);
  lcd.print("Line 3");
  lcd.setCursor(0, 3);
  lcd.print("Line 4");
}

void loop() {
  // Nothing here for test
}
