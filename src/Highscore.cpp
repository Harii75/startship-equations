#include "HighScore.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>

const std::string HIGH_SCORE_FILE = "highscores.txt";

// Function to add a high score to the file
void addHighScore(const std::string& playerName, int playerScore) {
    std::vector<HighScore> highScores;

    // Read existing high scores
    std::ifstream inFile(HIGH_SCORE_FILE);
    if (inFile.is_open()) {
        std::string line;
        while (std::getline(inFile, line)) {
            std::istringstream iss(line);
            std::string name;
            int score;
            if (iss >> name >> score) {
                highScores.push_back({name, score});
            }
        }
        inFile.close();
    }

    // Add the current player's high score
    highScores.push_back({playerName, playerScore});

    // Sort the scores in descending order
    std::sort(highScores.begin(), highScores.end(), [](const HighScore& a, const HighScore& b) {
        return a.score > b.score; // Descending order
    });

    // Write the updated scores back to the file
    std::ofstream outFile(HIGH_SCORE_FILE);
    if (outFile.is_open()) {
        for (const auto& entry : highScores) {
            outFile << entry.name << " " << entry.score << "\n";
        }
        outFile.close();
    }
}

// Function to display the top 10 high scores
void displayTop10HighScores() {
    std::vector<HighScore> highScores;

    // Read the high scores from the file
    std::ifstream inFile(HIGH_SCORE_FILE);
    if (inFile.is_open()) {
        std::string line;
        while (std::getline(inFile, line)) {
            std::istringstream iss(line);
            std::string name;
            int score;
            if (iss >> name >> score) {
                highScores.push_back({name, score});
            }
        }
        inFile.close();
    }

    // Sort the scores in descending order
    std::sort(highScores.begin(), highScores.end(), [](const HighScore& a, const HighScore& b) {
        return a.score > b.score; // Descending order
    });

    // Display the top 10 high scores
    std::cout << "Top 10 High Scores:\n\n";
    int count = 0;
    for (const auto& entry : highScores) {
        std::cout << count + 1 << ". " << entry.name << ": " << entry.score << "\n";
        if (++count == 10) break;
    }

    if (count == 0) {
        std::cout << "No high scores available.\n";
    }
}
