#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <string>
#include <vector>

struct HighScore {
    std::string name;
    int score;
};

void addHighScore(const std::string& playerName, int playerScore);
void displayTop10HighScores();

#endif 
