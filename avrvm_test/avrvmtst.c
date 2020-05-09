#include <avr/pgmspace.h>


// I/O registers
/*
#define IO_0 *((volatile char *) (IO_BASE + 0))
#define IO_1 *((volatile char *) (IO_BASE + 1))
#define IO_2 *((volatile char *) (IO_BASE + 2))
#define IO_3 *((volatile char *) (IO_BASE + 3))
#define IO_4 *((volatile char *) (IO_BASE + 4))
#define IO_5 *((volatile char *) (IO_BASE + 5))
*/

// Memory offsets
#define HOSTAPI_BASE 0x8000
#define IO_BASE 32

// Host functions
void (* const hostapi_createWindow)(int locationX, int locationY, int sizeX, int sizeY, int titleSize) = (void *)(HOSTAPI_BASE + 0);
void (* const hostapi_printWindowLen)(int x, int y, int windowNumber, int textSize) = (void *)(HOSTAPI_BASE + 1);
// void (* const hostapi_sleep)(uint32_t ms) = (void *)(HOSTAPI_BASE + 1);
uint8_t (* const hostapi_getActiveWindow)(void) = (void *)(HOSTAPI_BASE + 2);

static const char * const strs[] = {
    "avrvm!",
    "hello :P"
};

void main(void) {
    register unsigned char i;

    volatile char *ptr;
    volatile char c;

    i = 6; do {
        // *((char *) IO_BASE + i) = strs[0][i];

        // r = strs[0][i]
        ptr = (char *) strs[0]; c = ptr[i];

        // y = (char *) IO_BASE + i
        ptr = (char *) IO_BASE + i;

        // *y = r
        asm volatile("st %a0, %1" : "=e" (ptr), "=r" (c) :);

    } while (i != 0);

    hostapi_createWindow(7,21, 20,3, 6);

    i = 8; do {
        // *((char *) IO_BASE + i) = strs[1][i];

        // r = strs[1][i]
        ptr = (char *) strs[1]; c = ptr[i];

        // y = (char *) IO_BASE + i
        ptr = (char *) IO_BASE + i;

        // *y = r
        asm volatile("st %a0, %1" : "=e" (ptr), "=r" (c) :);

    } while (i != 0);

    hostapi_printWindowLen(1,1, hostapi_getActiveWindow(), 8);

    asm volatile("break");
}
