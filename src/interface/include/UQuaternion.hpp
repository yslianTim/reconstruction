#ifndef UQUATERNION_HPP_
#define UQUATERNION_HPP_

#include <cmath>
#include <iostream>
#include "Vector3D.hpp"


class Rotation3D;
class EulerAngle;

class UQuaternion
{
public:
    
    // Constructor
    UQuaternion(void) :
        fW(1.0), fI(0.0), fJ(0.0), fK(0.0) {}

    UQuaternion(const UQuaternion & q) :
        fW(q.fW), fI(q.fI), fJ(q.fJ), fK(q.fK) {}

    UQuaternion(double qw, double qi, double qj, double qk) :
        fW(qw), fI(qi), fJ(qj), fK(qk) {}

    UQuaternion(double qw, const Vector3D & vec) :
        fW(qw), fI(vec.X()), fJ(vec.Y()), fK(vec.Z()) {}


    // Destructor
    ~UQuaternion(void) {}

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
    UQuaternion GetConjugate(void) const { return UQuaternion( fW, -fI, -fJ, -fK ); }
    UQuaternion GetUnit(void) const { return UQuaternion( fW, fI, fJ, fK )/GetNorm(); }
    UQuaternion GetInverse(void) const;
    EulerAngle GetEuler(void) const;
    Rotation3D GetRotMatrix(void) const;
    UQuaternion & Normalize(void) { return (*this) /= GetNorm(); }
    Vector3D  Rotation(const Vector3D & vec);

    // Operator Overloading
    // -- General
    bool   operator== (const UQuaternion &) const;
    bool   operator!= (const UQuaternion &) const;
    double operator() (int) const;
    double operator[] (int i) const { return operator()(i); }
    UQuaternion operator- (void) const { return UQuaternion(-fW, -fI, -fJ, -fK); }
    UQuaternion & operator= (const UQuaternion &);

    // -- The algebra of scaler
    UQuaternion operator* (double s) const { return UQuaternion(fW*s, fI*s, fJ*s, fK*s); }
    UQuaternion operator/ (double) const;
    UQuaternion & operator*= (double);
    UQuaternion & operator/= (double);

    // -- The algebea of quaternion
    UQuaternion operator+ (const UQuaternion & q) const
        { return UQuaternion( fW+q.fW, fI+q.fI, fJ+q.fJ, fK+q.fK ); }
    UQuaternion operator- (const UQuaternion & q) const
        { return UQuaternion( fW-q.fW, fI-q.fI, fJ-q.fJ, fK-q.fK ); }
    UQuaternion operator* (const UQuaternion &) const;
    UQuaternion & operator+= (const UQuaternion &);
    UQuaternion & operator-= (const UQuaternion &);
    UQuaternion & operator*= (const UQuaternion & q) { return *this = (*this)*q; }

private:
    double fW;
    double fI;
    double fJ;
    double fK;
};

#endif