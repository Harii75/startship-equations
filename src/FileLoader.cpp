#include "FileLoader.h"
#include "tinyxml2.h"
#include <iostream>
#include <fstream>

bool FileLoader::verifyFileAccess(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file at " << filePath << std::endl;
        return false;
    }
    file.close();
    return true;
}

std::vector<Weapon> FileLoader::loadWeaponsFromXML(const std::string& filePath) {
    std::vector<Weapon> weapons;

    if (!verifyFileAccess(filePath)) return weapons;

    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filePath.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Error loading XML file: " << doc.ErrorStr() << std::endl;
        return weapons;
    }

    tinyxml2::XMLElement* weaponsElement = doc.FirstChildElement("weapons");
    if (!weaponsElement) {
        std::cerr << "No <weapons> element found in the XML file." << std::endl;
        return weapons;
    }

    for (tinyxml2::XMLElement* weaponElement = weaponsElement->FirstChildElement("weapon");
         weaponElement != nullptr; weaponElement = weaponElement->NextSiblingElement("weapon")) {

        Weapon weapon;

        const char* name = weaponElement->FirstChildElement("name")->GetText();
        if (name) {
            weapon.name = name;
        }

        weaponElement->FirstChildElement("damage")->QueryIntText(&weapon.damage);

        const char* rarity = weaponElement->FirstChildElement("rarity")->GetText();
        if (rarity) {
            weapon.rarity = rarity;
        }

        weapons.push_back(weapon);
    }

    return weapons;
}

std::vector<Stage> FileLoader::loadStagesFromXML(const std::string& filePath) {
    std::vector<Stage> stages;

    if (!verifyFileAccess(filePath)) return stages;

    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filePath.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Error loading XML file: " << doc.ErrorStr() << std::endl;
        return stages;
    }

    tinyxml2::XMLElement* stagesElement = doc.FirstChildElement("game")->FirstChildElement("stages");
    if (!stagesElement) {
        std::cerr << "No <stages> element found in the XML file." << std::endl;
        return stages;
    }

    for (tinyxml2::XMLElement* stageElement = stagesElement->FirstChildElement("stage");
         stageElement != nullptr; stageElement = stageElement->NextSiblingElement("stage")) {

        Stage stage;
        stage.id = stageElement->IntAttribute("id");
        stage.name = stageElement->FirstChildElement("name")->GetText();
        stage.description = stageElement->FirstChildElement("description")->GetText();

        // Parse levels
        for (tinyxml2::XMLElement* levelElement = stageElement->FirstChildElement("levels")->FirstChildElement("level");
             levelElement != nullptr; levelElement = levelElement->NextSiblingElement("level")) {

            Level level;
            level.name = levelElement->FirstChildElement("name")->GetText();
            level.difficulty = levelElement->FirstChildElement("difficulty")->GetText();

            // Parse enemies
            for (tinyxml2::XMLElement* enemyElement = levelElement->FirstChildElement("enemies")->FirstChildElement("enemy");
                 enemyElement != nullptr; enemyElement = enemyElement->NextSiblingElement("enemy")) {

                Enemy enemy;
                enemy.type = enemyElement->FirstChildElement("type")->GetText();
                enemyElement->FirstChildElement("health")->QueryIntText(&enemy.health);
                enemyElement->FirstChildElement("damage")->QueryIntText(&enemy.damage);

                level.enemies.push_back(enemy);
            }

            // Parse rewards
            tinyxml2::XMLElement* rewardElement = levelElement->FirstChildElement("rewards");
            rewardElement->FirstChildElement("gold")->QueryIntText(&level.rewards.gold);
            rewardElement->FirstChildElement("xp")->QueryIntText(&level.rewards.xp);
            if (rewardElement->FirstChildElement("special_item")) {
                level.rewards.special_item = rewardElement->FirstChildElement("special_item")->GetText();
            }

            // Parse conditions
            for (tinyxml2::XMLElement* conditionElement = levelElement->FirstChildElement("conditions")->FirstChildElement("condition");
                 conditionElement != nullptr; conditionElement = conditionElement->NextSiblingElement("condition")) {

                Condition condition;
                condition.condition_value = conditionElement->IntText();
                level.conditions.push_back(condition);
            }

            stage.levels.push_back(level);
        }

        stages.push_back(stage);
    }

    return stages;
}

std::vector<Buff> FileLoader::loadBuffsFromXML(const std::string& filePath) {
    std::vector<Buff> buffs;

    if (!verifyFileAccess(filePath)) return buffs;

    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filePath.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Error loading XML file: " << doc.ErrorStr() << std::endl;
        return buffs;
    }

    tinyxml2::XMLElement* buffsElement = doc.FirstChildElement("buffs");
    if (!buffsElement) {
        std::cerr << "No <buffs> element found in the XML file." << std::endl;
        return buffs;
    }

    for (tinyxml2::XMLElement* buffElement = buffsElement->FirstChildElement("buff");
         buffElement != nullptr; buffElement = buffElement->NextSiblingElement("buff")) {

        Buff buff;
        buffElement->FirstChildElement("id")->QueryIntText(&buff.id);
        buff.name = buffElement->FirstChildElement("name")->GetText();
        buffElement->FirstChildElement("duration")->QueryIntText(&buff.duration);
        buff.effect = buffElement->FirstChildElement("effect")->GetText();

        buffs.push_back(buff);
    }

    return buffs;
}

std::vector<NPC> FileLoader::loadNpcsFromXML(const std::string& filePath) {
    std::vector<NPC> npcs;

    if (!verifyFileAccess(filePath)) return npcs;

    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filePath.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Error loading XML file: " << doc.ErrorStr() << std::endl;
        return npcs;
    }

    tinyxml2::XMLElement* npcsElement = doc.FirstChildElement("npcs");
    if (!npcsElement) {
        std::cerr << "No <npcs> element found in the XML file." << std::endl;
        return npcs;
    }

    for (tinyxml2::XMLElement* npcElement = npcsElement->FirstChildElement("npc");
         npcElement != nullptr; npcElement = npcElement->NextSiblingElement("npc")) {

        NPC npc;
        npcElement->FirstChildElement("id")->QueryIntText(&npc.id);
        npcElement->FirstChildElement("stage")->QueryIntText(&npc.stage);
        npc.name = npcElement->FirstChildElement("name")->GetText();
        npc.quest = npcElement->FirstChildElement("Quest")->GetText();
        npc.reward = npcElement->FirstChildElement("Reward")->GetText();
        npc.equation = npcElement->FirstChildElement("Equation")->GetText();
        npc.answer = npcElement->FirstChildElement("Answer")->GetText();

        npcs.push_back(npc);
    }

    return npcs;
}
