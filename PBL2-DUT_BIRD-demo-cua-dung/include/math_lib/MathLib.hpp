#ifndef MATH_LIB_HPP
#define MATH_LIB_HPP

namespace math_lib {

// Hằng số Pi và các bội số
inline constexpr double PI = 3.14159265358979323846;
inline constexpr double TWO_PI = 2.0 * PI;
inline constexpr double HALF_PI = PI / 2.0;

/**
 * @brief Thay thế std::max từ <algorithm>
 * Trả về giá trị lớn hơn giữa a và b. Nếu bằng nhau, trả về a.
 */
template <typename T>
constexpr const T& max(const T& a, const T& b) {
    return (a < b) ? b : a;
}

/**
 * @brief Thay thế std::min từ <algorithm>
 * Trả về giá trị nhỏ hơn giữa a và b. Nếu bằng nhau, trả về a.
 */
template <typename T>
constexpr const T& min(const T& a, const T& b) {
    return (b < a) ? b : a;
}
/**
<<<<<<< Updated upstream
=======
 * @brief Thay thế std::clamp từ <algorithm>
 * Giới hạn val trong khoảng [low, high].
 */
template <typename T>
constexpr const T& clamp(const T& val, const T& low, const T& high) {
    return (val < low) ? low : ((val > high) ? high : val);
}

/**
 * @brief Thay thế std::sin từ <cmath>
 * Tính sin(x) với x tính bằng radian theo chuỗi Maclaurin kết hợp chuẩn hóa góc.
 */
float sin(float x);
double sin(double x);

/**
 * @brief Thay thế std::cos từ <cmath>
 * Tính cos(x) với x tính bằng radian.
 */
float cos(float x);
double cos(double x);

/**
 * @brief Bộ sinh số ngẫu nhiên giả (PRNG) tự viết dựa trên thuật toán Xorshift64
 * Hoàn toàn độc lập, không dùng <random> hay <cstdlib>.
 */
void setRandomSeed(unsigned long long seed);
unsigned long long randomU64();
float randomFloat01();                       // [0.0f, 1.0f)
float randomFloat(float min, float max);     // [min, max]
int randomInt(int min, int max);             // [min, max]

} // namespace math_lib

#endif // MATH_LIB_HPP

