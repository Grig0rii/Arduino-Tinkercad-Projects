void setup()
{
  pinMode(12, OUTPUT);
  pinMode(11, INPUT);
  pinMode(10, INPUT);
  pinMode(9, INPUT);
  pinMode(8, INPUT);
  pinMode(7, INPUT);
  pinMode(6, INPUT);
  pinMode(5, INPUT);
  pinMode(4, INPUT);
  pinMode(3, INPUT);
  pinMode(2, INPUT);
  pinMode(1, INPUT);
}

void loop()
{
  if (digitalRead(11) == HIGH){
    tone(12, 698, 100);
  }
  if (digitalRead(10) == HIGH){
    tone(12, 659, 100);
  }
  if (digitalRead(9) == HIGH){
    tone(12, 587, 100);
  }
  if (digitalRead(8) == HIGH){
    tone(12, 523, 100);
  }
  if (digitalRead(7) == HIGH){
    tone(12, 494, 100);
  }
  if (digitalRead(6) == HIGH){
    tone(12, 440, 100);
  }
  if (digitalRead(5) == HIGH){
    tone(12, 392, 100);
  }
  if (digitalRead(4) == HIGH){
    tone(12, 349, 100);
  }
  if (digitalRead(3) == HIGH){
    tone(12, 330, 100);
  }
  if (digitalRead(2) == HIGH){
    tone(12, 294, 100);
  }
  if (digitalRead(1) == HIGH){
    tone(12, 262, 100);
  }
  delay(10);
}