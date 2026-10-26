cmake_minimum_required(VERSION 3.21)
project(DutBird LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Tránh dùng cờ -isystem cho imported targets trên MSYS2/MinGW để không làm gãy include_next
set(CMAKE_NO_SYSTEM_FROM_IMPORTED ON)

# Thêm cờ cảnh báo để đảm bảo chất lượng code
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Include thư mục header
include_directories(include)

# Gom tất cả các file mã nguồn
file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
)

# Tự động hỗ trợ các đường dẫn MSYS2 phổ biến trên Windows nếu chưa thiết lập
if(WIN32)
    list(APPEND CMAKE_PREFIX_PATH "C:/msys64/ucrt64" "C:/msys64/mingw64")
endif()

# Tìm kiếm thư viện SFML phiên bản 3 (tương thích SFML 3.0 / 3.1)
find_package(SFML 3 COMPONENTS Graphics Window System Audio REQUIRED)

# Tạo file thực thi
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE include)

# Liên kết SFML 3 theo chuẩn namespace target chính thức
target_link_libraries(${PROJECT_NAME} PRIVATE 
    SFML::Graphics 
    SFML::Window 
    SFML::System 
    SFML::Audio
)

# Tự động copy các DLL cần thiết vào thư mục build khi biên dịch
if(WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND_EXPAND_LISTS
    )
    if(MINGW)
        get_filename_component(MINGW_BIN_DIR ${CMAKE_CXX_COMPILER} DIRECTORY)
        set(EXTRA_DLLS
            libfreetype-6.dll libbrotlidec.dll libbrotlicommon.dll libbz2-1.dll
            libpng16-16.dll zlib1.dll libharfbuzz-0.dll libglib-2.0-0.dll
            libgraphite2.dll libintl-8.dll libiconv-2.dll libpcre2-8-0.dll
            libgcc_s_seh-1.dll libstdc++-6.dll libwinpthread-1.dll
        )
        foreach(DLL_NAME ${EXTRA_DLLS})
            if(EXISTS "${MINGW_BIN_DIR}/${DLL_NAME}")
                add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_if_different
                        "${MINGW_BIN_DIR}/${DLL_NAME}"
                        $<TARGET_FILE_DIR:${PROJECT_NAME}>
                )
            endif()
        endforeach()
    endif()
endif()
