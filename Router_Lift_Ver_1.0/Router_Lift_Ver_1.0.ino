/* Router Lift Version 1.0
    Reads buttons
    Reads 4 digits from the keypad
    if "#" is pressed it pads with "0"'s
    coverts input digits to x.xxx format variable
*/
//I2C Libraries and setup
#include <EEPROM.h>
#include <Wire.h>
#include <math.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);
//AccelStepper Library and setup
#include <AccelStepper.h>
int dt = 500;
String versionNumber = "V1.1";

AccelStepper stepper; // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5

void setup() {
  Serial.begin(9600);
  delay(dt);
  // initialize the lcd
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  int lcdRow = 0;
  String lcdText = "Router Lift ";
  int textPosition = 2;
  writeTextToLcd(lcdRow, lcdText, textPosition);
  lcdText = versionNumber;
  textPosition = 14;
  writeTextToLcd(lcdRow, lcdText, textPosition);

  //set initial max speed and acceleration for the stepper motor
  int motorSpeed = 400;
  int motorAcceleration = 800;
  stepper.setMaxSpeed(motorSpeed);
  stepper.setAcceleration(motorAcceleration);

  //Set pins for input from the keypad
  for (int i = 22; i <= 33; i++) {
    pinMode(i, INPUT_PULLUP);
  }
  //Set pins for input from the buttons
  for (int i = 40; i <= 52; i++) {
    pinMode(i, INPUT_PULLUP);
  }
  //Set pins for input from the limit switches
  for (int i = 34; i <= 35; i++) {
    pinMode(i, INPUT_PULLUP);
  }
}
void loop() {
  // put your main code here, to run repeatedly:
  int lcdRow;
  lcdRow = 1;
  clearLcdRow(lcdRow);
  //lcdRow = 2;
  //clearLcdRow(lcdRow);
  //lcdRow = 3;
  //clearLcdRow(lcdRow);
  int button;
  int myButton;
  int pressedButton = 0;
  int pressedKey;
  int targetSteps;
  bool newData;
  bool runAllowed = false;
  int stepperDirection;
  int moveToLimit = 150; // Change to move to upper and lower limit swtiches
  //Call the getButton subroutine to return which button was pressed
  pressedButton = getButton(pressedButton);

  Serial.print("Got a button...");
  Serial.println(pressedButton);
  delay(dt / 2);
  //Calls to the appropriate subroutine for the returned button
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
    moveUp(pressedButton);
  }
  if (pressedButton == 6) {
    moveDown(pressedButton);
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

// Move the router down to the park position
float moveToPark() {
  int currentSteps = stepper.currentPosition();
  float currentPosition;
  Serial.println("Park....");
  int lcdRow = 3;
  clearLcdRow(lcdRow);
  lcdRow = 2;
  clearLcdRow(lcdRow);
  int textPosition = 2;
  String lcdText = "Move down to park";
  writeTextToLcd(lcdRow, lcdText, textPosition);
  delay(dt * 2);
  int speed = 400;
  int accel = 800;
  int targetSteps = 5000;
  int stepperDirection = -1;
  stepper.setMaxSpeed(speed);
  stepper.setAcceleration(accel);
  //Call moveRouter to move down to Park position
  moveRouter(targetSteps, stepperDirection);
  currentSteps = stepper.currentPosition();
  currentPosition = stepsToInches(currentSteps);
  Serial.print("Button #2:currentSteps= ");
  Serial.println(currentSteps);
  Serial.print("Button #2:currentPosition= ");
  Serial.println(currentPosition);
  //lcd.setCursor(15, 3);
  //lcd.print(currentPos, 3);
  lcdText = "Current Pos=";
  lcdRow = 3;
  clearLcdRow(lcdRow);
  textPosition = 0;
  int dataPosition = 12;
  int dataPrecision = 3;
  float lcdData = currentPosition;
  writeToLcd(lcdRow, lcdText, lcdData, textPosition, dataPosition, dataPrecision);
  lcdRow = 2;
  clearLcdRow(lcdRow);
  lcdText = "Parked";
  textPosition = 6;
  writeTextToLcd(lcdRow, lcdText, textPosition);
}

//Move up by one step
float moveUpByStep() {
  int currentSteps = stepper.currentPosition();
  float currentPosition;
  int lcdRow = 2;
  clearLcdRow(lcdRow);
  String lcdText = "Move up 1 step";
  int textPosition = 3;
  writeTextToLcd(lcdRow, lcdText, textPosition);
  Serial.println("Move up one step");
  Serial.print("Button 3:currentPosition: ");
  Serial.println(currentSteps);
  delay(dt * 2);
  int speed = 20;
  int accel = 0;
  int targetSteps = currentSteps + 1;
  Serial.print("Button 3:targetSteps: ");
  Serial.println(targetSteps);
  int stepperDirection = 1;
  stepper.setMaxSpeed(speed);
  stepper.setAcceleration(accel);
  stepper.moveTo(targetSteps * stepperDirection);
  stepper.run();
  currentSteps = stepper.currentPosition();
  currentPosition = stepsToInches(currentSteps);
  Serial.print("Button 3:currentPosition: ");
  Serial.println(currentSteps);
  lcdText = "current Pos=";
  lcdRow = 3;
  clearLcdRow(lcdRow);
  textPosition = 0;
  int dataPosition = 13;
  int dataPrecision = 3;
  float lcdData = currentPosition;
  writeToLcd(lcdRow, lcdText, lcdData, textPosition, dataPosition, dataPrecision);
  delay(dt * 2);
}

//Move down one step
float moveDownByStep() {
  int currentSteps = stepper.currentPosition();
  float currentPosition;
  int lcdRow = 2;
  clearLcdRow(lcdRow);
  String lcdText = "Move down 1 step";
  int textPosition = 0;
  writeTextToLcd(lcdRow, lcdText, textPosition);
  Serial.println("Button 4:Move down onestep");
  Serial.print("Button 4:currentPosition: ");
  Serial.println(currentSteps);
  lcdText = "current Pos=";
  lcdRow = 3;
  clearLcdRow(lcdRow);
  textPosition = 0;
  int dataPosition = 13;
  int dataPrecision = 3;
  float lcdData = currentPosition;
  writeToLcd(lcdRow, lcdText, lcdData, textPosition, dataPosition, dataPrecision);
  delay(dt * 2);
  int speed = 20;
  int accel = 0;
  int targetSteps = 1;
  int stepperDirection = -1;
  stepper.setMaxSpeed(speed);
  stepper.setAcceleration(accel);
  stepper.moveTo(targetSteps * stepperDirection);
  stepper.run();
  currentSteps = stepper.currentPosition();
  currentPosition = stepsToInches(currentSteps);
  Serial.print("Button 3:currentPosition: ");
  Serial.println(currentSteps);
  lcdText = "current Pos=";
  lcdRow = 3;
  clearLcdRow(lcdRow);
  textPosition = 0;
  dataPosition = 13;
  dataPrecision = 3;
  lcdData = currentPosition;
  writeToLcd(lcdRow, lcdText, lcdData, textPosition, dataPosition, dataPrecision);
  delay(dt * 2);
}

//Move up while button is pushed
float moveUp(int pressedButton) {
  Serial.print("Button 5:pressedButton: ");
  Serial.println(pressedButton);
  int readButton = 0;
  int readPin = 44;
  int currentSteps = stepper.currentPosition();
  float  currentPosition;
  //lcd.init();
  int lcdRow = 2;
  clearLcdRow(lcdRow);
  String lcdText = "Move up...";
  int textPosition = 0;
  writeTextToLcd(lcdRow, lcdText, textPosition);
  Serial.println("Move up....");
  //continue to move while button 5 is pressed
  while (readButton == 0) {
    readButton = digitalRead(readPin);
    stepper.moveTo(currentSteps + 1);
    currentSteps = currentSteps + 1;
    stepper.run();
  }
  currentSteps = stepper.currentPosition();
  currentPosition = stepsToInches(currentSteps);
  lcdText = "Current Pos=";
  lcdRow = 3;
  clearLcdRow(lcdRow);
  textPosition = 0;
  int dataPosition = 13;
  int dataPrecision = 3;
  float lcdData = currentPosition;
  writeToLcd(lcdRow, lcdText, lcdData, textPosition, dataPosition, dataPrecision);
  delay(dt * 2);
}

//Move down while button is pushed
float moveDown(int pressedButton) {
  Serial.print("Button 5:pressedButton: ");
  Serial.println(pressedButton);
  int readButton = 0;
  int readPin = 45;
  int currentSteps = stepper.currentPosition();
  float  currentPosition;
  int lcdRow = 2;
  clearLcdRow(lcdRow);
  String lcdText = "Move down...";
  int textPosition = 0;
  writeTextToLcd(lcdRow, lcdText, textPosition);
  Serial.println("Move down....");
  //continue to move while button 6 is pressed
  while (readButton == 0) {
    readButton = digitalRead(readPin);
    stepper.moveTo(currentSteps - 1);
    currentSteps = currentSteps - 1;
    stepper.run();
  }
  currentSteps = stepper.currentPosition();
  currentPosition = stepsToInches(currentSteps);
  lcdText = "current Pos=";
  lcdRow = 3;
  clearLcdRow(lcdRow);
  textPosition = 0;
  int dataPosition = 13;
  int dataPrecision = 3;
  float lcdData = currentPosition;
  writeToLcd(lcdRow, lcdText, lcdData, textPosition, dataPosition, dataPrecision);
  delay(dt * 2);
}

//Move router up to change bit
float bitChange() {
  int currentSteps = stepper.currentPosition();
  float  currentPosition;
  int lcdRow = 2;
  clearLcdRow(lcdRow);
  String lcdText = "Move up to chg bit";
  int textPosition = 0;
  writeTextToLcd(lcdRow, lcdText, textPosition);
  Serial.println("Bit change");
  lcdText = "current Pos=";
  lcdRow = 3;
  clearLcdRow(lcdRow);
  textPosition = 0;
  int dataPosition = 13;
  int dataPrecision = 3;
  float lcdData = currentPosition;
  writeToLcd(lcdRow, lcdText, lcdData, textPosition, dataPosition, dataPrecision);
  delay(dt * 2);
  int speed = 400;
  int accel = 800;
  int targetSteps = 5000;
  int stepperDirection = 1;
  stepper.setMaxSpeed(speed);
  stepper.setAcceleration(accel);
  //Call moveRouter to move down to Park position
  moveRouter(targetSteps, stepperDirection);
  currentSteps = stepper.currentPosition();
  currentPosition = stepsToInches(currentSteps);
  lcdText = "current Pos=";
  lcdRow = 3;
  clearLcdRow(lcdRow);
  textPosition = 0;
  dataPosition = 13;
  dataPrecision = 3;
  lcdData = currentPosition;
  writeToLcd(lcdRow, lcdText, lcdData, textPosition, dataPosition, dataPrecision);
}

//Move router up until bit touches plate
float zeroSet() {
  int currentSteps = stepper.currentPosition();
  float  currentPosition;
  int lcdRow = 2;
  clearLcdRow(lcdRow);
  String lcdText = "Move up to zero";
  int textPosition = 0;
  writeTextToLcd(lcdRow, lcdText, textPosition);
  Serial.println("Zero set");
  lcdText = "current Pos=";
  lcdRow = 3;
  clearLcdRow(lcdRow);
  textPosition = 0;
  int dataPosition = 13;
  int dataPrecision = 3;
  float lcdData = currentPosition;
  writeToLcd(lcdRow, lcdText, lcdData, textPosition, dataPosition, dataPrecision);
  delay(dt * 2);
  int speed = 400;
  int accel = 800;
  int targetSteps = 5000;
  int stepperDirection = 1;
  stepper.setMaxSpeed(speed);
  stepper.setAcceleration(accel);
  //  Call moveRouter to move up to touch zero plate
  moveRouter(targetSteps, stepperDirection);
  currentSteps = stepper.currentPosition();
  currentPosition = 0;
  stepper.setCurrentPosition(0);
  //lcd.setCursor(15, 3);
  //lcd.print(currentPosition, 3);
  lcdText = "current Pos=";
  lcdRow = 3;
  clearLcdRow(lcdRow);
  textPosition = 0;
  dataPosition = 13;
  dataPrecision = 3;
  lcdData = currentPosition;
  writeToLcd(lcdRow, lcdText, lcdData, textPosition, dataPosition, dataPrecision);
  Serial.print("new current position is: ");
  currentSteps = stepper.currentPosition();
  Serial.println(currentSteps);
}

//Save current position to Memory Location
float memorySave() {
  int currentSteps = stepper.currentPosition();
  float currentPosition;
  int memoryLocation;
  int pressedKey;
  int lcdRow = 2;
  clearLcdRow(lcdRow);
  String lcdText = "Enter number 0-9";
  int textPosition = 0;
  writeTextToLcd(lcdRow, lcdText, textPosition);
  Serial.println("Button 9:Memory Save");
  currentSteps = stepper.currentPosition();
  Serial.print("Button 9:before converting to inches, currentSteps: ");
  Serial.println(currentSteps);
  currentPosition = stepsToInches(currentSteps);
  memoryLocation = getKey(pressedKey);
  if (memoryLocation != 99) {
    //EEPROM.put(memoryLocation, currentSteps);
    Serial.print("Button 9;memory location: ");
    Serial.println(memoryLocation);
    Serial.print("Button 9:after converting to inches, currentSteps: ");
    Serial.println(currentSteps);
    Serial.print("Button 9:currentPosition: ");
    Serial.println(currentPosition);
    clearLcdRow(2);
    lcdRow = 2;
    clearLcdRow(lcdRow);
    lcdText = "Saved to #";
    textPosition = 0;
    writeTextToLcd(lcdRow, lcdText, textPosition);
    int lcdData = memoryLocation;
    int dataPosition = 10;
    int dataPrecision = 0;
    writeDataToLcd(lcdRow, lcdData, dataPosition, dataPrecision);
    lcdText = "current Pos=";
    lcdRow = 3;
    clearLcdRow(lcdRow);
    textPosition = 0;
    dataPosition = 13;
    dataPrecision = 3;
    lcdData = currentPosition;
    writeToLcd(lcdRow, lcdText, lcdData, textPosition, dataPosition, dataPrecision);
    delay(dt * 2);
  }
  else {
    lcdRow = 3;
    clearLcdRow(lcdRow);
    lcdRow = 2;
    clearLcdRow(lcdRow);
    lcdText = "Cancelled Save";
    textPosition = 3;
    writeTextToLcd(lcdRow, lcdText, textPosition);
    delay(dt * 4);
    clearLcdRow(lcdRow);
    lcdRow = 2;
    clearLcdRow(lcdRow);
  }
}

//Recall Memory Location and move route to that position
float memoryRecall() {
  Serial.println("Button 10:Memory Recall");
  int currentSteps;
  int recalledSteps;
  float currentPosition;
  float recalledPosition;
  float targetSteps;
  int memoryLocation;
  int pressedKey;
  int stepperDirection;
  int lcdRow = 2;
  clearLcdRow(lcdRow);
  String lcdText = "Enter number 0-9";
  int textPosition = 0;
  writeTextToLcd(lcdRow, lcdText, textPosition);
  memoryLocation = getKey(pressedKey);
  if (memoryLocation != 99) {
    Serial.print("Button 10;memory location: ");
    Serial.println(memoryLocation);
    //EEPROM.get(memoryLocation, recalledSteps);
    recalledPosition = stepsToInches(recalledSteps);
    currentSteps = stepper.currentPosition();
    currentPosition = stepsToInches(currentSteps);
    Serial.print("Button 10:recalledSteps: ");
    Serial.println(recalledSteps);
    Serial.print("Button 10:recalledPosition: ");
    Serial.println(recalledPosition);
    Serial.print("Button 10:currentSteps: ");
    Serial.println(currentSteps);
    Serial.print("Button 10:currentPosition: ");
    Serial.println(currentPosition);
    if (recalledSteps > currentSteps) {
      stepperDirection = -1;
      targetSteps = recalledSteps - currentSteps * stepperDirection;
    }
    if (currentSteps > recalledSteps) {
      stepperDirection = 1;
      targetSteps = currentSteps - recalledSteps * stepperDirection;
    }
    int speed = 400;
    int accel = 800;
    stepper.setMaxSpeed(speed);
    stepper.setAcceleration(accel);
    moveRouter(targetSteps, stepperDirection);
    currentSteps = stepper.currentPosition();
    float inchesFromSteps = stepsToInches(currentSteps);
    clearLcdRow(2);
    lcdRow = 2;
    clearLcdRow(lcdRow);
    lcdText = "Recalled #";
    textPosition = 0;
    writeTextToLcd(lcdRow, lcdText, textPosition);
    int lcdData = memoryLocation;
    int dataPosition = 10;
    int dataPrecision = 0;
    writeDataToLcd(lcdRow, lcdData, dataPosition, dataPrecision);
    lcdText = "current Pos=";
    lcdRow = 3;
    textPosition = 0;
    dataPosition = 13;
    dataPrecision = 3;
    lcdData = currentPosition;
    writeToLcd(lcdRow, lcdText, lcdData, textPosition, dataPosition, dataPrecision);
    delay(dt * 2);
  }
  else {
    lcdRow = 3;
    clearLcdRow(lcdRow);
    lcdRow = 2;
    clearLcdRow(lcdRow);
    lcdText = "Cancelled Recall";
    textPosition = 2;
    writeTextToLcd(lcdRow, lcdText, textPosition);
    delay(dt * 4);
    clearLcdRow(lcdRow);
    lcdRow = 2;
    clearLcdRow(lcdRow);
  }
}

//Move router up to cut a mortise
float cutMortise() {
  int currentSteps = stepper.currentPosition();
  float currentPosition = stepsToInches(currentSteps);
  int numberOfCuts;
  int pressedKey;
  float targetDepth;
  float inch;
  float cutDepth;
  int targetSteps;
  int stepperDirection = 1;
  float inputKey[4] {0, 0, 0, 0};
  int lcdRow = 2;
  String lcdText = "Enter mortise depth";
  int textPosition = 0;
  writeTextToLcd(lcdRow, lcdText, textPosition);
  Serial.println("Button 11:Enter depth...");
  lcdText = "current Pos=";
  lcdRow = 3;
  clearLcdRow(lcdRow);
  textPosition = 0;
  int dataPosition = 13;
  int dataPrecision = 3;
  float lcdData = currentPosition;
  writeToLcd(lcdRow, lcdText, lcdData, textPosition, dataPosition, dataPrecision);
  //Call the getKey subroutine to get keys pressed on the keypad
  for (int i = 0; i <= 3; i++) {
    pressedKey = getKey(pressedKey);
    inputKey[i] = pressedKey;
    inch = inputKey[i];
    if (inch == 99) {
      inputKey[i] = 0;
      break;
    }
    delay(dt / 2);
  }
  //assemble the depth value from the individual numbers entered on the keypad
  targetDepth = inputKey[0] + inputKey[1] / 10 + inputKey[2] / 100 + inputKey[3] / 1000;
  Serial.print("targetDepth is: ");
  Serial.println(targetDepth, 3);
  currentSteps = stepper.currentPosition();
  currentPosition = stepsToInches(currentSteps);
  lcdText = "Mortise Depth=";
  lcdRow = 2;
  clearLcdRow(lcdRow);
  textPosition = 0;
  dataPosition = 14;
  dataPrecision = 3;
  lcdData = targetDepth;
  writeToLcd(lcdRow, lcdText, lcdData, textPosition, dataPosition, dataPrecision);
  lcdText = "current Pos=";
  lcdRow = 3;
  clearLcdRow(lcdRow);
  textPosition = 0;
  dataPosition = 13;
  dataPrecision = 3;
  lcdData = currentPosition;
  writeToLcd(lcdRow, lcdText, lcdData, textPosition, dataPosition, dataPrecision);
  float maxStep = 0.125;
  int trialNumberOfCuts = round(targetDepth / maxStep);
  Serial.print("Button 11:trialNumberOfCuts= :");
  Serial.println(trialNumberOfCuts);
  float checkDepthOfCut = targetDepth / trialNumberOfCuts;
  if (checkDepthOfCut > maxStep) {
    numberOfCuts = trialNumberOfCuts + 1;
  }
  if (checkDepthOfCut <= maxStep) {
    numberOfCuts = trialNumberOfCuts;
  }
  Serial.println(" ");
  Serial.print("Button 11:numberOfCuts= ");
  Serial.println(numberOfCuts);
  float depthOfCut = targetDepth / numberOfCuts;
  currentSteps = stepper.currentPosition();
  currentPosition = stepsToInches(currentSteps);
  Serial.print("Button 11:maxStep= ");
  Serial.println(maxStep, 3);
  Serial.print("Button 11:targetDepth= ");
  Serial.println(targetDepth, 3);
  Serial.print("Button 11:numberOfCuts= ");
  Serial.println(numberOfCuts);
  Serial.print("Button 11:depthOfCut= ");
  Serial.println(depthOfCut, 3);
  delay(dt * 2);
  Serial.println("going to for loop....");
  for (int i = 0; i <= numberOfCuts - 1; i++) {
    Serial.print("for loop - i= ");
    Serial.println(i);
    lcdRow = 1;
    clearLcdRow(lcdRow);
    clearLcdRow(lcdRow);
    lcdText = "Press pedal";
    textPosition = 4;
    writeTextToLcd(lcdRow, lcdText, textPosition);
    //Serial.println("Button 11:got a Foot Pedal Press!");
    getFootPedalPress();
    cutDepth = currentPosition + depthOfCut;
    targetSteps = inchesToSteps(cutDepth);
    moveRouter(targetSteps, stepperDirection);
    currentSteps = stepper.currentPosition();
    currentPosition = stepsToInches(currentSteps);;
    lcdRow = 2;
    clearLcdRow(lcdRow);
    lcdText = "Step ";
    textPosition = 0;
    writeTextToLcd(lcdRow, lcdText, textPosition);
    lcdData = i + 1;
    dataPosition = 6;
    int dataPrecision = 0;
    writeDataToLcd(lcdRow, lcdData, dataPosition, dataPrecision);
    lcdText = " of ";
    textPosition = 7;
    writeTextToLcd(lcdRow, lcdText, textPosition);
    lcdData = numberOfCuts;
    dataPosition = 11;
    writeDataToLcd(lcdRow, lcdData, dataPosition, dataPrecision);
    lcdText = "current Pos=";
    lcdRow = 3;
    clearLcdRow(lcdRow);
    textPosition = 0;
    dataPosition = 13;
    dataPrecision = 3;
    lcdData = currentPosition;
    writeToLcd(lcdRow, lcdText, lcdData, textPosition, dataPosition, dataPrecision);
    Serial.print("Button 11:currentPosition= ");
    Serial.println(currentPosition);
  }
  currentSteps=stepper.currentPosition();
  stepperDirection = -1;
  moveRouter(currentSteps,stepperDirection);
  Serial.print("Button 11:current pos at end: ");
  Serial.println(stepper.currentPosition());
  lcdRow = 2;
  clearLcdRow(lcdRow);
  lcdRow = 1;
  clearLcdRow(lcdRow);
  lcdText = "Cut another Y/N?";
  textPosition = 2;
  writeTextToLcd(lcdRow, lcdText, textPosition);
  int yesOrNo = getKey(yesOrNo);
  if (yesOrNo == 1) {
    clearLcdRow(lcdRow);
    cutMortise();
  }
  Serial.println("Button 11:Done with Mortise");
  Serial.print("Button 11:currentPosition= ");
  Serial.println(stepper.currentPosition());
}


//Move router to the depth entered on the keypad
float moveToDepth(int pressedKey) {
  float inch;
  int currentSteps = stepper.currentPosition();
  int currentPosition = stepsToInches(currentSteps);
  float targetDepth;
  float inputKey[4] {0, 0, 0, 0};
  int lcdRow = 2;
  clearLcdRow(lcdRow);
  String lcdText = "Enter the depth:";
  int textPosition = 0;
  writeTextToLcd(lcdRow, lcdText, textPosition);
  Serial.println("Button 1:Enter depth...");
  lcdText = "Current Pos=";
  lcdRow = 3;
  clearLcdRow(lcdRow);
  textPosition = 0;
  int dataPosition = 12;
  int dataPrecision = 3;
  float lcdData = currentPosition;
  writeToLcd(lcdRow, lcdText, lcdData, textPosition, dataPosition, dataPrecision);
  //Call the getKey subroutine to get keys pressed on the keypad
  for (int i = 0; i <= 3; i++) {
    pressedKey = getKey(pressedKey);
    inputKey[i] = pressedKey;
    inch = inputKey[i];
    if (inch == 99) {
      inputKey[i] = 0;
      break;
    }
    delay(dt / 2);
  }
  //assemble the depth value from the individual numbers entered on the keypad
  targetDepth = inputKey[0] + inputKey[1] / 10 + inputKey[2] / 100 + inputKey[3] / 1000;
  lcdText = "Target Depth=";
  lcdRow = 2;
  clearLcdRow(lcdRow);
  textPosition = 0;
  dataPosition = 13;
  dataPrecision = 3;
  lcdData = targetDepth;
  writeToLcd(lcdRow, lcdText, lcdData, textPosition, dataPosition, dataPrecision);
  Serial.print("Button #1:Calculated depth: ");
  Serial.println(targetDepth, 3);
  lcdText = "Current Pos=";
  lcdRow = 3;
  //clearLcdRow(lcdRow);
  textPosition = 0;
  dataPosition = 12;
  dataPrecision = 3;
  lcdData = currentPosition;
  writeToLcd(lcdRow, lcdText, lcdData, textPosition, dataPosition, dataPrecision);
  int speed = 400;
  int accel = 800;
  Serial.print("Button #1:currentPosition: ");
  Serial.println(stepper.currentPosition());
  int targetSteps = inchesToSteps(targetDepth);
  int stepperDirection;
  if (targetDepth > currentPosition) {
    stepperDirection = 1;
    targetDepth = targetDepth - currentPosition;
  }
  if (currentPosition > targetDepth) {
    stepperDirection = -1;
    targetDepth = currentPosition - targetDepth * stepperDirection;
  }
  Serial.print("Button #1:targetSteps: ");
  Serial.println(targetSteps);
  Serial.print("Button #1:stepperDirection: ");
  Serial.println(stepperDirection);
  stepper.setMaxSpeed(speed);
  stepper.setAcceleration(accel);
  moveRouter(targetSteps, stepperDirection);
  currentSteps = stepper.currentPosition();
  float inchesFromSteps = stepsToInches(currentSteps);
  Serial.print("Button #1:after moving, currentSteps: ");
  Serial.println(currentSteps);
  Serial.print("Button #1:converted to inches: ");
  Serial.println(inchesFromSteps, 3);
  lcdRow = 3;
  //clearLcdRow(lcdRow);
  lcdData = inchesFromSteps;
  dataPosition = 12;
  dataPrecision = 3;
  writeDataToLcd(lcdRow, lcdData, dataPosition, dataPrecision);
}

//Subroutine to loop until a button is pressed
int getButton(int pressedButton) {
  int readButton;
  int gotButtonPress = 0;
  pressedButton = 0;

  while (gotButtonPress == 0) {
    for (int i = 0; i <= 11; i++) {
      readButton = digitalRead(i + 40);
      if (readButton == 0) {
        pressedButton = i + 1;
        Serial.println("getButton:got a button!");
        return pressedButton;
      }
    }
  }
}

//Subroutine to wait for a foot pedal press
void getFootPedalPress() {
  int readFootPedal = 1;
  int gotFootPedalPress = 0;
  while (gotFootPedalPress == 0) {
    readFootPedal = digitalRead(35);
    if (readFootPedal == 0) {
      delay(dt);
      return;
    }
  }
}

//Subroutine to read data from the keypad
int getKey(int pressedKey) {
  int readPin;
  int gotKeyPress = 0;
  int pinValue[11] {3, 6, 9, 99, 2, 5, 8, 0, 1, 4, 7};
  pressedKey = 0;

  while (gotKeyPress == 0) {
    for (int i = 0; i <= 10; i++) {
      readPin = digitalRead(i + 22);
      if (readPin == 0) {
        //Serial.print("getKey:got a keypress! ");
        //Serial.println(i);
        pressedKey = pinValue[i];
        return pressedKey;
      }
    }
  }
}

//Subroutine to move the router with the stepper motor
int moveRouter(int targetSteps, int stepperDirection) {
  int readButton;
  Serial.print("moveRouter:steps to move: ");
  Serial.println(targetSteps);
  stepper.moveTo(targetSteps * stepperDirection);
  for (int i = 1; i = targetSteps; i++) {
    int currentSteps = stepper.currentPosition();
    int currentPos;
    int distToGo = stepper.distanceToGo();
    //    Serial.println("currentSteps   targetSteps   distToGo");
    //    Serial.print("      ");
    //    Serial.print(currentSteps);
    //    Serial.print("           ");
    //    Serial.print(targetSteps);
    //    Serial.print("          ");
    //    Serial.println(distToGo);
    stepper.run();
    currentSteps = stepper.currentPosition();
    readButton = digitalRead(35);
    if (readButton == 0) {
      Serial.println("moveRouter:Limit Switch Detected");
      break;
    }
    if (currentSteps == targetSteps) {
      currentPos = stepsToInches(currentSteps);
      break;
    }
  }
  Serial.print("moveRouter:finished moving, position is: ");
  Serial.println(stepper.currentPosition());
}

//Subroutine to convert steps to inches
float stepsToInches(int stepsToConvert) {
  float distancePerStep = 0.000625;
  float inchesFromSteps = stepsToConvert * distancePerStep;
  Serial.print("stepsToInches:stepsToConvert: ");
  Serial.println(stepsToConvert);
  Serial.print("stepsToInches:calculated inches: ");
  Serial.println(inchesFromSteps, 3);
  return inchesFromSteps;
}

//Subroutine to convert inches to steps
float inchesToSteps(float inchesToConvert) {
  float distancePerStep = 0.000625;
  float steps = inchesToConvert / distancePerStep;
  int stepsFromInches = (int) steps;
  if (steps - stepsFromInches > 0.5) {
    stepsFromInches = stepsFromInches + 1;
  }
  Serial.print("inchesToSteps:unrounded steps from inches: ");
  Serial.println(steps);
  Serial.print("inchesToSteps:steps from inches: ");
  Serial.println(stepsFromInches);
  return stepsFromInches;
}

//Subroutine to write the lines 1-3 to the lcd
float writeToLcd(int lcdRow, String lcdText, float lcdData, int textPosition, int dataPosition, int dataPrecision) {
  lcd.setCursor(textPosition, lcdRow);
  lcd.print(lcdText);
  lcd.setCursor(dataPosition, lcdRow);
  lcd.print(lcdData, dataPrecision);
}

//Subroutine to add text only to a line
int writeTextToLcd(int lcdRow, String lcdText, int textPosition) {
  lcd.setCursor(textPosition, lcdRow);
  lcd.print(lcdText);
}

//Subroutine to add data only to a line
float writeDataToLcd(int lcdRow, float lcdData, int dataPosition, int dataPrecision) {
  lcd.setCursor(dataPosition, lcdRow);
  lcd.print(lcdData, dataPrecision);
}
//Subroutine to clear a line
int clearLcdRow(int lcdRow) {
  String blankRow = "                    ";
  lcd.setCursor(0, lcdRow);
  lcd.print(blankRow);
}
