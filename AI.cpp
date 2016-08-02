/*
 * AI.cpp
 *
 *  Created on: 2014/05/18
 *      Author: matsumoto
 */

#include "AI.h"
#include "Board.h"

#include <limits>
#include <algorithm>
#include <iostream>

AlphaBetaAI::AlphaBetaAI() : AI(){
	// �e�X�g�ϐ�
	this->count_eval = 0;
}

int AlphaBetaAI::evaluate(const Board& board){

	std::vector<Disc> discs = board.getUpdate();
	Point p;
	p.x = discs[0].x;
	p.y = discs[0].y;

	int i; // �]���l

	int i1 = evaluate_table(p);
	int w1 = 1; // �d�݌W��
	int i2 = board.getMovablePos().size();
	int w2 = 10; // �d�݌W��
	int i3 = board.countDisc(WHITE) - board.countDisc(BLACK);
	int w3 = 2;

	i = w1*i1 + w2*i2;

	if(board.getTurns() >= 30){
			i = i + w3*i3;
		}

	if(board.getTurns() >= 40){
		i = board.countDisc(WHITE) - board.countDisc(BLACK);
	}

	cout << "evaluate()" << endl;
	cout << "���̋ǖʂ̕]���l�͈ȉ��̂Ƃ���" << endl;
	cout << "�]���l : " << i << endl;
	cout << "********" << endl;

	count_eval++;
	return i;
}

int AlphaBetaAI::evaluate_table(Point p){

	// ���_�e�[�u��
	int eval[BOARD_SIZE+2][BOARD_SIZE+2]={{0,  0,  0,  0,  0,  0,  0,  0,  0,0},
										  {0,100,-50, 10,  0,  0, 10,-50,100,0},
										  {0,-50,-70, -5,-10,-10, -5,-70,-50,0},
										  {0, 10, -5,-10, -5, -5,-10, -5, 10,0},
										  {0,  0,-10, -5,  0,  0, -5,-10,  0,0},
										  {0,  0,-10, -5,  0,  0, -5,-10,  0,0},
										  {0, 10, -5,-10, -5, -5,-10, -5, 10,0},
										  {0,-50,-70, -5,-10,-10, -5,-70,-50,0},
										  {0,100,-50, 10,  0,  0, 10,-50,100,0},
										  {0,  0,  0,  0,  0,  0,  0,  0,  0,0}};

	return eval[p.x][p.y];
}

void AlphaBetaAI::sort(Board& board, std::vector<Point>& movables, int limit){

	std::vector<Move> moves;

	int eval=0;
	for(unsigned int i=0; i < movables.size(); i++){
		// int eval;

		board.move(movables[i]);
		eval = -alphabeta(board, limit-1, -INT_MAX, INT_MAX);
		board.undo();

		// �΂��łĂ���W + �]���l�����������N���X(Move)
		Move move;
		move.x = movables[i].x;
		move.y = movables[i].y;
		move.eval = eval;

		moves.push_back(move);

	}

	// �]���l�̑傫�����Ƀ\�[�g
	std::stable_sort(moves.begin(), moves.end(), MoveGreater());

	// ���ʂ̏����߂�
	movables.clear();
	for(unsigned int i=0; i < movables.size(); i++){
		movables.push_back(moves[i]);
	}

	return;

}


int AlphaBetaAI::alphabeta(Board& board, int limit, int alpha, int beta){

	if(board.isGameOver() || limit == 0){
		return evaluate(board);
	}

	int score;
	const std::vector<Point>& movables = board.getMovablePos();

	if(movables.empty()){
		board.pass();
		score = -alphabeta(board, limit, -beta, -alpha);
		board.undo();
		return score;
	}

	for(unsigned int i=0; i < movables.size(); i++){

		// debug
		cout << "Turn = " << board.getTurns() + 1 << endl;
		cout << "Color(1:BLACK, -1:WHITE) = " << board.getCurrentColor() << endl;
		cout << "i/Movables : " << i+1 << "/" << movables.size() << endl;
		cout << "(x, y) = (" << movables[i].x << ", " << movables[i].y << ")" << endl;

		cout << "******** ��L�̎��1��ł� " << endl;
		board.move(movables[i]);

		cout << "�ċA�֐�_before alphabeta() limit:[" << limit << "]" << endl;
		score = -alphabeta(board, limit-1, -beta, -alpha);
		cout << "�ċA�֐�_after alphabeta() limit:[" << limit << "]" << endl;

		cout << "******** 1��߂� " << endl;
		board.undo();

		if(score >= beta){ return score;}

		alpha = std::max(alpha, score);

	}

	return alpha;
}

int AlphaBetaAI::MinMax(Board& board, int limit){

	if(board.isGameOver() || limit == 0){
		return evaluate(board);
	}

	int score;
	const std::vector<Point>& movables = board.getMovablePos();

	if(movables.empty()){
		if(board.getCurrentColor() == WHITE){  // �R���s���[�^�̎��
			board.pass();
			score = MinMax(board, limit);
			board.undo();
			return score;
		}else{   // �l�̎��
			board.pass();
			score = MinMax(board, limit);
			board.undo();
			return score;
		}

	}

	int alpha;
	int beta;
	unsigned int i_alpha;
	unsigned int i_beta;

	if(board.getCurrentColor() == WHITE){  // �R���s���[�^�̎��
		alpha = numeric_limits<int>::min();
		for(unsigned int i=0; i < movables.size(); i++){
			// debug
			cout << "Turn = " << board.getTurns() + 1 << endl;
			cout << "Color(1:BLACK, -1:WHITE) = " << board.getCurrentColor() << endl;
			cout << "i/Movables : " << i+1 << "/" << movables.size() << endl;
			cout << "(x, y) = (" << movables[i].x << ", " << movables[i].y << ")" << endl;

			cout << "******** ��L�̎��1��ł� " << endl;
			board.move(movables[i]);

			//cout << "�ċA�֐�_before MinMax() limit:[" << limit << "]" << endl;
			score = MinMax(board, limit-1);
			//cout << "�ċA�֐�_after MinMax() limit:[" << limit << "]" << endl;

			cout << "******** 1��߂� " << endl;
			board.undo();

			if(score > alpha){ alpha = score; i_alpha = i; }
		}
		// ���̋ǖʂł̃R���s���[�^�ɂƂ��Ă̍ő�l��Ԃ��B
		cout << "*** ���̋ǖʂł̕]��(alpha �ő�l) ***" << endl;
		cout << "Turn = " << board.getTurns() + 1 << endl;
		cout << "�R���s���[�^���" << endl;
		cout << "(x, y) = (" << movables[i_alpha].x << ", " << movables[i_alpha].y << ")" << endl;
		cout << "�]���l: " << alpha << endl;
		return alpha;

	}else{     // �l�̎��
		beta = numeric_limits<int>::max();
		for(unsigned int i=0; i < movables.size(); i++){
			// debug
			cout << "Turn = " << board.getTurns() + 1 << endl;
			cout << "Color(1:BLACK, -1:WHITE) = " << board.getCurrentColor() << endl;
			cout << "i/Movables : " << i+1 << "/" << movables.size() << endl;
			cout << "(x, y) = (" << movables[i].x << ", " << movables[i].y << ")" << endl;

			cout << "******** ��L�̎��1��ł� " << endl;
			board.move(movables[i]);

			//cout << "�ċA�֐�_before MinMax() limit:[" << limit << "]" << endl;
			score = MinMax(board, limit-1);
			//cout << "�ċA�֐�_after MinMax() limit:[" << limit << "]" << endl;

			cout << "******** 1��߂� " << endl;
			board.undo();

			if(score < beta){ beta = score; i_beta = i; }
		}
		// ���̋ǖʂł̃R���s���[�^�ɂƂ��Ă̍ŏ��l��Ԃ��B
		cout << "*** ���̋ǖʂł̕]��(beta �ŏ��l) ***" << endl;
		cout << "Turn = " << board.getTurns() + 1 << endl;
		cout << "�l�̎��" << endl;
		cout << "(x, y) = (" << movables[i_beta].x << ", " << movables[i_beta].y << ")" << endl;
		cout << "�]���l: " << beta << endl;
		return beta;
	}

}

int AlphaBetaAI::getCountEval(Board& board){
	return this->count_eval;
}
void AlphaBetaAI::setCountEval(int count){
	this->count_eval = count;
}

void AlphaBetaAI::move(Board& board){

	// �΂��łĂ���W������vector���擾����B(getMovablePos()��const���͂������߁A�L���X�g����B�j
	std::vector<Point>& movables = (std::vector<Point>&)board.getMovablePos();

	if(movables.empty()){
		// �łĂ�ӏ����Ȃ���΃p�X����B
		board.pass();
		return;
	}

	if(movables.size() == 1){
		// �łĂ�ӏ�����J�������Ȃ�T�����s�킸�A�����ɑł��ĕԂ�B
		board.move(movables[0]);
		return;
	}

	/* Try No.1
	// �e�X�g�I�ɂ���ȊO�͓K���ȂƂ���randam�ɑłB
	int m_size = movables.size();
	int i = rand()%m_size;
	board.move(movables[i]);
	return;
	 *
	 */

	/* Try No.2
	int eval1 = 0;
	int alpha1 = 0;
	Point p1;
	for(unsigned int i=0; i < movables.size(); i++){
		board.move(movables[i]);
		eval1 = board.getMovablePos().size();
		board.undo();

		if(eval1 > alpha1){
			alpha1 = eval1;
			p1 = movables[i];
		}
	}

	board.move(p1);
	return;
	*
	*/

	/* Try No.3 ���_�e�[�u��
	int eval1 = -1000;
	int alpha1 = -1000;
	Point p1;
	for(unsigned int i=0; i < movables.size(); i++){
		eval1 = evaluate_table(movables[i]);

		if(eval1 > alpha1){
			alpha1 = eval1;
			p1 = movables[i];
		}
	}

	board.move(p1);
	return;
	*/

	// �ȉ����炪�{�i�I
	int limit;
	// ���O�Ɏ��ǂ������ȏ��Ƀ\�[�g
	//sort(board, movables, presearch_depth);

	if(MAX_TURNS - board.getTurns() <= wld_depth){
		// limit = numeric_limits<int>::max();
		cout << "�R���s���[�^���K���ǂ݂��J�n..." << endl;
		// limit = wld_depth;
		limit = 7;
	}else{
		limit = normal_depth;
		// limit = 3; // 3���ǂ�
		// limit = 7; // 7���ǂ�
	}

	int eval, alpha = numeric_limits<int>::min();
	// int beta = numeric_limits<int>::max();
	Point p;
	for(unsigned int i=0; i < movables.size(); i++){

		// debug
		cout << "Turn = " << board.getTurns() + 1 << endl;
		cout << "Color(1:BLACK, -1:WHITE) = " << board.getCurrentColor() << endl;
		cout << "i/Movables : " << i+1 << "/" << movables.size() << endl;
		cout << "move (x, y) = (" << movables[i].x << ", " << movables[i].y << ")" << endl;

		cout << "******** ��L�̎��1��ł� " << endl;
		board.move(movables[i]);

		//cout << "�ċA�֐�_before move() limit:[" << limit << "] �ł���Num:[" << i << "]"<< endl;
		// eval = -alphabeta(board, limit-1, -beta, -alpha);
		eval = MinMax(board, limit-1);
		//cout << "�ċA�֐�_after move() limit:[" << limit << "] �ł���Num:[" << i << "]"<< endl;

		cout << "******** 1��߂� " << endl;
		board.undo();

		if(eval > alpha){
			alpha = eval;
			p = movables[i];
		}
	}

	cout << "*** ���̋ǖʂł̕]��(alpha �ő�l) ***" << endl;
	cout << "Turn = " << board.getTurns() + 1 << endl;
	cout << "�R���s���[�^���" << endl;
	cout << "�]������: " << getCountEval(board) << endl;
	setCountEval(0);
	cout << "(x, y) = (" << p.x << ", " << p.y << ")" << endl;
	cout << "�]���l: " << alpha << endl;
	cout << "******** �R���s���[�^�̎v�l�̌��ʁA��L�̎��łB " << endl;
	board.move(p);

}





