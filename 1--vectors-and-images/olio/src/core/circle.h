#pragma once

#include <iostream>
#include "core/types.h"

namespace olio {

class Circle{
public:
    Circle();
    Circle(const Vec2d &center, double radius, const Vec3d &color);
    ~Circle();
    void SetCenter(const Vec2d &center);
    void SetRadius(double radius);
    void SetColor(const Vec3d &color);
    Vec2d GetCenter() const;
    double GetRadius() const;
    Vec3d GetColor() const;
    bool IsPointInside(const Vec2d &point) const;

protected:
    Vec2d Center;
    double Radius;
    Vec3d Color;
};

}