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
	// テスト変数
	this->count_eval = 0;
}

int AlphaBetaAI::evaluate(const Board& board){

	std::vector<Disc> discs = board.getUpdate();
	Point p;
	p.x = discs[0].x;
	p.y = discs[0].y;

	int i; // 評価値

	int i1 = evaluate_table(p);
	int w1 = 1; // 重み係数
	int i2 = board.getMovablePos().size();
	int w2 = 10; // 重み係数
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
	cout << "この局面の評価値は以下のとおり" << endl;
	cout << "評価値 : " << i << endl;
	cout << "********" << endl;

	count_eval++;
	return i;
}

int AlphaBetaAI::evaluate_table(Point p){

	// 得点テーブル
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

		// 石が打てる座標 + 評価値を持たせたクラス(Move)
		Move move;
		move.x = movables[i].x;
		move.y = movables[i].y;
		move.eval = eval;

		moves.push_back(move);

	}

	// 評価値の大きい順にソート
	std::stable_sort(moves.begin(), moves.end(), MoveGreater());

	// 結果の書き戻し
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

		cout << "******** 上記の手で1手打つ " << endl;
		board.move(movables[i]);

		cout << "再帰関数_before alphabeta() limit:[" << limit << "]" << endl;
		score = -alphabeta(board, limit-1, -beta, -alpha);
		cout << "再帰関数_after alphabeta() limit:[" << limit << "]" << endl;

		cout << "******** 1手戻す " << endl;
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
		if(board.getCurrentColor() == WHITE){  // コンピュータの手番
			board.pass();
			score = MinMax(board, limit);
			board.undo();
			return score;
		}else{   // 人の手番
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

	if(board.getCurrentColor() == WHITE){  // コンピュータの手番
		alpha = numeric_limits<int>::min();
		for(unsigned int i=0; i < movables.size(); i++){
			// debug
			cout << "Turn = " << board.getTurns() + 1 << endl;
			cout << "Color(1:BLACK, -1:WHITE) = " << board.getCurrentColor() << endl;
			cout << "i/Movables : " << i+1 << "/" << movables.size() << endl;
			cout << "(x, y) = (" << movables[i].x << ", " << movables[i].y << ")" << endl;

			cout << "******** 上記の手で1手打つ " << endl;
			board.move(movables[i]);

			//cout << "再帰関数_before MinMax() limit:[" << limit << "]" << endl;
			score = MinMax(board, limit-1);
			//cout << "再帰関数_after MinMax() limit:[" << limit << "]" << endl;

			cout << "******** 1手戻す " << endl;
			board.undo();

			if(score > alpha){ alpha = score; i_alpha = i; }
		}
		// この局面でのコンピュータにとっての最大値を返す。
		cout << "*** この局面での評価(alpha 最大値) ***" << endl;
		cout << "Turn = " << board.getTurns() + 1 << endl;
		cout << "コンピュータ手番" << endl;
		cout << "(x, y) = (" << movables[i_alpha].x << ", " << movables[i_alpha].y << ")" << endl;
		cout << "評価値: " << alpha << endl;
		return alpha;

	}else{     // 人の手番
		beta = numeric_limits<int>::max();
		for(unsigned int i=0; i < movables.size(); i++){
			// debug
			cout << "Turn = " << board.getTurns() + 1 << endl;
			cout << "Color(1:BLACK, -1:WHITE) = " << board.getCurrentColor() << endl;
			cout << "i/Movables : " << i+1 << "/" << movables.size() << endl;
			cout << "(x, y) = (" << movables[i].x << ", " << movables[i].y << ")" << endl;

			cout << "******** 上記の手で1手打つ " << endl;
			board.move(movables[i]);

			//cout << "再帰関数_before MinMax() limit:[" << limit << "]" << endl;
			score = MinMax(board, limit-1);
			//cout << "再帰関数_after MinMax() limit:[" << limit << "]" << endl;

			cout << "******** 1手戻す " << endl;
			board.undo();

			if(score < beta){ beta = score; i_beta = i; }
		}
		// この局面でのコンピュータにとっての最小値を返す。
		cout << "*** この局面での評価(beta 最小値) ***" << endl;
		cout << "Turn = " << board.getTurns() + 1 << endl;
		cout << "人の手番" << endl;
		cout << "(x, y) = (" << movables[i_beta].x << ", " << movables[i_beta].y << ")" << endl;
		cout << "評価値: " << beta << endl;
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

	// 石が打てる座標が並んだvectorを取得する。(getMovablePos()のconstをはずすため、キャストする。）
	std::vector<Point>& movables = (std::vector<Point>&)board.getMovablePos();

	if(movables.empty()){
		// 打てる箇所がなければパスする。
		board.pass();
		return;
	}

	if(movables.size() == 1){
		// 打てる箇所が一カ所だけなら探索を行わず、即座に打って返る。
		board.move(movables[0]);
		return;
	}

	/* Try No.1
	// テスト的にそれ以外は適当なところrandamに打つ。
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

	/* Try No.3 得点テーブル
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

	// 以下からが本格的
	int limit;
	// 事前に手を良さそうな順にソート
	//sort(board, movables, presearch_depth);

	if(MAX_TURNS - board.getTurns() <= wld_depth){
		// limit = numeric_limits<int>::max();
		cout << "コンピュータが必勝読みを開始..." << endl;
		// limit = wld_depth;
		limit = 7;
	}else{
		limit = normal_depth;
		// limit = 3; // 3手先読み
		// limit = 7; // 7手先読み
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

		cout << "******** 上記の手で1手打つ " << endl;
		board.move(movables[i]);

		//cout << "再帰関数_before move() limit:[" << limit << "] 打ち手Num:[" << i << "]"<< endl;
		// eval = -alphabeta(board, limit-1, -beta, -alpha);
		eval = MinMax(board, limit-1);
		//cout << "再帰関数_after move() limit:[" << limit << "] 打ち手Num:[" << i << "]"<< endl;

		cout << "******** 1手戻す " << endl;
		board.undo();

		if(eval > alpha){
			alpha = eval;
			p = movables[i];
		}
	}

	cout << "*** この局面での評価(alpha 最大値) ***" << endl;
	cout << "Turn = " << board.getTurns() + 1 << endl;
	cout << "コンピュータ手番" << endl;
	cout << "評価総数: " << getCountEval(board) << endl;
	setCountEval(0);
	cout << "(x, y) = (" << p.x << ", " << p.y << ")" << endl;
	cout << "評価値: " << alpha << endl;
	cout << "******** コンピュータの思考の結果、上記の手を打つ。 " << endl;
	board.move(p);

}





