#include <OneWire.h>


int calorPino = 11;
int RelePino = 10;
int temperatutaRampa = 19;
int tempMatura = 0;
OneWire ds(calorPino);
#define NUMLEITURAS 10

float j=0;
int leiturasMat[NUMLEITURAS];
int indexMat = 0;
int totalMat = 0;
int media = 0;

/*
int32_t last_action_time = 0;

void media(int tempMatura){
if(millis() - last_action_time < millis() + 200){
j+=1;
tenTot1 = tenTot1 + tempMatura; //add up readings


valorMatura= (tenTot1 /j ); //divide the total
}
if(millis() - lastConnectionTime >= millis() + 200){
tenTot1 = 0; //reset total variable
j=0;
}
}
*/
void setup() {
   Serial.begin(9600);
   pinMode(RelePino, OUTPUT);
   for (int i = 0; i < NUMLEITURAS; i++)
    leiturasMat[i] = 0;
   
}

void loop()
{
  totalMat -= leiturasMat[indexMat];               // subtrair a última leitura
  leiturasMat[indexMat] = getTemp(); // ler do sensor
  totalMat += leiturasMat[indexMat];               // adicionar leitura ao total
  indexMat = (indexMat + 1);                    // avançar ao próximo índice

  if (indexMat >= NUMLEITURAS){               // se estiver no fim do vetor...
    indexMat = 0;       // ...meia-volta ao início
  }
  media = totalMat / NUMLEITURAS;          // calcular a média
   

if(media>temperatutaRampa) {
digitalWrite(RelePino, HIGH);
//delay(15000);
//digitalWrite(RelePino, LOW);
}
else
{
  digitalWrite(RelePino, LOW);
}

Serial.print("A temperatura igual a:");
Serial.println(media);

 


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
//last_action_time = millis();
return Temperature;
}
