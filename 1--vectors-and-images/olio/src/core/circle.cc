#include "core/circle.h"
#include <cassert>
#include <cmath>

namespace olio{

using namespace std;

//Constructor
Circle::Circle() : Center{0, 0}, Radius{ 0 }, Color{0, 0, 0}
{

}

//Constructor2
Circle::Circle(const Vec2d &center, double radius, const Vec3d &color) : 
  Center{center[0], center[1]}, Radius{radius}, Color{color[0], color[1], color[2]}
{

}

//De-Constructor
Circle::~Circle()
{

}


void Circle::SetCenter(const Vec2d &center)
{
    Center = center;    
}

void Circle::SetRadius(double radius)
{
    Radius = radius;
}

void Circle::SetColor(const Vec3d &color)
{
    Color = color;
}

Vec2d Circle::GetCenter() const
{
    return Center;
}

double Circle::GetRadius() const
{
    return Radius;
}

Vec3d Circle::GetColor() const
{
    return Color;
}

bool Circle::IsPointInside(const Vec2d &point) const
{
    Vec2d vect = point - Center;
    double Dist = vect.norm();

    if(Dist <= Radius)
    {
        return true;
    }
    else
    {
        return false;
    }
}



}