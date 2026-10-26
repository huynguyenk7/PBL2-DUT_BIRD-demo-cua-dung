#ifndef BIRD_HPP
#define BIRD_HPP

#include <SFML/Graphics.hpp>
#include <optional>
#include "core/GameConfig.hpp"

namespace entities {

/**
 * @brief Lớp đại diện cho chú chim DUT Bird (tương thích SFML 3.x / 3.1).
 * 
 * Quản lý trạng thái chuyển động, trọng lực, cơ chế đập cánh nhảy (jump),
 * góc nghiêng theo quán tính sử dụng sf::Angle, hitbox va chạm chuẩn xác (sf::FloatRect position/size)
 * và các hiệu ứng bảo vệ (shield/invulnerability).
 */
class Bird : public sf::Drawable {
public:
    Bird();
    explicit Bird(const sf::Vector2f& startPosition);

    // Gán texture cho chim từ bên ngoài (nếu có asset file ảnh)
    void setTexture(const sf::Texture& texture);

    // Xử lý sự kiện từ bàn phím (Space) và chuột (Click chuột trái) theo chuẩn SFML 3
    void handleEvent(const sf::Event& event);

    // Cập nhật vị trí, vận tốc rơi và góc nghiêng của chim
    void update(float dt);

    // Thực hiện hành động nhảy (áp dụng vận tốc hướng lên)
    void jump();

    // Khởi tạo lại trạng thái của chim (dùng khi restart game)
    void reset(const sf::Vector2f& startPosition = {GameConfig::BIRD_START_X, GameConfig::BIRD_START_Y});

    // Getters & Setters
    sf::Vector2f getPosition() const;
    void setPosition(const sf::Vector2f& pos);

    float getVelocityY() const;
    void setVelocityY(float vy);

    sf::Angle getRotation() const;
    float getRotationDegrees() const;
    void setRotation(sf::Angle angle);
    void setRotationDegrees(float degrees);

    // Khung bao phủ toàn bộ của chim (SFML 3: sf::FloatRect chứa position và size)
    sf::FloatRect getGlobalBounds() const;

    // Hitbox va chạm đã thu hẹp viền thừa để tạo cảm giác chơi công bằng
    sf::FloatRect getHitbox() const;

    // Trạng thái sống/chết (phục vụ xử lý va chạm)
    bool isDead() const;
    void setDead(bool dead);

    // Trạng thái khiên bảo vệ (Shield) và bất tử
    bool hasShield() const;
    void setHasShield(bool shield);
    bool isInvulnerable() const;
    void triggerInvulnerability(float duration);
    bool takeHit();

    // SFML Drawable interface
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    void initFallbackShapes();
    void updatePhysics(float dt);
    void updateRotation(float dt);
    void updateVisuals();

private:
    sf::Vector2f m_position{GameConfig::BIRD_START_X, GameConfig::BIRD_START_Y};
    float m_velocityY{0.0f};
    sf::Angle m_rotation{sf::degrees(0.0f)};
    bool m_isDead{false};

    // Đồ họa Sprite & Texture (SFML 3: Sprite không có default constructor, dùng std::optional)
    std::optional<sf::Sprite> m_sprite;

    // Fallback Shapes (dùng khi chưa có ảnh texture, đảm bảo game vẽ được ngay)
    sf::CircleShape m_bodyShape;
    sf::CircleShape m_eyeShape;
    sf::CircleShape m_pupilShape;
    sf::ConvexShape m_beakShape;
    sf::CircleShape m_wingShape;

    // Khiên và hiệu ứng bất tử
    bool m_hasShield{false};
    float m_shieldAnimTimer{0.0f};
    sf::CircleShape m_shieldBubble;
    bool m_isInvulnerable{false};
    float m_invulnerableTimer{0.0f};
};

} // namespace entities

#endif // BIRD_HPP
