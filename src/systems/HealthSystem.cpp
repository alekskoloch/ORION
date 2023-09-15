#include "HealthSystem.h"

#include "../components/components.h"

void HealthSystem::updateHealth(entt::registry& registry)
{
    auto view = registry.view<Health>();
    for (auto entity : view)
    {
        auto& health = view.get<Health>(entity);
        if (health.currentHealthValue <= 0)
        {
            registry.destroy(entity);
        }
    }
}