#include "Ball.hpp"
#include "Block.hpp"

Ball::Ball() 
    : velocity(0.f, 0.f), gravity(0.f, 700.f), stage(nullptr) {
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
    ball.setFillColor(sf::Color(102,153,204));
    ball.setPosition(position);
}

/**
 * @param dt 한 프레임이 지나가는 시간
 * @brief 공의 위치를 업데이트 하는 함수
 * @details 중력과 속도를 기반으로 공의 위치를 바꿔주는 함수 
 */
void Ball::updatePosition(float dt) {

    sf::Vector2f initVelocity = velocity;

    // 중력 적용 부분
    if (!getIsStraight()) {
        velocity.y += gravity.y * dt;
        position.y += (initVelocity.y * dt) + 0.5f * dt * dt * gravity.y;
    }

    if (position.y < 0 || position.y >= 1024.f || position.x < 0 || position.x > 2048) {
        restartStage();
    }

    checkCollisionWithBlock(dt);
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
    setIsStraight(false);
    setVelocityX(-225.f);
    position.x += dt * velocity.x;
}

/**
 * @param dt 프레임 시간
 * @brief 오른으로 이동하는 함수
 * @details 좌표만 바꾸고 실질적인 이동은 updatePosition 함수에서 이루어짐
 */
void Ball::moveRight(float dt) {
    setIsStraight(false);
    setVelocityX(225.f);
    position.x += dt * velocity.x;
}

/**
 * @brief 블럭과의 충돌 및 상호작용 확인 함수
 * @param dt 프레임 시간
 */
void Ball::checkCollisionWithBlock(float dt) {

    sf::Vector2f blockCenter;
    sf::Vector2f delta;

    float overlapX, overlapY;

    for (const auto& block : (*stage).blockList) {

        blockCenter.x = (*block).getBoundary().left + ((*block).getBoundary().width / 2.f);
        blockCenter.y = (*block).getBoundary().top + ((*block).getBoundary().height / 2.f);

        delta = ball.getPosition() - blockCenter;

        overlapX = (((*block).getBoundary().width / 2.f) + ball.getRadius()) - std::abs(delta.x);
        overlapY = (((*block).getBoundary().height / 2.f) + ball.getRadius()) - std::abs(delta.y);

        if (auto stone = dynamic_cast<Stone*>(block.get())) {
            if (ball.getGlobalBounds().intersects((*block).getBoundary())) {
                if (overlapX < overlapY) {  // 좌우 충돌...더 깔끔하게 뭔가 바꿀 수 있을 거 같기도 하고
                    setIsStraight(false);
                    if (delta.x < 0) {      // 블럭 기준 왼쪽 충돌
                        position.x -= 450.f * dt;
                    }
                    else {
                        position.x += 450.f * dt;
                    }
                }
                else {                      // 상하 충돌
                    if (delta.y < 0) {      // 블럭 기준 위쪽 충돌
                        setVelocityY(-325.f);
                    }
                    else {
                        setVelocityY(100.f);
                    }
                }
            }
        }
        else if (auto jump = dynamic_cast<Jump*>(block.get())) {
            if (ball.getGlobalBounds().intersects((*block).getBoundary())) {
                if (overlapX < overlapY) {  // 좌우 충돌
                    setIsStraight(false);
                    if (delta.x < 0) {      // 블럭 기준 왼쪽 충돌
                        position.x -= 450.f * dt;
                    }
                    else {
                        position.x += 450.f * dt;
                    }
                }
                else {                      // 상하 충돌
                    if (delta.y < 0) {      // 블럭 기준 위쪽 충돌

                        // 점프 블럭이기 때문에, 속도를 다르게 설정
                        setVelocityY(-550.f);
                    }
                    else {
                        setVelocityY(100.f);
                    }
                }
            }   
        }
        else if (auto star = dynamic_cast<Star*>(block.get())) {
            if (star->getDrawable() && ball.getGlobalBounds().intersects((*block).getBoundary())) {
                star->setDrawable(false);
                stage->starCount--;
            }
        }
        else if (auto needle = dynamic_cast<Needle*>(block.get())) {
            if (ball.getGlobalBounds().intersects((*block).getBoundary())) {
                if (overlapX < overlapY) {  // 좌우 충돌
                    setIsStraight(false);
                    if (delta.x < 0) {      // 블럭 기준 왼쪽 충돌
                        position.x -= 450.f * dt;
                    }
                    else {
                        position.x += 450.f * dt;
                    }
                }
                else {                      // 상하 충돌
                    if (delta.y < 0) {      // 블럭 기준 위쪽 충돌
                        restartStage();
                    }
                    else {
                        setVelocityY(100.f);
                    }
                }
            }               
        }
        else if (auto breakable = dynamic_cast<Breakable*>(block.get())) {
            if (ball.getGlobalBounds().intersects((*block).getBoundary())) {
                if (overlapX < overlapY) {  // 좌우 충돌...더 깔끔하게 뭔가 바꿀 수 있을 거 같기도 하고
                    setIsStraight(false);
                    if (delta.x < 0) {      // 블럭 기준 왼쪽 충돌
                        position.x -= 450.f * dt;
                    }
                    else {
                        position.x += 450.f * dt;
                    }
                }
                else {                      // 상하 충돌
                    if (delta.y < 0) {      // 블럭 기준 위쪽 충돌
                        setVelocityY(-325.f);
                        breakable->setDrawable(false);
                    }
                    else {
                        setVelocityY(100.f);
                    }
                }
            }
        }
        else if (auto straight = dynamic_cast<Straight*>(block.get())) {
            if (ball.getGlobalBounds().intersects((*block).getBoundary())) {
                if (overlapX < overlapY) {  // 좌우 충돌...더 깔끔하게 뭔가 바꿀 수 있을 거 같기도 하고
                    setIsStraight(false);
                    if (delta.x < 0) {      // 블럭 기준 왼쪽 충돌
                        position.x -= 450.f * dt;
                    }
                    else {
                        position.x += 450.f * dt;
                    }
                }
                else {                      // 상하 충돌
                    if (delta.y < 0) {      // 블럭 기준 위쪽 충돌
                        if (straight->getIsRight()) {   // Straight 진행 방향이 오른쪽인 경우
                            position.x = straight->getBoundary().left + 96.f;
                            position.y = straight->getBoundary().top + 32.f;
                            setVelocityX(450.f);
                            setVelocityY(0.f);
                            setIsStraight(true);
                        }
                        else {
                            position.x = straight->getBoundary().left - 32.f;
                            position.y = straight->getBoundary().top + 32.f;
                            setVelocityX(-450.f);
                            setVelocityY(0.f);
                            setIsStraight(true);
                        }
                    }
                    else {
                        setVelocityY(100.f);
                    }
                }
            } 
        }
    }

}

/**
 * @brief 스테이지 설정
 * @details 플레이어가 현재 플레이 중인 스테이지의 정보를 가져오며, 공의 현재 상태를 초기화
 * @param stage
 */
void Ball::setStage(Stage* stage) {
    this->stage = stage;
    initPosition = stage->startPosition;
    position = initPosition;
    ball.setPosition(initPosition);
    initStarCount = (*stage).starCount;
    setVelocityX(0.f);
    setVelocityY(0.f);
}

/**
 * @brief 죽었을 경우, 스테이지를 다시 시작하는 함수
 * @details 초기 위치로 이동, 모든 블럭 복구, 설정 초기화 작업을 진행함
 */
void Ball::restartStage() {

    // 초기 위치로 이동
    position.x = initPosition.x;
    position.y = initPosition.y;
    velocity.y = 0;

    // 모든 블럭을 돌며, setDrawable 설정
    for (const auto& block : (*stage).blockList) {
        block->setDrawable(true);
    }

    setIsStraight(false);

    // 얻어야 할 별의 개수 초기화
    (*stage).starCount = initStarCount;
}

/**
 * @brief x축 방향의 속도 설정
 * @param velocityX X방향 속도
 */
void Ball::setVelocityX(float velocityX) {
    velocity.x = velocityX;
}

/**
 * @brief y축 방향의 속도 설정
 * @param velocityY Y방향 속도
 */
void Ball::setVelocityY(float velocityY) {
    velocity.y = velocityY;
}

void Ball::setIsStraight(bool isStraight) {
    this->isStraight = isStraight;
}

bool Ball::getIsStraight() {
    return this->isStraight;
}

void Ball::moveStraight(float dt) {
    position.x += velocity.x * dt;
}