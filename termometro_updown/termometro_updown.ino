// written by Dylon Jamna (ME!)
// include the library code
#include <EEPROM.h>
#include <LiquidCrystal.h>// include the library code
int tempPin = A0;  // make variables// thermistor is at A0
int led =13; // led is at pin
float temp;  // make a variable called temp
float settemp; // make a variable called temp
int swtu = 7;  // switch up is at pin 7
int swtd = 6;   // switch down is at pin 6
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // lcd is at 12,11,5,4,3,2

void setup() {
  pinMode (led,1);  // make led or pin13 an output
  Serial.begin (9600); // set the serial monitor tx and rx speed
  lcd.begin(16, 2); // set up all the "blocks" on the display
  lcd.setCursor(0,0); // set the cursor to colum 0 row 0
  lcd.print("hello, world!"); // display hello world for 1 second
  lcd.clear(); // clear the lcd
  EEPROM.read (1); // make the eeprom or atmega328 memory address 1
}

void loop() {

  int tvalue = analogRead(tempPin);  // make tvalue what ever we read on the tempPin
  float temp = (tvalue / 6.388888888889); // the math / conversion to temp
  lcd.setCursor (0,0); // set the cursor to 0,0
  lcd.print (temp);  // Print the current temp in f
  lcd.print ('F');
  Serial.println (temp);  // print the temp it the serial monitor
  settemp = EEPROM.read(1); // read the settemp on the eeprom

  delay (250); // wait for the lcd to refresh every 250 milliseconds
 
  if             // if we se the switch up pin reading on 1 or 5 volts
    (digitalRead(swtu)== 1 )
  {
    settemp ++  // add one to the settemp, the settemp is the ideal temperature for you
   

      ;
  }

else{// other wise do nothing

}

if
(digitalRead (swtd) == 1)// if we detect a 1 on the other switch pin
{
  (settemp --);// subtract one fromm the settemp
 
}
else {
// else, do nothing
}

if (temp > settemp) // if the temperature exceeds your chosen settemp
{
  digitalWrite (led, 1); // turn on the led
}
else // if that doesn't happen, then turn the led off
{
  digitalWrite (led,0);
}
}
