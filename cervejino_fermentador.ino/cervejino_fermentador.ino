/*
 Controlador de sistema de refrigeracao para fabricacao de cerveja caseira
 
 */
 #include <OneWire.h>
#include <LiquidCrystal.h> 
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); 
int ledBad = 13;
int temperturaPino = 20;
int bombaPino = 6;
int temperatutaRampa = 27;
//inicio float temperatura

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

//fim float temperatura
// the setup routine runs once when you press reset:
void setup() {   
lcd.begin(16, 2); //Inicia o LCD com dimensões 16x2(Colunas x Linhas)
lcd.setCursor(0, 0); //Posiciona o cursor na primeira coluna(0) e na primeira linha(0) do LCD
lcd.print("Bom dia cervejeiro!"); //Escreve no LCD "Olá Garagista!"
lcd.setCursor(0, 1); //Posiciona o cursor na primeira coluna(0) e na segunda linha(1) do LCD
lcd.print("Vamos aa luta!"); //Escreve no LCD "Luz"
delay(5000);
lcd.setCursor(0, 0); //Posiciona o cursor na primeira coluna(0) e na primeira linha(0) do LCD
lcd.print("Temp atual:"); //Escreve no LCD "Olá Garagista!"
lcd.setCursor(0, 1); //Posiciona o cursor na primeira coluna(0) e na segunda linha(1) do LCD
lcd.print("Alvo:"); //Escreve no LCD "Luz"

  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  pinMode(ledBad, OUTPUT);
  pinMode(bombaPino, OUTPUT);
  
}

// the loop routine runs over and over again forever:
void loop() {
  
  valorcalor = getTemp(); 

if(valorcalor>temperatutaRampa) {
digitalWrite(bombaPino, HIGH);
digitalWrite(ledBad, HIGH);
}
else
{
  digitalWrite(bombaPino, LOW);
  digitalWrite(ledBad, LOW);
}

  Serial.print("A temperatura igual a:");
  Serial.println(valorcalor);
  lcd.setCursor(13, 0);
  lcd.println(valorcalor);  
  lcd.setCursor(13, 1);
  lcd.println(temperatutaRampa);
  
       // delay in between reads for stabilit
 
  delay(1000); 

}
