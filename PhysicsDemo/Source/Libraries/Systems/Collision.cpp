#include "Collision.h"
#include "Entity.h"
#include "Components.h"
#include "Math/Geometry.h"

namespace jm
{
    ColliderSet build_coliders(Entity_registry& registry)
    {
        auto shape_entity_view = registry.view<const shape_component, const spatial3_component>();

        std::vector<SphereCollider> spheres;
        std::vector<BoxCollider> boxes;

        for (auto&& [entity, shape, spatial] : shape_entity_view.each())
        {
            switch (shape)
            {
            case shape_component::Sphere:
                spheres.push_back({ entity, math::sphere3{spatial.position, 1.0f} });
                break;
            default:
                boxes.push_back({ entity, math::box3{
                    spatial.position,
                    math::vector3_f32{1.0f},
                    math::quat_to_mat(spatial.orientation)} });
                break;
            }
        }

        return{ spheres, boxes };
    }

    entity_pick ray_cast(ColliderSet const& colliders, math::ray3<f32> const& ray)
    {
        f32 t_min = std::numeric_limits<f32>::infinity();
        Entity_id entity_closest = Null_entity_id;
        math::vector3_f32 offset{};
        for (SphereCollider const& collider: colliders.spheres)
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

    void resolve_collisions(Entity_registry& registry, ColliderSet const& colliders)
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