#pragma once

#include "../pch.h"

struct ShieldSchema
{
    float durability;
    float energyCost;
    float duration;

    std::string shieldTextureName;
    std::string shieldIconTextureName;
};

static ShieldSchema basicShield =
{
    1.f,
    50.f,
    3.f,
    "shield",
    "basic_shield_ico"
};