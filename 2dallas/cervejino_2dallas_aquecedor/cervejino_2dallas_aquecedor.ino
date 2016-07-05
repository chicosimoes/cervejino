/*
Codigo feito por Chico Simes para controlar fermentadores, maturadores e 
camaras frias para fermentacao, a principio, de cerveja.

Ciclo de hysteresis inspirado de mvencelj in instructables;


display com botoes de controle de temperaturas com botoes para ajuste de alvos.





*/

#include <OneWire.h>
#include <DallasTemperature.h>


//int TermMatPino = 4;
int ReleMatPino = 10;
//int TermFerPino = 5;
int ReleFerPino = 2;
int ReleAquePino = 7;

#define ONE_WIRE_BUS_1 4 // define o pino do termometro de maturacao
#define ONE_WIRE_BUS_2 5 // define o pino do termometro de fermentacao

OneWire oneWire_MAT(ONE_WIRE_BUS_1);
OneWire oneWire_FER(ONE_WIRE_BUS_2);
DallasTemperature sensor_MAT(&oneWire_MAT); //cria as variaveis do sensor
DallasTemperature sensor_FER(&oneWire_FER); //cria as variaveis do sensor

int temperaturaMaturacao = 14;//trashoad de maturacao
int temperaturaFermentacao = 17;//trashoad de fermentacao

//int tempMatura = 0;
//OneWire ds(TermMatPino);
///////////sistema de medias///////////////////////

#define NUMLEITURAS 10

//float j=0;
float leiturasMat[NUMLEITURAS];
int indexMat = 0;
float totalMat = 0;
float mediaMat = 0;

float leiturasFer[NUMLEITURAS];
int indexFer = 0;
float totalFer = 0;
float mediaFer = 0;

//////////////////////////////////////////////////////

void setup() {
  
  sensor_MAT.begin();
  sensor_FER.begin();
  
   Serial.begin(9600);
   pinMode(ReleMatPino, OUTPUT);
   pinMode(ReleFerPino, OUTPUT);
   pinMode(ReleAquePino, OUTPUT); 
   
   for (int i = 0; i < NUMLEITURAS; i++)
    leiturasMat[i] = 0;
    
    for (int i = 0; i < NUMLEITURAS; i++)
    leiturasFer[i] = 0;
   
}

void loop()
{
  
  //////////////tira a media maturacao////////////////
  sensor_MAT.requestTemperatures();// solicita a leitura do sensor
  totalMat -= leiturasMat[indexMat];               // subtrair a última leitura
  leiturasMat[indexMat] = sensor_MAT.getTempCByIndex(0); // ler do sensor
  totalMat += leiturasMat[indexMat];               // adicionar leitura ao total
  indexMat = (indexMat + 1);                    // avançar ao próximo índice

  if (indexMat >= NUMLEITURAS){               // se estiver no fim do vetor...
    indexMat = 0;       // ...meia-volta ao início
  }
  mediaMat = totalMat / NUMLEITURAS;          // calcular a média
  
  //////////////////////////tira a media fermentacao////////////////////////////////////////////
  sensor_FER.requestTemperatures();//solicita a leitura do sensor
  totalFer -= leiturasFer[indexFer];               // subtrair a última leitura
  leiturasFer[indexFer] = sensor_FER.getTempCByIndex(0); // ler do sensor
  totalFer += leiturasFer[indexFer];               // adicionar leitura ao total
  indexFer = (indexFer + 1);                    // avançar ao próximo índice

  if (indexFer >= NUMLEITURAS){               // se estiver no fim do vetor...
    indexFer = 0;       // ...meia-volta ao início
  }
  mediaFer = totalFer / NUMLEITURAS;          // calcular a média
 

//mediaMat = sensor_MAT.getTempCByIndex(0);


 
   //////////////////////////////////////

if(mediaMat>temperaturaMaturacao) {
digitalWrite(ReleMatPino, HIGH);
//delay(15000);
//digitalWrite(ReleMatPino, LOW);
}
else
{
  digitalWrite(ReleMatPino, LOW);
}




if(mediaFer>temperaturaFermentacao+1) {
digitalWrite(ReleFerPino, HIGH);
//delay(15000);
//digitalWrite(ReleMatPino, LOW);
}
else
{
  digitalWrite(ReleFerPino, LOW);
}

if(mediaFer<temperaturaFermentacao) {
digitalWrite(ReleAquePino, HIGH);
//delay(15000);
//digitalWrite(ReleMatPino, LOW);
}
else
{
  digitalWrite(ReleAquePino, LOW);
}

Serial.print("Temperatura do maturador:");
Serial.println(mediaMat, 2);
Serial.print("Temperatura do fermentador:");
Serial.println(mediaFer, 2);

 


delay(1000);
}


/*
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
*/
