int switchPush = 0;
int button = 0;
void setup()
{
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
}

void loop()
{
switchPush = digitalRead(7);
button = digitalRead(8);
  if (button == HIGH)
  {
    digitalWrite(6, HIGH);
    digitalWrite(5, LOW);
  }
  else
  {
    digitalWrite(6, LOW);
    digitalWrite(5, LOW);
  }
  if (switchPush == HIGH)
  {
    digitalWrite(4, HIGH);
    delay(5000);
    digitalWrite(4, LOW);
    delay(1000);
    digitalWrite(4, HIGH);
    delay(1000);
    digitalWrite(4, LOW);
    delay(1000);
    digitalWrite(4, HIGH);
    delay(1000);
    digitalWrite(4, LOW);
    digitalWrite(3, HIGH);
    delay(3000);
    digitalWrite(3, LOW);
    delay(1000);
    digitalWrite(3, HIGH);
    delay(1000);
    digitalWrite(3, LOW);
    delay(1000);
    digitalWrite(3, HIGH);
    delay(1000);
    digitalWrite(3, LOW);
    digitalWrite(2, HIGH);
    if (button == HIGH && digitalRead(2) == HIGH)
  {
    digitalWrite(5, HIGH);
    digitalWrite(6, LOW);
  }
    delay(5000);
    digitalWrite(2, LOW);
    delay(1000);
    digitalWrite(2, HIGH);
    delay(1000);
    digitalWrite(2, LOW);
    delay(1000);
    digitalWrite(2, HIGH);
    if (digitalRead(5) == HIGH)
  {
    digitalWrite(5, LOW);
    digitalWrite(6, HIGH);
  }
    delay(1000);
    digitalWrite(2, LOW);
  }
  delay(30);
}