#ifndef vector3d_h
#define vector3d_h


class Vector3D
{
    public:
        Vector3D(float x,float y, float z) : x(x), y(y), z(z) {};

        /**
         * Vector coordinate 
         */
        float x, y, z;

        /**
         * Normalize the vector by dividing all
         * it component by the given reference
         * @param ref Reference for the vector
         */
        Vector3D normalize(float ref);
};

#endif