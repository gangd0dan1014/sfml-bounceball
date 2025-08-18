#include <SFML/Graphics.hpp>
#include <vector>

#include "Block.hpp"
#include "Ball.hpp"
#include "StageManager.hpp"

/*
    해야할 일
    1. 블럭 추가하기
    2. 스테이지 더 만들기
    3. 공과의 상호작용 추가하기
    4. 버튼 상속 버전으로 바꾸기 like 블럭
*/

int main() {

    sf::RenderWindow window;
    window.create(sf::VideoMode(2048.f, 1024.f), "Bounce Ball");

    sf::Event event;
    sf::Clock clock;
    float deltaTime;

    BackGround backGround = BackGround();
    
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
                window.close();
            }
            else if (!stageManager.getInStage() && (event.type == sf::Event::MouseButtonPressed)) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
                    sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);

                    for (const auto& temp : stageManager.getButtonList()) {
                        if ((*temp).checkIsClicked(worldPos)) {
                            stage = &(stageManager.getStage((*temp).getStageNumber() - 1));
                            ball.setStage(stage);
                            stageManager.setInStage(true);
                        }
                    }
                }
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

            ball.updatePosition(deltaTime);
            window.draw(ball);

            // stage에 남은 별이 없을 경우 종료
            if (stage->starCount == 0) {
                stageManager.setInStage(false);
                stageManager.resetStage(stage->stageNumber);
            }
        }
        else {
            for (const auto& temp : stageManager.getButtonList()) {
                window.draw(*temp);
            }
        }

        window.display();
    }
}