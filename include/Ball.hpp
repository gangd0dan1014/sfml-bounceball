#pragma once

#include <SFML/Graphics.hpp>

#include "StageManager.hpp"

class Ball : public sf::Drawable, public sf::Transformable {

    private:
        sf::Vector2f initPosition;
        sf::Vector2f position;
        sf::Vector2f velocity;
        sf::Vector2f gravity;

        sf::CircleShape ball;

        void initSetting();

        void checkCollisionWithBlock(float dt);

        void setVelocityX(float velocityX);
        void setVelocityY(float velocityY);

        void restartStage();

        Stage* stage;
        int initStarCount;

        bool isStraight;
    public:
        void updatePosition(float dt);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;
        Ball();

        void moveLeft(float dt);
        void moveRight(float dt);
        void moveStraight(float dt);

        void setStage(Stage* stage);

        void setIsStraight(bool isStraight);
        bool getIsStraight();
};