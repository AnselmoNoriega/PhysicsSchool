#pragma once
#include "Entity.h"
#include "Geometry.h"
#include <optional>

namespace jm
{
    struct SphereCollider
    {
        Entity_id entity;
        math::sphere3<f32> sphere;
    };

    struct BoxCollider
    {
        Entity_id entity;
        math::box3<f32> box;
    };

    struct ColliderSet
    {
        std::vector<SphereCollider> spheres;
        std::vector<BoxCollider> boxes;
    };

    struct EntityOffset
    {
        Entity_id entity_id = Null_entity_id;
        math::vector3_f32 offset{};
    };

    using entity_pick = std::optional<EntityOffset>;

    ColliderSet build_coliders(Entity_registry& registry);
    entity_pick ray_cast(ColliderSet const& colliders, math::ray3<f32> const& ray);
    void resolve_collisions(Entity_registry& registry, ColliderSet const& colliders);
}
