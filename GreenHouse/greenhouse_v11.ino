#include <EEPROM.h>

#include <Servo.h>

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x20, 16, 2);
LiquidCrystal_I2C lcd1(0x21, 16, 2);
LiquidCrystal_I2C lcd2(0x22, 16, 2);
LiquidCrystal_I2C lcd3(0x23, 16, 2);
LiquidCrystal_I2C lcd4(0x24, 16, 2);

Servo servo;
float oldTemperature = 0;
float oldSolid = 0;
int soilDry;
int value = 0;
int startMill = 0;
bool pressBut = false;
bool pressBut1 = false;
int tempValue;
float voltage = 0;
float temperature = 0;
int settingsVal = 0;
float MaxTemp;
float NormTemp;
float MinTemp;
float WetSoil;
float NormSoil;
float DrySoil;
int pageNum = 10;
int OldValue1 = 0;
int OldValue2 = 0;
int OldValue3 = 0;
int OldValue4 = 0;
int OldValue5 = 0;
int OldValue6 = 0;
int marker;
void setup()
{
  EEPROM.get(24, marker);
  if (marker != 888)
  {
    MaxTemp = 25;
    NormTemp = 15;
    MinTemp = 5;
    DrySoil = 0;
    NormSoil = 500;
    WetSoil = 1000;
    EEPROM.put(0, MaxTemp);
    EEPROM.put(4, NormTemp);
    EEPROM.put(8, MinTemp);
    EEPROM.put(12, DrySoil);
    EEPROM.put(16, NormSoil);
    EEPROM.put(20, WetSoil);
    marker = 888;
    EEPROM.put(24, marker);
  }
  else
  {
    EEPROM.get(0, MaxTemp);
    EEPROM.get(4, NormTemp);
    EEPROM.get(8, MinTemp);
    EEPROM.get(12, DrySoil);
    EEPROM.get(16, NormSoil);
    EEPROM.get(20, WetSoil);
  }
  value = 0;
  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(8, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  servo.attach(6);
  servo.write(0);
  lcd.init();
  lcd.backlight();
  lcd1.init();
  lcd1.backlight();
  lcd2.init();
  lcd2.backlight();
  lcd3.init();
  lcd3.backlight();
  lcd4.init();
  lcd4.backlight();
  lcd.setCursor(0,0);
  lcd.print("SMART GREENHOUSE");
  lcd1.setCursor(0,0);
  lcd1.print("loading...");
  startMill = millis();
}

void loop()
{
  float SoilValue = analogRead(A0);
  float SensorValue = analogRead(A1);
  voltage = SensorValue * 5.0 / 1023;
  temperature = (voltage - 0.5) * 100;
  if (millis() - startMill >= 3000 && value == 0)
  {
    lcd.clear();
    lcd1.clear();
    lcd2.clear();
    lcd3.clear();
    lcd4.clear();
    value = 1;
  }
  if (value == 1)
  {
    lcd.setCursor(0,0);
    lcd.print("select mode:");
    lcd1.setCursor(0,0);
    lcd1.print("button1 - MANUAL");
    lcd1.setCursor(0,1);
    lcd1.print("button2 - AUTO");
  }
  if (value == 1)
  {
    lcd4.setCursor(0,0);
    lcd4.print("Press 6 button");
    lcd4.setCursor(0,1);
    lcd4.print("for settings");
  }
  if (value == 1 && digitalRead(11) == LOW)
  {
    value = 4;
    pageNum = 0;
    lcd.clear();
    lcd1.clear();
    lcd2.clear();
    lcd3.clear();
    lcd4.clear();
    lcd.setCursor(0,0);
    lcd.print("Select page");
    lcd.setCursor(0,1);
    lcd.print("By 1 & 2 button");
    lcd4.setCursor(0,0);
    lcd4.print("press 6 button");
    lcd4.setCursor(0,1);
    lcd4.print("for back");
  }
  else if (value == 4 && digitalRead(11) == LOW)
  {
    lcd.clear();
    lcd1.clear();
    lcd2.clear();
    lcd3.clear();
    lcd4.clear();
    value = 1;
    pageNum = 10;
    EEPROM.put(0, MaxTemp);
    EEPROM.put(4, NormTemp);
    EEPROM.put(8, MinTemp);
    EEPROM.put(12, DrySoil);
    EEPROM.put(16, NormSoil);
    EEPROM.put(20, WetSoil);
  }
  
  if (pageNum == 0)
  {
    lcd.setCursor(0,0);
    lcd.print("Select page");
    lcd.setCursor(0,1);
    lcd.print("By 1 & 2 button");
    lcd2.setCursor(0,0);
    lcd2.print("+ & -");
    lcd2.setCursor(0,1);
    lcd2.print("By 3 & 4 button");
    lcd1.setCursor(0,0);
    lcd1.print("Page 0");
    lcd4.setCursor(0,0);
    lcd4.print("press 6 button");
    lcd4.setCursor(0,1);
    lcd4.print("for back");
  }
  if (pageNum == 1)
  {
    lcd.setCursor(0,0);
    lcd.print("Mintemp=");
    if (MinTemp != OldValue1)
    {
      lcd2.clear();
      lcd2.setCursor(0,0);
      lcd2.print(MinTemp);
      OldValue1 = MinTemp;
    }
    else 
    {
      lcd2.setCursor(0,0);
      lcd2.print(MinTemp);
    }
    lcd1.setCursor(0,0);
    lcd1.print("Page 1");
  }
  if (pageNum == 2)
  {
    lcd.setCursor(0,0);
    lcd.print("Normtemp=");
    if (NormTemp != OldValue2)
    {
      lcd2.clear();
      lcd2.setCursor(0,0);
      lcd2.print(NormTemp);
      OldValue2 = NormTemp;
    }
    else 
    {
      lcd2.setCursor(0,0);
      lcd2.print(NormTemp);
    }
    lcd1.setCursor(0,0);
    lcd1.print("Page 2");
  }
  if (pageNum == 3)
  {
    lcd.setCursor(0,0);
    lcd.print("Maxtemp=");
    if (MaxTemp != OldValue3)
    {
      lcd2.clear();
      lcd2.setCursor(0,0);
      lcd2.print(MaxTemp);
      OldValue3 = MaxTemp;
    }
    else 
    {
      lcd2.setCursor(0,0);
      lcd2.print(MaxTemp);
    }
    lcd1.setCursor(0,0);
    lcd1.print("Page 3");
  }
  if (pageNum == 4)
  {
    lcd.setCursor(0,0);
    lcd.print("Drysoil=");
    if (DrySoil != OldValue4)
    {
      lcd2.clear();
      lcd2.setCursor(0,0);
      lcd2.print(DrySoil);
      OldValue4 = DrySoil;
    }
    else 
    {
      lcd2.setCursor(0,0);
      lcd2.print(DrySoil);
    }
    lcd1.setCursor(0,0);
    lcd1.print("Page 4");
  }
  if (pageNum == 5)
  {
    lcd.setCursor(0,0);
    lcd.print("Normsoil=");
    if (NormSoil != OldValue5)
    {
      lcd2.clear();
      lcd2.setCursor(0,0);
      lcd2.print(NormSoil);
      OldValue5 = NormSoil;
    }
    else 
    {
      lcd2.setCursor(0,0);
      lcd2.print(NormSoil);
    }
    lcd1.setCursor(0,0);
    lcd1.print("Page 5");
  }
  if (pageNum == 6)
  {
    lcd.setCursor(0,0);
    lcd.print("Wetsoil=");
    if (WetSoil != OldValue6)
    {
      lcd2.clear();
      lcd2.setCursor(0,0);
      lcd2.print(WetSoil);
      OldValue6 = WetSoil;
    }
    else 
    {
      lcd2.setCursor(0,0);
      lcd2.print(WetSoil);
    }
    lcd1.setCursor(0,0);
    lcd1.print("Page 6");
  }
  
  if (value == 4 && pageNum == 1 && digitalRead(10) == LOW)
  {
    MinTemp --;
  }
  else if (value == 4 && pageNum == 1 && digitalRead(9) == LOW)
  {
    MinTemp ++;
  }
  if (value == 4 && pageNum == 2 && digitalRead(10) == LOW)
  {
    NormTemp --;
  }
  else if (value == 4 && pageNum == 2 && digitalRead(9) == LOW)
  {
    NormTemp ++;
  }
  if (value == 4 && pageNum == 3 && digitalRead(10) == LOW)
  {
    MaxTemp --;
  }
  else if (value == 4 && pageNum == 3 && digitalRead(9) == LOW)
  {
    MaxTemp ++;
  }
  if (value == 4 && pageNum == 4 && digitalRead(10) == LOW && DrySoil>=25)
  {
    DrySoil -= 25;
  }
  else if (value == 4 && pageNum == 4 && digitalRead(9) == LOW && DrySoil < 1000)
  {
    DrySoil += 25;
  }
  if (value == 4 && pageNum == 5 && digitalRead(10) == LOW && NormSoil>= 25)
  {
    NormSoil -= 25;
  }
  else if (value == 4 && pageNum == 5 && digitalRead(9) == LOW && NormSoil < 1000)
  {
    NormSoil += 25;
  }
  if (value == 4 && pageNum == 6 && digitalRead(10) == LOW && WetSoil>=25)
  {
    WetSoil -= 25;
  }
  else if (value == 4 && pageNum == 6 && digitalRead(9) == LOW && WetSoil < 1000)
  {
    WetSoil += 25;
  }
  
  if (value == 4 && digitalRead(8) == LOW && pageNum == 0)
  {
    lcd.clear();
    lcd1.clear();
    lcd2.clear();
    lcd3.clear();
    lcd4.clear();
    pageNum = 6;
  }
  else if (value == 4 && digitalRead(8) == LOW && pageNum != 0)
  {
    lcd.clear();
    lcd1.clear();
    lcd2.clear();
    lcd3.clear();
    lcd4.clear();
    pageNum --;
  }
  if (value == 4 && digitalRead(7) == LOW && pageNum == 6)
  {
    lcd.clear();
    lcd1.clear();
    lcd2.clear();
    lcd3.clear();
    lcd4.clear();
    pageNum = 0;
  }
  else if (value == 4 && digitalRead(7) == LOW && pageNum != 6)
  {
    lcd.clear();
    lcd1.clear();
    lcd2.clear();
    lcd3.clear();
    lcd4.clear();
    pageNum ++;
  }
  if (digitalRead(8) == LOW && value == 1)
  {
    lcd.clear();
    lcd1.clear();
    lcd2.clear();
    lcd3.clear();
    lcd4.clear();
    value = 2;
  }
  if (digitalRead(7) == LOW && value == 1)
  {
    lcd.clear();
    lcd1.clear();
    lcd2.clear();
    lcd3.clear();
    lcd4.clear();
    value = 3;
  }
  if (value == 2 || value == 3)
  {
    if (settingsVal == 0)
    {
      lcd4.clear();
      settingsVal = 1;
    }
    lcd4.setCursor(0,0);
    lcd4.print("press 6 button");
    lcd4.setCursor(0,1);
    lcd4.print("for leave");
  }
  if (digitalRead(11) == LOW && (value == 2 || value == 3))
  {
    lcd.clear();
    lcd1.clear();
    lcd2.clear();
    lcd3.clear();
    lcd4.clear();
    value = 1;
    settingsVal = 0;
  }
  if (temperature <= MinTemp && (value == 2 || value == 3))
  {
   tempValue = 0;
   if (oldTemperature != temperature)
   {
     lcd.clear();
     lcd1.clear();
     lcd.setCursor(0,0);
     lcd.print(temperature);
     lcd1.setCursor(0,0);
     lcd1.print("Cold temp");
     oldTemperature = temperature;
   }
    else
    {
      lcd.setCursor(0,0);
      lcd.print(temperature);
      lcd1.setCursor(0,0);
      lcd1.print("Cold temp");
    }
  }
  else if (temperature <= NormTemp && (value == 2 || value == 3))
  {
    tempValue = 1;
    if (oldTemperature != temperature)
   {
     lcd.clear();
     lcd1.clear();
     lcd.setCursor(0,0);
     lcd.print(temperature);
     lcd1.setCursor(0,0);
     lcd1.print("Normal temp");
     oldTemperature = temperature;
   }
    else
    {
      lcd.setCursor(0,0);
      lcd.print(temperature);
      lcd1.setCursor(0,0);
      lcd1.print("Normal temp");
    }
  }
  else if (temperature <= MaxTemp && (value == 2 || value == 3))
  {
    tempValue = 2;
    if (oldTemperature != temperature)
   {
     lcd.clear();
     lcd1.clear();
     lcd.setCursor(0,0);
     lcd.print(temperature);
     lcd1.setCursor(0,0);
     lcd1.print("Hot temp");
     oldTemperature = temperature;
   }
    else
    {
      lcd.setCursor(0,0);
      lcd.print(temperature);
      lcd1.setCursor(0,0);
      lcd1.print("Hot temp");
    }
  }
  if (SoilValue <= DrySoil && (value == 2 || value == 3))
  {
    if (oldSolid != SoilValue)
   {
     lcd2.clear();
     lcd3.clear();
     lcd2.setCursor(0,0);
     lcd2.print(SoilValue);
     lcd3.setCursor(0,0);
     lcd3.print("Dry soil");
     oldSolid = SoilValue;
     soilDry = 1;
   }
    else
    {
      lcd2.setCursor(0,0);
      lcd2.print(SoilValue);
      lcd3.setCursor(0,0);
      lcd3.print("Dry soil");
    }
  }
  else if (SoilValue <= NormSoil && (value == 2 || value == 3))
  {
    if (oldSolid != SoilValue)
   {
     lcd2.clear();
     lcd3.clear();
     lcd2.setCursor(0,0);
     lcd2.print(SoilValue);
     lcd3.setCursor(0,0);
     lcd3.print("Normal soil");
     oldSolid = SoilValue;
     soilDry = 0;
   }
    else
    {
      lcd2.setCursor(0,0);
      lcd2.print(SoilValue);
      lcd3.setCursor(0,0);
      lcd3.print("Normal soil");
    }
  }
  else if (SoilValue <= WetSoil && (value == 2 || value == 3))
  {
    if (oldSolid != SoilValue)
   {
     lcd2.clear();
     lcd3.clear();
     lcd2.setCursor(0,0);
     lcd2.print(SoilValue);
     lcd3.setCursor(0,0);
     lcd3.print("Wet soil");
     oldSolid = SoilValue;
     soilDry = 0;
   }
    else
    {
      lcd2.setCursor(0,0);
      lcd2.print(SoilValue);
      lcd3.setCursor(0,0);
      lcd3.print("Wet soil");
    }
  }
  if (soilDry == 1 && value == 3 || SoilValue == 0 && value == 3)
  {
    digitalWrite(4, HIGH);
  }
  else 
  {
    digitalWrite(4, LOW);
  }
  if (value == 2 && digitalRead(10) == LOW)
  {
    digitalWrite(4, HIGH);
  }
  else if (value == 2 && digitalRead(10) == HIGH)
  {
    digitalWrite(4, LOW);
  }
  if (tempValue == 0 && value == 3)
  {
    servo.write(0);
    analogWrite(5, 0);
  }
  else if (tempValue == 1 && value == 3)
  {
    servo.write(90);
    analogWrite(5, 0);
  }
  else if (tempValue == 2 && value == 3)
  {
    servo.write(90);
    analogWrite(5, 1023);
  }
  if (value == 2  && digitalRead(9) == LOW && pressBut == false)
  {
    pressBut = true;
    servo.write(90);
  }
  else if (value == 2 && digitalRead(9) == LOW && pressBut == true)
  {
    pressBut = false;
    servo.write(0);
  }
  if (value == 2  && digitalRead(12) == LOW && pressBut1 == false)
  {
    pressBut1 = true;
    analogWrite(5, 1023);
  }
  else if (value == 2 && digitalRead(12) == LOW && pressBut1 == true)
  {
    pressBut1 = false;
    analogWrite(5, 0);
  }
}