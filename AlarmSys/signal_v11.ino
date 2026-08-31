#include <Keypad.h>

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x20, 16, 2);
int sensorValue = 0;
bool alarm = false;
unsigned long previousMillis = 0;
bool ledState = false;
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
String password = "8689";
String input = "";
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(10, INPUT);
  pinMode(11, OUTPUT);
  lcd.init();
  lcd.backlight();
}

void loop()
{
  sensorValue = digitalRead(10);
  if (sensorValue == HIGH)
  {
   alarm = true;
  }
  if (alarm == false)
  {
    noTone(11);
    digitalWrite(LED_BUILTIN, LOW);
  }
  if (alarm == true)
  {
    if (millis() - previousMillis >= 400)
{
  previousMillis = millis();

  ledState = !ledState;

  digitalWrite(LED_BUILTIN, ledState);

  if (ledState)
  {
    tone(11, 500);
    lcd.setCursor(0, 0);
    lcd.print("Alarm");
  }
  else
  {
    noTone(11);
    lcd.clear();
  }
}
  }
  char key = keypad.getKey();
  if (key && alarm == true)
  {
    if(key == '#')
    {
    if(password == input)
    {
      alarm = false;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("deactivate");
      input = "";
      delay(3000);
      lcd.clear();
    }
    else
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Wrong");
      input = "";
      delay(1000);
    }
    }
    else if (key == '*')
    {
      input = "";
    }
    else
    {
      input += key;
    }
  }
}