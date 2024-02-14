#include <Servo.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
int buzzer = 13;
Servo doorServo;
LiquidCrystal_I2C lcd(0x27, 16, 2);

int servoPin = 12;

String correctPassword_one = "";
String correctPassword_two = "";
String closePassword_one = "";
String magicPassword_one = "";
String magicPassword_two = "";
int counter =0;

const byte ROW_NUM = 4;
const byte COLUMN_NUM = 4;

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1', '2', '3', 'A'},
  {'7', '8', '9', 'C'},
  {'4', '5', '6', 'B'},
  {'*', '0', '#', 'D'}
};

byte pin_rows[ROW_NUM] = {9, 8, 7, 6};
byte pin_column[COLUMN_NUM] = {5, 4, 3, 2};

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

void setup() {
  pinMode(buzzer,OUTPUT);
  doorServo.attach(servoPin);
  doorServo.write(0);
  lcd.begin(16, 2);
  lcd.backlight();
  initializeSystem();
}

void loop() {
  String enteredPassword = readPassword();

  if (enteredPassword.equals(correctPassword_one)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Password correct");
    lcd.setCursor(0, 1);
    lcd.print("Opening door...");
    for(int i=0;i<180;i++){
    doorServo.write(i);
    delay(10);}
    int c=1;
    while(c){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter close password:");
    String closePassword_entered = readPassword();

    if (closePassword_entered.equals(closePassword_one)) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Door closed");
      for(int i=180;i>=0;i--){
      doorServo.write(i);
      delay(10);}
      c=0;
    } else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Incorrect close password");
      delay(2000);
    }
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter password:");
  } else {
    digitalWrite(buzzer,HIGH);
    delay(1000);
    digitalWrite(buzzer,LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Incorrect password");
    delay(2000);
    lcd.clear();
    handleMagicPassword();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter password:");
  }
}
char ch() {
  char key = keypad.getKey();
  while (!key) {
    key = keypad.getKey();
  }
  // lcd.print(key);
  return key;
}

String readPassword() {
  String password = "";
  char key = keypad.getKey();
  while (key != '#') {
    
    if (key) {
      password += key;
      lcd.setCursor(password.length() - 1, 1);
      lcd.print('*');
    }
    key = keypad.getKey();
  }

  int n = password.length();
  if (n < 4 ) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Password must be");
    lcd.setCursor(0, 1);
    lcd.print("between 4 and 8 characters.");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Enter password:");
    return readPassword();
  }

  lcd.clear();
  return password;
}

void handleMagicPassword() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("1) Enter magic:");
  lcd.setCursor(0, 1);
  lcd.print("2) Enter password again:");
  char choice = ch();

  if (choice == '1') {
    lcd.clear();
    lcd.print("Enter magic password:");
    magicPassword_two = readPassword();

    if (magicPassword_one.equals(magicPassword_two)) {
      lcd.print("Magic password correct");
      delay(2000);
      lcd.clear();
      initializeSystem();
    } else {
      lcd.print("Incorrect magic password");
      delay(2000);
      lcd.clear();
    }
  }
}

void initializeSystem() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  lcd.setCursor(0, 1);
  lcd.print("Please wait.");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Enter main password:");
  setPassword(correctPassword_one);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Confirm password:");
  setPassword(correctPassword_two);

  if (correctPassword_one != correctPassword_two) {
    lcd.clear();
    lcd.print("Passwords do not match.");
    delay(2000);
    initializeSystem(); // Restart initialization
    counter++;
  }
  if(counter == 0){
    lcd.clear();
    lcd.print("Enter magic password:");
    setPassword(magicPassword_one);

    lcd.clear();
    lcd.print("Enter close password:");
    setPassword(closePassword_one);

    lcd.clear();
    lcd.print("Initialization complete.");
    delay(2000);
    lcd.clear();
    lcd.print("Enter password:");
  }
    counter==0;

}

void setPassword(String &password) {
  password = "";
  while (true) {
    char key = keypad.getKey();
    if (key == '#') {
        int length = password.length();
        if (length >= 4) {
          lcd.clear();
          return;
        } else {
          lcd.clear();
          lcd.print("Password must be");
          lcd.setCursor(0, 1);
          lcd.print("between 4 and 8 characters.");
          delay(2000);
          lcd.clear();
          lcd.print("Enter password:");
        }
    } else if (key == '*') {
      if (password.length() > 0) {
        password.remove(password.length() - 1);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Enter password:");
        lcd.setCursor(0,1);
        for(int i=0;i<password.length();i++)
          lcd.print("*");
      }
    } else if (key) {
      // if (password.length() < 9) {
        password += key;
        lcd.setCursor(password.length() - 1, 1);
        lcd.print('*');
      
    }
    delay(50);
  }
}
