#include <cassert>
#include <iostream>
#include <exception>
#include <sstream>
#include <fstream>
#include "Player.h"
#include "ShipManager.h"
#include "InventoryManager.h"
#include "MainMenu.h"
#include "GameProgression.h"
#include "test.h"
#include "Utils.h"
#include "Weapon.h"
#include "Buff.h"
#include "Shop.h"
#include "TradeMenu.h"
#include "HighScore.h"

void suppressOutput(std::ostream& stream, std::ostream& backup, std::streambuf*& originalBuffer) {
    originalBuffer = stream.rdbuf();
    stream.rdbuf(backup.rdbuf());    
}

void restoreOutput(std::ostream& stream, std::streambuf* originalBuffer) {
    stream.rdbuf(originalBuffer);
}

bool runTestFunction(void (*testFunc)(), const std::string& testName) {
    std::ostringstream nullStream;
    std::streambuf* originalBuffer = nullptr;

    try {
        suppressOutput(std::cout, nullStream, originalBuffer);
        testFunc();
        restoreOutput(std::cout, originalBuffer);

        std::cout << testName << "\t - [ Passed ]" << std::endl;
        return true;
    } catch (const std::exception& e) {
        restoreOutput(std::cout, originalBuffer);
        std::cout << testName << "\t - [ Failed ]: " << e.what() << std::endl;
    } catch (...) {
        restoreOutput(std::cout, originalBuffer);
        std::cout << testName << "\t - [ Failed ]: Unknown error" << std::endl;
    }

    return false;
}

void testShipManager() {
    ShipManager ship;

    assert(ship.storage_capacity == 100);
    assert(ship.maxHealth == 100);
    assert(ship.currentHealth == 100);
    assert(ship.damage == 50);

    ship.upgrade_ship();
    assert(ship.maxHealth == 125);
    assert(ship.currentHealth == 125);

    ship.repair_ship();
    assert(ship.currentHealth == 125);
}

void testPlayer() {
    Player player("TestCaptain");

    assert(player.name == "TestCaptain");
    assert(player.gold == 500);
    assert(player.level == 1);
    assert(player.xp == 0);
    assert(player.xpTreshold == 240);
    assert(player.knowledge == 100);
    assert(player.ship != nullptr);
    assert(player.inventory != nullptr);

    int points = 300;
    player.levelUp(points);
    assert(player.level == 2);           
    assert(player.xpTreshold == 420);    
    assert(points == 60);                 

    player.currentLevel = 4;
    player.canPlayerTrade();
    assert(!player.canTrade);            

    player.currentLevel = 5;
    player.canPlayerTrade();
    assert(player.canTrade);
}

void testBuff() {
    Buff buff(1, "Test Buff", "Increases damage", 3);
    assert(buff.name == "Test Buff");
    assert(buff.effect == "Increases damage");
    assert(buff.duration == 3);
}

void testInventoryManager() {
    InventoryManager inventory;

    assert(inventory.items.empty()); 

    inventory.add_item(Weapon("Laser Gun", 20, "Rare"));
    assert(!inventory.items.empty());

    inventory.sell_item("Laser Gun");
    assert(inventory.items.empty());

    inventory.add_item(Weapon("Laser Gun", 20, "Rare"));
    inventory.add_item(Weapon("Plasma Cannon", 40, "Epic"));

    assert(inventory.items.size() == 2);
    assert(inventory.can_trade()); 
}

void testGameProgression() {
    Player player("TestCaptain");
    player.currentStage = 0;
    player.currentLevel = 0;

    std::vector<Stage> stages = {
        {
            1,
            "Test Stage",
            "A test stage description",
            {{"Level 1", "Easy", {}, {}, {}}}
        }
    };

    assert(player.currentStage == 0);
}

void testShop() {
    std::vector<Buff> buffs = {
        {1, "Quick Reflexes", "Increases calculation speed", 3},
        {2, "Damage Boost", "Increases damage output", 3},
        {3, "Gold Multiplier", "Increases gold rewards", 3}
    };

    Player player("TestCaptain");
    player.gold = 200;

    resetShopInventory(buffs);

    assert(!shopInventory.empty());
    assert(shopInventory.size() <= 3);

    for (const auto& item : shopInventory) {
        assert(item.first.name != ""); 
        assert(item.second > 0);      
    }

    int initialGold = player.gold;

    const auto& firstItem = shopInventory[0];
    if (player.gold >= firstItem.second) {
        player.gold -= firstItem.second;      
        player.addPurchasedBuff(firstItem.first); 
        shopInventory.erase(shopInventory.begin());
    }

}

void testTradeMenu() {
    Player player("TestCaptain");
    player.gold = 500;

    Weapon weapon1("Laser Gun", 20, "Rare");
    Weapon weapon2("Plasma Cannon", 40, "Epic");
    player.inventory->add_item(weapon1);
    player.inventory->add_item(weapon2);

    int initialGold = player.gold;
    int sellPrice = calculateSellPrice(weapon1);
    player.gold += sellPrice;
    player.inventory->remove_item("Laser Gun");

    assert(player.gold == initialGold + sellPrice);
    assert(player.inventory->items.size() == 1); //
}

void testHighScore() {
    addHighScore("Player1", 500);
    addHighScore("Player2", 800);
    addHighScore("Player3", 300);

    std::ifstream file("highscores.txt");
    std::string line;
    std::getline(file, line);
    assert(line.find("Player2") != std::string::npos); 
    file.close();

    std::ostringstream buffer;
    std::streambuf* old = std::cout.rdbuf(buffer.rdbuf());
    displayTop10HighScores();
    std::cout.rdbuf(old);

    assert(buffer.str().find("Player2: 800") != std::string::npos);
    assert(buffer.str().find("Player1: 500") != std::string::npos);
}


void runTests() {
    std::cout << "Running tests...\n";

    runTestFunction(testBuff, "Buff Test");
    runTestFunction(testShipManager, "ShipManager Test");
    runTestFunction(testInventoryManager, "InventoryManager Test");
    runTestFunction(testPlayer, "Player Test");
    runTestFunction(testShop, "Shop Test");
    runTestFunction(testHighScore, "HighScore Test");
    runTestFunction(testTradeMenu, "TradeMenu Test");
    

    std::cout << "\nAll tests completed!" << std::endl;
}