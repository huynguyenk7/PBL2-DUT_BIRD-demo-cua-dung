#include "entities/Bird.hpp"
#include "math_lib/MathLib.hpp"


namespace entities {

Bird::Bird() 
    : Bird(sf::Vector2f(GameConfig::BIRD_START_X, GameConfig::BIRD_START_Y)) {
}

Bird::Bird(const sf::Vector2f& startPosition) {
    initFallbackShapes();
    reset(startPosition);
}

void Bird::initFallbackShapes() {
    // Thân chim: Hình tròn màu vàng DUT
    const float radius = 16.0f;
    m_bodyShape.setRadius(radius);
    m_bodyShape.setOrigin({radius, radius});
    m_bodyShape.setFillColor(sf::Color(255, 204, 0)); // Màu vàng tươi
    m_bodyShape.setOutlineThickness(1.5f);
    m_bodyShape.setOutlineColor(sf::Color(40, 40, 40));

    // Cánh chim: Bầu dục nhỏ hơn
    m_wingShape.setRadius(8.0f);
    m_wingShape.setOrigin({8.0f, 8.0f});
    m_wingShape.setFillColor(sf::Color(245, 245, 245));
    m_wingShape.setOutlineThickness(1.0f);
    m_wingShape.setOutlineColor(sf::Color(50, 50, 50));

    // Mắt: Lòng trắng
    m_eyeShape.setRadius(5.0f);
    m_eyeShape.setOrigin({5.0f, 5.0f});
    m_eyeShape.setFillColor(sf::Color::White);
    m_eyeShape.setOutlineThickness(1.0f);
    m_eyeShape.setOutlineColor(sf::Color::Black);

    // Con ngươi: Màu đen
    m_pupilShape.setRadius(2.0f);
    m_pupilShape.setOrigin({2.0f, 2.0f});
    m_pupilShape.setFillColor(sf::Color::Black);

    // Mỏ chim: Tam giác màu cam
    m_beakShape.setPointCount(3);
    m_beakShape.setPoint(0, sf::Vector2f(0.0f, -5.0f));
    m_beakShape.setPoint(1, sf::Vector2f(12.0f, 0.0f));
    m_beakShape.setPoint(2, sf::Vector2f(0.0f, 5.0f));
    m_beakShape.setOrigin({0.0f, 0.0f});
    m_beakShape.setFillColor(sf::Color(255, 102, 0));
    m_beakShape.setOutlineThickness(1.0f);
    m_beakShape.setOutlineColor(sf::Color(50, 50, 50));

    // Bong bóng bảo vệ (Khiên Shield)
    m_shieldBubble.setRadius(25.0f);
    m_shieldBubble.setOrigin({25.0f, 25.0f});
    m_shieldBubble.setFillColor(sf::Color(64, 164, 255, 90));
    m_shieldBubble.setOutlineThickness(2.5f);
    m_shieldBubble.setOutlineColor(sf::Color(130, 210, 255, 220));
}

void Bird::setTexture(const sf::Texture& texture) {
    m_sprite.emplace(texture);
    auto bounds = m_sprite->getLocalBounds();
    m_sprite->setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});
    updateVisuals();
}

void Bird::handleEvent(const sf::Event& event) {
    if (m_isDead) {
        return;
    }

    // Xử lý sự kiện bàn phím và chuột theo chuẩn SFML 3 (std::variant / getIf)
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
        if (keyPressed->code == sf::Keyboard::Key::Space) {
            jump();
        }
    } else if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mousePressed->button == sf::Mouse::Button::Left) {
            jump();
        }
    }
}

void Bird::jump() {
    if (m_isDead) {
        return;
    }

    m_velocityY = GameConfig::JUMP_VELOCITY;
    m_rotation = sf::degrees(GameConfig::BIRD_ROTATION_UP);
}
bool Bird::hasShield() const {
    return m_hasShield;
}

void Bird::setHasShield(bool shield) {
    m_hasShield = shield;
}

bool Bird::isInvulnerable() const {
    return m_isInvulnerable;
}

void Bird::triggerInvulnerability(float duration) {
    m_isInvulnerable = true;
    m_invulnerableTimer = duration;
}
void Bird::update(float dt) {
    // Cập nhật hiệu ứng bất tử chớp nháy
    if (m_isInvulnerable) {
        m_invulnerableTimer -= dt;
        if (m_invulnerableTimer <= 0.0f) {
            m_isInvulnerable = false;
        }
    }

    // Hiệu ứng nhịp thở của khiên
    if (m_hasShield) {
        m_shieldAnimTimer += dt * 4.0f;
        float pulse = 24.0f + math_lib::sin(m_shieldAnimTimer) * 2.0f;
        m_shieldBubble.setRadius(pulse);
        m_shieldBubble.setOrigin({pulse, pulse});
    }

    updatePhysics(dt);
    updateRotation(dt);
    updateVisuals();
}

void Bird::updatePhysics(float dt) {
    // Áp dụng gia tốc trọng trường
    m_velocityY += GameConfig::GRAVITY * dt;
    if (m_velocityY > GameConfig::BIRD_MAX_FALL_SPEED) {
        m_velocityY = GameConfig::BIRD_MAX_FALL_SPEED;
    }

    m_position.y += m_velocityY * dt;

    // Giới hạn không cho chim bay vượt khỏi mép trên màn hình
    if (m_position.y < 0.0f) {
        m_position.y = 0.0f;
        if (m_velocityY < 0.0f) {
            m_velocityY = 0.0f;
        }
    }
}

void Bird::updateRotation(float dt) {
    float deg = m_rotation.asDegrees();
    if (m_velocityY > 0.0f) {
        deg += GameConfig::BIRD_ROTATION_SPEED * dt;
        if (deg > GameConfig::BIRD_ROTATION_DOWN_MAX) {
            deg = GameConfig::BIRD_ROTATION_DOWN_MAX;
        }
    } else {
        deg = GameConfig::BIRD_ROTATION_UP;
    }
    m_rotation = sf::degrees(deg);
}

void Bird::updateVisuals() {
    if (m_sprite.has_value()) {
        m_sprite->setPosition(m_position);
        m_sprite->setRotation(m_rotation);
    } else {
        m_bodyShape.setPosition(m_position);
        m_bodyShape.setRotation(m_rotation);

        const float rad = m_rotation.asRadians();
        const float cosA = math_lib::cos(rad);
        const float sinA = math_lib::sin(rad);

        // Vị trí cánh (hơi lùi về sau)
        sf::Vector2f wingOffset(-6.0f, 2.0f);
        m_wingShape.setPosition({m_position.x + wingOffset.x * cosA - wingOffset.y * sinA,
                                 m_position.y + wingOffset.x * sinA + wingOffset.y * cosA});
        m_wingShape.setRotation(m_rotation);

        // Vị trí mắt (phía trước trên)
        sf::Vector2f eyeOffset(6.0f, -5.0f);
        m_eyeShape.setPosition({m_position.x + eyeOffset.x * cosA - eyeOffset.y * sinA,
                                m_position.y + eyeOffset.x * sinA + eyeOffset.y * cosA});

        sf::Vector2f pupilOffset(8.0f, -5.0f);
        m_pupilShape.setPosition({m_position.x + pupilOffset.x * cosA - pupilOffset.y * sinA,
                                  m_position.y + pupilOffset.x * sinA + pupilOffset.y * cosA});

        // Vị trí mỏ (nhô hẳn ra ngoài phía trước)
        sf::Vector2f beakOffset(10.0f, 2.0f);
        m_beakShape.setPosition({m_position.x + beakOffset.x * cosA - beakOffset.y * sinA,
                                 m_position.y + beakOffset.x * sinA + beakOffset.y * cosA});
        m_beakShape.setRotation(m_rotation);
    }

    m_shieldBubble.setPosition(m_position);
}

void Bird::reset(const sf::Vector2f& startPosition) {
    m_position = startPosition;
    m_velocityY = 0.0f;
    m_rotation = sf::degrees(0.0f);
    m_isDead = false;
    m_hasShield = false;
    m_isInvulnerable = false;
    m_invulnerableTimer = 0.0f;
    m_shieldAnimTimer = 0.0f;

    updateVisuals();
}

sf::Vector2f Bird::getPosition() const {
    return m_position;
}

void Bird::setPosition(const sf::Vector2f& pos) {
    m_position = pos;
    updateVisuals();
}

float Bird::getVelocityY() const {
    return m_velocityY;
}

void Bird::setVelocityY(float vy) {
    m_velocityY = vy;
}

sf::Angle Bird::getRotation() const {
    return m_rotation;
}

float Bird::getRotationDegrees() const {
    return m_rotation.asDegrees();
}

void Bird::setRotation(sf::Angle angle) {
    m_rotation = angle;
    updateVisuals();
}

void Bird::setRotationDegrees(float degrees) {
    m_rotation = sf::degrees(degrees);
    updateVisuals();
}

sf::FloatRect Bird::getGlobalBounds() const {
    if (m_sprite.has_value()) {
        return m_sprite->getGlobalBounds();
    }
    return sf::FloatRect({m_position.x - 16.0f, m_position.y - 16.0f}, {32.0f, 32.0f});
}

sf::FloatRect Bird::getHitbox() const {
    sf::FloatRect bounds = getGlobalBounds();
    return sf::FloatRect(
        {bounds.position.x + GameConfig::HITBOX_BIRD_INSET_X,
         bounds.position.y + GameConfig::HITBOX_BIRD_INSET_Y},
        {math_lib::max(0.0f, bounds.size.x - 2.0f * GameConfig::HITBOX_BIRD_INSET_X),
         math_lib::max(0.0f, bounds.size.y - 2.0f * GameConfig::HITBOX_BIRD_INSET_Y)}
    );
}

bool Bird::isDead() const {
    return m_isDead;
}

void Bird::setDead(bool dead) {
    m_isDead = dead;
}

bool Bird::takeHit() {
    // Nếu chim đã chết hoặc đang trong trạng thái bất tử thì bỏ qua va chạm
    if (m_isDead || m_isInvulnerable) {
        return false;
    }

    // Nếu đang có khiên bảo vệ: Phá vỡ khiên, kích hoạt bất tử tạm thời và SỐNG SÓT
    if (m_hasShield) {
        m_hasShield = false;
        triggerInvulnerability(1.5f); // 1.5 giây bất tử sau khi vỡ khiên để tránh chạm liên tiếp
        return false;
    }

    m_isDead = true;
    return true;
}
void Bird::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    // Hiệu ứng nhấp nháy khi bất tử
    if (m_isInvulnerable) {
        int blinkState = static_cast<int>(m_invulnerableTimer * 20.0f) % 2;
        if (blinkState == 0) {
            return;
        }
    }

    if (m_sprite.has_value()) {
        target.draw(*m_sprite, states);
    } else {
        target.draw(m_bodyShape, states);
        target.draw(m_wingShape, states);
        target.draw(m_beakShape, states);
        target.draw(m_eyeShape, states);
        target.draw(m_pupilShape, states);
    }

    // Vẽ khiên chắn nếu đang kích hoạt
    if (m_hasShield) {
        target.draw(m_shieldBubble, states);
    }
}

} // namespace entities
