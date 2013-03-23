//敵,弾最大出現数
const int MAX_ENEMY=200;
const int MAX_BEAM=10;
const int MAX_ENEBEAM=10;
//画面サイズ
const int SCREEN_HEIGHT=25;
const int SCREEN_WIDTH=80;
//自機敵が動き回る領域高さ
const int FIELD_HEIGHT=25;
const int FIELD_WIDTH=50;
//自機,敵,弾の状態を表す定数
const int NORMAL=0;
const int DAMAGE=1;
const int DISAPPEAR=2;

//方向ベクトル
const int v[5][2]={	{ 1, 0},
					{ 2, 0},
					{ 0, 1},
					{ 1, 1},
					{ 2, 1}};
const int d[4][2]={	{ 1, 1},
					{-1, 1},
					{-1,-1},
					{ 1,-1}};
