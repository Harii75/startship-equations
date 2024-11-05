#include <cassert>
#include <iostream>
#include <exception>
#include <sstream>
#include "Player.h"
#include "ShipManager.h"
#include "InventoryManager.h"
#include "MainMenu.h"
#include "test.h"
#include "Utils.h"

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

        std::cout << testName << " - [ Passed ]" << std::endl;
        return true;
    } catch (const std::exception& e) {
        restoreOutput(std::cout, originalBuffer);
        std::cout << testName << " - [ Failed ]: " << e.what() << std::endl;
    } catch (...) {
        restoreOutput(std::cout, originalBuffer);
        std::cout << testName << " - [ Failed ]: Unknown error" << std::endl;
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

    ship.equip_weapon(20);
    assert(ship.damage == 70);
}

void testInventoryManager() {
    InventoryManager inventory;

    assert(inventory.items.empty());

    inventory.add_item("Laser Gun", "Weapon");
    assert(!inventory.items.empty());

    inventory.sell_item("Laser Gun");
    assert(inventory.items.empty());

    inventory.add_item("Plasma Shield", "Armor");
    inventory.add_item("Nano Medkit", "Medical");

    assert(inventory.items.size() == 2);
    assert(inventory.can_trade());
}

void testPlayer() {
    Player player("TestCaptain");

    assert(player.name == "TestCaptain");
    assert(player.gold == 500);
    assert(player.level == 1);
    assert(player.xp == 0);
    assert(player.xpTreshold == 240);
    assert(player.knowledge == 0);
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

void runTests() {
    std::cout << "Running tests...\n";

    runTestFunction(testShipManager, "ShipManager Test");
    runTestFunction(testInventoryManager, "InventoryManager Test");
    runTestFunction(testPlayer, "Player Test");

    std::cout << "All tests completed!" << std::endl;
    waitForKeypress(); 
    main_menu();
}
