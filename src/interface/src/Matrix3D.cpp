#include "Matrix3D.hpp"

Matrix3D & Matrix3D::operator= (const Matrix3D & matrixInput)
{
    matrix(0,0) = matrixInput.XX();
    matrix(0,1) = matrixInput.XY();
    matrix(0,2) = matrixInput.XZ();
    matrix(1,0) = matrixInput.YX();
    matrix(1,1) = matrixInput.YY();
    matrix(1,2) = matrixInput.YZ();
    matrix(2,0) = matrixInput.ZX();
    matrix(2,1) = matrixInput.ZY();
    matrix(2,2) = matrixInput.ZZ();
}