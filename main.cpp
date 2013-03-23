#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <fstream>

#define MAIN
#define DEBUG

#include "main.h"
#include "WinAPI.h"
#include "MyShip.h"
#include "MyShipBeam.h"
#include "Enemy.h"
#include "Enemy1.h"
#include "Enemy2.h"
#include "EnemyBeam.h"

const int CHARACTER_WIDTH=2, CHARACTER_HEIGHT=1;
const char* TITLE="COMMANDO - A game on Command Prompt";

#ifdef DEBUG
std::ofstream fout;
#endif

void start(MyShip** myship, int& score){
	//ゲーム開始前にやる初期化
	const int xs=30, ys=22;
	*myship=new MyShip(xs, ys, CHARACTER_WIDTH, CHARACTER_HEIGHT, 3, true);
	score=0;
}

void end(MyShip** myship, MyShipBeam** myshipBeam, Enemy** enemy, EnemyBeam** enemyBeam){
	//ゲーム終了時にやる後始末
	delete *myship;
	myship=NULL;
	for(int i=0;i<MAX_BEAM;++i){
		delete *(myshipBeam+i);
		*(myshipBeam+i)=NULL;
	}
	for(int i=0;i<MAX_ENEMY;++i){
		delete *(enemy+i);
		*(enemy+i)=NULL;
	}
	for(int i=0;i<MAX_ENEBEAM;++i){
		delete *(enemyBeam+i);
		*(enemyBeam+i)=NULL;
	}
}

void makeMyShipBeam(const MyShip* myship, MyShipBeam** myshipBeam){
	for(int i=0;i<MAX_BEAM;++i){
		if(*(myshipBeam+i)==NULL){
			*(myshipBeam+i)=new MyShipBeam(myship->x, myship->y-1, CHARACTER_WIDTH, CHARACTER_HEIGHT);
			break;
		}
	}
}

bool add(Enemy** array, Enemy* e, int max){
	for(int i=0;i<max;++i){
		if(*(array+i)==NULL){
			*(array+i)=e;
			return true;
		}
	}
	return false;
}

int makeEnemy(Enemy** enemy){
	const int MIN_ENEMY=3;
	int n=rand()%(10-MIN_ENEMY+1)+MIN_ENEMY;
	
	int pattern=rand()%2;

	for(int k=0;k<n;++k){
		for(int i=0;i<MAX_ENEMY;++i){
			if(*(enemy+i)==NULL){
				int x, y, r;
				bool ccw;

				switch(pattern){
				//菱形回転 "龠" をランダム配置
				case 0:
					r=rand()%2+2;
					ccw=(rand()%2==1);
					
					x=(rand()%(FIELD_WIDTH/2-2*r)+r)*2;
					y=rand()%(16-2*r)+r;
					
					*(enemy+i)=new Enemy1(x, y, CHARACTER_WIDTH, CHARACTER_HEIGHT, 1, r, 0, ccw);
					break;
				//往復移動 "只" をランダム配置
				case 1:
					x=rand()%FIELD_WIDTH,y=rand()%15;
					*(enemy+i)=new Enemy2(x, y, CHARACTER_WIDTH, CHARACTER_HEIGHT, 1);
					break;
				}
				break;
			}
			
		}
	}
	return n;
}

void makeEnemyBeam(const Enemy* enemy, EnemyBeam** enemyBeam){
	for(int i=0;i<MAX_ENEBEAM;++i){
		if(*(enemyBeam+i)==NULL){
			*(enemyBeam+i)=new EnemyBeam(enemy->x, enemy->y+1, CHARACTER_WIDTH, CHARACTER_HEIGHT, 2, 0);
			break;
		}
	}
}

bool checkHit(int x1, int y1, int w1, int h1, int x2, int y2, int h2, int w2){
	return x1<=x2+w2-1&&x1+w1-1>=x2&&y1<=y2+h2-1&&y1+h1-1>=y2;
}

int checkHitEnemy(MyShipBeam** mybeam, Enemy* enemy){
	int s=0;
	for(int i=0;i<MAX_BEAM;++i){
		if(*(mybeam+i)==NULL) continue;
		if(checkHit(enemy->x, enemy->y, enemy->width, enemy->height, (*(mybeam+i))->x, (*(mybeam+i))->y, (*(mybeam+i))->width, (*(mybeam+i))->height)){
			delete *(mybeam+i);
			*(mybeam+i)=NULL;
			if(--(enemy->HP)<=0)++s;
		}
	}
	return s;
}

bool checkHitMyShip_EnemyBeam(MyShip* myship, EnemyBeam** enemyBeam){
	for(int i=0;i<MAX_ENEBEAM;++i){
		if(*(enemyBeam+i)==NULL) continue;
		if(checkHit(myship->x, myship->y, myship->width, myship->height, (*(enemyBeam+i))->x, (*(enemyBeam+i))->y, (*(enemyBeam+i))->width, (*(enemyBeam+i))->height)){
			delete *(enemyBeam+i);
			*(enemyBeam+i)=NULL;
			myship->state=DAMAGE;
			return true;
		}
	}
	return false;
}

bool checkHitMyShip_Enemy(MyShip* myship, Enemy** enemy){
	for(int i=0;i<MAX_ENEMY;++i){
		if(*(enemy+i)==NULL) continue;
		if(checkHit(myship->x, myship->y, myship->width, myship->height, (*(enemy+i))->x, (*(enemy+i))->y, (*(enemy+i))->width, (*(enemy+i))->height)){
			myship->state=DAMAGE;
			return true;
		}
	}
}

void drawField(const MyShip* myship, MyShipBeam** myBeam, Enemy** enemy, EnemyBeam** eneBeam){
	write2Byte(myship->x, myship->y, myship->character, myship->attribute);
	for(int i=0;i<MAX_BEAM;++i){
		if(*(myBeam+i)!=NULL&&(*(myBeam+i))->state==NORMAL){
			write2Byte((*(myBeam+i))->x, (*(myBeam+i))->y, (*(myBeam+i))->character, (*(myBeam+i))->attribute);
		}
	}
	for(int i=0;i<MAX_ENEMY;++i){
		if(*(enemy+i)!=NULL&&(*(enemy+i))->state==NORMAL){
			write2Byte((*(enemy+i))->x, (*(enemy+i))->y, (*(enemy+i))->character, (*(enemy+i))->attribute);
		}
	}
	for(int i=0;i<MAX_ENEBEAM;++i){
		if(*(eneBeam+i)!=NULL&&(*(eneBeam+i))->state==NORMAL){
			write2Byte((*(eneBeam+i))->x, (*(eneBeam+i))->y, (*(eneBeam+i))->character, (*(eneBeam+i))->attribute);
		}
	}
}

void drawMenu(const MyShip* myship, int score, bool playing){
	fillRectangle(FIELD_WIDTH, 0, SCREEN_WIDTH-FIELD_WIDTH, SCREEN_HEIGHT, BACK_BLUE);
	char str[32];
	sprintf(str, "Score:   %08d", score);
	writeText(53, 4, str, CHAR_WHITE|BACK_BLUE);
	
	if(myship!=NULL) for(int i=0;i<myship->HP;++i) write2Byte(53+4*i, 14, myship->character, CHAR_WHITE|BACK_BLUE);
	
	if(playing){
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

bool isGameOver(MyShip* myship, Enemy** enemy){
	return myship->HP<=0;
}

int main(){
#ifdef DEBUG
	fout.open("log.txt", std::ios::out);
#endif

	initializeWindow(100, 200, SCREEN_WIDTH, SCREEN_HEIGHT, (char *)TITLE, true);

	MyShip* myship=NULL;
	MyShipBeam* myshipBeam[MAX_BEAM]={NULL};
	Enemy* enemy[MAX_ENEMY]={NULL};
	EnemyBeam* enemyBeam[MAX_ENEBEAM]={NULL};
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
			if(checkHitMyShip_EnemyBeam(myship, enemyBeam)|(checkHitMyShip_Enemy(myship, enemy))){
				--myship->HP;
			}

			if(myship->state==NORMAL){
				if(++touch>interval&&getKey('A')){
					makeMyShipBeam(myship, myshipBeam);
					touch=0;
				}
			}

			//自機弾関連処理
			for(int i=0;i<MAX_BEAM;++i){
				if(*(myshipBeam+i)!=NULL){
					if((*(myshipBeam+i))->state==NORMAL){
						(*(myshipBeam+i))->move();
					}
					else{
						delete *(myshipBeam+i);
						*(myshipBeam+i)=NULL;
					}
				}
			}

			//敵関連処理
			if(remainEnemy<=0){
				remainEnemy=makeEnemy(enemy);
			}
			for(int i=0;i<MAX_ENEMY;++i){
				if(*(enemy+i)!=NULL){
					if((*(enemy+i))->state==NORMAL){
						(*(enemy+i))->move();
						if(rand()%(40*remainEnemy)<1)makeEnemyBeam(*(enemy+i), enemyBeam);
						score+=checkHitEnemy(myshipBeam, *(enemy+i));
					}
					else{
						delete *(enemy+i);
						*(enemy+i)=NULL;
						--remainEnemy;
					}
				}
			}

			//敵弾関連処理
			for(int i=0;i<MAX_ENEBEAM;++i){
				if(*(enemyBeam+i)!=NULL){
					if((*(enemyBeam+i))->state==NORMAL){
						(*(enemyBeam+i))->move();
					}
					else{
						delete *(enemyBeam+i);
						*(enemyBeam+i)=NULL;
					}
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
