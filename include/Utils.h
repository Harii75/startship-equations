#ifndef UTILS_H
#define UTILS_H

#include <string>
#include "Player.h"

void waitForKeypress();
void clearScreen();
void loadingEffect(int durationInSeconds, const std::string& message);
void endGame(Player& player);
void finishGame(Player& player);

#endif 
