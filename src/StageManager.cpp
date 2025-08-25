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
    loadStageClearStatus();
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
        stage.lock = true;
        stage.isClear = false;

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
                    std::make_unique<Straight>(block["x"].get<int>(), block["y"].get<int>(), block["isRight"].get<bool>())
                );
            }
            else if (block["type"].get<std::string>().compare("Needle") == 0) {
                stage.blockList.push_back(
                    std::make_unique<Needle>(block["x"].get<int>(), block["y"].get<int>(), block["width"].get<int>())
                );
            }
            else if (block["type"].get<std::string>().compare("Breakable") == 0) {
                stage.blockList.push_back (
                    std::make_unique<Breakable>(block["x"].get<int>(), block["y"].get<int>())
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
    stage.lock = stageList[stageNumber - 1].lock;
    stage.isClear = stageList[stageNumber - 1].isClear;

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
                std::make_unique<Straight>(block["x"].get<int>(), block["y"].get<int>(), block["isRight"].get<bool>())
            );
        }
        else if (block["type"].get<std::string>().compare("Needle") == 0) {
            stage.blockList.push_back(
                std::make_unique<Needle>(block["x"].get<int>(), block["y"].get<int>(), block["width"].get<int>())
            );
        }
        else if (block["type"].get<std::string>().compare("Breakable") == 0) {
            stage.blockList.push_back (
                std::make_unique<Breakable>(block["x"].get<int>(), block["y"].get<int>())
            );
        }
    }

    stageList[stageNumber - 1] = std::move(stage);
}

Stage& StageManager::getStage(int stageNumber) {
    return stageList[stageNumber - 1];
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

void StageManager::markAsCleared(int stageNumber) {
    buttonList[stageNumber - 1]->markAsCleared();
}

/**
 * @brief 스테이지 클리여 여부를 반영하는 함수
 * @details 게임을 시작할 때, txt 파일을 읽어서 현재 스테이지의 개방 여부를 반영
 */
void StageManager::loadStageClearStatus() {
    std::ifstream stageStatusFile("assets/stageStatus.txt");
    int index = 0;

    if (stageStatusFile.is_open()) {
        int lock, clear;
        while (stageStatusFile >> lock >> clear) {
            if (lock) {
                stageList[index].lock = true;
                stageList[index].isClear = false;
            }
            else {
                stageList[index].lock = false;
                if (clear) {    // 개방 O && 클리어 O
                    stageList[index].isClear = true;
                    buttonList[index]->markAsCleared();
                }
                else {          // 개방 O && 클리어 X
                    stageList[index].isClear = false;
                }
            }
            index++;
        }
    }
    else {
        std::cout << "File Open Error";
    }
}

/**
 * @brief 스테이지 클리어 여부를 확인하는 함수
 * @details 게임을 끌 때마다, txt 파일에 stage 클리어 여부를 저장
 */
void StageManager::updateStageClearStatus() {
    std::ofstream stageStatusFile("assets/stageStatus.txt");

    if (stageStatusFile.is_open()) {
        for (const auto& stage : stageList) {
            if (stage.lock) {
                stageStatusFile << 1 << ' ' << 0 << std::endl;
            }
            else {
                if (stage.isClear) {
                    stageStatusFile << 0 << ' ' << 1 << std::endl;
                }
                else {
                    stageStatusFile << 0 << ' ' << 0 << std::endl;
                }
            }
        }
    }
    else {
        std::cout << "File Open Error";
    }
}

void StageManager::unlockStage(int stageNumber) {
    if (stageNumber == STAGE_COUNT) {
        return;
    }
    else {
        stageList[stageNumber].lock = false;
    }
}
