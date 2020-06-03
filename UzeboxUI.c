/*
 *  UzeboxUI
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Uzebox is a reserved trade mark
*/

#include <stdbool.h>
#include <avr/io.h>
#include <stdlib.h>
#include <math.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <uzebox.h>
#include <spiram.h>
#include <bootlib.h>

#include "embedvm/vmsrc/embedvm.h"

#include "data/tileset.inc"
#include "data/spriteset.inc"
//#include "data/fonts/font-8x8-full.inc"
//#include "data/fonts/font-8x8-full-invert.inc"
#include "data/fonts/envious-blackbg.inc"
#include "data/fonts/envious-whitebg.inc"

int btnHeld = 0;		// buttons held right now
int btnPressed = 0;		// buttons pressed this frame
int btnReleased = 0;	// buttons released this frame
int btnPrev = 0;		// buttons previously pressed

int wallpaperTile = 1;	// default wallpaper tile is 1

uint32_t frame = 0;		// frame counter
uint32_t uptime = 0;	// uptime counter in seconds

int appToLoad = 0;
bool loadApp = false;

struct EepromBlockStruct ebs;

void updateCursor();
void updateControllers();
void setFontColor(int font);
void updateMenubar();
void updateClick();
void handleMenuClick();
void updateActiveWindow();
void updateInactiveTitlebars();
void redrawAll();
void drawWallpaper();
void printWindow(int x, int y, int windowNumber, char *text);
void printWindowLen(int x, int y, int windowNumber, char *text, int textSize);
void printWindowInt(int x, int y, int windowNumber, unsigned int val);
void setWindowTile(int x, int y, int windowNumber, unsigned int tile);
void createButton(int locationX, int locationY, int sizeX, int sizeY, int windowNumber, int buttonNumber, char *text, void (*callbackFunc), int callbackArg1);
void updateButtonClicks();
void createWindow(int locationX, int locationY, int sizeX, int sizeY, char title[], int titleSize, bool isVM);
void destroyWindow(int windowNumber);
void clearWindow(int windowNumber, int tile);
void setActiveWindow(int windowNumber);
int getActiveWindow();
void initialize();
void vsyncCallback(void);
void createVM();
void createAboutWindow();
void createTilesWindow();
void settingsChangeWallpaper(int num);
void settingsSaveWallpaper();
void createSettingsWindow();

// Menu //////////////////////////////////////////////////////////////////

const char uzeMenu[][44] PROGMEM = { // 30 characters plus 1 for each null terminator. is the extra for null really needed? idk its 4 AM rn
	"About     ",
	"Apps     >",
	"Settings  ",
	"Tiles     ",
	"Reset     "
};

const char fileMenu[][22] PROGMEM = {
	"New File  ",
	"Open File "
};

const char editMenu[][22] PROGMEM = {
	"Copy      ",
	"Paste     "
};

const char windowMenu[][154] PROGMEM = {
	"             1",
	"             2",
	"             3",
	"             4",
	"             5",
	"             6",
	"             7",
	"             8",
	"             9",
	"            10"
};

const char appsMenu[][110] PROGMEM = {
	"          ",
	"          ",
	"          ",
	"          ",
	"          ",
	"          ",
	"          ",
	"          ",
	"          ",
	"          "
};

//////////////////////////////////////////////////////////////////////////

// EmbedVM ///////////////////////////////////////////////////////////////

struct embedvm_s vm[10] = { };

int16_t mem_read(uint16_t addr, bool is16bit, void *ctx) {
	if (addr + (is16bit ? 1 : 0) >= 32768) {
		return 0;
	}
	if (is16bit) {
		return (SpiRamReadU8(0,addr) << 8) | SpiRamReadU8(0,addr+1);
	}
	return SpiRamReadU8(0,addr);
}

void mem_write(uint16_t addr, int16_t value, bool is16bit, void *ctx) {
	if (addr + (is16bit ? 1 : 0) >= 32768) {
		return;
	}
	if (is16bit) {
		SpiRamWriteU8(0,addr,value >> 8);
		SpiRamWriteU8(0,addr+1,value);
	} else {
		SpiRamWriteU8(0,addr,value);
	}
}

char tempChar[64];
int tempInt = 0;

int16_t call_user(uint8_t funcid, uint8_t argc, int16_t *argv, void *ctx) {
	if (funcid == 0) { // build up a string one character at a time. yes this is weird, but it's the only way that would work
		if (argc == 0) {
			tempChar[0] = '\0';
			tempInt = 0;
		} else {
			tempChar[tempInt] = argv[0];
			tempInt++;
		}
		return 0;
	}
	if (funcid == 1) { // create a window
		int locationX = argv[0];
		int locationY = argv[1];
		int sizeX = argv[2];
		int sizeY = argv[3];
		int titleSize = argv[4];

		createWindow(locationX,locationY,sizeX,sizeY,tempChar,titleSize,true);
		return 0;
	}
	if (funcid == 2) { // print in a window
		int locationX = argv[0];
		int locationY = argv[1];
		int windowNumber = argv[2];
		int textSize = argv[3];
		int color = argv[4];

		setFontColor(color);
		printWindowLen(locationX,locationY,windowNumber,tempChar,textSize);
		return 0;
	}
	if (funcid == 3) { // return current active window number
		return getActiveWindow();
	}
	if (funcid == 4) { // clear window
		int windowNumber = argv[0];
		int tile = argv[1];
		clearWindow(windowNumber, tile);
		return 0;
	}
	if (funcid == 5) { // set a tile in a window
		int locationX = argv[0];
		int locationY = argv[1];
		int windowNumber = argv[2];
		int tile = argv[3];

		setWindowTile(locationX,locationY,windowNumber,tile);
		return 0;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////

/*const char cursor_map[] PROGMEM = {
	2,2, // width/height of map
	1,2, // tile indexes
	3,4
};*/

const char cursor_map[] PROGMEM = {
	1,2, // width/height of map
	1 // tile indexes
};

int activeWindow = 0; // keeps track of the window number that's currently active and being updated

int fontColor;
#define whitebg 0
#define blackbg 1

struct Window {
	bool created;
	bool isVM; // true if EmbedVM is running with this window
	bool VMrunning; // if this window is a VM, then this is true if it's currently running
	int x;
	int y;
	int sizeX;
	int sizeY;
	unsigned char title[10];
	int titleSize; // TODO: this shouldn't be needed, if i really need to get the size of the title then use sizeof()
	int clickX;
	int clickY;
	int prevX; // used while dragging to prevent the screen constantly being redrawn
	int prevY; // ^
	bool dragging;
} window[11];

struct App { // this struct simply holds app names and file names
	unsigned char name[11];
	unsigned char filename[12];
} app[10];

struct Button {
	bool created;
	void (*callback)(); // function that gets called when button is clicked
	int callbackIntArg; // argument that gets passed to callback function. there's probably a better way to do this, but i don't know how
	int window; // window number that this button lives on
} button[100];

struct Cursor {
	int x;
	int y;
	bool click; // cursor was clicked and is waiting to be used
	bool hold; // cursor is being held down
} cursor;

struct Menu {
	bool open;
	int selectedMenu; // the selected menu on the menubar
	int selectedMenuItem; // menu item that's currently being hovered over
	int clickedMenuItem; // the menu item that was clicked
} menu;

void updateCursor() {
	//MoveSprite(0,cursor.x,cursor.y,2,2); // 16x16, good looking but too big
	sprites[0].x = cursor.x;
	sprites[0].y = cursor.y;

	/*int mouse = ReadJoypadExt(1); // copied from whack-a-mole
	btnPressed = mouse & (mouse ^ btnPrev);
	//btnReleased = btnPrev & (mouse ^ btnPrev);

	if (mouse & 0x80) {
		cursor.x -= (mouse&0x7f);
		if (cursor.x<0) cursor.x = 0; 
	} else {
		cursor.x += (mouse&0x7f);
		if (cursor.x>240) cursor.x = 240;
	}
	
	if (mouse & 0x8000) {
		cursor.y -= ((mouse>>8)&0x7f);
		if (cursor.y<0) cursor.y = 0;
	} else {
		cursor.y += ((mouse>>8)&0x7f);
		if (cursor.y>216) cursor.y = 216;
	}

	if (btnPressed & BTN_MOUSE_LEFT) {
		cursor.click = true;
	}

	if (mouse & BTN_MOUSE_LEFT) {
		cursor.hold = true;
	}

	if ((mouse & BTN_MOUSE_LEFT) && ((btnPrev & BTN_MOUSE_LEFT) == 0)) {
		cursor.hold = false;
	}

	btnPrev = mouse;*/
}

void updateController() {
	btnHeld = ReadJoypad(0);
	btnPressed = btnHeld & (btnHeld ^ btnPrev);
	btnReleased = btnPrev & (btnHeld ^ btnPrev);

	if (btnHeld & BTN_UP && cursor.y > 0) {
		cursor.y--;
	}
	if (btnHeld & BTN_DOWN && cursor.y < 216) {
		cursor.y++;
	}
	if (btnHeld & BTN_LEFT && cursor.x > 0) {
		cursor.x--;
	}
	if (btnHeld & BTN_RIGHT && cursor.x < 240) {
		cursor.x++;
	}
	if (btnPressed & BTN_A) {
		cursor.click = true;
	}
	if (btnHeld & BTN_A) {
		cursor.hold = true;
	}
	if (btnReleased & BTN_A) {
		cursor.hold = false;
	}

	btnPrev = btnHeld;
}

void setFontColor(int font) {
	if (font == whitebg || font == 0) {
		SetFontTilesIndex(TILESET_SIZE+SPRITESET_SIZE+FONT_SIZE);
	}
	if (font == blackbg || font == 1) {
		SetFontTilesIndex(TILESET_SIZE+SPRITESET_SIZE);
	}

	fontColor = font;
}

void updateMenubar() {
	setFontColor(whitebg);

	if (menu.open && menu.selectedMenu == 1) { // Uze menu
		SetTile(1,0,6);
		for (int i=0; i<sizeof(uzeMenu)/sizeof(uzeMenu[0]); i++) { // draw menu items and highlight them if mouse is over them
			if (cursor.x > 1*8 && cursor.x < (1+10)*8 && cursor.y > (i+1)*8 && cursor.y < (i+2)*8) { // if mouse is over menu item currently being printed. wow this looks like garbage
				setFontColor(blackbg);
				menu.selectedMenuItem = i+1;
			} else {
				setFontColor(whitebg);
			}

			Print(1,i+1,uzeMenu[i]);
		}

		setFontColor(blackbg);

	} else {
		SetTile(1,0,5);
	}

	if (menu.open && menu.selectedMenu == 10) { // apps menu (apps menu is menu number 10, it isn't a normal menu)
		SetTile(1,0,6);
		for (int i=0; i<sizeof(appsMenu)/sizeof(appsMenu[0]); i++) { // draw menu items and highlight them if mouse is over them
			if (cursor.x > 1*8 && cursor.x < (1+10)*8 && cursor.y > (i+1)*8 && cursor.y < (i+2)*8) { // if mouse is over menu item currently being printed. wow this looks like garbage
				setFontColor(blackbg);
				menu.selectedMenuItem = i+1;
			} else {
				setFontColor(whitebg);
			}

		Print(1,i+1,appsMenu[i]);
		PrintRam(1,i+1,app[i].name);
	}
		setFontColor(blackbg);
	}

	if (menu.open && menu.selectedMenu == 2) { // file menu
		setFontColor(blackbg);
		Print(3,0,PSTR("File"));

		for (int i=0; i<sizeof(fileMenu)/sizeof(fileMenu[0]); i++) { // draw menu items and highlight them if mouse is over them
			if (cursor.x > 3*8 && cursor.x < (3+10)*8 && cursor.y > (i+1)*8 && cursor.y < (i+2)*8) { // if mouse is over menu item currently being printed. wow this looks like garbage
				setFontColor(blackbg);
				menu.selectedMenuItem = i+1;
			} else {
				setFontColor(whitebg);
			}

			Print(3,i+1,fileMenu[i]);
		}

		setFontColor(blackbg);

	} else {
		setFontColor(whitebg);
		Print(3,0,PSTR("File"));
	}

	if (menu.open && menu.selectedMenu == 3) { // edit menu
		setFontColor(blackbg);
		Print(8,0,PSTR("Edit"));

		for (int i=0; i<sizeof(editMenu)/sizeof(editMenu[0]); i++) { // draw menu items and highlight them if mouse is over them
			if (cursor.x > 8*8 && cursor.x < (8+10)*8 && cursor.y > (i+1)*8 && cursor.y < (i+2)*8) { // if mouse is over menu item currently being printed. wow this looks like garbage
				setFontColor(blackbg);
				menu.selectedMenuItem = i+1;
			} else {
				setFontColor(whitebg);
			}

			Print(8,i+1,editMenu[i]);
		}

		setFontColor(blackbg);

	} else {
		setFontColor(whitebg);
		Print(8,0,PSTR("Edit"));
	}

	if (menu.open && menu.selectedMenu == 4) { // window menu
		setFontColor(blackbg);
		Print(13,0,PSTR("Window"));

		for (int i=0; i<sizeof(windowMenu)/sizeof(windowMenu[0]); i++) { // draw menu items and highlight them if mouse is over them
			if (cursor.x > 13*8 && cursor.x < (13+10)*8 && cursor.y > (i+1)*8 && cursor.y < (i+2)*8) { // if mouse is over menu item currently being printed. wow this looks like garbage
				setFontColor(blackbg);
				menu.selectedMenuItem = i+1;
			} else {
				setFontColor(whitebg);
			}

			Print(13,i+1,windowMenu[i]);
			if (window[i+1].created) PrintRam(13,i+1,window[i+1].title);
		}

		setFontColor(blackbg);

	} else {
		setFontColor(whitebg);
		Print(13,0,PSTR("Window"));
	}
}

void updateClick() {
	if (cursor.click) { // clicked somewhere
		cursor.click = false; // click detected, mark it as "used"

		if (menu.open) { // check if the menu is open, if it is then the user probably clicked somewhere in the menu
			menu.clickedMenuItem = menu.selectedMenuItem; // set the clicked item as the item that's currently being hovered over
			menu.selectedMenuItem = 0;
			menu.open = false; // close the menu
			WaitVsync(1);

			drawWallpaper();
			redrawAll();
		}

		if (cursor.x > (window[getActiveWindow()].x)*8 && cursor.x < (window[getActiveWindow()].x+window[getActiveWindow()].sizeX)*8 && cursor.y > (window[getActiveWindow()].y)*8 && cursor.y < (window[getActiveWindow()].y+window[getActiveWindow()].sizeY)*8) { // clicked inside the active window
			window[getActiveWindow()].clickX = cursor.x - (window[getActiveWindow()].x*8); // set the coords of where the window was clicked, with the window corner as (0,0)
			window[getActiveWindow()].clickY = cursor.y - (window[getActiveWindow()].y*8);
		}

		if (cursor.x > (window[getActiveWindow()].x)*8 && cursor.x < (window[getActiveWindow()].x+window[getActiveWindow()].sizeX)*8 && cursor.y > (window[getActiveWindow()].y-1)*8 && cursor.y < (window[getActiveWindow()].y)*8) { // clicked on the titlebar of the active window
			if (cursor.x > (window[getActiveWindow()].x)*8 && cursor.x < (window[getActiveWindow()].x+1)*8 && cursor.y > (window[getActiveWindow()].y-1)*8 && cursor.y < (window[getActiveWindow()].y)*8) { // clicked close button
				destroyWindow(getActiveWindow());
			}
		}

		if (cursor.y <= 8) { // clicked somewhere in menubar
			menu.clickedMenuItem = 0; // clear clicked item, since nothing was clicked yet

			if (cursor.x > 8 && cursor.x < 16) { // clicked Uze menu
				if (!menu.open) { // menu isn't already open, let's open it
					menu.open = true;
					menu.selectedMenu = 1;
					WaitVsync(1);
				} else { // menu is already open, let's close it
					menu.open = false;
					menu.selectedMenu = 0;
					menu.selectedMenuItem = 0;

					drawWallpaper();
					redrawAll();
					WaitVsync(1);
				}
			}

			if (cursor.x > 24 && cursor.x < 56) { // clicked File menu
				if (!menu.open) { // menu isn't already open, let's open it
					menu.open = true;
					menu.selectedMenu = 2;
					WaitVsync(1);
				} else { // menu is already open, let's close it
					menu.open = false;
					menu.selectedMenu = 0;
					menu.selectedMenuItem = 0;
					
					drawWallpaper();
					redrawAll();
					WaitVsync(1);
				}
			}

			if (cursor.x > 64 && cursor.x < 96) { // clicked Edit menu
				if (!menu.open) { // menu isn't already open, let's open it
					menu.open = true;
					menu.selectedMenu = 3;
					WaitVsync(1);
				} else { // menu is already open, let's close it
					menu.open = false;
					menu.selectedMenu = 0;
					menu.selectedMenuItem = 0;

					drawWallpaper();
					redrawAll();
					WaitVsync(1);
				}
			}

			if (cursor.x > 104 && cursor.x < 152) { // clicked Window menu
				if (!menu.open) { // menu isn't already open, let's open it
					menu.open = true;
					menu.selectedMenu = 4;
					WaitVsync(1);
				} else { // menu is already open, let's close it
					menu.open = false;
					menu.selectedMenu = 0;
					menu.selectedMenuItem = 0;

					drawWallpaper();
					redrawAll();
					WaitVsync(1);
				}
			}
		}
	}
}

void handleMenuClick() {
	if (menu.clickedMenuItem != 0) { // clicked in menu
		if (menu.selectedMenu == 1 && menu.clickedMenuItem == 1) { // clicked the about option
			menu.clickedMenuItem = 0;
			menu.selectedMenu = 0;
			menu.selectedMenuItem = 0;
			drawWallpaper();
			redrawAll();
			createAboutWindow();
		}
		if (menu.selectedMenu == 1 && menu.clickedMenuItem == 2) { // clicked the apps option
			menu.clickedMenuItem = 0;
			menu.selectedMenu = 10;
			menu.selectedMenuItem = 0;
			drawWallpaper();
			redrawAll();
			menu.open = true;
		}
		if (menu.selectedMenu == 1 && menu.clickedMenuItem == 3) { // clicked the settings option
			menu.clickedMenuItem = 0;
			menu.selectedMenu = 0;
			menu.selectedMenuItem = 0;
			drawWallpaper();
			redrawAll();
			createSettingsWindow();
		}
		if (menu.selectedMenu == 1 && menu.clickedMenuItem == 4) { // clicked the tiles option
			menu.clickedMenuItem = 0;
			menu.selectedMenu = 0;
			menu.selectedMenuItem = 0;
			drawWallpaper();
			redrawAll();
			createTilesWindow();
		}
		if (menu.selectedMenu == 1 && menu.clickedMenuItem == 5) { // clicked the quit option
			menu.clickedMenuItem = 0;
			menu.selectedMenu = 0;
			menu.selectedMenuItem = 0;
			drawWallpaper();
			redrawAll();
			SoftReset();
		}
		if (menu.selectedMenu == 4 && menu.clickedMenuItem != 0) { // clicked an option in the window menu
			setActiveWindow(menu.clickedMenuItem);
			menu.clickedMenuItem = 0;
			menu.selectedMenu = 0;
			menu.selectedMenuItem = 0;
			drawWallpaper();
			redrawAll();
		}
		if (menu.selectedMenu == 10 && menu.clickedMenuItem != 0) { // clicked an option in the apps menu
			appToLoad = menu.clickedMenuItem-1;
			loadApp = true;
			menu.clickedMenuItem = 0;
			menu.selectedMenu = 0;
			menu.selectedMenuItem = 0;
			drawWallpaper();
			redrawAll();
		}
	}
}

void updateActiveWindow() {
	if (window[getActiveWindow()].created) {
		setFontColor(whitebg);
		Fill(window[getActiveWindow()].x,window[getActiveWindow()].y-1,window[getActiveWindow()].sizeX,1,8); // draw titlebar
		for (int curChar=0; curChar<window[getActiveWindow()].titleSize; curChar++) { // print window title TODO: update this to use PrintRam()
			PrintChar(window[getActiveWindow()].x+curChar+1,window[getActiveWindow()].y-1,window[getActiveWindow()].title[curChar]);
		}
		SetTile(window[getActiveWindow()].x,window[getActiveWindow()].y-1,14); // draw close button

		if (cursor.hold && (cursor.x > (window[getActiveWindow()].x)*8 && cursor.x < (window[getActiveWindow()].x+window[getActiveWindow()].sizeX)*8 && cursor.y > (window[getActiveWindow()].y-1)*8 && cursor.y < (window[getActiveWindow()].y)*8)) { // holding, and not on the close button
			window[getActiveWindow()].dragging = true;
		} else {
			if (!cursor.hold) {
				window[getActiveWindow()].dragging = false;
			}
		}

		if (window[getActiveWindow()].dragging) {
			if (window[getActiveWindow()].x != window[getActiveWindow()].prevX || window[getActiveWindow()].y != window[getActiveWindow()].prevY) {
				drawWallpaper(); // redraw wallpaper to remove old window tiles
				window[getActiveWindow()].prevX = window[getActiveWindow()].x;
				window[getActiveWindow()].prevY = window[getActiveWindow()].y;
				redrawAll(); // hacky, but it works. this redraws all windows on the screen
			}
			window[getActiveWindow()].x = (cursor.x/8)-(window[getActiveWindow()].sizeX/2);
			window[getActiveWindow()].y = (cursor.y/8)+1;

			if (window[getActiveWindow()].y < 2) {
				window[getActiveWindow()].y = 2; // don't allow window to go on top of menubar
			}

			if (window[getActiveWindow()].y+window[getActiveWindow()].sizeY > 27) {
				window[getActiveWindow()].y = 27-window[getActiveWindow()].sizeY; // don't allow window to go past the bottom of the screen
			}

			if (window[getActiveWindow()].x < 0) {
				window[getActiveWindow()].x = 0; // don't allow window to go past left side of the screen
			}

			if (window[getActiveWindow()].x+window[getActiveWindow()].sizeX > 30) {
				window[getActiveWindow()].x = 30-window[getActiveWindow()].sizeX; // don't allow window to go past right side of the screen
			}
		}

		for (int x=window[getActiveWindow()].x; x<window[getActiveWindow()].sizeX+window[getActiveWindow()].x; x++) {
			for (int y=window[getActiveWindow()].y; y<window[getActiveWindow()].sizeY+window[getActiveWindow()].y; y++) {
				SetTile(x,y,SpiRamReadU8(1,(((y-window[getActiveWindow()].y)*window[getActiveWindow()].sizeX)+(x-window[getActiveWindow()].x))+(getActiveWindow()*(24*29))));
			}
		}
	}
}

void updateInactiveTitlebars() {
	for (int i=1; i<9; i++) {
		if (window[i].created && i != getActiveWindow()) {
			setFontColor(blackbg);
			Fill(window[i].x,window[i].y-1,window[i].sizeX,1,0); // draw titlebar
			for (int curChar=0; curChar<window[i].titleSize; curChar++) { // print title
				PrintChar(window[i].x+curChar+1,window[i].y-1,window[i].title[curChar]);
			}
		}
	}
}

void redrawAll() {
	int prevActive = getActiveWindow(); // save window that was active before running this

	for (int i=0; i<10; i++) { // redraw all windows
		setActiveWindow(i);
		updateActiveWindow();
	}

	updateInactiveTitlebars();

	setActiveWindow(prevActive); // restore active window
}

void drawWallpaper() {
	Fill(0,1,30,26,wallpaperTile); // draw background
}

void printWindow(int x, int y, int windowNumber, char *text) { // mostly copied from the Uzebox kernel source
	int i = 0;
	char c;
	while(1) {
		c = text[i++];
		if (c != 0) {
			c = ((c&127)-32);
			if (fontColor == whitebg) setWindowTile(x++,y,windowNumber,TILESET_SIZE+SPRITESET_SIZE+FONT_SIZE+c);
			if (fontColor == blackbg) setWindowTile(x++,y,windowNumber,TILESET_SIZE+SPRITESET_SIZE+c);
		} else {
			break;
		}
	}
}

void printWindowLen(int x, int y, int windowNumber, char *text, int textSize) {
	int i = 0;
	char c;
	for (int num=0; num<textSize; num++) {
		c = text[i++];
		if (c != 0) {
			c = ((c&127)-32);
			if (fontColor == whitebg) setWindowTile(x++,y,windowNumber,TILESET_SIZE+SPRITESET_SIZE+FONT_SIZE+c);
			if (fontColor == blackbg) setWindowTile(x++,y,windowNumber,TILESET_SIZE+SPRITESET_SIZE+c);
		}
	}
}

void printWindowInt(int x, int y, int windowNumber, unsigned int val) { // mostly copied from the Uzebox kernel source
	unsigned char c,i;

	for (i=0; i<5; i++) {
		c = val%10;
		if (val>0 || i==0) {
			if (fontColor == whitebg) setWindowTile(x--,y,windowNumber,TILESET_SIZE+SPRITESET_SIZE+FONT_SIZE+c+16);
			if (fontColor == blackbg) setWindowTile(x--,y,windowNumber,TILESET_SIZE+SPRITESET_SIZE+c+16);
		}
		val = val/10;
	}
}

void setWindowTile(int x, int y, int windowNumber, unsigned int tile) {
	if (getActiveWindow() != 0) SpiRamWriteU8(1,((y*window[windowNumber].sizeX)+x)+(windowNumber*(24*29)),tile);
}

void createButton(int locationX, int locationY, int sizeX, int sizeY, int windowNumber, int buttonNumber, char *text, void (*callbackFunc), int callbackArg1) { // create a button. x and y are the location in the window, not on the whole screen
	for (int x=locationX; x<locationX+sizeX; x++) { // create a button map in the upper 32kb of bank 1
		for (int y=locationY; y<locationY+sizeY; y++) { // ^ to see the button map, add 32768 to the location where the window tiles are read from
			SpiRamWriteU8(1,(((y*window[windowNumber].sizeX)+x)+(windowNumber*(24*29)))+32768,buttonNumber);
		}
	}

	button[buttonNumber].created = true;
	button[buttonNumber].window = windowNumber;
	button[buttonNumber].callback = callbackFunc;
	button[buttonNumber].callbackIntArg = callbackArg1;

	printWindow(locationX,locationY,windowNumber,text);
}

void updateButtonClicks() {
	int buttonNumber = SpiRamReadU8(1,((((window[getActiveWindow()].clickY/8)*window[getActiveWindow()].sizeX)+((window[getActiveWindow()].clickX/8)))+(getActiveWindow()*(24*29)))+32768); // this will contain the button number that was clicked
	if (buttonNumber != 0 && buttonNumber >= 1 && buttonNumber < 100 && button[buttonNumber].created) { // check that the button number is valid, works around a bug that only exists on real hardware
		window[getActiveWindow()].clickX = 300; // reset back to default value of 300, otherwise it will keep thinking the button is clicked until the user clicks somewhere else
		window[getActiveWindow()].clickY = 300;
		button[buttonNumber].callback(button[buttonNumber].callbackIntArg); // call the function assigned to this button
	}
}

void createWindow(int locationX, int locationY, int sizeX, int sizeY, char title[], int titleSize, bool isVM) { // locationX and locationY are where the actual window contents start, not the titlebar
	int newWindowNum = 1; // window number for the window that will be created here
	int numberOfUsedSlots = 0;

	for (int i=10; i>0; i--) { // check for an empty window slot, starting from the bottom
		if (!window[i].created) {
			newWindowNum = i;
		} else {
			numberOfUsedSlots++;
		}
	}

	if (numberOfUsedSlots < 10) { // only create a new window if there is an empty window slot
		window[newWindowNum].created = true;

		window[newWindowNum].isVM = isVM;
		if (isVM)
			window[newWindowNum].VMrunning = true;
		else
			window[newWindowNum].VMrunning = false;

		window[newWindowNum].x = locationX;
		window[newWindowNum].y = locationY;
		window[newWindowNum].prevX = locationX;
		window[newWindowNum].prevY = locationY;
		window[newWindowNum].sizeX = sizeX;
		window[newWindowNum].sizeY = sizeY;
		for (int i=0; i<titleSize; i++) {
			window[newWindowNum].title[i] = title[i];
		}
		window[newWindowNum].titleSize = titleSize;
		window[newWindowNum].clickX = 300; // 300 if not clicked anywhere
		window[newWindowNum].clickY = 300;
		window[newWindowNum].dragging = false;

		clearWindow(newWindowNum,3); // fill new window with white tiles, also clears the button map for this window
		setActiveWindow(newWindowNum);

		// find the center point of the window
		int centerX = (locationX + (locationX + sizeX)) / 2;
		int centerY = (locationY + (locationY + sizeY)) / 2;

		int animSizeX = 0;
		int animSizeY = 0;

		for (int x=centerX; x<locationX+sizeX; x++) { // this animation is very poorly written, and it's not always 100% accurate
			for (int y=centerY; y<locationY+sizeY; y++) { // however, the animation plays so fast that it looks good enough
				Fill(centerX-animSizeX,centerY-animSizeY,animSizeX,animSizeY,8);
				Fill(centerX-animSizeX,centerY,animSizeX,animSizeY,8);
				Fill(centerX,centerY-animSizeY,animSizeX,animSizeY,8);
				Fill(centerX,centerY,animSizeX,animSizeY,8);
			}
			WaitVsync(1);
			if (animSizeX < (((centerX + (sizeX/2)) - locationX)) - 1) {
				animSizeX++;
			}
			if (animSizeY < (((centerY + (sizeY/2)) - locationY)) - 1) {
				animSizeY++;
			}
		}

		// ensure windows are updated on screen, in case multiple windows are created before the main loop continues running
		redrawAll();
	} else {
		Print(14,25,PSTR("No empty slots!")); // temp, only until i add a dialog box function
	}
}

void destroyWindow(int windowNumber) {
	SetTile(window[windowNumber].x, window[windowNumber].y-1, 15); // draw square in the close button
	WaitVsync(3);
	window[windowNumber].created = false;

	if (window[windowNumber].isVM) window[windowNumber].VMrunning = false;
	window[windowNumber].isVM = false;

	for (int i=0; i<10; i++) {
		window[getActiveWindow()].title[i] = '\0';
	}

	for (int buttonNum=0; buttonNum<100; buttonNum++) { // remove any buttons that may have been on this window
		if (button[buttonNum].created && button[buttonNum].window == windowNumber) {
			button[buttonNum].created = false;
			button[buttonNum].window = 0;
			button[buttonNum].callback = 0;
		}
	}

	drawWallpaper();
	redrawAll();

	//Print(20,25,PSTR("Destroyed"));
	setActiveWindow(getActiveWindow()-1);
}

void clearWindow(int windowNumber, int tile) {
	for (int x=window[windowNumber].x; x<window[windowNumber].sizeX+window[windowNumber].x; x++) {
		for (int y=window[windowNumber].y; y<window[windowNumber].sizeY+window[windowNumber].y; y++) {
			SpiRamWriteU8(1,(((y-window[windowNumber].y)*window[windowNumber].sizeX)+(x-window[windowNumber].x))+(windowNumber*(24*29)),tile); // set window background to tile
		}
	}
	for (int x=window[windowNumber].x; x<window[windowNumber].sizeX+window[windowNumber].x; x++) {
		for (int y=window[windowNumber].y; y<window[windowNumber].sizeY+window[windowNumber].y; y++) {
			SpiRamWriteU8(1,((((y-window[windowNumber].y)*window[windowNumber].sizeX)+(x-window[windowNumber].x))+(windowNumber*(24*29)))+32768,0); // clear button map
		}
	}
}

void setActiveWindow(int windowNumber) { // very simple, but i'm making this a function in case i need to add more later
	activeWindow = windowNumber;
}

int getActiveWindow() {
	return activeWindow;
}

void initialize() {
	ClearVram();
	SetTileTable(tileset);
	SetSpritesTileTable(spriteset);
	setFontColor(whitebg);

	sprites[0].tileIndex = 1;

	if (EepromReadBlock(48879,&ebs) != 0) {
		ebs.data[0] = 1; // byte 0 is the wallpaper tile
		EepromWriteBlock(&ebs);
	}

	wallpaperTile = ebs.data[0]; // byte 0 is the wallpaper tile

	Fill(0,0,30,27,wallpaperTile); // draw background

	// draw menubar
	SetTile(0,0,2);
	Fill(1,0,28,1,3);
	SetTile(29,0,4);

	SetTile(1,0,5); // draw Uze menu button

	//MapSprite(0, cursor_map);

	cursor.x = 4;
	cursor.y = 4;
	cursor.click = false;

	menu.open = false;
	menu.selectedMenu = 0;
	menu.selectedMenuItem = 0;
	menu.clickedMenuItem = 0;

	/*SetRenderingParameters(20,8); // only render first line, speeds up clearing spiram
	for (unsigned int i=0; i<60000; i++) { // clear spiram
		SpiRamWriteU8(0,i,0);
		SpiRamWriteU8(1,i,0);
		PrintInt(8,0,i,false);
	}
	SetRenderingParameters(20,216);
	Fill(1,0,28,1,3);*/

	for (int i=0; i<100; i++) {
		button[i].created = false; // no buttons are created yet, so set all to false
	}

	if (!SpiRamInit()) { // initialize SPIRAM
		Print(3,0,PSTR("No SPI RAM!"));
		while(1);
	}
}

void vsyncCallback(void) {
	frame++;
	uptime = frame/60;
}

int main() {
	SetUserPreVsyncCallback(&vsyncCallback);
	initialize();

	// sd card stuff
	u8  res;
	sdc_struct_t sd_struct;
	u8  buf[512];
	u32 t32;
	sd_struct.bufp = &(buf[0]);

	res = FS_Init(&sd_struct);
	if (res != 0U) {
		Print(3,0,PSTR("No SD Card!"));
		PrintChar(36U, 3U, res + '0');
		while(1);
	}

	ebs.id = 48879;

	t32 = FS_Find(&sd_struct, // look for uzeboxui.txt
	    ((u16)('U') << 8) |
	    ((u16)('Z')     ),
	    ((u16)('E') << 8) |
	    ((u16)('B')     ),
	    ((u16)('O') << 8) |
	    ((u16)('X')     ),
	    ((u16)('U') << 8) |
	    ((u16)('I')     ),
	    ((u16)('T') << 8) |
	    ((u16)('X')     ),
	    ((u16)('T') << 8) |
	    ((u16)(0)       ));

	if (t32 == 0U){
		Print(3,0,PSTR("No file!"));
		while(1);
	}

	FS_Select_Cluster(&sd_struct, t32);
	FS_Reset_Sector(&sd_struct);
	FS_Read_Sector(&sd_struct); // read from file

	// parse the config file
	int nameIndex = 0;
	int appIndex = 0;
	bool readingName = false;
	bool readingFilename = false;
	for (int i=0; i<512; i++) {
		if (buf[i] == '[') {
			readingName = true;
			continue;
		}
		if (buf[i] == ']') {
			readingName = false;
			app[appIndex].name[nameIndex] = '\0';
			nameIndex = 0;
			continue;
		}
		if (buf[i] == '(') {
			readingFilename = true;
			continue;
		}
		if (buf[i] == ')') {
			readingFilename = false;
			app[appIndex].filename[nameIndex] = '\0';
			nameIndex = 0;
			continue;
		}
		if (buf[i] == '\\') {
			readingName = false;
			readingFilename = false;
			nameIndex = 0;
			appIndex++;
			continue;
		}
		if (readingName) {
			app[appIndex].name[nameIndex] = buf[i];
			nameIndex++;
		}
		if (readingFilename) {
			app[appIndex].filename[nameIndex] = buf[i];
			nameIndex++;
		}
	}

	EnableSnesMouse(0,cursor_map);

	while(1) { // main loop
		updateController();
		updateCursor();
		updateClick();

		updateInactiveTitlebars();
		updateActiveWindow();

		handleMenuClick();
		updateMenubar();

		updateButtonClicks();

		if (loadApp) {
			loadApp = false;

			setFontColor(whitebg);
			Fill(1,0,28,1,3);
			SetTile(1,0,5);
			PrintRam(3,0,app[appToLoad].name);
			WaitVsync(20);

			t32 = FS_Find(&sd_struct, // look for file
	    		((u16)(app[appToLoad].filename[0]) << 8) |
	    		((u16)(app[appToLoad].filename[1])     ),
	    		((u16)(app[appToLoad].filename[2]) << 8) |
	    		((u16)(app[appToLoad].filename[3])     ),
	    		((u16)(app[appToLoad].filename[4]) << 8) |
	    		((u16)(app[appToLoad].filename[5])     ),
	    		((u16)(app[appToLoad].filename[6]) << 8) |
	    		((u16)(app[appToLoad].filename[7])     ),
	    		((u16)(app[appToLoad].filename[8]) << 8) |
	    		((u16)(app[appToLoad].filename[9])     ),
	    		((u16)(app[appToLoad].filename[10]) << 8) |
	    		((u16)(0)       ));

			if (t32 == 0U){
				Fill(1,0,28,1,3);
				SetTile(1,0,5);
				Print(3,0,PSTR("No file!"));
				PrintRam(12,0,app[appToLoad].filename);
				WaitVsync(120);
				Fill(1,0,28,1,3);
			}

			FS_Select_Cluster(&sd_struct, t32);
			FS_Reset_Sector(&sd_struct);
			FS_Read_Sector(&sd_struct); // read from file

			int newWindowNum = 1;
			for (int i=10; i>0; i--) { // check for an empty window/VM slot, starting from the bottom
				if (!window[i].created) // this is to figure out where the application data needs to be for the new VM
					newWindowNum = i;
			}

			for (int i=0; i<512; i++) {
				SpiRamWriteU8(0,(3072*(newWindowNum-1))+i,buf[i]); // read data from file into bank 0
			}

			createVM();

			Fill(1,0,28,1,3);
		}

		if (window[getActiveWindow()].isVM && window[getActiveWindow()].VMrunning) {
			for (int i=10; i>0; i--) { // execute 10 instructions
				embedvm_exec(&vm[getActiveWindow()]);
				PrintHexInt(25,0,vm[getActiveWindow()].ip);
			}
		}

		WaitVsync(1);
	}
}

void createVM() {
	int newWindowNum = 1; // the window number that will be assigned to the window created by this VM
	int numberOfUsedSlots = 0;

	for (int i=10; i>0; i--) { // check for an empty window/VM slot, starting from the bottom
		if (!window[i].created) {
			newWindowNum = i;
		} else {
			numberOfUsedSlots++;
		}
	}

	if (numberOfUsedSlots < 10) { // only create a new window if there is an empty window slot
		PrintInt(5,25,(3072*(newWindowNum-1)),false);

		vm[newWindowNum].ip = 3072*(newWindowNum-1); // this entry point should contain a jump to main()
		vm[newWindowNum].sp = vm[newWindowNum].sfp = 32768 - (3072*(newWindowNum-1)); // each VM gets 3KB for its stack
		vm[newWindowNum].mem_read = &mem_read;
		vm[newWindowNum].mem_write = &mem_write;
		vm[newWindowNum].call_user = &call_user;
		embedvm_interrupt(&vm[newWindowNum], 3072*(newWindowNum-1));

		window[newWindowNum].isVM = true;
		window[newWindowNum].VMrunning = true;

		Print(23,0,PSTR("0x"));
		for (int i=250; i>0; i--) { // execute 250 instructions, should be enough for the application to create a window
			embedvm_exec(&vm[newWindowNum]);
			PrintHexInt(25,0,vm[getActiveWindow()].ip);
		}
	} else {
		Print(14,25,PSTR("No empty slots!")); // temp, only until i add a dialog box function
	}
}

// About window

void createAboutWindow() {
	createWindow(5,5,10,10,"About",5,false);
	clearWindow(getActiveWindow(),0); // fill window with black tiles (tile 0)

	for (int x=3; x<8; x++) {
		for (int y=1; y<5; y++) {
			setWindowTile(x,y,getActiveWindow(),23+((y-3)*5)+x); // draw Uzebox logo
		}
	}

	setFontColor(blackbg);
	printWindow(1,6,getActiveWindow(),"UzeboxUI");
	printWindow(1,7,getActiveWindow(),"by ry755");
	printWindow(1,8,getActiveWindow(),"& luawtf");
}

// Tiles window (tile info)

void createTilesWindow() {
	createWindow(5,5,11,13,"Tile Info",9,false);

	setFontColor(whitebg);
	printWindow(1,1,getActiveWindow(),"Total");
	printWindowInt(8,3,getActiveWindow(),256-RAM_TILES_COUNT); // 256 is the max amount of tiles that can be used
	printWindow(5,3,getActiveWindow(),"/");
	printWindowInt(4,3,getActiveWindow(),TILESET_SIZE+SPRITESET_SIZE+FONT_SIZE+FONT_INVERT_SIZE); // sum of sizes of all tilesets

	printWindow(1,5,getActiveWindow(),"RAM Tiles");
	printWindowInt(2,7,getActiveWindow(),RAM_TILES_COUNT);

	printWindow(1,9,getActiveWindow(),"Fonts");
	printWindowInt(4,11,getActiveWindow(),FONT_SIZE+FONT_INVERT_SIZE);
}

// Settings window

void settingsChangeWallpaper(int num) {
	wallpaperTile += num;
	setWindowTile(3,3,getActiveWindow(),wallpaperTile); // update the tile in the settings
	drawWallpaper();
	redrawAll();
}

void settingsSaveWallpaper() {
	EepromReadBlock(48879,&ebs);
	ebs.data[0] = wallpaperTile; // byte 0 is the wallpaper tile
	EepromWriteBlock(&ebs);
}

void createSettingsWindow() {
	createWindow(5,5,16,5,"Settings",8,false);
	setFontColor(whitebg);
	printWindow(1,1,getActiveWindow(),"Wallpaper");

	setWindowTile(3,3,getActiveWindow(),wallpaperTile);
	createButton(1,3,1,1,getActiveWindow(),1,"<",settingsChangeWallpaper,-1);
	createButton(5,3,1,1,getActiveWindow(),2,">",settingsChangeWallpaper,1);
	createButton(7,3,4,1,getActiveWindow(),3,"Save",settingsSaveWallpaper,0);
}
