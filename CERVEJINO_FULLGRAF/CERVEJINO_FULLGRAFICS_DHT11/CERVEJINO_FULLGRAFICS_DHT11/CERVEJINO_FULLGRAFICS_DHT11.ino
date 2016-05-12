/*

  Graphics.pde

  How to mix Graphics with M2tklib
  
  U8glib Example

  m2tklib = Mini Interative Interface Toolkit Library

  >>> Before compiling: Please remove comment from the constructor of the 
  >>> connected graphics display (see below).

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
  
  Two methods:
    - mix/combine menu and graphics
    - switch between menu and graphcs
    
   Graphic procedure contain two parts:
    - draw
    - update, which also returns if a redraw is required
    
    draw_graphics():
      depending on m2tk state, draw graphics
    
    update_graphics():
      depending on m2tk state, update graphics for animation and force redraw
      
   
*/

#include "U8glib.h"
#include "M2tk.h"
#include "utility/m2ghu8g.h"
#include "DHT.h"

#define DHTPIN 2     // what digital pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);
//

int32_t last_action_time = 0;
uint8_t is_menu_active = 1;

  
  // Read temperature as Celsius (the default)

  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

// setup u8g object, please remove comment from one of the following constructor calls
// IMPORTANT NOTE: The complete list of supported devices is here: http://code.google.com/p/u8glib/wiki/device
U8GLIB_ST7920_128X64_1X u8g( 6, 5, 4, U8G_PIN_NONE);

// DOGS102 shield 
// uint8_t uiKeyUpPin = 5;
// uint8_t uiKeyDownPin = 3;
// uint8_t uiKeySelectPin = 4;

// DOGM132, DOGM128 and DOGXL160 shield
uint8_t uiKeyUpPin = 10;         
uint8_t uiKeyDownPin = 8;
uint8_t uiKeySelectPin = 9;

//================================================================
// m2tklib forward declarations

extern M2tk m2;
M2_EXTERN_ALIGN(el_top);


/*
//================================================================
// low level graphics

uint8_t is_bigger = 0;
uint32_t next_state_change = 0;
uint8_t size = 1;

// draw a rectange at x/y 
void draw_rectangle(uint8_t x, uint8_t y) {
  u8g.setDefaultForegroundColor();
  u8g.drawBox(x,y,size,size);
}

*/
uint8_t draw_matura(void)  //Maturacao-  
{
  u8g.setFont(u8g_font_unifont);  
  u8g.drawStr( 74, 15, "MATURA");  
  u8g.drawStr( 74, 15, "MATURA");  
  u8g.drawFrame(64,0,64,64);  
  u8g.drawFrame(66,2,60,60);
  float t = dht.readTemperature();
  char temp[5];
  u8g.drawStr( 75, 35,dtostrf(t,3,1,temp));
  u8g.drawStr( 90, 55, "\260C"); 
} 

uint8_t draw_fermenta(void)  //Maturacao-  
{
  u8g.setFont(u8g_font_unifont);  
  u8g.drawStr( 74, 15, "FERMENTA");  
  u8g.drawStr( 74, 15, "FERMENTA");  
  u8g.drawFrame(64,0,64,64);  
  u8g.drawFrame(66,2,60,60);
    float h = dht.readHumidity();
  char temp[5];
  u8g.drawStr( 75, 35,dtostrf(h,3,1,temp));
 // u8g.drawStr( 75, 35, dtostrf(dht.readHumidity(), 5, 2, Str));
  u8g.drawStr( 90, 55, "\260C"); 
} 

/*
// state machine for the animation of the rectangle
// will return none-zero if an update is required
uint8_t update_rectangle(void) {
  if ( next_state_change < millis() ) {
    next_state_change = millis();
    next_state_change += 300;
    if ( is_bigger == 0 ) {
      size -= 1;
      if ( size <= 1 )
        is_bigger = 1;
    }
    else {
      size += 1;
      if ( size >= 10 )
        is_bigger = 0;
    }
    return 1;
  }
  return 0;
}

//================================================================
// m2tk related code

uint8_t y = 0;                   // position of the low level graphics

void fn_inc(m2_el_fnarg_p fnarg) {
  if ( y < 63-10 )
    y += 1;
}

void fn_dec(m2_el_fnarg_p fnarg) {
  if ( y > 0 )
    y -= 1;
}
*/




M2_BUTTON(el_maturacao, "x30y41", "Maturac", NULL);
M2_BUTTON(el_fermentacao, "x30y21", "fermentac", NULL);
M2_ROOT(el_leave_combine, "x30y1", "Back", &el_top);
M2_LIST(el_btn_list) = { &el_maturacao, &el_fermentacao, &el_leave_combine};
M2_XYLIST(el_comute, NULL, el_btn_list);



M2_LABEL(el_goto_title, NULL, "Compostino");
M2_ROOT(el_goto_maturacao, NULL, "Maturacao", &el_maturacao);
M2_ROOT(el_goto_fermentacao, NULL, "Fermentacao", &el_fermentacao);             // selecting this, will remove all menues
M2_LIST(list_menu) = {&el_goto_title, &el_goto_maturacao, &el_goto_fermentacao};
M2_VLIST(el_menu_vlist, NULL, list_menu);
M2_ALIGN(el_top, "W0H64", &el_menu_vlist);

M2tk m2(&el_top, m2_es_arduino, m2_eh_4bs, m2_gh_u8g_ffs);

//================================================================
// high level draw and update procedures

void draw_graphics(void) {
  // show the graphics depending on the current toplevel element
  
  if ( m2.getRoot() == &el_maturacao ) {
      // combine is active, do add the rectangle
      // menu is on the right, put the rectangle to the left
      dht.readTemperature();
      draw_matura();
  }

  if ( m2.getRoot() == &el_fermentacao ) {
      // all menus are gone, show the rectangle
      dht.readHumidity();
      draw_fermenta();
  }
}

// update graphics, will return none-zero if an update is required
uint8_t update_graphics(void) {  
  if ( m2.getRoot() == &el_maturacao ) {
      // combine is active, update the rectangle for animation
      dht.readTemperature();
      return draw_matura();
  }
  
    if ( m2.getRoot() == &el_fermentacao ) {
      // combine is active, update the rectangle for animation
      dht.readHumidity();
      return draw_fermenta();
  }

  if ( m2.getRoot() == &m2_null_element ) {
      // check for any keys and assign a suitable menu again
      if ( m2.getKey() != M2_KEY_NONE )
        m2.setRoot(&el_top);
    
      // all menus are gone, rectangle is shown, so do update
      return 0;
  }
  
  // no update for the graphics required
  return 0;
}


//================================================================
// overall draw procedure for u8glib

void draw(void) {
  last_action_time = millis();
  draw_graphics();
  m2.draw();
}

//================================================================
// Arduino setup and loop

void setup(void) {
  // Connect u8glib with m2tklib
  m2_SetU8g(u8g.getU8g(), m2_u8g_box_icon);

  // Assign u8g font to index 0
  m2.setFont(0, u8g_font_7x13);

  // Setup keys
  m2.setPin(M2_KEY_SELECT, uiKeySelectPin);
  m2.setPin(M2_KEY_NEXT, uiKeyDownPin);
}

void loop() {
  
  
  m2.checkKey();
  //if(is_menu_active != 0)
  if ( m2.handleKey() != 0 || update_graphics() != 0 ) {
     
    u8g.firstPage();  
    do {
      //last_action_time = 0;
      draw();
       } while( u8g.nextPage() );}
    /*   
    else if (last_action_time >= millis() + 200){
      u8g.firstPage();  
    do {
      
      draw();
       } while( u8g.nextPage() );}
   
*/
          

}



