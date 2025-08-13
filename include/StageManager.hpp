#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

#include "json.hpp"
#include "Block.hpp"

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

    // 별의 개수 추가할까
} Stage;

class StageManager {
    private:
        std::vector<Stage> stageList;

        void loadFromJson();
        std::string jsonRelativePath;
    public:
        StageManager(std::string json);
        Stage& getStage(int stageNumber);
};