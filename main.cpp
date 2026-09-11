#include <windows.h>
#include <shellapi.h>
#include "resource.h"

#define WM_TRAYICON (WM_USER + 1)

NOTIFYICONDATA nid;
HMENU hMenu;

void ToggleTheme()
{
    HKEY hKey;
    const char* path = "Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize";

    if (RegOpenKeyExA(HKEY_CURRENT_USER, path, 0, KEY_ALL_ACCESS, &hKey) == ERROR_SUCCESS)
    {
        DWORD value = 1, size = sizeof(DWORD);
        RegQueryValueExA(hKey, "AppsUseLightTheme", NULL, NULL, (LPBYTE)&value, &size);

        DWORD newValue = (value == 0) ? 1 : 0;

        RegSetValueExA(hKey, "AppsUseLightTheme", 0, REG_DWORD, (BYTE*)&newValue, sizeof(DWORD));

        RegCloseKey(hKey);

        SendMessageTimeout(
            HWND_BROADCAST,
            WM_SETTINGCHANGE,
            0,
            (LPARAM)L"ImmersiveColorSet",
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
            RegSetValueExA(hKey, "ToggleTheme", 0, REG_SZ,
                (BYTE*)exePath, (DWORD)(strlen(exePath) + 1));
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
            CheckMenuItem(hMenu, 2, MF_BYCOMMAND | (IsStartupEnabled() ? MF_CHECKED : MF_UNCHECKED));
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
        case 1:
            ToggleTheme();
            break;
        case 2:
            ToggleStartup();
            break;
        case 3:
            Shell_NotifyIcon(NIM_DELETE, &nid);
            PostQuitMessage(0);
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
        0, CLASS_NAME, L"TrayApp",
        0, 0, 0, 0, 0,
        NULL, NULL, hInstance, NULL
    );

    hMenu = CreatePopupMenu();
    AppendMenu(hMenu, MF_STRING, 1, L"Toggle Theme");
    AppendMenu(hMenu, MF_STRING, 2, L"Run at startup");
    AppendMenu(hMenu, MF_STRING, 3, L"Quit");

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

    return 0;
}