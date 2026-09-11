# Simple Window Toggle Theme

[English](README.md) | 简体中文

一个轻量级的 Windows 11 系统托盘小工具，用于快速切换浅色 / 深色主题设置。

原项目会同时切换 Windows 系统主题和应用主题。这个 Fork 在此基础上增加了可选的**切换对象**，可以决定究竟修改哪一项主题设置。

## 功能

- 双击托盘图标即可切换当前选中的主题对象
- 可在托盘右键菜单的 **Toggle target** 中选择：
  - **Apps only** — 只切换 `AppsUseLightTheme`
  - **Windows only** — 只切换 `SystemUsesLightTheme`
  - **Apps + Windows** — 同时切换两者，并保持二者同步
- 默认仍然是 **Apps only**，保留 v1.0.0 的使用习惯
- 会记住上一次选择的切换对象，重启程序后仍然保留
- 托盘右键菜单可设置开机启动
- 原生 Windows 小程序，无需安装

因此你既可以让 Windows 本身长期保持深色，只按需切换应用主题，也可以随时切回类似原版的“系统 + 应用一起切换”模式。

## 使用方法

1. 从最新 Release 下载带版本号的程序，例如 `ToggleTheme-v1.1.0.exe`。
2. 运行该程序。
3. 右键托盘图标，打开 **Toggle target**，选择：
   - Apps only
   - Windows only
   - Apps + Windows
4. 双击托盘图标，或点击 **Toggle Theme**，即可切换当前选中的对象。
5. 右键托盘图标还可以设置开机启动或退出程序。

## 下载

可从 [Releases 页面](https://github.com/trycancry/simple-window-toggle-theme/releases) 下载正式发布版本。

当前版本：[v1.1.0](https://github.com/trycancry/simple-window-toggle-theme/releases/tag/v1.1.0)

v1.1.0 已加入可选“切换对象”功能，并会记住上一次选择的模式。

## 编译

本仓库已经配置 GitHub Actions，因此本地不需要安装 Visual Studio。

普通向 `main` 分支推送代码时，**不会自动触发编译**。

手动在 **Actions** 页面点击 **Run workflow** 时，会生成 `ToggleTheme-dev.exe`，对应的 Artifact 名称为 `ToggleTheme-dev-windows-x64`。

当创建 `v*` 格式的版本标签（例如 `v1.1.0`）时，GitHub Actions 会自动编译，把版本号写入 Windows 文件属性，并创建 GitHub Release。此时程序文件名为 `ToggleTheme-v1.1.0.exe`，Artifact 名称为 `ToggleTheme-v1.1.0-windows-x64`。

例如：

```bash
git tag v1.1.0
git push origin v1.1.0
```

## 演示

> 下方 GIF 来自原项目，展示的可能是原版“Windows 系统主题和应用主题一起切换”的行为。

![Demo](sample.gif)

## 与原版的区别

原版会同时修改：

- `SystemUsesLightTheme`
- `AppsUseLightTheme`

当前 Fork 支持三种模式：

- 只修改 `AppsUseLightTheme`
- 只修改 `SystemUsesLightTheme`
- 同时修改两者

因此可以根据自己的使用习惯决定切换范围。

## 致谢

本项目 Fork 自 [yusteafy/simple-window-toggle-theme](https://github.com/yusteafy/simple-window-toggle-theme)。
