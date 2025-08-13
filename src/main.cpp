#include <SFML/Graphics.hpp>
#include <vector>

#include "Block.hpp"
#include "Ball.hpp"
#include "StageManager.hpp"


int main() {

    sf::RenderWindow window;
    window.create(sf::VideoMode(2048.f, 1024.f), "Bounce Ball");

    sf::Event event;
    sf::Clock clock;
    float deltaTime;

    BackGround backGround = BackGround();
    Ball ball = Ball(sf::Vector2f(1024.f, 512.f));

    StageManager stageManager = StageManager("assets/stages.json");
    Stage& stage = stageManager.getStage(0);

    while (window.isOpen()) {

        deltaTime = clock.restart().asSeconds();

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        // 좌우 이동 부분
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            ball.moveLeft(deltaTime);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            ball.moveRight(deltaTime);
        }


        window.clear();

        // 배경 그리기
        window.draw(backGround);

        // 맵 그리기
        for (const auto& block : stage.blockList) {
            window.draw(*block);
        }

        ball.updatePosition(deltaTime);
        window.draw(ball);

        window.display();
    }
}