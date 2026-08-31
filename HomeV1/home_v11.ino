#include <Keypad.h>

#include <Servo.h>

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x20,16,2);
LiquidCrystal_I2C lcd1(0x21,16,2);

Servo servo;

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] =
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9,8,7,6};
byte colPins[COLS] = {5,4,3,2};

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

int sensorValue;
int sensorValue1;
int value;
int doorValue;
int venValue;
int doorVal;
unsigned long prevMillis = 0;
unsigned long prevMillis1 = 0;
bool alarm = false;
bool ledState = false;
bool showingWrongScreen = false;
String password = "8689";
String input = "";

void setup()
{
  lcd.init();
  lcd1.init();
  lcd.backlight();
  lcd1.backlight();
  servo.attach(10);
  servo.write(0);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(A0, INPUT_PULLUP);
  pinMode(A1, INPUT_PULLUP);
  pinMode(A2, INPUT_PULLUP);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(1, INPUT);
}
void loop()
{
  if (showingWrongScreen && (millis() - prevMillis1 >= 1000))
  {
    showingWrongScreen = false;
    lcd1.clear();
    value = 0;
    doorValue = 0;
    venValue = 0;
  }
  if (digitalRead(A0) == LOW && alarm == false && (value == 0 || value == 2))
  {
    value = 1;
    lcd1.setCursor(0,0);
    lcd1.print("                  ");
    lcd1.setCursor(0,0);
    lcd1.print("Signal ON");
  }
  else if (digitalRead(A0) == HIGH && alarm == false && (value == 0 || value == 1))
  {
    value = 2;
    lcd1.setCursor(0,0);
    lcd1.print("                  ");
    lcd1.setCursor(0,0);
    lcd1.print("Signal OFF");
  }
  if(digitalRead(A1) == LOW && alarm == false && (doorValue == 0 || doorValue == 2))
  {
    doorValue = 1;
    lcd1.setCursor(0,1);
    lcd1.print("                  ");
    lcd1.setCursor(0,1);
    lcd1.print("Door mode ON");
  }
  else if (digitalRead(A1) == HIGH && alarm == false && (doorValue == 0 || doorValue == 1))
  {
    doorValue = 2;
    lcd1.setCursor(0,1);
    lcd1.print("                  ");
    lcd1.setCursor(0,1);
    lcd1.print("Door mode OFF");
  }
  if(digitalRead(A2) == LOW && alarm == false && (venValue == 0 || venValue == 2))
  {
    venValue = 1;
    lcd.setCursor(0,0);
    lcd.print("                  ");
    lcd.setCursor(0,0);
    lcd.print("Ven mode ON");
  }
  else if (digitalRead(A2) == HIGH && alarm == false && (venValue == 0 || venValue == 1))
  {
    venValue = 2;
    lcd.setCursor(0,0);
    lcd.print("                  ");
    lcd.setCursor(0,0);
    lcd.print("Ven mode OFF");
  }
  sensorValue = digitalRead(1);
  if (sensorValue == HIGH && digitalRead(A0) == LOW && alarm == false)
  {
    alarm = true;
    lcd.clear();
    lcd1.clear();
  }
  if (alarm == true)
  {
    if (millis() - prevMillis >= 400)
    {
      prevMillis = millis();
      ledState = !ledState;
      digitalWrite(LED_BUILTIN, ledState);
      
      if(ledState)
      {
        lcd.setCursor(0,0);
        lcd.print("ALARM");
        tone(12,500,200);
      }
      else
      {
        lcd.clear();
        noTone(12);
      }
    }
  }
  char key = keypad.getKey();
  if(key)
  {
    if (key == '#' && alarm == true)
    {
      if (input == password)
      {
        alarm = false;
        lcd.clear();
        lcd1.setCursor(0,0);
        lcd1.print("Correct");
        noTone(12);
        digitalWrite(LED_BUILTIN, LOW);
        showingWrongScreen = true; 
        prevMillis1 = millis();
        input = "";
      }
      else if (input != password)
      {
          lcd1.setCursor(0,0);
          lcd1.print("WRONG!");
          showingWrongScreen = true; 
          prevMillis1 = millis();
          input = "";
      }
    }
    else if (key == '*' && alarm == true)
    {
      input = "";
    }
    else if (key && alarm == true)
    {
      input += key;
    }
  }
  doorVal = digitalRead(A1);
  long duration;
  long cm;
  pinMode(A3, OUTPUT);
  digitalWrite(A3, LOW);
  delayMicroseconds(3);
  digitalWrite(A3, HIGH);
  delayMicroseconds(5);
  digitalWrite(A3, LOW);
  pinMode(A3, INPUT);
  duration = pulseIn(A3, HIGH);
  cm = duration / 29 / 2;
  if (cm <= 100 && doorVal == LOW)
  {
    servo.write(90);
  }
  else
  {
    servo.write(0);
  }
  if (digitalRead(A2) == LOW)
  {
    digitalWrite(11, HIGH);
  }
  else
  {
    digitalWrite(11, LOW);
  }
}