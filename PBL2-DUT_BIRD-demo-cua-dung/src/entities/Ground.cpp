#include "entities/Ground.hpp"
#include "math_lib/MathLib.hpp"

namespace entities {

Ground::Ground() {
    initFallbackShapes();
}

void Ground::initFallbackShapes() {
    // Lớp đất cát (Dirt) phía dưới
    m_dirtShape.setSize({m_width, m_height});
    m_dirtShape.setPosition({0.0f, m_posY});
    m_dirtShape.setFillColor(sf::Color(222, 216, 149)); // Màu đất cát vàng sáng

    // Lớp cỏ xanh (Grass) phía trên
    const float grassHeight = 16.0f;
    m_grassShape.setSize({m_width, grassHeight});
    m_grassShape.setPosition({0.0f, m_posY});
    m_grassShape.setFillColor(sf::Color(115, 191, 46)); // Màu cỏ xanh lục đặc trưng Flappy Bird
    m_grassShape.setOutlineThickness(1.5f);
    m_grassShape.setOutlineColor(sf::Color(85, 140, 30));

    // Dải họa tiết sọc cỏ
    m_grassStripe.setSize({12.0f, 6.0f});
    m_grassStripe.setFillColor(sf::Color(90, 160, 35));
}

void Ground::setTexture(const sf::Texture& texture) {
    m_texturePtr = &texture;
    // Bật chế độ lặp lại để cuộn vô tận
    const_cast<sf::Texture*>(m_texturePtr)->setRepeated(true);

    m_sprite.emplace(texture);
    m_sprite->setPosition({0.0f, m_posY});
    m_sprite->setTextureRect(sf::IntRect({0, 0}, {static_cast<int>(m_width), static_cast<int>(m_height)}));
}

void Ground::update(float dt) {
    if(m_isPaused){
        return;
    }
    m_offsetX += m_speed * dt;
    if (m_sprite.has_value() && m_texturePtr != nullptr) {
        float textureWidth = static_cast<float>(m_texturePtr->getSize().x);
        if (textureWidth > 0.0f && m_offsetX >= textureWidth) {
            m_offsetX -= textureWidth;
        }

        m_sprite->setTextureRect(sf::IntRect(
            {static_cast<int>(m_offsetX), 0},
            {static_cast<int>(m_width), static_cast<int>(m_height)}
        ));
    } else {
        // Fallback: Chu kỳ lặp 24 pixel cho các vệt cỏ cuộn
        const float patternWidth = 24.0f;
        if (m_offsetX >= patternWidth) {
            m_offsetX -= patternWidth;
        }
    }
}
void Ground::setPaused(bool paused) {
    m_isPaused = paused;
}

bool Ground::isPaused() const {
    return m_isPaused;
}
void Ground::reset() {
    m_offsetX = 0.0f;
    if (m_sprite.has_value() && m_texturePtr != nullptr) {
        m_sprite->setTextureRect(sf::IntRect({0, 0}, {static_cast<int>(m_width), static_cast<int>(m_height)}));
    }
}

sf::FloatRect Ground::getHitbox() const {
    // Áp dụng co lùi biên trên của mặt đất (SFML 3: sf::FloatRect(position, size))
    return sf::FloatRect(
        {0.0f, m_posY + GameConfig::HITBOX_GROUND_INSET_Y},
        {m_width, math_lib::max(0.0f, m_height - GameConfig::HITBOX_GROUND_INSET_Y)}
    );
}

sf::FloatRect Ground::getBounds() const {
    return sf::FloatRect({0.0f, m_posY}, {m_width, m_height});
}

float Ground::getSpeed() const {
    return m_speed;
}

void Ground::setSpeed(float speed) {
    m_speed = speed;
}

float Ground::getOffsetY() const {
    return m_posY;
}

void Ground::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (m_sprite.has_value()) {
        target.draw(*m_sprite, states);
    } else {
        // Vẽ khối đất và lớp cỏ nền
        target.draw(m_dirtShape, states);
        target.draw(m_grassShape, states);

        // Vẽ các vệt cỏ cuộn chuyển động (tạo cảm giác mặt đất đang chạy)
        const float patternWidth = 24.0f;
        sf::RectangleShape stripe = m_grassStripe;
        for (float x = -m_offsetX; x < m_width + patternWidth; x += patternWidth) {
            stripe.setPosition({x, m_posY + 10.0f});
            target.draw(stripe, states);
        }
    }
}

} // namespace entities
