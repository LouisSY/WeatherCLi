# WeatherCLi - 天气应用 (Qt 6 Quick + C++)

一个跨平台的桌面天气应用，使用 **Qt 6 QML UI** 和 **C++ 后端** 构建。

[English version](README.md)

## 项目介绍

**WeatherCLi** 是一个简洁而功能完整的天气查询应用，让用户可以轻松查询任何城市的实时天气信息。


### 技术栈

- **前端**: Qt 6 QML + QtQuick Controls
- **后端**: C++ + QNetworkAccessManager
- **数据处理**: QJsonDocument / QJsonObject
- **构建系统**: CMake 3.21+
- **编译器**: C++20


## 快速开始

### 前置要求

- **Qt 6** (Core, Gui, Qml, Quick, Network 模块)
- **CMake** 3.21 或更高版本
- **C++20** 编译器支持

### 命令行构建

```bash
# 进入项目目录
cd your-directory/WeatherCLi

# 配置 CMake
cmake -S . -B cmake-build-debug

# 编译项目
cmake --build cmake-build-debug -j 4

# 运行应用

# macOS 用户:
./cmake-build-debug/WeatherCLi.app/Contents/MacOS/WeatherCLi

# Windows/Linux 用户:
./cmake-build-debug/WeatherCLi
```

## 使用说明

### 基础使用

1. **启动应用** - 运行可执行文件，应用窗口会打开
2. **输入城市名** - 在输入框中输入你想查询的城市名
   - 支持英文城市名: Beijing, London, New York, Tokyo, Paris 等
3. **点击"Get Weather"按钮** - 获取天气信息
4. **查看结果** - 在下方面板显示:
   - 🌡️ **Temperature** - 温度（摄氏度）
   - 📝 **Description** - 天气描述
   - 💧 **Humidity** - 湿度百分比
   - 💨 **Wind Speed** - 风速（米/秒）

### UI 界面说明

```
┌─────────────────────────────────┐
│     Current Weather             │  ← 标题
├─────────────────────────────────┤
│ [Enter city name          ]     │  ← 输入框
├─────────────────────────────────┤
│ [      Get Weather        ]     │  ← 查询按钮
├─────────────────────────────────┤
│ ⟳ Loading...                    │  ← 加载指示器
├─────────────────────────────────┤
│ Temperature: 15.5 C             │  ← 天气信息
│ Description: Clear sky          │     显示区域
│ Humidity: 65 %                  │
│ Wind Speed: 5.2 m/s             │
│                                 │
│ Error message (if any)          │  ← 错误提示
└─────────────────────────────────┘
```

## 配置 API Key (可选)

> **注意**: 本应用需要使用 OpenWeatherMap API，可以在官网免费获得。请按以下步骤配置：

### 配置步骤

1. **复制配置模板**
   ```bash
   cp config.example.json config.json
   ```

2. **编辑 `config.json`** 并填入你的 API Key
   ```json
   {
       "openweathermap_api_key": "your_api_key_here"
   }
   ```

3. **重新编译**
   ```bash
   cmake --build cmake-build-debug -j 4
   ```

### 安全说明

- **`config.json` 已被 `.gitignore` 忽略** - 不会上传到 Git
- **API Key 存储** - 只在本地保存

## 常见问题排查

### 问题 1: "City not found"
- **原因**: 输入的城市名不存在或拼写错误
- **解决**: 检查城市名拼写，使用英文城市名

### 问题 2: "Network error"
- **原因**: 网络连接问题或 API 服务不可用
- **解决**: 检查网络连接，稍后重试

### 问题 3: "API key not loaded"
- **原因**: `config.json` 文件丢失或格式错误
- **解决**: 重新创建 `config.json` 或使用 Open-Meteo API（无需 Key）

### 问题 4: 应用无法启动
- **原因**: Qt 库缺失或版本不兼容
- **解决**: 确保已安装 Qt 6 的所有必需模块

## 项目结构

```
WeatherCLi/
├── CMakeLists.txt           # CMake 构建配置
├── main.cpp                 # 应用入口
├── main.qml                 # QML 用户界面
├── weatherbackend.h         # C++ 后端头文件
├── weatherbackend.cpp       # C++ 后端实现
├── config.json              # API Key 配置 (在 .gitignore 中)
├── config.example.json      # 配置模板
├── .gitignore               # Git 忽略文件列表
├── README.md                # 本文档
└── cmake-build-debug/       # 构建输出目录
```

## 许可证

本项目采用开源许可证发布，欢迎自由使用和修改。

## 贡献

欢迎提交 Issue 和 Pull Request！

---

**最后更新**: 2026 年 3 月 20 日




