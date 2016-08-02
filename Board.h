/*
 * Board.h
 *
 *  Created on: 2014/05/14
 *      Author: matsumoto
 */

#ifndef BOARD_H_
#define BOARD_H_

#include <vector>
#include <iostream>
#include "Reversi.h"

using namespace std;

const int BOARD_SIZE = 8;
const int MAX_TURNS = 60;

class Board{

private:
	Color RawBoard[BOARD_SIZE + 2][BOARD_SIZE + 2];
	unsigned int Turns; // 手数(0からはじめる）
	Color CurrentColor; // 現在のプレーヤー

	std::vector<std::vector<Disc> > UpdateLog;
	std::vector<Point> MovablePos[MAX_TURNS + 1];
	unsigned int MovableDir[MAX_TURNS + 1][BOARD_SIZE + 2][BOARD_SIZE +2];
	ColorStorage<unsigned int> Discs; // 各色の石の数

	enum Direction{
		NONE		=   0, // 石を打てない。
		UPPER		=   1, // 上方向に石を裏返せる。
		UPPER_LEFT	=   2, // 左上方向に石を裏返せる。
		LEFT		=   4, // 左方向に石を裏返せる。
		LOWER_LEFT	=   8, // 左下方向に石を裏返せる。
		LOWER		=  16, // 下方向に石を裏返せる。
		LOWER_RIGHT	=  32, // 右下方向に石を裏返せる。
		RIGHT		=  64, // 右方向に石を裏返せる。
		UPPER_RIGHT	= 128, // 右上方向に石を裏返せる。
	}; // 方向を表す実数

	void flipDiscs(const Point& point);
	unsigned int checkMobility(const Disc& disc) const;
	void initMovable();

public:
	Board();

	void init(); // ボードをゲーム開始直後の状態にする。
	bool move(const Point& point); // pointで指定された位置に石を打つ。
	bool pass(); // パスする。
	bool undo(); // １つ前の手の状態に戻す。
	bool isGameOver() const; // ゲームが終了しているかどうかを判定する。

	unsigned int countDisc(Color color) const; // 指定した石の個数を返す。
	Color getColor(const Point& p) const; // 指定した座標の石の色を調べる。
	const std::vector<Point>& getMovablePos() const; // 石を打てる座標が並んだvectorを返す。
	std::vector<Disc> getUpdate() const; // 直前の手で返された石が並んだvectorを返す。
	Color getCurrentColor() const; // 現在の手番の色を返す。
	unsigned int getTurns() const; // 現在の手数を返す。

	// テスト用メソッド
	void BoardTestPrint();
	int BoardTestMain();

};


#endif /* BOARD_H_ */
