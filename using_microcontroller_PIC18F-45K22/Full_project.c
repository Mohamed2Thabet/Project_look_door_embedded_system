// Keypad module connections
char keypadPort at PORTC;

// LCD pinout settings
sbit LCD_RS at RB5_bit;
sbit LCD_EN at RB4_bit;
sbit LCD_D7 at RB0_bit;
sbit LCD_D6 at RB1_bit;
sbit LCD_D5 at RB2_bit;
sbit LCD_D4 at RB3_bit;

// Pin direction
sbit LCD_RS_Direction at TRISB5_bit;
sbit LCD_EN_Direction at TRISB4_bit;
sbit LCD_D7_Direction at TRISB0_bit;
sbit LCD_D6_Direction at TRISB1_bit;
sbit LCD_D5_Direction at TRISB2_bit;
sbit LCD_D4_Direction at TRISB3_bit;

#define MAX_SIZE 16
int i, counter = 0;
char userPassword[MAX_SIZE]; // Increased size to store null-terminated strings
char confirmPassword[MAX_SIZE];
char magicPassword1[MAX_SIZE];
char magicPassword2[MAX_SIZE];
char closePassword1[MAX_SIZE];
char closePassword2[MAX_SIZE];
char userInput[3]; // Increased size for null-terminated string


void getPassword(char password[], int size) {
  int i;
  int j;
  for (i = 0; i < size - 1; i++) {
    while (password[i] == 0) {
      password[i] = Keypad_Key_Click();
    }

    switch (password[i]) {
      case 1: password[i] = '7'; break;
      case 2: password[i] = '8'; break;
      case 3: password[i] = '9'; break;
      case 4: password[i] = 'A'; break;
      case 5: password[i] = '4'; break;
      case 6: password[i] = '5'; break;
      case 7: password[i] = '6'; break;
      case 8: password[i] = 'B'; break;
      case 9: password[i] = '1'; break;
      case 10: password[i] = '2'; break;
      case 11: password[i] = '3'; break;
      case 12: password[i] = 'C'; break;
      case 13: password[i] = '*'; break;
      case 14: password[i] = '0'; break;
      case 15: password[i] = '#'; break;
      case 16: password[i] = 'D'; break;
    }

      if (password[i] == '*') {
      if (i > 0) {
        password[i] = 0;
        password[i - 1] = 0;
        i -= 2;
      } else {
        password[i] = 0;
        i--;
      }
      Lcd_Out(2, 1, "               ");
      for( j=0;j<=i;j++)
        Lcd_Chr(2, j + 1, '*');
      continue;
    }
    else if (password[i] == '#') {
    password[i] = 0;
          i--;
         if( i>2 && i<=7){

      break;     }
      else
      {
       Lcd_Out(2, 1, "pass 4 => 8");
       Delay_ms(200);
       Lcd_Out(2, 1, "continue pass");
       Delay_ms(200);
       Lcd_Out(2, 1, "               ");
       for( j=0;j<=i;j++)
        Lcd_Chr(2, j + 1, '*');
      continue;
      }

    }
    Lcd_Chr(2, i + 1, '*');
  }
}



void clearArray(char arr[], int size) {
  for (i = 0; i < size - 1; i++)
    arr[i] = 0;
}

void initializeSystem() {
  mainPasswordIncorrect:
  Lcd_Out(1, 1, "New Password");
  getPassword(userPassword, MAX_SIZE);
  Lcd_Cmd(_LCD_CLEAR);
  Lcd_Out(1, 1, "Confirm Password");
  getPassword(confirmPassword, MAX_SIZE);
  Lcd_Cmd(_LCD_CLEAR);
  if (strcmp(userPassword, confirmPassword) != 0) {
    clearArray(confirmPassword, MAX_SIZE);
    clearArray(userPassword, MAX_SIZE);
    goto mainPasswordIncorrect;
  }

  magicPasswordIncorrect:
  Lcd_Out(1, 1, "Magic Password");
  getPassword(magicPassword1, MAX_SIZE);
  Lcd_Cmd(_LCD_CLEAR);
  Lcd_Out(1, 1, "Confirm Magic");
  getPassword(magicPassword2, MAX_SIZE);
  Lcd_Cmd(_LCD_CLEAR);
  if (strcmp(magicPassword1, magicPassword2) != 0) {
    clearArray(magicPassword1, MAX_SIZE);
    clearArray(magicPassword2, MAX_SIZE);
    goto magicPasswordIncorrect;
  }

  closePasswordIncorrect:
  Lcd_Out(1, 1, "Close Password");
  getPassword(closePassword1, MAX_SIZE);
  Lcd_Cmd(_LCD_CLEAR);
  Lcd_Out(1, 1, "Confirm Close");
  getPassword(closePassword2, MAX_SIZE);
  Lcd_Cmd(_LCD_CLEAR);
  if (strcmp(closePassword1, closePassword2) != 0) {
    clearArray(closePassword1, MAX_SIZE);
    clearArray(closePassword2, MAX_SIZE);
    goto closePasswordIncorrect;
  }
}

void rotateServoToZero() {
  unsigned int i;

  for (i = 0; i < 50; i++) {
    PORTA = 1;
    Delay_us(749);
    PORTA = 0;

  }
}


void rotateServo90() {
  unsigned int i;
  for (i = 0; i < 50; i++) {
    PORTA = 1;
    Delay_us(1500);
    PORTA = 0;

  }
}
int isPasswordCorrect(char inputPassword[], char correctPassword[]) {
  return strcmp(inputPassword, correctPassword) == 0;
}

void openDoor() {
  Lcd_Out(1, 1, "Open Door");
  TRISA = 0;
  rotateServo90();
  Delay_ms(200);
  Lcd_Cmd(_LCD_CLEAR);
}
void magic_name()
{

     // while (userInput[0] == '1') {
        clearArray(magicPassword2, MAX_SIZE);
        Lcd_Out(1, 1, "Enter Magic");
        getPassword(magicPassword2, MAX_SIZE);
        Lcd_Cmd(_LCD_CLEAR);

        if (isPasswordCorrect(magicPassword1, magicPassword2)) {
          Lcd_Out(1, 1, "Magic Correct");
          Lcd_Out(2, 1, "Data Again");
          Delay_ms(1500);
          clearArray(userPassword, MAX_SIZE);
          clearArray(confirmPassword, MAX_SIZE);
          clearArray(magicPassword1, MAX_SIZE);
          clearArray(magicPassword2, MAX_SIZE);
          clearArray(closePassword1, MAX_SIZE);
          clearArray(closePassword2, MAX_SIZE);
          Lcd_Cmd(_LCD_CLEAR);
          initializeSystem();
          }
          //counter=1;
     //    clearArray(userInput, 2);
      //}   clearArray(userInput, 2);
}
void closeDoor() {
  close_agin:
  clearArray(closePassword2, MAX_SIZE);
  Lcd_Out(1, 1, "Enter Close");
  getPassword(closePassword2, MAX_SIZE);
  Lcd_Cmd(_LCD_CLEAR);

  if (isPasswordCorrect(closePassword1, closePassword2)) {
    Lcd_Out(1, 1, "Door Closed");
    TRISA = 0;
    rotateServoToZero();
    Delay_ms(200);
    Lcd_Cmd(_LCD_CLEAR);
  }else
  {
        Lcd_Out(1, 1, "1) Magic Password");
        Lcd_Out(2, 1, "Anything close");
        getPassword(userInput, 2);
        Lcd_Cmd(_LCD_CLEAR);
        if(userInput[0]== '1')
        {    clearArray(userInput, 2);
              magic_name();
            //  if(counter ==1)
              //{
              // counter=0;
               goto close_agin;
              //}

        }
        else
        {
         clearArray(userInput, 2);
         goto close_agin;
        }

  }
}

void main() {
  Keypad_Init();
  ANSELB = 0;
  ANSELC = 0;

  Lcd_Init();
  Lcd_Cmd(_LCD_CURSOR_OFF);
  Lcd_Cmd(_LCD_CLEAR);

  initializeSystem();

  while (1) {
    mainPasswordAgain:
    Lcd_Cmd(_LCD_CLEAR);
    clearArray(userPassword, MAX_SIZE);
    Lcd_Out(1, 1, "Enter Password");
    getPassword(userPassword, MAX_SIZE);
    Lcd_Cmd(_LCD_CLEAR);

    if (isPasswordCorrect(userPassword, confirmPassword)) {
      openDoor();
      closeDoor();
    } else {
            Lcd_Out(1, 1, "1) Magic Password");
            Lcd_Out(2, 1, "Anything Password");
            getPassword(userInput, 2);
            Lcd_Cmd(_LCD_CLEAR);
            if(userInput[0] == '1')
              {
                 clearArray(userInput, 2);
                 magic_name();
              }
            else
            {
                 clearArray(userInput, 2);
                 goto mainPasswordAgain;
            }
        }

    }
  }