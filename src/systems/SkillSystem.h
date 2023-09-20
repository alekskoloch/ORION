#pragma once

#include "../pch.h"

class SkillSystem
{
public:
    static void addDamageMultiplier(entt::registry& registry, float multiplier);
    static void addWeaponDamageMultiplier(entt::registry& registry, float multiplier);
    static void addSingleShotWeaponDamageMultiplier(entt::registry& registry, float multiplier);
    static float getWeaponDamage(entt::registry& registry, entt::entity entity);

    static void addWeaponEnergyCostMultiplier(entt::registry& registry, float multiplier);
    static void addSingleShotWeaponEnergyCostMultiplier(entt::registry& registry, float multiplier);
    static float getWeaponEnergyCost(entt::registry& registry, entt::entity entity);

    static void addEnergyRegenerationMultiplier(entt::registry& registry, float multiplier);
    static float getEnergyRegeneration(entt::registry& registry, entt::entity entity);

    static void addShieldTimeDurationMultiplier(entt::registry& registry, float multiplier);
    static float getShieldTimeDurationMultiplier(entt::registry& registry);
};