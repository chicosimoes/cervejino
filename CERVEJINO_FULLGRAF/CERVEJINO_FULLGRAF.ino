


#include "U8glib.h"
#include <OneWire.h>


int calorPino = 11;
int RelePino = 10;
int temperatutaRampa = 10;
int tempMatura = 0;
OneWire ds(calorPino);
char str[4];


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

U8GLIB_ST7920_128X64_1X u8g( 6, 5, 4, U8G_PIN_NONE);   // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, di=17,rw=16

#define KEY_NONE 0
#define KEY_PREV 1
#define KEY_NEXT 2
#define KEY_SELECT 3
#define KEY_BACK 4


// DOGM128-Shield	 configuration values
// DOGXL60-Shield configuration values
uint8_t uiKeyPrev = A0;
uint8_t uiKeyNext = A1;
uint8_t uiKeySelect = A2;
uint8_t uiKeyBack = 12;

uint8_t uiKeyCodeFirst = KEY_NONE;
uint8_t uiKeyCodeSecond = KEY_NONE;
uint8_t uiKeyCode = KEY_NONE;

/*
void uiSetup(void) {
  // configure input keys 
  
  pinMode(uiKeyPrev, INPUT);           // set pin to input
  digitalWrite(uiKeyPrev, HIGH);       // turn on pullup resistors
  pinMode(uiKeyNext, INPUT);           // set pin to input
  digitalWrite(uiKeyNext, HIGH);       // turn on pullup resistors
  pinMode(uiKeySelect, INPUT);           // set pin to input
  digitalWrite(uiKeySelect, HIGH);       // turn on pullup resistors
  pinMode(uiKeyBack, INPUT);           // set pin to input
  digitalWrite(uiKeyBack, HIGH);       // turn on pullup resistors
}

void uiStep(void) {
  uiKeyCodeSecond = uiKeyCodeFirst;
  if ( digitalRead(uiKeyPrev) == LOW )
    uiKeyCodeFirst = KEY_PREV;
  else if ( digitalRead(uiKeyNext) == LOW )
    uiKeyCodeFirst = KEY_NEXT;
  else if ( digitalRead(uiKeySelect) == LOW )
    uiKeyCodeFirst = KEY_SELECT;
  else if ( digitalRead(uiKeyBack) == LOW )
    uiKeyCodeFirst = KEY_BACK;
  else 
    uiKeyCodeFirst = KEY_NONE;
  
  if ( uiKeyCodeSecond == uiKeyCodeFirst )
    uiKeyCode = uiKeyCodeFirst;
  else
    uiKeyCode = KEY_NONE;
}
*/

int display = 1;  
  
void u8g_prepare() 
{  
  u8g.setFont(u8g_font_4x6);  
  u8g.setFontRefHeightExtendedText();  
  u8g.setDefaultForegroundColor();  
  u8g.setFontPosTop();  
} 

////////////////////////////////////////////////////tela1/////////////////////////////
void u8g_Tela1()  //Tela 1 -  
{
  u8g.setFont(u8g_font_unifont);  
  u8g.drawStr( 74, 15, "MATURA");  
  u8g.drawStr( 74, 15, "MATURA");  
  u8g.drawFrame(64,0,64,64);  
  u8g.drawFrame(66,2,60,60);
  u8g.drawStr( 75, 35, dtostrf(tempMatura, 5, 2, str));
  u8g.drawStr( 90, 55, "\260C"); 
}  
void draw() //Rotina Desenho  
{
  u8g_prepare();    
    u8g_Tela1(); 
}

void drawTest(void) {
  u8g.setFont(u8g_font_unifont);
  u8g.drawStr( 0, 20, "INICIANDO");
}

void checaRele(){
  /////ACIONAMENTO DO RELE//////////
  

}

void setup() {

   Serial.begin(9600);
    pinMode(RelePino, OUTPUT);
   
 u8g.firstPage();  
  do {
    drawTest();
  } while( u8g.nextPage() ); 
   
}


void loop() 
{ 
  tempMatura = getTemp(); 
delay(1000); 
if(tempMatura>temperatutaRampa) {
digitalWrite(RelePino, HIGH);
delay(15000);
digitalWrite(RelePino, LOW);
}
else
{
  digitalWrite(RelePino, LOW);
}

Serial.print("A temperatura igual a:");
Serial.println(tempMatura);

  delay(2000);
u8g.firstPage();  
  do {;
    draw();
  } while( u8g.nextPage() );

  


delay(1000); 
} 
