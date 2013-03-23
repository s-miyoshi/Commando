#include <windows.h>
#include <cstring>
#include <cstdio>
#include "WinAPI.h"

SHORT WINDOW_HEIGHT;
SHORT WINDOW_WIDTH;

HANDLE ScreenBuf[2];
HWND hwnd;
bool DoubleBuf=false;
//現在書込むバッファ(表示されない裏の方)
int CurrentBuf=0;

void cls(){
	//画面消去 画面範囲全てを半角空白属性なしで埋める
	COORD coordzero={0, 0};
	DWORD written;
	FillConsoleOutputCharacter(ScreenBuf[CurrentBuf], ' ', WINDOW_HEIGHT*WINDOW_WIDTH, coordzero, &written);
	FillConsoleOutputAttribute(ScreenBuf[CurrentBuf], 0, WINDOW_HEIGHT*WINDOW_WIDTH, coordzero, &written);
	SetConsoleCursorPosition(ScreenBuf[CurrentBuf], coordzero);
}

void write1Byte(int x, int y, char ch, unsigned short attribute){
	COORD coord={x, y};
	DWORD written;
	FillConsoleOutputCharacter(ScreenBuf[CurrentBuf], ch, 1, coord, &written);
	FillConsoleOutputAttribute(ScreenBuf[CurrentBuf], attribute, 1, coord, &written);
}

void write2Byte(int x, int y, const char* ch, unsigned short attribute){
	COORD coord={x, y};
	DWORD written;
	WORD attributes[2]={attribute, attribute};
	WriteConsoleOutputCharacter(ScreenBuf[CurrentBuf], ch, 2, coord, &written);
	WriteConsoleOutputAttribute(ScreenBuf[CurrentBuf], attributes, 2, coord, &written);
}

void writeText(int x, int y, const char* text,  unsigned short attribute){
	COORD coord={x, y};
	DWORD written;
	for(int i=0;i<strlen(text);++i){
		FillConsoleOutputCharacter(ScreenBuf[CurrentBuf], *(text+i), 1, coord, &written);
		FillConsoleOutputAttribute(ScreenBuf[CurrentBuf], attribute, 1, coord, &written);
		++coord.X;
	}
}

void fillRectangle(int x, int y, int width, int height,  unsigned short attribute){
	DWORD written;
	for(int r=y;r<y+height;++r){
		COORD coord={x, r};
		FillConsoleOutputAttribute(ScreenBuf[CurrentBuf], attribute, width, coord, &written);
	}
}

void initializeWindow(int x, int y, int width, int height, char *title, bool doublebuf){
	DoubleBuf=doublebuf;

	WINDOW_HEIGHT=height;
	WINDOW_WIDTH=width;

	//スクリーンバッファ作成
	ScreenBuf[0]=CreateConsoleScreenBuffer(GENERIC_READ|GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	COORD coord={WINDOW_WIDTH, WINDOW_HEIGHT};
	SetConsoleScreenBufferSize(ScreenBuf[0], coord);

	//画面サイズ変更
	SMALL_RECT rect;
	rect.Left=0;
	rect.Top=0;
	rect.Right=WINDOW_WIDTH-1;
	rect.Bottom=WINDOW_HEIGHT-1;
	SetConsoleWindowInfo(ScreenBuf[0], TRUE, &rect);

	SetConsoleActiveScreenBuffer(ScreenBuf[0]);
	CurrentBuf=0;

	//カーソル移動(表示されないが念の為)
	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.dwSize = 100;
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(ScreenBuf[0], &cursorInfo);

	//ダブルバッファリング用のもう一つのバッファ
	if(DoubleBuf){
		ScreenBuf[1]=CreateConsoleScreenBuffer(GENERIC_READ|GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		SetConsoleScreenBufferSize(ScreenBuf[1], coord);
		SetConsoleWindowInfo(ScreenBuf[1], TRUE, &rect);
		SetConsoleCursorInfo(ScreenBuf[1], &cursorInfo);
		SetConsoleActiveScreenBuffer(ScreenBuf[1]);
	}

	//コンソールのウインドウハンドル取得(IsActiveで使う)
	SetConsoleTitle(title);
	hwnd=FindWindow(NULL, title);
}

void terminateWindow(){
	//スクリーンバッファ解放
	CloseHandle(ScreenBuf[0]);
	if(DoubleBuf)CloseHandle(ScreenBuf[1]);
}

bool getKey(int key){
	return GetAsyncKeyState(key)<0;
}

unsigned long getTime(){
	return GetTickCount();
}

void switchBuffer(){
	//バッファ切替
	if(!DoubleBuf)return;
	SetConsoleActiveScreenBuffer(ScreenBuf[CurrentBuf]);
	CurrentBuf=CurrentBuf==1?0:1;
}

bool isActive(){
	//最前のウインドウ(アクティブ)はこのウインドウか?
	return GetForegroundWindow()==hwnd;
}
