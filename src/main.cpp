#include <SFML/Graphics.hpp>
#include "Block.hpp"
#include "Ball.hpp"

#include <vector>

int main() {

    sf::RenderWindow window;
    window.create(sf::VideoMode(2048.f, 1024.f), "Bounce Ball");

    sf::Event event;
    sf::Clock clock;
    float deltaTime;

    BackGround backGround = BackGround();
    Ball ball = Ball(sf::Vector2f(1024.f, 512.f));

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
        window.draw(backGround);

        ball.updatePosition(deltaTime);
        window.draw(ball);

        window.display();
    }
}