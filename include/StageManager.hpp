#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

#include "json.hpp"
#include "Block.hpp"
#include "Button.hpp"

/**
 * @brief 하나의 스테이지에 대한 정보를 담은 구조체
 */
typedef struct Stage {
    // 스테이지 번호
    int stageNumber;

    // 플레이어 시작 위치
    sf::Vector2f startPosition;

    // 맵을 구성하는 블럭을 담은 벡터
    std::vector<std::unique_ptr<Block>> blockList;

    // 맵에 존재하는 별의 개수
    int starCount;
} Stage;

class StageManager {
    private:

        const int STAGE_COUNT = 21;

        std::vector<Stage> stageList;
        std::vector<std::unique_ptr<Button>> buttonList;
        
        void loadFromJson();
        std::string jsonRelativePath;
        
        void initSetting();
        
        bool inStage;

        sf::Font globalFont;
    public:
        StageManager(std::string json);
        Stage& getStage(int stageNumber);
        
        void setInStage(bool inStage);
        bool getInStage();

        std::vector<std::unique_ptr<Button>>& getButtonList();

        void resetStage(int stageNumber);
};

