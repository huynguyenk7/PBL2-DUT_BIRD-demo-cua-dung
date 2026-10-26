#ifndef PIPE_HPP
#define PIPE_HPP

#include <SFML/Graphics.hpp>
#include <optional>
#include "core/GameConfig.hpp"

namespace entities {

/**
 * @brief Lớp đại diện cho một cặp ống cống chướng ngại vật (Ống trên & Ống dưới) tương thích SFML 3.x / 3.1.
 * 
 * Quản lý vị trí di chuyển sang trái, tọa độ khoảng hở (gap),
 * kiểm tra chim bay qua để cộng điểm (passed),
 * cung cấp hitbox va chạm riêng biệt cho cả ống trên và ống dưới (sf::FloatRect position/size),
 * hỗ trợ render bằng Texture hoặc Fallback Shape.
 */
class Pipe : public sf::Drawable {
public:
    Pipe() = default;
    Pipe(float startX, float gapY, 
         float gapSize = GameConfig::PIPE_GAP, 
         float width = GameConfig::PIPE_WIDTH);
    bool hasSpawnedGift{false};    
    // Gán texture cho ống trên và ống dưới (nếu có asset file ảnh)
    void setTexture(const sf::Texture& topTexture, const sf::Texture& bottomTexture);

    // Cập nhật vị trí dịch chuyển sang trái
    void update(float dt);

    // Kiểm tra ống đã trôi hoàn toàn khỏi màn hình bên trái chưa
    bool isOffScreen() const;

    // Trạng thái đã ghi điểm (chim đã vượt qua ống này chưa)
    bool isPassed() const;
    void setPassed(bool passed);

    // Hitbox va chạm chính xác (SFML 3: sf::FloatRect chứa position và size)
    sf::FloatRect getTopHitbox() const;
    sf::FloatRect getBottomHitbox() const;

    // Khung bao phủ vật lý đầy đủ của ống trên và ống dưới
    sf::FloatRect getTopBounds() const;
    sf::FloatRect getBottomBounds() const;

    // Getters & Setters
    float getX() const;
    void setX(float x);

    float getGapY() const;
    void setGapY(float gapY);

    float getGapSize() const;
    void setGapSize(float gapSize);

    float getWidth() const;
    float getSpeed() const;
    void setSpeed(float speed);

    // SFML Drawable interface
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    void initFallbackShapes();
    void updateVisuals();

private:
    float m_x{0.0f};
    float m_gapY{0.0f};
    float m_gapSize{GameConfig::PIPE_GAP};
    float m_width{GameConfig::PIPE_WIDTH};
    float m_speed{GameConfig::PIPE_SPEED};
    bool m_passed{false};

    // Đồ họa Texture (SFML 3: dùng std::optional<sf::Sprite>)
    std::optional<sf::Sprite> m_spriteTop;
    std::optional<sf::Sprite> m_spriteBottom;

    // Fallback Shapes (dùng khi chưa có ảnh texture)
    sf::RectangleShape m_topBodyShape;
    sf::RectangleShape m_topRimShape;
    sf::RectangleShape m_bottomBodyShape;
    sf::RectangleShape m_bottomRimShape;
};

} // namespace entities

#endif // PIPE_HPP;
