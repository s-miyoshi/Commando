#include "MyShipBeam.h"
#include "main.h"
#include "WinAPI.h"
#include <cstring>

void MyShipBeam::move(){
	if(state!=NORMAL)return;
	--y;
	if(y<0)state=DISAPPEAR;
}

MyShipBeam::MyShipBeam(int x, int y, int height, int width){
	this->x=x;
	this->y=y;
	this->height=height;
	this->width=width;
	state=NORMAL;
	character=(char *)"ƒ¢";
	attribute=CHAR_WHITE;
}
