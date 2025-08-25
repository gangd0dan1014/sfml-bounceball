#include <SFML/Graphics.hpp>
#include <vector>

#include "Block.hpp"
#include "Ball.hpp"
#include "StageManager.hpp"

/*
    해야할 일
    1. 스테이지 추가하는 중
    2. 클리어 여부 기록 내용?? txt 활용할까
*/

int main() {

    sf::RenderWindow window;
    window.create(sf::VideoMode(2048.f, 1024.f), "Bounce Ball");

    sf::Event event;
    sf::Clock clock;
    float deltaTime;

    BackGround backGround = BackGround();
    sf::Text text;
    sf::Font font;
    font.loadFromFile("assets/fonts/ARLRDBD.ttf");
    text.setFont(font);
    text.setCharacterSize(36);
    text.setFillColor(sf::Color::Black);
    text.setString("Press ESC to exit");
    text.setPosition(sf::Vector2f(0,0));
    
    
    StageManager stageManager = StageManager("assets/stages.json");
    Stage* stage = &(stageManager.getStage(0));
    
    Ball ball = Ball();
    ball.setStage(stage);

    while (window.isOpen()) {

        deltaTime = clock.restart().asSeconds();

        window.clear(sf::Color::White);

        // 이벤트 처리 부분
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                stageManager.updateStageClearStatus();
                window.close();
            }
            else if (!stageManager.getInStage() && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                stageManager.updateStageClearStatus();
                window.close();
            }
            else if (!stageManager.getInStage() && (event.type == sf::Event::MouseButtonPressed)) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

                    for (const auto& temp : stageManager.getButtonList()) {
                        if ((*temp).checkIsClicked(worldPos)) {
                            stage = &(stageManager.getStage((*temp).getStageNumber()));
                            if (stage->lock) {
                                continue;
                            }
                            ball.setStage(stage);
                            stageManager.setInStage(true);
                        }
                    }
                }
            }
            else if(stageManager.getInStage() && (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)) {
                stageManager.setInStage(false);
                stageManager.resetStage(stage->stageNumber);
            }
        }

        // 로직 부분
        if (stageManager.getInStage()) {

            // 좌우 이동 부분
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                ball.moveLeft(deltaTime);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                ball.moveRight(deltaTime);
            }
            
            // Straight 이동 부분
            if (ball.getIsStraight()) {
                ball.moveStraight(deltaTime);
            }

            // 배경 그리기
            window.draw(backGround);
            // 맵 그리기
            for (const auto& block : (*stage).blockList) {
                if ((*block).getDrawable()) {
                    window.draw(*block);
                }
            }

            // 공의 위치 업데이트
            ball.updatePosition(deltaTime);
            window.draw(ball);

            // stage에 남은 별이 없을 경우 종료
            if (stage->starCount == 0) {
                stageManager.setInStage(false);
                stageManager.resetStage(stage->stageNumber);
                stageManager.markAsCleared(stage->stageNumber);
                stageManager.unlockStage(stage->stageNumber);
                stage->isClear = true;
            }
        }
        else {
            for (const auto& temp : stageManager.getButtonList()) {
                window.draw(*temp);
            }
        }
        window.draw(text);
        window.display();
    }
}
