#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

class Block : public sf::Drawable, public sf::Transformable {

    private:

        const int BLOCK_SIZE = 64;

        void initSetting();

        void createRenderTexture();

        sf::Sprite sprite;
        sf::RenderTexture renderTexture;
        sf::Texture texture;

        sf::Vector2f position;
        sf::Vector2i blockCount;

        std::string imageRelativePath;
    public:
        Block(int x, int y, int width, int height, std::string image);

        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

};

class BackGround : public Block {
    public:
        BackGround() : Block(0,0,32,16,"assets/images/background.jpg") {}
};