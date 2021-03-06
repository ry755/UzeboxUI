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

#include "data/patches.inc"
#include "data/tileset.inc"
#include "data/spriteset.inc"
#include "data/fonts/envious-blackbg.inc"
#include "data/fonts/envious-whitebg.inc"

int btnHeld = 0;                  // buttons held right now
int btnPressed = 0;               // buttons pressed this frame
int btnReleased = 0;              // buttons released this frame
int btnPrev = 0;                  // buttons previously pressed

int wallpaperTile = 1;            // default wallpaper tile is 1

uint32_t frame = 0;               // frame counter
uint32_t uptime = 0;              // uptime counter in seconds

int numberOfApps = 0;

struct EepromBlockStruct ebs;

extern unsigned char ram_tiles[];

// sd card stuff
u8  res;
sdc_struct_t sd_struct;
u32 t32;

unsigned int appSectors = 0;      // sectors required to load the selected app

const char cursor_map[] PROGMEM = {
	1,2, // width/height of map
	1 // tile indexes
};

int activeWindow = 0;             // keeps track of the window number that's currently active and being updated

int fontColor;
#define whitebg 0
#define blackbg 1

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
void printWindowInt(int x, int y, int windowNumber, unsigned int val, bool zeropad);
void setWindowTile(int x, int y, int windowNumber, unsigned int tile);
void createButton(int locationX, int locationY, int sizeX, int sizeY, int windowNumber, int buttonNumber, char *text, void (*callbackFunc), int callbackArg1);
void createVMButton(int locationX, int locationY, int sizeX, int sizeY, int windowNumber, int buttonNumber, char *text, int textSize);
void updateButtonClicks();
void createWindow(int locationX, int locationY, int sizeX, int sizeY, char title[], int titleSize, bool isVM);
void destroyWindow(int windowNumber);
void clearWindow(int windowNumber, int tile);
void setActiveWindow(int windowNumber);
void setAppName(int app, int index, char character);
char getAppName(int app, int index);
void setAppFileName(int app, int index, char character);
char getAppFileName(int app, int index);
void initialize();
void initScreen();
void splash();
void loadAppsConfig();
void loadApp(int app);
void vsyncCallback(void);
void createVM();
void createAboutWindow();
void createTilesWindow();
void settingsChangeWallpaper(int num);
void settingsSaveWallpaper();
void createSettingsWindow();

struct Button {
	bool created;
	void (*callback)();           // function that gets called when button is clicked
	int callbackIntArg;           // argument that gets passed to callback function
	bool isVM;                    // true if this button lives on a VM window
	bool VMwasClicked;            // if this button is on a VM window, then this will be true if it was clicked. this allows easy checking by the VM
};

struct Window {
	bool created;
	bool isVM;                    // true if EmbedVM is running with this window
	bool VMrunning;               // if this window is a VM, then this is true if it's currently running
	bool dragging;                // true if window is being dragged
	int x;
	int y;
	int sizeX;
	int sizeY;
	unsigned char title[10];
	int clickX;
	int clickY;
	int prevX;                    // used while dragging to prevent the screen constantly being redrawn
	int prevY;                    // ^

	struct Button button[15];
} window[10];

struct Cursor {
	int x;
	int y;
	bool click;                   // cursor was clicked and is waiting to be used
	bool hold;                    // cursor is being held down
} cursor;

struct Menu {
	bool open;
	int selectedMenu;             // the selected menu on the menubar
	int selectedMenuItem;         // menu item that's currently being hovered over
	int clickedMenuItem;          // the menu item that was clicked
} menu;

// Menu //////////////////////////////////////////////////////////////////

const char uzeMenu[][44] PROGMEM = {
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

//////////////////////////////////////////////////////////////////////////

// EmbedVM ///////////////////////////////////////////////////////////////

unsigned int activeVM = 0;

struct embedvm_s vm[11] = { };

int16_t mem_read(uint16_t addr, bool is16bit, void *ctx) {
	/*if (((3072*(activeVM-1))+addr) + (is16bit ? 1 : 0) >= 32768) {
		PrintInt(8,23,0,false);
		return 0;
	}*/
	if (is16bit) {
		return (SpiRamReadU8(0,((3072*(activeVM-1))+addr)) << 8) | SpiRamReadU8(0,((3072*(activeVM-1))+addr)+1);
	}
	//Print(1,23,PSTR("r: "));
	//PrintInt(8,23,((3072*(activeVM-1))+addr),false);
	return SpiRamReadU8(0,((3072*(activeVM-1))+addr));
}

void mem_write(uint16_t addr, int16_t value, bool is16bit, void *ctx) {
	/*if (((3072*(activeVM-1))+addr) + (is16bit ? 1 : 0) >= 32768) {
		PrintInt(8,24,0,false);
		return;
	}*/
	if (is16bit) {
		SpiRamWriteU8(0,((3072*(activeVM-1))+addr),value >> 8);
		SpiRamWriteU8(0,((3072*(activeVM-1))+addr)+1,value);
	} else {
		SpiRamWriteU8(0,((3072*(activeVM-1))+addr),value);
	}

	//Print(1,24,PSTR("w: "));
	//PrintInt(8,24,((3072*(activeVM-1))+addr),false);
}

char tempChar[32];
int tempInt = 0;

int16_t call_user(uint8_t funcid, uint8_t argc, int16_t *argv, void *ctx) {
	if (funcid == 0) { // build up a string one character at a time. yes this is weird, but it's the only way that would work
		if (argc == 0) {
			tempChar[0] = '\0';
			tempInt = 0;
		} else {
			if (tempInt < 32) {
				tempChar[tempInt] = argv[0];
				tempInt++;
			}
		}
		return 0;
	}
	if (funcid == 1 && argc == 5) { // create a window
		int locationX = argv[0];
		int locationY = argv[1];
		int sizeX = argv[2];
		int sizeY = argv[3];
		int titleSize = argv[4];

		createWindow(locationX,locationY,sizeX,sizeY,tempChar,titleSize,true);
		return 0;
	}
	if (funcid == 2 && argc == 5) { // print in a window
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
		return activeWindow;
	}
	if (funcid == 4 && argc == 2) { // clear window
		int windowNumber = argv[0];
		int tile = argv[1];
		clearWindow(windowNumber, tile);
		return 0;
	}
	if (funcid == 5 && argc == 4) { // set a tile in a window
		int locationX = argv[0];
		int locationY = argv[1];
		int windowNumber = argv[2];
		int tile = argv[3];

		setWindowTile(locationX,locationY,windowNumber,tile);
		return 0;
	}
	if (funcid == 6 && argc == 4) { // set a ramtile in a window
		int locationX = argv[0];
		int locationY = argv[1];
		int windowNumber = argv[2];
		int tile = argv[3];

		setWindowTile(locationX,locationY,windowNumber,tile-RAM_TILES_COUNT);
		return 0;
	}
	if (funcid == 7 && argc == 4) { // write to a ramtile
		int locationX = argv[0];
		int locationY = argv[1];
		int ramtile = argv[2];
		int color = argv[3];

		ram_tiles[(ramtile*64)+((locationY*8)+locationX)] = color;
		return 0;
	}
	if (funcid == 8 && argc == 2) { // play patch
		unsigned char patch = argv[0];
		unsigned char volume = argv[1];

		TriggerFx(patch,volume,false);
		return 0;
	}
	if (funcid == 9 && argc == 4) { // play note
		unsigned char channel = argv[0];
		unsigned char patch = argv[1];
		unsigned char note = argv[2];
		unsigned char volume = argv[3];

		TriggerNote(channel,patch,note,volume);
		return 0;
	}
	if (funcid == 10 && argc == 6) { // print window int
		int locationX = argv[0];
		int locationY = argv[1];
		int windowNumber = argv[2];
		unsigned int num = argv[3];
		int color = argv[4];
		bool zeros = argv[5];

		setFontColor(color);
		printWindowInt(locationX,locationY,windowNumber,num,zeros);
		return 0;
	}
	if (funcid == 11 && argc == 8) { // create button
		int locationX = argv[0];
		int locationY = argv[1];
		int sizeX = argv[2];
		int sizeY = argv[3];
		int windowNumber = argv[4];
		int buttonNumber = argv[5];
		int textSize = argv[6];
		int color = argv[7];

		setFontColor(color);
		createVMButton(locationX,locationY,sizeX,sizeY,windowNumber,buttonNumber,tempChar,textSize);
		return 0;
	}
	if (funcid == 12 && argc == 2) { // check button status
		int windowNumber = argv[0];
		int buttonNumber = argv[1];

		bool status = window[windowNumber].button[buttonNumber].VMwasClicked;
		if (status) window[windowNumber].button[buttonNumber].VMwasClicked = false; // button was checked, set the status back to false
		return status;
	}
	if (funcid == 13) { // get uptime in seconds
		return uptime;
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////

void updateCursor() {
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
	if (font == whitebg) {
		SetFontTilesIndex(TILESET_SIZE+SPRITESET_SIZE+FONT_SIZE);
	}
	if (font == blackbg) {
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
		for (int i=0; i<numberOfApps; i++) { // draw menu items and highlight them if mouse is over them
			if (cursor.x > 1*8 && cursor.x < (1+10)*8 && cursor.y > (i+1)*8 && cursor.y < (i+2)*8) { // if mouse is over menu item currently being printed. wow this looks like garbage
				setFontColor(blackbg);
				menu.selectedMenuItem = i+1;
			} else {
				setFontColor(whitebg);
			}

		Print(1,i+1,PSTR("          "));
		//PrintRam(1,i+1,app[i].name);
		for (int x=1; x<11; x++) {
			if (getAppName(i,x-1) != 0) PrintChar(x,i+1,getAppName(i,x-1));
		}
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

		int num = 0;
		for (int i=0; i<10; i++) { // check for slots
			if (window[i].created) {
				num = i;
			}
		}

		for (int i=1; i<num+1; i++) { // draw menu items and highlight them if mouse is over them
			if (cursor.x > 13*8 && cursor.x < (13+10)*8 && cursor.y > (i)*8 && cursor.y < (i+1)*8) { // if mouse is over menu item currently being printed. wow this looks like garbage
				setFontColor(blackbg);
				menu.selectedMenuItem = i;
			} else {
				setFontColor(whitebg);
			}

			//Print(13,i+1,windowMenu[i]);
			Print(13,i,PSTR("          "));
			if (window[i].title[0] != '\0') {
				if (window[i].created)
					PrintRam(13,i,window[i].title);
			}
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

		if (cursor.x > (window[activeWindow].x)*8 && cursor.x < (window[activeWindow].x+window[activeWindow].sizeX)*8 && cursor.y > (window[activeWindow].y)*8 && cursor.y < (window[activeWindow].y+window[activeWindow].sizeY)*8) { // clicked inside the active window
			window[activeWindow].clickX = cursor.x - (window[activeWindow].x*8); // set the coords of where the window was clicked, with the window corner as (0,0)
			window[activeWindow].clickY = cursor.y - (window[activeWindow].y*8);
		}

		if (cursor.x > (window[activeWindow].x)*8 && cursor.x < (window[activeWindow].x+window[activeWindow].sizeX)*8 && cursor.y > (window[activeWindow].y-1)*8 && cursor.y < (window[activeWindow].y)*8) { // clicked on the titlebar of the active window
			if (cursor.x > (window[activeWindow].x)*8 && cursor.x < (window[activeWindow].x+1)*8 && cursor.y > (window[activeWindow].y-1)*8 && cursor.y < (window[activeWindow].y)*8) { // clicked close button
				destroyWindow(activeWindow);
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
			loadApp(menu.clickedMenuItem-1);
			menu.clickedMenuItem = 0;
			menu.selectedMenu = 0;
			menu.selectedMenuItem = 0;
			drawWallpaper();
			redrawAll();
		}
	}
}

void updateActiveWindow() {
	if (window[activeWindow].created) {
		setFontColor(whitebg);
		Fill(window[activeWindow].x,window[activeWindow].y-1,window[activeWindow].sizeX,1,8); // draw titlebar
		PrintRam(window[activeWindow].x+1,window[activeWindow].y-1,window[activeWindow].title); // print window title
		SetTile(window[activeWindow].x,window[activeWindow].y-1,14); // draw close button

		if (cursor.hold && (cursor.x > (window[activeWindow].x)*8 && cursor.x < (window[activeWindow].x+window[activeWindow].sizeX)*8 && cursor.y > (window[activeWindow].y-1)*8 && cursor.y < (window[activeWindow].y)*8)) { // holding, and not on the close button
			window[activeWindow].dragging = true;
		} else {
			if (!cursor.hold) {
				window[activeWindow].dragging = false;
			}
		}

		if (window[activeWindow].dragging) {
			if (window[activeWindow].x != window[activeWindow].prevX || window[activeWindow].y != window[activeWindow].prevY) {
				drawWallpaper(); // redraw wallpaper to remove old window tiles
				window[activeWindow].prevX = window[activeWindow].x;
				window[activeWindow].prevY = window[activeWindow].y;
				redrawAll(); // hacky, but it works. this redraws all windows on the screen
			}
			window[activeWindow].x = (cursor.x/8)-(window[activeWindow].sizeX/2);
			window[activeWindow].y = (cursor.y/8)+1;

			if (window[activeWindow].y < 2) {
				window[activeWindow].y = 2; // don't allow window to go on top of menubar
			}

			if (window[activeWindow].y+window[activeWindow].sizeY > 27) {
				window[activeWindow].y = 27-window[activeWindow].sizeY; // don't allow window to go past the bottom of the screen
			}

			if (window[activeWindow].x < 0) {
				window[activeWindow].x = 0; // don't allow window to go past left side of the screen
			}

			if (window[activeWindow].x+window[activeWindow].sizeX > 30) {
				window[activeWindow].x = 30-window[activeWindow].sizeX; // don't allow window to go past right side of the screen
			}
		}

		for (int x=window[activeWindow].x; x<window[activeWindow].sizeX+window[activeWindow].x; x++) {
			for (int y=window[activeWindow].y; y<window[activeWindow].sizeY+window[activeWindow].y; y++) {
				SetTile(x,y,SpiRamReadU8(1,(((y-window[activeWindow].y)*window[activeWindow].sizeX)+(x-window[activeWindow].x))+(activeWindow*(24*29))));
			}
		}
	}
}

void updateInactiveTitlebars() {
	for (int i=1; i<10; i++) {
		if (window[i].created && i != activeWindow) {
			setFontColor(blackbg);
			Fill(window[i].x,window[i].y-1,window[i].sizeX,1,0); // draw titlebar
			PrintRam(window[i].x+1,window[i].y-1,window[i].title); // print window title
		}
	}
}

void redrawAll() {
	int prevActive = activeWindow; // save window that was active before running this

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

void printWindowInt(int x, int y, int windowNumber, unsigned int val, bool zeropad) { // mostly copied from the Uzebox kernel source
	unsigned char c,i;

	for (i=0; i<5; i++) {
		c = val%10;
		if (val>0 || i==0) {
			if (fontColor == whitebg) setWindowTile(x--,y,windowNumber,TILESET_SIZE+SPRITESET_SIZE+FONT_SIZE+c+16);
			if (fontColor == blackbg) setWindowTile(x--,y,windowNumber,TILESET_SIZE+SPRITESET_SIZE+c+16);
		} else {
			if (zeropad) {
				if (fontColor == whitebg) setWindowTile(x--,y,windowNumber,TILESET_SIZE+SPRITESET_SIZE+FONT_SIZE+16);
				if (fontColor == blackbg) setWindowTile(x--,y,windowNumber,TILESET_SIZE+SPRITESET_SIZE+16);
			}
		}
		val = val/10;
	}
}

void setWindowTile(int x, int y, int windowNumber, unsigned int tile) {
	if (activeWindow != 0) SpiRamWriteU8(1,((y*window[windowNumber].sizeX)+x)+(windowNumber*(24*29)),tile);
}

void createButton(int locationX, int locationY, int sizeX, int sizeY, int windowNumber, int buttonNumber, char *text, void (*callbackFunc), int callbackArg1) { // create a button. x and y are the location in the window, not on the whole screen
	for (int x=locationX; x<locationX+sizeX; x++) { // create a button map in the upper 32kb of bank 1
		for (int y=locationY; y<locationY+sizeY; y++) { // ^ to see the button map, add 32768 to the location where the window tiles are read from
			SpiRamWriteU8(1,(((y*window[windowNumber].sizeX)+x)+(windowNumber*(24*29)))+32768,buttonNumber);
		}
	}

	window[windowNumber].button[buttonNumber].created = true;
	window[windowNumber].button[buttonNumber].callback = callbackFunc;
	window[windowNumber].button[buttonNumber].callbackIntArg = callbackArg1;

	window[windowNumber].button[buttonNumber].isVM = false;
	window[windowNumber].button[buttonNumber].VMwasClicked = false;

	printWindow(locationX,locationY,windowNumber,text);
}

void createVMButton(int locationX, int locationY, int sizeX, int sizeY, int windowNumber, int buttonNumber, char *text, int textSize) { // create a button in a VM. x and y are the location in the window, not on the whole screen
	for (int x=locationX; x<locationX+sizeX; x++) { // create a button map in the upper 32kb of bank 1
		for (int y=locationY; y<locationY+sizeY; y++) { // ^ to see the button map, add 32768 to the location where the window tiles are read from
			SpiRamWriteU8(1,(((y*window[windowNumber].sizeX)+x)+(windowNumber*(24*29)))+32768,buttonNumber);
		}
	}

	window[windowNumber].button[buttonNumber].created = true;

	window[windowNumber].button[buttonNumber].isVM = true;
	window[windowNumber].button[buttonNumber].VMwasClicked = false;

	printWindowLen(locationX,locationY,windowNumber,text,textSize);
}

void updateButtonClicks() {
	int buttonNumber = SpiRamReadU8(1,((((window[activeWindow].clickY/8)*window[activeWindow].sizeX)+((window[activeWindow].clickX/8)))+(activeWindow*(24*29)))+32768); // this will contain the button number that was clicked
	if (buttonNumber != 0 && buttonNumber >= 1 && buttonNumber < 100 && window[activeWindow].button[buttonNumber].created && !window[activeWindow].button[buttonNumber].isVM) { // (not VM) check that the button number is valid, works around a bug that only exists on real hardware
		window[activeWindow].clickX = 300; // reset back to default value of 300, otherwise it will keep thinking the button is clicked until the user clicks somewhere else
		window[activeWindow].clickY = 300;
		window[activeWindow].button[buttonNumber].callback(window[activeWindow].button[buttonNumber].callbackIntArg); // call the function assigned to this button
	}

	if (buttonNumber != 0 && buttonNumber >= 1 && buttonNumber < 100 && window[activeWindow].button[buttonNumber].created && window[activeWindow].button[buttonNumber].isVM) { // (VM) check that the button number is valid, works around a bug that only exists on real hardware
		window[activeWindow].clickX = 300; // reset back to default value of 300, otherwise it will keep thinking the button is clicked until the user clicks somewhere else
		window[activeWindow].clickY = 300;
		window[activeWindow].button[buttonNumber].VMwasClicked = true; // this button was clicked. when the VM checks the button, this gets reset back to false
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
		window[newWindowNum].title[titleSize] = '\0';
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
		window[activeWindow].title[i] = '\0';
	}

	for (int buttonNum=0; buttonNum<10; buttonNum++) { // remove any buttons that may have been on this window
		if (window[windowNumber].button[buttonNum].created) {
			window[windowNumber].button[buttonNum].created = false;
			window[windowNumber].button[buttonNum].callback = 0;
		}
	}

	drawWallpaper();
	redrawAll();

	setActiveWindow(activeWindow-1);
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

void setActiveWindow(int windowNumber) {
	activeWindow = windowNumber;
	if (window[windowNumber].isVM && window[windowNumber].VMrunning)
		activeVM = windowNumber;
}

void setAppName(int app, int index, char character) {
	SpiRamWriteU8(0,32770+(app*11)+index,character);
}

char getAppName(int app, int index) {
	return SpiRamReadU8(0,32770+(app*11)+index);
}

void setAppFileName(int app, int index, char character) {
	SpiRamWriteU8(0,32880+(app*12)+index,character);
}

char getAppFileName(int app, int index) {
	return SpiRamReadU8(0,32880+(app*12)+index);
}

void initialize() {
	ClearVram();
	SetTileTable(tileset);
	SetSpritesTileTable(spriteset);
	setFontColor(whitebg);
	SetUserRamTilesCount(9);
	InitMusicPlayer(patches);

	sprites[0].tileIndex = 1;

	if (EepromReadBlock(48879,&ebs) != 0) {
		ebs.data[0] = 1; // byte 0 is the wallpaper tile
		EepromWriteBlock(&ebs);
	}

	wallpaperTile = ebs.data[0]; // byte 0 is the wallpaper tile

	//MapSprite(0, cursor_map);

	cursor.x = 4;
	cursor.y = 4;
	cursor.click = false;

	menu.open = false;
	menu.selectedMenu = 0;
	menu.selectedMenuItem = 0;
	menu.clickedMenuItem = 0;

	for (int num=0; num<10; num++) {
		for (int i=0; i<10; i++) {
			window[num].button[i].created = false; // no buttons are created yet, so set all to false
		}
	}

	sd_struct.bufp = &(vram[30]);

	res = FS_Init(&sd_struct);
	if (res != 0U) {
		Print(3,0,PSTR("No SD Card!"));
		while(1);
	}

	ebs.id = 48879; // eeprom block id

	if (!SpiRamInit()) { // initialize SPIRAM
		Print(3,0,PSTR("No SPI RAM!"));
		while(1);
	}
}

void initScreen() {
	Fill(0,0,30,27,wallpaperTile); // draw background

	// draw menubar
	SetTile(0,0,2);
	Fill(1,0,28,1,3);
	SetTile(29,0,4);

	SetTile(1,0,5); // draw Uze menu button
}

void splash() {
	SetRenderingParameters(FIRST_RENDER_LINE,8);
	t32 = FS_Find(&sd_struct, // look for splash.raw
		((u16)('S') << 8) |
		((u16)('P')     ),
		((u16)('L') << 8) |
		((u16)('A')     ),
		((u16)('S') << 8) |
		((u16)('H')     ),
		((u16)(' ') << 8) |
		((u16)(' ')     ),
		((u16)('R') << 8) |
		((u16)('A')     ),
		((u16)('W') << 8) |
    	((u16)(0)       ));

	if (t32 == 0U) { // splash.bin not found, use Uzebox logo instead
		ClearVram();
		for (int x=((SCREEN_TILES_H/2U)-2U); x<((SCREEN_TILES_H/2U)-2U)+5; x++) {
			for (int y=12; y<16; y++) {
				SetTile(x,y,(((y-3)*5)+x)-42);
			}
		}
	} else { // splash.bin was found! let's load it into ramtiles
		FS_Select_Cluster(&sd_struct, t32);
		FS_Reset_Sector(&sd_struct);
		FS_Read_Sector(&sd_struct); // read from file

		int index = 0;
		for (int ramtile=0; ramtile<8; ramtile++) {
			for (int y=0; y<8; y++) {
				for (int x=0; x<8; x++) {
					if (ramtile == 0 && x == 0 && y == 0) index = 0;
					if (ramtile == 1 && x == 0 && y == 0) index = 8;
					if (ramtile == 2 && x == 0 && y == 0) index = 16;
					if (ramtile == 3 && x == 0 && y == 0) index = 24;
					if (ramtile == 4 && x == 0 && y == 0) index = 256;
					if (ramtile == 5 && x == 0 && y == 0) index = 264;
					if (ramtile == 6 && x == 0 && y == 0) index = 272;
					if (ramtile == 7 && x == 0 && y == 0) index = 280;
					ram_tiles[(ramtile*64)+((y*8)+x)] = vram[30+index]; // vram is used as the sd card buffer
					index++;
				}
				index += 24;
			}
		}
		
		ClearVram();
		SetTile(((SCREEN_TILES_H/2U)-1U)-1,12,0-RAM_TILES_COUNT);
		SetTile(((SCREEN_TILES_H/2U)-1U)+0,12,1-RAM_TILES_COUNT);
		SetTile(((SCREEN_TILES_H/2U)-1U)+1,12,2-RAM_TILES_COUNT);
		SetTile(((SCREEN_TILES_H/2U)-1U)+2,12,3-RAM_TILES_COUNT);
		SetTile(((SCREEN_TILES_H/2U)-1U)-1,13,4-RAM_TILES_COUNT);
		SetTile(((SCREEN_TILES_H/2U)-1U)+0,13,5-RAM_TILES_COUNT);
		SetTile(((SCREEN_TILES_H/2U)-1U)+1,13,6-RAM_TILES_COUNT);
		SetTile(((SCREEN_TILES_H/2U)-1U)+2,13,7-RAM_TILES_COUNT);
		SetTile(((SCREEN_TILES_H/2U)-1U)-1,14,31);
		SetTile(((SCREEN_TILES_H/2U)-1U),14,32);
		SetTile(((SCREEN_TILES_H/2U)-1U)+1,14,33);
		SetTile(((SCREEN_TILES_H/2U)-1U)+2,14,34);
		SetTile(((SCREEN_TILES_H/2U)-1U)+3,14,35);

	}

	FadeOut(1,true);
	SetRenderingParameters(FIRST_RENDER_LINE,FRAME_LINES);
	FadeIn(1,true);
	WaitVsync(30);
	FadeOut(1,true);
	ClearVram();
	FadeIn(1,true);
	WaitVsync(1);
}

void loadAppsConfig() { // parse the apps menu config file
	SetRenderingParameters(FIRST_RENDER_LINE,8);

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
	// TODO: add some basic protection to this,
	// don't allow app names longer than 10 characters,
	// filenames longer than 11 characters, etc.
	int nameIndex = 0;
	bool readingName = false;
	bool readingFilename = false;
	for (int i=0; i<512; i++) {
		if (vram[i+30] == '[') {
			readingName = true;
			continue;
		}
		if (vram[i+30] == ']') {
			readingName = false;
			//app[numberOfApps].name[nameIndex] = '\0';
			setAppName(numberOfApps,nameIndex,'\0');
			nameIndex = 0;
			continue;
		}
		if (vram[i+30] == '(') {
			readingFilename = true;
			continue;
		}
		if (vram[i+30] == ')') {
			readingFilename = false;
			//app[numberOfApps].filename[nameIndex] = '\0';
			setAppFileName(numberOfApps,nameIndex,'\0');
			nameIndex = 0;
			continue;
		}
		if (vram[i+30] == ';') {
			readingName = false;
			readingFilename = false;
			nameIndex = 0;
			numberOfApps++;
			continue;
		}
		if (readingName) {
			//app[numberOfApps].name[nameIndex] = vram[i+30];
			setAppName(numberOfApps,nameIndex,vram[i+30]);
			nameIndex++;
		}
		if (readingFilename) {
			//app[numberOfApps].filename[nameIndex] = vram[i+30];
			setAppFileName(numberOfApps,nameIndex,vram[i+30]);
			nameIndex++;
		}
	}
}

void loadApp(int app) {
	setFontColor(whitebg);
	Fill(1,0,28,1,3);
	SetTile(1,0,5);
	for (int x=3; x<13; x++) {
		if (getAppName(app,x-3) != 0) PrintChar(x,0,getAppName(app,x-3));
	}
	WaitVsync(20);

	SetRenderingParameters(FIRST_RENDER_LINE,8);

	t32 = FS_Find(&sd_struct, // look for file
	    ((u16)(getAppFileName(app,0)) << 8) |
	    ((u16)(getAppFileName(app,1))     ),
	    ((u16)(getAppFileName(app,2)) << 8) |
	    ((u16)(getAppFileName(app,3))     ),
	    ((u16)(getAppFileName(app,4)) << 8) |
	    ((u16)(getAppFileName(app,5))     ),
	    ((u16)(getAppFileName(app,6)) << 8) |
	    ((u16)(getAppFileName(app,7))     ),
	    ((u16)(getAppFileName(app,8)) << 8) |
	    ((u16)(getAppFileName(app,9))     ),
	    ((u16)(getAppFileName(app,10)) << 8) |
	    ((u16)(0)       ));

	if (t32 == 0U) { // file not found
		Fill(1,0,28,1,3);
		SetTile(1,0,5);
		Print(3,0,PSTR("No file!"));
		for (int x=12; x<22; x++) {
			if (getAppName(app,x-12) != 0) PrintChar(x,0,getAppFileName(app,x-12));
		}
		drawWallpaper();
		redrawAll();
		SetRenderingParameters(FIRST_RENDER_LINE,FRAME_LINES);
		WaitVsync(120);
		Fill(1,0,28,1,3);
	} else {
		FS_Select_Cluster(&sd_struct, t32);
		FS_Reset_Sector(&sd_struct);
		FS_Read_Sector(&sd_struct); // read from file
	}

	appSectors = vram[3+30];
	if (appSectors > 6 && t32 != 0U) { // don't allow files greater than 3KB
		Fill(1,0,28,1,3);
		SetTile(1,0,5);
		Print(3,0,PSTR("File too big!"));
		for (int x=17; x<27; x++) {
			if (getAppName(app,x-17) != 0) PrintChar(x,0,getAppFileName(app,x-17));
		}
		drawWallpaper();
		redrawAll();
		SetRenderingParameters(FIRST_RENDER_LINE,FRAME_LINES);
		WaitVsync(120);
		Fill(1,0,28,1,3);
	}

	if (appSectors <= 6 && t32 != 0U) {
		int newWindowNum = 1;
		int numberOfUsedSlots = 0;
		for (int i=10; i>0; i--) { // check for an empty window/VM slot, starting from the bottom
			if (!window[i].created) { // this is to figure out where the application data needs to be for the new VM
				newWindowNum = i;
			} else {
				numberOfUsedSlots++;
			}
		}

		for (int sector=0; sector<appSectors; sector++) {
			for (int i=0; i<512; i++) {
				SpiRamWriteU8(0,((3072*(newWindowNum-1))+i)+(sector*512),vram[i+30]); // read data from file into bank 0
			}
			FS_Next_Sector(&sd_struct);
			FS_Read_Sector(&sd_struct);
		}

		drawWallpaper();
		redrawAll();

		SetRenderingParameters(FIRST_RENDER_LINE,FRAME_LINES);

		createVM(newWindowNum,numberOfUsedSlots);
	}

	Fill(1,0,28,1,3);
	SetTile(1,0,5);
}

void vsyncCallback(void) {
	frame++;
	uptime = frame/60;
}

int main() {
	SetUserPreVsyncCallback(&vsyncCallback);
	initialize();
	loadAppsConfig();

	ClearVram();
	splash();

	initScreen();

	EnableSnesMouse(0,cursor_map);

	while(1) { // main loop
		updateController();
		updateCursor();
		updateClick();

		//updateInactiveTitlebars();
		updateActiveWindow();

		handleMenuClick();
		updateMenubar();

		updateButtonClicks();

		if (window[activeWindow].isVM && window[activeWindow].VMrunning) {
			//Print(1,20,PSTR("sp:"));
			//Print(1,21,PSTR("ip:"));
			for (int i=25; i>0; i--) { // execute 25 instructions
				embedvm_exec(&vm[activeWindow]);
				//PrintInt(8,20,vm[activeWindow].sp,false);
				//PrintInt(8,21,vm[activeWindow].ip,false);
			}
		}

		WaitVsync(1);
	}
}

void createVM(int newWindowNum, int numberOfUsedSlots) {
	if (numberOfUsedSlots < 10) { // only create a new window if there is an empty window slot
		vm[newWindowNum].ip = 0x0000; // this entry point should contain a jump to main()
		vm[newWindowNum].sp = vm[newWindowNum].sfp = 33024;
		vm[newWindowNum].mem_read = &mem_read;
		vm[newWindowNum].mem_write = &mem_write;
		vm[newWindowNum].call_user = &call_user;
		activeVM = newWindowNum;

		embedvm_interrupt(&vm[newWindowNum], 0x0000);

		window[newWindowNum].isVM = true;
		window[newWindowNum].VMrunning = true;

		for (int i=250; i>0; i--) { // execute 250 instructions, should be enough for the application to create a window
			embedvm_exec(&vm[newWindowNum]);
		}
	} else {
		Print(14,25,PSTR("No empty slots!")); // temp, only until i add a dialog box function
	}
}

// About window

void createAboutWindow() {
	createWindow(5,5,10,10,"About",5,false);
	clearWindow(activeWindow,0); // fill window with black tiles (tile 0)

	for (int x=3; x<8; x++) {
		for (int y=1; y<5; y++) {
			setWindowTile(x,y,activeWindow,23+((y-3)*5)+x); // draw Uzebox logo
		}
	}

	setFontColor(blackbg);
	printWindow(1,6,activeWindow,"UzeboxUI");
	printWindow(1,7,activeWindow,"by ry755");
	printWindow(1,8,activeWindow,"& luawtf");
}

// Tiles window (tile info)

void createTilesWindow() {
	createWindow(5,5,11,13,"Tile Info",9,false);

	setFontColor(whitebg);
	printWindow(1,1,activeWindow,"Total");
	printWindowInt(8,3,activeWindow,256-RAM_TILES_COUNT,false); // 256 is the max amount of tiles that can be used
	printWindow(5,3,activeWindow,"/");
	printWindowInt(4,3,activeWindow,TILESET_SIZE+SPRITESET_SIZE+FONT_SIZE+FONT_INVERT_SIZE,false); // sum of sizes of all tilesets

	printWindow(1,5,activeWindow,"RAM Tiles");
	printWindowInt(3,7,activeWindow,RAM_TILES_COUNT,false);

	printWindow(1,9,activeWindow,"Fonts");
	printWindowInt(4,11,activeWindow,FONT_SIZE+FONT_INVERT_SIZE,false);
}

// Settings window

void settingsChangeWallpaper(int num) {
	wallpaperTile += num;
	setWindowTile(3,3,activeWindow,wallpaperTile); // update the tile in the settings
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
	printWindow(1,1,activeWindow,"Wallpaper");

	setWindowTile(3,3,activeWindow,wallpaperTile);
	createButton(1,3,1,1,activeWindow,1,"<",settingsChangeWallpaper,-1);
	createButton(5,3,1,1,activeWindow,2,">",settingsChangeWallpaper,1);
	createButton(7,3,4,1,activeWindow,3,"Save",settingsSaveWallpaper,0);
}
