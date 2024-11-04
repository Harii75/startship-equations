#include "Player.h" 
#include "Buff.h"   
#include <iostream>

void applyBuff(Player& player, const Buff& buff) {
    if (buff.name == "Calculation Focus") {

        std::cout << "Buff applied: Calculation Focus - Increased accuracy by 0.1.\n";
    } else if (buff.name == "Quick Reflexes") {

        std::cout << "Buff applied: Quick Reflexes - Reduced calculation time by 5 seconds.\n";
    } else if (buff.name == "Math Mastery") {

        std::cout << "Buff applied: Math Mastery - Gained 10 knowledge points.\n";
    } else if (buff.name == "Adaptive Learning") {

        std::cout << "Buff applied: Adaptive Learning - Increased knowledge by 5.\n";
    } else if (buff.name == "Enhanced Logic") {

        std::cout << "Buff applied: Enhanced Logic - Further increased accuracy by 0.2.\n";
    } else if (buff.name == "Mathematical Intuition") {

        std::cout << "Buff applied: Mathematical Intuition - Further reduced calculation time by 3 seconds.\n";
    } else if (buff.name == "Precision Calculation") {

        std::cout << "Buff applied: Precision Calculation - Slightly increased accuracy.\n";
    } else if (buff.name == "Golden Ratio") {

        std::cout << "Buff applied: Golden Ratio - Gained 50 gold.\n";
    } else if (buff.name == "Mental Fortitude") {

        std::cout << "Buff applied: Mental Fortitude - Increased ship health by 25.\n";
    } else if (buff.name == "Astro Calculus") {

        std::cout << "Buff applied: Astro Calculus - Gained 20 knowledge points.\n";
    } else if (buff.name == "Intuitive Math") {

        std::cout << "Buff applied: Intuitive Math - Increased knowledge by 10.\n";
    } else {
        std::cout << "Buff applied: " << buff.name << " - No specific effect defined.\n";
    }
}
