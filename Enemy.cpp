#include "Enemy.h"
#include "main.h"

Enemy::Enemy(int x, int y, int width, int height, int hp){
	this->x=x;
	this->y=y;
	this->width=width;
	this->height=height;
	this->HP=hp;
	state=NORMAL;
}

bool Enemy::outOfField(){
	return (x<0&&d[s][0]<0) || (x>=FIELD_WIDTH&&d[s][0]>0) || (y<0&&d[s][1]<0) || (y>=FIELD_HEIGHT&&d[s][1]>0);
}
