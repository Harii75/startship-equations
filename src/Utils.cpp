#include "Utils.h"
#include <iostream>
#include <ctime> 
#include <conio.h>

void waitForKeypress() {
    std::cout << "\nPress any button to continue...";
    _getch();
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void loadingEffect(int durationInSeconds, const std::string& message) {
    std::cout << message;
    for (int i = 0; i < durationInSeconds; ++i) {
        std::cout << ".";
        std::cout.flush();
        clock_t end_time = clock() + CLOCKS_PER_SEC;
        while (clock() < end_time) {}
    }
}
