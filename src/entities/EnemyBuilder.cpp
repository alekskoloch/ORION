#include "EnemyBuilder.h"

#include "../systems/WeaponsSystem.h"
#include "../systems/ProceduralGenerationSystem.h"

#include "../utils/GraphicsOperations.h"

EnemyBuilder::EnemyBuilder(entt::registry& registry) : registry(registry)
{
    
}

EnemyBuilder& EnemyBuilder::createEnemy(std::string name)
{
    enemy = registry.create();

    registry.emplace<Enemy>(enemy);
    registry.emplace<Name>(enemy);
    
    sf::Sprite sprite = CreateSprite(name);
    registry.emplace<Collision>(enemy, sprite.getGlobalBounds());
    registry.emplace<Renderable>(enemy, sprite);

    return *this;
}

EnemyBuilder& EnemyBuilder::addEntityState(float attackRange, float idleRange)
{
    registry.emplace<EntityState>(enemy);
    auto& enemyState = registry.get<EntityState>(enemy);
    enemyState.attackRange = attackRange;
    enemyState.idleRange = idleRange;

    return *this;
}

EnemyBuilder& EnemyBuilder::addPosition(sf::Vector2f position)
{
    registry.emplace<Position>(enemy, position);

    return *this;
}

EnemyBuilder& EnemyBuilder::addSpeed(float speed)
{
    registry.emplace<Speed>(enemy, speed);
    registry.emplace<Velocity>(enemy, sf::Vector2f(0.f, 0.f));

    return *this;
}

EnemyBuilder& EnemyBuilder::addHealth(float health)
{
    registry.emplace<Health>(enemy, health, health);

    return *this;
}

EnemyBuilder& EnemyBuilder::addExperience(float experience)
{
    //TODO: Level is temporary
    registry.emplace<Experience>(enemy, 1, experience);

    return *this;
}

EnemyBuilder& EnemyBuilder::addDrop(int dropChance, int dropRate)
{
    registry.emplace<Drop>(enemy, dropChance, dropRate);

    return *this;
}

EnemyBuilder& EnemyBuilder::setWeapon(WeaponSchema weaponSchema)
{
    WeaponsSystem::loadWeapon(this->registry, weaponSchema, this->enemy);

    return *this;
}

EnemyBuilder& EnemyBuilder::setWaypointMovement()
{
    // TODO: Move this to a separate function
    int numOfWaypoints = ProceduralGenerationSystem::GetRandomNumber(2, 6);
    std::vector<sf::Vector2f> waypoints;

    auto enemyPos = registry.get<Position>(enemy);

    for (int i = 0; i < numOfWaypoints; i++)
    {
        float x = ProceduralGenerationSystem::GetRandomNumber(enemyPos.position.x - 500.f, enemyPos.position.x + 500.f);
        float y = ProceduralGenerationSystem::GetRandomNumber(enemyPos.position.y - 500.f, enemyPos.position.y + 500.f);

        waypoints.push_back(sf::Vector2f(x, y));
    }

    registry.emplace<WaypointMovement>(enemy, waypoints);

    return *this;
}