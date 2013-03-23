#include "WinAPI.h"
#include "main.h"
#include "EnemyBeam.h"

void EnemyBeam::move(){
	x+=v[s][0]*d[t][0];
	y+=v[s][1]*d[t][1];
	if(y>FIELD_HEIGHT)state=DISAPPEAR;
}

EnemyBeam::EnemyBeam(int x,int y,int height,int width,int s,int t){
	this->x=x;
	this->y=y;
	this->width=width;
	this->height=height;
	this->s=s;
	this->t=t;
	state=NORMAL;
	character=(char *)"Åﬁ";
	attribute=CHAR_WHITE;
}
