#include "CRotation3D.hpp"
#include "CEulerAngle.hpp"
#include "CQuaternion.hpp"

void CEulerAngle::Print(void) const
{
    std::cout <<"( Phi = "<< fPhi <<", Theta = "<< fTheta <<", Psi = "<< fPsi <<" ) ; ";
    std::cout <<"Euler angle in ZYZ convension."<< std::endl;
}

CQuaternion CEulerAngle::GetQuaternion(void) const
{
    double qw, qi, qj, qk;
    qw = cos( fTheta/2.0 ) * cos( (fPhi+fPsi)/2.0 );
    qi = sin( fTheta/2.0 ) * sin( (fPhi-fPsi)/2.0 );
    qj = sin( fTheta/2.0 ) * cos( (fPhi-fPsi)/2.0 );
    qk = cos( fTheta/2.0 ) * sin( (fPhi+fPsi)/2.0 );
    return CQuaternion(qw, qi, qj, qk);
}

CRotation3D CEulerAngle::GetRotMatrix(void) const
{
    double xx, xy, xz, yx, yy, yz, zx, zy, zz;
    double cPhi   = cos(fPhi),   sPhi   = sin(fPhi);
    double cTheta = cos(fTheta), sTheta = sin(fTheta);
    double cPsi   = cos(fPsi),   sPsi   = sin(fPsi);
    xx = - sPhi*sPsi + cTheta*cPhi*cPsi;
    xy = - cPhi*sPsi - cTheta*sPhi*cPsi;
    xz =   sTheta*cPsi;
    yx =   sPhi*cPsi + cTheta*cPhi*sPsi;
    yy =   cPhi*cPsi - cTheta*sPhi*sPsi;
    yz =   sTheta*sPsi;
    zx = - sTheta*cPhi;
    zy =   sTheta*sPhi;
    zz =   cTheta;
    return CRotation3D(xx, xy, xz, yx, yy, yz, zx, zy, zz);
}
