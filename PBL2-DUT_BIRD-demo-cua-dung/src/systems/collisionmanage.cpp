#include "systems/collisionmanage.hpp"
#include<optional>
namespace systems {

bool CollisionManager::checkCollisions(entities::Bird& bird, 
                                     const std::vector<std::unique_ptr<entities::Pipe>>& pipes, 
                                     const entities::Ground& ground) {
    // Nếu chim đã chết rồi thì bỏ qua kiểm tra
    if (bird.isDead()) {
        return false;
    }

    bool hasCollided = false;

    // 1. Kiểm tra va chạm với các ống nước
    if (checkPipeCollisions(bird, pipes)) {
        hasCollided = true;
    }

    // 2. Kiểm tra va chạm với mặt đất
    if (checkGroundCollision(bird, ground)) {
        hasCollided = true;
    }

    // Nếu có xảy ra va chạm, gọi hàm takeHit() của chim
    // (takeHit tự xử lý: có khiên -> phá khiên + bất tử, không có khiên -> chết thật)
    if (hasCollided) {
        return bird.takeHit(); // Trả về true nếu chim thực sự chết sau cú va chạm này
    }

    return false; // Không có va chạm nào xảy ra
}

bool CollisionManager::checkPipeCollisions(entities::Bird& bird, const std::vector<std::unique_ptr<entities::Pipe>>& pipes) {
    sf::FloatRect birdHitbox = bird.getHitbox();

    for (const auto& pipe : pipes) {
        // Sử dụng hàm findIntersection chuẩn của SFML 3 để kiểm tra giao nhau
        if (birdHitbox.findIntersection(pipe->getTopHitbox()).has_value() || 
            birdHitbox.findIntersection(pipe->getBottomHitbox()).has_value()) {
            return true;
        }
    }
    return false;
}

bool CollisionManager::checkGroundCollision(entities::Bird& bird, const entities::Ground& ground) {
    sf::FloatRect birdHitbox = bird.getHitbox();
    sf::FloatRect groundHitbox = ground.getHitbox();

    // Kiểm tra xem hitbox của chim có chạm vào mặt đất không
    if (birdHitbox.findIntersection(groundHitbox)) {
        return true;
    }

    return false;
}

} // namespace systems