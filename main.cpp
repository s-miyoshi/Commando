#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <fstream>
#include <list>

#define MAIN
#define DEBUG

#include "main.h"
#include "WinAPI.h"
#include "MyShip.h"
#include "MyShipBeam.h"
#include "Enemy.h"
#include "Enemy1.h"
#include "Enemy2.h"
#include "Enemy3.h"
#include "EnemyBeam.h"

const int CHARACTER_WIDTH=2, CHARACTER_HEIGHT=1;
const char* TITLE="COMMANDO - A game on Command Prompt";

#ifdef DEBUG
std::ofstream fout;
#endif

void start(MyShip** myship, int& score){
	//ゲーム開始前にやる初期化
	const int xs=(FIELD_WIDTH-CHARACTER_WIDTH)/2, ys=22;
	*myship=new MyShip(xs, ys, CHARACTER_WIDTH, CHARACTER_HEIGHT, 3, true);
	score=0;
}

void end(MyShip** myship, std::list<MyShipBeam*>& myshipBeam, std::list<Enemy*>& enemy, std::list<EnemyBeam*>& enemyBeam){
	//ゲーム終了時にやる後始末
	delete *myship;
	myship=NULL;
	
	for(std::list<MyShipBeam*>::iterator i=myshipBeam.begin();i!=myshipBeam.end();++i) delete *i;
	myshipBeam.clear();
	for(std::list<Enemy*>::iterator i=enemy.begin();i!=enemy.end();++i) delete *i;
	enemy.clear();
	for(std::list<EnemyBeam*>::iterator i=enemyBeam.begin();i!=enemyBeam.end();++i) delete *i;
	enemyBeam.clear();
}

void makeMyShipBeam(const MyShip* myship, std::list<MyShipBeam*>& myshipBeam){
	myshipBeam.push_back(new MyShipBeam(myship->x, myship->y-1, CHARACTER_WIDTH, CHARACTER_HEIGHT));
}


int makeEnemy(std::list<Enemy*>& enemy){
	int pattern=(rand()%2)*2;
	switch(pattern){
	
	//左右一杯往復 只
	case 0:
		{
			int n=rand()%9+8;
			const int MAX_Y=18;
			bool* used=new bool[MAX_Y];

			for(int i=0;i<MAX_Y;++i) *(used+i)=false;

			for(int i=0;i<n;++i){
				int x=rand()%FIELD_WIDTH-CHARACTER_WIDTH;
				int y=rand()%MAX_Y;
				for(;used[y];y=rand()%MAX_Y) fout<<' '<<y;
				used[y]=true;
				enemy.push_back(new Enemy2(x, y, CHARACTER_WIDTH, CHARACTER_HEIGHT, 1));
			}
			delete used;
			return n;
			break;
		}

	//四近傍円(菱形) 龠
	case 1:
		{
			int total=0;
			int n=rand()%3+3;
			int used[n][3];
			for(int i=0;i<n;++i) for(int j=0;j<3;++j) used[i][j]=(j==2?0:10000);
			
			for(int i=0;i<n;++i){
				int r, x, y;
				bool improper=false;
/*				do{
					r=rand()%3+2;
					x=(rand()%(FIELD_WIDTH/2-2*r)+r)*2;
					y=rand()%(16-2*r)+r;
					fout<<x<<" "<<y<<" "<<r<<std::endl;
					for(int j=0;j<i;++j){
						if(abs(x-used[j][0])+abs(y-used[j][1])<=r+used[j][2]) improper=true;
					}
				}while(improper);
				used[i][0]=x; used[i][1]=y;
*/				bool ccw=(rand()%2==1);
				int k=4;

				for(int j=0;j<k;++j){
					int s=( j==1 ? 2 : ( j==2 ? 1 : j ));
					enemy.push_back(new Enemy1(x, y, CHARACTER_WIDTH, CHARACTER_HEIGHT, 1, r, s, ccw));
				}
				total+=k;
			}
			return total;
			break;
		}
	//集合->離散 "兌"
	case 2:
		{
			int m=rand()%3+1;
			int total=0;
			const int MAX_Y=18, MIN_Y=5;
			bool* used=new bool[MAX_Y];
			for(int i=0;i<m;++i){
				int n=0;
				switch(m){
				case 3:
					n=rand()%4+4;
					break;
				case 2:
					n=rand()%5+6;
					break;
				case 1:
					n=rand()%7+10;
					break;
				}

				int x=rand()%(FIELD_WIDTH-n*CHARACTER_WIDTH);
				int y=rand()%(MAX_Y-MIN_Y)+MIN_Y;
				for(;used[y];y=rand()%(MAX_Y-MIN_Y)+MIN_Y);
				used[y]=true;
				int r=20+rand()%10;
				int wait=200+(rand()%5)*40;
				for(int j=0;j<n;++j){
					enemy.push_back(new Enemy3(x+j*CHARACTER_WIDTH, y, CHARACTER_WIDTH, CHARACTER_HEIGHT, 1, r, rand()%3+2, rand()%2, wait));
				}
				total+=n;
			}
		delete used;
		return total;
		break;
		}
	}
}

void makeEnemyBeam(const Enemy* enemy, std::list<EnemyBeam*>& enemyBeam){
	enemyBeam.push_back(new EnemyBeam(enemy->x, enemy->y+1, CHARACTER_WIDTH, CHARACTER_HEIGHT, 2, 0));
}

bool checkHit(int x1, int y1, int w1, int h1, int x2, int y2, int h2, int w2){
	return x1<=x2+w2-1&&x1+w1-1>=x2&&y1<=y2+h2-1&&y1+h1-1>=y2;
}

template<typename T1, typename T2> bool checkHit_Chars(T1 e, std::list<T2>& bs){
	for(typename std::list<T2>::iterator b=bs.begin();b!=bs.end();++b){
		if(checkHit(e->x, e->y, e->width, e->height, (*b)->x, (*b)->y, (*b)->width, (*b)->height)){
			(*b)->state=DISAPPEAR;
			return true;
		}
	}
	return false;
}

template<typename T> void drawCharacters(std::list<T> character){
	for(typename std::list<T>::iterator i=character.begin();i!=character.end();++i){
		if((*i)->x<0||(*i)->x>=FIELD_WIDTH||(*i)->y<0||(*i)->y>=FIELD_HEIGHT) continue;
		write2Byte((*i)->x, (*i)->y, (*i)->character, (*i)->attribute);
	}
}

void drawField(const MyShip* myship, const std::list<MyShipBeam*>& myshipBeam, const std::list<Enemy*>& enemy, const std::list<EnemyBeam*>& enemyBeam){
	write2Byte(myship->x, myship->y, myship->character, myship->attribute);
	drawCharacters<MyShipBeam*>(myshipBeam);
	drawCharacters<Enemy*>(enemy);
	drawCharacters<EnemyBeam*>(enemyBeam);
}

void drawMenu(const MyShip* myship, int score, bool playing){
	fillRectangle(FIELD_WIDTH, 0, SCREEN_WIDTH-FIELD_WIDTH, SCREEN_HEIGHT, BACK_BLUE);
	char str[32];
	sprintf(str, "Score:   %08d", score);
	writeText(53, 4, str, CHAR_WHITE|BACK_BLUE);
	
	if(playing){
		if(myship!=NULL) for(int i=0;i<myship->HP;++i) write2Byte(53+4*i, 14, myship->character, CHAR_WHITE|BACK_BLUE);
		writeText(53, 17, "Press", CHAR_YELLOW|BACK_BLUE);
		writeText(53, 18, "ESC KEY", CHAR_YELLOW|BACK_BLUE);
		writeText(53, 19, "to Quit.", CHAR_YELLOW|BACK_BLUE);
	}
}

void drawGameOver(){
	static char* Pattern_Gameover[]=
	{(char *)" ####  ###  #   # #####   ###  #   # ##### #### ", 
	 (char *)"#     #   # ## ## #      #   # #   # #     #   #", 
	 (char *)"#  ## #   # # # # ####   #   # #   # ####  #### ", 
	 (char *)"#   # ##### # # # #      #   # #   # #     # #  ", 
	 (char *)"#   # #   # #   # #      #   #  # #  #     #  # ", 
	 (char *)" #### #   # #   # #####   ###    #   ##### #   #"};
	int x=1, y=8;
	for(int i=0;i<6;++i){
		writeText(x, y+i, Pattern_Gameover[i], CHAR_RED);
	}
	writeText(10, 20, "Please Press SPACE KEY to Start.", CHAR_YELLOW);
	writeText(10, 21, "Please Press DELETE KEY to Quit.", CHAR_YELLOW);
}

bool isGameOver(MyShip* myship, std::list<Enemy*> enemy){
	return myship->HP<=0;
}

int main(){
#ifdef DEBUG
	fout.open("log.txt", std::ios::out);
#endif

	initializeWindow(100, 200, SCREEN_WIDTH, SCREEN_HEIGHT, (char *)TITLE, true);

	MyShip* myship=NULL;
	std::list<MyShipBeam*> myshipBeam;
	std::list<Enemy*> enemy;
	std::list<EnemyBeam*> enemyBeam;
	
	int score=0;
	int touch=0;
	const int interval=10;
	bool playing=false;
	int remainEnemy=0;
	srand(time(NULL));
	
	while(true){
		//アクティブウインドウでないなら何もしない
		if(!isActive())continue;

		//同期Wait用のループ開始時刻取得
		unsigned long stime=getTime();

		//画面消去
		cls();

		if(playing){
		//ゲーム中
			if(getKey(VK_ESCAPE)||isGameOver(myship, enemy)){
				fout<<"end"<<std::endl;
				end(&myship, myshipBeam, enemy, enemyBeam);
				playing=false;
				remainEnemy=0;
				continue;
			}

			//自機関連処理
			myship->move();
			if(myship->state==NORMAL && (checkHit_Chars<MyShip*, EnemyBeam*>(myship, enemyBeam)|(checkHit_Chars<MyShip*, Enemy*>(myship, enemy)))){
				myship->state=DAMAGE;
				--myship->HP;
			}

			if(myship->state==NORMAL){
				if(++touch>interval&&getKey('A')){
					makeMyShipBeam(myship, myshipBeam);
					touch=0;
				}
			}

			//自機弾関連処理
			for(std::list<MyShipBeam*>::iterator i=myshipBeam.begin();i!=myshipBeam.end();){
				if((*i)->state==NORMAL){
					(*i)->move();
					++i;
				}
				else{
					delete *i;
					i=myshipBeam.erase(i);
				}
			}
			
			//敵関連処理
			if(remainEnemy<=0){
				remainEnemy=makeEnemy(enemy);
			}
			
			for(std::list<Enemy*>::iterator i=enemy.begin();i!=enemy.end();){
				if((*i)->state==NORMAL){
					(*i)->move();
					if((*i)->shootable&&rand()%(40*remainEnemy)<1)makeEnemyBeam(*i, enemyBeam);
					
					if(checkHit_Chars<Enemy*, MyShipBeam*>(*i, myshipBeam)){
						(*i)->state=DISAPPEAR;
						++score;
					}
					++i;
				}
				else{
					delete *i;
					i=enemy.erase(i);
					--remainEnemy;
				}
			}

			//敵弾関連処理
			for(std::list<EnemyBeam*>::iterator i=enemyBeam.begin();i!=enemyBeam.end();){
				if((*i)->state==NORMAL){
					(*i)->move();
					++i;
				}
				else{
					delete *i;
					i=enemyBeam.erase(i);
				}
			}

			//フィールド描画
			drawField(myship, myshipBeam, enemy, enemyBeam);
		}
		else{
		//ゲームオーバー中
			if(getKey(VK_SPACE)){
				playing=true;
				start(&myship, score);
			}
			else if(getKey(VK_DELETE)){
				break;
			}
			drawGameOver();
		}

		//メニュー描画
		drawMenu(myship, score, playing);

		//スクリーンバッファ切替(描画反映)
		switchBuffer();

		//同期Wait
		while(getTime()-stime<30);
	}

	terminateWindow();
#ifdef DEBUG
	fout.close();
#endif
	return 0;
}
