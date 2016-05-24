/*
  Controlador de camares de refrigerao para fabricao de Cerveja e outros fermentados
  usando 2 sensores DS18B20 
  
  autor: chicosimoes
  20/5/2016
  
  baseado em exemplos das libs usadas
 
   
*/

#include "U8glib.h"
#include <OneWire.h>
#include <DallasTemperature.h>

int temperaturaMat = 10; // define a temperatira de trashroad para maturacao
int RelePinoMat = 10;  // define quel o pino do rele de maturacao

int temperaturaFer = 19; // define a temperatira de trashroad para fermentacao
int RelePinoFer =7;  // define quel o pino do rele de fermentacao


#define NUMLEITURAS 3


float leiturasMat[NUMLEITURAS];
int indexMat = 0;
int totalMat = 0;
int mediaMat = 0;  

#define ONE_WIRE_BUS_1 11
#define ONE_WIRE_BUS_2 12

OneWire oneWire_MAT(ONE_WIRE_BUS_1);
OneWire oneWire_FER(ONE_WIRE_BUS_2);

DallasTemperature sensor_MAT(&oneWire_MAT);
DallasTemperature sensor_FER(&oneWire_FER);


float leiturasFer[NUMLEITURAS];
int indexFer = 0;
int totalFer = 0;
int mediaFer = 0;

U8GLIB_ST7920_128X64_1X u8g( 6, 5, 4, U8G_PIN_NONE);

uint8_t draw_lcd(void)  //Maturacao-  
{
  u8g.setFont(u8g_font_unifont);  
  u8g.drawStr( 74, 15, "MATURA");    
  u8g.drawFrame(64,0,64,64);  
  float t = mediaMat;
  char temp[5];
  u8g.drawStr( 70, 35,dtostrf(t,3,1,temp));
  u8g.drawStr( 110, 35, "\260C"); 
 
  u8g.drawStr( 4, 15, "FERMENT");  
 
  u8g.drawFrame(0,0,64,64);  
    float f = mediaFer;
  u8g.drawStr( 5, 35,dtostrf(f,3,1,temp));
  u8g.drawStr( 45, 35, "\260C");

 
}


void medias(void){  
  totalMat -= leiturasMat[indexMat];               // subtrair a última leitura
  leiturasMat[indexMat] = sensor_MAT.getTempCByIndex(0); // ler do sensor
  totalMat += leiturasMat[indexMat];               // adicionar leitura ao total
  indexMat = (indexMat + 1);                    // avançar ao próximo índice

  if (indexMat >= NUMLEITURAS){               // se estiver no fim do vetor...
    indexMat = 0;       // ...meia-volta ao início
  }
  mediaMat = totalMat / NUMLEITURAS;          // calcular a média

      //////
    totalFer -= leiturasFer[indexFer];               // subtrair a última leitura
  leiturasFer[indexFer] = sensor_FER.getTempCByIndex(0) ; // ler do sensor
  totalFer += leiturasFer[indexFer];               // adicionar leitura ao total
  indexFer = (indexFer + 1);                    // avançar ao próximo índice

  if (indexFer >= NUMLEITURAS){               // se estiver no fim do vetor...
    indexFer = 0;       // ...meia-volta ao início
  }
  mediaFer = totalFer / NUMLEITURAS;  // calcular a média

   
  
}

void u8g_prepare(void) {
  u8g.setFont(u8g_font_6x10);
  u8g.setFontRefHeightExtendedText();
  u8g.setDefaultForegroundColor();
  u8g.setFontPosTop();
}

void draw_graphics(void) {

    medias();
    mudaRele();
      u8g.firstPage();  
    do {
      draw_lcd();
       } while( u8g.nextPage() );
    
}

void mudaRele(void){
  
  if(mediaMat>temperaturaMat) {
digitalWrite(RelePinoMat, HIGH);
}
else
{
  digitalWrite(RelePinoMat, LOW);
}
  if(mediaFer>temperaturaFer) {
digitalWrite(RelePinoFer, HIGH);
}
else
{
  digitalWrite(RelePinoFer, LOW);
}
}


void setup(void) {
  
  // flip screen, if required
  //u8g.setRot180();
  // assign default color value
  if ( u8g.getMode() == U8G_MODE_R3G3B2 )
    u8g.setColorIndex(255);     // white
  else if ( u8g.getMode() == U8G_MODE_GRAY2BIT )
    u8g.setColorIndex(3);         // max intensity
  else if ( u8g.getMode() == U8G_MODE_BW )
    u8g.setColorIndex(1);         // pixel on 
  //u8g.setContrast(0x30); 
  pinMode(10, OUTPUT);          
  digitalWrite(10, HIGH); 
   pinMode(7, OUTPUT);          
  digitalWrite(7, HIGH); 
  u8g_prepare(); 
  
    sensor_MAT.begin();
    sensor_FER.begin();
  
  
  for (int i = 0; i < NUMLEITURAS; i++)
   leiturasMat[i] = 0;
    for (int i = 0; i < NUMLEITURAS; i++)
      leiturasFer[i] = 0;
}

void loop() {
  
     
    u8g.firstPage();  
    do {
      
      draw_graphics();
       } while( u8g.nextPage() );
    
    

}



