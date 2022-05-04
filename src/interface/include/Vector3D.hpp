#ifndef VECTOR3D_HPP_
#define VECTOR3D_HPP_

#include <iostream>
#include <vector>
#include <Eigen/Dense>


class Vector3D
{
    Eigen::Vector3d vector;
    
public:
    Vector3D(void) :
        vector(0.0, 0.0, 0.0) {}

    Vector3D(double x, double y, double z) :
        vector(x, y, z) {}

    Vector3D(const Vector3D & vectorInput) :
        vector(vectorInput.X(), vectorInput.Y(), vectorInput.Z()) {}
    
    ~Vector3D(void) {}

    double X(void) const { return vector(0); }
    double Y(void) const { return vector(1); }
    double Z(void) const { return vector(2); }

    Vector3D & operator= (const Vector3D &);

private:
    

};

#endif