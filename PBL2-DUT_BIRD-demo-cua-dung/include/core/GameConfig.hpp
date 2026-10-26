#ifndef GAMECONFIG_HPP
#define GAMECONFIG_HPP
#include <string_view>

namespace GameConfig {
    // Window settings
    inline constexpr unsigned int WINDOW_WIDTH = 480;
    inline constexpr unsigned int WINDOW_HEIGHT = 640;
    inline constexpr std::string_view WINDOW_TITLE = "DUT BIRD";
    inline constexpr unsigned int FPS = 60;
    inline constexpr unsigned int VSYNC_ENABLED = false;

    // Physics settings
    inline constexpr float GRAVITY = 980.0f;
    inline constexpr float JUMP_VELOCITY = -340.0f;

    // Bird settings
    inline constexpr float BIRD_START_X = 140.0f;
    inline constexpr float BIRD_START_Y = 280.0f;
    inline constexpr float BIRD_MAX_FALL_SPEED = 600.0f;
    inline constexpr float BIRD_ROTATION_UP = -25.0f;        // Góc ngẩng khi đập cánh (độ)
    inline constexpr float BIRD_ROTATION_DOWN_MAX = 75.0f;   // Góc chúc đầu tối đa khi rơi (độ)
    inline constexpr float BIRD_ROTATION_SPEED = 380.0f;     // Tốc độ xoay đầu (độ/giây)

    // Ground settings
    inline constexpr float GROUND_HEIGHT = 112.0f;
    inline constexpr float GROUND_Y = static_cast<float>(WINDOW_HEIGHT) - GROUND_HEIGHT; // 528.0f
    inline constexpr float GROUND_SPEED = 180.0f;

    // Pipe settings
    inline constexpr float PIPE_SPEED = 180.0f;
    inline constexpr float PIPE_SPAWN_INTERVAL = 1.5f;
    inline constexpr float PIPE_GAP = 140.0f;
    inline constexpr float PIPE_WIDTH = 70.0f;
    inline constexpr float PIPE_MIN_GAP_Y = 130.0f;                       // Điểm tâm gap tối thiểu cách mép trên
    inline constexpr float PIPE_MAX_GAP_Y = GROUND_Y - 130.0f;            // Điểm tâm gap tối đa cách mặt đất (398.0f)

    // Collision & Shrunk Hitbox settings (tạo cảm giác va chạm công bằng cho người chơi)
    inline constexpr float HITBOX_BIRD_INSET_X = 6.0f;   // Co lùi mép ngang của chim (bỏ lông đuôi/mỏ thừa)
    inline constexpr float HITBOX_BIRD_INSET_Y = 5.0f;   // Co lùi mép dọc của chim (bỏ mào/chân thừa)
    inline constexpr float HITBOX_PIPE_INSET_X = 4.0f;   // Co lùi mép hai bên thành ống
    inline constexpr float HITBOX_PIPE_INSET_Y = 4.0f;   // Co lùi mép miệng ống
    inline constexpr float HITBOX_GROUND_INSET_Y = 4.0f; // Co lùi mặt trên của cỏ
    inline constexpr bool DEBUG_HITBOXES_DEFAULT = false;
    // PowerUp
    inline constexpr float Rate_spawn_giftbox = 0.05f; 
}

#endif