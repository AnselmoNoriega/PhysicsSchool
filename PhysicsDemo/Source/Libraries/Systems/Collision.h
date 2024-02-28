#pragma once
#include "Entity.h"
#include "Geometry.h"
#include <optional>

namespace jm
{
	struct sphere_collider
	{
		Entity_id entity;
		math::sphere3<f32> sphere;
	};

	struct box_collider
	{
		Entity_id entity;
		math::box3<f32> box;
	};

	struct collider_set
	{
		std::vector<sphere_collider> spheres{};
		std::vector<box_collider> boxes{};
	};

    struct EntityOffset
    {
        Entity_id entity_id = Null_entity_id;
        math::vector3_f32 offset{};
    };

    using entity_pick = std::optional<EntityOffset>;

	collider_set build_coliders(Entity_registry& registry);
    entity_pick ray_cast(collider_set const& colliders, math::ray3<f32> const& ray);
    void resolve_collisions(Entity_registry& registry, collider_set const& colliders);
}
