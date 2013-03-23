class Enemy{
public:
	int x, y, width, height, state, HP;
	bool fireable;
	char* character;
	unsigned short attribute;
	virtual void move()=0;
	bool outOfField();
	Enemy(int x, int y, int width, int height, int hp);
	
protected:
	int s, t, tick, delay;
};
