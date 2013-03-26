#include "Enemy3.h"
#include "main.h"
#include "WinAPI.h"

void Enemy3::move(){
	if(++tick<delay) return;
	tick=0;
	
	if(x==dstx&&y==dsty){
		if(!arrive){
			arrive=true;
			shootable=true;
		}
		else if(++turn>limit){
			arrive=false;
			s=(s+2)%4;
			shootable=false;
		}
	}
	if(!arrive){

		x+=v[t][0]*d[s][0];
		y+=v[t][1]*d[s][1];
	}
	if(HP<=0||outOfField()) state=DISAPPEAR;
}

Enemy3::Enemy3(int x, int y, int width, int height, int hp, int r, int t, int s,  int wait):Enemy(x-r*v[t][0]*d[s][0], y-r*v[t][1]*d[s][1], width, height, hp){
	dstx=x;
	dsty=y;
	this->s=s;
	this->t=t;
	shootable=false;
	arrive=false;
	character=(char *)"™[";
	attribute=CHAR_WHITE;
	limit=wait;
	delay=2;
}
