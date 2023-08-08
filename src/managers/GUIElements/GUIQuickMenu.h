#pragma once

#include "../../pch.h"

class GUIQuickMenu
{
public:
    GUIQuickMenu(sf::RenderWindow& window, entt::registry& registry);

    void update();
    void draw();

    uint8_t getSelectedTile() const { return this->selectedTile; }

private:
    sf::RenderWindow& window;
    entt::registry& registry;

    std::vector<sf::Sprite> quickMenuTiles;
    std::vector<sf::Sprite> quickMenuIcons;

    uint8_t selectedTile = 0;

    void initializeQuickMenu();
};