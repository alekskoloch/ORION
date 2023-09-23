#pragma once

#include "../pch.h"

#include "../components/components.h"
#include "../components/tagComponents.h"

#include "../systems/SkillSystem.h"

enum class RequirementType
{
    None,
    OrangeStone,
    GreenStone,
};

struct SkillSchema
{
    sf::Vector2f position;
    std::string name;
    bool multiLevel;
    std::vector<std::string> descriptions;
    std::string textureName;
    std::string hoverTextureName;
    std::string activeTextureName;
    std::vector<std::function<void(entt::registry& registry)>> callbacks;
    std::vector<RequirementType> requirements;
    std::vector<SkillSchema> skillsToUnlock = {};

    unsigned int maxLevel = 1;
    unsigned int currentLevel = 0;
};

static SkillSchema EnergyFrugality
{
    sf::Vector2f(2320, 280.f),
    "Energy Frugality",
    false,
    {
        "Single shoot weapons use 5% less energy",
        "Weapons use 5% less energy"
    },
    "skill",
    "skillHover",
    "skillActive",
    {
        [](entt::registry& registry)
        {
            SkillSystem::addSingleShotWeaponEnergyCostMultiplier(registry, -0.05f);
        },
        [](entt::registry& registry)
        {
            SkillSystem::addSingleShotWeaponEnergyCostMultiplier(registry, 0.05f);
            SkillSystem::addWeaponEnergyCostMultiplier(registry, -0.05f);
        }
    },
    {RequirementType::None, RequirementType::None},
    {},
    2
};

static SkillSchema SlaughterSurge
{
    sf::Vector2f(1920.f, 280.f),
    "Slaughter Surge",
    false,
    {"Increases the damage inflicted by 5%"},
    "skill",
    "skillHover",
    "skillActive",
    {[](entt::registry& registry)
    {
        SkillSystem::addDamageMultiplier(registry, 0.05f);
    }},
    {RequirementType::None}
};

static SkillSchema SingularStrikeMastery
{
    sf::Vector2f(1520.f, 280.f),
    "Singular Strike Mastery",
    true,
    {
        "Allows special shot for single shoot weapons",
        "A special shoot from single shoot weapons costs 50% less energy",
        "A special shoot from single shoot weapons has a small chance of activating Basic Shield"
    },
    "skill",
    "skillHover",
    "skillActive",
    {
        [](entt::registry& registry)
        {
            SkillSystem::enableSingleShotWeaponSpecialShot(registry);
        },
        [](entt::registry& registry)
        {
            
        },
        [](entt::registry& registry)
        {
            
        }
    },
    {RequirementType::None, RequirementType::OrangeStone, RequirementType::GreenStone},
    {},
    3
};

static SkillSchema LethalPrecision
{
    sf::Vector2f(1920.f, 680.f),
    "Lethal Precision",
    false,
    {
        "All single shot weapons deal damage increased by 5%",
        "All single shot weapons deal damage increased by 10%"    
    },
    "skill",
    "skillHover",
    "skillActive",
    {
        [](entt::registry& registry)
        {
            SkillSystem::addSingleShotWeaponDamageMultiplier(registry, 0.05f);
        },
        [](entt::registry& registry)
        {
            SkillSystem::addSingleShotWeaponDamageMultiplier(registry, 0.05f);
        }
    },
    {RequirementType::None, RequirementType::OrangeStone},
    {SingularStrikeMastery, SlaughterSurge, EnergyFrugality},
    2
};

static SkillSchema OrionProtocol
{
    sf::Vector2f(1920.f, 1080.f),
    "Orion protocol",
    true,
    {
        "All bullets deal damage increased by 5%",
        "All shields last 5% longer",
        "Energy regeneration works 5% better"
    },
    "skill",
    "skillHover",
    "skillActive",
    {
        [](entt::registry& registry)
        {
            SkillSystem::addWeaponDamageMultiplier(registry, 0.05f);
        },
        [](entt::registry& registry)
        {
            SkillSystem::addShieldTimeDurationMultiplier(registry, 0.05f);
        },
        [](entt::registry& registry)
        {
            SkillSystem::addEnergyRegenerationMultiplier(registry, 0.05f);
        }
    },
    {RequirementType::OrangeStone, RequirementType::GreenStone, RequirementType::OrangeStone},
    {LethalPrecision},
    3
};