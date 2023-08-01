#pragma once

#include <cmath>

#include <SFML/Graphics.hpp>

#include "../components/components.h"

inline float lerp(float a, float b, float f)
{
    return a + f * (b - a);
}

inline float DistanceToMouse(const sf::Vector2f& mousePosition, const Position& position)
{
    return std::sqrt(std::pow(mousePosition.x - position.position.x, 2) + std::pow(mousePosition.y - position.position.y, 2));
}

inline float TargetAngle(const sf::Vector2f& mousePosition, const Position& position, float offset = 90.0f)
{
    return std::atan2(mousePosition.y - position.position.y, mousePosition.x - position.position.x) * (180 / M_PI) + offset;
}

inline float AngleDifference(float targetAngle, float currentAngle)
{
    float angleDifference = targetAngle - currentAngle;
    if (angleDifference > 180.0f)
    {
        angleDifference -= 360.0f;
    }
    else if (angleDifference < -180.0f)
    {
        angleDifference += 360.0f;
    }
    return angleDifference;
}