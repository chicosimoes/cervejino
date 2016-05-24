/*

  MenuX2L.pde
  
  U8glib Example

  m2tklib = Mini Interative Interface Toolkit Library

  Copyright (C) 2013  olikraus@gmail.com

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

#include <stdlib.h>		// for itoa
#include "U8glib.h"
#include "M2tk.h"
#include "utility/m2ghu8g.h"
#include <EEPROM.h>
#include <OneWire.h>
#include <DallasTemperature.h>
uint8_t valueMat = EEPROM.read(1);
int RelePinoMat = 10;  // define quel o pino do rele de maturacao
uint8_t valueFer = EEPROM.read(2);
int RelePinoFer =7;  // define quel o pino do rele de fermentacao

#define ONE_WIRE_BUS_1 11
#define ONE_WIRE_BUS_2 12

OneWire oneWire_MAT(ONE_WIRE_BUS_1);
OneWire oneWire_FER(ONE_WIRE_BUS_2);

DallasTemperature sensor_MAT(&oneWire_MAT);
DallasTemperature sensor_FER(&oneWire_FER);

//#define NUMLEITURAS 3


//float leiturasMat[NUMLEITURAS];
//int indexMat = 0;
//int totalMat = 0;
uint8_t mediaMat = 0;  




//float leiturasFer[NUMLEITURAS];
//int indexFer = 0;
//int totalFer = 0;
uint8_t mediaFer = 0;
 
U8GLIB_ST7920_128X64_1X u8g( 6, 5, 4, U8G_PIN_NONE);


//=================================================
// Variables for Menu Process

// DOGS102 shield 
// uint8_t uiKeyUpPin = 5;
// uint8_t uiKeyDownPin = 3;
// uint8_t uiKeySelectPin = 4;
// uint8_t uiKeyExitPin = 0;
/*
// DOGM132, DOGM128 and DOGXL160 shield
uint8_t uiKeyUpPin = 10;         
uint8_t uiKeyDownPin = 8;
uint8_t uiKeySelectPin = 9;

uint8_t uiKeyExitPin = 0;

*/
//=================================================
// Forward declaration of the toplevel element
M2_EXTERN_ALIGN(top_el_x2l_menu);


//=================================================
// Define three user menus, just for demonstration.
//void displayMediaMat(m2_el_fnarg_p fnarg) {
//mediaMat = sensor_MAT.getTempCByIndex(0);
//const char *label_cb(m2_rom_void_p element)

//}

//M2_LABELFN(el_labefn, NULL, label_cb);
//M2_U32NUM(el_u8_cb, "c6r1", &mesiPiesi);
M2_LABEL(el_TempMat,"c6r1", "mediaMat" );
M2_LABEL(el_Mat, NULL , "valueMat" );
M2_LABEL(el_TempFer, NULL, "mediaFer" );
M2_LABEL(el_Fer, NULL , "valueFer" );
M2_LIST(list) = { 
    &el_TempMat, &el_TempFer, 
    &el_Mat, &el_Fer};
M2_GRIDLIST(list_element, "c2",list);
//M2_VLIST(el_menu_vlist, NULL, list_menu);
M2_ROOT(el_mnu0_sel, NULL, NULL, &top_el_x2l_menu);
M2_ALIGN(top_el_mnu0_sel, "W64H64", &list_element);

M2_ROOT(el_mnu1_sel, "t1", "Menu 1 selected", &top_el_x2l_menu);
M2_ALIGN(top_el_mnu1_sel, "-1|1W64H64", &el_mnu1_sel);

M2_ROOT(el_mnu2_sel, "t1", "Menu 2 selected", &top_el_x2l_menu);
M2_ALIGN(top_el_mnu2_sel, "-1|1W64H64", &el_mnu2_sel);

M2_ROOT(el_mnu3_sel, "t1", "Menu 3 selected", &top_el_x2l_menu);
M2_ALIGN(top_el_mnu3_sel, "-1|1W64H64", &el_mnu3_sel);


//=================================================
uint8_t value = EEPROM.read(1);
char buf[20];

// define callback procedure, which returns a menu entry with a value
const char *xmenu_Maturacao(uint8_t idx, uint8_t msg)
{  
  if ( msg == M2_STRLIST_MSG_GET_STR ) {
    strcpy(buf, " Value: ");
    itoa((int)value, buf+strlen(buf), 10);
    return buf;
  }
  return "";
}

// define callback procedures which increment and decrement a value
const char *xmenuMat_inc(uint8_t idx, uint8_t msg) {
  if ( msg == M2_STRLIST_MSG_SELECT  ) {
      value++;
      EEPROM.write(1, value);
  }
  return "";
}

const char *xmenuMat_dec(uint8_t idx, uint8_t msg) {
  if ( msg == M2_STRLIST_MSG_SELECT  ) {
      value--;
      EEPROM.write(1, value);
  }
  return "";
}

//uint8_t valueFer = EEPROM.read(2);

// define callback procedure, which returns a menu entry with a value
const char *xmenu_Fermentacao(uint8_t idx, uint8_t msg)
{  
  if ( msg == M2_STRLIST_MSG_GET_STR ) {
    strcpy(buf, " Value: ");
    itoa((int)valueFer, buf+strlen(buf), 10);
    return buf;
  }
  return "";
}

// define callback procedures which increment and decrement a value
const char *xmenuFer_inc(uint8_t idx, uint8_t msg) {
  if ( msg == M2_STRLIST_MSG_SELECT  ) {
      valueFer++;
      EEPROM.write(2, valueFer);
  }
  return "";
}

const char *xmenuFer_dec(uint8_t idx, uint8_t msg) {
  if ( msg == M2_STRLIST_MSG_SELECT  ) {
      valueFer--;
      EEPROM.write(2, valueFer);
  }
  return "";
}

//=================================================
// this is the overall menu structure for the X2L Menu

m2_xmenu_entry xmenu_data[] = 
{
  { "Menu 0", NULL, NULL },
  { ". cervejino", &top_el_mnu0_sel, NULL },
  { "Menu Mat", NULL, NULL },		/* expandable main menu entry */
  { ".", NULL, xmenu_Maturacao },		/* The label of this menu line is returned by the callback procedure */
  { ". Inc", NULL, xmenuMat_inc },		/* This callback increments the value */
  { ". Dec", NULL, xmenuMat_dec },		/* This callback decrements the value */
  { "Menu Fer", NULL, NULL },
  { ".", NULL, xmenu_Fermentacao },		/* The label of this menu line is returned by the callback procedure */
  { ". Inc", NULL, xmenuFer_inc },		/* This callback increments the value */
  { ". Dec", NULL, xmenuFer_dec },		/* This callback decrements the value */

 // { ". Sub 2-1", &top_el_mnu1_sel, NULL },
  //{ ". Sub 2-2", &top_el_mnu2_sel, NULL},
 // { ". Sub 2-3", &top_el_mnu3_sel, NULL },
  { NULL, NULL, NULL },
};

//=================================================
// This is the main menu dialog box

// The first visible line and the total number of visible lines.
// Both values are written by M2_X2LMENU and read by M2_VSB
uint8_t el_x2l_first = 0;
uint8_t el_x2l_cnt = 3;

// M2_X2LMENU definition
// Option l4 = four visible lines
// Option e15 = first column has a width of 15 pixel
// Option W43 = second column has a width of 43/64 of the display width
// Option F3 = select font 3 for the extra column (icons)

M2_X2LMENU(el_x2l_strlist, "l4e15W43F3", &el_x2l_first, &el_x2l_cnt, xmenu_data, 65,102,'\0');
M2_SPACE(el_x2l_space, "W1h1");
M2_VSB(el_x2l_vsb, "l4W2r1", &el_x2l_first, &el_x2l_cnt);
M2_LIST(list_x2l) = { &el_x2l_strlist, &el_x2l_space, &el_x2l_vsb };
M2_HLIST(el_x2l_hlist, NULL, list_x2l);
M2_ALIGN(top_el_x2l_menu, "-1|1W64H64", &el_x2l_hlist);


//=================================================
// m2 object and constructor
M2tk m2(&top_el_x2l_menu, m2_es_arduino_rotary_encoder, m2_eh_4bs, m2_gh_u8g_ffs);

//=================================================
// Draw procedure, Arduino Setup & Loop


void draw(void) {
  m2.draw();
}

void setup(void) {
  // Connect u8glib with m2tklib
  m2_SetU8g(u8g.getU8g(), m2_u8g_box_icon);

  // Assign u8g font to index 0
  m2.setFont(0, u8g_font_6x13r);

  // Assign icon font to index 3
  m2.setFont(3, u8g_font_m2icon_7);

  // Setup keys
 // m2.setPin(M2_KEY_SELECT, uiKeySelectPin);
 // m2.setPin(M2_KEY_PREV, uiKeyUpPin);
 // m2.setPin(M2_KEY_NEXT, uiKeyDownPin);
 // m2.setPin(M2_KEY_EXIT, uiKeyExitPin);    
  
  
  m2.setPin(M2_KEY_SELECT, 9);
  m2.setPin(M2_KEY_ROT_ENC_A, 10);
  m2.setPin(M2_KEY_ROT_ENC_B, 8);
 
 EEPROM.read (1);
 EEPROM.read (2);

  sensor_MAT.begin();
  sensor_FER.begin();
/*  
    for (int i = 0; i < NUMLEITURAS; i++)
   leiturasMat[i] = 0;
    for (int i = 0; i < NUMLEITURAS; i++)
      leiturasFer[i] = 0;
 */
  
}

void loop() {
  // menu management
  
  
  m2.checkKey();
  if ( m2.handleKey() != 0 ) {
    u8g.firstPage();  
    do {
      m2.checkKey();
      draw();
    } while( u8g.nextPage() );
  }
}

