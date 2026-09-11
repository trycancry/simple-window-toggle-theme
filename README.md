# Simple Window Toggle Theme

English | [简体中文](README.zh-CN.md)

A lightweight Windows 11 system tray utility for quickly switching light and dark theme settings.

The original project toggles the Windows system theme and app theme together. This fork adds a selectable **toggle target**, so you can choose exactly which theme setting should change.

## Features

- Double-click the tray icon to toggle the currently selected target
- Choose the toggle target from the tray menu:
  - **Apps only** — toggles `AppsUseLightTheme`
  - **Windows only** — toggles `SystemUsesLightTheme`
  - **Apps + Windows** — toggles both and keeps them synchronized
- **Apps only** remains the default mode
- The selected toggle target is remembered between launches
- Optional run-at-startup setting from the tray menu
- Small native Windows executable with no installer required

This means you can keep Windows itself permanently dark while toggling app colors when needed, or switch back to the original system-wide behavior at any time.

## Usage

1. Download the versioned executable, such as `ToggleTheme-v1.1.0.exe`, from the latest release.
2. Run the executable.
3. Right-click the tray icon and open **Toggle target** to select:
   - Apps only
   - Windows only
   - Apps + Windows
4. Double-click the tray icon, or click **Toggle Theme**, to switch the selected target.
5. Right-click the tray icon for startup and quit options.

## Download

Download released builds from the [Releases page](https://github.com/trycancry/simple-window-toggle-theme/releases).

Current release: [v1.0.0](https://github.com/trycancry/simple-window-toggle-theme/releases/tag/v1.0.0)

> v1.0.0 supports the **Apps only** behavior. The selectable toggle target is currently on `main` and is intended for the next release.

## Build

The repository includes a GitHub Actions workflow, so Visual Studio is not required locally.

Builds are **not** triggered by normal pushes to `main`.

A manual **Run workflow** build produces `ToggleTheme-dev.exe` inside an artifact named `ToggleTheme-dev-windows-x64`.

Creating a version tag matching `v*` (for example `v1.1.0`) automatically builds the executable, embeds the tag version in the Windows file metadata, and publishes a GitHub Release containing `ToggleTheme-v1.1.0.exe` inside an artifact named `ToggleTheme-v1.1.0-windows-x64`.

## Demo

> The GIF below comes from the original project and may show the original system-wide theme switching behavior.

![Demo](sample.gif)

## Credits

Forked from [yusteafy/simple-window-toggle-theme](https://github.com/yusteafy/simple-window-toggle-theme).
