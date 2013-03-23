class MyShip{
public:
	void move();
	int x, y, xs, ys;
	int height, width;
	int HP;
	int state;
	char* character;
	unsigned short attribute;
	MyShip(int x, int y, int width, int height, int HP, bool vertical);
private:
	bool vert;
	int damageState;
	int damageCount;

	static const int UNMOVABLE=0, BLINK=1;
};
