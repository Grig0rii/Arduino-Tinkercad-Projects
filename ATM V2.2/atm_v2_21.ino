#include <EEPROM.h>

#include <Keypad.h>

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x20, 16, 2);
LiquidCrystal_I2C lcd1(0x27, 16, 2);

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
long amount = 0;
unsigned int attempts = 3;
int value = 0;
int marker;
int currentAccount = -1;
unsigned long PrevMillis;
unsigned long lockStart;
bool ledState = false;
bool alarm = false;

struct Account
{
  String password;
  long balance;
  long lastOperation;
};

int EEPROM_START = 20;
int EEPROM_START1 = 32;

Account accounts[3] =
{
  {"1234", 1000, 0},
  {"8689", 5000, 0},
  {"9999", 3000, 0}
};

int findAccount(String password)
{
  for (int i = 0; i < 3; i++)
  {
    if (accounts[i].password == password)
    {
      return i;
    }
  }

  return -1;
}

void saveAccounts()
{
  for (int i = 0; i < 3; i++)
  {
    EEPROM.put(EEPROM_START + i * sizeof(long), accounts[i].balance);
    EEPROM.put(EEPROM_START1 + i * sizeof(long), accounts[i].lastOperation);
  }
}

void loadAccounts()
{
  for (int i = 0; i < 3; i++)
  {
    EEPROM.get(EEPROM_START + i * sizeof(long), accounts[i].balance);
    EEPROM.get(EEPROM_START1 + i * sizeof(long), accounts[i].lastOperation);
  }
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(12, OUTPUT);
  lcd.init();
  lcd1.init();
  lcd.backlight();
  lcd1.backlight();
  loadAccounts();
  lcd1.setCursor(0,0);
  lcd1.print("MINI ATM");
  lcd.setCursor(0,0);
  lcd.print("Enter password:");
  EEPROM.get(0, marker);

if (marker != 12345)
{
  accounts[0].balance = 1000;
  accounts[1].balance = 5000;
  accounts[2].balance = 3000;
  accounts[0].lastOperation = 0;
  accounts[1].lastOperation = 0;
  accounts[2].lastOperation = 0;

  saveAccounts();

  marker = 12345;
  EEPROM.put(0, marker);
}
else
{
  loadAccounts();
}
}

void loop()
{
  if (attempts == 0 && value == 0 && alarm == false)
  {
    lcd.clear();
    alarm = true;
    lockStart = millis();
  }
  if (alarm == true)
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
    }
  if (alarm == true && millis() - lockStart >= 10000)
{
    alarm = false;
    attempts = 3;
    input = "";
    lcd.clear();
    lcd1.clear();
    lcd.setCursor(0,0);
    lcd.print("Enter password:");
    lcd1.setCursor(0,0);
    lcd1.print("MINI ATM");
    input = "";
    value = 0;
}
  char key = keypad.getKey();
  if (key && alarm == false)
  {
    digitalWrite(LED_BUILTIN, HIGH);
    tone(12,500,200);
    digitalWrite(LED_BUILTIN, LOW);
    if(key == '#')
    {
      if(value == 0 && findAccount(input) != -1)
      {
        currentAccount = findAccount(input);
        attempts = 3;
        input = "";
        value = 1;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Correct");
        delay(1000);
        lcd.clear();
        lcd1.clear();
        lcd.setCursor(0,0);
        lcd.print("1-Balance&History");
        lcd.setCursor(0,1);
        lcd.print("2-Withdraw");
        lcd1.setCursor(0,0);
        lcd1.print("3-Deposit");
        lcd1.setCursor(0,1);
        lcd1.print("*-Back");
      }
      else if (value == 0 && findAccount(input) == -1 && attempts > 0)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Incorrect");
        attempts --;
        lcd.setCursor(0,1);
        lcd.print("attempts:");
        lcd.print(attempts);
        delay(1000);
        lcd.clear();
        input = "";
        lcd.setCursor(0,0);
        lcd.print("Enter password:");
      }
      else if (value == 3)
      {
        amount = input.toInt();
        if(accounts[currentAccount].balance >= amount)
        {
        accounts[currentAccount].balance -= amount;
        accounts[currentAccount].lastOperation = 0;
        accounts[currentAccount].lastOperation -= amount;
        saveAccounts();
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("Complete");
        delay(2000);
        lcd.clear();
        lcd1.clear();
        lcd.setCursor(0,0);
        lcd.print("1-Balance&History");
        lcd.setCursor(0,1);
        lcd.print("2-Withdraw");
        lcd1.setCursor(0,0);
        lcd1.print("3-Deposit");
        lcd1.setCursor(0,1);
        lcd1.print("*-Back");
        value = 1;
        }
        else
        {
        lcd.setCursor(0,1);
        lcd.print("No enough money");
        delay(2000);
        lcd.clear();
        lcd1.clear();
        lcd.setCursor(0,0);
        lcd.print("1-Balance&History");
        lcd.setCursor(0,1);
        lcd.print("2-Withdraw");
        lcd1.setCursor(0,0);
        lcd1.print("3-Deposit");
        lcd1.setCursor(0,1);
        lcd1.print("*-Back");
        value = 1;
        }
      }
      else if (value == 4)
      {
        amount = input.toInt();
        accounts[currentAccount].balance += amount;
        accounts[currentAccount].lastOperation = 0;
        accounts[currentAccount].lastOperation += amount;
        saveAccounts();
        lcd.clear();
        lcd.setCursor(0,1);
        lcd.print("Complete");
        delay(2000);
        lcd.clear();
        lcd1.clear();
        lcd.setCursor(0,0);
        lcd.print("1-Balance&History");
        lcd.setCursor(0,1);
        lcd.print("2-Withdraw");
        lcd1.setCursor(0,0);
        lcd1.print("3-Deposit");
        lcd1.setCursor(0,1);
        lcd1.print("*-Back");
        value = 1;
      }
    }
    else if(key == '*')
    {
      if(value == 1)
      {
        lcd.clear();
        lcd1.clear();
        lcd.setCursor(0,0);
        lcd.print("Enter password:");
        lcd1.setCursor(0,0);
        lcd1.print("MINI ATM");
        input = "";
        value = 0;
      }
      else if(value == 2 || value == 3 || value == 4)
      {
        input = "";
        value = 1;
        lcd.clear();
        lcd1.clear();
        lcd.setCursor(0,0);
        lcd.print("1-Balance&History");
        lcd.setCursor(0,1);
        lcd.print("2-Withdraw");
        lcd1.setCursor(0,0);
        lcd1.print("3-Deposit");
        lcd1.setCursor(0,1);
        lcd1.print("*-Back");
      }
      else
      {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Enter password:");
      input = "";
      value = 0;
      }
    }
    else if (value == 1 && key == '1')
    {
      lcd.clear();
      lcd1.clear();
      lcd.setCursor(0,0);
      lcd.print("Balance:");
      lcd.setCursor(0,1);
      lcd.print(accounts[currentAccount].balance);
      lcd1.setCursor(0,0);
      lcd1.print("History:");
      lcd1.setCursor(8,0);
      lcd1.print(accounts[currentAccount].lastOperation);
      lcd1.setCursor(0,1);
      lcd1.print("*-Back");
      value = 2;
    }
    else if (value == 1 && key == '2')
    {
      value = 3;
      input = "";
      amount = 0;
      lcd.clear();
      lcd1.clear();
      lcd.setCursor(0,0);
      lcd.print("Enter amount:");
      lcd1.setCursor(0,1);
      lcd1.print("*-Back");
    }
    else if (value == 1 && key == '3')
    {
      value = 4;
      input = "";
      amount = 0;
      lcd.clear();
      lcd1.clear();
      lcd.setCursor(0,0);
      lcd.print("Enter amount:");
      lcd1.setCursor(0,1);
      lcd1.print("*-Back");
    }
    else if (value == 0)
    {
      input += key;
      lcd.setCursor(0,1);
      lcd.print(input);
    }
    else if (value == 3)
    {
      input += key;
      lcd.setCursor(0,1);
      lcd.print(input);
    }
    else if (value == 4)
    {
      input += key;
      lcd.setCursor(0,1);
      lcd.print(input);
    }
  }
}