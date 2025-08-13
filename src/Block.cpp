#include "Block.hpp"

/**
 * @param x 시작 x좌표
 * @param y 시작 y 좌표
 * @param width x방향 블럭의 개수
 * @param height y방향 블럭의 개수
 * @param image 블럭에 적용될 이미지 상대 경로
 * @brief 블럭 생성자
 */
Block::Block(int x, int y, int width, int height, std::string image) 
    :position(x * BLOCK_SIZE,y * BLOCK_SIZE), blockCount(width, height), imageRelativePath(image), drawable(true) {
    initSetting();
}

/**
 * @brief 블럭을 만들기 위한 기본 세팅
 * @details 블럭을 만들기 위한 함수들을 모아둔 함수
 */
void Block::initSetting() {
    if (!texture.loadFromFile(imageRelativePath)) {
        std::cout << imageRelativePath << ' ' << "이미지 로딩 실패";
    }
    createRenderTexture();
    sprite.setPosition(position);
    boundary = sprite.getGlobalBounds();
}

/**
 * @brief 블럭을 가상공간에 그리는 함수
 * @details 블럭을 가상공간에 그리고, 저장하는 함수
 */
void Block::createRenderTexture() {
    renderTexture.create(blockCount.x * BLOCK_SIZE , blockCount.y * BLOCK_SIZE);

    for (int y = 0; y < blockCount.y; y++) {
        for (int x = 0; x < blockCount.x; x++) {
            sf::Sprite temp(texture);
            temp.setPosition(x * BLOCK_SIZE, y * BLOCK_SIZE);
            renderTexture.draw(temp);
        }
    }

    renderTexture.display();
    sprite.setTexture(renderTexture.getTexture());
}

void Block::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(sprite, states);
}

sf::FloatRect Block::getBoundary() {
    return boundary;
}

