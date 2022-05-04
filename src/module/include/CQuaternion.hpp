#ifndef CQUATERNION_HPP_
#define CQUATERNION_HPP_

#include <cmath>
#include <iostream>
#include "Vector3D.hpp"


class CRotation3D;
class CEulerAngle;

class CQuaternion
{
public:
    
    // Constructor
    CQuaternion(void) :
        fW(1.0), fI(0.0), fJ(0.0), fK(0.0) {}

    CQuaternion(const CQuaternion & q) :
        fW(q.fW), fI(q.fI), fJ(q.fJ), fK(q.fK) {}

    CQuaternion(double qw, double qi, double qj, double qk) :
        fW(qw), fI(qi), fJ(qj), fK(qk) {}

    CQuaternion(double qw, const Vector3D & vec) :
        fW(qw), fI(vec.X()), fJ(vec.Y()), fK(vec.Z()) {}


    // Destructor
    ~CQuaternion(void) {}

    void Print(void) const;
    
    double W(void) const { return fW; }
    double I(void) const { return fI; }
    double J(void) const { return fJ; }
    double K(void) const { return fK; }
    void        SetComponents(double w, double i, double j, double k)
    { fW=w, fI=i, fJ=j, fK=k; }
    void        GetComponents(double & w, double & i, double & j, double & k) const
    { w=fW, i=fI, j=fJ, k=fK; }
    double      GetNorm(void) const { return sqrt(GetNorm2()); }
    double      GetNorm2(void) const { return fW*fW + fI*fI + fJ*fJ + fK*fK; }
    Vector3D    GetVector(void) const { return Vector3D(fI, fJ, fK); }
    CQuaternion GetConjugate(void) const { return CQuaternion( fW, -fI, -fJ, -fK ); }
    CQuaternion GetUnit(void) const { return CQuaternion( fW, fI, fJ, fK )/GetNorm(); }
    CQuaternion GetInverse(void) const;
    CEulerAngle GetEuler(void) const;
    CRotation3D GetRotMatrix(void) const;
    CQuaternion & Normalize(void) { return (*this) /= GetNorm(); }
    Vector3D  Rotation(const Vector3D & vec);

    // Operator Overloading
    // -- General
    bool   operator== (const CQuaternion &) const;
    bool   operator!= (const CQuaternion &) const;
    double operator() (int) const;
    double operator[] (int i) const { return operator()(i); }
    CQuaternion operator- (void) const { return CQuaternion(-fW, -fI, -fJ, -fK); }
    CQuaternion & operator= (const CQuaternion &);

    // -- The algebra of scaler
    CQuaternion operator* (double s) const { return CQuaternion(fW*s, fI*s, fJ*s, fK*s); }
    CQuaternion operator/ (double) const;
    CQuaternion & operator*= (double);
    CQuaternion & operator/= (double);

    // -- The algebea of quaternion
    CQuaternion operator+ (const CQuaternion & q) const
    { return CQuaternion( fW+q.fW, fI+q.fI, fJ+q.fJ, fK+q.fK ); }
    CQuaternion operator- (const CQuaternion & q) const
    { return CQuaternion( fW-q.fW, fI-q.fI, fJ-q.fJ, fK-q.fK ); }
    CQuaternion operator* (const CQuaternion &) const;
    CQuaternion & operator+= (const CQuaternion &);
    CQuaternion & operator-= (const CQuaternion &);
    CQuaternion & operator*= (const CQuaternion & q) { return *this = (*this)*q; }

private:
    double fW;
    double fI;
    double fJ;
    double fK;
}; // === class CQuaternion





#endif