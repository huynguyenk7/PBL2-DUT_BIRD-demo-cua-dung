#ifndef PIPE_SPAWNER_SYSTEM_HPP
#define PIPE_SPAWNER_SYSTEM_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "entities/Pipe.hpp"
#include "core/GameConfig.hpp"
#include "math_lib/MathLib.hpp"

namespace systems {

/**
 * @brief Hệ thống quản lý vòng đời của các chướng ngại vật Pipe (SFML 3.x / 3.1).
 * 
 * - Tự động tạo cặp ống sau mỗi khoảng thời gian m_spawnInterval.
 * - Sinh tọa độ khe hở gapY ngẫu nhiên trong vùng an toàn bằng thuật toán math_lib::randomFloat.
 * - Quản lý danh sách ống bằng std::unique_ptr<entities::Pipe> và tự động hủy ống ra khỏi màn hình (zero memory leak).
 * - Cung cấp giao diện kiểm tra va chạm tập trung và kiểm tra tính điểm khi chim vượt qua ống.
 */
class PipeSpawnerSystem {
public:
    explicit PipeSpawnerSystem(float spawnInterval = GameConfig::PIPE_SPAWN_INTERVAL);
    ~PipeSpawnerSystem() = default;

    // Ngăn chặn copy để quản lý tài nguyên unique_ptr an toàn
    PipeSpawnerSystem(const PipeSpawnerSystem&) = delete;
    PipeSpawnerSystem& operator=(const PipeSpawnerSystem&) = delete;

    PipeSpawnerSystem(PipeSpawnerSystem&&) noexcept = default;
    PipeSpawnerSystem& operator=(PipeSpawnerSystem&&) noexcept = default;

    // Cập nhật timer, sinh ống mới, di chuyển ống và xóa ống ra khỏi màn hình
    void update(float dt);

    // Vẽ toàn bộ các cặp ống lên target
    void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const;

    // Kiểm tra nếu chim vừa vượt qua một ống chưa được tính điểm (trả về true để cộng điểm)
    bool checkPassedPipe(float birdX);

    // Điều khiển trạng thái
    void setPaused(bool paused);
    bool isPaused() const;
    void reset();

    // Thiết lập Texture cho các ống sinh ra trong tương lai (nếu có asset ảnh)
    void setTextures(const sf::Texture* topTex, const sf::Texture* bottomTex);

    // Truy xuất số lượng ống đang hoạt động
    size_t getPipeCount() const;

    // Truy xuất danh sách ống phục vụ test hoặc render mở rộng
    const std::vector<std::unique_ptr<entities::Pipe>>& getPipes() const;

private:
    void spawnPipe();

private:
    std::vector<std::unique_ptr<entities::Pipe>> m_pipes;
    float m_spawnInterval{GameConfig::PIPE_SPAWN_INTERVAL};
    float m_spawnTimer{0.0f};
    bool m_isPaused{false};

    const sf::Texture* m_topTexture{nullptr};
    const sf::Texture* m_bottomTexture{nullptr};
};

} // namespace systems

#endif // PIPE_SPAWNER_SYSTEM_HPP