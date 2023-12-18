/*
Project Matrix
Student: Ioana-Cristina Ursoiu
Group: 364 (Tuesday 9-12)
*/
#include <LiquidCrystal.h>
#include <LedControl.h>
#include "pitches.h"

// LCD pin assignments
const byte rs = 9, en = 8, d4 = 7, d5 = 6, d6 = 5, d7 = 4;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// LED Matrix pin assignments
const int dinPin = 12, clockPin = 11, loadPin = 10;
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);

// Joystick pin assignments
const int pinSW = 2;
const int pinX = A0;
const int pinY = A1;

// Joystick time logic
unsigned long lastJoystickChangeTime = 0;
bool joystickStateChanged = false;
const int joystickDelay = 800;
unsigned long lastDebounceTime = 0;
const int debounceDelay = 200; // Example debounce delay in milliseconds
unsigned long previousMillis = 0;
unsigned long lastArrowChangeTime = 0;

// Buzzer pin
const int buzzerPin = 13;

// Menu options
String menuItems[] = {"Start Game", "High Score", "Settings", "About", "How to Play"};
int menuIndex = 0;

bool gameStarted = false;
int playerScore = 0;
int playerCombo = 0;
int currentPattern = 0;   // Index of the current pattern
int level = 1;
int introMessageTime = 1500;

// Predefined patterns for the game (up, down, left, right)
const byte patterns[4][8] = {
  {B00011000, B00111100, B01111110, B11111111, B00111100, B00111100, B00111100, B00111100}, // Up
  {B00111100, B00111100, B00111100, B00111100, B11111111, B01111110, B00111100, B00011000}, // Down
  {B00001000, B00001100, B11111110, B11111111, B11111111, B11111110, B00001100, B00001000}, // Left
  {B00010000, B00110000, B01111111, B11111111, B11111111, B01111111, B00110000, B00010000}  // Right // Right
};

// Shape of you (Level 1 - Easy)
const int melody[] = {
  NOTE_CS4, NOTE_E4, NOTE_CS4, NOTE_CS4, NOTE_E4,
  NOTE_CS4, NOTE_CS4, NOTE_E4, NOTE_CS4, NOTE_DS4,
  NOTE_CS4, NOTE_CS4, NOTE_E4, NOTE_CS4,
  NOTE_B3,
  NOTE_CS4, NOTE_E4, NOTE_CS4, NOTE_CS4, NOTE_E4,
  NOTE_CS4, NOTE_DS4, NOTE_CS4, NOTE_E4,
  NOTE_B3,
  NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4,
  NOTE_E4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_FS4, NOTE_GS4, NOTE_GS4,
  NOTE_GS4, NOTE_E4, NOTE_FS4, NOTE_B4, NOTE_GS4, NOTE_GS4, NOTE_GS4, NOTE_GS4, NOTE_GS4,
  NOTE_FS4, NOTE_FS4, NOTE_FS4, NOTE_GS4, NOTE_FS4, NOTE_FS4, NOTE_FS4, NOTE_FS4, NOTE_FS4, NOTE_GS4, NOTE_FS4,
  NOTE_E4, NOTE_CS4, NOTE_CS4, NOTE_GS4, NOTE_GS4,
  NOTE_GS4, NOTE_GS4, NOTE_GS4, NOTE_GS4, NOTE_GS4, NOTE_GS4, NOTE_GS4, NOTE_GS4, NOTE_GS4,
  NOTE_GS4, NOTE_B4, NOTE_GS4, NOTE_GS4, NOTE_FS4, NOTE_FS4, NOTE_E4, NOTE_GS4,
  NOTE_FS4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_B4, NOTE_GS4, NOTE_GS4, NOTE_GS4, NOTE_FS4,
  NOTE_FS4, NOTE_FS4, NOTE_FS4, NOTE_FS4, NOTE_FS4, NOTE_FS4, NOTE_FS4, NOTE_FS4,
  NOTE_E4, NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_CS4,
  NOTE_GS3, NOTE_B3,
  NOTE_CS4, NOTE_CS4, NOTE_FS4, NOTE_GS4, NOTE_E4, NOTE_FS4,
  NOTE_B3,
  NOTE_E4, NOTE_FS4, NOTE_GS4, NOTE_FS4, NOTE_E4, NOTE_CS4, NOTE_E4, NOTE_GS4,
  NOTE_FS4, NOTE_E4, NOTE_FS4, NOTE_CS4, NOTE_B4, NOTE_GS4, NOTE_GS4, NOTE_FS4, NOTE_FS4,
  NOTE_E4, NOTE_CS4, NOTE_E4, NOTE_FS4, NOTE_GS4, NOTE_FS4, NOTE_E4,
  NOTE_FS4, NOTE_E4, NOTE_CS4, NOTE_CS4,
  NOTE_B3,
  NOTE_CS4, NOTE_CS4, NOTE_FS4, NOTE_GS4, NOTE_E4, NOTE_FS4, NOTE_FS4,
  NOTE_B3,
  NOTE_FS4, NOTE_GS4, NOTE_FS4, NOTE_E4, NOTE_CS4, NOTE_E4, NOTE_GS4, NOTE_FS4,
  NOTE_E4, NOTE_FS4, NOTE_CS4, NOTE_B4, NOTE_GS4, NOTE_GS4, NOTE_FS4, NOTE_FS4, NOTE_E4,
  NOTE_CS4, NOTE_B4, NOTE_GS4, NOTE_GS4, NOTE_FS4, NOTE_FS4, NOTE_E4,
  NOTE_CS4, NOTE_CS4,
  NOTE_GS3, NOTE_B3,
  NOTE_E4, NOTE_FS4, NOTE_GS4, NOTE_FS4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_FS4, NOTE_FS4,
  NOTE_E4, NOTE_FS4, NOTE_GS4, NOTE_FS4, NOTE_E4, NOTE_E4, NOTE_E4, NOTE_FS4|
  NOTE_CS4, NOTE_E4, NOTE_FS4, NOTE_GS4, NOTE_CS4,
  NOTE_E4, NOTE_E4, NOTE_FS4, NOTE_FS4, NOTE_E4, NOTE_FS4, NOTE_GS4,
  NOTE_FS4, NOTE_E4, NOTE_E4, NOTE_FS4, NOTE_CS4, NOTE_CS4,
  NOTE_FS4, NOTE_GS4, NOTE_B4, NOTE_GS4, NOTE_FS4, NOTE_E4, NOTE_E4, NOTE_FS4,
  NOTE_E4, NOTE_FS4, NOTE_GS4, NOTE_FS4, NOTE_E4, NOTE_E4, NOTE_FS4,
  NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_E4, NOTE_FS4, NOTE_GS4, NOTE_CS4, NOTE_E4,
  NOTE_FS4, NOTE_E4, NOTE_FS4, NOTE_E4, NOTE_FS4, NOTE_GS4,
  NOTE_FS4, NOTE_E4, NOTE_E4, NOTE_FS4, NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_E4, NOTE_E4,
  NOTE_FS4, NOTE_FS4, NOTE_GS4, NOTE_GS4,
  NOTE_E4, NOTE_FS4, NOTE_GS4, NOTE_FS4, NOTE_E4, NOTE_E4, NOTE_FS4, NOTE_CS4, NOTE_CS4,
  NOTE_CS4, NOTE_E4, NOTE_E4, NOTE_FS4, NOTE_FS4, NOTE_GS4,
  NOTE_GS4, NOTE_E4, NOTE_FS4, NOTE_GS4, NOTE_FS4,
  NOTE_E4, NOTE_E4, NOTE_FS4, NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_E4, NOTE_E4,
  NOTE_FS4, NOTE_FS4, NOTE_GS4, NOTE_GS4,
  NOTE_E4, NOTE_FS4, NOTE_GS4, NOTE_FS4, NOTE_E4, NOTE_E4, NOTE_FS4, NOTE_CS4, NOTE_CS4,
  NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_CS4, NOTE_E4, NOTE_FS4, NOTE_GS4, NOTE_CS4, NOTE_E4, NOTE_FS4,
  NOTE_CS4,
};
int durations[] = {
  2,2,4,2,2,4,2,2,4,2,2,2,2,2,2,2,2,4,2,2,4,2,2,2,2,8,8,4,4,8,8,8,8,8,8,8,2,8,8,4,2,8,8,4,8,8,8,4,4,8,8,8,8,8,4,8,4,8,8,8,8,4,2,8,8,8,8,4,8,8,8,8,8,8,2,8,4,8,8,8,8,8,2,4,8,8,2,4,8,8,8,8,8,8,2,8,8,4,4,4,8,8,2,4,4,2,4,2,2,4,8,8,8,2,2,8,4,8,4,4,4,4,8,8,8,8,2,8,8,4,8,8,4,2,8,4,8,4,4,4,4,2,2,2,2,4,8,8,8,2,2,2,4,8,4,4,4,4,8,8,8,8,2,8,8,4,8,8,4,2,8,8,4,8,8,4,2,2,4,2,8,8,4,8,8,4,8,8,2,8,8,4,8,8,4,8,8,2,8,8,4,4,4,8,8,2,8,8,4,8,8,8,4,8,2,4,4,8,8,4,4,8,2,8,8,4,4,4,8,8,4,8,8,8,8,8,8,4,4,4,4,8,2,8,8,4,8,8,8,4,8,4,4,4,4,4,4,4,2,8,8,4,8,8,8,4,8,4,4,4,4,4,4,4,2,8,8,4,8,8,8,4,8,4,4,4,4,4,4,4,2,8,8,4,8,8,8,4,8,4,8,8,8,8,8,8,4,4,4,4,2
};

int noteIndex = 0;
bool correctMove = false;
bool correctNoteHit = false;
int levelDuration = 1;
bool initializare = false;

void setup() {
  Serial.begin(9600);
  
  // Initialize LCD
  lcd.begin(16, 2);

  // Initialize LED Matrix
  lc.shutdown(0, false);
  lc.setIntensity(0, 2);
  lc.clearDisplay(0);

  // Initialize Buzzer
  pinMode(buzzerPin, OUTPUT);

  // Initialize Joystick
  pinMode(pinX, INPUT);
  pinMode(pinY, INPUT);
  pinMode(pinSW, INPUT_PULLUP);
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.print("Dance Dance");
  lcd.setCursor(3, 1);
  lcd.print("Revolution");
}

void loop() {
  if(millis() >= introMessageTime){
    if (gameStarted == false)
      readJoystick();
    else{
      playSong(melody, durations, sizeof(melody) / sizeof(melody[0]));
      displayDancePattern(levelDuration);
      if(readPlayerInput() == true)
      {
        lc.clearDisplay(0);
        correctNoteHit = true;
      }  
    }
  } 
}

void displayMenu() {
      if(initializare == false){
        lcd.clear();
        initializare = true;
      }
      lcd.setCursor(0, 0);
      lcd.print("> " + menuItems[menuIndex]);
      lcd.setCursor(2, 1);
      lcd.print(menuItems[(menuIndex + 1) % 5]);
}

void readJoystick() {
  int xValue = analogRead(pinX);
  int yValue = analogRead(pinY);
  bool buttonPressed = digitalRead(pinSW) == LOW;
  if (millis() - lastJoystickChangeTime >= joystickDelay) {
    joystickStateChanged = false;
  }
  // Update menu index based on joystick movement
  if (xValue > 900 && yValue > 400 && yValue < 600 && !joystickStateChanged) {
    menuIndex--;
    lcd.clear();
    joystickStateChanged = true;
    lastJoystickChangeTime = millis();
  }
  if (xValue < 100 && yValue > 400 && yValue < 600 && !joystickStateChanged){
    menuIndex++;
    lcd.clear();
    joystickStateChanged = true;
    lastJoystickChangeTime = millis();
  } 

  // Loop around the menu
  if (menuIndex < 0) menuIndex = 4;
  if (menuIndex > 4) menuIndex = 0;

  // Check for button press to select a menu item
  if (buttonPressed && (millis() - lastDebounceTime >= debounceDelay)) {
    lastDebounceTime = millis();
    selectMenuItem(menuIndex);
  }

  displayMenu();
}

void selectMenuItem(int index) {
  switch (index) {
    case 0:
      // Code to start the game
      startGame();
      break;
    case 1:
      // Code to show high scores
      showHighScores();
      break;
    case 2:
      // Code to access settings
      accessSettings();
      break;
    case 3:
      // Code for 'About' information
      showAboutInfo();
      break;
    case 4:
      // Code for 'How to Play' instructions
      showHowToPlay();
      break;
  }
}

// Example implementations of functions called in selectMenuItem
void startGame() {
  // Initialize game variables
  gameStarted = true;
  playerScore = 0;
  playerCombo = 0;
  joystickStateChanged = false;
  lastArrowChangeTime = 0;
  previousMillis = 0;
  noteIndex = 0;

  if(level == 1)
    levelDuration = 10000;
  else if(level == 2)
    levelDuration = 8000;
  else
    levelDuration = 5000;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Score: " + playerScore);
  lcd.setCursor(0, 1);
  lcd.print("Combo: " + playerCombo);
  playSong(melody, durations, sizeof(melody) / sizeof(melody[0]));
  lc.clearDisplay(0);
}

void showHighScores() {
  // Display high scores on the LCD
}

void accessSettings() {
  // Navigate to the settings menu
}

void showAboutInfo() {
  // Display information about the game
}

void showHowToPlay() {
  // Display instructions on how to play the game
}

void displayDancePattern(int noteDuration) {

  if(millis() - lastArrowChangeTime >= noteDuration || !lastArrowChangeTime)
  {
    lc.clearDisplay(0);
    lastArrowChangeTime = millis();
    currentPattern = random(4); // Move to the next pattern
    // Display the current pattern on the LED matrix
    for (int i = 0; i < 8; i++) {
      lc.setRow(0, i, patterns[currentPattern][i]);
    correctNoteHit = false;
  }
  }
}

void playSong(int melody[], int durations[], int notesCount) {
 if (noteIndex < 310) {
    unsigned long noteDuration = 1000 / durations[noteIndex];
    unsigned long noteLengthMillis = noteDuration * 1.30; // Duration to wait before the next note
    if(millis() - previousMillis >= noteDuration || !previousMillis){
      if(previousMillis){
        noteIndex++;
      }
      previousMillis = millis();
      noTone(buzzerPin); // Stop any previous tone
      tone(buzzerPin, melody[noteIndex]); // Start playing the next note
    }
  }
  else if (noteIndex > sizeof(melody) / sizeof(melody[0]))
    nextLevel();
}

bool readPlayerInput() {
  int xValue = analogRead(pinX);
  int yValue = analogRead(pinY);
  // Check if the joystick movement matches the current pattern
  // This is a simplified version. You'll need to adjust it based on your joystick's behavior
  if (millis() - lastJoystickChangeTime >= joystickDelay) {
        joystickStateChanged = false;
    }

  if ((currentPattern == 0 && xValue > 900 && yValue > 400 && yValue < 600) || // Up
      (currentPattern == 1 && xValue < 100 && yValue > 400 && yValue < 600) || // Down
      (currentPattern == 2 && yValue > 900 && xValue > 400 && xValue < 600) || // Left
      (currentPattern == 3 && yValue < 100 && xValue > 400 && xValue < 600) && // Right
      !joystickStateChanged && correctNoteHit == false) { 
    correctMove = true;
  }

  if ((xValue > 900 && yValue > 400 && yValue < 600) || // Up
      (xValue < 100 && yValue > 400 && yValue < 600) || // Down
      (yValue < 100 && xValue > 400 && xValue < 600) || // Left
      (yValue > 900 && xValue > 400 && xValue < 600) && //Right
      !joystickStateChanged) { 
    joystickStateChanged = true;
    lastJoystickChangeTime = millis();
  }

  if (correctMove && joystickStateChanged && correctNoteHit == false && millis() - lastJoystickChangeTime < joystickDelay) {
    playerCombo++;
    playerScore += playerCombo;
    correctMove = false; // Increase score by the combo value
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Score: " + playerScore);
    lcd.setCursor(0, 1);
    lcd.print("Combo: " + playerCombo);
    return true;
  } 
  else if(joystickStateChanged && correctNoteHit == false && millis() - lastJoystickChangeTime < joystickDelay) {
    playerCombo = 0; // Reset combo on incorrect move
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Score: " + playerScore);
    lcd.setCursor(0, 1);
    lcd.print("Combo: " + playerCombo);
    return false;
  }
  return false;
}

void nextLevel() {
  if (playerScore > 10) {
    level++;
    if(level<4)
      startGame();
    else{
      lcd.clear();
      lcd.setCursor(4, 0);
      lcd.print("You win!");
      delay(1000);
      gameStarted = false;
    }
  }
  else{
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("You lost!");
    delay(1000);
  }
}