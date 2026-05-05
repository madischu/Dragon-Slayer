#include "GuiGame.h"

#include <array>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <string>
#include <utility>
#include <vector>
#include <windows.h>
#include <windowsx.h>

namespace
{
    constexpr int IdMap = 1001;
    constexpr int IdInteract = 1002;
    constexpr int IdQuests = 1003;
    constexpr int IdInventory = 1004;
    constexpr int IdLog = 1005;
    constexpr int IdQuit = 1006;

    constexpr COLORREF DarkBackground = RGB(23, 24, 28);
    constexpr COLORREF PanelBackground = RGB(232, 226, 214);
    constexpr COLORREF TextBlack = RGB(15, 15, 15);
    constexpr COLORREF TextWhite = RGB(245, 245, 245);
    constexpr COLORREF ButtonOrange = RGB(255, 174, 30);
    constexpr COLORREF Parchment = RGB(218, 196, 151);
    constexpr COLORREF ParchmentDark = RGB(166, 132, 82);
    constexpr COLORREF RouteShadow = RGB(96, 72, 47);
    constexpr COLORREF RouteLine = RGB(235, 184, 85);
    constexpr COLORREF CurrentMarker = RGB(32, 110, 73);
    constexpr COLORREF ReachableMarker = RGB(51, 112, 169);
    constexpr COLORREF LockedMarker = RGB(115, 104, 88);


    struct MapLocation
    {
        const char* name;
        const wchar_t* symbol;
        int x;
        int y;
    };

    constexpr std::array<MapLocation, 6> MapLocations =
    {
        MapLocation { "Town Square", L"T", 120, 170 },
        MapLocation { "Store", L"$", 120, 60 },
        MapLocation { "Caves", L"C", 280, 170 },
        MapLocation { "Dark Forest", L"F", 280, 95 },
        MapLocation { "Ancient Ruins", L"R", 280, 25 },
        MapLocation { "Dragon's Lair", L"D", 600, 95 }
    };

    std::wstring widen(const std::string& text)
    {
        return std::wstring(text.begin(), text.end());
    }

    bool containsName(const std::vector<std::string>& names, const std::string& target)
    {
        for (const std::string& name : names)
        {
            if (name == target)
            {
                return true;
            }
        }

        return false;
    }

    GuiGame game;
    HWND statusLabel = nullptr;
    HWND outputPanel = nullptr;
    HWND outputBox = nullptr;
    HWND mapView = nullptr;
    bool isMapVisible = false;
    HBRUSH darkBackgroundBrush = nullptr;
    HBRUSH panelBackgroundBrush = nullptr;
    HBRUSH buttonBrush = nullptr;
    HFONT outputFont = nullptr;

    void setText(HWND handle, const std::string& text)
    {
        std::string normalizedText;
        normalizedText.reserve(text.size());

        for (char character : text)
        {
            if (character == '\n' && (normalizedText.empty() || normalizedText.back() != '\r'))
            {
                normalizedText += '\r';
            }

            normalizedText += character;
        }

        std::wstring wideText = widen(normalizedText);
        SetWindowTextW(handle, wideText.c_str());
    }

    void refreshStatus()
    {
        setText(statusLabel, game.getStatusText());
        if (isMapVisible)
        {
            InvalidateRect(mapView, nullptr, TRUE);
        }
    }

    void setButtonLabel(HWND window, int id, const wchar_t* label)
    {
        SetWindowTextW(GetDlgItem(window, id), label);
    }

    void updateButtonLabels(HWND window)
    {
        if (game.getMode() == GuiMode::Store)
        {
            setButtonLabel(window, IdMap, L"Buy Health");
            setButtonLabel(window, IdInteract, L"Buy Weapon");
            setButtonLabel(window, IdQuests, L"Sell Weapon");
            setButtonLabel(window, IdInventory, L"Buy Potion");
            setButtonLabel(window, IdLog, L"Buy Armor");
            setButtonLabel(window, IdQuit, L"Leave Store");
            return;
        }

        if (game.getMode() == GuiMode::Combat)
        {
            setButtonLabel(window, IdMap, L"Attack");
            setButtonLabel(window, IdInteract, L"Dodge");
            setButtonLabel(window, IdQuests, L"Potion");
            setButtonLabel(window, IdInventory, L"Run Away");
            setButtonLabel(window, IdLog, L"Inventory");
            setButtonLabel(window, IdQuit, L"Action Log");
            return;
        }

        if (game.getMode() == GuiMode::CaveChoice)
        {
            setButtonLabel(window, IdMap, L"Slime");
            setButtonLabel(window, IdInteract, L"Fanged Beast");
            setButtonLabel(window, IdQuests, L"Leave Caves");
            setButtonLabel(window, IdInventory, L"Inventory");
            setButtonLabel(window, IdLog, L"Action Log");
            setButtonLabel(window, IdQuit, L"Map");
            return;
        }

        setButtonLabel(window, IdMap, L"Map");
        setButtonLabel(window, IdInteract, L"Interact");
        setButtonLabel(window, IdQuests, L"Quests");
        setButtonLabel(window, IdInventory, L"Inventory");
        setButtonLabel(window, IdLog, L"Action Log");
        setButtonLabel(window, IdQuit, L"Quit");
    }

    HWND createButton(HWND parent, int id, const wchar_t* text)
    {
        return CreateWindowW(
            L"BUTTON",
            text,
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
            0,
            0,
            120,
            32,
            parent,
            reinterpret_cast<HMENU>(static_cast<INT_PTR>(id)),
            GetModuleHandleW(nullptr),
            nullptr);
    }

    void drawButton(const DRAWITEMSTRUCT* drawItem)
    {
        RECT buttonRect = drawItem->rcItem;
        FillRect(drawItem->hDC, &buttonRect, buttonBrush);

        UINT edge = (drawItem->itemState & ODS_SELECTED) ? EDGE_SUNKEN : EDGE_RAISED;
        DrawEdge(drawItem->hDC, &buttonRect, edge, BF_RECT);

        wchar_t buttonText[128]{};
        GetWindowTextW(drawItem->hwndItem, buttonText, 128);

        SetBkMode(drawItem->hDC, TRANSPARENT);
        SetTextColor(drawItem->hDC, TextBlack);
        DrawTextW(drawItem->hDC, buttonText, -1, &buttonRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
    }

    POINT getMapPoint(const RECT& clientRect, const MapLocation& location)
    {
        constexpr int designWidth = 700;
        constexpr int designHeight = 300;
        constexpr int padding = 36;

        int width = clientRect.right - clientRect.left - (padding * 2);
        int height = clientRect.bottom - clientRect.top - (padding * 2);

        POINT point{};
        point.x = padding + (location.x * width) / designWidth;
        point.y = padding + (location.y * height) / designHeight;
        return point;
    }

    const MapLocation* findMapLocation(const std::string& name)
    {
        for (const MapLocation& location : MapLocations)
        {
            if (name == location.name)
            {
                return &location;
            }
        }

        return nullptr;
    }

    void drawMapLine(HDC deviceContext, const RECT& clientRect, const MapLocation& from, const MapLocation& to)
    {
        POINT fromPoint = getMapPoint(clientRect, from);
        POINT toPoint = getMapPoint(clientRect, to);

        HPEN shadowPen = CreatePen(PS_SOLID, 9, RouteShadow);
        HPEN oldPen = static_cast<HPEN>(SelectObject(deviceContext, shadowPen));
        MoveToEx(deviceContext, fromPoint.x, fromPoint.y + 2, nullptr);
        LineTo(deviceContext, toPoint.x, toPoint.y + 2);

        HPEN routePen = CreatePen(PS_SOLID, 5, RouteLine);
        SelectObject(deviceContext, routePen);
        MoveToEx(deviceContext, fromPoint.x, fromPoint.y, nullptr);
        LineTo(deviceContext, toPoint.x, toPoint.y);

        SelectObject(deviceContext, oldPen);
        DeleteObject(shadowPen);
        DeleteObject(routePen);
    }

    void drawMapBackground(HDC deviceContext, const RECT& clientRect)
    {
        FillRect(deviceContext, &clientRect, panelBackgroundBrush);

        RECT mapRect = clientRect;
        InflateRect(&mapRect, -10, -10);
        int mapWidth = mapRect.right - mapRect.left;
        int mapHeight = mapRect.bottom - mapRect.top;

        HBRUSH parchmentBrush = CreateSolidBrush(Parchment);
        FillRect(deviceContext, &mapRect, parchmentBrush);
        DeleteObject(parchmentBrush);

        HPEN borderPen = CreatePen(PS_SOLID, 3, ParchmentDark);
        HPEN oldPen = static_cast<HPEN>(SelectObject(deviceContext, borderPen));
        HBRUSH oldBrush = static_cast<HBRUSH>(SelectObject(deviceContext, GetStockObject(NULL_BRUSH)));
        RoundRect(deviceContext, mapRect.left, mapRect.top, mapRect.right, mapRect.bottom, 18, 18);
        SelectObject(deviceContext, oldBrush);
        SelectObject(deviceContext, oldPen);
        DeleteObject(borderPen);

        HBRUSH forestBrush = CreateSolidBrush(RGB(103, 130, 82));
        HBRUSH oldRegionBrush = static_cast<HBRUSH>(SelectObject(deviceContext, forestBrush));
        Ellipse(
            deviceContext,
            mapRect.left + (mapWidth * 47) / 100,
            mapRect.top + (mapHeight * 30) / 100,
            mapRect.left + (mapWidth * 74) / 100,
            mapRect.top + (mapHeight * 82) / 100);
        SelectObject(deviceContext, oldRegionBrush);
        DeleteObject(forestBrush);

        HBRUSH mountainBrush = CreateSolidBrush(RGB(135, 126, 111));
        oldRegionBrush = static_cast<HBRUSH>(SelectObject(deviceContext, mountainBrush));
        POINT mountains[3]
        {
            { mapRect.left + (mapWidth * 30) / 100, mapRect.top + (mapHeight * 73) / 100 },
            { mapRect.left + (mapWidth * 40) / 100, mapRect.top + (mapHeight * 37) / 100 },
            { mapRect.left + (mapWidth * 50) / 100, mapRect.top + (mapHeight * 73) / 100 }
        };
        Polygon(deviceContext, mountains, 3);
        SelectObject(deviceContext, oldRegionBrush);
        DeleteObject(mountainBrush);

        SetBkMode(deviceContext, TRANSPARENT);
        SetTextColor(deviceContext, RGB(92, 66, 38));
        RECT titleRect { mapRect.left + 16, mapRect.top + 12, mapRect.right - 16, mapRect.top + 42 };
        DrawTextW(deviceContext, L"World Map", -1, &titleRect, DT_CENTER | DT_SINGLELINE);
    }

    void drawRoundedOutputPanel(HDC deviceContext, const RECT& clientRect)
    {
        FillRect(deviceContext, &clientRect, panelBackgroundBrush);

        RECT outputRect = clientRect;
        InflateRect(&outputRect, -6, -6);

        HBRUSH outputBrush = CreateSolidBrush(DarkBackground);
        HBRUSH oldBrush = static_cast<HBRUSH>(SelectObject(deviceContext, outputBrush));
        HPEN borderPen = CreatePen(PS_SOLID, 3, PanelBackground);
        HPEN oldPen = static_cast<HPEN>(SelectObject(deviceContext, borderPen));

        RoundRect(deviceContext, outputRect.left, outputRect.top, outputRect.right, outputRect.bottom, 18, 18);

        SelectObject(deviceContext, oldBrush);
        SelectObject(deviceContext, oldPen);
        DeleteObject(outputBrush);
        DeleteObject(borderPen);
    }

    void drawLocation(HDC deviceContext, const RECT& clientRect, const MapLocation& location)
    {
        constexpr int radius = 28;
        POINT point = getMapPoint(clientRect, location);
        std::string currentLocation = game.getCurrentLocation();
        std::vector<std::string> availablePaths = game.getAvailablePaths();

        bool isCurrent = currentLocation == location.name;
        bool isReachable = containsName(availablePaths, location.name);

        COLORREF fillColor = LockedMarker;

        if (isReachable)
        {
            fillColor = ReachableMarker;
        }

        if (isCurrent)
        {
            fillColor = CurrentMarker;
        }

        HBRUSH shadowBrush = CreateSolidBrush(RGB(91, 71, 48));
        HBRUSH oldBrush = static_cast<HBRUSH>(SelectObject(deviceContext, shadowBrush));
        Ellipse(
            deviceContext,
            point.x - radius + 4,
            point.y - radius + 5,
            point.x + radius + 4,
            point.y + radius + 5);
        SelectObject(deviceContext, oldBrush);
        DeleteObject(shadowBrush);

        HBRUSH brush = CreateSolidBrush(fillColor);
        oldBrush = static_cast<HBRUSH>(SelectObject(deviceContext, brush));

        HPEN pen = CreatePen(PS_SOLID, isCurrent ? 4 : 2, RGB(54, 43, 31));
        HPEN oldPen = static_cast<HPEN>(SelectObject(deviceContext, pen));

        Ellipse(
            deviceContext,
            point.x - radius,
            point.y - radius,
            point.x + radius,
            point.y + radius);

        SetBkMode(deviceContext, TRANSPARENT);
        SetTextColor(deviceContext, TextWhite);
        RECT symbolRect
        {
            point.x - radius,
            point.y - 11,
            point.x + radius,
            point.y + 15
        };
        DrawTextW(deviceContext, location.symbol, -1, &symbolRect, DT_CENTER | DT_SINGLELINE);

        SetTextColor(deviceContext, RGB(38, 28, 18));

        std::wstring name = widen(location.name);
        RECT labelRect
        {
            point.x - 76,
            point.y + radius + 8,
            point.x + 76,
            point.y + radius + 54
        };
        DrawTextW(deviceContext, name.c_str(), -1, &labelRect, DT_CENTER | DT_WORDBREAK);

        SelectObject(deviceContext, oldBrush);
        SelectObject(deviceContext, oldPen);
        DeleteObject(brush);
        DeleteObject(pen);
    }

    const MapLocation* hitTestMapLocation(HWND window, int x, int y)
    {
        constexpr int hitRadius = 30;
        RECT clientRect{};
        GetClientRect(window, &clientRect);

        for (const MapLocation& location : MapLocations)
        {
            POINT point = getMapPoint(clientRect, location);
            int dx = x - point.x;
            int dy = y - point.y;

            if ((dx * dx) + (dy * dy) <= hitRadius * hitRadius)
            {
                return &location;
            }
        }

        return nullptr;
    }

    void layoutWindow(HWND window);

    LRESULT CALLBACK outputProcedure(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
        case WM_SIZE:
        {
            RECT rect{};
            GetClientRect(window, &rect);
            MoveWindow(outputBox, 20, 20, rect.right - 40, rect.bottom - 40, TRUE);
            return 0;
        }

        case WM_PAINT:
        {
            PAINTSTRUCT paint{};
            HDC deviceContext = BeginPaint(window, &paint);

            RECT clientRect{};
            GetClientRect(window, &clientRect);
            drawRoundedOutputPanel(deviceContext, clientRect);

            EndPaint(window, &paint);
            return 0;
        }

        case WM_CTLCOLORSTATIC:
        case WM_CTLCOLOREDIT:
        {
            HDC deviceContext = reinterpret_cast<HDC>(wParam);
            SetTextColor(deviceContext, TextWhite);
            SetBkColor(deviceContext, DarkBackground);
            return reinterpret_cast<LRESULT>(darkBackgroundBrush);
        }

        default:
            break;
        }

        return DefWindowProcW(window, message, wParam, lParam);
    }

    LRESULT CALLBACK mapProcedure(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
        case WM_LBUTTONDOWN:
        {
            int x = GET_X_LPARAM(lParam);
            int y = GET_Y_LPARAM(lParam);
            const MapLocation* location = hitTestMapLocation(window, x, y);

            if (location != nullptr)
            {
                std::string message;
                bool traveled = game.travelToLocation(location->name, message);
                setText(outputBox, message);

                if (traveled)
                {
                    isMapVisible = false;
                    layoutWindow(GetParent(window));
                    setText(outputBox, message + "\r\n\r\n" + game.interact());
                    updateButtonLabels(GetParent(window));
                }

                refreshStatus();
            }

            return 0;
        }

        case WM_PAINT:
        {
            PAINTSTRUCT paint{};
            HDC deviceContext = BeginPaint(window, &paint);

            RECT clientRect{};
            GetClientRect(window, &clientRect);

            drawMapBackground(deviceContext, clientRect);

            for (const auto& path : game.getMapPaths())
            {
                const MapLocation* from = findMapLocation(path.first);
                const MapLocation* to = findMapLocation(path.second);

                if (from != nullptr && to != nullptr)
                {
                    drawMapLine(deviceContext, clientRect, *from, *to);
                }
            }

            for (const MapLocation& location : MapLocations)
            {
                drawLocation(deviceContext, clientRect, location);
            }

            SetBkMode(deviceContext, TRANSPARENT);
            SetTextColor(deviceContext, RGB(62, 45, 27));
            RECT helpRect { 28, clientRect.bottom - 38, clientRect.right - 28, clientRect.bottom - 14 };
            DrawTextW(deviceContext, L"Where would you like to go?", -1, &helpRect, DT_CENTER | DT_SINGLELINE);

            EndPaint(window, &paint);
            return 0;
        }

        default:
            break;
        }

        return DefWindowProcW(window, message, wParam, lParam);
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

        int buttonY = y;

        for (int id : { IdMap, IdInteract, IdQuests, IdInventory, IdLog, IdQuit })
        {
            MoveWindow(GetDlgItem(window, id), margin, buttonY, buttonWidth, buttonHeight, TRUE);
            buttonY += buttonHeight + 8;
        }

        int contentX = margin + buttonWidth + margin;
        int contentWidth = width - contentX - margin;
        int contentHeight = height - y - margin;

        if (isMapVisible)
        {
            MoveWindow(mapView, contentX, y, contentWidth, contentHeight, TRUE);
            ShowWindow(mapView, SW_SHOW);
            ShowWindow(outputPanel, SW_HIDE);
            InvalidateRect(mapView, nullptr, TRUE);
        }
        else
        {
            ShowWindow(mapView, SW_HIDE);
            MoveWindow(outputPanel, contentX, y, contentWidth, contentHeight, TRUE);
            ShowWindow(outputPanel, SW_SHOW);
            InvalidateRect(outputPanel, nullptr, TRUE);
        }
    }

    LRESULT CALLBACK windowProcedure(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
        case WM_CREATE:
            darkBackgroundBrush = CreateSolidBrush(DarkBackground);
            panelBackgroundBrush = CreateSolidBrush(PanelBackground);
            buttonBrush = CreateSolidBrush(ButtonOrange);
            outputFont = CreateFontW(
                18,
                0,
                0,
                0,
                FW_BOLD,
                FALSE,
                FALSE,
                FALSE,
                DEFAULT_CHARSET,
                OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY,
                FF_ROMAN,
                L"Georgia");

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

            createButton(window, IdMap, L"Map");
            createButton(window, IdInteract, L"Interact");
            createButton(window, IdQuests, L"Quests");
            createButton(window, IdInventory, L"Inventory");
            createButton(window, IdLog, L"Action Log");
            createButton(window, IdQuit, L"Quit");
            updateButtonLabels(window);

            mapView = CreateWindowW(
                L"DragonSlayerMapView",
                L"",
                WS_CHILD | WS_BORDER,
                0,
                0,
                0,
                0,
                window,
                nullptr,
                GetModuleHandleW(nullptr),
                nullptr);

            outputPanel = CreateWindowW(
                L"DragonSlayerOutputView",
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

            outputBox = CreateWindowW(
                L"EDIT",
                L"",
                WS_CHILD | WS_VISIBLE | ES_MULTILINE | ES_READONLY | ES_AUTOVSCROLL | WS_VSCROLL,
                0,
                0,
                0,
                0,
                outputPanel,
                nullptr,
                GetModuleHandleW(nullptr),
                nullptr);

            SendMessageW(outputBox, WM_SETFONT, reinterpret_cast<WPARAM>(outputFont), TRUE);

            refreshStatus();
            layoutWindow(window);
            setText(outputBox, "Welcome to Dragon Slayer. You must defeat the dragon that is preventing people from leaving the town. You are in the Town Square.");
            return 0;

        case WM_SIZE:
            layoutWindow(window);
            return 0;

        case WM_CTLCOLORSTATIC:
        {
            HDC deviceContext = reinterpret_cast<HDC>(wParam);
            HWND control = reinterpret_cast<HWND>(lParam);

            if (control == statusLabel)
            {
                SetTextColor(deviceContext, TextBlack);
                SetBkColor(deviceContext, PanelBackground);
                return reinterpret_cast<LRESULT>(panelBackgroundBrush);
            }

            if (control == outputBox)
            {
                SetTextColor(deviceContext, TextWhite);
                SetBkColor(deviceContext, DarkBackground);
                return reinterpret_cast<LRESULT>(darkBackgroundBrush);
            }

            break;
        }

        case WM_CTLCOLOREDIT:
        {
            HDC deviceContext = reinterpret_cast<HDC>(wParam);
            SetTextColor(deviceContext, TextWhite);
            SetBkColor(deviceContext, DarkBackground);
            return reinterpret_cast<LRESULT>(darkBackgroundBrush);
        }

        case WM_DRAWITEM:
            drawButton(reinterpret_cast<DRAWITEMSTRUCT*>(lParam));
            return TRUE;

        case WM_COMMAND:
            if (game.getMode() == GuiMode::Store)
            {
                isMapVisible = false;

                switch (LOWORD(wParam))
                {
                case IdMap:
                    setText(outputBox, game.buyHealth());
                    break;
                case IdInteract:
                    setText(outputBox, game.buyWeapon());
                    break;
                case IdQuests:
                    setText(outputBox, game.sellWeapon());
                    break;
                case IdInventory:
                    setText(outputBox, game.buyPotion());
                    break;
                case IdLog:
                    setText(outputBox, game.buyArmor());
                    break;
                case IdQuit:
                    setText(outputBox, game.leaveStore());
                    isMapVisible = true;
                    break;
                default:
                    break;
                }

                updateButtonLabels(window);
                refreshStatus();
                layoutWindow(window);
                return 0;
            }

            if (game.getMode() == GuiMode::Combat)
            {
                isMapVisible = false;

                switch (LOWORD(wParam))
                {
                case IdMap:
                    setText(outputBox, game.attackMonster());
                    break;
                case IdInteract:
                    setText(outputBox, game.dodgeMonster());
                    break;
                case IdQuests:
                    setText(outputBox, game.useHealthPotion());
                    break;
                case IdInventory:
                    setText(outputBox, game.runFromCombat());
                    break;
                case IdLog:
                    setText(outputBox, game.getInventoryText());
                    break;
                case IdQuit:
                    setText(outputBox, game.getLogText());
                    break;
                default:
                    break;
                }

                updateButtonLabels(window);
                refreshStatus();
                layoutWindow(window);
                return 0;
            }

            if (game.getMode() == GuiMode::CaveChoice)
            {
                isMapVisible = false;

                switch (LOWORD(wParam))
                {
                case IdMap:
                    setText(outputBox, game.fightSlime());
                    break;
                case IdInteract:
                    setText(outputBox, game.fightFangedBeast());
                    break;
                case IdQuests:
                    setText(outputBox, game.leaveCaves());
                    break;
                case IdInventory:
                    setText(outputBox, game.getInventoryText());
                    break;
                case IdLog:
                    setText(outputBox, game.getLogText());
                    break;
                case IdQuit:
                    isMapVisible = true;
                    setText(outputBox, "Click a connected location on the map to travel.");
                    break;
                default:
                    break;
                }

                updateButtonLabels(window);
                refreshStatus();
                layoutWindow(window);
                return 0;
            }

            switch (LOWORD(wParam))
            {
            case IdMap:
                isMapVisible = true;
                setText(outputBox, "Click a connected location on the map to travel.");
                layoutWindow(window);
                InvalidateRect(mapView, nullptr, TRUE);
                return 0;
            case IdInteract:
                isMapVisible = false;
                setText(outputBox, game.interact());
                updateButtonLabels(window);
                refreshStatus();
                layoutWindow(window);
                return 0;
            case IdQuests:
                isMapVisible = false;
                setText(outputBox, game.getQuestsText());
                layoutWindow(window);
                return 0;
            case IdInventory:
                isMapVisible = false;
                setText(outputBox, game.getInventoryText());
                layoutWindow(window);
                return 0;
            case IdLog:
                isMapVisible = false;
                setText(outputBox, game.getLogText());
                layoutWindow(window);
                return 0;
            case IdQuit:
                DestroyWindow(window);
                return 0;
            default:
                break;
            }
            break;

        case WM_DESTROY:
            DeleteObject(darkBackgroundBrush);
            DeleteObject(panelBackgroundBrush);
            DeleteObject(buttonBrush);
            DeleteObject(outputFont);
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
    srand(static_cast<unsigned int>(time(nullptr)));

    const wchar_t windowClassName[] = L"DragonSlayerGuiWindow";
    const wchar_t mapClassName[] = L"DragonSlayerMapView";
    const wchar_t outputClassName[] = L"DragonSlayerOutputView";

    WNDCLASSW mapClass{};
    mapClass.lpfnWndProc = mapProcedure;
    mapClass.hInstance = instance;
    mapClass.lpszClassName = mapClassName;
    mapClass.hCursor = LoadCursor(nullptr, IDC_HAND);
    mapClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    RegisterClassW(&mapClass);

    WNDCLASSW outputClass{};
    outputClass.lpfnWndProc = outputProcedure;
    outputClass.hInstance = instance;
    outputClass.lpszClassName = outputClassName;
    outputClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    outputClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    RegisterClassW(&outputClass);

    WNDCLASSW windowClass{};
    windowClass.lpfnWndProc = windowProcedure;
    windowClass.hInstance = instance;
    windowClass.lpszClassName = windowClassName;
    windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    windowClass.hbrBackground = CreateSolidBrush(DarkBackground);
    RegisterClassW(&windowClass);

    HWND window = CreateWindowExW(
        0,
        windowClassName,
        L"Dragon Slayer",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        900,
        560,
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
