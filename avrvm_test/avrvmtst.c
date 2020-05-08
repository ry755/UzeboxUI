#include <stdint.h>
#include <string.h>
#include <avr/pgmspace.h>

#define HOSTAPI_BASE 0x8000
#define IO_BASE 32
// I/O registers
/*#define IO_0 *((volatile char *) (IO_BASE + 0))
#define IO_1 *((volatile char *) (IO_BASE + 1))
#define IO_2 *((volatile char *) (IO_BASE + 2))
#define IO_3 *((volatile char *) (IO_BASE + 3))
#define IO_4 *((volatile char *) (IO_BASE + 4))
#define IO_5 *((volatile char *) (IO_BASE + 5))*/
// Host functions
void (* const hostapi_createWindow)(int locationX, int locationY, int sizeX, int sizeY, int titleSize) = (void *)(HOSTAPI_BASE + 0);
void (* const hostapi_printWindowLen)(int x, int y, int windowNumber, int textSize) = (void *)(HOSTAPI_BASE + 1);
//void (* const hostapi_sleep)(uint32_t ms) = (void *)(HOSTAPI_BASE + 1);
uint8_t (* const hostapi_getActiveWindow)(void) = (void *)(HOSTAPI_BASE + 2);

static void writeCharIO(int ioAddr, char val) {
    volatile char *io = (volatile char *)IO_BASE+ioAddr;
	*io = val;
}

static void createWindow(int locationX, int locationY, int sizeX, int sizeY, char *title, int titleSize) {
    /*for (int i=0; i<titleSize; i++) { // write text to IO registers
		writeCharIO(i,title[i]);
	}*/
    writeCharIO(0,title[0]);
    writeCharIO(1,title[1]);
    writeCharIO(2,title[2]);
    writeCharIO(3,title[3]);
    writeCharIO(4,title[4]);
    writeCharIO(5,title[5]);
    writeCharIO(6,title[6]);
    hostapi_createWindow(locationX,locationY,sizeX,sizeY,titleSize);
}

static void printWindow(int x, int y, int windowNumber, char *text, int textSize) {
    /*for (int i=0; i<textSize; i++) { // write text to IO registers
        writeCharIO(i,text[i]);
	}*/
    writeCharIO(0,text[0]);
    writeCharIO(1,text[1]);
    writeCharIO(2,text[2]);
    writeCharIO(3,text[3]);
    writeCharIO(4,text[4]);
    writeCharIO(5,text[5]);
    writeCharIO(6,text[6]);
    writeCharIO(7,text[7]);
    hostapi_printWindowLen(x,y,windowNumber,textSize);
}

void main(void) {
    createWindow(7,21,20,3,"avrvm!",6);
    printWindow(1,1,hostapi_getActiveWindow(),"hello :P",8);
    asm("break");
}
