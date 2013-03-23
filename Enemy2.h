#ifndef MAIN
	#include "Enemy.h"
#endif

class Enemy2 : public Enemy{
public:
	void move();
	Enemy2(int x, int y, int width, int height, int hp);
};
