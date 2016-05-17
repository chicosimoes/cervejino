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

#define NUMLEITURAS 3

float leiturasMat[NUMLEITURAS];
int indexMat = 0;
int totalMat = 0;
int mediaMat = 0;  
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

 M2_ROOT(el_show, NULL, "cervejino", &el_show);

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
    float h = dht.readHumidity();
 // char temp[5];
  u8g.drawStr( 5, 35,dtostrf(h,3,1,temp));
 // u8g.drawStr( 75, 35, dtostrf(dht.readHumidity(), 5, 2, Str));
  u8g.drawStr( 45, 35, "\260C");
// m2.setRoot(&el_show); 
 
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



/*
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
*/
//M2_ALIGN(el_top, "W0H64", NULL);

//M2tk m2(&el_top, m2_es_arduino, m2_eh_4bs, m2_gh_u8g_ffs);


uint8_t n1 = 0;
uint8_t n2 =  0;

M2_LABEL(el_l1, NULL,"Tm:" );
M2_U8NUM(el_u1, "c2", -5, 40, &n1);
M2_LABEL(el_l2, NULL, "Hm:");
M2_U8NUM(el_u2, "c2", 0, 99, &n2);
M2_BUTTON(el_back,"f4", "ok",fn_ok);

M2_LIST(list) = { &el_l1, &el_u1, &el_l2, &el_u2, &el_back };
M2_GRIDLIST(el_gridlist, "c2", list);
M2_ALIGN(el_top, "-1|1W32H32", &el_gridlist);

// m2 object and constructor
// Note: Use the "m2_eh_4bd" handler, which fits better to the "m2_es_arduino_rotary_encoder" 
M2tk m2(&el_top, m2_es_arduino_rotary_encoder, m2_eh_4bd, m2_gh_u8g_bf);

void fn_ok(m2_el_fnarg_p fnarg) {
  /* do something with the number */

  m2.setRoot(&el_show);

}




//================================================================
// high level draw and update procedures

void draw_graphics(void) {
  // show the graphics depending on the current toplevel element
  
// if ( m2.getRoot() == &el_show ) {
      // combine is active, do add the rectangle
      // menu is on the right, put the rectangle to the left
      ///////
  totalMat -= leiturasMat[indexMat];               // subtrair a última leitura
  leiturasMat[indexMat] = dht.readTemperature() ; // ler do sensor
  totalMat += leiturasMat[indexMat];               // adicionar leitura ao total
  indexMat = (indexMat + 1);                    // avançar ao próximo índice

  if (indexMat >= NUMLEITURAS){               // se estiver no fim do vetor...
    indexMat = 0;       // ...meia-volta ao início
  }
  mediaMat = totalMat / NUMLEITURAS;          // calcular a média
      //////
      u8g.firstPage();  
    do {
      //last_action_time = 0;
    //  m2_SetRoot(&el_top);
      draw_lcd();
       } while( u8g.nextPage() );
  
  //  }
 //   else
 //   m2.draw();

//  else if ( m2.getRoot() == &el_top ) {
      // all menus are gone, show the rectangle
      
  //    m2.draw();
//  }
  
}
/*
// update graphics, will return none-zero if an update is required
uint8_t update_graphics(void) {  
  if ( m2.getRoot() == &el_show ) {
      // combine is active, update the rectangle for animation
      ///////
        totalMat -= leiturasMat[indexMat];               // subtrair a última leitura
  leiturasMat[indexMat] = dht.readTemperature(); // ler do sensor
  totalMat += leiturasMat[indexMat];               // adicionar leitura ao total
  indexMat = (indexMat + 1);                    // avançar ao próximo índice

  if (indexMat >= NUMLEITURAS){               // se estiver no fim do vetor...
    indexMat = 0;       // ...meia-volta ao início
  }
  mediaMat = totalMat / NUMLEITURAS;          // calcular a média
      ///////
      draw_lcd();
  }
  
    if ( m2.getRoot() == &el_top ) {
      // combine is active, update the rectangle for animation
      m2.draw();
      
  }

  if ( m2.getRoot() == &m2_null_element ) {
      // check for any keys and assign a suitable menu again
      if ( m2.getKey() != M2_KEY_NONE )
       // m2.setRoot(&el_top);
    
      // all menus are gone, rectangle is shown, so do update
      return 0;
  }
     
  
  
  // no update for the graphics required
  return 0;
}
*/

//================================================================
// overall draw procedure for u8glib

void draw(void) {
m2.getRoot();
// uint8_t m2_GetKey();
  //if(is_menu_active != 0)
  if ( m2.getRoot() == &el_top) {
     
    u8g.firstPage();  
    do {
      //last_action_time = 0;
    //  m2_SetRoot(&el_top);
      m2.draw();
       } while( u8g.nextPage() );}
       
       
  if ( m2.getRoot() == &el_show) {
   u8g.firstPage();  
    do {
       u8g.setFont(u8g_font_unifont);  
  draw_lcd();
       } while( u8g.nextPage() );}
  }
 // 
 
 //m2.draw();


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
  
  for (int i = 0; i < NUMLEITURAS; i++)
    leiturasMat[i] = 0;
}

void loop() {
  
 //m2.setRoot(&el_top);
  m2.checkKey();
  //if(is_menu_active != 0)
  if ( m2.handleKey() != 0 )// && m2.getRoot() == &el_top ) 
  {
     
    u8g.firstPage();  
    do {
      //last_action_time = 0;
      draw();
       } while( u8g.nextPage() );
     }
       
   // else 
   //   u8g.firstPage();  
   // do {
      
   //   m2.draw();
   //    } while( u8g.nextPage() );
   

          

}



