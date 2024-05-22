#ifndef vector3d_h
#define vector3d_h

#include <Arduino.h>


/**
 * 3D Vector with x, y and z coordinates
 */
class Vector3D
{
    public:
        Vector3D(float x,float y, float z) : x(x), y(y), z(z) {};

        /**
         * Vector coordinate 
         */
        float x, y, z;

        /**
         * Normalize the vector by multiplying all
         * its components by the given reference
         * @param ref Factor that will multiply the coordinates
         * @return A new vector instance with multiplied coordinates
         */
        Vector3D normalize(float ref);

        /**
         * Get the printable and plottable string version of this vector 
         */
        String toString();
};

#endif