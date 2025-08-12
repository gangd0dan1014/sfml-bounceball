#pragma once

#include <SFML/Graphics.hpp>

class Ball : public sf::Drawable, public sf::Transformable {

    private:
        sf::Vector2f initPosition;
        sf::Vector2f position;
        sf::Vector2f velocity;
        sf::Vector2f gravity;

        sf::CircleShape ball;

        void initSetting();

    public:
        void updatePosition(float dt);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        Ball(sf::Vector2f startPosition);

        void moveLeft(float dt);
        void moveRight(float dt);
};