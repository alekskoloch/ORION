#include "PointSystem.h"

#include "../components/components.h"
#include "../components/tagComponents.h"

#include "../managers/TextureManager.h"

#include "../utils/MathOperations.h"

void PointSystem::addPointOfInterest(entt::registry& registry, sf::Vector2f pointPosition)
{
    auto pointOfInterest = registry.create();
    registry.emplace<PointOfInterest>(pointOfInterest);
    registry.emplace<Position>(pointOfInterest, pointPosition);
    sf::Sprite sprite;
    sprite.setPosition(pointPosition);
    sprite.setTexture(TextureManager::getInstance().getTexture("pointOfInterest"));
    registry.emplace<Renderable>(pointOfInterest, sprite);
}