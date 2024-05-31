#include "Vector3D.h"


String Vector3D::toString()
{
    return String(x) + F(",") + String(y) + F(",") + String(z);
}

Vector3D Vector3D::operator*(float const &k)
{
    return Vector3D(x*k, y*k, z*k);
}

Vector3D Vector3D::operator/(float const &k)
{
    return Vector3D(x/k, y/k, z/k);
}

Vector3D Vector3D::operator-(Vector3D const &other)
{
    return Vector3D(x - other.x, y - other.y, z - other.z);
}

Vector3D Vector3D::operator+(Vector3D const &other)
{
    return Vector3D(x + other.x, y + other.y, z + other.z);
}