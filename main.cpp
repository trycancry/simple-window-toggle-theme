#include <windows.h>
#include <shellapi.h>
#include <cstring>
#include "resource.h"

#define WM_TRAYICON (WM_USER + 1)

constexpr UINT ID_TOGGLE_THEME = 1;
constexpr UINT ID_RUN_AT_STARTUP = 2;
constexpr UINT ID_QUIT = 3;
constexpr UINT ID_TARGET_APPS = 10;
constexpr UINT ID_TARGET_WINDOWS = 11;
constexpr UINT ID_TARGET_BOTH = 12;

const char* THEME_PATH = "Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize";
const char* SETTINGS_PATH = "Software\\SimpleWindowToggleTheme";
const char* TOGGLE_TARGET_VALUE = "ToggleTarget";

enum class ToggleTarget : DWORD
{
    AppsOnly = 0,
    WindowsOnly = 1,
    AppsAndWindows = 2
};

NOTIFYICONDATA nid;
HMENU hMenu;
HMENU hTargetMenu;
ToggleTarget gToggleTarget = ToggleTarget::AppsOnly;

ToggleTarget LoadToggleTarget()
{
    HKEY hKey;
    DWORD value = static_cast<DWORD>(ToggleTarget::AppsOnly);
    DWORD type = 0;
    DWORD size = sizeof(value);

    if (RegOpenKeyExA(HKEY_CURRENT_USER, SETTINGS_PATH, 0, KEY_QUERY_VALUE, &hKey) == ERROR_SUCCESS)
    {
        if (RegQueryValueExA(hKey, TOGGLE_TARGET_VALUE, NULL, &type, reinterpret_cast<LPBYTE>(&value), &size) != ERROR_SUCCESS ||
            type != REG_DWORD || value > static_cast<DWORD>(ToggleTarget::AppsAndWindows))
        {
            value = static_cast<DWORD>(ToggleTarget::AppsOnly);
        }
        RegCloseKey(hKey);
    }

    return static_cast<ToggleTarget>(value);
}

void SaveToggleTarget(ToggleTarget target)
{
    HKEY hKey;
    DWORD disposition;

    if (RegCreateKeyExA(
            HKEY_CURRENT_USER,
            SETTINGS_PATH,
            0,
            NULL,
            REG_OPTION_NON_VOLATILE,
            KEY_SET_VALUE,
            NULL,
            &hKey,
            &disposition) == ERROR_SUCCESS)
    {
        DWORD value = static_cast<DWORD>(target);
        RegSetValueExA(
            hKey,
            TOGGLE_TARGET_VALUE,
            0,
            REG_DWORD,
            reinterpret_cast<const BYTE*>(&value),
            sizeof(value));
        RegCloseKey(hKey);
    }
}

void SetToggleTarget(ToggleTarget target)
{
    gToggleTarget = target;
    SaveToggleTarget(target);
}

UINT GetToggleTargetMenuId()
{
    switch (gToggleTarget)
    {
    case ToggleTarget::WindowsOnly:
        return ID_TARGET_WINDOWS;
    case ToggleTarget::AppsAndWindows:
        return ID_TARGET_BOTH;
    case ToggleTarget::AppsOnly:
    default:
        return ID_TARGET_APPS;
    }
}

void UpdateTargetMenuChecks()
{
    CheckMenuRadioItem(
        hTargetMenu,
        ID_TARGET_APPS,
        ID_TARGET_BOTH,
        GetToggleTargetMenuId(),
        MF_BYCOMMAND);
}

void ToggleTheme()
{
    HKEY hKey;

    if (RegOpenKeyExA(HKEY_CURRENT_USER, THEME_PATH, 0, KEY_QUERY_VALUE | KEY_SET_VALUE, &hKey) == ERROR_SUCCESS)
    {
        DWORD value = 1;
        DWORD size = sizeof(value);
        DWORD newValue;

        switch (gToggleTarget)
        {
        case ToggleTarget::WindowsOnly:
            RegQueryValueExA(hKey, "SystemUsesLightTheme", NULL, NULL, reinterpret_cast<LPBYTE>(&value), &size);
            newValue = (value == 0) ? 1 : 0;
            RegSetValueExA(hKey, "SystemUsesLightTheme", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&newValue), sizeof(newValue));
            break;

        case ToggleTarget::AppsAndWindows:
            // Use the current app theme as the reference, then keep both values synchronized.
            RegQueryValueExA(hKey, "AppsUseLightTheme", NULL, NULL, reinterpret_cast<LPBYTE>(&value), &size);
            newValue = (value == 0) ? 1 : 0;
            RegSetValueExA(hKey, "AppsUseLightTheme", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&newValue), sizeof(newValue));
            RegSetValueExA(hKey, "SystemUsesLightTheme", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&newValue), sizeof(newValue));
            break;

        case ToggleTarget::AppsOnly:
        default:
            RegQueryValueExA(hKey, "AppsUseLightTheme", NULL, NULL, reinterpret_cast<LPBYTE>(&value), &size);
            newValue = (value == 0) ? 1 : 0;
            RegSetValueExA(hKey, "AppsUseLightTheme", 0, REG_DWORD, reinterpret_cast<const BYTE*>(&newValue), sizeof(newValue));
            break;
        }

        RegCloseKey(hKey);

        SendMessageTimeout(
            HWND_BROADCAST,
            WM_SETTINGCHANGE,
            0,
            reinterpret_cast<LPARAM>(L"ImmersiveColorSet"),
            SMTO_ABORTIFHUNG,
            100,
            NULL
        );
    }
}

bool IsStartupEnabled()
{
    HKEY hKey;
    const char* runPath = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
    bool enabled = false;

    if (RegOpenKeyExA(HKEY_CURRENT_USER, runPath, 0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
        enabled = (RegQueryValueExA(hKey, "ToggleTheme", NULL, NULL, NULL, NULL) == ERROR_SUCCESS);
        RegCloseKey(hKey);
    }
    return enabled;
}

void ToggleStartup()
{
    HKEY hKey;
    const char* runPath = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";

    if (RegOpenKeyExA(HKEY_CURRENT_USER, runPath, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
    {
        char exePath[MAX_PATH];
        GetModuleFileNameA(NULL, exePath, MAX_PATH);

        if (RegQueryValueExA(hKey, "ToggleTheme", NULL, NULL, NULL, NULL) == ERROR_SUCCESS)
        {
            RegDeleteValueA(hKey, "ToggleTheme");
        }
        else
        {
            RegSetValueExA(
                hKey,
                "ToggleTheme",
                0,
                REG_SZ,
                reinterpret_cast<const BYTE*>(exePath),
                static_cast<DWORD>(std::strlen(exePath) + 1));
        }

        RegCloseKey(hKey);
    }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (msg == WM_TRAYICON)
    {
        if (lParam == WM_RBUTTONUP)
        {
            POINT pt;
            GetCursorPos(&pt);
            SetForegroundWindow(hwnd);

            CheckMenuItem(
                hMenu,
                ID_RUN_AT_STARTUP,
                MF_BYCOMMAND | (IsStartupEnabled() ? MF_CHECKED : MF_UNCHECKED));
            UpdateTargetMenuChecks();

            TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, NULL);
        }
        else if (lParam == WM_LBUTTONDBLCLK)
        {
            ToggleTheme();
        }
    }
    else if (msg == WM_COMMAND)
    {
        switch (LOWORD(wParam))
        {
        case ID_TOGGLE_THEME:
            ToggleTheme();
            break;
        case ID_RUN_AT_STARTUP:
            ToggleStartup();
            break;
        case ID_QUIT:
            Shell_NotifyIcon(NIM_DELETE, &nid);
            PostQuitMessage(0);
            break;
        case ID_TARGET_APPS:
            SetToggleTarget(ToggleTarget::AppsOnly);
            break;
        case ID_TARGET_WINDOWS:
            SetToggleTarget(ToggleTarget::WindowsOnly);
            break;
        case ID_TARGET_BOTH:
            SetToggleTarget(ToggleTarget::AppsAndWindows);
            break;
        }
    }
    else if (msg == WM_DESTROY)
    {
        PostQuitMessage(0);
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"TrayApp";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"TrayApp",
        0,
        0,
        0,
        0,
        0,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    gToggleTarget = LoadToggleTarget();

    hTargetMenu = CreatePopupMenu();
    AppendMenuW(hTargetMenu, MF_STRING, ID_TARGET_APPS, L"Apps only");
    AppendMenuW(hTargetMenu, MF_STRING, ID_TARGET_WINDOWS, L"Windows only");
    AppendMenuW(hTargetMenu, MF_STRING, ID_TARGET_BOTH, L"Apps + Windows");

    hMenu = CreatePopupMenu();
    AppendMenuW(hMenu, MF_STRING, ID_TOGGLE_THEME, L"Toggle Theme");
    AppendMenuW(hMenu, MF_POPUP, reinterpret_cast<UINT_PTR>(hTargetMenu), L"Toggle target");
    AppendMenuW(hMenu, MF_SEPARATOR, 0, NULL);
    AppendMenuW(hMenu, MF_STRING, ID_RUN_AT_STARTUP, L"Run at startup");
    AppendMenuW(hMenu, MF_STRING, ID_QUIT, L"Quit");

    nid = {};
    nid.cbSize = sizeof(NOTIFYICONDATA);
    nid.hWnd = hwnd;
    nid.uID = 1;
    nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    nid.uCallbackMessage = WM_TRAYICON;
    nid.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));

    Shell_NotifyIcon(NIM_ADD, &nid);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    DestroyMenu(hMenu);
    return 0;
}
