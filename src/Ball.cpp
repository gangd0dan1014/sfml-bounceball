#include "Ball.hpp"
#include "Block.hpp"

Ball::Ball(sf::Vector2f startPosition) 
    : position(startPosition), initPosition(startPosition), velocity(0.f, 0.f), gravity(0.f, 490.f) {
    initSetting();
}

/**
 * @brief 공 객체에 대한 기본 설정
 * @details 공 객체 크기, 색, 초기 위치 설정
 */
void Ball::initSetting() {

    // sf::CircleShape 기본 세팅
    ball.setRadius(16.f);
    ball.setOrigin(sf::Vector2f(16.f, 16.f));
    ball.setFillColor(sf::Color::Cyan);
    ball.setPosition(position);
}

/**
 * @param dt 한 프레임이 지나가는 시간
 * @brief 공의 위치를 업데이트 하는 함수
 * @details 중력과 속도를 기반으로 공의 위치를 바꿔주는 함수 
 */
void Ball::updatePosition(float dt) {

    sf::Vector2f initVelocity = velocity;

    velocity.y += gravity.y * dt;
    position.y += (initVelocity.y * dt) + 0.5f * dt * dt * gravity.y;


    if (position.y < 0 || position.y >= 1024.f || position.x < 0 || position.x > 2048) {
        position.x = initPosition.x;
        position.y = initPosition.y;
        velocity.y = 0.f;
    }
    ball.setPosition(position);
}

/**
 * @param target 공이 그려질 대상
 * @param states 공의 현재 상태
 * @brief 공을 화면에 그리는 함수
 * @details 공의 현재 상태를 반영하여 화면에 그림
 */
void Ball::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    states.transform *= getTransform();
    target.draw(ball, states);
}

/**
 * @param dt 프레임 시간
 * @brief 왼쪽으로 이동하는 함수
 * @details 좌표만 바꾸고 실질적인 이동은 updatePosition 함수에서 이루어짐
 */
void Ball::moveLeft(float dt) {
    velocity.x = -250.f;
    position.x += dt * velocity.x;
}

/**
 * @param dt 프레임 시간
 * @brief 오른으로 이동하는 함수
 * @details 좌표만 바꾸고 실질적인 이동은 updatePosition 함수에서 이루어짐
 */
void Ball::moveRight(float dt) {
    velocity.x = 250.f;
    position.x += dt * velocity.x;
}