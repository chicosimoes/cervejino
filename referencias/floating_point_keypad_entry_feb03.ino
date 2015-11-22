
/*

Floating point keypad entry.
This sketch enables a floating point number up to 999.999 to be entered via a keypad
The keypad is a 5x4 matrix
A check is done on the final number as we may wish to limit the upper range

  
 |========================| 
 | Written by Phil Sydor  |
 |                        |
 | V0.1 February 2012     |
 |========================|
 
 
 */


const int numRows = 5;       // number of rows in the keypad
const int numCols = 4;       // number of columns
const int debounceTime = 20; // number of milliseconds for switch to be stable
char key_value = 0;             // the value of the key being pressed

float fp_degree_digit_value = 0;// used only for display purposes
float fp_degree = 0;// used as temporary hold


boolean select_degree = false; // degree selection mode
boolean decimal_point_active = false;

unsigned long degree_digit_value = 0;     
unsigned long digit_value = 0;            
unsigned long first_digit = 0;  // first and second digits have to be unsigned long,        
unsigned long second_digit = 0;           // used to determine which digit we are at same for the rest below.

int third_digit = 0;            // all other values can be int as they will never exceed 65535
int fourth_digit = 0;
int fifth_digit = 0;
int sixth_digit = 0;
int decimal_point = 0;
int key_pressed_number = 0;     // the number of the key pressed,

int divider = 1;               //1,2,3 - this is then used to calculate divide_by
int divide_by = 1;             //1,10,100,1000 - this is used to divide the "unsigned long degree_digit_value" to obtain 
//                             //the "float fp_degree_digit_value" which is then displayed

//Serial_lcd setup
#include <SoftwareSerial.h>
#define CR 250  // set values for CR = command Register for the LCD
#define CD 1    // set value for CD = command Data for the LCD

const int rxpin = 15;           // pin used to receive (not used in this version) 
const int txpin = 14;           // pin used to send to LCD
const byte line_1 = 0x80;       // Move cursor to the first line of the LCD
const byte line_2 = 0xC0;       // Move cursor to the second line of the LCD
const byte blink_ON = 0x0F;     // cursor blink on
const byte blink_OFF = 0x0C;    // cursor blink off
SoftwareSerial serial_lcd(rxpin, txpin); // new serial port on pins 2 and 3




// keymap defines the character returned when the corresponding key is pressed
const char keymap[numRows][numCols] = {
  { 
    '1', '2', '3', 'A'                                                                                                                                                                                                                                                                      } 
  ,
  { 
    '4', '5', '6', 'B'                                                                                                                                                                                                                                                                      } 
  ,
  { 
    '7', '8', '9', 'C'                                                                                                                                                                                                                                                                      } 
  ,
  { 
    'I', '0', '.', 'D'                                                                                                                                                                                                                                                                      } 
  ,
  { 
    'E', 'F', 'G', 'H'                                                                                                                                                                                                                                                                      } 
  ,
};

// this array determines the Arduino pins used for the rows and columns
const int rowPins[numRows] = { 
  2, 3, 4, 5, 6 }; // Rows 0 through 4
const int colPins[numCols] = { 
  7, 8, 9, 10 };    // Columns 0 through 3
//================================================================================================


//++++++++++++++++++++++++++++++++++++++ Start of program setup ++++++++++++++++++++++++++++++++++
void setup()
{
  //Serial.begin(9600);
  serial_lcd.begin(9600); //initialize the software serial port also for 9600
  lcd_clear();
  serial_lcd.print("Welcome to the");
  lcd_line2();
  serial_lcd.print("FP keypad V1");
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
    if (finished == true)     // this sets the key to the reset key so we can start again
    {                         // it is only used when gear cutting is finished
      key = 'B';              // 'B' is the reset key
      finished = false;       // finished is reset to its initial state
    }
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
      serial_lcd.print(".");
      decimal_point_active = true;
      decimal_point = key_pressed_number;
      decimal_point_check();
      if (select_step || select_gear == true)
      {
        lcd_line1(); 
        serial_lcd.print("Decimal not OK");
        lcd_line2();
        serial_lcd.print("for Step or Gear");
        delay(2000);
        set_all_to_0();

      }
      break;
    case 'A':
      lcd_clear();
      serial_lcd.print("OK");    // OK is selected when the number we want is correct.
   
      if (decimal_point_active == true)   
      {
        decimal_point_place();
      }

      display_result();
      check_range();
      break;
    case 'B':                  // Resets all the parameters to their initial state
      lcd_clear();
      serial_lcd.print("Reset");
      set_all_to_0();
      fp_selected();
      break; 
    case 'C':                  // 
          lcd_clear();
      serial_lcd.print("Menu C");
       // procedure_C();
      break;
    case 'D':                  //
       lcd_clear();
      serial_lcd.print("Menu D");
      // procedure_D();
      break;
    case 'E':  
   lcd_clear();
      serial_lcd.print("Menu E");    
      // procedure_E();
      break;
    case 'F':                 // 
       lcd_clear();
      serial_lcd.print("Menu F");
      // procedure_F();
      break;
    case 'G':                 // 
       lcd_clear();
      serial_lcd.print("Menu G");
      // procedure_G();
      break;
    case 'H':                 // 
       lcd_clear();
      serial_lcd.print("Menu H");
      // procedure_H();
      break;
    case 'I':                 // 
       lcd_clear();
      serial_lcd.print("Menu I");
      // procedure_I();
      break;
    } 

    //========================= FP Value ==========================================================
    // Here we enter the digits for the floating point number
    // we start with a long integer value the decimal point will indicate the significance of the digits.


    switch (key_pressed_number)
    {
    case 1:  //if (key_pressed_number == 1)
      degree_digit_value = first_digit; // these are int so to preserve the range after calculations below we have to re-assign
      break;
    case 2:
      degree_digit_value = (first_digit * 10) + second_digit; 
      break;
    case 3:
      degree_digit_value = (first_digit * 100) + (second_digit * 10) + third_digit;  
      break;
    case 4:
      degree_digit_value = (first_digit * 1000) + (second_digit * 100) + (third_digit *10) + fourth_digit; 
      break;
    case 5:
      degree_digit_value = (first_digit * 10000) + (second_digit * 1000) + (third_digit *100) + (fourth_digit * 10) + fifth_digit;  
      break;
    case 6:
      degree_digit_value = (first_digit * 100000) + (second_digit * 10000) + (third_digit *1000) + (fourth_digit * 100) + (fifth_digit * 10) + sixth_digit;  
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
// Makes up the number for gear, step or degree from the keys pressed
int make_number()
{

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
  case 5:
    sixth_digit = key_value - '0'; //change second_digit to integer value from ascii
    key_pressed_number = key_pressed_number + 1;
    break;
  }

}
//============= Check to see that the gear/step/degree value is in range ==========================

void check_range()
{
  
  //============= Check to see that the degree value is in range ====================================
  // range for degrees entered is <360     
  else if (select_degree == true)
  {
    if (fp_degree_digit_value > 359.995) // change this to whatever value up to 999.999
    {
      lcd_clear();
      lcd_line1();
      serial_lcd.print("Too big >359.995");
      lcd_line2();
      serial_lcd.print("Start again");
      fp_selected();
      delay(2000);
      set_all_to_0();
    }
  }
}
//============================== decimal point check ==============================================
void decimal_point_check()
{
  if(decimal_point >= 4)
  {
    lcd_clear();                          //Clear lcd screen
    serial_lcd.print("Too many digits");  // Write this on the first line
    lcd_line2();                          // go to line 2
    serial_lcd.print("before decimal");   // write this on line 2
    delay(2000);                          // wait 2 seconds to allow display to be viewed
    lcd_clear();                          // clear the display
    serial_lcd.print("Start again");      // Write thi on the first line
    delay(2000);                          // wait 2 seconds to allow display to be viewed
    set_all_to_0();
  }
}
//============================== decimal point place ==============================================
void decimal_point_place()          // this is only used in degree mode
{
  if (decimal_point == 0 && key_pressed_number == 1) // this is a unique case, no decimal point is being entered
  {
    divide_by = 10;
    return;
  }
  divider = key_pressed_number - decimal_point; // turns out that this works well to create the decimal point position for all cases
  //                                            //other than the case where no decimal poinbt is entered
  switch (divider)  
  {
  case 0:
    divide_by = 1;
    break;
  case 1:
    divide_by = 10;
    break;
  case 2:
    divide_by = 100;
    break;
  case 3:
    divide_by = 1000;
    break;
  }
}
//=========================== display_result ======================================================
void display_result()
{
 
    fp_degree_digit_value = degree_digit_value;
    fp_degree_digit_value = fp_degree_digit_value / divide_by;
    // serial_lcd.print("Number of int degrees = ");
    // serial_lcd.print(degree_digit_value); 
    lcd_clear();
    serial_lcd.print("Number of deg.= ");
    lcd_line2();
    serial_lcd.print(fp_degree_digit_value,3);
    delay(2000);   //Wait a couple of seconds to view last message
  
  //serial_lcd.print(memoryFree()); //Switched on occasionally to ensure that memory space is adequate
}


//========================Clear LCD Screen ========================================================
//
void lcd_clear() // clears the LCD screen and go to line 1, make cursor blink
{
  serial_lcd.write(CR);
  serial_lcd.write(1);
  serial_lcd.write(CR);
  serial_lcd.write(line_1);
  serial_lcd.write(CR);
  serial_lcd.write(blink_ON);

}
//=================================================================================================
//=======================Move to LCD line 1========================================================
//
void lcd_line1()
{
  serial_lcd.write(CR);
  serial_lcd.write(line_1); 
}
//=================================================================================================
//=======================Move to LCD line 1========================================================
void lcd_line2()
{
  serial_lcd.write(CR);
  serial_lcd.write(line_2); 
}
//=================================================================================================
//=====================================Free Ram ===================================================
// function to return the amount of free RAM 
// variables created by the build process when compiling the sketch
/*
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
 }
 */
/*//========================= test function =================================================     

void here_now() // used to test the logic of the software, a breakpoint.
{
  lcd_clear();
  lcd_line1();
  serial_lcd.print("Here now"); 
}
*/
//=========================================================================================     
// These procedures initiliase values for Step, Degree or Gear selection.

boolean fp_selected() // Here we are in Degree mode, keep this until reset or changed
{

  select_degree = true;

  first_digit = 0;
  second_digit = 0;
  third_digit = 0;
  fourth_digit = 0;
  fifth_digit = 0;
  key_pressed_number = 0;
  digit_value = 0;
  degree_digit_value = 0;
  fp_degree_digit_value = 0; 
  decimal_point = 0;
  divide_by = 1;
  decimal_point_active = false;
  current_tooth = 1;
  clockwise_direction = true;
}

//============================== set_all_to_0 ======================================================
// Resets values to initial states to start again.
void set_all_to_0() 
{
  lcd_clear();
  serial_lcd.print("Re-Start, select");
  lcd_line2();
  serial_lcd.print("Gear Step Degree");
  key_pressed_number = 0;
  key_value = 0;
  first_digit = 0;
  second_digit = 0;
  third_digit = 0;
  fourth_digit = 0;
  fifth_digit = 0;
  sixth_digit = 0;
  digit_value = 0;
  degree_digit_value = 0; 
  fp_degree_digit_value = 0; 
  select_step = false; 
  select_degree = false; 
  select_gear = false; 
  decimal_point = 0;
  divide_by = 1;
  decimal_point_active = false;
  current_tooth = 1;
  clockwise_direction = true;
  
}
//+++++++++++++++++++++++++  End of program ++++++++++++++++++++++++++++++++++++++++++++++++++++++++

































































































































