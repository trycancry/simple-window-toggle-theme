# Simple Window Toggle Theme

[English](README.md) | 简体中文

一个轻量级的 Windows 11 系统托盘小工具，用于快速切换**默认应用模式**的浅色 / 深色主题。

这个 Fork 修改了原项目的行为：**只切换应用主题，不修改 Windows 系统主题**。

## 功能

- 双击系统托盘图标即可切换默认应用模式：
  - 浅色 → 深色
  - 深色 → 浅色
- **不会修改 Windows 系统主题**
- 托盘右键菜单可设置开机启动
- 原生 Windows 小程序，无需安装

例如，你可以一直保持：

- Windows 模式：**深色**
- 应用模式：**浅色 / 深色**，通过托盘图标切换

这正适合希望任务栏、开始菜单等 Windows 界面始终保持深色，但又希望根据需要切换应用主题的使用习惯。

## 使用方法

1. 从最新 Release 下载 `ToggleTheme.exe`。
2. 运行 `ToggleTheme.exe`。
3. 双击系统托盘中的程序图标，即可切换应用浅色 / 深色模式。
4. 右键托盘图标可以设置开机启动或退出程序。

## 下载

可从 [Releases 页面](https://github.com/trycancry/simple-window-toggle-theme/releases) 下载最新版本。

当前版本：[v1.0.0](https://github.com/trycancry/simple-window-toggle-theme/releases/tag/v1.0.0)

## 编译

本仓库已经配置 GitHub Actions，因此本地不需要安装 Visual Studio。

普通向 `main` 分支推送代码时，**不会自动触发编译**。

如果只是想临时编译，可以进入仓库的 **Actions** 页面，手动点击 **Run workflow**，GitHub 会编译 Windows x64 Release 版本并上传为 Actions Artifact。

当创建 `v*` 格式的版本标签（例如 `v1.1.0`）时，GitHub Actions 会自动编译，并创建 GitHub Release，同时附带编译好的 `ToggleTheme.exe`。

例如：

```bash
git tag v1.1.0
git push origin v1.1.0
```

## 演示

> 下方 GIF 来自原项目，展示的可能是原版“Windows 系统主题和应用主题一起切换”的行为，与当前 Fork 的实际行为有所不同。

![Demo](sample.gif)

## 与原版的区别

原版会同时修改：

- `SystemUsesLightTheme`
- `AppsUseLightTheme`

当前 Fork 只修改：

- `AppsUseLightTheme`

因此 Windows 系统模式会保持不变。

## 致谢

本项目 Fork 自 [yusteafy/simple-window-toggle-theme](https://github.com/yusteafy/simple-window-toggle-theme)。
