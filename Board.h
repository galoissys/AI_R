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
	unsigned int Turns; // �萔(0����͂��߂�j
	Color CurrentColor; // ���݂̃v���[���[

	std::vector<std::vector<Disc> > UpdateLog;
	std::vector<Point> MovablePos[MAX_TURNS + 1];
	unsigned int MovableDir[MAX_TURNS + 1][BOARD_SIZE + 2][BOARD_SIZE +2];
	ColorStorage<unsigned int> Discs; // �e�F�̐΂̐�

	enum Direction{
		NONE		=   0, // �΂�łĂȂ��B
		UPPER		=   1, // ������ɐ΂𗠕Ԃ���B
		UPPER_LEFT	=   2, // ��������ɐ΂𗠕Ԃ���B
		LEFT		=   4, // �������ɐ΂𗠕Ԃ���B
		LOWER_LEFT	=   8, // ���������ɐ΂𗠕Ԃ���B
		LOWER		=  16, // �������ɐ΂𗠕Ԃ���B
		LOWER_RIGHT	=  32, // �E�������ɐ΂𗠕Ԃ���B
		RIGHT		=  64, // �E�����ɐ΂𗠕Ԃ���B
		UPPER_RIGHT	= 128, // �E������ɐ΂𗠕Ԃ���B
	}; // ������\������

	void flipDiscs(const Point& point);
	unsigned int checkMobility(const Disc& disc) const;
	void initMovable();

public:
	Board();

	void init(); // �{�[�h���Q�[���J�n����̏�Ԃɂ���B
	bool move(const Point& point); // point�Ŏw�肳�ꂽ�ʒu�ɐ΂�łB
	bool pass(); // �p�X����B
	bool undo(); // �P�O�̎�̏�Ԃɖ߂��B
	bool isGameOver() const; // �Q�[�����I�����Ă��邩�ǂ����𔻒肷��B

	unsigned int countDisc(Color color) const; // �w�肵���΂̌���Ԃ��B
	Color getColor(const Point& p) const; // �w�肵�����W�̐΂̐F�𒲂ׂ�B
	const std::vector<Point>& getMovablePos() const; // �΂�łĂ���W������vector��Ԃ��B
	std::vector<Disc> getUpdate() const; // ���O�̎�ŕԂ��ꂽ�΂�����vector��Ԃ��B
	Color getCurrentColor() const; // ���݂̎�Ԃ̐F��Ԃ��B
	unsigned int getTurns() const; // ���݂̎萔��Ԃ��B

	// �e�X�g�p���\�b�h
	void BoardTestPrint();
	int BoardTestMain();

};


#endif /* BOARD_H_ */
