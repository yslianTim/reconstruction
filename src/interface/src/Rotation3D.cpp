#include "Rotation3D.hpp"
#include "UQuaternion.hpp"
#include "EulerAngle.hpp"


void Rotation3D::Print(void) const
{
    std::cout <<"     | "<< fXX <<", "<< fXY <<", "<< fXZ <<" |"<< std::endl;
    std::cout <<" R = | "<< fYX <<", "<< fYY <<", "<< fYZ <<" |"<< std::endl;
    std::cout <<"     | "<< fZX <<", "<< fZY <<", "<< fZZ <<" |"<< std::endl;
}

EulerAngle Rotation3D::GetEuler(void) const
{
    // TODO: deal with singularities of Euler angle (gimbal locks)
    double phi, theta, psi;
    double atan_phi = atan2( fZY,-fZX );
    double atan_psi = atan2( fYZ, fXZ );
    phi   = ( atan_phi<0.0 ) ? atan_phi + 2.0*PI() : atan_phi;
    theta = acos(fZZ);
    psi   = ( atan_psi<0.0 ) ? atan_psi + 2.0*PI() : atan_psi;
    return EulerAngle(phi, theta, psi);
}

UQuaternion Rotation3D::GetQuaternion(void) const
{
    double qw, qi, qj, qk;
    qw = 0.5*sqrt( 1.0 + fXX + fYY + fZZ );
    qi = 0.5*sqrt( 1.0 + fXX - fYY - fZZ )*( fZY - fYZ )/fabs( fZY - fYZ );
    qj = 0.5*sqrt( 1.0 - fXX + fYY - fZZ )*( fXZ - fZX )/fabs( fXZ - fZX );
    qk = 0.5*sqrt( 1.0 - fXX - fYY + fZZ )*( fYX - fXY )/fabs( fYX - fXY );
    return UQuaternion(qw, qi, qj, qk);
}

bool Rotation3D::operator== (const Rotation3D & rot) const
{
    if( fXX != rot.fXX ) return false;
    if( fXY != rot.fXY ) return false;
    if( fXZ != rot.fXZ ) return false;
    if( fYX != rot.fYZ ) return false;
    if( fYY != rot.fYY ) return false;
    if( fYZ != rot.fYZ ) return false;
    if( fZX != rot.fZX ) return false;
    if( fZY != rot.fZY ) return false;
    if( fZZ != rot.fZZ ) return false;
    return true;
}

bool Rotation3D::operator!= (const Rotation3D & rot) const
{
    return !(operator==(rot));
}

double Rotation3D::operator() (int i) const
{
    switch(i)
    {
        case 0: return fXX;
        case 1: return fXY;
        case 2: return fXZ;
        case 3: return fYX;
        case 4: return fYY;
        case 5: return fYZ;
        case 6: return fZX;
        case 7: return fZY;
        case 8: return fZZ;
        default:
            std::cout <<"[ERROR] Operator()(i) using out of range index i: "<< i << std::endl;
    }
    return 0;
}

Rotation3D & Rotation3D::operator= (const Rotation3D & rot)
{
    SetComponents( rot(0), rot(1), rot(2),
                   rot(3), rot(4), rot(5), 
                   rot(6), rot(7), rot(8)  );
    return (*this);
}

Vector3D Rotation3D::operator* (const Vector3D & vec) const
{
    return Vector3D(
        fXX*vec.X() + fXY*vec.Y() + fXZ*vec.Z(),
        fYX*vec.X() + fYY*vec.Y() + fYZ*vec.Z(),
        fZX*vec.X() + fZY*vec.Y() + fZZ*vec.Z()
    );
}
