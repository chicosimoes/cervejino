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

// setup u8g object, please remove comment from one of the following constructor calls
// IMPORTANT NOTE: The complete list of supported devices is here: http://code.google.com/p/u8glib/wiki/device

//U8GLIB_NHD27OLED_BW u8g(13, 11, 10, 9);	// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_NHD27OLED_2X_BW u8g(13, 11, 10, 9);	// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_NHD27OLED_GR u8g(13, 11, 10, 9);	// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_NHD27OLED_2X_GR u8g(13, 11, 10, 9);	// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_NHD31OLED_BW u8g(13, 11, 10, 9);	// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_NHD31OLED_2X_BW u8g(13, 11, 10, 9);	// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_NHD31OLED_GR u8g(13, 11, 10, 9);	// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_NHD31OLED_2X_GR u8g(13, 11, 10, 9);	// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGS102 u8g(13, 11, 10, 9);		// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGM132 u8g(13, 11, 10, 9);		// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGM128 u8g(13, 11, 10, 9);		// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_ST7920_128X64_1X u8g(8, 9, 10, 11, 4, 5, 6, 7, 18, 17, 16);   // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, di=17,rw=16
//U8GLIB_ST7920_128X64_4X u8g(8, 9, 10, 11, 4, 5, 6, 7, 18, 17, 16);   // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, di=17,rw=16
//U8GLIB_ST7920_128X64_1X u8g(18, 16, 17);	// SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17
//U8GLIB_ST7920_128X64_4X u8g(18, 16, 17);	// SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17
//U8GLIB_ST7920_192X32_1X u8g(8, 9, 10, 11, 4, 5, 6, 7, 18, 17, 16);   // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, di=17,rw=16
//U8GLIB_ST7920_192X32_4X u8g(8, 9, 10, 11, 4, 5, 6, 7, 18, 17, 16);   // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, di=17,rw=16
//U8GLIB_ST7920_192X32_1X u8g(18, 16, 17);	// SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17
//U8GLIB_ST7920_192X32_4X u8g(18, 16, 17);	// SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17
//U8GLIB_ST7920_192X32_1X u8g(13, 11, 10);	// SPI Com: SCK = en = 13, MOSI = rw = 11, CS = di = 10
//U8GLIB_ST7920_192X32_4X u8g(10);		// SPI Com: SCK = en = 13, MOSI = rw = 11, CS = di = 10, HW SPI
//U8GLIB_ST7920_202X32_1X u8g(8, 9, 10, 11, 4, 5, 6, 7, 18, 17, 16);   // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, di=17,rw=16
//U8GLIB_ST7920_202X32_4X u8g(8, 9, 10, 11, 4, 5, 6, 7, 18, 17, 16);   // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, di=17,rw=16
//U8GLIB_ST7920_202X32_1X u8g(18, 16, 17);	// SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17
//U8GLIB_ST7920_202X32_4X u8g(18, 16, 17);	// SPI Com: SCK = en = 18, MOSI = rw = 16, CS = di = 17
//U8GLIB_LM6059 u8g(13, 11, 10, 9);		// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_LM6063 u8g(13, 11, 10, 9);		// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGXL160_BW u8g(10, 9);		// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGXL160_GR u8g(13, 11, 10, 9);	// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGXL160_2X_BW u8g(13, 11, 10, 9);	// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_DOGXL160_2X_GR u8g(13, 11, 10, 9);	// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_PCD8544 u8g(13, 11, 10, 9, 8);		// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, Reset = 8
//U8GLIB_PCF8812 u8g(13, 11, 10, 9, 8);		// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, Reset = 8
//U8GLIB_KS0108_128 u8g(8, 9, 10, 11, 4, 5, 6, 7, 18, 14, 15, 17, 16); 		// 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, cs1=14, cs2=15,di=17,rw=16
//U8GLIB_LC7981_160X80 u8g(8, 9, 10, 11, 4, 5, 6, 7,  18, 14, 15, 17, 16); 	// 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, cs=14 ,di=15,rw=17, reset = 16
//U8GLIB_LC7981_240X64 u8g(8, 9, 10, 11, 4, 5, 6, 7,  18, 14, 15, 17, 16); 	// 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, cs=14 ,di=15,rw=17, reset = 16
//U8GLIB_LC7981_240X128 u8g(8, 9, 10, 11, 4, 5, 6, 7,  18, 14, 15, 17, 16); 	// 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, cs=14 ,di=15,rw=17, reset = 16
//U8GLIB_ILI9325D_320x240 u8g(18,17,19,U8G_PIN_NONE,16 );  			// 8Bit Com: D0..D7: 0,1,2,3,4,5,6,7 en=wr=18, cs=17, rs=19, rd=U8G_PIN_NONE, reset = 16
//U8GLIB_SBN1661_122X32 u8g(8,9,10,11,4,5,6,7,14,15, 17, U8G_PIN_NONE, 16); 	// 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 cs1=14, cs2=15,di=17,rw=16,reset = 16
//U8GLIB_SSD1306_128X64 u8g(13, 11, 10, 9);	// SW SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_SSD1306_128X64 u8g(10, 9);		// HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);	// HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)
//U8GLIB_SSD1306_128X32 u8g(13, 11, 10, 9);	// SW SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_SSD1306_128X32 u8g(10, 9);             // HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)
//U8GLIB_SSD1306_128X32 u8g(U8G_I2C_OPT_NONE);	// HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)
//U8GLIB_SSD1309_128X64 u8g(13, 11, 10, 9);	// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9
//U8GLIB_SSD1327_96X96_GR u8g(U8G_I2C_OPT_NONE);	// I2C
//U8GLIB_SSD1327_96X96_2X_GR u8g(U8G_I2C_OPT_NONE);	// I2C
//U8GLIB_NHD_C12864 u8g(13, 11, 10, 9, 8);	// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, RST = 8
//U8GLIB_NHD_C12832 u8g(13, 11, 10, 9, 8);	// SPI Com: SCK = 13, MOSI = 11, CS = 10, A0 = 9, RST = 8
//U8GLIB_T6963_240X128 u8g(8, 9, 10, 11, 4, 5, 6, 7, 14, 15, 17, 18, 16); // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7, cs=14, a0=15, wr=17, rd=18, reset=16
//U8GLIB_T6963_240X64 u8g(8, 9, 10, 11, 4, 5, 6, 7, 14, 15, 17, 18, 16); // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7, cs=14, a0=15, wr=17, rd=18, reset=16
//U8GLIB_T6963_128X64 u8g(8, 9, 10, 11, 4, 5, 6, 7, 14, 15, 17, 18, 16); // 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7, cs=14, a0=15, wr=17, rd=18, reset=16
U8GLIB_ST7920_128X64_1X u8g( 6, 5, 4, U8G_PIN_NONE);


//=================================================
// Variables for Menu Process

// DOGS102 shield 
// uint8_t uiKeyUpPin = 5;
// uint8_t uiKeyDownPin = 3;
// uint8_t uiKeySelectPin = 4;
// uint8_t uiKeyExitPin = 0;

// DOGM132, DOGM128 and DOGXL160 shield
//uint8_t uiKeyUpPin = 10;         
//uint8_t uiKeyDownPin = 8;
//uint8_t uiKeySelectPin = 9;
//uint8_t uiKeyExitPin = 0;


//=================================================
// Forward declaration of the toplevel element
M2_EXTERN_ALIGN(top_el_x2l_menu);


//=================================================
// Define three user menus, just for demonstration. 
M2_ROOT(el_mnu0_sel, "t1", "Menu 0 selected", &top_el_x2l_menu);
M2_ALIGN(top_el_mnu0_sel, "-1|1W64H64", &el_mnu0_sel);

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

uint8_t valueFer = EEPROM.read(2);

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

