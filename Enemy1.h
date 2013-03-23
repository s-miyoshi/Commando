#ifndef MAIN
	#include "Enemy.h"
#endif

class Enemy1 : public Enemy{
public:
	void move();
	Enemy1(int x, int y, int width, int height, int hp, int r, int s, bool ccw);
private:
	bool ccw;
	int interval, count;
};
