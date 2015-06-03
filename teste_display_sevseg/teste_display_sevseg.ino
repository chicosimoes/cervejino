
 #include "SevSeg.h"
 SevSeg sevseg; //Instantiate a seven segment object
 
 void setup() {
   byte numDigits = 4;
   byte digitPins[] = {11, 10, 9, 6};
   byte segmentPins[] = {A1, 3, 4, 5, A0, 7, 8};
   sevseg.begin(COMMON_ANODE, numDigits, digitPins, segmentPins);
   
}
 void loop(){
   sevseg.setNumber(3141,0); // Displays '3.141'
   sevseg.refreshDisplay();
   sevseg.setBrightness(50);
 }
