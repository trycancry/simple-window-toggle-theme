# Simple Window Toggle Theme

English | [简体中文](README.zh-CN.md)

A lightweight Windows 11 system tray utility for quickly toggling the **default app theme** between light and dark mode.

This fork changes the original behavior so that it **only changes the app theme** and leaves the Windows system theme untouched.

## Features

- Double-click the tray icon to toggle the default app theme:
  - Light → Dark
  - Dark → Light
- Does **not** change the Windows system theme
- Optional run-at-startup setting from the tray menu
- Small native Windows executable with no installer required

For example, you can keep:

- Windows mode: **Dark**
- App mode: **Light / Dark**, toggled from the tray icon

## Usage

1. Download `ToggleTheme.exe` from the latest release.
2. Run `ToggleTheme.exe`.
3. Double-click its system tray icon to switch the app theme.
4. Right-click the tray icon for startup and quit options.

## Download

Download the latest build from the [Releases page](https://github.com/trycancry/simple-window-toggle-theme/releases).

Current release: [v1.0.0](https://github.com/trycancry/simple-window-toggle-theme/releases/tag/v1.0.0)

## Build

The repository includes a GitHub Actions workflow, so Visual Studio is not required locally.

Builds are **not** triggered by normal pushes to `main`.

You can build a Windows x64 Release executable manually from the **Actions** page using **Run workflow**. Creating a version tag matching `v*` (for example `v1.1.0`) will automatically build the executable and publish a GitHub Release with `ToggleTheme.exe` attached.

## Demo

> The GIF below comes from the original project and may show the original system-wide theme switching behavior.

![Demo](sample.gif)

## Credits

Forked from [yusteafy/simple-window-toggle-theme](https://github.com/yusteafy/simple-window-toggle-theme).
