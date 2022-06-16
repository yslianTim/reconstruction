#ifndef MATRIX3D_HPP_
#define MATRIX3D_HPP_

#include <iostream>
#include <vector>
#include <Eigen/Dense>


class Matrix3D
{
public:
    Matrix3D(void)
    {
        matrix << Eigen::Matrix3d::Zero();
    }

    Matrix3D(double xx, double xy, double xz,
             double yx, double yy, double yz,
             double zx, double zy, double zz)
    {
        matrix << xx, xy, xz,
                  yx, yy, yz,
                  zx, zy, zz;
    }
    
    Matrix3D(const Matrix3D & matrixInput)
    {
        matrix << matrixInput.XX(), matrixInput.XY(), matrixInput.XZ(),
                  matrixInput.YX(), matrixInput.YY(), matrixInput.YZ(),
                  matrixInput.ZX(), matrixInput.ZY(), matrixInput.ZZ();
    }

    
    ~Matrix3D(void) {}

    double XX(void) const { return matrix(0,0); }
    double XY(void) const { return matrix(0,1); }
    double XZ(void) const { return matrix(0,2); }
    double YX(void) const { return matrix(1,0); }
    double YY(void) const { return matrix(1,1); }
    double YZ(void) const { return matrix(1,2); }
    double ZX(void) const { return matrix(2,0); }
    double ZY(void) const { return matrix(2,1); }
    double ZZ(void) const { return matrix(2,2); }

    void SetComponents(double xx, double xy, double xz,
                       double yx, double yy, double yz,
                       double zx, double zy, double zz)
    {
        matrix << xx, xy, xz,
                  yx, yy, yz,
                  zx, zy, zz;
    }

    void GetComponents(double & xx, double & xy, double & xz,
                       double & yx, double & yy, double & yz,
                       double & zx, double & zy, double & zz) const
    {
        xx=XX(), xy=XY(), xz=XZ(),
        yx=YX(), yy=YY(), yz=YZ(),
        zx=ZX(), zy=ZY(), zz=ZZ();
    }

    Matrix3D & operator= (const Matrix3D &);

private:
    Eigen::Matrix3d matrix;

};

#endif