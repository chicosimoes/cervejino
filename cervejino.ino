/*
Codigo feito por Chico Simes para controlar fermentadores, maturadores e 
camaras frias para fermentacao, a principio, de cerveja.

Ciclo de hysteresis inspirado de mvencelj in instructables;


display com botoes de controle de temperaturas com botoes para ajuste de alvos.





*/

#include <OneWire.h>
#include <DallasTemperature.h>
//#include <EEPROM.h>


unsigned long ultimavez;
int comandoFer;
int comandoMat;//variavel que vai conter o estado do que fazer
float ultimaTempFer;
float ultimaTempMat;
float temperaturaFermentacao = 17;
float temperaturaMaturacao = 14;
//float temperaturaMaturacao = EEPROM.read(1);//trashoad de maturacao
//float temperaturaFermentacao = EEPROM.read(2);//trashoad de fermentacao
//float Tf = EEPROM.read(1);     //temperatura de fermentacao definida pelos botoes buttonI and buttonD
float Th = 1.0;      //hysteresis
//float Tfr;          //chamber temperature/temperatura da camara de fermentacao

int botaoFermenos = 6;  //button which decrease desired T / botao menos fermentacao
int botaoFermais = 9;  //button which increase desired T / botao mais fermentador
int botaoMatmenos = 1;  //button which decrease desired T / botao menos fermentacao
int botaoMatmais = 3;  //button which increase desired T / botao mais fermentador


int ReleMatPino = 10;
int ReleFerPino = 2;
int ReleAquePino = 7;

#define ONE_WIRE_BUS_1 4 // define o pino do termometro de maturacao
#define ONE_WIRE_BUS_2 5 // define o pino do termometro de fermentacao

OneWire oneWire_MAT(ONE_WIRE_BUS_1);
OneWire oneWire_FER(ONE_WIRE_BUS_2);
DallasTemperature sensor_MAT(&oneWire_MAT); //cria as variaveis do sensor
DallasTemperature sensor_FER(&oneWire_FER); //cria as variaveis do sensor



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
  sensor_FER.setWaitForConversion(false);
  sensor_MAT.setWaitForConversion(false);
  
   Serial.begin(9600);
   pinMode(ReleMatPino, OUTPUT);
   pinMode(ReleFerPino, OUTPUT);
   pinMode(ReleAquePino, OUTPUT); 
   
   ultimavez = millis();
   
   for (int i = 0; i < NUMLEITURAS; i++)
    leiturasMat[i] = 0;
    
    for (int i = 0; i < NUMLEITURAS; i++)
    leiturasFer[i] = 0;
   
}

void loop()
{
    int estado_botaoFer;
    int estado_botaoMat;
//  if((millis()-ultimavez)>800){
  //     Tc = sensors.getTempCByIndex(0); 
 //      sensors.requestTemperatures(); //Send the command to get temperatures.
      
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
// ultimavez = millis();
 //}
///////////////////////////printa resultados////////////////
 //Serial.setCursor(0, 0);
  Serial.print("Fer. Atual=");
  Serial.print(mediaFer,2); 
  Serial.print("C ");
  //Serial.setCursor(0, 1);
  Serial.print("Fer. Alvo=");
  Serial.print(temperaturaFermentacao,2);
  Serial.println("C ");
 //Serial.setCursor(0, 0);
  Serial.print("Mat. Atual=");
  Serial.print(mediaMat,2); 
  Serial.print("C ");
  //Serial.setCursor(0, 1);
  Serial.print("Mat. Alvo=");
  Serial.print(temperaturaMaturacao,2);
  Serial.println("C ");





//////////////////////////////////////checa botoes//////////////
/*
  int downFer = 0;
  estado_botaoFer=digitalRead(botaoFermenos); 
  if(estado_botaoFer==0) {       
       temperaturaFermentacao = temperaturaFermentacao - 0.5;
//       EEPROM.write(2, temperaturaFermentacao);
       downFer = 1;
                     }
  estado_botaoFer=digitalRead(botaoFermais);
  if(estado_botaoFer==0) {
       temperaturaFermentacao = temperaturaFermentacao + 0.5;
//        EEPROM.write(2, temperaturaFermentacao);
       downFer = 1;  
                     }
  if (downFer) {     //for faster/better button response
       delay(200);
           }

  int downMat = 0;
  estado_botaoMat=digitalRead(botaoMatmenos); 
  if(estado_botaoMat==0) {       
       temperaturaMaturacao = temperaturaMaturacao - 0.5;
//       EEPROM.write(2, temperaturaMaturacao);
       downMat = 1;
                     }
  estado_botaoMat=digitalRead(botaoMatmais);
  if(estado_botaoMat==0) {
       temperaturaMaturacao = temperaturaMaturacao + 0.5;
//        EEPROM.write(2, temperaturaMaturacao);
       downMat = 1;  
                     }
  if (downMat) {     //for faster/better button response
       delay(200);
           }
           
*/           
//////////////////define o que fazer com as leituras de temperaturas//////////////

 if(mediaFer<=(temperaturaFermentacao-Th/2)){
          digitalWrite(7, HIGH);
          digitalWrite(2, LOW); 
   //   comandoFer = 1; 
 }      
 if(mediaFer>=(temperaturaFermentacao+Th/2)){
       digitalWrite(2, HIGH);
       digitalWrite(7, LOW);
     //  comandoFer = -1;
 }     
 if(mediaFer>temperaturaFermentacao && ultimaTempFer<=temperaturaFermentacao) { //hysteresis
       digitalWrite(2, LOW);
       digitalWrite(7, LOW);
      
      // comandoFer = 0; 
 }   
 if(mediaFer=temperaturaFermentacao) { //hysteresis
       digitalWrite(2, LOW);
       digitalWrite(7, LOW);
 //      comandoFer = 0; 
                       }
 ultimaTempFer = mediaFer;

//if(mediaMat<=(temperaturaMaturacao-Th/2)){
//             digitalWrite(10, LOW);
 // comandoMat = 10; 
//                 }
 if(mediaMat>=(temperaturaMaturacao+Th/2)){
  digitalWrite(10, HIGH);    
  // comandoMat = -10;
 }
   else{
//   }
 //if(mediaMat>temperaturaMaturacao && ultimaTempMat<=temperaturaMaturacao) { //hysteresis
              digitalWrite(10, LOW);
 //comandoMat = 100; 
                       }
// if(mediaMat=temperaturaMaturacao) { //hysteresis
//             digitalWrite(10, LOW);
 // comandoMat = 100; 
     //                  }
 ultimaTempMat = mediaMat;
/*
///////////////////aciona os reles conforme definido///////////////////////
 ///Serial.setCursor(9, 0);  
 if(comandoFer==-1) { //cooling
       digitalWrite(2, HIGH);
       digitalWrite(7, LOW);
 //      Serial.print("Fermentador GELANDO");   
               }
 if(comandoFer==1){ //heating
       digitalWrite(7, HIGH);
       digitalWrite(2, LOW);
   //    Serial.print("Fermentador ESQUENTANDO");
             }
 if(comandoFer==0) { //off
       digitalWrite(2, LOW);
       digitalWrite(7, LOW); 
   //    Serial.print("Fermentador DI BOA");
             }
 if(comandoMat==-10) { //cooling
       digitalWrite(10, HIGH);
  //     digitalWrite(7, LOW);
  //     Serial.print("Maturador GELANDO");   
               }
 if(comandoMat==10){ //heating
//       digitalWrite(7, HIGH);
       digitalWrite(10, LOW);
 //      Serial.print("Maturador ESQUENTANDO");
             }
 if(comandoMat==100) { //off
       digitalWrite(10, LOW);
 //      digitalWrite(7, LOW); 
 //      Serial.print("Maturador DI BOA");
       }

*/

delay(1000);


}
/*
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
