/// @addtogroup Arduino
/// @{
#ifndef vector3d_h
#define vector3d_h

#include <Arduino.h>


/** 3D Vector with x, y and z coordinates */
class Vector3D
{
    public:
        Vector3D() : x(0.0), y(0.0), z(0.0) {};
        Vector3D(float x,float y, float z) : x(x), y(y), z(z) {};

        float x, y, z;

        /** Get the printable and plottable string version of this vector */
        String toString();

        /** Define the operation to do when multiply by a scalar value */
        Vector3D operator*(float const &k);

        /** Define the operation to do when dividing by a scalar value */
        Vector3D operator/(float const &k);

        /** Define the operation to do when substracting two vectors */
        Vector3D operator-(Vector3D const &other);

        /** Define the operation to do when adding two vectors */
        Vector3D operator+(Vector3D const &other);
};

#endif

/// @} 