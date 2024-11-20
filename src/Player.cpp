#include "Player.h"
#include <iostream>
#include <algorithm>

Player::Player(const std::string& name) 
    : name(name), gold(50), level(1), xp(0), xpTreshold(240), knowledge(5), lastShopResetLevel(0) {
    ship = new ShipManager();
    inventory = new InventoryManager();
}

Player::~Player() {
    delete ship;
    delete inventory;
}

void Player::display_stats() {
    std::cout << "\n######################################################\n";
    std::cout << "                 Captain: " << name << "\n";
    std::cout << "   Gold: " << gold << "\t\t\tLevel: " << level << "   XP: " << xp << "/" << xpTreshold <<  "\n";
    std::cout << "   Damage: " << ship->damage <<  "\t\t\tHealth: " << ship->currentHealth <<"/" << ship->maxHealth << "\n";
    std::cout << "   Stage: [ " << currentStage+1 << " - " << currentLevel+1 << " ]\t\tKnowledge: " << knowledge << " \n";
    std::cout << "                    Score: " << highscore << " \n";
    std::cout << "######################################################\n";
}   

void Player::levelUp(int& points) {
    if (points >= xpTreshold) {
        points -= xpTreshold;
        xpTreshold = static_cast<int>(xpTreshold * 1.75);
        level++;
        std::cout << "Congratulations! You've reached level " << level << "!\n";
        std::cout << "You've got 25 gold!\n";
        gold+=25;
        ship->maxHealth = ship->maxHealth*1.12;
        ship->currentHealth = ship->currentHealth*1.12;
    }
}

void Player::canPlayerTrade() {
    canTrade = (currentLevel % 5 == 0);
}

bool Player::shouldResetShop() {
    if ((currentLevel == 1 || currentLevel == 9) && currentLevel > lastShopResetLevel) {
        lastShopResetLevel = currentLevel;  
        return true; 
    }
    return false;  
}



int Player::calculateDamage(int correctAnswer, int playerAnswer) {
    int difference = std::abs(correctAnswer - playerAnswer);
    if (difference == 0) {
        return ship->damage * 2;
    } else if (difference <= 2) {
        return ship->damage;
    } else if (difference <= 5) {
        return ship->damage / 2;
    } else {
        return 0;
    }
}

void Player::listBuffs() const {
    std::cout << "Active buff(s):\n";
    for (const auto& buff : purchasedBuffs) {
        std::cout << buff.name << " - Duration: " << buff.duration << " rounds\n";
    }
}

void Player::degradeBuffs() {
    for (auto& buff : purchasedBuffs) {
        buff.duration -= 1;
    }

    purchasedBuffs.erase(
        std::remove_if(
            purchasedBuffs.begin(),
            purchasedBuffs.end(),
            [](const Buff& buff) { return buff.duration <= 0; }
        ),
        purchasedBuffs.end()
    );
}
