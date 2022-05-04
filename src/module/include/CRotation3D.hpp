#ifndef CROTATION3D_HPP_
#define CROTATION3D_HPP_

#include <cmath>
#include <iostream>
#include "Vector3D.hpp"
#include "Matrix3D.hpp"

using namespace Eigen;

class CEulerAngle;
class CQuaternion;

class CRotation3D
{
public:

    // Constructor
    CRotation3D(void) :
        fXX(1.0), fXY(0.0), fXZ(0.0),
        fYX(0.0), fYY(1.0), fYZ(0.0),
        fZX(0.0), fZY(0.0), fZZ(1.0) {}

    CRotation3D(double xx, double xy, double xz,
                double yx, double yy, double yz,
                double zx, double zy, double zz) :
        fXX(xx), fXY(xy), fXZ(xz),
        fYX(yx), fYY(yy), fYZ(yz),
        fZX(zx), fZY(zy), fZZ(zz) {}

    CRotation3D(const Matrix3D & matrix) :
        fXX(matrix.XX()), fXY(matrix.XY()), fXZ(matrix.XZ()),
        fYX(matrix.YX()), fYY(matrix.YY()), fYZ(matrix.YZ()),
        fZX(matrix.ZX()), fZY(matrix.ZY()), fZZ(matrix.ZZ()) {}
    

    // Destructor
    ~CRotation3D(void) {}


    void Print(void) const;

    void SetComponents(double xx, double xy, double xz,
                       double yx, double yy, double yz,
                       double zx, double zy, double zz)
    {
        fXX=xx, fXY=xy, fXZ=xz, fYX=yx, fYY=yy, fYZ=yz, fZX=zx, fZY=zy, fZZ=zz;
    }

    void GetComponents(double & xx, double & xy, double & xz,
                       double & yx, double & yy, double & yz,
                       double & zx, double & zy, double & zz) const
    {
        xx=fXX, xy=fXY, xz=fXZ, yx=fYX, yy=fYY, yz=fYZ, zx=fZX, zy=fZY, zz=fZZ;
    }

    CEulerAngle GetEuler(void) const;
    CQuaternion GetQuaternion(void) const;

    // Operator Overloading
    bool          operator== (const CRotation3D &) const;
    bool          operator!= (const CRotation3D &) const;
    double        operator() (int i) const;
    double        operator[] (int i) const { return operator()(i); }
    CRotation3D & operator=  (const CRotation3D &);
    Vector3D      operator*  (const Vector3D &) const;

private:
/*
    Definition of Rotation Matrix:
            | fXX fXY fXZ |
        R = | fYX fYY fYZ |
            | fZX fZY fZZ |
*/
    double fXX, fXY, fXZ; //
    double fYX, fYY, fYZ; // 9 elements for representing 3 x 3 rotation matrix
    double fZX, fZY, fZZ; //

    static double PI(void) { return M_PI; }

};

#endif