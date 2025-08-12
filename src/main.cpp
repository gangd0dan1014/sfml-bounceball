#include <SFML/Graphics.hpp>
#include "Block.hpp"

int main() {

    sf::RenderWindow window;
    window.create(sf::VideoMode(2048.f, 1024.f), "Bounce Ball");

    sf::Event event;

    BackGround backGround = BackGround();

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
        window.draw(backGround);
        window.display();
    }
}