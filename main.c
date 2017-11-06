#include <Wire.h>
#include <LiquidCrystal_I2C.h>

typedef struct Bar {
  int x = 15;
  int y;
};

LiquidCrystal_I2C lcd(0x3F, 16, 2);

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

  pinMode(2, INPUT_PULLUP);
}

int score = 0;
int highScore = 0;

unsigned long previousMillis = 0;
long gameSpeed = 200;
int gameTick = 0;

Bar bar;
int playerY = 0;
int playerX = 0;

int barX = 15;
int barY = 0;

void loop() {
  handleButton();
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis >= gameSpeed) {
    previousMillis = currentMillis;
    tick();
    gameTick++;
  }
}

void tick() {
  barUpdate(bar);
  updateCrystal();
}

void updateCrystal() {
  lcd.clear();
  lcd.setCursor(barX, barY);
  lcd.print("#");
  lcd.setCursor(playerX, playerY);
  lcd.print(">");
}

void barUpdate(Bar bar) {
  if(barX <= playerX) {
    if(barY == playerY) {
      endGame();
    } else {
      score++;
      if(gameSpeed >= 80) {
        gameSpeed -= 10;
      } /*else if(gameTick % 15 == 0 && playerX < 9) {
        playerX++;
      }*/
    }
    
    barX = 15;
    barY = random(2);
  } else {
    barX--;
  }
}

void endGame() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Game Over");
  delay(3000);
  
  if(score > highScore) {
    highScore = score;
  }
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Score ");
  lcd.setCursor(6, 0);
  lcd.print(score);
  lcd.setCursor(0, 1);
  lcd.print("High Score ");
  lcd.setCursor(11, 1);
  lcd.print(highScore);
  delay(5000);

  score = 0;
  gameSpeed = 200;
  playerX = 0;
}

int buttonStatus = 1;

void handleButton() {
  int value = digitalRead(2);
  
  if(value != buttonStatus) {
    /*Serial.print("value: ");
    Serial.println(value);*/
    buttonStatus = value;

    if(!value) {
      playerY = playerY == 1 ? 0 : 1;
      updateCrystal();
    }
  }
}
