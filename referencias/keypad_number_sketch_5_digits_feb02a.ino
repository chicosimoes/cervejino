
/*
  Keypad sketch
 sends the numbers or 'functions' pressed on a keypad to a serial port LCD
 and produces a 5 digit integer number displayed on the LCD serial screen.
 This uses a software serial driver for a serial 1602 LCD display.
 
 There is another sketch for entering 5 digits, this shows how to expand
 the sketch to the number of digits required.
 
 The keypad routine to scan the keys is based on 'Reading a keypad'
 Arduino cookbook by Michael Margolis. Page 149
 
 |=======================| 
 | Keypad number sketch  |
 | 5 digits              |
 | Written by Phil Sydor.|
 | February 2012   V1.0  |
 | ======================|
 You may use this sketch as you wish
 
 Press button A = ok - after entering a number
 Press button B = reset - to clear and start again
 */

const int numRows = 5;       // number of rows in the keypad
const int numCols = 4;       // number of columns
const int debounceTime = 20; // number of milliseconds for switch to be stable
//                           // you may need to change this value depending
//                           // on the switches you have. Some experimentation
//                           // may be required

int key_pressed_number = 0;     // the number of the key pressed, 3 keys 

char key_value = 0;             // the value of the key being pressed - used whewn in 5 digit mode
unsigned long first_digit = 0;  // first and second digits have to be unsigned long,        
unsigned long second_digit = 0; // used to determine which digit we are at same for the rest below.
int third_digit = 0;            // all other values can be int as they will never exceed 65535
int fourth_digit = 0;
int fifth_digit = 0;
int sixth_digit = 0;
unsigned long digit_value = 0;      

//Serial_lcd setup
#include <SoftwareSerial.h>  // this is the software serial emulator
#define CR 250  // set values for CR = command Register for the LCD
#define CD 1    // set value for CD = command Data for the LCD

const int rxpin = 15;           // pin used to receive (not active/used in this version) 
const int txpin = 14;           // pin used to send to LCD
const byte line_1 = 0x80;       // Move cursor to the first line of the LCD
const byte line_2 = 0xC0;       // Move cursor to the second line of the LCD
const byte blink_ON = 0x0F;     // cursor blink on
const byte blink_OFF = 0x0C;    // cursor blink off
SoftwareSerial serial_lcd(rxpin, txpin); // new serial port on pins 2 and 3


// keymap defines the character returned when the corresponding key is pressed
const char keymap[numRows][numCols] = {
  { 
    '1', '2', '3', 'A'                                                                                                                                                                                                                            } 
  ,
  { 
    '4', '5', '6', 'B'                                                                                                                                                                                                                            } 
  ,
  { 
    '7', '8', '9', 'C'                                                                                                                                                                                                                            } 
  ,
  { 
    'I', '0', '.', 'D'                                                                                                                                                                                                                            } 
  ,
  { 
    'E', 'F', 'G', 'H'                                                                                                                                                                                                                            } 
  ,
};

// this array determines the Arduino pins used for the rows and columns of the keypad
const int rowPins[numRows] = { 
  2, 3, 4, 5, 6 }; // Rows 0 through 4
const int colPins[numCols] = { 
  7, 8, 9, 10 };    // Columns 0 through 3
//++++++++++++++++++++++++++++++++++++++ Start of program setup ++++++++++++++++++++++++++++++++++
void setup()
{
  serial_lcd.begin(9600); //initialize the software serial port for 9600 baud
  lcd_clear();            // Clear the lcd display
  serial_lcd.print("Welcome to the");
  lcd_line2();            // move cursor to line 2
  serial_lcd.print("Keypad entry V1");
  delay (1000);           // wait 1 second so that message may be seen
  enter_number_display(); // display enter number message

  for (int row = 0; row < numRows; row++)
  {
    pinMode(rowPins[row],INPUT);       // Set row pins as input
    digitalWrite(rowPins[row],HIGH);   // turn on Pull-ups
  }
  for (int column = 0; column < numCols; column++)
  {
    pinMode(colPins[column],OUTPUT);     // Set column pins as outputs 
    // for writing
    digitalWrite(colPins[column],HIGH);  // Make all columns inactive
  }
}
//++++++++++++++++++++++++++++++++++++++ Start of program loop +++++++++++++++++++++++++++++++++++
void loop()
{
  char key = getKey();
  key_value = key;
  if( key != 0)   // if the character is not 0 then it's a valid key press 
  {       
    switch(key)
    {
    case '1':
      serial_lcd.print("1");
      make_number();
      break;
    case '2':
      serial_lcd.print("2");
      make_number();
      break;
    case '3':
      serial_lcd.print("3");
      make_number();
      break;
    case '4':
      serial_lcd.print("4");
      make_number();
      break;
    case '5':
      serial_lcd.print("5");
      make_number();
      break;
    case '6':
      serial_lcd.print("6");
      make_number();
      break;
    case '7':
      serial_lcd.print("7");
      make_number();
      break;
    case '8':
      serial_lcd.print("8");
      make_number();
      break;
    case '9':
      serial_lcd.print("9");
      make_number();
      break;
    case '0':
      serial_lcd.print("0");
      make_number();
      break;
    case '.':
      // decimal point not allowed - we are only entering 3 digits
      // and not a floating point number
      lcd_clear();
      serial_lcd.print("Decimal point");
      lcd_line2();
      serial_lcd.print("not allowed");
      delay(2000);
      set_all_to_0();
      lcd_clear();
      enter_number_display();
      break;
    case 'A':
      lcd_clear();
      serial_lcd.print("Menu OK");    // 
      delay(500);  //hold display briefly by waiting half a second
      display_result();
      break;
    case 'B':               
      lcd_clear();
      serial_lcd.print("Menu Reset");
      delay(500);  //hold display briefly by waiting half a second
      set_all_to_0();
      lcd_clear();
      enter_number_display();
      break; 
    case 'C':                  // 
      lcd_clear();
      serial_lcd.print("Menu C");
      // place procedure call here for menu option
      break;
    case 'D':                  //
      lcd_clear();
      serial_lcd.print("Menu D");
      // place procedure call here for menu option
      break;
    case 'E':                  //
      lcd_clear();
      serial_lcd.print("Menu E");
      // place procedure call here for menu option
      break;
    case 'F':                 // 
      lcd_clear();
      serial_lcd.print("Menu F");
      // place procedure call here for menu option
      break;
    case 'G':                 // 
      lcd_clear();
      serial_lcd.print("Menu G");
      // place procedure call here for menu option
      break;
    case 'H':                 // 
      lcd_clear();
      serial_lcd.print("Menu H");
      // place procedure call here for menu option
      break;
    case 'I':                 // 
      lcd_clear();
      serial_lcd.print("Menu I");
      // place procedure call here for menu option
      break;
    } 
    //========================= number Value ============================================================
    // Here we enter the 3 digits.
    switch (key_pressed_number)
    {
    case 1: // if (key_pressed_number == 1)
      digit_value = first_digit;
      break;
    case 2:
      digit_value = (first_digit * 10) + second_digit;
      break;
    case 3:
      digit_value = (first_digit * 100) + (second_digit * 10) + third_digit;
      break;
    case 4:
      digit_value = (first_digit * 1000) + (second_digit * 100) + (third_digit *10) + fourth_digit;
      break;
    case 5:
      digit_value = (first_digit * 10000) + (second_digit * 1000) + (third_digit *100) + (fourth_digit * 10) + fifth_digit;
      break;
    } 
  }
}
//=================================================================================================
// returns with the key pressed, or 0 if no key is pressed
char getKey()
{
  char key = 0;                                  // 0 indicates no key pressed

  for(int column = 0; column < numCols; column++)
  {
    digitalWrite(colPins[column],LOW);         // Activate the current column.
    for(int row = 0; row < numRows; row++)     // Scan all rows for a key press.
    {
      if(digitalRead(rowPins[row]) == LOW)     // Is a key pressed?
      {
        delay(debounceTime);                   // debounce
        while(digitalRead(rowPins[row]) == LOW)
          ;                                  // wait for key to be released
        key = keymap[row][column];             // Remember which key was pressed.
      }
    }
    digitalWrite(colPins[column],HIGH);     // De-activate the current column.
  }
  return key;                               // returns the key pressed or 0 if none
}
//============================ Make number ========================================================
// Makes up the number from the keys pressed
int make_number()
{
  // make up the number for a value, maximum is 3 digits

  switch (key_pressed_number)
  {
  case 0:
    first_digit = key_value - '0'; //change first_digit to integer value from ascii
    key_pressed_number = key_pressed_number + 1;
    break; 
  case 1:
    second_digit = key_value - '0'; //change second_digit to integer value from ascii
    key_pressed_number = key_pressed_number + 1;
    break;
  case 2:
    third_digit = key_value - '0'; //change second_digit to integer value from ascii
    key_pressed_number = key_pressed_number + 1;
    break;
  case 3:
    fourth_digit = key_value - '0'; //change second_digit to integer value from ascii
    key_pressed_number = key_pressed_number + 1;
    break;
  case 4:
    fifth_digit = key_value - '0'; //change second_digit to integer value from ascii
    key_pressed_number = key_pressed_number + 1;
    break;
  case 5: // too many digits
    lcd_clear();
    serial_lcd.print("Number too long");
    delay (2000);         // wait 2 seconds for display
    lcd_clear();          // clear display
    set_all_to_0();       // initialise all parameters
    enter_number_display();
    break;
  }

}
//=========================== display_result ======================================================
void display_result()
{
  lcd_clear();
  serial_lcd.print("Number = ");
  //  lcd_line2();//this will display the number on line 2, otherwise the number is displayed on line 1
  serial_lcd.print(digit_value);
}
//====================================Free RAM====================================================
/*  // function to return the amount of free RAM 
 // variables created by the build process when compiling the sketch
 extern int __bss_end;
 extern void *__brkval;
 
 int memoryFree()
 {
 int freeValue;
 
 if((int)__brkval == 0)
 freeValue = ((int)&freeValue) - ((int)&__bss_end);
 else
 freeValue = ((int)&freeValue) - ((int)__brkval);
 return freeValue;
 } */
//================================= lcd clear =====================================================
//
void lcd_clear() // clears the LCD screen and go to line 1, make cursor blink
{
  serial_lcd.write(CR);        // set LCD to command mode
  serial_lcd.write(1);         // 1 = clear the lcd display
  serial_lcd.write(CR);        // set LCD to command mode
  serial_lcd.write(line_1);    // moves cursor to line 1 first position
  serial_lcd.write(CR);        //set LCD to command mode
  serial_lcd.write(blink_ON);  // set cursor to blink

}
//=================================================================================================
//=======================Move to LCD line 1========================================================
//
void lcd_line1()
{
  serial_lcd.write(CR);       //set LCD to command mode
  serial_lcd.write(line_1);   // moves cursor to line 1 first position
}
//=================================================================================================
//=======================Move to LCD line 1========================================================
void lcd_line2()
{
  serial_lcd.write(CR);       //set LCD to command mode
  serial_lcd.write(line_2);   // moves cursor to line 2 first position
}
//=================================================================================================
//
void enter_number_display()
{
  lcd_clear();
  serial_lcd.print("Enter number"); // prompts user to enter a number
  lcd_line2();                      // move to line 2 to entyer number
}
//=================================================================================================
/* //========================= test function =========================================================
 void here_now() // used to test the logic of the software,like a breakpoint.
 {
 serial_lcd.print("here now"); 
 } */
//=================================================================================================

//============================== set all to 0 ======================================================
// Resets values to initial states to start again.
void set_all_to_0() 
{
  key_pressed_number = 0;
  key_value = 0;
  first_digit = 0;
  second_digit = 0;
  third_digit = 0;
  fourth_digit = 0;
  fifth_digit = 0;
  sixth_digit = 0;
  digit_value = 0;
}
//+++++++++++++++++++++++++  End of program ++++++++++++++++++++++++++++++++++++++++++++++++++++++++












































































































