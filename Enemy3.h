#ifndef MAIN
	#include "Enemy.h"
#endif

class Enemy3: Enemy{
public:
	void move();
	Enemy3(int x, int y, int width, int height, int hp, int r, int t, int s, int wait);
private:
	int dstx, dsty, wait, count;
	bool arrive;
};

