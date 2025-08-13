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
        sf::FloatRect boundary;

        bool drawable;

        sf::Vector2f position;
        sf::Vector2i blockCount;

        std::string imageRelativePath;
    public:
        Block(int x, int y, int width, int height, std::string image);

        void draw(sf::RenderTarget& target, sf::RenderStates states) const;

        sf::FloatRect getBoundary();

        void setDrawable(bool drawable) {
            this->drawable = drawable;
        }
        bool getDrawable() {
            return this->drawable;
        }

};

// 배경 블럭
class BackGround : public Block {
    public:
        BackGround() : Block(0,0,32,16,"assets/images/background.jpg") {}
};

// 기본 돌 블럭
class Stone : public Block {
    public:
        Stone(int x, int y, int width, int height) : Block(x,y,width,height, "assets/images/stone.jpg") {}
};

class Jump : public Block {
    public:
        Jump(int x, int y) : Block(x,y,1,1,"assets/images/jump.jpg") {}
};

// 차후 수정 필요
// 가시는 반블럭 판정이라 수정 필요함
class Needle : public Block {
    public:
};

class Breakable : public Block {
    public:
        Breakable(int x, int y, int width, int height, std::string image) : Block(x,y,width,height, image) {}
};

class Straight : public Block {
    public:
        Straight(int x, int y) : Block(x,y,1,1,"assets/images/straight.jpg") {}
};

class Star : public Block {
    public:
        Star(int x, int y) : Block(x,y,1,1,"assets/images/star.png") {}
};
