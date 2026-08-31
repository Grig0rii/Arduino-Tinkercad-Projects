#include <Servo.h>

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x20, 16,2);
LiquidCrystal_I2C lcd1(0x21, 16,2);
LiquidCrystal_I2C lcd2(0x22, 16,2);
LiquidCrystal_I2C lcd3(0x23, 16,2);

Servo servo;
Servo servo1;

long duration = 0;
long cm = 0;
long duration1 = 0;
long cm1 = 0;
long duration2 = 0;
long cm2 = 0;
long duration3 = 0;
long cm3 = 0;
long duration4 = 0;
long cm4 = 0;
unsigned int num = 0;
unsigned long millisPrev = 0;
unsigned long millisPrev1 = 0;
bool gateOpen = false;
bool gateOpen1 = false;
unsigned int freeSpace = 3;
void setup()
{
  servo.attach(4);
  servo1.attach(7);
  servo.write(0);
  servo1.write(0);
  lcd.init();
  lcd1.init();
  lcd2.init();
  lcd3.init();
  lcd.backlight();
  lcd1.backlight();
  lcd2.backlight();
  lcd3.backlight();
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(1, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(2, HIGH);
  digitalWrite(1, HIGH);
  lcd1.setCursor(0,0);
  lcd1.print("Free");
  lcd2.setCursor(0,0);
  lcd2.print("Free");
  lcd3.setCursor(0,0);
  lcd3.print("Free");
}
void loop()
{
  lcd.setCursor(0,0);
  lcd.print(num);
  lcd.setCursor(0,1);
  lcd.print("Free places:");
  lcd.print(freeSpace);
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW);
  delayMicroseconds(3);
  digitalWrite(8, HIGH);
  delayMicroseconds(5);
  digitalWrite(8, LOW);
  pinMode(8, INPUT);
  duration = pulseIn(8, HIGH);
  cm = duration / 29 / 2;
  if (cm <= 100 && gateOpen == false)
  {
    servo.write(90);
    gateOpen = true;
    millisPrev = millis();
  }
  if (gateOpen == true)
    {
    if(millis() - millisPrev >= 10000)
    {
    servo.write(0);
    gateOpen = false;
    num ++;
    freeSpace --;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(num);
    lcd.setCursor(0,1);
    lcd.print("Free places:");
    lcd.print(freeSpace);
    }
    }
  pinMode(9, OUTPUT);
  digitalWrite(9, LOW);
  delayMicroseconds(3);
  digitalWrite(9, HIGH);
  delayMicroseconds(5);
  digitalWrite(9, LOW);
  pinMode(9, INPUT);
  duration1 = pulseIn(9, HIGH);
  cm1 = duration1 / 29 / 2;
  if (cm1 <= 100 && gateOpen1 == false)
  {
    servo1.write(90);
    gateOpen1 = true;
    millisPrev1 = millis();
  }
  if (gateOpen1 == true)
    {
    if(millis() - millisPrev1 >= 10000)
    {
    servo1.write(0);
    gateOpen1 = false;
    num --;
    freeSpace ++;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(num);
    lcd.setCursor(0,1);
    lcd.print("Free places:");
    lcd.print(freeSpace);
    }
    }
  pinMode(3, OUTPUT);
  digitalWrite(3, LOW);
  delayMicroseconds(3);
  digitalWrite(3, HIGH);
  delayMicroseconds(5);
  digitalWrite(3, LOW);
  pinMode(3, INPUT);
  duration2 = pulseIn(3, HIGH);
  cm2 = duration2 / 29 / 2;
  if (cm2 <= 100 && digitalRead(10) == LOW)
  {
    lcd1.clear();
    lcd1.setCursor(0,0);
    lcd1.print("Occupied");
    digitalWrite(1, LOW);
    digitalWrite(10, HIGH);
  }
  else if (cm2 > 100 && digitalRead(10) == HIGH)
  {
    lcd1.clear();
    lcd1.setCursor(0,0);
    lcd1.print("Free");
    digitalWrite(1, HIGH);
    digitalWrite(10, LOW);
  }
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  delayMicroseconds(3);
  digitalWrite(5, HIGH);
  delayMicroseconds(5);
  digitalWrite(5, LOW);
  pinMode(5, INPUT);
  duration3 = pulseIn(5, HIGH);
  cm3 = duration3 / 29 / 2;
  if (cm3 <= 100 && digitalRead(11) == LOW)
  {
    lcd2.clear();
    lcd2.setCursor(0,0);
    lcd2.print("Occupied");
    digitalWrite(2, LOW);
    digitalWrite(11, HIGH);
  }
  else if (cm3 > 100 && digitalRead(11) == HIGH)
  {
    lcd2.clear();
    lcd2.setCursor(0,0);
    lcd2.print("Free");
    digitalWrite(2, HIGH);
    digitalWrite(11, LOW);
  }
  pinMode(6, OUTPUT);
  digitalWrite(6, LOW);
  delayMicroseconds(3);
  digitalWrite(6, HIGH);
  delayMicroseconds(5);
  digitalWrite(6, LOW);
  pinMode(6, INPUT);
  duration4 = pulseIn(6, HIGH);
  cm4 = duration4 / 29 / 2;
  if (cm4 <= 100 && digitalRead(12) == LOW)
  {
    lcd3.clear();
    lcd3.setCursor(0,0);
    lcd3.print("Occupied");
    digitalWrite(LED_BUILTIN, LOW);
    digitalWrite(12, HIGH);
  }
  else if (cm4 > 100 && digitalRead(12) == HIGH)
  {
    lcd3.clear();
    lcd3.setCursor(0,0);
    lcd3.print("Free");
    digitalWrite(LED_BUILTIN, HIGH);
    digitalWrite(12, LOW);
  }
}