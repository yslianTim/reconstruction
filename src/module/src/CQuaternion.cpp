#include "CRotation3D.hpp"
#include "CQuaternion.hpp"
#include "CEulerAngle.hpp"


void CQuaternion::Print(void) const
{
    std::cout <<"( qW = "<< fW <<", qI = "<< fI <<", qJ = "<< fJ <<", qK = "<< fK <<" ) ; ";
    std::cout <<"Norm = "<< GetNorm() << std::endl;
}

CQuaternion CQuaternion::GetInverse(void) const
{
    if( GetNorm()==0 ) {
        std::cout <<"[ERROR] Quaternion should be nonzero to obtain inverse!"<< std::endl;
        return (*this);
    }
    return GetConjugate()/GetNorm2();
}

CEulerAngle CQuaternion::GetEuler(void) const
{
    // TODO: deal with singularities of Euler angle (gimbal locks)
    double phi, theta, psi;
    double wi=fW*fI, wj=fW*fJ, wk=fW*fK, ij=fI*fJ, ik=fI*fK, jk=fJ*fK;
    double w2=fW*fW, i2=fI*fI, j2=fJ*fJ, k2=fK*fK;
    phi   = 2.0*( atan2(-wj-ik+sqrt((w2+k2)*(i2+j2))+2.0*ik, jk+wi) );
    theta = 2.0*( acos(sqrt(w2+k2)) );
    psi   = 2.0*( atan2(-wj-ik+sqrt((w2+k2)*(i2+j2)), jk-wi) );
    return CEulerAngle(phi, theta, psi);
}

CRotation3D CQuaternion::GetRotMatrix(void) const
{
    double xx, xy, xz, yx, yy, yz, zx, zy, zz;
    double wi=fW*fI, wj=fW*fJ, wk=fW*fK, ij=fI*fJ, ik=fI*fK, jk=fJ*fK;
    double w2=fW*fW, i2=fI*fI, j2=fJ*fJ, k2=fK*fK;
    xx = 1.0 - 2.0*j2 - 2.0*k2;
    xy =       2.0*ij - 2.0*wk;
    xz =       2.0*wj + 2.0*ik;
    yx =       2.0*wk + 2.0*ij;
    yy = 1.0 - 2.0*i2 - 2.0*k2;
    yz =       2.0*jk - 2.0*wi;
    zx =       2.0*ik - 2.0*wj;
    zy =       2.0*wi + 2.0*jk;
    zz = 1.0 - 2.0*i2 - 2.0*j2;
    return CRotation3D(xx, xy, xz, yx, yy, yz, zx, zy, zz);
}

Vector3D CQuaternion::Rotation(const Vector3D & vec)
{
    if( GetNorm()-1.0 > 1.0E-8 ) {
        std::cout <<"[ERROR] Only unit quaternion can be used for rotation!"<< std::endl;
        return vec;
    }
    CQuaternion qunit(*this);
    CQuaternion qvec(0.0, vec);
    qvec *= qunit.GetConjugate();
    qunit *= qvec;
    return qunit.GetVector();
}

// Operator Overloading
bool CQuaternion::operator== (const CQuaternion &q) const
{
    return ( fW == q.fW && fI == q.fI && fJ == q.fJ && fK == q.fK ) ? true : false;
}
bool CQuaternion::operator!= (const CQuaternion &q) const
{
    return ( fW != q.fW || fI != q.fI && fJ != q.fJ && fK != q.fK ) ? true : false;
}
double CQuaternion::operator() (int i) const
{
    switch(i)
    {
        case 0:
            return fW;
        case 1:
            return fI;
        case 2:
            return fJ;
        case 3:
            return fK;
        default:
            std::cout <<"[ERROR] Operator()(i) using out of range index i: "<< i << std::endl;
    }
    return fW;
}
CQuaternion & CQuaternion::operator= (const CQuaternion & q)
{
    fW = q.fW;
    fI = q.fI;
    fJ = q.fJ;
    fK = q.fK;
    return (*this);
}

// -- The algebra of scaler
CQuaternion CQuaternion::operator/ (double s) const
{
    if (s==0) {
        std::cout <<"[ERROR] Dividing with zero value! skip this division."<< std::endl;
        return (*this);
    } else {
        return CQuaternion(fW/s, fI/s, fJ/s, fK/s);
    }
}
CQuaternion & CQuaternion::operator*= (double s)
{
    fW *= s;
    fI *= s;
    fJ *= s;
    fK *= s;
    return (*this);
}
CQuaternion & CQuaternion::operator/= (double s)
{
    if (s==0) {
        std::cout <<"[ERROR] Dividing with zero value! skip this division."<< std::endl;
        return (*this);
    } else {
        fW /= s;
        fI /= s;
        fJ /= s;
        fK /= s;
        return (*this);
    }
}

// -- The algebea of quaternion
CQuaternion CQuaternion::operator* (const CQuaternion & q) const
{
    return CQuaternion( fW*q.fW - fI*q.fI - fJ*q.fJ - fK*q.fK,
                        fW*q.fI + fI*q.fW + fJ*q.fK - fK*q.fJ,
                        fW*q.fJ - fI*q.fK + fJ*q.fW + fK*q.fI,
                        fW*q.fK + fI*q.fJ - fJ*q.fI + fK*q.fW );
}
CQuaternion & CQuaternion::operator+= (const CQuaternion & q)
{
    fW += q.fW;
    fI += q.fI;
    fJ += q.fJ;
    fK += q.fK;
    return (*this);
}
CQuaternion & CQuaternion::operator-= (const CQuaternion & q)
{
    fW -= q.fW;
    fI -= q.fI;
    fJ -= q.fJ;
    fK -= q.fK;
    return (*this);
}
