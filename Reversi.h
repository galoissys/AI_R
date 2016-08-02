/*
 * Reversi.h
 *
 *  Created on: 2014/05/14
 *      Author: matsumoto
 */

#ifndef REVERSI_H_
#define REVERSI_H_

#include <iostream>
#include <string>
#include <stdexcept>
#include <sstream>

using namespace std;

/*
 * 座標を表すクラス
 */
class Point{

public:
	int x;
	int y;

	Point(){
		this->x = 0;
		this->y = 0;
	}

	Point(int x, int y){
		this->x = x;
		this->y = y;
	}

	//テスト用メソッド
	Point(std::string coordstr) throw(std::invalid_argument){

		if(coordstr.length() < 2) throw std::invalid_argument(
				"The argument must be Reversi style coordinates!");

		this->x = coordstr[0] - 'a' +1;
		this->y = coordstr[1] - '1' +1;
	}

	operator std::string() const{

		std::ostringstream oss;
		oss << static_cast<char>('a' +x-1) << static_cast<char>('1' +y-1);

		return oss.str();
	}

};


/*
 * 色を表す定数
 */
typedef int Color;
const Color EMPTY = 0;
const Color WHITE = -1;
const Color BLACK = 1;
const Color WALL  = 2;

/*
 * 石を表すクラス
 */
class Disc : public Point{

public:
	Color color;

	Disc() : Point(0, 0){
		color = EMPTY;
	}

	Disc(int x, int y, Color color) : Point(x, y){
		this->color = color;
	}
};

/*
 * 各色の石の数
 */
template<typename T> class ColorStorage{

private:
	T data[3];
public:
	T& operator[](Color color){
		return data[color + 1];
	}

	const T& operator[](Color color) const{
		return data[color + 1];
	}
};

#endif /* REVERSI_H_ */
