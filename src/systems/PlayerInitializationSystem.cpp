#include "PlayerInitializationSystem.h"

#include "../managers/TextureManager.h"
#include "../components/components.h"
#include "../components/tagComponents.h"

// TODO: Change start position
static float playerStartPositionX = 100.f;
static float playerStartPositionY = 100.f;

void PlayerInitializationSystem::initializePlayer(entt::registry& registry)
{
    TextureManager::getInstance().loadTexture("PLAYER_TEXTURE", ASSETS_PATH + std::string("player.png"));

    auto player = registry.create();
    registry.emplace<Player>(player);

    sf::Sprite sprite(TextureManager::getInstance().getTexture("PLAYER_TEXTURE"));
    sprite.setOrigin(sprite.getGlobalBounds().width / 2.f, sprite.getGlobalBounds().height / 2.f);
    registry.emplace<Renderable>(player, sprite);
    registry.emplace<Position>(player, sf::Vector2f(playerStartPositionX, playerStartPositionY));

    registry.emplace<Input>(player);
    registry.emplace<Speed>(player, 1000.f);
    registry.emplace<Velocity>(player, sf::Vector2f(0.f, 0.f));
    registry.emplace<Acceleration>(player, 1000.f, 1000.f);
    registry.emplace<RotationTowardsMouse>(player, true, 600.f, 2.f);
    registry.emplace<Weapon>(player, 0.1f, 1000.f, "BULLET_TEXTURE", true);
}