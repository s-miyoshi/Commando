class MyShipBeam{
public:
	int x, y, height, width, state;
	char* character;
	unsigned short attribute;
	void move();
	MyShipBeam(int x, int y, int width, int height);
};
