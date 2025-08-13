#include <iostream>
#include <fstream>

#include "StageManager.hpp"


using json = nlohmann::json;

StageManager::StageManager(std::string json) : jsonRelativePath(json) {
    loadFromJson();
}

/**
 * @brief json으로부터 맵의 정보를 가져오는 함수
 */
void StageManager::loadFromJson() {
    std::ifstream file(jsonRelativePath);
    json j;
    file >> j;

    for (const auto& stageJson : j["stages"]) {
        Stage stage;
        stage.stageNumber = stageJson["stageNumber"].get<int>();
        stage.startPosition.x = stageJson["startX"].get<int>() * 64.f;
        stage.startPosition.y = stageJson["startY"].get<int>() * 64.f;

        for (const auto& block : stageJson["blocks"]) {
            if (block["type"].get<std::string>().compare("Stone") == 0) {
                stage.blockList.push_back(
                    std::make_unique<Stone>(block["x"].get<int>(), block["y"].get<int>(), block["width"].get<int>(), block["height"].get<int>())
                );
            }
            else if (block["type"].get<std::string>().compare("Star") == 0) {
                stage.blockList.push_back(
                    std::make_unique<Star>(block["x"].get<int>(), block["y"].get<int>())
                );
            }
            else if (block["type"].get<std::string>().compare("Jump") == 0) {
                stage.blockList.push_back(
                    std::make_unique<Jump>(block["x"].get<int>(), block["y"].get<int>())
                );
            }
            else if (block["type"].get<std::string>().compare("Straight") == 0) {
                stage.blockList.push_back(
                    std::make_unique<Straight>(block["x"].get<int>(), block["y"].get<int>())
                );
            }
        }

        // stage에 대한 소유권은 StageManager에서 관리
        stageList.push_back(std::move(stage));
    }
}

Stage& StageManager::getStage(int stageNumber) {
    return stageList[stageNumber];
}