#include "entities/Pipe.hpp"
#include "math_lib/MathLib.hpp"

namespace entities {

Pipe::Pipe(float startX, float gapY, float gapSize, float width)
    : m_x(startX), m_gapY(gapY), m_gapSize(gapSize), m_width(width) {
    initFallbackShapes();
    updateVisuals();
}

void Pipe::initFallbackShapes() {
    sf::Color pipeColor(115, 191, 46);      // Màu xanh lục ống cống
    sf::Color outlineColor(60, 120, 20);     // Viền ống cống đậm
    const float outlineThickness = 2.0f;

    // Thân ống trên
    m_topBodyShape.setFillColor(pipeColor);
    m_topBodyShape.setOutlineThickness(outlineThickness);
    m_topBodyShape.setOutlineColor(outlineColor);

    // Vành miệng ống trên
    m_topRimShape.setFillColor(pipeColor);
    m_topRimShape.setOutlineThickness(outlineThickness);
    m_topRimShape.setOutlineColor(outlineColor);

    // Thân ống dưới
    m_bottomBodyShape.setFillColor(pipeColor);
    m_bottomBodyShape.setOutlineThickness(outlineThickness);
    m_bottomBodyShape.setOutlineColor(outlineColor);

    // Vành miệng ống dưới
    m_bottomRimShape.setFillColor(pipeColor);
    m_bottomRimShape.setOutlineThickness(outlineThickness);
    m_bottomRimShape.setOutlineColor(outlineColor);
}

void Pipe::setTexture(const sf::Texture& topTexture, const sf::Texture& bottomTexture) {
    m_spriteTop.emplace(topTexture);
    m_spriteBottom.emplace(bottomTexture);
    updateVisuals();
}

void Pipe::update(float dt) {
    m_x -= m_speed * dt;
    updateVisuals();
}

void Pipe::updateVisuals() {
    const float halfGap = m_gapSize / 2.0f;
    const float topHeight = math_lib::max(0.0f, m_gapY - halfGap);
    const float bottomY = m_gapY + halfGap;
    const float bottomHeight = math_lib::max(0.0f, GameConfig::GROUND_Y - bottomY);

    const float rimHeight = 24.0f;
    const float rimExtraWidth = 8.0f; // Vành miệng ống nở rộng hơn thân mỗi bên 4px

    if (m_spriteTop.has_value() && m_spriteBottom.has_value()) {
        // Căn chỉnh sprite ống trên (quay ngược xuống)
        auto topBounds = m_spriteTop->getLocalBounds();
        if (topBounds.size.x > 0.0f && topBounds.size.y > 0.0f) {
            float scaleX = m_width / topBounds.size.x;
            float scaleY = topHeight / topBounds.size.y;
            m_spriteTop->setScale({scaleX, scaleY});
            m_spriteTop->setPosition({m_x, 0.0f});
        }

        // Căn chỉnh sprite ống dưới (chĩa thẳng lên)
        auto botBounds = m_spriteBottom->getLocalBounds();
        if (botBounds.size.x > 0.0f && botBounds.size.y > 0.0f) {
            float scaleX = m_width / botBounds.size.x;
            float scaleY = bottomHeight / botBounds.size.y;
            m_spriteBottom->setScale({scaleX, scaleY});
            m_spriteBottom->setPosition({m_x, bottomY});
        }
    } else {
        // Căn chỉnh fallback shapes cho ống trên
        m_topBodyShape.setSize({m_width, math_lib::max(0.0f, topHeight - rimHeight)});
        m_topBodyShape.setPosition({m_x, 0.0f});

        m_topRimShape.setSize({m_width + rimExtraWidth, rimHeight});
        m_topRimShape.setPosition({m_x - rimExtraWidth / 2.0f, math_lib::max(0.0f, topHeight - rimHeight)});

        // Căn chỉnh fallback shapes cho ống dưới
        m_bottomRimShape.setSize({m_width + rimExtraWidth, rimHeight});
        m_bottomRimShape.setPosition({m_x - rimExtraWidth / 2.0f, bottomY});

        m_bottomBodyShape.setSize({m_width, math_lib::max(0.0f, bottomHeight - rimHeight)});
        m_bottomBodyShape.setPosition({m_x, bottomY + rimHeight});
    }
}

bool Pipe::isOffScreen() const {
    // Ống trôi hoàn toàn khỏi mép trái màn hình (tính cả độ nở của vành)
    return (m_x + m_width + 10.0f < 0.0f);
}

bool Pipe::isPassed() const {
    return m_passed;
}

void Pipe::setPassed(bool passed) {
    m_passed = passed;
}

sf::FloatRect Pipe::getTopBounds() const {
    const float topHeight = math_lib::max(0.0f, m_gapY - (m_gapSize / 2.0f));
    return sf::FloatRect({m_x, 0.0f}, {m_width, topHeight});
}

sf::FloatRect Pipe::getBottomBounds() const {
    const float bottomY = m_gapY + (m_gapSize / 2.0f);
    const float bottomHeight = math_lib::max(0.0f, GameConfig::GROUND_Y - bottomY);
    return sf::FloatRect({m_x, bottomY}, {m_width, bottomHeight});
}

sf::FloatRect Pipe::getTopHitbox() const {
    sf::FloatRect bounds = getTopBounds();
    return sf::FloatRect(
        {bounds.position.x + GameConfig::HITBOX_PIPE_INSET_X, 0.0f},
        {math_lib::max(0.0f, bounds.size.x - 2.0f * GameConfig::HITBOX_PIPE_INSET_X),
         math_lib::max(0.0f, bounds.size.y - GameConfig::HITBOX_PIPE_INSET_Y)}
    );
}

sf::FloatRect Pipe::getBottomHitbox() const {
    sf::FloatRect bounds = getBottomBounds();
    return sf::FloatRect(
        {bounds.position.x + GameConfig::HITBOX_PIPE_INSET_X,
         bounds.position.y + GameConfig::HITBOX_PIPE_INSET_Y},
        {math_lib::max(0.0f, bounds.size.x - 2.0f * GameConfig::HITBOX_PIPE_INSET_X),
         math_lib::max(0.0f, bounds.size.y - GameConfig::HITBOX_PIPE_INSET_Y)}
    );
}

float Pipe::getX() const {
    return m_x;
}

void Pipe::setX(float x) {
    m_x = x;
    updateVisuals();
}

float Pipe::getGapY() const {
    return m_gapY;
}

void Pipe::setGapY(float gapY) {
    m_gapY = gapY;
    updateVisuals();
}

float Pipe::getGapSize() const {
    return m_gapSize;
}

void Pipe::setGapSize(float gapSize) {
    m_gapSize = gapSize;
    updateVisuals();
}

float Pipe::getWidth() const {
    return m_width;
}

float Pipe::getSpeed() const {
    return m_speed;
}

void Pipe::setSpeed(float speed) {
    m_speed = speed;
}

void Pipe::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (m_spriteTop.has_value() && m_spriteBottom.has_value()) {
        target.draw(*m_spriteTop, states);
        target.draw(*m_spriteBottom, states);
    } else {
        target.draw(m_topBodyShape, states);
        target.draw(m_topRimShape, states);
        target.draw(m_bottomRimShape, states);
        target.draw(m_bottomBodyShape, states);
    }
}

} // namespace entities
