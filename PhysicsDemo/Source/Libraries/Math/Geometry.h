#pragma once

#include "MathTypes.h"

namespace jm::math
{
    template <typename T>
    struct sphere3
    {
        vector3<T> center{};
        T radius{};
    };

    template <typename T>
    struct box3
    {
        vector3<T> position{};
        vector3<T> extents{};
        math::matrix33<T> axes{};
    };

    template <typename T>
    struct ray3
    {
        vector3<T> origin{};
        vector3<T> direction{};
    };

    template <typename T>
    bool intersect(sphere3<T> const& sphere, ray3<T> const& ray, T& t)
    {
        const vector3<T> offset = ray.origin - sphere.center;
        const T b = dot(offset, ray.direction);
        const T c = dot(offset, offset) - sphere.radius * sphere.radius;

        if (c > T(0) && b > T(0))
        {
            return false;
        }

        const T D = b * b - c;
        if (D < T(0))
        {
            return false;
        }

        t = -b - std::sqrt(D);
        if (t < T(0))
        {
            t = T(0);
        }

        return true;
    };

    template <typename T>
    bool intersect(sphere3<T> const& a, sphere3<T> const& b)
    {
        const vector3<T> center_displacement = a.center - b.center;
        return dot(center_displacement, center_displacement) < a.radius + b.radius + math::epsilon<T>();
    };

    template <typename T>
    bool intersect(vector3<T> const& a, box3<T> const& b)
    {
        vector3<T> box_local_point = a - b.position;
        box_local_point = glm::transpose(b.axes) * box_local_point;

        return std::abs(box_local_point.x) < b.extents.x + math::epsilon<T>() &&
            std::abs(box_local_point.y) < b.extents.y + math::epsilon<T>() &&
            std::abs(box_local_point.z) < b.extents.z + math::epsilon<T>();
    };

    template <typename T>
    bool intersect(sphere3<T> const& a, box3<T> const& b)
    {
        vector3<T> box_local_point = a.center - b.position;
        box_local_point = glm::transpose(b.axes) * box_local_point;

        return std::abs(box_local_point.x) < b.extents.x + a.radius + math::epsilon<T>() &&
            std::abs(box_local_point.y) < b.extents.y + a.radius + math::epsilon<T>() &&
            std::abs(box_local_point.z) < b.extents.z + a.radius + math::epsilon<T>();

    };
}