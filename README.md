# WeatherCLi - Weather Application (Qt 6 Quick + C++)

A cross-platform desktop weather application built with **Qt 6 QML UI** and **C++ backend**.

[中文版](README_ZH.md)

## Project Introduction

**WeatherCLi** is a simple yet fully-featured weather query application that allows users to easily check real-time weather information for any city in the world.


### Technology Stack

- **Frontend**: Qt 6 QML + QtQuick Controls
- **Backend**: C++ + QNetworkAccessManager
- **Data Processing**: QJsonDocument / QJsonObject
- **Build System**: CMake 3.21+
- **Compiler**: C++20



## Quick Start

### Prerequisites

- **Qt 6** (Core, Gui, Qml, Quick, Network modules)
- **CMake** 3.21 or higher
- **C++20** compiler support

### Command Line Build

```bash
# Enter project directory
cd your-directory/WeatherCLi

# Configure CMake
cmake -S . -B cmake-build-debug

# Build project
cmake --build cmake-build-debug -j 4

# Run application

# For macOS users:
./cmake-build-debug/WeatherCLi.app/Contents/MacOS/WeatherCLi

# For Windows/Linux users:
./cmake-build-debug/WeatherCLi
```

## Usage Instructions

### Basic Usage

1. **Launch Application** - Run the executable and the application window will open
2. **Enter City Name** - Type the name of the city you want to query in the input box
   - Supports English city names: Beijing, London, New York, Tokyo, Paris, etc.
3. **Click "Get Weather" Button** - Fetch the weather information
4. **View Results** - The information panel below will display:
   - 🌡️ **Temperature** - Temperature in Celsius
   - 📝 **Description** - Weather description
   - 💧 **Humidity** - Humidity percentage
   - 💨 **Wind Speed** - Wind speed in meters per second

### UI Interface Guide

```
┌─────────────────────────────────┐
│     Current Weather             │  ← Title
├─────────────────────────────────┤
│ [Enter city name          ]     │  ← Input field
├─────────────────────────────────┤
│ [      Get Weather        ]     │  ← Query button
├─────────────────────────────────┤
│ ⟳ Loading...                    │  ← Loading indicator
├─────────────────────────────────┤
│ Temperature: 15.5 C             │  ← Weather information
│ Description: Clear sky          │     display area
│ Humidity: 65 %                  │
│ Wind Speed: 5.2 m/s             │
│                                 │
│ Error message (if any)          │  ← Error message
└─────────────────────────────────┘
```

## Configure API Key

> **Note**: This application uses OpenWeatherMap API by default, which can be obtained free. Please follow these steps to configure your own API key:

### Configuration Steps

1. **Copy Configuration Template**
   ```bash
   cp config.example.json config.json
   ```

2. **Edit `config.json`** and fill in your API Key
   ```json
   {
       "openweathermap_api_key": "your_api_key_here"
   }
   ```

3. **Rebuild**
   ```bash
   cmake --build cmake-build-debug -j 4
   ```

### Security Notes

- **`config.json` is ignored by `.gitignore`** - Will not be uploaded to Git
- **API Key Storage** - Stored only locally to protect your keys

## Troubleshooting

### Problem 1: "City not found"
- **Cause**: The city name does not exist or is misspelled
- **Solution**: Check the city name spelling and use English city names

### Problem 2: "Network error"
- **Cause**: Network connection issue or API service is unavailable
- **Solution**: Check your network connection and try again later

### Problem 3: "API key not loaded"
- **Cause**: `config.json` file is missing or has incorrect format
- **Solution**: Recreate `config.json` or use Open-Meteo API (no Key required)

### Problem 4: Application fails to start
- **Cause**: Qt libraries are missing or version is incompatible
- **Solution**: Ensure all required Qt 6 modules are installed

## Project Structure

```
WeatherCLi/
├── CMakeLists.txt           # CMake build configuration
├── main.cpp                 # Application entry point
├── main.qml                 # QML user interface
├── weatherbackend.h         # C++ backend header file
├── weatherbackend.cpp       # C++ backend implementation
├── config.json              # API Key configuration (in .gitignore)
├── config.example.json      # Configuration template
├── .gitignore               # Git ignore file list
├── README.md                # English documentation (this file)
├── README_ZH.md             # Chinese documentation
└── cmake-build-debug/       # Build output directory
```


## License

This project is released under an open source license. You are welcome to use and modify it freely.

## Contribution

Pull requests and issue reports are welcome!

---

**Last Updated**: March 20, 2026

