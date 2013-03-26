class Enemy{
public:
	int x, y, width, height, state, HP;
	bool shootable;
	char* character;
	unsigned short attribute;
	virtual void move()=0;
	Enemy(int x, int y, int width, int height, int hp);
	
protected:
	bool outOfField();
	int s, t, tick, delay, turn, limit;
};
