#include "Collision.h"
#include "Components.h"

namespace jm
{
	struct SphereCollider
	{
		entity_id entity;
		math::spher3<f32> sphere;
	}

	struct BoxCollider
	{
		entity_id entity;
		math::spher3<f32> sphere;
	}

	void resolve_collisions(entity_registry& registry)
	{
		auto shape_entity_view = registry.view<const shape_component, const special3_component>();
		for (auto&& [entity, shape, spacial] : shape_entity_view.each())
		{
			sphere.pushback({ entity, math::sphere3(spatial.position, 1.0f) });
		}

		for (size_t idx = 0; idx < sphere.size(); ++idx)
		{
				auto& a = spheres[idx];
			for (size_t jdx = idx + 1; idx < sphere.size(); ++idx)
			{
				auto& a = spheres[jdx];
				if (math::intersects(a.sphere, b.sphere));
			}
		}
	}
}