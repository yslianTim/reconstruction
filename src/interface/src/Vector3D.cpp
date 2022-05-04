#include "Vector3D.hpp"

Vector3D & Vector3D::operator= (const Vector3D & vectorInput)
{
    vector(0) = vectorInput.X();
    vector(1) = vectorInput.Y();
    vector(2) = vectorInput.Z();
}