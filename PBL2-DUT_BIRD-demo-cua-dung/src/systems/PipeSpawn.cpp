#include "systems/PipeSpawn.hpp"

namespace systems {

PipeSpawnerSystem::PipeSpawnerSystem(float spawnInterval)
    : m_spawnInterval(spawnInterval),
      m_spawnTimer(0.0f),
      m_isPaused(false) {
}

void PipeSpawnerSystem::update(float dt) {
    if (m_isPaused) {
        return;
    }

    // 1. Tích lũy thời gian và tự động sinh cặp ống mới
    m_spawnTimer += dt;
    if (m_spawnTimer >= m_spawnInterval) {
        m_spawnTimer = 0.0f;
        spawnPipe();
    }

    // 2. Di chuyển các cặp ống sang bên trái
    for (auto& pipe : m_pipes) {
        pipe->update(dt);
    }

    // 3. Tự động thu hồi và xóa các ống đã trôi hoàn toàn ra khỏi màn hình bên trái
    // Thuật toán lọc xóa 2 con trỏ không dùng thư viện ngoài
    size_t writeIdx = 0;
    for (size_t readIdx = 0; readIdx < m_pipes.size(); ++readIdx) {
        if (!m_pipes[readIdx]->isOffScreen()) {
            if (writeIdx != readIdx) {
                m_pipes[writeIdx] = std::move(m_pipes[readIdx]);
            }
            ++writeIdx;
        }
    }
    m_pipes.erase(m_pipes.begin() + writeIdx, m_pipes.end());
}

void PipeSpawnerSystem::spawnPipe() {
    // Tọa độ xuất hiện bên ngoài mép phải màn hình
    const float startX = static_cast<float>(GameConfig::WINDOW_WIDTH) + 20.0f;

    // Sinh độ cao khe hở gapY ngẫu nhiên trong vùng an toàn bằng thư viện math_lib tự viết
    const float gapY = math_lib::randomFloat(GameConfig::PIPE_MIN_GAP_Y, GameConfig::PIPE_MAX_GAP_Y);

    auto newPipe = std::make_unique<entities::Pipe>(startX, gapY, GameConfig::PIPE_GAP, GameConfig::PIPE_WIDTH);

    // Gán texture nếu đã nạp sẵn
    if (m_topTexture != nullptr && m_bottomTexture != nullptr) {
        newPipe->setTexture(*m_topTexture, *m_bottomTexture);
    }

    m_pipes.push_back(std::move(newPipe));
}

void PipeSpawnerSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (const auto& pipe : m_pipes) {
        target.draw(*pipe, states);
    }
}

bool PipeSpawnerSystem::checkPassedPipe(float birdX) {
    for (auto& pipe : m_pipes) {
        // Nếu chim đã bay qua mép phải của ống mà ống chưa được tính điểm
        if (!pipe->isPassed() && birdX > pipe->getX() + pipe->getWidth()) {
            pipe->setPassed(true);
            return true;
        }
    }
    return false;
}

void PipeSpawnerSystem::setPaused(bool paused) {
    m_isPaused = paused;
}

bool PipeSpawnerSystem::isPaused() const {
    return m_isPaused;
}

void PipeSpawnerSystem::reset() {
    m_pipes.clear();
    m_spawnTimer = 0.0f;
    m_isPaused = false;
}

void PipeSpawnerSystem::setTextures(const sf::Texture* topTex, const sf::Texture* bottomTex) {
    m_topTexture = topTex;
    m_bottomTexture = bottomTex;
    for (auto& pipe : m_pipes) {
        if (m_topTexture != nullptr && m_bottomTexture != nullptr) {
            pipe->setTexture(*m_topTexture, *m_bottomTexture);
        }
    }
}

size_t PipeSpawnerSystem::getPipeCount() const {
    return m_pipes.size();
}

//Truy xuất danh sách ống để CollisionManager đem ra kiểm tra va chạm
const std::vector<std::unique_ptr<entities::Pipe>>& PipeSpawnerSystem::getPipes() const {
    return m_pipes;
}

} // namespace systems