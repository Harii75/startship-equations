#include "MainMenu.h"
#include "GameProgression.h"
#include "Scenarios.h"
#include "ShipMenu.h"
#include "TradeMenu.h"
#include "Player.h"
#include "Utils.h"
#include "FileLoader.h"
#include <iostream>
#include <ctime>

int main() {
    srand(static_cast<unsigned int>(time(0))); // Seed random number generator
    main_menu();
    return 0;
}
