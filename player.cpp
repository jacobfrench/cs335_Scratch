#include <string>
#include <iostream>

using namespace std;

class Player {

  private:
    //we may not even need posX
    //for pong
    std::string name;
    int score;




  public:
    float posX1;
    float posY1;
    float posX2;
    float posY2;

    Player();
    Player(std::string name);

    float        getPosY();
    std::string  getName();
    int          getScore();

    void setPosY(int posY);
    void setName(std::string name);
    void setScore(int score);
    void movePaddleUp();
};



Player::Player() {
  name = "Default";
  posX1 = 0.6;
  posY1 = 0.3;
  posX2 = 0.5;
  posY2 = 0.8;
}

Player::Player(std::string name) {
  this->name = name;
  score = 0;
}

string Player::getName() {
  return name;
}

int Player::getScore() {
  return score;
}



void Player::setName(string name) {
  this->name = name;
}

void Player::setScore(int score) {
  this->score = score;
}
