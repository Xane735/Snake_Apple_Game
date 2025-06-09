#include <LiquidCrystal.h>

// LCD Pin Configuration: RS=7, EN=8, D4=9, D5=10, D6=11, D7=12
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

// Joystick analog axes and button pin
#define JOY_X A0
#define JOY_Y A1
#define BTN_PIN 2

// Buzzer pin
#define BUZZER_PIN 6

// LCD dimensions
const int width = 20;
const int height = 4;

// Snake position arrays
int snakeX[80]; // Stores X positions of each snake segment
int snakeY[80]; // Stores Y positions of each snake segment
int snakeLength = 3; // Initial length of the snake

// Snake direction vector
int dirX = 1;
int dirY = 0;

// Apple position
int appleX, appleY;

// Game state
bool gameRunning = false;
unsigned long lastMoveTime = 0;
const int moveDelay = 400; // Time between snake movements (ms)

void setup() {
  pinMode(BTN_PIN, INPUT_PULLUP); // Button with internal pull-up
  lcd.begin(20, 4);               // Initialize the 20x4 LCD
  randomSeed(analogRead(A2));    // Seed randomness from floating analog pin
  showSplash();                  // Display welcome screen
}

void loop() {
  // Wait for button press to start the game
  if (!gameRunning) {
    if (digitalRead(BTN_PIN) == LOW) {
      startGame();
    }
    return;
  }

  readJoystick(); // Read user input
  if (millis() - lastMoveTime > moveDelay) {
    moveSnake();               // Update snake position
    lastMoveTime = millis();   // Reset movement timer
  }
}

void showSplash() {
  lcd.clear();
  lcd.setCursor(3, 1);
  lcd.print("SNAKE GAME");
  lcd.setCursor(2, 2);
  lcd.print("Press to Start");
}

void startGame() {
  gameRunning = true;
  snakeLength = 3;
  dirX = 1; dirY = 0; // Start moving right

  // Initialize snake head and body
  snakeX[0] = 10; snakeY[0] = 2;
  snakeX[1] = 9;  snakeY[1] = 2;
  snakeX[2] = 8;  snakeY[2] = 2;

  spawnApple();   // Place apple
  drawScreen();   // Draw initial screen
  beepBuzzer(1);  // Beep: start game
}

void readJoystick() {
  int x = analogRead(JOY_X);
  int y = analogRead(JOY_Y);

  // Change direction based on joystick position, prevent direct reversal
  if (x < 400 && dirX == 0) { dirX = -1; dirY = 0; } // Left
  else if (x > 600 && dirX == 0) { dirX = 1; dirY = 0; } // Right
  else if (y < 400 && dirY == 0) { dirX = 0; dirY = -1; } // Up
  else if (y > 600 && dirY == 0) { dirX = 0; dirY = 1; } // Down
}

void moveSnake() {
  // Move each segment to the previous segment's position
  for (int i = snakeLength - 1; i > 0; i--) {
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
  }

  // Update head position
  snakeX[0] = (snakeX[0] + dirX + width) % width;
  snakeY[0] = (snakeY[0] + dirY + height) % height;

  // Check for collision with self
  if (checkSelfCollision()) {
    gameOver();
    return;
  }

  // Check for apple
  if (snakeX[0] == appleX && snakeY[0] == appleY) {
    if (snakeLength < 80) snakeLength++; // Increase snake length
    spawnApple();  // Reposition apple
    beepBuzzer(2); // Beep: eat apple

    // Optional: level up sound every 5 apples
    if (snakeLength % 5 == 0) {
      beepBuzzer(4);
    }
  }

  drawScreen(); // Refresh LCD with new snake and apple positions
}

bool checkSelfCollision() {
  // Check head against all body segments
  for (int i = 1; i < snakeLength; i++) {
    if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i])
      return true;
  }
  return false;
}

void gameOver() {
  gameRunning = false;
  lcd.clear();

  lcd.setCursor(5, 0);
  lcd.print("GAME OVER");

  lcd.setCursor(3, 1);
  lcd.print("Final Score:");

  lcd.setCursor(9, 2);
  lcd.print(snakeLength); // Display snake length as score

  lcd.setCursor(2, 3);
  lcd.print("Press to Restart");

  beepBuzzer(3); // Beep: game over sound
}

void spawnApple() {
  bool valid;
  do {
    valid = true;
    appleX = random(0, width);
    appleY = random(0, height);

    // Ensure apple doesn't spawn on the snake
    for (int i = 0; i < snakeLength; i++) {
      if (snakeX[i] == appleX && snakeY[i] == appleY) {
        valid = false;
        break;
      }
    }
  } while (!valid);
}

void drawScreen() {
  lcd.clear();

  // Draw apple
  lcd.setCursor(appleX, appleY);
  lcd.write('*');

  // Draw snake
  for (int i = 0; i < snakeLength; i++) {
    lcd.setCursor(snakeX[i], snakeY[i]);
    lcd.write(i == 0 ? 'O' : 'o'); // Head 'O', body 'o'
  }
}

// Passive buzzer sound effects
void beepBuzzer(int type) {
  switch (type) {
    case 1:  // Start game beep
      tone(BUZZER_PIN, 1000, 100);
      delay(120);
      break;

    case 2:  // Apple eaten - two short beeps
      for (int i = 0; i < 2; i++) {
        tone(BUZZER_PIN, 1200, 75);
        delay(100);
      }
      break;

    case 3:  // Game over - long beep sequence
      for (int i = 0; i < 10 && !gameRunning; i++) {
        tone(BUZZER_PIN, 400, 300);
        delay(400);
      }
      break;

    case 4:  // Level up - ascending tones
      tone(BUZZER_PIN, 1500, 100);
      delay(120);
      tone(BUZZER_PIN, 1700, 100);
      delay(120);
      tone(BUZZER_PIN, 1900, 100);
      delay(150);
      break;
  }
}
