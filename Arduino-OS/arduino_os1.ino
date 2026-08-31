#include <EEPROM.h>

#include <LiquidCrystal_I2C.h>

#include <Keypad.h>

LiquidCrystal_I2C lcd(0x20, 16, 2);
LiquidCrystal_I2C lcd1(0x21, 16, 2);
LiquidCrystal_I2C lcd2(0x22, 16, 2);
LiquidCrystal_I2C lcd3(0x23, 16, 2);

#define LOADING 0
#define PASSWORD 1
#define ARDUINOOS 2
#define CALCULATOR 3
#define TIMER 4
#define GAMES 5
#define SETTINGS 6
#define MUSIC 7
#define NOTES 8
#define SYSTEM 9
#define SHUTDOWN 10
#define REACTIONGAME 11
#define GUESSNUMGAME 12
#define GUESS4NUMGAME 13
#define WINREACTION 14
#define WINGUESSNUM 15
#define WINGUESS4NUM 16
#define RECORDES 17
#define SETTINGSPASS 18
#define SETPASS 19
#define BACKLIGHT 20
#define SURE 21
#define ONOFFPASS 22

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
int password;
int hours;
int minutes;
int seconds;
char operation;
int num1;
int num2;
int result;
int amount;
int value;
int timeVal;
bool exitVal = false;
unsigned long timerStart;
unsigned long timerDuration;
unsigned long StartTime;
bool timerRunning = false;
bool alarmRunning = false;
unsigned long alarmStart;
unsigned long millisPrev;
unsigned long melodyPrev = 0;
int melodyNote = 0;
bool melodyRunning = false;
bool loading = true;
int marker = 0;
char randomKey;
int bestReaction = 0;
int bestAttempts = 0;
int best4NumAttempts = 0;
int randomCount;
int countNum;
int attemptsNum = 0;
int attemptsNum4 = 0;
int secret1;
int secret2;
int secret3;
int secret4;
int guess1;
int guess2;
int guess3;
int guess4;
int bulls = 0;
int cows = 0;
bool winingLcd = false;
bool backlightval = true;
bool passw = true;
void setup()
{
  randomSeed(analogRead(A0)); 
  value = LOADING;
  lcd.init();
  lcd1.init();
  lcd2.init();
  lcd3.init();
  EEPROM.get(0, marker);
  if (marker != 12345)
  {
    bestReaction = 0;
    bestAttempts = 0;
    best4NumAttempts = 0;
    password = 8689;
    backlightval = true;
    passw = true;
    
    EEPROM.put(4, bestReaction);
    EEPROM.put(8, bestAttempts);
    EEPROM.put(12, best4NumAttempts);
    EEPROM.put(16, password);
    EEPROM.put(20, backlightval);
    EEPROM.put(24, passw);
    marker = 12345;
    EEPROM.put(0, marker);
  }
  else
  {
    EEPROM.get(4, bestReaction);
    EEPROM.get(8, bestAttempts);
    EEPROM.get(12, best4NumAttempts);
    EEPROM.get(16, password);
    EEPROM.get(20, backlightval);
    EEPROM.get(24, passw);
  }
  if (backlightval == true)
  {
    lcd.backlight();
    lcd1.backlight();
    lcd2.backlight();
    lcd3.backlight();
  }
  else if (backlightval == false)
  {
    lcd.noBacklight();
    lcd1.noBacklight();
    lcd2.noBacklight();
    lcd3.noBacklight();
  }
  lcd.setCursor(0,0);
  lcd.print("Arduino OS");
  lcd.setCursor(0,1);
  lcd.print("Loading...");
  millisPrev = millis();
  lcd1.setCursor(0,0);
  lcd1.print("...");
  lcd2.setCursor(0,0);
  lcd2.print("...");
  lcd3.setCursor(0,0);
  lcd3.print("...");
  pinMode(10, INPUT_PULLUP);
  pinMode(11, OUTPUT);
  pinMode(12, INPUT_PULLUP);
}

void loop()
{
  if (loading == true && millis() - millisPrev >= 5000)
  {
    if (passw == true)
    {
    lcd.clear();
    lcd1.clear();
    lcd2.clear();
    lcd3.clear();
    loading = false;
    lcd.setCursor(0,0);
    lcd.print("Enter password:");
    value = PASSWORD;
    }
    else
    {
      value = ARDUINOOS;
      lcd.clear();
      lcd1.clear();
      lcd2.clear();
      lcd3.clear();
      lcd.setCursor(0,0);
      lcd.print("Calculator-1");
      lcd.setCursor(0,1);
      lcd.print("TIMER-2");
      lcd1.setCursor(0,0);
      lcd1.print("Games-3");
      lcd1.setCursor(0,1);
      lcd1.print("Settings-4");
      lcd2.setCursor(0,0);
      lcd2.print("Music-5");
      lcd2.setCursor(0,1);
      lcd2.print("Notes-6");
      lcd3.setCursor(0,0);
      lcd3.print("System-7");
      lcd3.setCursor(0,1);
      lcd3.print("ShutDown-8");
    }
  }
  if (digitalRead(10) == LOW && exitVal == false && (value == WINREACTION || value == WINGUESSNUM || value == WINGUESS4NUM || value == RECORDES))
  {
    winingLcd = false;
    value = GAMES;
    exitVal = true;
    lcd.clear();
    lcd1.clear();
    lcd2.clear();
    lcd3.clear(); 
    lcd.setCursor(0,0);
    lcd.print("Games");
    lcd1.setCursor(0,0);
    lcd1.print("1-ReactionGame");
    lcd1.setCursor(0,1);
    lcd1.print("2-GuessNumGame");
    lcd2.setCursor(0,0);
    lcd2.print("3-Bulls&Cows");
    lcd2.setCursor(0,1);
    lcd2.print("4-BestScores");
  }
  if (digitalRead(10) == LOW && exitVal == true)
  {
    value = ARDUINOOS;
    exitVal = false;
    lcd.clear();
    lcd1.clear();
    lcd2.clear();
    lcd3.clear();
    lcd.setCursor(0,0);
    lcd.print("Calculator-1");
    lcd.setCursor(0,1);
    lcd.print("TIMER-2");
    lcd1.setCursor(0,0);
    lcd1.print("Games-3");
    lcd1.setCursor(0,1);
    lcd1.print("Settings-4");
    lcd2.setCursor(0,0);
    lcd2.print("Music-5");
    lcd2.setCursor(0,1);
    lcd2.print("Notes-6");
    lcd3.setCursor(0,0);
    lcd3.print("System-7");
    lcd3.setCursor(0,1);
    lcd3.print("ShutDown-8");
  }
  if (digitalRead(10) == LOW && exitVal == false && value == SETTINGSPASS)
  {
    value = SETTINGS;
    exitVal = true;
    lcd.clear();
    lcd1.clear();
    lcd2.clear();
    lcd3.clear();
    lcd.setCursor(0,0);
    lcd.print("Settings");
    lcd1.setCursor(0,0);
    lcd1.print("1-password");
    lcd1.setCursor(0,1);
    lcd1.print("2-backlight");
  }
  if (digitalRead(10) == LOW && exitVal == false && (value == SETPASS || value == ONOFFPASS))
  {
    exitVal = false;
    value = SETTINGSPASS;
    lcd.clear();
    lcd1.clear();
    lcd2.clear();
    lcd3.clear();
    lcd.setCursor(0,0);
    lcd.print("1-setPass");
    lcd.setCursor(0,1);
    lcd.print("2-PassOnOff");
    lcd1.setCursor(0,0);
    lcd1.print("Exit-button2");
  }
  if (digitalRead(10) == LOW && exitVal == false && value == BACKLIGHT)
  {
    value = SETTINGS;
    exitVal = true;
    lcd.clear();
    lcd1.clear();
    lcd2.clear();
    lcd3.clear();
    lcd.setCursor(0,0);
    lcd.print("Settings");
    lcd1.setCursor(0,0);
    lcd1.print("1-password");
    lcd1.setCursor(0,1);
    lcd1.print("2-backlight");
  }
  if (timerRunning)
  {
    if (millis() - timerStart >= timerDuration)
      {
         timerRunning = false;
         alarmRunning = true;
         alarmStart = millis();
         melodyRunning = true;
         melodyPrev = millis();
         melodyNote = 0;
      }
  }
  if (alarmRunning && digitalRead(12)==LOW)
  {
      melodyRunning = false;
      noTone(11);
      alarmRunning = false;
  }
  if (melodyRunning)
{
    if (millis() - melodyPrev >= 300)
    {
        melodyPrev = millis();

        if (melodyNote == 0)
            tone(11, 523);

        else if (melodyNote == 1)
            tone(11, 587);

        else if (melodyNote == 2)
            tone(11, 659);

        else if (melodyNote == 3)
            tone(11, 698);

        else if (melodyNote == 4)
            tone(11, 784);

        else if (melodyNote == 5)
            tone(11, 659);

        else if (melodyNote == 6)
            tone(11, 523);

        melodyNote++;

        if (melodyNote > 6)
            melodyNote = 0;
    }
}
  char key = keypad.getKey();
  if (key)
  {
    if (value == PASSWORD && key == '#' && input.toInt() == password && passw)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("CORRECT");
      input = "";
      delay(2000);
      value = ARDUINOOS;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Calculator-1");
      lcd.setCursor(0,1);
      lcd.print("TIMER-2");
      lcd1.setCursor(0,0);
      lcd1.print("Games-3");
      lcd1.setCursor(0,1);
      lcd1.print("Settings-4");
      lcd2.setCursor(0,0);
      lcd2.print("Music-5");
      lcd2.setCursor(0,1);
      lcd2.print("Notes-6");
      lcd3.setCursor(0,0);
      lcd3.print("System-7");
      lcd3.setCursor(0,1);
      lcd3.print("ShutDown-8");
    }
    else if (value == PASSWORD && key == '#' && input.toInt() != password && passw)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("INCORRECT");
      input = "";
      delay(2000);
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Enter password:");
    }
    else if (value == PASSWORD && key == '*')
    {
      input = "";
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Enter password:");
    }
    else if (value == PASSWORD)
    {
      input += key;
      lcd.setCursor(0,1);
      lcd.print(input);
    }
    if (value == ARDUINOOS && key == '1')
    {
      input = "";
      value = CALCULATOR;
      exitVal = true;
      lcd.clear();
      lcd1.clear();
      lcd2.clear();
      lcd3.clear();
      lcd.setCursor(0,0);
      lcd.print("Calculator");
    }
      else if (value == CALCULATOR && key == '#')
      {
        num2 = input.toInt();
        if (operation == '+')
        {
          result = num1 + num2;
        }
        else if (operation == '-')
        {
          result = num1 - num2;
        }
        else if (operation == '*')
        {
          result = num1 * num2;
        }
        else if (operation == '/')
        {
          result = num1 / num2;
        }
        lcd1.clear();
        lcd1.setCursor(0,0);
        lcd1.print(result);
        input = "";
      }
      else if (value == CALCULATOR && key == '*')
      {
        input = "";
        lcd1.clear();
      }
      else if (value == CALCULATOR && key == 'A')
      {
        num1 = input.toInt();
        input = "";
        operation = '+';
        lcd1.clear();
        lcd1.setCursor(0,0);
        lcd1.print("+");
      }
      else if (value == CALCULATOR && key == 'B')
      {
        num1 = input.toInt();
        input = "";
        operation = '-';
        lcd1.clear();
        lcd1.setCursor(0,0);
        lcd1.print("-");
      }
      else if (value == CALCULATOR && key == 'C')
      {
        num1 = input.toInt();
        input = "";
        operation = '*';
        lcd1.clear();
        lcd1.setCursor(0,0);
        lcd1.print("*");
      }
      else if (value == CALCULATOR && key == 'D')
      {
        num1 = input.toInt();
        input = "";
        operation = '/';
        lcd1.clear();
        lcd1.setCursor(0,0);
        lcd1.print("/");
      }
      else if(value == CALCULATOR && key >= '0' && key <= '9')
      {
        input += key;
        lcd1.clear();
        lcd1.setCursor(0,0);
        lcd1.print(input);
      }
    if (value == ARDUINOOS && key == '2')
    {
      value = TIMER;
      exitVal = true;
      lcd.clear();
      lcd1.clear();
      lcd2.clear();
      lcd3.clear();
      lcd.setCursor(0,0);
      lcd.print("TIMER");
      lcd1.setCursor(0,0);
      lcd1.print("Hours");
      lcd1.setCursor(0,1);
      lcd1.print(hours);
      lcd2.setCursor(0,0);
      lcd2.print("Minutes");
      lcd2.setCursor(0,1);
      lcd2.print(minutes);
      lcd3.setCursor(0,0);
      lcd3.print("Seconds");
      lcd3.setCursor(0,1);
      lcd3.print(seconds);
    }
    else if(value == TIMER && key == '1')
    {
      timeVal = 0;
    }
    else if(value == TIMER && key == '2')
    {
      timeVal = 1;
    }
    else if(value == TIMER && key == '3')
    {
      timeVal = 2;
    }
    else if (value == TIMER && timeVal == 0 && key == 'A' && hours <24)
    {
      hours ++;
      lcd1.clear();
      lcd1.setCursor(0,0);
      lcd1.print("Hours");
      lcd1.setCursor(0,1);
      lcd1.print(hours);
    }
    else if (value == TIMER && timeVal == 1 && key == 'A' && minutes <59)
    {
      minutes ++;
      lcd2.clear();
      lcd2.setCursor(0,0);
      lcd2.print("Minutes");
      lcd2.setCursor(0,1);
      lcd2.print(minutes);
    }
    else if(value == TIMER && timeVal == 2 && key == 'A' && seconds < 59)
    {
      seconds ++;
      lcd3.clear();
      lcd3.setCursor(0,0);
      lcd3.print("Seconds");
      lcd3.setCursor(0,1);
      lcd3.print(seconds);
    }
    else if (value == TIMER && timeVal == 0 && key == 'B' && hours>0)
    {
      hours --;
      lcd1.clear();
      lcd1.setCursor(0,0);
      lcd1.print("Hours");
      lcd1.setCursor(0,1);
      lcd1.print(hours);
    }
    else if (value == TIMER && timeVal == 1 && key == 'B' && minutes>0)
    {
      minutes --;
      lcd2.clear();
      lcd2.setCursor(0,0);
      lcd2.print("Minutes");
      lcd2.setCursor(0,1);
      lcd2.print(minutes);
    }
    else if(value == TIMER && timeVal == 2 && key == 'B' && seconds>0)
    {
      seconds --;
      lcd3.clear();
      lcd3.setCursor(0,0);
      lcd3.print("Seconds");
      lcd3.setCursor(0,1);
      lcd3.print(seconds);
    }
    else if (value == TIMER && key == '#' && !timerRunning)
    {
      lcd.clear();
      lcd1.clear();
      lcd2.clear();
      lcd3.clear();
      lcd.setCursor(0,0);
      lcd.print("Done!");
      lcd1.setCursor(0,0);
      lcd1.print("Press button2");
      lcd1.setCursor(0,1);
      lcd1.print("to leave");
      timerDuration =
          (unsigned long)hours * 3600000UL +
          (unsigned long)minutes * 60000UL +
          (unsigned long)seconds * 1000UL;
      timerStart = millis();
      timerRunning = true;
     }
    if (value == ARDUINOOS && key == '3')
    {
      value = GAMES;
      exitVal = true;
      lcd.clear();
      lcd1.clear();
      lcd2.clear();
      lcd3.clear(); 
      lcd.setCursor(0,0);
      lcd.print("Games");
      lcd1.setCursor(0,0);
      lcd1.print("1-ReactionGame");
      lcd1.setCursor(0,1);
      lcd1.print("2-GuessNumGame");
      lcd2.setCursor(0,0);
      lcd2.print("3-Bulls&Cows");
      lcd2.setCursor(0,1);
      lcd2.print("4-BestScores");
    }
    else if (key == '1' && value == GAMES)
    {
      value = REACTIONGAME;
      input = "";
      lcd.clear();
      lcd1.clear();
      lcd2.clear();
      lcd3.clear();
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
    else if (key == randomKey && value == REACTIONGAME)
    {
      exitVal = false;
      value = WINREACTION;
      unsigned long reaction = millis() - StartTime;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("YOU WIN!");
      lcd.setCursor(0,1);
      lcd.print(reaction);
      lcd.print("ms");
      lcd1.setCursor(0,0);
      lcd1.print("Exit-button2");
      if (bestReaction == 0 || bestReaction > reaction)
      {
        bestReaction = reaction;
        EEPROM.put(4, bestReaction);
      }
    }
    else if (key == '2' && value == GAMES)
    {
      attemptsNum = 0;
      input = "";
      exitVal = true;
      value = GUESSNUMGAME;
      randomCount = random(1, 101);
      lcd.clear();
      lcd1.clear();
      lcd2.clear();
      lcd3.clear();
      lcd.setCursor(0,0);
      lcd.print("Write your num");
      lcd.setCursor(0,1);
      lcd.print("and enter #");
    }
    else if(value == GUESSNUMGAME && key >= '0' && key <= '9')
    {
      input += key;
      lcd1.setCursor(0,0);
      lcd1.print(input);
    }
    else if (key == '#' && value == GUESSNUMGAME)
    {
      countNum = input.toInt();
      attemptsNum++;
      if (countNum == randomCount)
      {
        value = WINGUESSNUM;
        exitVal = false;
        lcd.clear();
        lcd1.clear();
        lcd2.clear();
        lcd3.clear();
        lcd.setCursor(0,0);
        lcd.print("YOU WIN!");
        lcd.setCursor(0,1);
        lcd.print("Attempts:");
        lcd.setCursor(9,1);
        lcd.print(attemptsNum);
        lcd1.setCursor(0,0);
        lcd1.print("Exit-button2");
        if(bestAttempts == 0 || attemptsNum < bestAttempts)
        {
          bestAttempts = attemptsNum;
          EEPROM.put(8, bestAttempts);
        }
        input = "";
      }
      else if(countNum < randomCount)
      {
        lcd.clear();
        lcd1.clear();
        lcd2.clear();
        lcd3.clear();
        input = "";
        lcd.setCursor(0,0);
        lcd.print("TOO LOW");
      }
      else if (countNum > randomCount)
      {
        lcd.clear();
        lcd1.clear();
        lcd2.clear();
        lcd3.clear();
        input = "";
        lcd.setCursor(0,0);
        lcd.print("TOO HIGH");
      }
    }
    else if (key == '3' && value == GAMES)
    {
      attemptsNum4 = 0;
      input = "";
      exitVal = true;
      value = GUESS4NUMGAME;
      lcd.clear();
      lcd1.clear();
      lcd2.clear();
      lcd3.clear();
      lcd.setCursor(0,0);
      lcd.print("Write your num4");
      lcd.setCursor(0,1);
      lcd.print("and enter #");
      secret1 = random(0, 10);
      secret2 = random(0, 10);
      secret3 = random(0, 10);
      secret4 = random(0, 10);
    }
    else if(value == GUESS4NUMGAME && key >= '0' && key <= '9' && winingLcd == false)
    {
      input += key;
      lcd1.setCursor(0,0);
      lcd1.print(input);
    }
    else if (key == '#' && value == GUESS4NUMGAME && input.length() == 4 && winingLcd == false)
    {
      lcd2.setCursor(0,0);
      lcd2.print("LastAttempt");
      lcd2.setCursor(0,1);
      lcd2.print(input);
      lcd.clear();
      lcd1.clear();
      lcd3.clear();
      attemptsNum4++;
      int guess1 = input[0] - '0';
      int guess2 = input[1] - '0';
      int guess3 = input[2] - '0';
      int guess4 = input[3] - '0';
      bulls = 0;
      cows = 0;
      if (guess1 == secret1)
      {
        bulls++;
      }
      if (guess2 == secret2)
      {
        bulls++;
      }
      if (guess3 == secret3)
      {
        bulls++;
      }
      if (guess4 == secret4)
      {
        bulls++;
      }
      if (guess1 != secret1)
      {
        if (guess1 == secret2)
        {
          cows++;
        }

        if (guess1 == secret3)
        {
          cows++;
        }

        if (guess1 == secret4)
        {
          cows++;
        }
      }
      if (guess2 != secret2)
      {
        if(guess2 == secret1)
        {
          cows++;
        }
        if(guess2 == secret3)
        {
          cows++;
        }
        if(guess2 == secret4)
        {
          cows++;
        }
      }
      if (guess3 != secret3)
      {
        if (guess3 == secret1)
        {
          cows++;
        }
        if (guess3 == secret2)
        {
          cows++;
        }
        if (guess3 == secret4)
        {
          cows++;
        }
      }
      if (guess4 != secret4)
      {
        if (guess4 == secret1)
        {
          cows++;
        }
        if (guess4 == secret2)
        {
          cows++;
        }
        if (guess4 == secret3)
        {
          cows++;
        }
      }
      lcd.setCursor(0,0);
      lcd.print("BULLS:");
      lcd.setCursor(6,0);
      lcd.print(bulls);
      lcd.setCursor(0,1);
      lcd.print("COWS:");
      lcd.setCursor(5,1);
      lcd.print(cows);
      input = "";
      if (bulls == 4 && value == GUESS4NUMGAME)
      {
        value = WINGUESS4NUM;
        winingLcd = true;
        exitVal = false;
        lcd.clear();
        lcd1.clear();
        lcd2.clear();
        lcd3.clear();
        lcd.setCursor(0,0);
        lcd.print("YOU WIN!");
        lcd.setCursor(0,1);
        lcd.print("Attempts:");
        lcd.setCursor(9,1);
        lcd.print(attemptsNum4);
        lcd1.setCursor(0,0);
        lcd1.print("Exit-Button2");
        if (best4NumAttempts == 0 || best4NumAttempts > attemptsNum4)
        {
          best4NumAttempts = attemptsNum4;
          EEPROM.put(12, best4NumAttempts);
        }
      }
    }
    else if (key == '4' && value == GAMES)
    {
      value = RECORDES;
      exitVal = false;
      lcd.clear();
      lcd1.clear();
      lcd2.clear();
      lcd3.clear();
      lcd.setCursor(0,0);
      lcd.print("BestRecordes");
      lcd.setCursor(0,1);
      lcd.print("Exit-button2");
      lcd1.setCursor(0,0);
      lcd1.print("BestReaction:");
      lcd1.setCursor(0,1);
      EEPROM.get(4, bestReaction);
      lcd1.print(bestReaction);
      lcd2.setCursor(0,0);
      lcd2.print("BestAttempts:");
      lcd2.setCursor(0,1);
      EEPROM.get(8, bestAttempts);
      lcd2.print(bestAttempts);
      lcd3.setCursor(0,0);
      lcd3.print("BestAttempts4:");
      lcd3.setCursor(0,1);
      EEPROM.get(12, best4NumAttempts);
      lcd3.print(best4NumAttempts);
    }
    else if (key == '4' && value == ARDUINOOS)
    {
      value = SETTINGS;
      exitVal = true;
      lcd.clear();
      lcd1.clear();
      lcd2.clear();
      lcd3.clear();
      lcd.setCursor(0,0);
      lcd.print("Settings");
      lcd1.setCursor(0,0);
      lcd1.print("1-password");
      lcd1.setCursor(0,1);
      lcd1.print("2-backlight");
    }
    else if (value == SETTINGS && key == '1')
    {
      exitVal = false;
      value = SETTINGSPASS;
      lcd.clear();
      lcd1.clear();
      lcd2.clear();
      lcd3.clear();
      lcd.setCursor(0,0);
      lcd.print("1-setPass");
      lcd.setCursor(0,1);
      lcd.print("2-PassOnOff");
      lcd1.setCursor(0,0);
      lcd1.print("Exit-button2");
    }
    else if (value == SETTINGSPASS && key == '1')
    {
      value = SETPASS;
      lcd.clear();
      lcd1.clear();
      lcd2.clear();
      lcd3.clear();
      lcd.setCursor(0,0);
      lcd.print("EnterNewPassword");
      lcd.setCursor(0,1);
      lcd.print("Exit-button2");
    }
    else if (value == SETPASS && key == '#' && input != "")
    {
      value = SURE;
      lcd.clear();
      lcd1.clear();
      lcd2.clear();
      lcd3.clear();
      lcd.setCursor(0,0);
      lcd.print("Are u sure?");
      lcd.setCursor(0,1);
      lcd.print("#-yes     *-no");
      lcd1.setCursor(0,0);
      lcd1.print("Password is:");
      lcd1.setCursor(0,1);
      lcd1.print(input);
    }
    else if (value == SETPASS && key == '*')
    {
      lcd1.clear();
      input = "";
    }
    else if (value == SETPASS && key)
    {
      input+=key;
      lcd1.setCursor(0,0);
      lcd1.print(input);
    }
    else if (value == SURE && key == '#')
    {
      password = input.toInt();
      EEPROM.put(16, password);
      lcd.clear();
      lcd1.clear();
      lcd2.clear();
      lcd3.clear();
      lcd.setCursor(0,0);
      lcd.print("DONE");
      delay(3000);
      value = SETTINGS;
      exitVal = true;
      lcd.clear();
      lcd1.clear();
      lcd2.clear();
      lcd3.clear();
      lcd.setCursor(0,0);
      lcd.print("Settings");
      lcd1.setCursor(0,0);
      lcd1.print("1-password");
      lcd1.setCursor(0,1);
      lcd1.print("2-backlight");
    }
    else if (value == SURE && key == '*')
    {
      value = SETPASS;
      lcd.clear();
      lcd1.clear();
      lcd2.clear();
      lcd3.clear();
      lcd.setCursor(0,0);
      lcd.print("EnterNewPassword");
      lcd.setCursor(0,1);
      lcd.print("Exit-button2");
    }
    else if (value == SETTINGSPASS && key == '2')
    {
      value = ONOFFPASS;
      lcd.clear();
      lcd1.clear();
      lcd2.clear();
      lcd3.clear();
      lcd.setCursor(0,0);
      lcd.print("press1-ON");
      lcd.setCursor(0,1);
      lcd.print("press2-OFF");
      lcd1.setCursor(0,0);
      lcd1.print("Exit-button2");
      lcd2.setCursor(0,0);
      if (passw == true)
      {
        lcd2.print("NOW ON");
      }
      else 
      {
        lcd2.print("NOW OFF");
      }
    }
    else if (value == ONOFFPASS && key == '1')
    {
      passw = true;
    }
    else if (value == ONOFFPASS && key == '2')
    {
      passw = false;
    }
    else if (value == SETTINGS && key == '2')
    {
      value = BACKLIGHT;
      exitVal = false;
      lcd.clear();
      lcd1.clear();
      lcd2.clear();
      lcd3.clear();
      lcd.setCursor(0,0);
      lcd.print("Press1 to ON");
      lcd.setCursor(0,1);
      lcd.print("Press2 to OFF");
      lcd1.setCursor(0,0);
      lcd1.print("Exit-button2");
    }
    else if (value == BACKLIGHT && key == '1')
    {
      backlightval = true;
      EEPROM.put(20, backlightval);
    }
    else if (value == BACKLIGHT && key == '2')
    {
      backlightval = false;
      EEPROM.put(20, backlightval);
    }
    if (backlightval == true)
  {
    lcd.backlight();
    lcd1.backlight();
    lcd2.backlight();
    lcd3.backlight();
  }
  else if (backlightval == false)
  {
    lcd.noBacklight();
    lcd1.noBacklight();
    lcd2.noBacklight();
    lcd3.noBacklight();
  }
  if (key == '5' && value == ARDUINOOS)
  {
    value = MUSIC;
    lcd.clear();
    lcd1.clear();
    lcd2.clear();
    lcd3.clear();
    lcd.setCursor(0,0);
    lcd.print("Music");
    lcd1.setCursor(0,0);
    lcd1.print("Music1-1");
    lcd2.setCursor(0,0);
    lcd2.print("Music2-2");
    lcd3.setCursor(0,0);
    lcd3.print("Music3-3");
  }
   else if (key == '1' && value == MUSIC)
   {
    tone(11, 262, 400);
    delay(450);

    tone(11, 330, 400);
    delay(450);

    tone(11, 392, 500);
    delay(550);

    tone(11, 523, 700);
    delay(750);

    tone(11, 494, 300);
    delay(350);

    tone(11, 440, 400);
    delay(450);

    tone(11, 392, 600);
    delay(650);

    noTone(11);
   }
  }
}