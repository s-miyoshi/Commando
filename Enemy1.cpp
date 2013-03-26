#include "Enemy1.h"
#include "main.h"
#include "WinAPI.h"

void Enemy1::move(){
	if(++tick<delay) return;
	tick=0;
	
	if(++count>=interval){
		s=(s+(ccw?3:1))%4;
		count=0;
		++turn;
	}

	x+=v[t][0]*d[s][0];
	y+=v[t][1]*d[s][1];
	if(HP<=0)state=DISAPPEAR;
}

Enemy1::Enemy1(int x, int y, int width, int height, int hp,int r, int s, bool ccw):Enemy(x + ( ( (s==3&&!ccw) || (s==0&&ccw) ) ? -2*r : ( ( (s==1&&!ccw) || (s==2&&ccw) ) ? 2*r : 0 ) ), y + ( ( (s==0&&!ccw) || (s==1&&ccw) ) ? -r : ( ( (s==2&&!ccw) || (s==3&&ccw) ) ? r : 0 ) ), width, height, hp){
	character=(char *)"êž";
	attribute=(s==0?CHAR_RED:s==1?CHAR_GREEN:s==2?CHAR_YELLOW:CHAR_WHITE);
	this->s=s;
	t=4;
	this->ccw=ccw;
	count=-1;
	interval=r;
	delay=10;
	shootable=true;
	limit=rand()%10+20;
}
