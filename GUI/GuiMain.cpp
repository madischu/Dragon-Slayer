#include "ActionStack.h"
#include "Player.h"
#include "WorldMap.h"

#include <sstream>
#include <string>
#include <vector>
#include <windows.h>

namespace
{
    constexpr int IdTravel = 1001;
    constexpr int IdInteract = 1002;
    constexpr int IdQuests = 1003;
    constexpr int IdInventory = 1004;
    constexpr int IdLog = 1005;
    constexpr int IdQuit = 1006;
    constexpr int IdPathCombo = 1007;

    std::wstring widen(const std::string& text)
    {
        return std::wstring(text.begin(), text.end());
    }

    class GuiGame
    {
    public:
        GuiGame()
        {
            worldMap.buildDefaultMap();
            addAction("Started a new game");
        }

        std::string getStatusText() const
        {
            std::ostringstream output;
            output << "    Location: " << worldMap.getCurrentLocation()
                   << "    Health: " << player.getHealth()
                   << "    Gold: " << player.getGold()
                   << "    XP: " << player.getXP()
                   << "    Weapon: " << player.getCurrentWeapon().getName()
                   << "    Items: " << player.getInventory().size();
            return output.str();
        }

        std::vector<std::string> getAvailablePaths() const
        {
            return worldMap.getAvailablePaths();
        }

        std::string travelToPath(int pathChoice)
        {
            std::string previousLocation = worldMap.getCurrentLocation();

            if (!worldMap.moveToLocation(pathChoice))
            {
                return "Choose a valid destination first.";
            }

            std::string newLocation = worldMap.getCurrentLocation();
            addAction("Moved from " + previousLocation + " to " + newLocation);
            return "Traveled to " + newLocation + ".";
        }

        std::string interact()
        {
            std::string location = worldMap.getCurrentLocation();

            if (location == "Town Square")
            {
                addAction("Visited Town Square");
                return "You are standing in the Town Square.";
            }

            if (location == "Store")
            {
                addAction("Opened Store from GUI");
                return "Store GUI is ready for the next step.";
            }

            if (location == "Dragon's Lair")
            {
                addAction("Entered the Dragon's Lair from GUI");
                return "Dragon combat will be connected after the combat loop is separated from console input.";
            }

            addAction("Explored " + location + " from GUI");
            return "Combat encounters for " + location + " will be connected after the combat loop is separated from console input.";
        }

        std::string getQuestsText()
        {
            std::ostringstream output;
            output << "View Quests Here";
            return output.str();
        }

        std::string getInventoryText()
        {
            std::ostringstream output;
            output << "Inventory\r\n";

            std::vector<std::string> knownItems =
            {
                "Stick",
                "Dagger",
                "Claw Hammer",
                "Sword",
                "Health Potion",
                "Leather Armor",
                "Slime Gel",
                "Beast Fang",
                "Ghoul Bone",
                "Dragon Scale"
            };

            bool foundItem = false;

            for (const std::string& itemName : knownItems)
            {
                if (player.getInventory().containsItem(itemName))
                {
                    output << "- " << itemName << "\r\n";
                    foundItem = true;
                }
            }

            if (!foundItem)
            {
                output << "No items.";
            }

            return output.str();
        }

        std::string getLogText() const
        {
            std::ostringstream output;
            output << "Recent Actions\r\n";

            for (int i = static_cast<int>(actions.size()) - 1; i >= 0; --i)
            {
                output << "- " << actions[i] << "\r\n";
            }

            return output.str();
        }

    private:
        Player player;
        WorldMap worldMap;
        ActionStack actionLog;
        std::vector<std::string> actions;

        void addAction(const std::string& action)
        {
            actionLog.push(action);
            actions.push_back(action);
        }
    };

    GuiGame game;
    HWND statusLabel = nullptr;
    HWND outputBox = nullptr;
    HWND pathCombo = nullptr;

    void setText(HWND handle, const std::string& text)
    {
        std::wstring wideText = widen(text);
        SetWindowTextW(handle, wideText.c_str());
    }

    void refreshStatus()
    {
        setText(statusLabel, game.getStatusText());

        SendMessageW(pathCombo, CB_RESETCONTENT, 0, 0);

        for (const std::string& path : game.getAvailablePaths())
        {
            std::wstring widePath = widen(path);
            SendMessageW(pathCombo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(widePath.c_str()));
        }

        SendMessageW(pathCombo, CB_SETCURSEL, 0, 0);
    }

    HWND createButton(HWND parent, int id, const wchar_t* text)
    {
        return CreateWindowW(
            L"BUTTON",
            text,
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            0,
            0,
            120,
            32,
            parent,
            reinterpret_cast<HMENU>(static_cast<INT_PTR>(id)),
            GetModuleHandleW(nullptr),
            nullptr);
    }

    void layoutWindow(HWND window)
    {
        RECT rect{};
        GetClientRect(window, &rect);

        int width = rect.right - rect.left;
        int height = rect.bottom - rect.top;
        int margin = 16;
        int buttonWidth = 150;
        int buttonHeight = 34;
        int y = margin;

        MoveWindow(statusLabel, margin, y, width - (margin * 2), 24, TRUE);
        y += 36;

        MoveWindow(GetDlgItem(window, IdPathCombo), margin, y, buttonWidth, 220, TRUE);
        y += buttonHeight + 12;

        int buttonY = y;

        for (int id : { IdTravel, IdInteract, IdQuests, IdInventory, IdLog, IdQuit })
        {
            MoveWindow(GetDlgItem(window, id), margin, buttonY, buttonWidth, buttonHeight, TRUE);
            buttonY += buttonHeight + 8;
        }

        int outputX = margin + buttonWidth + margin;
        MoveWindow(outputBox, outputX, y - buttonHeight - 12, width - outputX - margin, height - y + buttonHeight - 4, TRUE);
    }

    LRESULT CALLBACK windowProcedure(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
        case WM_CREATE:
            statusLabel = CreateWindowW(
                L"STATIC",
                L"",
                WS_CHILD | WS_VISIBLE,
                0,
                0,
                0,
                0,
                window,
                nullptr,
                GetModuleHandleW(nullptr),
                nullptr);

            pathCombo = CreateWindowW(
                L"COMBOBOX",
                L"",
                WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_VSCROLL,
                0,
                0,
                0,
                0,
                window,
                reinterpret_cast<HMENU>(static_cast<INT_PTR>(IdPathCombo)),
                GetModuleHandleW(nullptr),
                nullptr);

            createButton(window, IdTravel, L"Travel");
            createButton(window, IdInteract, L"Interact");
            createButton(window, IdQuests, L"Quests");
            createButton(window, IdInventory, L"Inventory");
            createButton(window, IdLog, L"Action Log");
            createButton(window, IdQuit, L"Quit");

            outputBox = CreateWindowW(
                L"EDIT",
                L"",
                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_MULTILINE | ES_READONLY | ES_AUTOVSCROLL | WS_VSCROLL,
                0,
                0,
                0,
                0,
                window,
                nullptr,
                GetModuleHandleW(nullptr),
                nullptr);

            refreshStatus();
            setText(outputBox, "Welcome to Dragon Slayer.");
            return 0;

        case WM_SIZE:
            layoutWindow(window);
            return 0;

        case WM_COMMAND:
            switch (LOWORD(wParam))
            {
            case IdTravel:
            {
                int selectedIndex = static_cast<int>(SendMessageW(pathCombo, CB_GETCURSEL, 0, 0));
                setText(outputBox, game.travelToPath(selectedIndex + 1));
                refreshStatus();
                return 0;
            }
            case IdInteract:
                setText(outputBox, game.interact());
                refreshStatus();
                return 0;
            case IdQuests:
                setText(outputBox, game.getQuestsText());
                return 0;
            case IdInventory:
                setText(outputBox, game.getInventoryText());
                return 0;
            case IdLog:
                setText(outputBox, game.getLogText());
                return 0;
            case IdQuit:
                DestroyWindow(window);
                return 0;
            default:
                break;
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        default:
            break;
        }

        return DefWindowProcW(window, message, wParam, lParam);
    }
}

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR, int showCommand)
{
    const wchar_t className[] = L"DragonSlayerGuiWindow";

    WNDCLASSW windowClass{};
    windowClass.lpfnWndProc = windowProcedure;
    windowClass.hInstance = instance;
    windowClass.lpszClassName = className;
    windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    windowClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);

    RegisterClassW(&windowClass);

    HWND window = CreateWindowExW(
        0,
        className,
        L"Dragon Slayer",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        760,
        460,
        nullptr,
        nullptr,
        instance,
        nullptr);

    if (window == nullptr)
    {
        return 0;
    }

    ShowWindow(window, showCommand);

    MSG message{};
    while (GetMessageW(&message, nullptr, 0, 0) > 0)
    {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }

    return static_cast<int>(message.wParam);
}
