#include <EEPROM.h>

#include <Keypad.h>

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x20, 16, 2);
LiquidCrystal_I2C lcd1(0x21, 16, 2);

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

String input = "";
int bestLvl=0;
int lvl=0;
String lvl1 = "132";
String lvl2 = "8364";
String lvl3 = "04826";
int countNum;
int value = 0;
int randomCount;
unsigned long bestReaction = 0;
char randomKey;
unsigned long StartTime = 0;
int attemptsNum=0;
int bestAttempts=0;
int marker = 0;

void setup()
{
  value = 10;
  randomSeed(analogRead(A0)); 
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(12, OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd1.init();
  lcd1.backlight();
  lcd.setCursor(0,0);
  lcd.print("GameCenter");
  lcd.setCursor(0,1);
  lcd.print("Loading...");
  delay(3000);
  value = 0;
  lcd.clear();
  lcd1.clear();
  lcd.setCursor(0,0);
  lcd.print("1-ReactionGame");
  lcd.setCursor(0,1);
  lcd.print("2-GuessNumGame");
  lcd1.setCursor(0,0);
  lcd1.print("3-MemoryGame");
  lcd1.setCursor(0,1);
  lcd1.print("4-BestScores");
  
  EEPROM.get(50, marker);
  if (marker != 12345)
  {
    EEPROM.put(0, bestReaction);
    EEPROM.put(4, bestAttempts);
    EEPROM.put(8, bestLvl);
    marker = 12345;
    EEPROM.put(50, marker);
  }
  else
  {
    EEPROM.get(0, bestReaction);
    EEPROM.get(4, bestAttempts);
    EEPROM.get(8, bestLvl);
  }
}

void loop()
{
  char key = keypad.getKey();
  if (key)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    tone(12, 500, 200);
    digitalWrite(LED_BUILTIN, LOW);
    
    if (key == '1' && value == 0)
    {
      value = 1;
      input = "";
      lcd.clear();
      lcd1.clear();
      lcd.setCursor(0,0);
      lcd.print("get ready...");
      delay(random(1000, 8000));
      randomKey = keys[random(0, ROWS)][random(0, COLS)];
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("PRINT:");
      StartTime = millis();
      lcd.setCursor(6,0);
      lcd.print(randomKey);
    }
    else if (key == randomKey && value == 1)
    {
      unsigned long reaction = millis() - StartTime;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("YOU WIN!");
      lcd.setCursor(0,1);
      lcd.print(reaction);
      lcd.print("ms");
      lcd1.setCursor(0,0);
      lcd1.print("Exit - *");
      if (bestReaction == 0)
      {
        bestReaction = reaction;
        EEPROM.put(0, bestReaction);
      }
      else if (bestReaction > reaction)
      {
        bestReaction = reaction;
        EEPROM.put(0, bestReaction);
      }
      value = 14;
    }
    
    else if (key == '*' && (value == 13 || value == 14 || value == 15 || value == 20))
    {
      attemptsNum = 0;
      input = "";
      value = 0;
      lcd.clear();
      lcd1.clear();
      lcd.setCursor(0,0);
      lcd.print("1-ReactionGame");
      lcd.setCursor(0,1);
      lcd.print("2-GuessNumGame");
      lcd1.setCursor(0,0);
      lcd1.print("3-MemoryGame");
      lcd1.setCursor(0,1);
      lcd1.print("4-BestScores");
    }
    
    else if (key == '2' && value == 0)
    {
      input = "";
      value = 3;
      randomCount = random(1, 101);
      lcd.clear();
      lcd1.clear();
      lcd.setCursor(0,0);
      lcd.print("Write your num");
      lcd.setCursor(0,1);
      lcd.print("and enter #");
    }
    else if(value == 3 && key >= '0' && key <= '9')
    {
      input += key;
      lcd1.setCursor(0,0);
      lcd1.print(input);
    }
    else if (key == '#' && value == 3)
    {
      countNum = input.toInt();
      attemptsNum++;
      if (countNum == randomCount)
      {
        value = 13;
        lcd.clear();
        lcd1.clear();
        lcd.setCursor(0,0);
        lcd.print("YOU WIN!");
        lcd.setCursor(0,1);
        lcd.print("Attempts:");
        lcd.setCursor(9,1);
        lcd.print(attemptsNum);
        lcd1.setCursor(0,0);
        lcd1.print("Exit - *");
        if(bestAttempts == 0 || attemptsNum < bestAttempts)
        {
          bestAttempts = attemptsNum;
          EEPROM.put(4, bestAttempts);
        }
        input = "";
      }
      else if(countNum < randomCount)
      {
        lcd.clear();
        lcd1.clear();
        input = "";
        lcd.setCursor(0,0);
        lcd.print("TOO LOW");
      }
      else if (countNum > randomCount)
      {
        lcd.clear();
        lcd1.clear();
        input = "";
        lcd.setCursor(0,0);
        lcd.print("TOO HIGH");
      }
    }
    
    else if (key == '3' && value == 0)
    {
      value = 5;
      lcd.clear();
      lcd1.clear();
      input = "";
      lcd.setCursor(0,0);
      lcd.print("Memorise");
      lcd.setCursor(0,1);
      lcd.print("132");
      delay(5000);
      input = "";
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("EnterNums:");
      lcd.setCursor(0,1);
      lcd.print(input);
    }
    else if (value == 5 && key >= '0' && key <= '9')
    {
      input += key;
      lcd.setCursor(0,1);
      lcd.print(input);
    }
    else if (value == 6 && key >= '0' && key <= '9')
    {
      input += key;
      lcd.setCursor(0,1);
      lcd.print(input);
    }
    else if (value == 7 && key >= '0' && key <= '9')
    {
      input += key;
      lcd.setCursor(0,1);
      lcd.print(input);
    }
    else if (key == '#' && value == 5)
    {
      if (input == lvl1)
      {
        value = 6;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Correct!");
        lvl = 1;
        if (lvl > bestLvl)
        {
          bestLvl = lvl;
          EEPROM.put(8, bestLvl);
        }
        delay(3000);
        lcd.clear();
        lcd1.clear();
        input = "";
        lcd.setCursor(0,0);
        lcd.print("Memorise");
        lcd.setCursor(0,1);
        lcd.print("8364");
        delay(5000);
        input = "";
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("EnterNums:");
        lcd.setCursor(0,1);
        lcd.print(input);
      }
      else 
      {
        value = 20;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Wrong!");
        lcd1.setCursor(0,0);
        lcd1.print("Exit - *");
      }
    }
    
    else if (key == '#' && value == 6)
    {
      if (input == lvl2)
      {
        value = 7;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Correct!");
        lvl = 2;
        if (lvl > bestLvl)
        {
          bestLvl = lvl;
          EEPROM.put(8, bestLvl);
        }
        delay(3000);
        lcd.clear();
        lcd1.clear();
        input = "";
        lcd.setCursor(0,0);
        lcd.print("Memorise");
        lcd.setCursor(0,1);
        lcd.print("04826");
        delay(5000);
        input = "";
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("EnterNums:");
        lcd.setCursor(0,1);
        lcd.print(input);
      }
      else 
      {
        value = 20;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Wrong!");
        lcd1.setCursor(0,0);
        lcd1.print("Exit - *");
      }
    }
    
    else if(key == '#' && value == 7)
    {
      if (input == lvl3)
      {
        value = 20;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Correct!");
        lvl = 3;
        if (lvl > bestLvl)
        {
          bestLvl = lvl;
          EEPROM.put(8, bestLvl);
        }
        lcd1.setCursor(0,0);
        lcd1.print("ALL LEVEL DONE!");
        lcd1.setCursor(0,1);
        lcd1.print("Exit - *");
      }
      else 
      {
        value = 20;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Wrong!");
        lcd1.setCursor(0,0);
        lcd1.print("Exit - *");
      }
    }

    else if (key == '4' && value == 0)
    {
      value = 20;
      lcd.clear();
      lcd1.clear();
      
      lcd.setCursor(0,0);
      lcd.print("React:");
      lcd.print(bestReaction);
      lcd.print("ms");
      
      lcd.setCursor(0,1);
      lcd.print("Attempts:");
      lcd.print(bestAttempts);
      
      lcd1.setCursor(0,0);
      lcd1.print("MemLvl:");
      lcd1.print(bestLvl);
      
      lcd1.setCursor(0,1);
      lcd1.print("Exit - *");
    }
  }
}
