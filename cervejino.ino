#include <OneWire.h>


int calorPino = 11;
int RelePino = 10;
int temperatutaRampa = 10;
int tempMatura = 0;
OneWire ds(calorPino);


void setup() {
   
   pinMode(RelePino, OUTPUT);
   Serial.begin(9600);
}

void loop()
{
tempMatura = getTemp(); 

if(tempMatura>temperatutaRampa) {
digitalWrite(RelePino, HIGH);
//delay(15000);
//digitalWrite(RelePino, LOW);
}
else
{
  digitalWrite(RelePino, LOW);
}

Serial.print("A temperatura igual a:");
Serial.println(tempMatura);

 


delay(1000);
}



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
