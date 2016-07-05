#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>

unsigned long lasttime;
int comand;
float oldTc;
float Td = EEPROM.read(1);     //temperaturadefinida pelos botoes buttonI and buttonD
float Th = 1.0;      //hysteresis
float Tc;          //chamber temperature/temperatura da camara
int buttonD = 9;  //button which decrease desired T / botao menos
int buttonI = 8;  //button which increase desired T / botao mais
int relay1 = 5;   
int relay2 = 4;

//FUNCTION WHICH READS CHAMBER TEMPERATURE
#define ONE_WIRE_BUS 6 // Temperature sensors data wire is plugged into pin 6 on the Arduino.
OneWire oneWire(ONE_WIRE_BUS);
//Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
LiquidCrystal lcd(A3,A2,A1,A0,13,12); 

void setup(void){ 
  lcd.begin(16, 2);
  delay(200);
  // Start up the library
  sensors.begin();
  sensors.setWaitForConversion(false);
  pinMode(relay1, OUTPUT); 
  pinMode(relay2, OUTPUT); 
  lasttime = millis();
}

void loop(void)
{
  int button_state;
  if((millis()-lasttime)>800){
       Tc = sensors.getTempCByIndex(0); 
       sensors.requestTemperatures(); //Send the command to get temperatures.
       lasttime = millis();
                            }
  lcd.setCursor(0, 0);
  lcd.print("F");
  lcd.print(Tc,1); 
  lcd.print("C ");
  lcd.setCursor(0, 1);
  lcd.print("T");
  lcd.print(Td,1);
  lcd.print("C ");
  int down = 0;
  button_state=digitalRead(buttonD); 
  if(button_state==0) {       
       Td = Td - 0.5;
       EEPROM.write(1, Td);
       down = 1;
                     }
  button_state=digitalRead(buttonI);
  if(button_state==0) {
       Td = Td + 0.5;
        EEPROM.write(1, Td);
       down = 1;  
                     }
  if (down) {     //for faster/better button response
       delay(200);
           }

//SELECTING BETWEEN HEATING, COOLING AND OFF
 if(Tc<=(Td-Th/2)){
       comand = 1;
       digitalWrite(relay1, HIGH);
       digitalWrite(relay2, LOW);
       lcd.print("ESQUENTANDO"); 
                 }
 if(Tc>=(Td+Th/2)){
       comand = -1;
       digitalWrite(relay2, HIGH);
       digitalWrite(relay1, LOW);
                 }
 if(Tc>Td && oldTc<=Td) { //hysteresis
       comand = 0; 
              digitalWrite(relay2, LOW);
       digitalWrite(relay1, LOW); 
       lcd.print("DI BOA");
                       }
 if(Tc=Td) { //hysteresis
       comand = 0;
             digitalWrite(relay2, LOW);
       digitalWrite(relay1, LOW); 
       lcd.print("DI BOA"); 
                       }
 oldTc = Tc;
 /*
//commands for heating, cooling and off
 lcd.setCursor(9, 0);  
 if(comand==-1) { //cooling
       digitalWrite(4, HIGH);
       digitalWrite(5, LOW);
       lcd.print("GELANDO");   
               }
 if(comand==1){ //heating
       digitalWrite(5, HIGH);
       digitalWrite(4, LOW);
       lcd.print("ESQUENTANDO");
             }
 if(comand==0) { //off
       digitalWrite(4, HIGH);
       digitalWrite(5, HIGH); 
       lcd.print("DI BOA");
              } 
              */
}








