#include "Enemy2.h"
#include "main.h"
#include "WinAPI.h"

void Enemy2::move(){
	if(++tick<delay) return;
	tick=0;
	
	if(x>=FIELD_WIDTH-1){s=1;}
	else if(x<=0){s=0;}
	x+=v[t][0]*d[s][0];
	y+=v[t][1]*d[s][1];
	if(HP<=0||outOfField()) state=DISAPPEAR;
}

Enemy2::Enemy2(int x,int y,int width,int height,int hp):Enemy(x, y, width, height, hp){
	character=(char *)"‘ü";
	attribute=CHAR_WHITE;
	s=0;
	t=0;
	tick=0;
	delay=2;
	fireable=true;
}
