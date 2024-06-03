#include "pch.h"
#include "ConfigManager.hpp"

ConfigManager& ConfigManager::getInstance()
{
    static ConfigManager instance;
    return instance;
}

void ConfigManager::loadOrCreateConfig()
{
    std::ifstream file(CONFIG_PATH + std::string("gameConfig.json"));
    nlohmann::json config;

    if (file.is_open())
    {
        file >> config;
        file.close();
    }
    else
    {
        config["resolution"]["width"] = 1920;
        config["resolution"]["height"] = 1080;
        config["frameRateLimit"] = 60;
        config["windowMode"] = "Windowed";
        config["tutorialEnabled"] = true;

        std::ofstream file(CONFIG_PATH + std::string("gameConfig.json"));
        file << config;
        file.close();
    }

    this->loadConfig();
}

void ConfigManager::loadConfig()
{
    std::ifstream file(CONFIG_PATH + std::string("gameConfig.json"));
    nlohmann::json config;

    if (file.is_open())
    {
        file >> config;
        file.close();
    }

    this->SCREEN_WIDTH = config["resolution"]["width"];
    this->SCREEN_HEIGHT = config["resolution"]["height"];
    this->FRAME_RATE_LIMIT = config["frameRateLimit"];
    this->WINDOW_STYLE = config["windowMode"].get<std::string>();
}

float ConfigManager::getScale()
{
    constexpr unsigned int k4KWidth = 3840;
    constexpr unsigned int k4KHeight = 2160;
    constexpr float k4KScale = 1.0f;

    float scaleX = static_cast<float>(SCREEN_WIDTH) / k4KWidth;
    float scaleY = static_cast<float>(SCREEN_HEIGHT) / k4KHeight;

    return std::min(scaleX, scaleY) / k4KScale;
}

bool ConfigManager::isTutorialEnabled()
{
    std::ifstream file(CONFIG_PATH + std::string("gameConfig.json"));
    nlohmann::json config;

    if (file.is_open())
    {
        file >> config;
        file.close();
    }

    return config["tutorialEnabled"].get<bool>();
}