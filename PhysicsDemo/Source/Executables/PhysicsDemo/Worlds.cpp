#include "Worlds.h"

#include "Systems/Entity.h"
#include "Systems/Components.h"

#include "Random.h"

namespace jm
{
	Entity_id CreateSphereEntity(Entity_registry& registry
		, f32 radius
		, f32 mass
		, math::vector3_f32 position
		, math::quaternion_f32 orientation
		, math::vector3_f32 velocity
		, math::vector3_f32 angular_velocity)
	{
		auto entity = registry.create();
		registry.emplace<spatial3_component>(entity, position, orientation);
		registry.emplace<linear_body3_component>(entity, velocity, mass);
		registry.emplace<rotational_body3_component>(entity, angular_velocity, math::get_sphere_inertia(mass, radius));
		registry.emplace<sphere_shape_component>(entity, radius);
		registry.emplace<collidable_component>(entity);
		return entity;
	}

	Entity_id CreateSphereEntity(Entity_registry& registry
		, f32 radius
		, f32 mass
		, math::vector3_f32 position
		, math::quaternion_f32 orientation)
	{
		return CreateSphereEntity(registry, radius, mass, position, orientation, 6.f * math::random::unit_ball<f32>(), math::random::unit_ball<f32>());
	}

	Entity_id CreateBoxEntity(Entity_registry& registry
		, math::vector3_f32 extents
		, f32 mass
		, math::vector3_f32 position
		, math::quaternion_f32 orientation
		, math::vector3_f32 velocity
		, math::vector3_f32 angular_velocity)
	{
		auto entity = registry.create();
		registry.emplace<spatial3_component>(entity, position, orientation);
		registry.emplace<linear_body3_component>(entity, velocity, mass);
		registry.emplace<rotational_body3_component>(entity, angular_velocity, math::get_box_inertia(mass, extents));
		registry.emplace<box_shape_component>(entity, extents);
		registry.emplace<collidable_component>(entity);
		return entity;
	}
	Entity_id CreateSphereEntity(Entity_registry& registry, math::vector3_f32 const& position, math::quaternion_f32 const& rotation)
	{
		return CreateSphereEntity(registry, 1.0f, 2.0f, position, rotation);
	}

	Entity_id CreateBoxEntity(Entity_registry& registry, math::vector3_f32 const& position, math::quaternion_f32 const& rotation, math::vector3_f32 const& extents)
	{
		return CreateBoxEntity(registry, extents, 2.0f, position, rotation, 6.f * math::random::unit_ball<f32>(), math::random::unit_ball<f32>());
	}

	Entity_id CreateBoxEntity(Entity_registry& registry, math::vector3_f32 const& position, math::quaternion_f32 const& rotation)
	{
		return CreateBoxEntity(registry, position, rotation, math::vector3_f32{ 1.0f });
	}

	void CreateBasicWorld(Entity_registry& registry)
	{
		CreateSphereEntity(registry, 8.0f * math::random::unit_ball<f32>(), math::random::unit_quaternion<f32>());
		CreateSphereEntity(registry, 8.0f * math::random::unit_ball<f32>(), math::random::unit_quaternion<f32>());
		CreateSphereEntity(registry, 8.0f * math::random::unit_ball<f32>(), math::random::unit_quaternion<f32>());
		CreateSphereEntity(registry, 8.0f * math::random::unit_ball<f32>(), math::random::unit_quaternion<f32>());
		
		CreateBoxEntity(registry, 8.0f * math::random::unit_ball<f32>(), math::random::unit_quaternion<f32>());
		CreateBoxEntity(registry, 8.0f * math::random::unit_ball<f32>(), math::random::unit_quaternion<f32>());
		CreateBoxEntity(registry, 8.0f * math::random::unit_ball<f32>(), math::random::unit_quaternion<f32>());
	}
}