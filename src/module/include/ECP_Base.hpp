#ifndef ECP_HPP_
#define ECP_HPP_

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

#include "Vector3D.hpp"

#include "vec3.hpp"
#include "pointing.hpp"

class ECP_Base
{
    int order, nside, npface, npix;
    int nPhi, nTheta, nGrid;
    float sizePhi, sizeTheta;

public:
    ECP_Base(void) {}

    void Set(int order)
    {
        ECP_Base::order  = order;
        nside  = 1<<order;
        npface = nside<<order;
        npix   = 12*npface;
        ECP_Base::nPhi = 2*sqrt(npix/2);
        ECP_Base::nTheta = sqrt(npix/2);
        ECP_Base::sizePhi = 2.0*M_PI / ECP_Base::nPhi;
        ECP_Base::sizeTheta = M_PI / (ECP_Base::nTheta+1);
        ECP_Base::nGrid = ECP_Base::nPhi * ECP_Base::nTheta;
    }

    void Print(void) const;
    int NumberOfGrid() const { return nGrid; }
    int NumberOfPhi() const { return nPhi; }
    int NumberOfTheta() const { return nTheta; }
    int GetPhiIndex(int iGrid) const { return iGrid%nPhi; }
    int GetThetaIndex(int iGrid) const { return iGrid/nPhi; }
    double GridToPhi(int iGrid) const { return GetPhiIndex(iGrid)*ECP_Base::sizePhi; }
    double GridToTheta(int iGrid) const { return (GetThetaIndex(iGrid)+1)*ECP_Base::sizeTheta; }
    double GridToX(int iGrid) const { return sin(GridToTheta(iGrid))*cos(GridToPhi(iGrid)); }
    double GridToY(int iGrid) const { return sin(GridToTheta(iGrid))*sin(GridToPhi(iGrid)); }
    double GridToZ(int iGrid) const { return cos(GridToTheta(iGrid)); }
    vec3 GridToVec3(int iGrid) const { return vec3(GridToX(iGrid),GridToY(iGrid),GridToZ(iGrid)); }
    void query_disc(const pointing &ptg, double radius, std::vector<int>& listGrid) const;
    double distance(vec3 v1, vec3 v2) const { return atan2(crossprod(v1,v2).Length(), dotprod(v1,v2)); }

private:
    static bool cmp(std::pair<int,double>a, std::pair<int,double>b) { return a.second < b.second; }

};

/*
template<typename I, typename F> inline I ifloor(F arg)
{
    return (arg>=0) ? I(arg) : I(arg)-1;
}
template<typename I> inline unsigned int isqrt(I arg)
{
    using namespace std;
    if (sizeof(I)<=4)
        return unsigned (sqrt(arg+0.5));
    else
    {
        long double arg2 = arg;
        return unsigned (sqrt(arg2+0.5));
    }
}

class ECP_Base
{
protected:
    static short ctab[0x100], utab[0x100];
    static const int jrll[];
    static const int jpll[];
    int order_, nside_, npface_, ncap_, npix_;
    double fact1_, fact2_;
    inline int ring_above(double z) const;
    void in_ring(int iz, double phi0, double dphi, std::vector<int> &listir) const;

public:
    int xyf2nest(int ix, int iy, int face_num) const; //
    void nest2xyf(int pix, int &ix, int &iy, int &face_num) const; //
    void ring2xyf(int pix, int &ix, int &iy, int &face_num) const;

    ECP_Base() : order_(-1), nside_(0), npface_(0), ncap_(0), npix_(0), fact1_(0), fact2_(0) {}
    ECP_Base(int order) { Set(order); }

    void Set(int order)
    {
        order_  = order;
        nside_  = 1<<order;
        npface_ = nside_<<order_;
        ncap_   = (npface_-nside_)<<1;
        npix_   = 12*npface_;
        fact2_  = 4./npix_;
        fact1_  = (nside_<<1)*fact2_;
    }

    int Nside() const { return nside_; } //
    int Npix() const { return npix_; } //
    int ring2nest(int pix) const;
    void pix2ang_z_phi(int pix, double &z, double &phi) const; //
    void query_disc(const pointing &dir, double radius, std::vector<int> &listpix) const; // 
    double max_pixrad() const; // 
};*/



#endif