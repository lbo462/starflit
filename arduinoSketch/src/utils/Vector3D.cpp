#include "Vector3D.h"


Vector3D Vector3D::normalize(float ref)
{
    return Vector3D(
        x * ref,
        y * ref,
        z * ref
    );
}

String Vector3D::toString()
{
    return String(x) + F(",") + String(y) + F(",") + String(z) + F(",") + String(20.0);
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