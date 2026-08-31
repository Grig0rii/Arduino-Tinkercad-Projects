#include <EEPROM.h>

#include <Servo.h>

#include <Keypad.h>

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x20, 16, 2);
LiquidCrystal_I2C lcd1(0x21, 16, 2);

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
int password;
String input = "";
int marker;
int value = 0;
int attempt = 3;
bool alarm = false;
unsigned long PrevMillis = 0;
bool ledState = false;
String newInput = "";
char star = '*';
void setup()
{
EEPROM.get(10, marker);
  if (marker != 888)
{
  password = 8689;
  EEPROM.put(10, 888);
  EEPROM.put(0, password);
}
else 
{
  EEPROM.get(0, password);
}
  lcd.init();
  lcd.backlight();
  lcd1.init();
  lcd1.backlight();
  servo.attach(10);
  servo.write(0);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(12, OUTPUT);
  lcd.setCursor(0,0);
  lcd.print("Enter password:");
  lcd1.setCursor(0,0);
  lcd1.print("Safe");
}

void loop()
{
    if (value == 0 && attempt == 0)
  {
    alarm = true;
  }

  if (alarm)
  {
    if (millis() - PrevMillis >= 500)
    {
      PrevMillis = millis();

      ledState = !ledState;
      digitalWrite(LED_BUILTIN, ledState);

      if (ledState)
      {
        tone(12, 500);
        lcd.setCursor(0, 0);
        lcd.print("ALARM");
        lcd1.setCursor(0, 0);
        lcd1.print("ALARM");
      }
      else
      {
        noTone(12);
        lcd.clear();
        lcd1.clear();
      }
    }
    char alarmKey = keypad.getKey();

    if (alarmKey)
    {
      if (alarmKey == '#')
      {
        if (input.toInt() == password)
        {
          alarm = false;
          attempt = 3;
          input = "";
          ledState = false;

          noTone(12);
          digitalWrite(LED_BUILTIN, LOW);

          lcd.clear();
          lcd.print("Correct");

          lcd1.clear();
          lcd1.print("Safe");

          delay(3000);

          servo.write(90);
          value = 1;

          lcd.clear();
          lcd.print("Change password-");
          lcd.setCursor(0, 1);
          lcd.print("press 1");

          lcd1.clear();
          lcd1.print("For close-");
          lcd1.setCursor(0, 1);
          lcd1.print("press *");
        }
        else
        {
          input = "";

          lcd.clear();
          lcd.print("Wrong!");
        }
      }
      else if (alarmKey == '*')
      {
        input = "";
      }
      else
      {
        input += alarmKey;
      }
    }

    return;
  }
  char key = keypad.getKey();
  if (key)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    tone(12, 500, 200);
    digitalWrite(LED_BUILTIN, LOW);
    if (key == '#' && value == 0)
    {
      if (input.toInt() == password)
      {
        alarm = false;
        attempt = 3;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Correct");
        delay(3000);
        lcd.clear();
        servo.write(90);
        value = 1;
        input = "";
        newInput = "";
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Change password-");
        lcd.setCursor(0,1);
        lcd.print("press 1");
        lcd1.clear();
        lcd1.setCursor(0,0);
        lcd1.print("For close-");
        lcd1.setCursor(0,1);
        lcd1.print("press *");
        input = "";
      }
      else if (input.toInt() != password && attempt > 0)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Incorrect");
        attempt --;
        delay(1000);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Attempts remain:");
        lcd.setCursor(0,1);
        lcd.print(attempt);
        delay(3000);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Enter password:");
        lcd1.clear();
        lcd1.setCursor(0,0);
        lcd1.print("Safe");
        value = 0;
        newInput = "";
        input = "";
      }
    }
    else if (key == '#' && value == 2)
    {
      value = 3;
      lcd.clear();
      lcd1.clear();
      lcd.setCursor(0,0);
      lcd.print("Are u sure?");
      lcd.setCursor(0,1);
      lcd.print("Yes #     No *");
      lcd1.setCursor(0,0);
      lcd1.print("new password is:");
      lcd1.setCursor(0,1);
      lcd1.print(input);
    }
    else if (key == '#' && value == 3)
    {
      password = input.toInt();
      EEPROM.put(0, password);
      lcd.clear();
      lcd1.clear();
      lcd.setCursor(0,0);
      lcd.print("Changed");
      delay(3000);
      servo.write(0);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Enter password:");
      lcd1.clear();
      lcd1.setCursor(0,0);
      lcd1.print("Safe");
      value = 0;
      newInput = "";
      input = "";
    }
    else if (key == '*' && value == 0)
    {
      input = "";
      newInput = "";
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Enter password:");
      lcd1.clear();
      lcd1.setCursor(0,0);
      lcd1.print("Safe");
      value = 0;
      input = "";
    }
    else if (key == '*' && value == 1)
    {
      input = "";
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Close");
      servo.write(0);
      delay(3000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Enter password:");
      lcd1.clear();
      lcd1.setCursor(0,0);
      lcd1.print("Safe");
      value = 0;
      input = "";
    }
    else if (key == '*' && value == 2)
    {
        value = 1;
        input = "";
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Change password-");
        lcd.setCursor(0,1);
        lcd.print("press 1");
        lcd1.clear();
        lcd1.setCursor(0,0);
        lcd1.print("For close-");
        lcd1.setCursor(0,1);
        lcd1.print("press *");
        input = "";
    }
    else if (key == '*' && value == 3)
    {
      input = "";
      newInput = "";
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("New password:");
      lcd1.clear();
      lcd1.setCursor(0,0);
      lcd1.print("For exit-");
      lcd1.setCursor(0,1);
      lcd1.print("press *");
      value = 2;
    }
    else if (value == 0)
    {
      input += key;
      newInput += star;
      lcd.setCursor(0, 1);
      lcd.print(newInput);
    }
    else if (value == 1 && key == '1')
    {
      input = "";
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("New password:");
      lcd1.clear();
      lcd1.setCursor(0,0);
      lcd1.print("For exit-");
      lcd1.setCursor(0,1);
      lcd1.print("press *");
      value = 2;
    }
    else if (value == 2)
    {
      input += key;
      lcd.setCursor(0, 1);
      lcd.print(input);
    }
  }
}