/*
 * Board.cpp
 *
 *  Created on: 2014/05/15
 *      Author: matsumoto
 */

#include "Reversi.h"
#include "Board.h"

/*
 * メソッド： flipDiscs
 * アクセス： private
 * 宣言　　： void flipDiscs(const Point& point)
 * 機能　　： pointで指定された位置に石を打ち、はさみ込めるすべての石を裏返す。
 * 　　　　　　「打った石」と「裏返した石」をUpdatelogに挿入する。
 *
 */

void Board::flipDiscs(const Point& point){

	int x;
	int y;

	// 行った操作を表す石
	Disc ope_disc;
	ope_disc.x = point.x;
	ope_disc.y = point.y;
	ope_disc.color = CurrentColor;

	int dir = MovableDir[Turns][point.x][point.y];

	std::vector<Disc> update;

	RawBoard[point.x][point.y] = CurrentColor;
	update.push_back(ope_disc);

	// 上に置けるか？
	if(dir & UPPER){
		y = point.y;
		ope_disc.x = point.x;
		while(RawBoard[point.x][--y] != CurrentColor){
			RawBoard[point.x][y] = CurrentColor;
			ope_disc.y = y;
			update.push_back(ope_disc);
		}
	}

	// 下に置けるか？
	if(dir & LOWER){
		y = point.y;
		ope_disc.x = point.x;
		while(RawBoard[point.x][++y] != CurrentColor){
			RawBoard[point.x][y] = CurrentColor;
			ope_disc.y = y;
			update.push_back(ope_disc);
		}
	}

	// 左に置けるか？
	if(dir & LEFT){
		x = point.x;
		ope_disc.y = point.y;
		while(RawBoard[--x][point.y] != CurrentColor){
			RawBoard[x][point.y] = CurrentColor;
			ope_disc.x = x;
			update.push_back(ope_disc);
		}
	}

	// 右に置けるか？
	if(dir & RIGHT){
		x = point.x;
		ope_disc.y = point.y;
		while(RawBoard[++x][point.y] != CurrentColor){
			RawBoard[x][point.y] = CurrentColor;
			ope_disc.x = x;
			update.push_back(ope_disc);
		}
	}

	// 右上に置けるか？
	if(dir & UPPER_RIGHT){
		x = point.x;
		y = point.y;
		while(RawBoard[++x][--y] != CurrentColor){
			RawBoard[x][y] = CurrentColor;
			ope_disc.x = x;
			ope_disc.y = y;
			update.push_back(ope_disc);
		}
	}

	// 左上に置けるか？
	if(dir & UPPER_LEFT){
		x = point.x;
		y = point.y;
		while(RawBoard[--x][--y] != CurrentColor){
			RawBoard[x][y] = CurrentColor;
			ope_disc.x = x;
			ope_disc.y = y;
			update.push_back(ope_disc);
		}
	}

	// 左下に置けるか？
	if(dir & LOWER_LEFT){
		x = point.x;
		y = point.y;
		while(RawBoard[--x][++y] != CurrentColor){
			RawBoard[x][y] = CurrentColor;
			ope_disc.x = x;
			ope_disc.y = y;
			update.push_back(ope_disc);
		}
	}

	// 右下に置けるか？
	if(dir & LOWER_RIGHT){
		x = point.x;
		y = point.y;
		while(RawBoard[++x][++y] != CurrentColor){
			RawBoard[x][y] = CurrentColor;
			ope_disc.x = x;
			ope_disc.y = y;
			update.push_back(ope_disc);
		}
	}

	// 石の数を更新
	int discdiff = update.size();

	Discs[CurrentColor] += discdiff;
	Discs[-CurrentColor] -= discdiff - 1;
	Discs[EMPTY]--;

	UpdateLog.push_back(update);

}


/*
 * メソッド： checkMobility
 * アクセス： private
 * 宣言　　： unsigned int checkMobility(const Disc& disc)
 * 機能　　： discで指定された座標に、disc.colorの色の石を打てるかどうか、
 * 　　　　　　また、どの方向に石を裏返せるかを判定する。
 * 　　　　　　石を裏返せる方向にフラグが立った整数値が返る。
 *
 */

unsigned int Board::checkMobility(const Disc& disc) const{

	unsigned int dir = NONE; // 石を裏返せる方向にフラグが立った整数値

	// すでに石があったら置けない。
	if(RawBoard[disc.x][disc.y] != EMPTY){
		dir = NONE;
		return dir;
	}

	int x;
	int y;

	// 上方向をチェック(y - 1)
	if(RawBoard[disc.x][disc.y - 1] == -disc.color){
		x = disc.x;
		y = disc.y - 2;
		while(RawBoard[x][y] == -disc.color){ y--; }
		if(RawBoard[x][y] == disc.color) dir = dir | UPPER; // 上方向にフラグを立てる。
	}

	// 下方向をチェック(y + 1)
	if(RawBoard[disc.x][disc.y + 1] == -disc.color){
		x = disc.x;
		y = disc.y + 2;
		while(RawBoard[x][y] == -disc.color){ y++; }
		if(RawBoard[x][y] == disc.color) dir = dir | LOWER; // 下方向にフラグを立てる。
	}

	// 左方向をチェック(x - 1)
	if(RawBoard[disc.x - 1][disc.y] == -disc.color){
		x = disc.x - 2;
		y = disc.y;
		while(RawBoard[x][y] == -disc.color){ x--; }
		if(RawBoard[x][y] == disc.color) dir = dir | LEFT; // 左方向にフラグを立てる。
	}

	// 右方向をチェック(x + 1)
	if(RawBoard[disc.x + 1][disc.y] == -disc.color){
		x = disc.x + 2;
		y = disc.y;
		while(RawBoard[x][y] == -disc.color){ x++; }
		if(RawBoard[x][y] == disc.color) dir = dir | RIGHT; // 右方向にフラグを立てる。
	}

	// 右上方向をチェック(x + 1, y - 1)
	if(RawBoard[disc.x + 1][disc.y - 1] == -disc.color){
		x = disc.x + 2;
		y = disc.y - 2;
		while(RawBoard[x][y] == -disc.color){ x++; y--; }
		if(RawBoard[x][y] == disc.color) dir = dir | UPPER_RIGHT; // 右上方向にフラグを立てる。
	}

	// 左上方向をチェック(x - 1, y - 1)
	if(RawBoard[disc.x - 1][disc.y - 1] == -disc.color){
		x = disc.x - 2;
		y = disc.y - 2;
		while(RawBoard[x][y] == -disc.color){ x--; y--; }
		if(RawBoard[x][y] == disc.color) dir = dir | UPPER_LEFT; // 左上方向にフラグを立てる。
	}

	// 左下方向をチェック(x - 1, y + 1)
	if(RawBoard[disc.x - 1][disc.y + 1] == -disc.color){
		x = disc.x - 2;
		y = disc.y + 2;
		while(RawBoard[x][y] == -disc.color){ x--; y++; }
		if(RawBoard[x][y] == disc.color) dir = dir | LOWER_LEFT; // 左下方向にフラグを立てる。
	}

	// 右下方向をチェック(x + 1, y + 1)
	if(RawBoard[disc.x + 1][disc.y + 1] == -disc.color){
		x = disc.x + 2;
		y = disc.y + 2;
		while(RawBoard[x][y] == -disc.color){ x++; y++; }
		if(RawBoard[x][y] == disc.color) dir = dir | LOWER_RIGHT; // 右下方向にフラグを立てる。
	}

	return dir;

}


/*
 * メソッド： initMovable
 * アクセス： private
 * 宣言　　： void initMovavle()
 * 機能　　： MovablePos[Turns]とMovableDir[Turns]を再計算する。
 *
 */

void Board::initMovable(){

	Disc disc(0, 0, CurrentColor);

	int dir;

	MovablePos[Turns].clear();

	for(int x = 1; x <= BOARD_SIZE; x++){
		disc.x = x;
		for(int y = 1; y <= BOARD_SIZE; y++){
			disc.y = y;

			dir = checkMobility(disc);
			if(dir != NONE){
				MovablePos[Turns].push_back(disc);
			}
			MovableDir[Turns][x][y] = dir;
		}
	}

}


/*
 * メソッド： Board
 * アクセス： public
 * 宣言　　： Board()
 * 機能　　： Boardクラスのコンストラクタ
 *
 */

Board::Board(){

	init();

}

/*
 * メソッド： init
 * アクセス： public
 * 宣言　　： void init()
 * 機能　　： ボードをゲーム開始直後の状態にする。Boardクラスのインスタンスが
 * 　　　　　　生成された直後は、コンストラクタによって同様の初期化処理が呼ばれているので、
 * 　　　　　　initを呼ぶ必要はない。
 *
 */

void Board::init(){

	// 全マスを空きマスに設定
	for(int x = 1; x <= BOARD_SIZE; x++){
		for(int y = 1; y <= BOARD_SIZE; y++){
			RawBoard[x][y] = EMPTY;
		}
	}

	// 壁の設定
	for(int y = 0; y < BOARD_SIZE + 2; y++){
		RawBoard[0][y] = WALL;
		RawBoard[BOARD_SIZE + 1][y] = WALL;
	}

	for(int x = 0; x < BOARD_SIZE +2; x++){
		RawBoard[x][0] = WALL;
		RawBoard[x][BOARD_SIZE + 1] = WALL;
	}

	// 初期配置
	RawBoard[4][4] = WHITE;
	RawBoard[5][5] = WHITE;
	RawBoard[4][5] = BLACK;
	RawBoard[5][4] = BLACK;

	// 石数の初期設定
	Discs[BLACK] = 2;
	Discs[WHITE] = 2;
	Discs[EMPTY] = BOARD_SIZE*BOARD_SIZE - 4;

	Turns = 0; // 手数は0から数える。
	CurrentColor = BLACK; // 先手は黒

	UpdateLog.clear(); // updateをすべて消去

	initMovable();

}


/*
 * メソッド： move
 * アクセス： public
 * 宣言　　： bool move(const Point& point)
 * 機能　　： pointで指定された位置に石を打つ。
 * 　　　　　　処理が成功したらtrue、失敗したらfalseが返る。
 *
 */

bool Board::move(const Point& point){

	if(point.x < 0 || point.x > BOARD_SIZE) return false;
	if(point.y < 0 || point.y > BOARD_SIZE) return false;
	if(MovableDir[Turns][point.x][point.y] == NONE) return false;

	flipDiscs(point);

	Turns++;
	CurrentColor = -CurrentColor;

	initMovable();

	return true;

}


/*
 * メソッド： pass
 * アクセス： public
 * 宣言　　： bool pass()
 * 機能　　： パスする。成功したらtrueが返る。パスできない場合(打つ手がある場合)はfalseが返る。
 *
 */

bool Board::pass(){

	// 打つ手があるなら、パスできない。
	if(MovablePos[Turns].size() != 0) return false;

	// ゲームが終了しているなら、パスはできない。
	if(isGameOver()) return false;

	// 色を相手側の色にする。
	CurrentColor = -CurrentColor;
	// 空のupdateを挿入しておく。
	UpdateLog.push_back(std::vector<Disc>());

	initMovable();

	return true;

}


/*
 * メソッド： undo
 * アクセス： public
 * 宣言　　： bool undo()
 * 機能　　： 直前の一手に戻す。成功したらtrueが返る。
 * 　　　　　　元に戻せない場合、すなわちまだ一手も打ってない場合はfalseが返る。
 *
 */

bool Board::undo(){

	// ゲーム開始時点ならもう戻れない。
	if(Turns == 0) return false;

	// 色を相手側の色にする。
	CurrentColor = -CurrentColor;

	// 直前の一手のログを取得する。
	const std::vector<Disc>& update = UpdateLog.back();

	// 前回がパスかどうかで場合分け
	if(update.empty()){ // 前回がパスの場合

		// MovablePosとMovableDirを再構築
		MovablePos[Turns].clear();
		for(int x=1; x <= BOARD_SIZE; x++){
			for(int y=1; y <= BOARD_SIZE; y++){
				MovableDir[Turns][x][y] = NONE;
			}
		}
	}else{ //前回がパスでない場合

		Turns--;

		// 石を元に戻す。
		RawBoard[update[0].x][update[0].y] = EMPTY;
		for(unsigned int i=1; i<update.size(); i++){
			RawBoard[update[i].x][update[i].y] = -CurrentColor;
		}

		// 石数を更新
		unsigned int discdiff = update.size();
		Discs[CurrentColor] -= discdiff;
		Discs[-CurrentColor] += discdiff - 1;
		Discs[EMPTY]++;

	}

	// 不要になったupdateを１つ削除
	UpdateLog.pop_back();

	return true;
}


/*
 * メソッド： isGameOver
 * アクセス： public
 * 宣言　　： bool isGameOver()
 * 機能　　： ゲームが終了していればtrueを、終了していなければfalseを返す。
 *
 */

bool Board::isGameOver() const{

	// 60手に達していたらゲーム終了
	int Turns_int = Turns;
	if(Turns_int == MAX_TURNS) return true;

	// 打てる手があるならゲーム終了ではない。
	if(MovablePos[Turns].size() != 0) return false;

	// 現在の手番と逆の色が打てるかどうか調べる。
	Disc disc;
	disc.color = -CurrentColor;

	for(int x=1; x<=BOARD_SIZE; x++){
		disc.x = x;
		for(int y=1; y<=BOARD_SIZE; y++){
			disc.y = y;
			// 置ける箇所が１つでもあればゲーム終了ではない。
			if(checkMobility(disc) != NONE) return false;
		}
	}

	return true;
}


/*
 * メソッド： countDisc
 * アクセス： public
 * 宣言　　： unsigned int countDisc(Color color) const
 * 機能　　： colorで指定された石の数を数える。色にはBLACK, WHITE, EMPTYを指定可能。
 *
 */

unsigned int Board::countDisc(Color color) const{
	return Discs[color];
}

/*
 * メソッド： getColor
 * アクセス： public
 * 宣言　　： Color getColor(const Point& point) const
 * 機能　　： pointで指定された位置の色を返す。
 *
 */

Color Board::getColor(const Point& p) const{
	return RawBoard[p.x][p.y];
}

/*
 * メソッド： getMovablePos
 * アクセス： public
 * 宣言　　： const std::vector<Point>& getMovablePos() const
 * 機能　　： 石が打てる座標が並んだvectorを返す。const付き参照が返る。
 *
 */

const std::vector<Point>& Board::getMovablePos() const{
	return MovablePos[Turns];
}


/*
 * メソッド： getUpdate
 * アクセス： public
 * 宣言　　： std::vector<Disc> getUpdate() const
 * 機能　　： 直前の手で打った石と裏返した石が並んだvectorを返す。
 *
 */

std::vector<Disc> Board::getUpdate() const{

	if(UpdateLog.empty()) return std::vector<Disc>();
	else return UpdateLog.back();

}

/*
 * メソッド： getCurrentColor
 * アクセス： public
 * 宣言　　： Color getCurrentColor() const
 * 機能　　： 現在の手番の色を返す。
 *
 */

Color Board::getCurrentColor() const{
	return CurrentColor;
}

/*
 * メソッド： getTurns
 * アクセス： public
 * 宣言　　： unsigned int getTurns() const
 * 機能　　： 現在の手数を返す。最初は0から始まる。
 *
 */

unsigned int Board::getTurns() const{
	return Turns;
}




/*
 * 以下は、テストメソッド
 * アクセス： public
 *
 */

void Board::BoardTestPrint(){
	cout << "   a b c d e f g h " << endl;
	for(int y = 1; y <= 8; y++){
		cout << " " << y;
		for(int x = 1; x <= 8; x++){
			switch(getColor(Point(x, y))){
			case BLACK:
				cout << " ●";
				break;
			case WHITE:
				cout << " ○";
				break;
			default:
				cout << "  ";
				break;
			}
		}
		cout << endl;
	}
}

int Board::BoardTestMain(){

	while(true){

		BoardTestPrint();
		cout << "黒石" << countDisc(BLACK) << " ";
		cout << "白石" << countDisc(WHITE) << " ";
		cout << "空マス" << countDisc(EMPTY) << endl << endl;

		switch(getCurrentColor()){
		case BLACK:
			cout << "黒＞";
			break;
		case WHITE:
			cout << "白＞";
			break;
		}

		cout << "手を入力してください:";
		Point p;

		string in;
		cin >> in;

		if(in == "p"){ // パス
			bool rcd;
			rcd = pass();
			if(rcd == false){
				cerr << "パスできません！" << endl;
			}
			continue;
		}

		if(in == "u"){ // undo
			undo();
			continue;
		}

		if(in == "e"){
			cout << "-------- end --------" << endl;
			return 0;
		}

		try{
			Point parse(in);
			p = parse;
		}catch(invalid_argument& e){
			cerr << "リバーシ形式の手を入力してください！" << endl;
			continue;
		}

		bool rcd;
		rcd = move(p);
		if(rcd == false){
			cerr << "そこには置けません！" << endl;
			continue;
		}

		if(isGameOver()){
			BoardTestPrint();
			cout << "黒石" << countDisc(BLACK) << " ";
			cout << "白石" << countDisc(WHITE) << " ";
			cout << "空マス" << countDisc(EMPTY) << endl << endl;
			cout << "-------- ゲーム終了 --------" << endl;
			return 0;
		}

	}

	return 0;

}
