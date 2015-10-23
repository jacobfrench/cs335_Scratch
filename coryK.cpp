/*Author: Cory Kitchens
 * Programe Name: coryK.cpp
 * Purpose: Submits score to a php file
 * that reads through submitted scores and displays high scores
 *
 * */
#include <iostream>
#include <stdlib.h>

using namespace std;

void submitScore() {
  char postToWeb[] = "curl --data param1=5 http://cs.csubak.edu/~ckitchens/cs335/finalproject/index.php";
  system(postToWeb);
}

