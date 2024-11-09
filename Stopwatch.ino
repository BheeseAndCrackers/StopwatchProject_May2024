#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Define LED and button pins
int LED1 = A0;
int LED2 = A1;
int LED3 = A2;
int LED4 = A3;
int BUT1 = 6; // Start/Resume
int BUT2 = 7; // Stop/Pause
int BUT3 = 8; // Lap
int BUT4 = 9; // Reset

// Variables for stopwatch
unsigned long startTime = 0;
unsigned long pausedTime = 0;
unsigned long lapStartTime = 0; // Time when lap button was last pressed
unsigned long elapsedTime = 0;
unsigned long lastLapTime = 0; // Time elapsed since last lap button press
bool running = false;

void setup() {
  // Initialize pins
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(BUT1, INPUT);
  pinMode(BUT2, INPUT);
  pinMode(BUT3, INPUT);
  pinMode(BUT4, INPUT);

  // Initialize LCD, startup flash
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("STOPWATCH");
  delay(1000);
  lcd.clear();

  // Display initial time on LCD
  lcd.setCursor(6, 0);
  lcd.print("TIME");
  lcd.setCursor(4, 1);
  lcd.print("0:00:000");
}

void updateDisplay(unsigned long currentTime) {
  //math, gross!
  unsigned long milliseconds = currentTime % 1000;
  unsigned long seconds = (currentTime / 1000) % 60;
  unsigned long minutes = (currentTime / 1000) / 60;
  //time label, printing currentTime
  lcd.setCursor(6,0);
  lcd.print("TIME");
  lcd.setCursor(4, 1);
  lcd.print(minutes);
  lcd.print(':');
  if (seconds < 10) lcd.print('0');
  lcd.print(seconds);
  lcd.print(':');
  if (milliseconds < 10) lcd.print("00");
  else if (milliseconds < 100) lcd.print('0');
  lcd.print(milliseconds);
}

void updateLapDisplay(unsigned long lastLapTime) {
  //lap time 'flashes' for 1.5s, limited LCD space.
  lcd.clear();
  unsigned long lapMilliseconds = lastLapTime % 1000;
  unsigned long lapSeconds = (lastLapTime / 1000) % 60;
  unsigned long lapMinutes = (lastLapTime / 1000) / 60;
  //lap label and printing lap time
  lcd.setCursor(6,0);
  lcd.print("LAP");
  lcd.setCursor(4, 1);
  lcd.print(lapMinutes);
  lcd.print(':');
  if (lapSeconds < 10) lcd.print('0');
  lcd.print(lapSeconds);
  lcd.print(':');
  if (lapMilliseconds < 10) lcd.print("00");
  else if (lapMilliseconds < 100) lcd.print('0');
  lcd.print(lapMilliseconds);
  delay(1500);
  lcd.clear();
}

void loop() {
  // Start button pressed
  if (digitalRead(BUT1) == HIGH && !running) {
    startTime = millis();
    running = true;
  }

  // Stop button pressed
  if (digitalRead(BUT2) == HIGH && running) {
    digitalWrite(LED2, HIGH);
    pausedTime += millis() - startTime;
    running = false;
  }

  

  // Lap button pressed
  if (digitalRead(BUT3) == HIGH && running) {
    digitalWrite(LED3, HIGH);
    if (lapStartTime == 0) {
      // First lap press: Calculate time since start
      lastLapTime = millis() - startTime;
    } else {
      // Subsequent lap press: Calculate time since last lap
      lastLapTime = millis() - lapStartTime;
    }
    lapStartTime = millis(); // Update lap start time
    updateLapDisplay(lastLapTime);
    
  }else{
    digitalWrite(LED3, LOW);
  }

  // Reset button pressed
  if (digitalRead(BUT4) == HIGH) {
    digitalWrite(LED4, HIGH);
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    setup();
    //resets vars
    running = false;
    startTime = 0;
    pausedTime = 0;
    lapStartTime = 0;
    elapsedTime = 0;
    lastLapTime = 0;
    lcd.clear();
    lcd.setCursor(6, 0);
    lcd.print("TIME");
    lcd.setCursor(4, 1);
    lcd.print("0:00:000");

  }else {
    digitalWrite(LED4, LOW);
  }

  // Update display when running
  if (running) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    updateDisplay(pausedTime + (millis() - startTime));
  }else{
    digitalWrite(LED1, LOW);
  }
}
