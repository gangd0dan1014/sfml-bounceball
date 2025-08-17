#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class Button : public sf::Drawable, public sf::Transformable {
    private:
        int number;
        sf::Sprite button;
        sf::Texture texture;

        sf::Text text;

        void initSetting();

        sf::Vector2f position;
    public:
        Button(sf::Vector2f position, int number, const sf::Font& font);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
            states.transform *= getTransform();
            target.draw(button, states);
            target.draw(text, states);
        }
        bool checkIsClicked(sf::Vector2f mousePointerPos);
        int getStageNumber() {
            return this->number;
        }
};