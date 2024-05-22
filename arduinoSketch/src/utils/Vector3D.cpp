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
    return String(x) + F(",") + String(y) + F(",") + String(z);
}