#pragma once

#include "../pch.h"

#include "../schema/WeaponsSchema.h"

using ShotFunction = std::function<void(entt::registry& registry, sf::RenderWindow& window, entt::entity& entity)>;

enum class Weapons
{
    RedWeapon
};

enum class ShotType
{
    /* 0 */     None = 0,
//              BASIC TYPES OF SHOTS
    /* 1 */     SingleShot = 1,
    /* 2 */     TripleShot,
    /* 3 */     Shuriken,
    /* 4 */     DoubleShuriken,
    /* 5 */     Nail,
//              SPECIAL TYPES OF SHOTS
    /* 100 */   FullCircle = 100,
    /* 101 */   TripleSalvo
};

class WeaponsSystem
{
public:
    static void changeWeapon(entt::registry& registry, WeaponSchema weapon);
    static void loadWeapon(entt::registry& registry, const WeaponSchema& weaponSchema, entt::entity ownerEntity);
    static void loadWeaponFromConfig(entt::registry& registry, entt::entity ownerEntity, Weapons weaponID);
    static void updateWeaponCooldown(entt::registry& registry, sf::Time deltaTime);
    static float getWeaponDamage(entt::registry& registry);
    static float getWeaponShotEnergyCost(entt::registry& registry);
    static float getWeaponSpecialShotEnergyCost(entt::registry& registry);
    static ShotFunction getWeaponShotFunction(ShotType shotType);
};