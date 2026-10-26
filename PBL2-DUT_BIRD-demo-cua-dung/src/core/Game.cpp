#include "core/Game.hpp"
#include "math_lib/MathLib.hpp"
#include <iostream>
#include <string>

namespace core {

Game::Game()
    : m_window(sf::VideoMode({GameConfig::WINDOW_WIDTH, GameConfig::WINDOW_HEIGHT}),
               std::string(GameConfig::WINDOW_TITLE),
               sf::Style::Close | sf::Style::Titlebar) {
    
    if (GameConfig::VSYNC_ENABLED) {
        m_window.setVerticalSyncEnabled(true);
    } else {
        m_window.setFramerateLimit(GameConfig::FPS);
    }
}

void Game::run() {
    while (m_window.isOpen() && m_isRunning) {
        float dt = m_clock.restart().asSeconds();
        
        // Giới hạn delta time (clamp) bằng hàm tự viết trong math_lib
        // Tránh giật lag hoặc hiện tượng nhảy xuyên vật thể
        dt = math_lib::clamp(dt, 0.0f, 0.1f);

        processEvents();
        update(dt);
        render();
    }
}

void Game::processEvents() {
    // SFML 3: pollEvent trả về std::optional<sf::Event>
    while (const auto event = m_window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            m_window.close();
            m_isRunning = false;
        }
        
        if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            // Nhấn phím Escape để đóng game
            if (keyPressed->code == sf::Keyboard::Key::Escape) {
                m_window.close();
                m_isRunning = false;
            }
            // Nhấn phím R để Replay khi thua
            if (keyPressed->code == sf::Keyboard::Key::R && m_isGameOver) {
                reset();
            }
        }

        // Chuyển sự kiện phím Space / Chuột trái cho chim nhảy nếu chưa game over
        if (!m_isGameOver) {
            m_bird.handleEvent(*event);
        }
    }
}

void Game::update(float dt) {
    if (m_isGameOver) {
        // Khi game over, chim vẫn rơi tự do chạm đất nếu chưa chạm đất
        m_bird.update(dt);
        return;
    }

   // 1. Cập nhật các thực thể và hệ sinh ống
    m_bird.update(dt);
    m_ground.update(dt);
    m_pipespawner.update(dt);
    m_powerUpSystem.update(dt, m_bird, m_pipespawner.getPipes());

    // Kiểm tra va chạm hộp quà
    m_powerUpSystem.checkCollisions(m_bird);
    // 2. Kiểm tra tính điểm khi chim bay qua ống
    if (m_pipespawner.checkPassedPipe(m_bird.getPosition().x)) {
        m_score++;
        std::cout << "Score: " << m_score << std::endl;
    }
    m_powerUpSystem.checkCollisions(m_bird);
    bool isActuallyDead = systems::CollisionManager::checkCollisions(
        m_bird, 
        m_pipespawner.getPipes(), // hoặc m_pipeSystem.getPipes() tùy theo tên biến của bạn
        m_ground
    );

    // 4. Xử lý khi chim thực sự chết (Đã hết khiên hoặc không có khiên mà đâm vào vật cản)
    if (isActuallyDead || m_bird.isDead()) {
        m_isGameOver = true;
        m_ground.setPaused(true);
        m_pipespawner.setPaused(true); // hoặc m_pipeSystem.setPaused(true)
    }
}

void Game::render() {
    // 1. Màu xanh da trời đặc trưng Flappy Bird
    m_window.clear(sf::Color(113, 197, 207));

    // 2. Dựng hình ống (chân ống nằm phía dưới mặt đất)
    m_pipespawner.draw(m_window);

     //vẽ hộp quà
    m_powerUpSystem.draw(m_window);

    // 3. Dựng hình mặt đất (che chân ống)
    m_window.draw(m_ground);

    // 4. Dựng hình chim
    m_window.draw(m_bird);

    m_window.display();
}

void Game::reset() {
    m_bird.reset();
    m_ground.reset();
    m_pipespawner.reset();
    m_isGameOver = false;
    m_score = 0;
}

} // namespace core