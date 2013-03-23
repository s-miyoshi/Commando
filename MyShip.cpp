#include "MyShip.h"
#include "main.h"
#include "WinAPI.h"

void MyShip::move(){
	if(getKey('L')) x=x<FIELD_WIDTH-2?x+1:FIELD_WIDTH-2;
	if(getKey('H')) x=x>0?x-1:0;
	if(vert){
		if(getKey('K')) y=y>0?y-1:0;
		if(getKey('J')) y=y<FIELD_HEIGHT-1?y+1:FIELD_HEIGHT-1;
	}

	if(state==DAMAGE){
		++damageCount;
		switch(damageState){
		case UNMOVABLE:
			attribute=0;
			x=xs;
			y=ys;
			if(damageCount>30){
				damageCount=0;
				damageState=BLINK;
			}
			break;
		case BLINK:
			if(damageCount%3==0){
				attribute=attribute==0?CHAR_WHITE:0;
			}
			if(damageCount>30){
				state=NORMAL;
				damageCount=0;
				attribute=CHAR_WHITE;
				damageState=UNMOVABLE;
			}
			break;
		}
	}
}

MyShip::MyShip(int x, int y, int width, int height, int HP, bool vertical){
	this->x=x;
	this->y=y;
	this->xs=x;
	this->ys=y;
	this->width=width;
	this->height=height;
	this->HP=HP;
	this->state=NORMAL;
	this->vert=vertical;
	character=(char *)"“Ê";
	attribute=CHAR_WHITE;
	damageCount=0;
	damageState=UNMOVABLE;
}
