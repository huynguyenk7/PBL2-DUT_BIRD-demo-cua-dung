#pragma once
#include <vector>
#include "entities/Bird.hpp"
#include "entities/Pipe.hpp"
#include "entities/Ground.hpp"

namespace systems {

class CollisionManager {
public:
    CollisionManager() = default;
    ~CollisionManager() = default;

    // Kiểm tra toàn bộ va chạm trong game (Chim vs Ống nước & Chim vs Mặt đất)
    // Trả về true nếu chim thực sự chết (Game Over), false nếu vẫn sống (hoặc vừa vỡ khiên)
    static bool checkCollisions(entities::Bird& bird, 
                         const std::vector<std::unique_ptr<entities::Pipe>>& pipes, 
                         const entities::Ground& ground);

private:
    static bool checkPipeCollisions(entities::Bird& bird, const std::vector<std::unique_ptr<entities::Pipe>>& pipes);
    static bool checkGroundCollision(entities::Bird& bird, const entities::Ground& ground);
};

} // namespace systems