#include <LiquidCrystal.h>

// LCD: RS=7, EN=8, D4=9, D5=10, D6=11, D7=12
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

#define JOY_X A0
#define JOY_Y A1
#define BTN_PIN 2  // External button connected to D2

const int width = 20;
const int height = 4;

int snakeX[80];  // max 80 cells
int snakeY[80];
int snakeLength = 3;

int dirX = 1;  // Start moving right
int dirY = 0;

int appleX, appleY;
bool gameRunning = false;
unsigned long lastMoveTime = 0;
const int moveDelay = 400;

void setup() {
  pinMode(BTN_PIN, INPUT_PULLUP); // External button with internal pull-up
  lcd.begin(20, 4);
  randomSeed(analogRead(A2));  // Seed random from unused pin
  showSplash();
}

void loop() {
  if (!gameRunning) {
    if (digitalRead(BTN_PIN) == LOW) {
      startGame();
    }
    return;
  }

  readJoystick();
  if (millis() - lastMoveTime > moveDelay) {
    moveSnake();
    lastMoveTime = millis();
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
  dirX = 1;
  dirY = 0;

  // Start from middle
  snakeX[0] = 10; snakeY[0] = 2;
  snakeX[1] = 9;  snakeY[1] = 2;
  snakeX[2] = 8;  snakeY[2] = 2;

  spawnApple();
  drawScreen();
}

void readJoystick() {
  int x = analogRead(JOY_X);
  int y = analogRead(JOY_Y);

  if (x < 400 && dirX == 0) { dirX = -1; dirY = 0; }
  else if (x > 600 && dirX == 0) { dirX = 1; dirY = 0; }
  else if (y < 400 && dirY == 0) { dirX = 0; dirY = -1; }
  else if (y > 600 && dirY == 0) { dirX = 0; dirY = 1; }
}

void moveSnake() {
  // Shift body
  for (int i = snakeLength - 1; i > 0; i--) {
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
  }

  // Move head
  snakeX[0] += dirX;
  snakeY[0] += dirY;

  // Check collision
  if (snakeX[0] < 0 || snakeX[0] >= width || snakeY[0] < 0 || snakeY[0] >= height || checkSelfCollision()) {
    gameOver();
    return;
  }

  // Check apple
  if (snakeX[0] == appleX && snakeY[0] == appleY) {
    if (snakeLength < 80) snakeLength++;
    spawnApple();
  }

  drawScreen();
}

bool checkSelfCollision() {
  for (int i = 1; i < snakeLength; i++) {
    if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) return true;
  }
  return false;
}

void gameOver() {
  gameRunning = false;
  lcd.clear();
  lcd.setCursor(5, 1);
  lcd.print("Game Over!");
  lcd.setCursor(2, 2);
  lcd.print("Press to Restart");
}

void spawnApple() {
  bool valid;
  do {
    valid = true;
    appleX = random(0, width);
    appleY = random(0, height);
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
    lcd.write(i == 0 ? 'O' : 'o');
  }
}
