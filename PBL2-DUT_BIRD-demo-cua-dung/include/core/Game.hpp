#pragma once

#include <SFML/Graphics.hpp>
#include "GameConfig.hpp"
#include "entities/Bird.hpp"
#include "entities/Ground.hpp"
#include "entities/Pipe.hpp"
#include "systems/PipeSpawn.hpp"
#include "systems/collisionmanage.hpp"
#include "systems/PowerUp.hpp"

namespace core {

class Game {
public:
    Game();
    ~Game() = default;

    // Ngăn chặn copy để bảo toàn tài nguyên window và context
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    // Vòng lặp chính của trò chơi
    void run();

private:
    void processEvents();
    void update(float dt);
    void render();
    void reset();

private:
    sf::RenderWindow m_window;
    sf::Clock m_clock;
    bool m_isRunning{true};
    bool m_isGameOver{false};
    int m_score{0};

    // Các thực thể và hệ thống cốt lõi
    entities::Bird m_bird;
    entities::Ground m_ground;
    entities::Pipe m_pipe;
    systems::PipeSpawnerSystem m_pipespawner;
    systems::PowerUpSystem m_powerUpSystem;
};

} // namespace core