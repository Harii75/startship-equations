#ifndef FILELOADER_H
#define FILELOADER_H

#include <string>
#include <vector>
#include "Weapon.h"
#include "Stage.h"
#include "Buff.h"
#include "NPC.h"

class FileLoader {
public:
    static std::vector<Weapon> loadWeaponsFromXML(const std::string& filePath);
    static std::vector<Stage> loadStagesFromXML(const std::string& filePath);
    static std::vector<Buff> loadBuffsFromXML(const std::string& filePath);
    static std::vector<NPC> loadNpcsFromXML(const std::string& filePath);

private:
    static bool verifyFileAccess(const std::string& filePath);
};

#endif 
