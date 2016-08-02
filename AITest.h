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
			// パス
			cout << "あなたはパスです。" << endl;
			board.pass();
			return;
		}

		while(true){

			cout << "手を入力してください(U:取消/X:終了):";
			string in;
			cin >> in;

			if(in == "U" || in == "u") throw UndoException();
			if(in == "X" || in == "x") throw ExitException();

			Point p;
			try{
				p = Point(in);
			}catch(invalid_argument& e){
				cout << "正しい形式で入力してください！" << endl;
				continue;
			}

			bool rcd;
			rcd = board.move(p);
			if(rcd == false){
				cout << "そこには置けません！" << endl;
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

		cout << "コンピュータが思考中..." << endl;
		sleep(3);

		Ai->move(board);
		cout << "完了" << endl;
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
			cout << "ゲーム終了" << endl;
			board.BoardTestPrint();
			cout << "黒石" << board.countDisc(BLACK) << " ";
			cout << "白石" << board.countDisc(WHITE) << endl;
			return 0;
		}

		// プレーヤーの交代
		if(current_player == 0){
			current_player = 1;
		}
		else{current_player = 0;}
		// current_player = ++current_player % 2;

	}

	return 0;

}

#endif /* AITEST_H_ */
