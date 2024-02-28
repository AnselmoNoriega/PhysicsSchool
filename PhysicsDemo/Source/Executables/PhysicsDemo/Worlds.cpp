#include "Worlds.h"

#include "Systems/Entity.h"
#include "Systems/Components.h"

#include "Random.h"

namespace jm
{
	void AddSphereEntity(Entity_registry& registry, math::vector3_f32 const& position, math::quaternion_f32 const& rotation)
	{
		Entity_id e = registry.create();
		registry.emplace<spatial3_component>(e,position, rotation);
		registry.emplace<sphere_shape_component>(e, 1.0f);
		registry.emplace<linear_body3_component>(e, math::zero3, 2.0f);
	}

	void AddBoxEntity(Entity_registry& registry, math::vector3_f32 const& position, math::quaternion_f32 const& rotation, math::vector3_f32 const& extents = math::zero3)
	{
		extents;
		Entity_id e = registry.create();
		registry.emplace<spatial3_component>(e, position, rotation);
		registry.emplace<box_shape_component>(e, math::vector3_f32{ 1.0f, 1.0f, 1.0f });
		registry.emplace<linear_body3_component>(e, math::zero3, 2.0f);
	}

	void CreateBasicWorld(Entity_registry& registry)
	{
		AddSphereEntity(registry, 8.0f * math::random::unit_ball<f32>(), math::random::unit_quaternion<f32>());
		AddSphereEntity(registry, 8.0f * math::random::unit_ball<f32>(), math::random::unit_quaternion<f32>());
		AddSphereEntity(registry, 8.0f * math::random::unit_ball<f32>(), math::random::unit_quaternion<f32>());
		AddSphereEntity(registry, 8.0f * math::random::unit_ball<f32>(), math::random::unit_quaternion<f32>());
		
		AddBoxEntity(registry, 8.0f * math::random::unit_ball<f32>(), math::random::unit_quaternion<f32>());
		AddBoxEntity(registry, 8.0f * math::random::unit_ball<f32>(), math::random::unit_quaternion<f32>());
		AddBoxEntity(registry, 8.0f * math::random::unit_ball<f32>(), math::random::unit_quaternion<f32>());
	}
}