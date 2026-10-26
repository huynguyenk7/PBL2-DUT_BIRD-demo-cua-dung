#ifndef POWER_UP_SYSTEM_HPP
#define POWER_UP_SYSTEM_HPP
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "entities/Bird.hpp"
#include "math_lib/MathLib.hpp"
#include "entities/Pipe.hpp"
#include "core/GameConfig.hpp"
enum class PowerUpType {
    Shield,       // Khiên bảo vệ
    SpeedBoost,   // Tăng tốc độ
    EnlargeBird   // Phóng to chim
};
namespace systems {

// Cấu trúc đại diện cho một hộp quà trên màn hình
struct GiftBox {
    sf::Vector2f position;
    sf::FloatRect hitbox;
    PowerUpType type;
    bool collected{false};
    
    // Hàm vẽ và cập nhật vị trí trôi sang trái cùng tốc độ với ống
    void update(float dt, float speed);
    void draw(sf::RenderTarget& target) const;
};

class PowerUpSystem {
public:
    PowerUpSystem();
    ~PowerUpSystem() = default;

    void update(float dt, entities::Bird& bird,const std::vector<std::unique_ptr<entities::Pipe>>& pipes);
    void draw(sf::RenderTarget& target) const;
    
    // Sinh hộp quà ngẫu nhiên (có thể gọi kèm lúc sinh ống)
    void spawnGiftBox(float x, float y);

    // Kiểm tra va chạm giữa chim và các hộp quà
    void checkCollisions(entities::Bird& bird);

    void reset();

private:
    std::vector<GiftBox> m_giftBoxes;
    
    // Quản lý thời gian hiệu lực của các hiệu ứng đang bật trên chim
    float m_shieldTimer{0.0f};
    float m_speedBoostTimer{0.0f};
    float m_enlargeTimer{0.0f};
    float m_spawnTimer = 0.0f;       // Bộ đếm thời gian trôi qua từ lần sinh hộp quà gần nhất
    float m_nextSpawnTime = 3.0f;
};

} // namespace systems

#endif // POWER_UP_SYSTEM_HPP