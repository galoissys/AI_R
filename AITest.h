/*
 * AITest.h
 *
 *  Created on: 2014/05/19
 *      Author: matsumoto
 */

#ifndef AITEST_H_
#define AITEST_H_


#include <iostream>
#include <string>
#include <memory>

#include "AI.h"
#include "Board.h"

using namespace std;


class Player{

public:
	Player(){}
	virtual ~Player(){}
	virtual void onTurn(Board& board) = 0;

};

class UndoException{};

class ExitException{};

class GameOverException{};

class HumanPlayer : public Player{

public:
	void onTurn(Board& board){

		if(board.getMovablePos().empty()){
			// �p�X
			cout << "���Ȃ��̓p�X�ł��B" << endl;
			board.pass();
			return;
		}

		while(true){

			cout << "�����͂��Ă�������(U:���/X:�I��):";
			string in;
			cin >> in;

			if(in == "U" || in == "u") throw UndoException();
			if(in == "X" || in == "x") throw ExitException();

			Point p;
			try{
				p = Point(in);
			}catch(invalid_argument& e){
				cout << "�������`���œ��͂��Ă��������I" << endl;
				continue;
			}

			bool rcd;
			rcd = board.move(p);
			if(rcd == false){
				cout << "�����ɂ͒u���܂���I" << endl;
				continue;
			}

			if(board.isGameOver()) throw GameOverException();

			break;
		}
	}
};


class AIPlayer : public Player{

	AI* Ai;

public:
	AIPlayer() : Ai(NULL){
		Ai = new AlphaBetaAI();
	}

	~AIPlayer(){
		delete Ai;
	}

	void onTurn(Board& board){

		cout << "�R���s���[�^���v�l��..." << endl;
		sleep(3);

		Ai->move(board);
		cout << "����" << endl;
		if(board.isGameOver()) throw GameOverException();

	}
};

class AITest{
public:
	int AITestMain();
};

int AITest::AITestMain(){

	auto_ptr<Player> player[2];
	int current_player = 0;
	Board board;

	player[0].reset(new HumanPlayer());
	player[1].reset(new AIPlayer());

	while(true){

		board.BoardTestPrint();

		try{
			player[current_player]->onTurn(board);
		}catch(UndoException& e){
			do{
				board.undo();board.undo();
			}while(board.getMovablePos().empty());
			continue;
		}catch(ExitException& e){
			return 0;
		}catch(GameOverException& e){
			cout << "�Q�[���I��" << endl;
			board.BoardTestPrint();
			cout << "����" << board.countDisc(BLACK) << " ";
			cout << "����" << board.countDisc(WHITE) << endl;
			return 0;
		}

		// �v���[���[�̌��
		if(current_player == 0){
			current_player = 1;
		}
		else{current_player = 0;}
		// current_player = ++current_player % 2;

	}

	return 0;

}

#endif /* AITEST_H_ */
