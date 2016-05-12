/*

  RotEnc.pde
  
  U8glib Example (Incremental Rotary Encoder)

  m2tklib = Mini Interative Interface Toolkit Library

  Copyright (C) 2012  olikraus@gmail.com

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "U8glib.h"
#include "M2tk.h"
#include "utility/m2ghu8g.h"
#include "DHT.h"

#define DHTPIN 2     // what digital pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

U8GLIB_ST7920_128X64_1X u8g(6, 5, 4);	// SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17



//=================================================
// Forward declaration of the toplevel element
M2_EXTERN_ALIGN(top_menu);

//=================================================
// Simple dialog: Input two values n1 and n2

uint8_t n1 = 0;
uint8_t n2 =  0;

M2_LABEL(el_l1, NULL,"Tm:" );
M2_U8NUM(el_u1, "c2", -5, 40, &n1);
M2_LABEL(el_l2, NULL, "Hm:");
M2_U8NUM(el_u2, "c2", 0, 99, &n2);

M2_LIST(list) = { &el_l1, &el_u1, &el_l2, &el_u2 };
M2_GRIDLIST(el_gridlist, "c2", list);
M2_ALIGN(top_menu, "-1|1W32H32", &el_gridlist);

// m2 object and constructor
// Note: Use the "m2_eh_4bd" handler, which fits better to the "m2_es_arduino_rotary_encoder" 
M2tk m2(&top_menu, m2_es_arduino_rotary_encoder, m2_eh_4bd, m2_gh_u8g_bf);

//=================================================
// Arduino Setup & Loop

void setup(void) {
  /////////////////////////DHT11///////////////////////
 Serial.begin(9600);
  Serial.println("DHTxx test!");

  dht.begin();
  ////////////////////////////////////////////////////
  
  // Connect u8glib with m2tklib
  m2_SetU8g(u8g.getU8g(), m2_u8g_box_icon);

  // Assign u8g font to index 0
  m2.setFont(0, u8g_font_7x13r);

  // define button for the select message
  m2.setPin(M2_KEY_SELECT, 9);          // dogm128 shield, 2nd from top
  
  // The incremental rotary encoder is conected to these two pins
  m2.setPin(M2_KEY_ROT_ENC_A, 10);
  m2.setPin(M2_KEY_ROT_ENC_B, 8);
}

void loop() {
   m2.checkKey(); 
  ////////////////////////////DHT11////////////////////
  // Wait a few seconds between measurements.
  //delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");
  ///////////////////////////////////////////
  // check rotary encoder also inside the picture loop
  
  // process events and redraw menu if required
  if ( m2.handleKey() != 0 ) {
    u8g.firstPage();  
    do {
      // check rotary encoder also inside the picture loop
      m2.checkKey();
      // draw menu
      m2.draw();
    } while( u8g.nextPage() );
  }
}

