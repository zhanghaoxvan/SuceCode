#pragma once

#include <string>

namespace kernel{
    namespace Color{
        // 重置
        constexpr const char* RESET = "\033[0m";

        // 文本颜色
        constexpr const char* BLACK = "\033[30m";
        constexpr const char* RED = "\033[31m";
        constexpr const char* GREEN = "\033[32m";
        constexpr const char* YELLOW = "\033[33m";
        constexpr const char* BLUE = "\033[34m";
        constexpr const char* MAGENTA = "\033[35m";
        constexpr const char* CYAN = "\033[36m";
        constexpr const char* WHITE = "\033[37m";

        // 高亮文本颜色
        constexpr const char* BRIGHT_BLACK = "\033[90m";
        constexpr const char* BRIGHT_RED = "\033[91m";
        constexpr const char* BRIGHT_GREEN = "\033[92m";
        constexpr const char* BRIGHT_YELLOW = "\033[93m";
        constexpr const char* BRIGHT_BLUE = "\033[94m";
        constexpr const char* BRIGHT_MAGENTA = "\033[95m";
        constexpr const char* BRIGHT_CYAN = "\033[96m";
        constexpr const char* BRIGHT_WHITE = "\033[97m";

        // 文本样式
        constexpr const char* BOLD = "\033[1m";
        constexpr const char* ITALIC = "\033[3m";
        constexpr const char* UNDERLINE = "\033[4m";
    }

    inline std::string colorize(const std::string& text, const std::string& color)
    {
        return color + text + Color::RESET;
    }

}