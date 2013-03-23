class EnemyBeam{
public:
	int x,y,width,height,state;
	char* character;
	unsigned short attribute;
	void move();
	EnemyBeam(int x,int y,int width,int height,int s,int t);
private:
	int s,t;
};
