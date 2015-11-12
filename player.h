#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <iostream>

using namespace std;

class Player {
	private:
		std::string name;
		int score;
	public:
		float posX1;
		float posY1;
		float posX2;
		float posY2;

		Player();
		Player(std::string name);

		float getPosY();
		std::string getName();
		int getScore();
		void setPosY(int posY);
		void setName(std::string name);
		void setScore(int score);
		void movePaddleUp();
};
#endif
