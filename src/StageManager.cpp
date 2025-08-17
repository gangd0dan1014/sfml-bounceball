#include <iostream>
#include <fstream>

#include "StageManager.hpp"


using json = nlohmann::json;

StageManager::StageManager(std::string json) : jsonRelativePath(json) {
    initSetting();
}

void StageManager::initSetting() {
    loadFromJson();
    setInStage(false);

    // 버튼 폰트 생성
    if (!globalFont.loadFromFile("assets/fonts/ARLRDBD.ttf")) {
        std::cout << "버튼 폰트 불러오기 실패";
    }

    // 버튼 위치 설정
    sf::Vector2f pos(200.f, 150.f);
    for (int i = 0; i < STAGE_COUNT; i++) {
        buttonList.push_back(std::make_unique<Button>(pos, i + 1, globalFont));

        if ((i + 1) % 7 == 0) {
            pos = sf::Vector2f(200.f, pos.y + 300.f);
        } else {
            pos += sf::Vector2f(250.f, 0.f);
        }
    }
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
        stage.starCount = stageJson["starCount"].get<int>();

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

void StageManager::resetStage(int stageNumber) {
    std::ifstream file(jsonRelativePath);
    json j;
    file >> j;

    const auto& stageJson = j["stages"][stageNumber - 1];
    Stage stage;
    stage.stageNumber = stageJson["stageNumber"].get<int>();
    stage.startPosition.x = stageJson["startX"].get<int>() * 64.f;
    stage.startPosition.y = stageJson["startY"].get<int>() * 64.f;
    stage.starCount = stageJson["starCount"].get<int>();

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

    stageList[stageNumber - 1] = std::move(stage);
}

Stage& StageManager::getStage(int stageNumber) {
    return stageList[stageNumber];
}

void StageManager::setInStage(bool inStage) {
    this->inStage = inStage;
}

bool StageManager::getInStage() {
    return this->inStage;
}

std::vector<std::unique_ptr<Button>>& StageManager::getButtonList() {
    return this->buttonList;
}
