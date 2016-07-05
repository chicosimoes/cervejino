/*
Codigo feito por Chico Simes para controlar fermentadores, maturadores e 
camaras frias para fermentacao, a principio, de cerveja.

display com botoes de controle de temperaturas com botoes para ajuste de alvos.





*/

#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>

//int TermMatPino = 4;
int ReleMatPino = 10;
//int TermFerPino = 5;
int ReleFerPino = 2;
int ReleAquePino = 7;

unsigned long ultimavez;
int comandoFer;
int comandoMat;//variavel que vai conter o estado do que fazer
float ultimaTempFer;
float ultimaTempMat;
float temperaturaMaturacao; //= 14;//trashoad de maturacao
float temperaturaFermentacao; //= 27;//trashoad de fermentacao

int botaoFermenos = 6;  //button which decrease desired T / botao menos fermentacao
int botaoFermais = 9;  //button which increase desired T / botao mais fermentador
int botaoMatmenos = 1;  //button which decrease desired T / botao menos fermentacao
int botaoMatmais = 3;  //button which increase desired T / botao mais fermentador

#define ONE_WIRE_BUS_1 4 // define o pino do termometro de maturacao
#define ONE_WIRE_BUS_2 5 // define o pino do termometro de fermentacao

OneWire oneWire_MAT(ONE_WIRE_BUS_1);
OneWire oneWire_FER(ONE_WIRE_BUS_2);
DallasTemperature sensor_MAT(&oneWire_MAT); //cria as variaveis do sensor
DallasTemperature sensor_FER(&oneWire_FER); //cria as variaveis do sensor

LiquidCrystal lcd(A3,A2,A1,A0,13,12);

//int temperaturaMaturacao = 14;//trashoad de maturacao
//int temperaturaFermentacao = 17;//trashoad de fermentacao
float Th = 1.0;
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
   
  lcd.begin(16, 2); // set up all the "blocks" on the display
  lcd.setCursor(0,0); // set the cursor to colum 0 row 0
  lcd.print("VIVA A VIDA!"); // display hello world for 1 second
  lcd.clear(); // clear the lcd
  EEPROM.read (1); // make the eeprom or atmega328 memory address 1
  EEPROM.read (2); // make the eeprom or atmega328 memory address 2
    
   for (int i = 0; i < NUMLEITURAS; i++)
    leiturasMat[i] = 0;
    
    for (int i = 0; i < NUMLEITURAS; i++)
    leiturasFer[i] = 0;
   
}

void loop()
{
temperaturaMaturacao = EEPROM.read(1);
temperaturaFermentacao = EEPROM.read(2);
    
 //////////////////////////////////////checa botoes//////////////


  if (digitalRead(botaoFermenos)==1) //se tem corrente no botao(apertado)
    {     
       (temperaturaFermentacao --);//adiciona um
       EEPROM.write(2, temperaturaFermentacao);//grava na memoria 

                     }

    
  if (digitalRead(botaoFermais)==1)//botao apertado
 {
       (temperaturaFermentacao ++);//adiciona um
        EEPROM.write(2, temperaturaFermentacao);//grava
    
                     }



if (digitalRead(botaoMatmenos)==1)
{       
       (temperaturaMaturacao --);
       EEPROM.write(1, temperaturaMaturacao);
     
                     }
if (digitalRead(botaoMatmais)==1)
  {
       (temperaturaMaturacao ++);
        EEPROM.write(1, temperaturaMaturacao);
 
                     }
 
   delay(200);      
 
           
  
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

if(mediaMat>temperaturaMaturacao+Th/2) {
digitalWrite(ReleMatPino, HIGH);
//delay(15000);
//digitalWrite(ReleMatPino, LOW);
}
else
{
  digitalWrite(ReleMatPino, LOW);
}




if(mediaFer>temperaturaFermentacao+Th/2) {
digitalWrite(ReleFerPino, HIGH);
//delay(15000);
//digitalWrite(ReleMatPino, LOW);
}
else
{
  digitalWrite(ReleFerPino, LOW);
}

if(mediaFer<temperaturaFermentacao-Th/2) {
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
Serial.print("Alvo maturador:");
Serial.println(temperaturaMaturacao, 2);
Serial.print("Alvo fermentador:");
Serial.println(temperaturaFermentacao, 2);
  lcd.setCursor(0, 0);
  lcd.print("F");
  lcd.print(mediaFer,1); 
  lcd.print("C ");
  lcd.print("M");
  lcd.print(mediaMat,1);
  lcd.print("C ");
  
  lcd.setCursor(0, 1);
   lcd.print("Ft");
  lcd.print(temperaturaFermentacao,0); 
  lcd.print("C ");
  lcd.print("Mt");
  lcd.print(temperaturaMaturacao,0);
  lcd.print("C ");


 


//delay(1000);
}



