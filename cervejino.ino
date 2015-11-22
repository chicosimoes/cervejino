
#include <OneWire.h>

#include "SevSeg.h"
SevSeg displayTemp; //Instantiate a seven segment object

int temperturaPino = 2;
int buzinaPino = 11;
int temperatutaRampa = 40;
OneWire ds(temperturaPino);

int valorcalor = 0;

float getTemp(){
byte data[12];
byte addr[8];
if ( !ds.search(addr)) {
//no more sensors on chain, reset search
ds.reset_search();
return -1000;
}
if ( OneWire::crc8( addr, 7) != addr[7]) {
Serial.println("CRC is not valid!");
return -1000;
}
if ( addr[0] != 0x10 && addr[0] != 0x28) {
Serial.print("Device is not recognized");
return -1000;
}
ds.reset();
ds.select(addr);
ds.write(0x44,1);
byte present = ds.reset();
ds.select(addr);
ds.write(0xBE);
for (int i = 0; i < 9; i++) {
data[i] = ds.read();
}
ds.reset_search();
byte MSB = data[1];
byte LSB = data[0];
float TRead = ((MSB << 8) | LSB);
int Temperature = int(TRead / 16);
return Temperature;
}

void setup() {
   byte numDigits = 4;
   byte digitPins[] = {12, 10, 9, 6};
   byte segmentPins[] = {A1, 3, 4, 5, A0, 7, 8};
   displayTemp.begin(COMMON_ANODE, numDigits, digitPins, segmentPins);
   
   pinMode(buzinaPino, OUTPUT);
   Serial.begin(9600);
}

void loop()
{
valorcalor = getTemp(); 

if(valorcalor>temperatutaRampa) {
digitalWrite(buzinaPino, HIGH);
}
else
{
  digitalWrite(buzinaPino, LOW);
}

Serial.print("A temperatura igual a:");
Serial.println(valorcalor);

   displayTemp.setNumber(valorcalor,3); // Displays '3.141'
   displayTemp.refreshDisplay();
   displayTemp.setBrightness(50);


//delay(5);
}


