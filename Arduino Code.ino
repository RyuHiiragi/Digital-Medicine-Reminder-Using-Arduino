#include <Wire.h>
#include <LiquidCrystal.h>
#include <DS1302.h>

// Define pins for RTC
DS1302 rtc(D2, D3, D4);

// Define pins for LCD
LiquidCrystal lcd(D8, D9, D10, D11, D12, D13);

// Define pins for LEDs and buzzer
#define RED_LED_PIN 6
#define GREEN_LED_PIN 7
#define BUZZER_PIN 5
#define BUTTON_PIN 14

// Alarm times (HH:MM:SS)
const int alarmHour = 8;    // Example: 8 AM
const int alarmMinute = 0;
const int alarmSecond = 0;

bool alarmTriggered = false;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Initialize RTC
  rtc.halt(false);
  rtc.writeProtect(false);

  // Set initial time (optional, comment out after setting)
  // rtc.setDOW(FRIDAY);        // Day of the week
  // rtc.setTime(12, 0, 0);     // Time (HH:MM:SS)
  // rtc.setDate(27, 2, 2025);  // Date (DD:MM:YYYY)

  // Initialize LCD
  lcd.begin(16, 2);
  lcd.print("Medicine Reminder");
  delay(2000);
  lcd.clear();

  // Set pin modes
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
}

void loop() {
  // Get current time from RTC
  int hour = rtc.getTime().hour;
  int minute = rtc.getTime().min;
  int second = rtc.getTime().sec;

  // Check if it's time for the alarm
  if (hour == alarmHour && minute == alarmMinute && second == alarmSecond) {
    triggerAlarm();
  }

  // Check if the button is pressed to acknowledge the alarm
  if (digitalRead(BUTTON_PIN) == HIGH) {
    acknowledgeAlarm();
  }

  // Display current time on LCD
  lcd.setCursor(0, 0);
  lcd.print("Time: ");
  lcd.print(hour);
  lcd.print(":");
  lcd.print(minute);
  lcd.print(":");
  lcd.print(second);

  delay(1000); // Update every second
}

void triggerAlarm() {
  if (!alarmTriggered) {
    lcd.clear();
    lcd.print("Time to take");
    lcd.setCursor(0, 1);
    lcd.print("your medicine!");
    digitalWrite(RED_LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    alarmTriggered = true;
  }
}

void acknowledgeAlarm() {
  lcd.clear();
  lcd.print("Alarm Acknowledged");
  digitalWrite(RED_LED_PIN, LOW);
  digitalWrite(GREEN_LED_PIN, HIGH);
  digitalWrite(BUZZER_PIN, LOW);
  alarmTriggered = false;
  delay(2000);
  lcd.clear();
}
