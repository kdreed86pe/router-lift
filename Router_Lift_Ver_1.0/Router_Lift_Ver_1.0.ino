#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);
float currentPos = 1.267;

/* ReadButton Ver2
    This is the final version
    Reads 4 digits from the keypad
    if "#" is pressed it pads with "0"'s
    coverts input digits to x.xxx format variable
*/
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  delay(250);

  lcd.init();                      // initialize the lcd
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(2, 0);
  lcd.print("Router Lift V1.0");

  Serial.println(currentPos,3);

  //Set pins for input from the keypad
  for (int i = 22; i <= 33; i++) {
    pinMode(i, INPUT_PULLUP);
  }
  //Set pins for input from the buttons
  for (int i = 40; i <= 52; i++) {
    pinMode(i, INPUT_PULLUP);
  }
}
void loop() {
  // put your main code here, to run repeatedly:

  int button;
  int myButton;
  int pressedButton = 0;
  int pressedKey;

  pressedButton = getButton(pressedButton);
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

  lcd.backlight();
  lcd.setCursor(2, 0);
  lcd.print("Router Lift V1.0");
  #lcd.setCursor(0, 1);
  #lcd.print("Pressed button=");
  #lcd.setCursor(18, 1);
  #lcd.print(pressedButton);
  Serial.println("Got a button...");
  delay(250);

  if (pressedButton == 1) {
    moveToDepth(0);
  }
  if (pressedButton == 2) {
    moveToPark();
  }
  if (pressedButton == 3) {
    moveUpByStep();
  }
  if (pressedButton == 4) {
    moveDownByStep();
  }
  if (pressedButton == 5) {
    moveUp();
  }
  if (pressedButton == 6) {
    moveUp();
  }
  if (pressedButton == 7) {
    bitChange();
  }
  if (pressedButton == 8) {
    zeroSet();
  }
  if (pressedButton == 9) {
    memorySave();
  }
  if (pressedButton == 10) {
    memoryRecall();
  }
  if (pressedButton == 11) {
    cutMortise();
  }
}

float moveToPark() {
  lcd.init();
  lcd.setCursor(2, 0);
  lcd.print("Router Lift V1.0");
  lcd.setCursor(0, 2);
  lcd.print("Move down to park");
  Serial.println("Park....");
  lcd.setCursor(0, 3);
  lcd.print("Current Pos = ");
  lcd.setCursor(15, 3);
  lcd.print(currentPos, 3);
  delay(1000);
}

float moveUpByStep() {
  lcd.init();
  lcd.setCursor(2, 0);
  lcd.print("Router Lift V1.0");
  lcd.setCursor(3, 2);
  lcd.print("Move up 0.001");
  Serial.println("Move up 0.001");
  lcd.setCursor(0, 3);
  lcd.print("Current Pos = ");
  lcd.setCursor(15, 3);
  lcd.print(currentPos, 3);
  delay(1000);
}

float moveDownByStep() {
  lcd.init();
  lcd.setCursor(2, 0);
  lcd.print("Router Lift V1.0");
  lcd.setCursor(0, 2);
  lcd.print("Move down 0.001");
  Serial.println("Move down 0.001");
  lcd.setCursor(0, 3);
  lcd.print("Current Pos = ");
  lcd.setCursor(15, 3);
  lcd.print(currentPos, 3);
  delay(1000);
}

float moveUp() {
  lcd.init();
  lcd.setCursor(2, 0);
  lcd.print("Router Lift V1.0");
  lcd.setCursor(0, 2);
  lcd.print("Move up");
  Serial.println("Move up....");
  lcd.setCursor(0, 3);
  lcd.print("Current Pos = ");
  lcd.setCursor(15, 3);
  lcd.print(currentPos, 3);
  delay(1000);
}

float moveDown() {
  lcd.init();
  lcd.setCursor(2, 0);
  lcd.print("Router Lift V1.0");
  lcd.setCursor(0, 2);
  lcd.print("Move down");
  Serial.println("Move down...");
  lcd.setCursor(0, 3);
  lcd.print("Current Pos = ");
  lcd.setCursor(15, 3);
  lcd.print(currentPos, 3);
  delay(1000);
}

float bitChange() {
  lcd.init();
  lcd.setCursor(2, 0);
  lcd.print("Router Lift V1.0");
  lcd.setCursor(0, 2);
  lcd.print("Move up to chg bit");
  Serial.println("Bit change");
  lcd.setCursor(0, 3);
  lcd.print("Current Pos = ");
  lcd.setCursor(15, 3);
  lcd.print(currentPos, 3);
  delay(1000);
}

float zeroSet() {
  lcd.init();
  lcd.setCursor(2, 0);
  lcd.print("Router Lift V1.0");
  lcd.setCursor(0, 2);
  lcd.print("Move up to zero");
  Serial.println("Zero set");
  lcd.setCursor(0, 3);
  lcd.print("Current Pos = ");
  lcd.setCursor(15, 3);
  lcd.print(currentPos, 3);
  delay(1000);
}

float memorySave() {
  lcd.init();
  lcd.setCursor(2, 0);
  lcd.print("Router Lift V1.0");
  lcd.setCursor(0, 2);
  lcd.print("Save to memory");
  Serial.println("Memory Save");
  lcd.setCursor(0, 3);
  lcd.print("Current Pos = ");
  lcd.setCursor(15, 3);
  lcd.print(currentPos, 3);
  delay(1000);
}

float memoryRecall() {
  lcd.init();
  lcd.setCursor(2, 0);
  lcd.print("Router Lift V1.0");
  lcd.setCursor(0, 2);
  lcd.print("Recall from memory");
  Serial.println("Memory Recall");
  lcd.setCursor(0, 3);
  lcd.print("Current Pos = ");
  lcd.setCursor(15, 3);
  lcd.print(currentPos, 3);
  delay(1000);
}

float cutMortise() {
  lcd.init();
  lcd.setCursor(2, 0);
  lcd.print("Router Lift V1.0");
  lcd.setCursor(0, 2);
  lcd.print("Cut a mortise");
  Serial.println("Cut a mortise....");
  lcd.setCursor(0, 3);
  lcd.print("Current Pos = ");
  lcd.setCursor(15, 3);
  lcd.print(currentPos, 3);
  delay(1000);
}
float moveToDepth(int pressedKey) {
  lcd.init();
  float inch;
  float TargetDepth;
  float inputKey[4] {0, 0, 0, 0};
  lcd.setCursor(2, 0);
  lcd.print("Router Lift V1.0");
  lcd.setCursor(0, 2);
  lcd.print("Enter the depth:");
  Serial.print("Enter depth...");
  lcd.setCursor(0, 3);
  lcd.print("Current Pos = ");
  lcd.setCursor(15, 3);
  lcd.print(currentPos, 3);
  for (int i = 0; i <= 3; i++) {
    pressedKey = getKey(pressedKey);
    inputKey[i] = pressedKey;
    inch = inputKey[i];
    if (inch == 99) {
      inputKey[i] = 0;
      break;
    }
   
    delay(250);
  }
  TargetDepth = inputKey[0] + inputKey[1] / 10 + inputKey[2] / 100 + inputKey[3] / 1000;
  lcd.init();
  lcd.setCursor(2, 0);
  lcd.print("Router Lift V1.0");
  lcd.setCursor(0, 2);
  lcd.print("Target Depth = ");
  lcd.setCursor(15, 2);
  lcd.print(TargetDepth, 3);
  Serial.println("Calculated depth...");
  lcd.setCursor(0, 3);
  lcd.print("Current Pos = ");
  lcd.setCursor(15, 3);
  lcd.print(currentPos, 3);
}

int getButton(int pressedButton) {
  int readButton;
  int gotButtonPress = 0;
  pressedButton = 0;

  while (gotButtonPress == 0) {
    for (int i = 0; i <= 11; i++) {
      readButton = digitalRead(i + 40);
      if (readButton == 0) {
        pressedButton = i + 1;
        return pressedButton;
      }
    }
  }
}

int getKey(int pressedKey) {
  int readPin;
  int gotKeyPress = 0;
  int pinValue[11] {3, 6, 9, 99, 2, 5, 8, 0, 1, 4, 7};
  pressedKey = 0;

  while (gotKeyPress == 0) {
    for (int i = 0; i <= 10; i++) {
      readPin = digitalRead(i + 22);
      if (readPin == 0) {
        pressedKey = pinValue[i];
        return pressedKey;
      }
    }
  }
}
