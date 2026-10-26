#include"systems/PowerUp.hpp"
#include <algorithm>
namespace systems{
   // 1. Cập nhật vị trí hộp quà di chuyển trôi sang trái (cùng tốc độ với ống nước)
void GiftBox::update(float dt, float speed) {
    position.x -= speed * dt;
    hitbox.position = position; // Cập nhật lại vị trí hitbox đi theo sprite/hình vẽ
}

// 2. Vẽ hộp quà lên màn hình
void GiftBox::draw(sf::RenderTarget& target) const {
    // Tạm thời dùng hình chữ nhật hoặc sprite để vẽ hộp quà
    sf::RectangleShape rect(sf::Vector2f(30.0f, 30.0f));
    rect.setPosition(position);
    rect.setFillColor(sf::Color::Yellow); // Màu vàng cho hộp quà/item
    target.draw(rect);
}

// --- Khởi tạo PowerUpSystem ---
PowerUpSystem::PowerUpSystem() {
    // Constructor mặc định
}

void PowerUpSystem::update(float dt, entities::Bird& bird,const std::vector<std::unique_ptr<entities::Pipe>>& pipes) {
    // --- 1. TỰ ĐỘNG SINH HỘP QUÀ KHI CÓ ỐNG MỚI ---
    for (const auto& pipePtr : pipes) {
        if (pipePtr && !pipePtr->hasSpawnedGift && pipePtr->getX() >= GameConfig::WINDOW_WIDTH - 5.0f) {
            pipePtr->hasSpawnedGift = true; // Đánh dấu ống này đã được xét để không bị lặp

            // Tỉ lệ ngẫu nhiên xuất hiện hộp quà
            if (math_lib::randomFloat01() <GameConfig:: Rate_spawn_giftbox) {
                spawnGiftBox(pipePtr->getX(), pipePtr->getGapY());
            }
        }
    }
    // --- Cập nhật vị trí các hộp quà đang có ---
    for (auto& box : m_giftBoxes) {
        box.update(dt, GameConfig::PIPE_SPEED); 
    }

    // Xóa hộp quà khi bay ra khỏi màn hình hoặc đã bị nhặt
    m_giftBoxes.erase(
        std::remove_if(m_giftBoxes.begin(), m_giftBoxes.end(), [](const GiftBox& box) {
            return box.position.x + 30.0f < 0.0f || box.collected;
        }),
        m_giftBoxes.end()
    );

    // --- Quản lý thời gian khiên ---
    if (m_shieldTimer > 0.0f) {
        m_shieldTimer -= dt;
        if (m_shieldTimer <= 0.0f) {
            m_shieldTimer = 0.0f;
            bird.setHasShield(false);
        }
    }
}

void PowerUpSystem::draw(sf::RenderTarget& target) const {
    for (const auto& box : m_giftBoxes) {
        if (!box.collected) {
            box.draw(target);
        }
    }
}

void PowerUpSystem::spawnGiftBox(float x, float y) {
    GiftBox box;
    box.position = sf::Vector2f(x, y);
    box.hitbox = sf::FloatRect({x, y}, {30.0f, 30.0f});
    box.type = PowerUpType::Shield; // Tạm thời mặc định sinh ra hộp quà khiên
    box.collected = false;
    
    m_giftBoxes.push_back(box);
}

void PowerUpSystem::checkCollisions(entities::Bird& bird) {
    sf::FloatRect birdHitbox = bird.getHitbox();

    for (auto& box : m_giftBoxes) {
        if (!box.collected && birdHitbox.findIntersection(box.hitbox).has_value()) {
            box.collected = true; // Đánh dấu hộp quà đã được nhặt
            
            // Kích hoạt hiệu ứng khiên 10 giây cho chim
           bird.setHasShield(true);
            m_shieldTimer = 10.0f;
        }
    }
}

void PowerUpSystem::reset() {
    m_giftBoxes.clear();
    m_shieldTimer = 0.0f;
    m_speedBoostTimer = 0.0f;
    m_enlargeTimer = 0.0f;
}
}