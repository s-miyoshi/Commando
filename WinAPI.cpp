#include <windows.h>
#include <cstring>
#include <cstdio>
#include "WinAPI.h"

SHORT WINDOW_HEIGHT;
SHORT WINDOW_WIDTH;

HANDLE ScreenBuf[2];
HWND hwnd;
bool DoubleBuf=false;
//���ݏ����ރo�b�t�@(�\������Ȃ����̕�)
int CurrentBuf=0;

void cls(){
	//��ʏ��� ��ʔ͈͑S�Ă𔼊p�󔒑����Ȃ��Ŗ��߂�
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

	//�X�N���[���o�b�t�@�쐬
	ScreenBuf[0]=CreateConsoleScreenBuffer(GENERIC_READ|GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	COORD coord={WINDOW_WIDTH, WINDOW_HEIGHT};
	SetConsoleScreenBufferSize(ScreenBuf[0], coord);

	//��ʃT�C�Y�ύX
	SMALL_RECT rect;
	rect.Left=0;
	rect.Top=0;
	rect.Right=WINDOW_WIDTH-1;
	rect.Bottom=WINDOW_HEIGHT-1;
	SetConsoleWindowInfo(ScreenBuf[0], TRUE, &rect);

	SetConsoleActiveScreenBuffer(ScreenBuf[0]);
	CurrentBuf=0;

	//�J�[�\���ړ�(�\������Ȃ����O�̈�)
	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.dwSize = 100;
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(ScreenBuf[0], &cursorInfo);

	//�_�u���o�b�t�@�����O�p�̂�����̃o�b�t�@
	if(DoubleBuf){
		ScreenBuf[1]=CreateConsoleScreenBuffer(GENERIC_READ|GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		SetConsoleScreenBufferSize(ScreenBuf[1], coord);
		SetConsoleWindowInfo(ScreenBuf[1], TRUE, &rect);
		SetConsoleCursorInfo(ScreenBuf[1], &cursorInfo);
		SetConsoleActiveScreenBuffer(ScreenBuf[1]);
	}

	//�R���\�[���̃E�C���h�E�n���h���擾(IsActive�Ŏg��)
	SetConsoleTitle(title);
	hwnd=FindWindow(NULL, title);
}

void terminateWindow(){
	//�X�N���[���o�b�t�@���
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
	//�o�b�t�@�ؑ�
	if(!DoubleBuf)return;
	SetConsoleActiveScreenBuffer(ScreenBuf[CurrentBuf]);
	CurrentBuf=CurrentBuf==1?0:1;
}

bool isActive(){
	//�őO�̃E�C���h�E(�A�N�e�B�u)�͂��̃E�C���h�E��?
	return GetForegroundWindow()==hwnd;
}
