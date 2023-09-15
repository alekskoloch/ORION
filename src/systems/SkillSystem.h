#pragma once

#include "../pch.h"

class SkillSystem
{
public:
    static void updateWeaponDamageMultiplier(entt::registry& registry, float multiplier);
    static float getWeaponDamage(entt::registry& registry, entt::entity entity); 
};