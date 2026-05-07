#include "ConsoleColor.h"

#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

namespace
{
#ifdef _WIN32
    bool enableVirtualTerminalColors()
    {
        // Windows needs this mode enabled before RGB ANSI colors such as LightPurple can render.
        HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD consoleMode;

        if (console == INVALID_HANDLE_VALUE || !GetConsoleMode(console, &consoleMode))
        {
            return false;
        }

        if ((consoleMode & ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0)
        {
            return true;
        }

        return SetConsoleMode(console, consoleMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING) != 0;
    }

    const char* getAnsiCode(ConsoleColor::Color color)
    {
        switch (color)
        {
            case ConsoleColor::Color::LightPurple:
                return "\033[38;2;205;170;255m";
            case ConsoleColor::Color::Brown:
                return "\033[38;2;139;90;43m";
            case ConsoleColor::Color::LightBrown:
                return "\033[38;2;181;137;91m";
            case ConsoleColor::Color::Default:
            default:
                return "\033[0m";
        }
    }

    WORD getWindowsAttribute(ConsoleColor::Color color)
    {
        switch (color)
        {
            case ConsoleColor::Color::Black:
                return 0;
            case ConsoleColor::Color::DarkBlue:
                return FOREGROUND_BLUE;
            case ConsoleColor::Color::DarkGreen:
                return FOREGROUND_GREEN;
            case ConsoleColor::Color::DarkCyan:
                return FOREGROUND_GREEN | FOREGROUND_BLUE;
            case ConsoleColor::Color::DarkRed:
                return FOREGROUND_RED;
            case ConsoleColor::Color::DarkMagenta:
                return FOREGROUND_RED | FOREGROUND_BLUE;
            case ConsoleColor::Color::DarkYellow:
            case ConsoleColor::Color::Brown:
                return FOREGROUND_RED | FOREGROUND_GREEN;
            case ConsoleColor::Color::Gray:
                return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
            case ConsoleColor::Color::DarkGray:
                return FOREGROUND_INTENSITY;
            case ConsoleColor::Color::Blue:
                return FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            case ConsoleColor::Color::Green:
                return FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            case ConsoleColor::Color::Cyan:
                return FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            case ConsoleColor::Color::Red:
                return FOREGROUND_RED | FOREGROUND_INTENSITY;
            case ConsoleColor::Color::Magenta:
            case ConsoleColor::Color::LightPurple:
                return FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            case ConsoleColor::Color::Yellow:
            case ConsoleColor::Color::Gold:
            case ConsoleColor::Color::LightBrown:
                return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
            case ConsoleColor::Color::White:
                return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            case ConsoleColor::Color::Default:
            default:
                break;
        }

        return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    }

    HANDLE getConsole()
    {
        return GetStdHandle(STD_OUTPUT_HANDLE);
    }

    WORD getDefaultAttribute()
    {
        HANDLE console = getConsole();
        CONSOLE_SCREEN_BUFFER_INFO consoleInfo;

        if (console != INVALID_HANDLE_VALUE && GetConsoleScreenBufferInfo(console, &consoleInfo))
        {
            return consoleInfo.wAttributes;
        }

        return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
    }
#else
    const char* getAnsiCode(ConsoleColor::Color color)
    {
        switch (color)
        {
            case ConsoleColor::Color::Black:
                return "\033[30m";
            case ConsoleColor::Color::DarkBlue:
                return "\033[34m";
            case ConsoleColor::Color::DarkGreen:
                return "\033[32m";
            case ConsoleColor::Color::DarkCyan:
                return "\033[36m";
            case ConsoleColor::Color::DarkRed:
                return "\033[31m";
            case ConsoleColor::Color::DarkMagenta:
                return "\033[35m";
            case ConsoleColor::Color::DarkYellow:
                return "\033[33m";
            case ConsoleColor::Color::Gray:
                return "\033[37m";
            case ConsoleColor::Color::DarkGray:
                return "\033[90m";
            case ConsoleColor::Color::Blue:
                return "\033[94m";
            case ConsoleColor::Color::Green:
                return "\033[92m";
            case ConsoleColor::Color::Cyan:
                return "\033[96m";
            case ConsoleColor::Color::Red:
                return "\033[91m";
            case ConsoleColor::Color::Magenta:
                return "\033[95m";
            case ConsoleColor::Color::LightPurple:
                return "\033[38;2;205;170;255m";
            case ConsoleColor::Color::Yellow:
                return "\033[93m";
            case ConsoleColor::Color::Gold:
                return "\033[33m";
            case ConsoleColor::Color::Brown:
                return "\033[38;2;139;90;43m";
            case ConsoleColor::Color::LightBrown:
                return "\033[38;2;181;137;91m";
            case ConsoleColor::Color::White:
                return "\033[97m";
            case ConsoleColor::Color::Default:
            default:
                return "\033[0m";
        }
    }
#endif
}

void ConsoleColor::set(Color color)
{
#ifdef _WIN32
    static const WORD defaultAttribute = getDefaultAttribute();
    HANDLE console = getConsole();

    if ((color == Color::LightPurple || color == Color::Brown || color == Color::LightBrown)
        && enableVirtualTerminalColors())
    {
        std::cout << getAnsiCode(color);
        return;
    }

    if (console == INVALID_HANDLE_VALUE)
    {
        return;
    }

    SetConsoleTextAttribute(console, color == Color::Default ? defaultAttribute : getWindowsAttribute(color));
#else
    std::cout << getAnsiCode(color);
#endif
}

void ConsoleColor::reset()
{
#ifdef _WIN32
    if (enableVirtualTerminalColors())
    {
        std::cout << "\033[0m";
        return;
    }
#endif

    set(Color::Default);
}

void ConsoleColor::print(const std::string& message, Color color)
{
    set(color);
    std::cout << message;
    reset();
}

void ConsoleColor::printLine(const std::string& message, Color color)
{
    print(message, color);
    std::cout << std::endl;
}
