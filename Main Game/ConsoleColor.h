#ifndef CONSOLECOLOR_H
#define CONSOLECOLOR_H

#include <iosfwd>
#include <string>

namespace ConsoleColor
{
    enum class Color
    {
        Default,
        Black,
        DarkBlue,
        DarkGreen,
        DarkCyan,
        DarkRed,
        DarkMagenta,
        DarkYellow,
        Gray,
        DarkGray,
        Blue,
        Green,
        Cyan,
        Red,
        Magenta,
        Yellow,
        Gold,
        White
    };

    void set(Color color);
    void reset();
    void print(const std::string& message, Color color);
    void printLine(const std::string& message, Color color);
}

#endif
