#include "Collision.h"
#include "Entity.h"
#include "Components.h"
#include "Math/Geometry.h"

namespace jm
{
    collider_set build_coliders(Entity_registry& registry)
    {
        std::vector<sphere_collider> spheres;
        std::vector<box_collider> boxes;
        {
            auto sphere_entity_view = registry.view<const sphere_shape_component, const spatial3_component>();
            spheres.reserve(sphere_entity_view.size_hint());
            for (auto&& [entity, shape, spatial] : sphere_entity_view.each())
            {
                spheres.push_back({ entity, math::sphere3<f32>{spatial.position, shape.radius} });
            }
        }
        {
            auto box_entity_view = registry.view<const box_shape_component, const spatial3_component>();
            boxes.reserve(box_entity_view.size_hint());
            for (auto&& [entity, shape, spatial] : box_entity_view.each())
            {
                boxes.push_back({ entity, math::box3<f32>{spatial.position, shape.extents, math::quat_to_mat(spatial.orientation)} });
            }
        }
        return { spheres, boxes };
    }

    entity_pick ray_cast(collider_set const& colliders, math::ray3<f32> const& ray)
    {
        f32 t_min = std::numeric_limits<f32>::infinity();
        Entity_id entity_closest = Null_entity_id;
        math::vector3_f32 offset{};
        for (sphere_collider const& collider: colliders.spheres)
        {
            f32 t_intersect = std::numeric_limits<f32>::infinity();
            if (math::intersect(collider.sphere, ray, t_intersect) && t_intersect < t_min)
            {
                entity_closest = collider.entity;
                t_min = t_intersect;
                offset = (ray.origin + t_intersect * ray.direction) - collider.sphere.center;
            }
        }

        /*for (SphereCollider const& collider : colliders.spheres)
        {

        }*/

        if (entity_closest != Null_entity_id)
        {
            return EntityOffset{entity_closest, offset};
        }

        return std::nullopt;
    }

    void resolve_collisions(Entity_registry& registry, collider_set const& colliders)
    {
        registry;

        for (size_t idx = 0; idx < colliders.spheres.size(); ++idx)
        {
            auto& b = colliders.spheres[idx];
            for (size_t jdx = idx + 1; jdx < colliders.spheres.size(); ++jdx)
            {
                auto& a = colliders.spheres[jdx];
                if (math::intersect(a.sphere, b.sphere))
                {
                    
                }
            }

            for (size_t jdx = idx + 1; jdx < colliders.boxes.size(); ++jdx)
            {
                auto& a = colliders.boxes[jdx];
                if (math::intersect(b.sphere, a.box))
                {
                    
                }
            }

        }
    }
}