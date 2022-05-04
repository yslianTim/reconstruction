#ifndef CEULERANGLE_HPP_
#define CEULERANGLE_HPP_

#include <cmath>
#include <iostream>

class CRotation3D;
class CQuaternion;

class CEulerAngle
{
public:

    // Constructor
    CEulerAngle(void) :
        fPhi(0.0), fTheta(0.0), fPsi(0.0) {}

    CEulerAngle(double phi, double theta, double psi) :
        fPhi(phi), fTheta(theta), fPsi(psi) {}

    CEulerAngle(const CEulerAngle & angle) :
        fPhi(angle.fPhi), fTheta(angle.fTheta), fPsi(angle.fPsi) {}
    

    // Destructor
    ~CEulerAngle(void) {}

    void Print(void) const;

    double Phi(void) const   { return fPhi; }
    double Theta(void) const { return fTheta; }
    double Psi(void) const   { return fPsi; }
    void   SetComponents(double phi, double theta, double psi) { fPhi=phi, fTheta=theta, fPsi=psi; }
    void   GetComponents(double & phi, double & theta, double & psi) const { phi=fPhi, theta=fTheta, psi=fPsi; }
    CQuaternion GetQuaternion(void) const;
    CRotation3D GetRotMatrix(void) const;

private:
    double fPhi;    // Rotation angle along Z axis in a range [-PI, PI]
    double fTheta;  // Rotation angle along Y axis in a range [  0, PI]
    double fPsi;    // Rotation angle along Z axis in a range [-PI, PI]

    static double PI(void) { return M_PI; }

}; // === class CEulerAngle

#endif