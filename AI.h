/*
 * AI.h
 *
 *  Created on: 2014/05/18
 *      Author: matsumoto
 */

#ifndef AI_H_
#define AI_H_

#include "Board.h"
#include <vector>
#include <functional>

using namespace std;


class AI{

public:

	unsigned int presearch_depth;
	unsigned int normal_depth;
	unsigned int wld_depth;
	unsigned int perfect_depth;

	// AI() : presearch_depth(3), normal_depth(5), wld_depth(15), perfect_depth(13){}
	AI(){
		this->presearch_depth=3;
		this->normal_depth=5;
		this->wld_depth=15;
		this->perfect_depth=13;
	}

	virtual ~AI(){}

	virtual void move(Board&)=0;

};


class AlphaBetaAI : public AI{

private:
	int evaluate(const Board& board);
	void sort(Board& board, std::vector<Point>&, int limit);
	int alphabeta(Board& board, int limit, int alpha, int beta);
	int MinMax(Board& board, int limit);
	// テストメソッド
	int evaluate_table(Point);
	// テスト変数
	int count_eval;

protected:
	// 石が打てる座標に評価値を持たせたクラスMove
	class Move : public Point{
	public:
		int eval;
		Move(){ Point(0, 0); this->eval=0;}
	};

	// Moveインスタンスの評価値を比較する。
	class MoveGreater{
	public:
		bool operator() (const Move& lhs, const Move& rhs){
			return(lhs.eval > rhs.eval);
		}
	};

public:
	AlphaBetaAI();
	void move(Board& board);
	// テストメソッド
	int getCountEval(Board& board);
	void setCountEval(int count);

};


#endif /* AI_H_ */
