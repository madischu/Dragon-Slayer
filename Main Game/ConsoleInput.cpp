#include "ConsoleInput.h"
#include "ConsoleColor.h"

#include <iostream>
#include <limits>

void ConsoleInput::printInvalidInput()
{
    ConsoleColor::printLine("Invalid input. Try again.", ConsoleColor::Color::Red);
}

bool ConsoleInput::readInt(int& value)
{
    // Keep retrying after non-numeric input so callers do not have to duplicate cleanup logic.
    while (true)
    {
        if (std::cin >> value)
        {
            return true;
        }

        if (std::cin.eof())
        {
            return false;
        }

        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        printInvalidInput();
    }
}
