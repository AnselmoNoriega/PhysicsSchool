#include "Collision.h"
#include "Components.h"
#include "Math/Geometry.h"

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

    void resolve_collisions(Entity_registry& registry)
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
                break;
            }
        }

        for (size_t idx = 0; idx < spheres.size(); ++idx)
        {
            auto& b = spheres[idx];
            for (size_t jdx = idx + 1; jdx < spheres.size(); ++jdx)
            {
                auto& a = spheres[jdx];
                if (math::intersect(a.sphere, b.sphere))
                {
                    
                }
            }

            for (size_t jdx = idx + 1; jdx < boxes.size(); ++jdx)
            {
                auto& a = boxes[jdx];
                if (math::intersect(b.sphere, a.box))
                {
                    
                }
            }

        }
    }
}