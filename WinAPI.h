#include <windows.h>

void cls();
void write1Byte(int x, int y, char ch, unsigned short attribute);
void write2Byte(int x, int y, const char* ch, unsigned short attribute);
void writeText(int x, int y, const char* text, unsigned short attribute);
void fillRectangle(int x, int y, int width, int height, unsigned short attribute);
bool getKey(int key);
void initializeWindow(int x, int y, int width, int height, char *title, bool doublebuf=false);
void terminateWindow();
unsigned long getTime();
void switchBuffer();
bool isActive();

//ëÆê´ÉtÉâÉO
const unsigned short BACK_BLUE=BACKGROUND_BLUE;
const unsigned short CHAR_BLUE=FOREGROUND_BLUE;
const unsigned short CHAR_GREEN=FOREGROUND_GREEN;
const unsigned short CHAR_RED=FOREGROUND_RED;
const unsigned short CHAR_YELLOW=FOREGROUND_GREEN|FOREGROUND_RED;
const unsigned short CHAR_CYAN=FOREGROUND_GREEN|FOREGROUND_BLUE;
const unsigned short CHAR_MAGENTA=FOREGROUND_RED|FOREGROUND_BLUE;
const unsigned short CHAR_WHITE=FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE;
