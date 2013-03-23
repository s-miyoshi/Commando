#include "Enemy3.h"
#include "main.h"
#include "WinAPI.h"

void Enemy3::move(){
	if(x==dstx&&y==dsty){
		if(!arrive){
			arrive=true;
			fireable=true;
		}
		else if(++count>wait){
			arrive=false;
			s=(s+2)%4;
			fireable=false;
		}
	}
	if(!arrive){

		x+=v[t][0]*d[s][0];
		y+=v[t][1]*d[s][1];
	}
	if(HP<=0||outOfField()) state=DISAPPEAR;
}

Enemy3::Enemy3(int x, int y, int width, int height, int hp, int r, int t, int s,  int wait):Enemy(x-r*v[t][0]*d[s][0], x-r*v[t][1]*d[s][1], width, height, hp){
	this->wait=wait;
	dstx=x;
	dsty=y;
	this->s=s;
	this->t=t;
	count=0;
	fireable=false;
	arrive=false;
}
