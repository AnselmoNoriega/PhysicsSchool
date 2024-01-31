#include "Worlds.h"

#include "Systems/Entity"
#include "Systems/Components"

namespace jm
{
	void AddSphereEntity(entity_registry& registry, math::vector3_f32 const& position, math::quaternion_f32 const& rotation)
	{
		entity_id e = registry.create();
		registry.emplace<spacial3_component>(e, position, rotation);
		registry.emplace<shape_component>(e, shape_component::sphere);
	}
{
	void AddBoxEntity(entity_registry& registry, math::vector3_f32 const& position, math::quaternion_f32 const& rotation, math::vector3_f32 const& extents = math::zero3)
	{
		entity_id e = registry.create();
		registry.emplace<spacial3_component>(e, position, rotation);
		registry.emplace<shape_component>(e, shape_component::box);
	}

	void CreateBasicWorldId(entity_registry& registry)
	{
		entity_id entity0 = registry.create();

		registry.emplace<spatial3_component>(entity0, math::vector3_f32{ 0.0f, 0.0f, 1.0f });

		entity_id entity1 = registry.create();

		registry.emplace<spatial3_component>(entity1, math::vector3_f32{ 1.0f, -3.0f, 1.0f });
	}
}
