/*
 * Board.cpp
 *
 *  Created on: 2014/05/15
 *      Author: matsumoto
 */

#include "Reversi.h"
#include "Board.h"

/*
 * ���\�b�h�F flipDiscs
 * �A�N�Z�X�F private
 * �錾�@�@�F void flipDiscs(const Point& point)
 * �@�\�@�@�F point�Ŏw�肳�ꂽ�ʒu�ɐ΂�ł��A�͂��ݍ��߂邷�ׂĂ̐΂𗠕Ԃ��B
 * �@�@�@�@�@�@�u�ł����΁v�Ɓu���Ԃ����΁v��Updatelog�ɑ}������B
 *
 */

void Board::flipDiscs(const Point& point){

	int x;
	int y;

	// �s���������\����
	Disc ope_disc;
	ope_disc.x = point.x;
	ope_disc.y = point.y;
	ope_disc.color = CurrentColor;

	int dir = MovableDir[Turns][point.x][point.y];

	std::vector<Disc> update;

	RawBoard[point.x][point.y] = CurrentColor;
	update.push_back(ope_disc);

	// ��ɒu���邩�H
	if(dir & UPPER){
		y = point.y;
		ope_disc.x = point.x;
		while(RawBoard[point.x][--y] != CurrentColor){
			RawBoard[point.x][y] = CurrentColor;
			ope_disc.y = y;
			update.push_back(ope_disc);
		}
	}

	// ���ɒu���邩�H
	if(dir & LOWER){
		y = point.y;
		ope_disc.x = point.x;
		while(RawBoard[point.x][++y] != CurrentColor){
			RawBoard[point.x][y] = CurrentColor;
			ope_disc.y = y;
			update.push_back(ope_disc);
		}
	}

	// ���ɒu���邩�H
	if(dir & LEFT){
		x = point.x;
		ope_disc.y = point.y;
		while(RawBoard[--x][point.y] != CurrentColor){
			RawBoard[x][point.y] = CurrentColor;
			ope_disc.x = x;
			update.push_back(ope_disc);
		}
	}

	// �E�ɒu���邩�H
	if(dir & RIGHT){
		x = point.x;
		ope_disc.y = point.y;
		while(RawBoard[++x][point.y] != CurrentColor){
			RawBoard[x][point.y] = CurrentColor;
			ope_disc.x = x;
			update.push_back(ope_disc);
		}
	}

	// �E��ɒu���邩�H
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

	// ����ɒu���邩�H
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

	// �����ɒu���邩�H
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

	// �E���ɒu���邩�H
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

	// �΂̐����X�V
	int discdiff = update.size();

	Discs[CurrentColor] += discdiff;
	Discs[-CurrentColor] -= discdiff - 1;
	Discs[EMPTY]--;

	UpdateLog.push_back(update);

}


/*
 * ���\�b�h�F checkMobility
 * �A�N�Z�X�F private
 * �錾�@�@�F unsigned int checkMobility(const Disc& disc)
 * �@�\�@�@�F disc�Ŏw�肳�ꂽ���W�ɁAdisc.color�̐F�̐΂�łĂ邩�ǂ����A
 * �@�@�@�@�@�@�܂��A�ǂ̕����ɐ΂𗠕Ԃ��邩�𔻒肷��B
 * �@�@�@�@�@�@�΂𗠕Ԃ�������Ƀt���O�������������l���Ԃ�B
 *
 */

unsigned int Board::checkMobility(const Disc& disc) const{

	unsigned int dir = NONE; // �΂𗠕Ԃ�������Ƀt���O�������������l

	// ���łɐ΂���������u���Ȃ��B
	if(RawBoard[disc.x][disc.y] != EMPTY){
		dir = NONE;
		return dir;
	}

	int x;
	int y;

	// ��������`�F�b�N(y - 1)
	if(RawBoard[disc.x][disc.y - 1] == -disc.color){
		x = disc.x;
		y = disc.y - 2;
		while(RawBoard[x][y] == -disc.color){ y--; }
		if(RawBoard[x][y] == disc.color) dir = dir | UPPER; // ������Ƀt���O�𗧂Ă�B
	}

	// ���������`�F�b�N(y + 1)
	if(RawBoard[disc.x][disc.y + 1] == -disc.color){
		x = disc.x;
		y = disc.y + 2;
		while(RawBoard[x][y] == -disc.color){ y++; }
		if(RawBoard[x][y] == disc.color) dir = dir | LOWER; // �������Ƀt���O�𗧂Ă�B
	}

	// ���������`�F�b�N(x - 1)
	if(RawBoard[disc.x - 1][disc.y] == -disc.color){
		x = disc.x - 2;
		y = disc.y;
		while(RawBoard[x][y] == -disc.color){ x--; }
		if(RawBoard[x][y] == disc.color) dir = dir | LEFT; // �������Ƀt���O�𗧂Ă�B
	}

	// �E�������`�F�b�N(x + 1)
	if(RawBoard[disc.x + 1][disc.y] == -disc.color){
		x = disc.x + 2;
		y = disc.y;
		while(RawBoard[x][y] == -disc.color){ x++; }
		if(RawBoard[x][y] == disc.color) dir = dir | RIGHT; // �E�����Ƀt���O�𗧂Ă�B
	}

	// �E��������`�F�b�N(x + 1, y - 1)
	if(RawBoard[disc.x + 1][disc.y - 1] == -disc.color){
		x = disc.x + 2;
		y = disc.y - 2;
		while(RawBoard[x][y] == -disc.color){ x++; y--; }
		if(RawBoard[x][y] == disc.color) dir = dir | UPPER_RIGHT; // �E������Ƀt���O�𗧂Ă�B
	}

	// ����������`�F�b�N(x - 1, y - 1)
	if(RawBoard[disc.x - 1][disc.y - 1] == -disc.color){
		x = disc.x - 2;
		y = disc.y - 2;
		while(RawBoard[x][y] == -disc.color){ x--; y--; }
		if(RawBoard[x][y] == disc.color) dir = dir | UPPER_LEFT; // ��������Ƀt���O�𗧂Ă�B
	}

	// �����������`�F�b�N(x - 1, y + 1)
	if(RawBoard[disc.x - 1][disc.y + 1] == -disc.color){
		x = disc.x - 2;
		y = disc.y + 2;
		while(RawBoard[x][y] == -disc.color){ x--; y++; }
		if(RawBoard[x][y] == disc.color) dir = dir | LOWER_LEFT; // ���������Ƀt���O�𗧂Ă�B
	}

	// �E���������`�F�b�N(x + 1, y + 1)
	if(RawBoard[disc.x + 1][disc.y + 1] == -disc.color){
		x = disc.x + 2;
		y = disc.y + 2;
		while(RawBoard[x][y] == -disc.color){ x++; y++; }
		if(RawBoard[x][y] == disc.color) dir = dir | LOWER_RIGHT; // �E�������Ƀt���O�𗧂Ă�B
	}

	return dir;

}


/*
 * ���\�b�h�F initMovable
 * �A�N�Z�X�F private
 * �錾�@�@�F void initMovavle()
 * �@�\�@�@�F MovablePos[Turns]��MovableDir[Turns]���Čv�Z����B
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
 * ���\�b�h�F Board
 * �A�N�Z�X�F public
 * �錾�@�@�F Board()
 * �@�\�@�@�F Board�N���X�̃R���X�g���N�^
 *
 */

Board::Board(){

	init();

}

/*
 * ���\�b�h�F init
 * �A�N�Z�X�F public
 * �錾�@�@�F void init()
 * �@�\�@�@�F �{�[�h���Q�[���J�n����̏�Ԃɂ���BBoard�N���X�̃C���X�^���X��
 * �@�@�@�@�@�@�������ꂽ����́A�R���X�g���N�^�ɂ���ē��l�̏������������Ă΂�Ă���̂ŁA
 * �@�@�@�@�@�@init���ĂԕK�v�͂Ȃ��B
 *
 */

void Board::init(){

	// �S�}�X���󂫃}�X�ɐݒ�
	for(int x = 1; x <= BOARD_SIZE; x++){
		for(int y = 1; y <= BOARD_SIZE; y++){
			RawBoard[x][y] = EMPTY;
		}
	}

	// �ǂ̐ݒ�
	for(int y = 0; y < BOARD_SIZE + 2; y++){
		RawBoard[0][y] = WALL;
		RawBoard[BOARD_SIZE + 1][y] = WALL;
	}

	for(int x = 0; x < BOARD_SIZE +2; x++){
		RawBoard[x][0] = WALL;
		RawBoard[x][BOARD_SIZE + 1] = WALL;
	}

	// �����z�u
	RawBoard[4][4] = WHITE;
	RawBoard[5][5] = WHITE;
	RawBoard[4][5] = BLACK;
	RawBoard[5][4] = BLACK;

	// �ΐ��̏����ݒ�
	Discs[BLACK] = 2;
	Discs[WHITE] = 2;
	Discs[EMPTY] = BOARD_SIZE*BOARD_SIZE - 4;

	Turns = 0; // �萔��0���琔����B
	CurrentColor = BLACK; // ���͍�

	UpdateLog.clear(); // update�����ׂď���

	initMovable();

}


/*
 * ���\�b�h�F move
 * �A�N�Z�X�F public
 * �錾�@�@�F bool move(const Point& point)
 * �@�\�@�@�F point�Ŏw�肳�ꂽ�ʒu�ɐ΂�łB
 * �@�@�@�@�@�@����������������true�A���s������false���Ԃ�B
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
 * ���\�b�h�F pass
 * �A�N�Z�X�F public
 * �錾�@�@�F bool pass()
 * �@�\�@�@�F �p�X����B����������true���Ԃ�B�p�X�ł��Ȃ��ꍇ(�ł肪����ꍇ)��false���Ԃ�B
 *
 */

bool Board::pass(){

	// �ł肪����Ȃ�A�p�X�ł��Ȃ��B
	if(MovablePos[Turns].size() != 0) return false;

	// �Q�[�����I�����Ă���Ȃ�A�p�X�͂ł��Ȃ��B
	if(isGameOver()) return false;

	// �F�𑊎葤�̐F�ɂ���B
	CurrentColor = -CurrentColor;
	// ���update��}�����Ă����B
	UpdateLog.push_back(std::vector<Disc>());

	initMovable();

	return true;

}


/*
 * ���\�b�h�F undo
 * �A�N�Z�X�F public
 * �錾�@�@�F bool undo()
 * �@�\�@�@�F ���O�̈��ɖ߂��B����������true���Ԃ�B
 * �@�@�@�@�@�@���ɖ߂��Ȃ��ꍇ�A���Ȃ킿�܂������ł��ĂȂ��ꍇ��false���Ԃ�B
 *
 */

bool Board::undo(){

	// �Q�[���J�n���_�Ȃ�����߂�Ȃ��B
	if(Turns == 0) return false;

	// �F�𑊎葤�̐F�ɂ���B
	CurrentColor = -CurrentColor;

	// ���O�̈��̃��O���擾����B
	const std::vector<Disc>& update = UpdateLog.back();

	// �O�񂪃p�X���ǂ����ŏꍇ����
	if(update.empty()){ // �O�񂪃p�X�̏ꍇ

		// MovablePos��MovableDir���č\�z
		MovablePos[Turns].clear();
		for(int x=1; x <= BOARD_SIZE; x++){
			for(int y=1; y <= BOARD_SIZE; y++){
				MovableDir[Turns][x][y] = NONE;
			}
		}
	}else{ //�O�񂪃p�X�łȂ��ꍇ

		Turns--;

		// �΂����ɖ߂��B
		RawBoard[update[0].x][update[0].y] = EMPTY;
		for(unsigned int i=1; i<update.size(); i++){
			RawBoard[update[i].x][update[i].y] = -CurrentColor;
		}

		// �ΐ����X�V
		unsigned int discdiff = update.size();
		Discs[CurrentColor] -= discdiff;
		Discs[-CurrentColor] += discdiff - 1;
		Discs[EMPTY]++;

	}

	// �s�v�ɂȂ���update���P�폜
	UpdateLog.pop_back();

	return true;
}


/*
 * ���\�b�h�F isGameOver
 * �A�N�Z�X�F public
 * �錾�@�@�F bool isGameOver()
 * �@�\�@�@�F �Q�[�����I�����Ă����true���A�I�����Ă��Ȃ����false��Ԃ��B
 *
 */

bool Board::isGameOver() const{

	// 60��ɒB���Ă�����Q�[���I��
	int Turns_int = Turns;
	if(Turns_int == MAX_TURNS) return true;

	// �łĂ�肪����Ȃ�Q�[���I���ł͂Ȃ��B
	if(MovablePos[Turns].size() != 0) return false;

	// ���݂̎�ԂƋt�̐F���łĂ邩�ǂ������ׂ�B
	Disc disc;
	disc.color = -CurrentColor;

	for(int x=1; x<=BOARD_SIZE; x++){
		disc.x = x;
		for(int y=1; y<=BOARD_SIZE; y++){
			disc.y = y;
			// �u����ӏ����P�ł�����΃Q�[���I���ł͂Ȃ��B
			if(checkMobility(disc) != NONE) return false;
		}
	}

	return true;
}


/*
 * ���\�b�h�F countDisc
 * �A�N�Z�X�F public
 * �錾�@�@�F unsigned int countDisc(Color color) const
 * �@�\�@�@�F color�Ŏw�肳�ꂽ�΂̐��𐔂���B�F�ɂ�BLACK, WHITE, EMPTY���w��\�B
 *
 */

unsigned int Board::countDisc(Color color) const{
	return Discs[color];
}

/*
 * ���\�b�h�F getColor
 * �A�N�Z�X�F public
 * �錾�@�@�F Color getColor(const Point& point) const
 * �@�\�@�@�F point�Ŏw�肳�ꂽ�ʒu�̐F��Ԃ��B
 *
 */

Color Board::getColor(const Point& p) const{
	return RawBoard[p.x][p.y];
}

/*
 * ���\�b�h�F getMovablePos
 * �A�N�Z�X�F public
 * �錾�@�@�F const std::vector<Point>& getMovablePos() const
 * �@�\�@�@�F �΂��łĂ���W������vector��Ԃ��Bconst�t���Q�Ƃ��Ԃ�B
 *
 */

const std::vector<Point>& Board::getMovablePos() const{
	return MovablePos[Turns];
}


/*
 * ���\�b�h�F getUpdate
 * �A�N�Z�X�F public
 * �錾�@�@�F std::vector<Disc> getUpdate() const
 * �@�\�@�@�F ���O�̎�őł����΂Ɨ��Ԃ����΂�����vector��Ԃ��B
 *
 */

std::vector<Disc> Board::getUpdate() const{

	if(UpdateLog.empty()) return std::vector<Disc>();
	else return UpdateLog.back();

}

/*
 * ���\�b�h�F getCurrentColor
 * �A�N�Z�X�F public
 * �錾�@�@�F Color getCurrentColor() const
 * �@�\�@�@�F ���݂̎�Ԃ̐F��Ԃ��B
 *
 */

Color Board::getCurrentColor() const{
	return CurrentColor;
}

/*
 * ���\�b�h�F getTurns
 * �A�N�Z�X�F public
 * �錾�@�@�F unsigned int getTurns() const
 * �@�\�@�@�F ���݂̎萔��Ԃ��B�ŏ���0����n�܂�B
 *
 */

unsigned int Board::getTurns() const{
	return Turns;
}




/*
 * �ȉ��́A�e�X�g���\�b�h
 * �A�N�Z�X�F public
 *
 */

void Board::BoardTestPrint(){
	cout << "   a b c d e f g h " << endl;
	for(int y = 1; y <= 8; y++){
		cout << " " << y;
		for(int x = 1; x <= 8; x++){
			switch(getColor(Point(x, y))){
			case BLACK:
				cout << " ��";
				break;
			case WHITE:
				cout << " ��";
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
		cout << "����" << countDisc(BLACK) << " ";
		cout << "����" << countDisc(WHITE) << " ";
		cout << "��}�X" << countDisc(EMPTY) << endl << endl;

		switch(getCurrentColor()){
		case BLACK:
			cout << "����";
			break;
		case WHITE:
			cout << "����";
			break;
		}

		cout << "�����͂��Ă�������:";
		Point p;

		string in;
		cin >> in;

		if(in == "p"){ // �p�X
			bool rcd;
			rcd = pass();
			if(rcd == false){
				cerr << "�p�X�ł��܂���I" << endl;
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
			cerr << "���o�[�V�`���̎����͂��Ă��������I" << endl;
			continue;
		}

		bool rcd;
		rcd = move(p);
		if(rcd == false){
			cerr << "�����ɂ͒u���܂���I" << endl;
			continue;
		}

		if(isGameOver()){
			BoardTestPrint();
			cout << "����" << countDisc(BLACK) << " ";
			cout << "����" << countDisc(WHITE) << " ";
			cout << "��}�X" << countDisc(EMPTY) << endl << endl;
			cout << "-------- �Q�[���I�� --------" << endl;
			return 0;
		}

	}

	return 0;

}
