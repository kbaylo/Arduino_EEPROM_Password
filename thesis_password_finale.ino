/*  
    @SOLAR  TRICYCLE W/ PASSWORD DEVICE
      |A renewable and economical vehicle with digital protection
    @School
      | (Eulogio Amang Rodriguez Institute of Science and Technology)
    @Team
      |BSCOE 2B 
    @Batch
      |2013-2014
    @Last modified
      |OCtober 20 2013
*/
#include <Keypad.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <Password.h>

/// VARIABLE FOR KEYBOARD
const byte ROWS = 4; 
const byte COLS = 4;

/// SET KEYPAD KEYS
char keys[ROWS][COLS] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};

byte rowPins[ROWS] = { 11, 10, 9, 8 };
byte colPins[COLS] = { 7, 6, 5, 4 };


Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

LiquidCrystal lcd(14, 15, 2, 3, 12, 13);

int motorTrigger = 19;
int addressCounter; 
int count;
char storedPassword[10];
bool passAuthentication = false;
bool changePass = false;
bool pwCorrect = false;
char charCount[10];
int i = 0;
char value[10];
int m = 0;

void setup()
{
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(motorTrigger, OUTPUT);
  lcd.begin(16, 2);
  lcd.clear();  
  lcd.setCursor(0, 0);
  lcd.print("Enter Password");
  lcd.setCursor(0, 1);
  lcd.print("                ");
  digitalWrite(motorTrigger, LOW);
}

Password pw = Password(storedPassword);

void loop()
{
           
  char key = kpd.getKey();
  
  for(int q = 1; q <= EEPROM.read(0); q++)
  {
  storedPassword[q - 1] = EEPROM.read(q);
  }
  
  if(!changePass & !pwCorrect){
           
    kpd.addEventListener(keypadEvent);
  }
  else if(!changePass & pwCorrect){
    digitalWrite(motorTrigger, HIGH);
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Solar Tricycle");
    lcd.setCursor(0, 1);
    lcd.print("Enjoy Riding  :)");
    kpd.addEventListener(keypadLocker);
    delay(20);
  }
  else{
    kpd.addEventListener(keypadEventchangePass);
  }
  
  for(int q = 1; q <= EEPROM.read(0); q++)
  {
  storedPassword[q - 1] = EEPROM.read(q);
  }
}


void keypadEvent(KeypadEvent keyPress)
{
  if(passAuthentication){ 
       
    switch (kpd.getState()){
    case PRESSED:
      switch (keyPress){
        case '#':
          m = 0;
          checkPassword2();
          break;   
        case '*':
          pw.reset();
          lcd.clear();
          lcd.print("Current Pass?");
          lcd.setCursor(0, 1);
          lcd.print("                ");
          break;
        case 'B':
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Enter Password");
            lcd.setCursor(0, 1);
            lcd.print("                ");
            passAuthentication = false;
            pw.reset();
            break;
        default:
          lcd.setCursor(0, 0);
          lcd.print("Current Pass?");
          lcd.setCursor(m, 1);
          lcd.print("*");
          m++;
          pw.append(keyPress);
          break;
        }
     break;
    }
  }
  else{
    switch (kpd.getState()){
      case PRESSED:
        switch (keyPress){
          case '#':
            m = 0;
            checkPassword();
            break;
            
          case '*':
            pw.reset();
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Enter Password");
            lcd.setCursor(0, 1);
            lcd.print("                ");
            m = 0;
            break;
            
          case 'B':
            m = 0;
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Current Pass?");
            lcd.setCursor(0, 1);
            lcd.print("                ");
            passAuthentication = true;
            pw.reset();
            break;
            
          default:
            lcd.setCursor(0, 0);
            lcd.print("Enter Password");
            lcd.setCursor(m, 1);
            lcd.print("*");
            pw.append(keyPress);
            m++;
            break;
        }
      break; 
    }
  }
}

void keypadEventchangePass(KeypadEvent keyPress)
{
  switch (kpd.getState()){
    case PRESSED:
      m = 0;
      switch (keyPress){
        case '#':
          for(int c = 0; c <= 20; c++){
            EEPROM.write(c, 0);
          }
          char value[10];
          for(int k = i; k >= 0; k--){
            if(k!= 0){
              EEPROM.write(k, charCount[k - 1]);
            }
            else{
              EEPROM.write(0, i);
            }
          }
          for(int j = 1; j  <= 10; j++){
            value[j] = EEPROM.read(j);
            changePass = false;
            passAuthentication = false;
            }
            lcd.setCursor(0, 0);
            lcd.print("New Password is");
            lcd.setCursor(0, 1);
            lcd.print(value);
            delay(2000);
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Enter Password");
            pw.reset();
            memset(storedPassword,0,10);
            memset(charCount,0,10);
            i = 0;
          break;   
        case '*':
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Textbox cleared");
          lcd.setCursor(0, 1);
          lcd.print("                ");
          memset(charCount,0,10);
          i = 0;
          break;
        default:
          charCount[i] = keyPress;
          lcd.clear();
          lcd.setCursor(0, 0);
            lcd.print("New Password?");
            lcd.setCursor(i, 1);
            lcd.print(charCount[i]);
          i++;
          break;
      }
    break;
    }
  }
  
void keypadLocker(KeypadEvent keyPress)
{
  switch (kpd.getState()){
    case PRESSED:
      switch (keyPress){
        case 'D':
          digitalWrite(motorTrigger, LOW);
          pwCorrect = false;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Enter Password");
          break;
    }
  }
}  
void checkPassword()
{
    if (pw.evaluate())
  {
    pwCorrect = true;
    pw.reset(); 
  }
  else
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Password");
    lcd.setCursor(0, 1);
    lcd.print("Incorrect!");
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter Password");
    lcd.setCursor(0, 1);
    lcd.print("                ");
    pw.reset();
  }
}

void checkPassword2()
{
    if (pw.evaluate())
    {
      pw.reset();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("New Password?");
      lcd.setCursor(0, 1);
    lcd.print("                ");
      changePass = true;
    }
    else{
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Password");
      lcd.setCursor(0, 1);
      lcd.print("mismatched!");
      delay(1000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Enter Password");
      changePass = false;
      passAuthentication = false;
      pw.reset();
    }
}


