#ifndef GROUND_HPP
#define GROUND_HPP

#include <SFML/Graphics.hpp>
#include <optional>
#include "core/GameConfig.hpp"

namespace entities {

/**
 * @brief Lớp đại diện cho mặt đất trong trò chơi (tương thích SFML 3.x / 3.1).
 * 
 * Quản lý chuyển động cuộn vô tận (infinite scrolling loop),
 * hỗ trợ render từ Texture hoặc tự vẽ bằng Shape fallback,
 * và cung cấp hitbox va chạm mặt đất (sf::FloatRect position/size) cho Bird.
 */
class Ground : public sf::Drawable {
public:
    Ground();

    // Gán texture từ bên ngoài cho mặt đất
    void setTexture(const sf::Texture& texture);

    // Cập nhật độ dời cuộn theo delta time
    void update(float dt);

    // Đặt lại trạng thái cuộn ban đầu
    void reset();

    // Hitbox va chạm với chim (áp dụng co lùi HITBOX_GROUND_INSET_Y)
    sf::FloatRect getHitbox() const;

    // Toàn bộ vùng bao phủ của mặt đất
    sf::FloatRect getBounds() const;

    // Getters & Setters
    float getSpeed() const;
    void setSpeed(float speed);

    float getOffsetY() const;

    //hàm điều khiển tạm dừng
    void setPaused(bool paused);
    bool isPaused() const;
    // SFML Drawable interface
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    void initFallbackShapes();

private:
    float m_speed{GameConfig::GROUND_SPEED};
    float m_offsetX{0.0f};
    float m_width{static_cast<float>(GameConfig::WINDOW_WIDTH)};
    float m_height{GameConfig::GROUND_HEIGHT};
    float m_posY{GameConfig::GROUND_Y};
    bool m_isPaused{false};
    // Đồ họa Texture (SFML 3: dùng std::optional)
    const sf::Texture* m_texturePtr{nullptr};
    std::optional<sf::Sprite> m_sprite;

    // Fallback Shapes (dùng khi chưa có ảnh texture)
    sf::RectangleShape m_grassShape;   // Lớp cỏ xanh phía trên
    sf::RectangleShape m_dirtShape;    // Lớp đất nâu phía dưới
    sf::RectangleShape m_grassStripe;  // Dải trang trí cỏ
};

} // namespace entities

#endif // GROUND_HPP
