#include "Button.hpp"

Button::Button(sf::Vector2f position, int number, const sf::Font& font) : position(position), number(number) {
    initSetting();
    text.setFont(font);
}

void Button::initSetting() {

    // 버튼 이미지 생성
    if (!texture.loadFromFile("assets/images/button.jpg")) {
        std::cout << "버튼 이미지 불러오기 실패";
    }

    button.setTexture(texture);
    button.setPosition(position);

    text.setString(std::to_string(number));
    text.setCharacterSize(48);
    text.setFillColor(sf::Color::White);
    text.setPosition(position + sf::Vector2f(10.f, 0.f));
}


/**
 * @brief 버튼이 클릭되었는지 확인하는 함수
 * @details 버튼의 경계와 마우스 포인터의 위치를 비교하여서, 마우스 포인터가 경계 내부일 경우 참
 */
bool Button::checkIsClicked(sf::Vector2f mousePointerPos) {
    return button.getGlobalBounds().contains(mousePointerPos);
}
