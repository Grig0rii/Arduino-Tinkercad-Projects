#include <Servo.h>

Servo servo;
Servo servo1;
long duration = 0;
long cm = 0;
unsigned long previousMillis = 0;
int sensorValue;
bool alarm = false;
bool buzzerTone = false;
int value = 0;
void setup()
{
  servo.attach(10);
  servo1.attach(11);
  servo.write(0);
  servo1.write(0);
  pinMode(8, INPUT);
  pinMode(9, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
}
void move()
{
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
}
void stop()
{
  digitalWrite(2, LOW);
  digitalWrite(3, LOW);
  digitalWrite(4, LOW);
  digitalWrite(5, LOW);
}
void loop()
{
  pinMode(7, OUTPUT);
  digitalWrite(7, LOW);
  delayMicroseconds(3);
  digitalWrite(7, HIGH);
  delayMicroseconds(5);
  digitalWrite(7, LOW);
  pinMode(7, INPUT);
  duration = pulseIn(7, HIGH);
  cm = duration / 29 / 2;
  
  if (cm <= 100)
  {
    servo.write(90);
    servo1.write(90);
    value = 1;
  }
  else
  {
    servo.write(0);
    servo1.write(0);
    value = 0;
  }
  if (value == 1)
  {
    stop();
  }
  else
  {
    move();
  }
  sensorValue = digitalRead(8);
  if (sensorValue == HIGH)
  {
   value = 1;
   alarm = true;
  }
  if (alarm == false)
  {
    noTone(9);
  }
  if (alarm == true)
  {
    if (millis() - previousMillis >= 400)
  {
    previousMillis = millis();
    buzzerTone = !buzzerTone;
  if (buzzerTone)
  {
    tone(9, 500);
  }
  else
  {
    noTone(9);
  }
}
  }  
}