#include "ECP_Base.hpp"

void ECP_Base::Print(void) const
{
    std::cout << "Order = " << order << std::endl;
    std::cout << "nPhi = " << nPhi << std::endl;
    std::cout << "nTheta = " << nTheta << std::endl;
    std::cout << "nGrid = " << nGrid << std::endl;
}

void ECP_Base::query_disc(const pointing &ptg, double radius, std::vector<int>& listGrid) const
{
    auto thisX = sin(ptg.theta)*cos(ptg.phi);
    auto thisY = sin(ptg.theta)*sin(ptg.phi);
    auto thisZ = cos(ptg.theta);
    vec3 vthis(thisX, thisY, thisZ);
    std::vector<std::pair<int, double>> list;
    list.reserve(nGrid);
    for (int iGrid = 0; iGrid < nGrid; ++iGrid)
    {
        auto dist = distance(vthis, GridToVec3(iGrid));
        if (dist > radius) continue;
        list.push_back(std::make_pair(iGrid, dist));
        //std::cout << "[" << iGrid << ", " << dist << "]" << std::endl;
    }
    list.shrink_to_fit();
    std::sort(list.begin(), list.end(), cmp);

    listGrid.clear();
    listGrid.reserve(list.size());
    //std::cout << "Sorted:" << std::endl;
    for (auto & i : list)
    {
        //std::cout << "[" << i.first << ", " << i.second << "]" << std::endl;
        listGrid.push_back(i.first);
    }
}


/*
using namespace std;

short ECP_Base::ctab[];
short ECP_Base::utab[];
const int ECP_Base::jrll[] = { 2,2,2,2,3,3,3,3,4,4,4,4 };
const int ECP_Base::jpll[] = { 1,3,5,7,0,2,4,6,1,3,5,7 };

int ECP_Base::ring_above(double z) const
{
    double az=abs(z);
    if (az>(2.0/3.0)) // polar caps
    {
        int iring = int(nside_*sqrt(3*(1-az)));
        return (z>0) ? iring : 4*nside_-iring-1;
    }
    else // ----- equatorial region ---------
    return int(nside_*(2-1.5*z));
}

void ECP_Base::in_ring(int iz, double phi0, double dphi, vector<int> &listir) const
{
    int nr, ir, ipix1;
    double shift=0.5;

    if (iz<nside_) // north pole
    {
        ir = iz;
        nr = ir*4;
        ipix1 = 2*ir*(ir-1);        //    lowest pixel number in the ring
    }
    else if (iz>(3*nside_)) // south pole
    {
        ir = 4*nside_ - iz;
        nr = ir*4;
        ipix1 = npix_ - 2*ir*(ir+1); // lowest pixel number in the ring
    }
    else // equatorial region
    {
        ir = iz - nside_ + 1;           //    within {1, 2*nside + 1}
        nr = nside_*4;
        if ((ir&1)==0) shift = 0;
        ipix1 = ncap_ + (ir-1)*nr; // lowest pixel number in the ring
    }

    int ipix2 = ipix1 + nr - 1;       //    highest pixel number in the ring

    // ----------- constructs the pixel list --------------
    if (dphi > (M_PI-1e-7))
        for (int i=ipix1; i<=ipix2; ++i) listir.push_back(i);
    else
    {
        int ip_lo = ifloor<int>(nr*(1.0/(2.0*M_PI))*(phi0-dphi) - shift)+1;
        int ip_hi = ifloor<int>(nr*(1.0/(2.0*M_PI))*(phi0+dphi) - shift);
        int pixnum = ip_lo+ipix1;
        if (pixnum<ipix1) pixnum += nr;
        for (int i=ip_lo; i<=ip_hi; ++i, ++pixnum)
        {
            if (pixnum>ipix2) pixnum -= nr;
            listir.push_back(pixnum);
        }
    }
}

void ECP_Base::nest2xyf (int pix, int &ix, int &iy, int &face_num) const
{
    face_num = pix>>(2*order_);
    pix &= (npface_-1);
    int raw = (pix&0x5555) | ((pix&0x55550000)>>15);
    ix = ctab[raw&0xff] | (ctab[raw>>8]<<4);
    pix >>= 1;
    raw = (pix&0x5555) | ((pix&0x55550000)>>15);
    iy = ctab[raw&0xff] | (ctab[raw>>8]<<4);
}

int ECP_Base::xyf2nest(int ix, int iy, int face_num) const
{
    return (face_num<<(2*order_)) +
            (utab[ix&0xff] | (utab[ix>>8]<<16)
        | (utab[iy&0xff]<<1) | (utab[iy>>8]<<17));
}

void ECP_Base::ring2xyf(int pix, int &ix, int &iy, int &face_num) const
{
    int iring, iphi, kshift, nr;
    int nl2 = 2*nside_;

    if (pix<ncap_) // North Polar cap
    {
        iring = int(0.5*(1+isqrt(1+2*pix))); //counted from North pole
        iphi  = (pix+1) - 2*iring*(iring-1);
        kshift = 0;
        nr = iring;
        face_num=0;
        int tmp = iphi-1;
        if (tmp>=(2*iring))
        {
            face_num=2;
            tmp-=2*iring;
        }
        if (tmp>=iring) ++face_num;
    }
    else if (pix<(npix_-ncap_)) // Equatorial region
    {
        int ip = pix - ncap_;
        if (order_>=0)
        {
            iring = (ip>>(order_+2)) + nside_; // counted from North pole
            iphi  = (ip&(4*nside_-1)) + 1;
        }
        else
        {
            iring = (ip/(4*nside_)) + nside_; // counted from North pole
            iphi  = (ip%(4*nside_)) + 1;
        }
        kshift = (iring+nside_)&1;
        nr = nside_;
        unsigned int ire = iring-nside_+1;
        unsigned int irm = nl2+2-ire;
        int ifm, ifp;
        if (order_>=0)
        {
            ifm = (iphi - ire/2 + nside_ -1) >> order_;
            ifp = (iphi - irm/2 + nside_ -1) >> order_;
        }
        else
        {
            ifm = (iphi - ire/2 + nside_ -1) / nside_;
            ifp = (iphi - irm/2 + nside_ -1) / nside_;
        }
        if (ifp == ifm) // faces 4 to 7
            face_num = (ifp==4) ? 4 : ifp+4;
        else if (ifp<ifm) // (half-)faces 0 to 3
            face_num = ifp;
        else // (half-)faces 8 to 11
            face_num = ifm + 8;
    }
    else // South Polar cap
    {
        int ip = npix_ - pix;
        iring = int(0.5*(1+isqrt(2*ip-1))); //counted from South pole
        iphi  = 4*iring + 1 - (ip - 2*iring*(iring-1));
        kshift = 0;
        nr = iring;
        iring = 2*nl2-iring;
        face_num=8;
        int tmp = iphi-1;
        if (tmp>=(2*nr))
        {
            face_num=10;
            tmp-=2*nr;
        }
        if (tmp>=nr) ++face_num;
    }

    int irt = iring - (jrll[face_num]*nside_) + 1;
    int ipt = 2*iphi- jpll[face_num]*nr - kshift -1;
    if (ipt>=nl2) ipt-=8*nside_;

    ix =  (ipt-irt) >>1;
    iy =(-(ipt+irt))>>1;
}

int ECP_Base::ring2nest (int pix) const
{
    //planck_assert(order_>=0, "ring2nest: need hierarchical map");
    int ix, iy, face_num;
    ring2xyf (pix, ix, iy, face_num);
    return xyf2nest (ix, iy, face_num);
}

void ECP_Base::pix2ang_z_phi (int pix, double &z, double &phi) const
{
    int nl4 = nside_*4;
    int face_num, ix, iy;
    nest2xyf(pix,ix,iy,face_num);

    int jr = (jrll[face_num]<<order_) - ix - iy - 1;
    int nr, kshift;
    if (jr<nside_)
    {
        nr = jr;
        z = 1 - nr*nr*fact2_;
        kshift = 0;
    }
    else if (jr > 3*nside_)
    {
        nr = nl4-jr;
        z = nr*nr*fact2_ - 1;
        kshift = 0;
    }
    else
    {
        nr = nside_;
        z = (2*nside_-jr)*fact1_;
        kshift = (jr-nside_)&1;
    }
    int jp = (jpll[face_num]*nr + ix -iy + 1 + kshift) / 2;
    if (jp>nl4) jp-=nl4;
    if (jp<1) jp+=nl4;
    phi = (jp-(kshift+1)*0.5)*((M_PI/2.0)/nr);
}

void ECP_Base::query_disc(const pointing &ptg, double radius, vector<int>& listpix) const
{
    listpix.clear();

    double dth1 = fact2_;
    double dth2 = fact1_;
    double cosang = cos(radius);
    double z0 = cos(ptg.theta);
    double xa = 1./sqrt((1-z0)*(1+z0));
    double rlat1  = ptg.theta - radius;
    double zmax = cos(rlat1);
    int irmin = ring_above (zmax)+1;

    if (rlat1<=0) // north pole in the disc
        for (int m=1; m<irmin; ++m) // rings completely in the disc
            in_ring(m, 0, M_PI, listpix);

    double rlat2  = ptg.theta + radius;
    double zmin = cos(rlat2);
    int irmax = ring_above (zmin);

    // ------------- loop on ring number ---------------------
    for (int iz=irmin; iz<=irmax; ++iz) // rings partially in the disc
    {
        double z;
        if (iz<nside_) // north polar cap
            z = 1.0 - iz*iz*dth1;
        else if (iz <= (3*nside_)) // tropical band + equat.
            z = (2*nside_-iz) * dth2;
        else
            z = -1.0 + (4*nside_-iz)*(4*nside_-iz)*dth1;

        // --------- phi range in the disc for each z ---------
        double x = (cosang-z*z0)*xa;
        double ysq = 1-z*z-x*x;
        //planck_assert(ysq>=0, "error in query_disc()");
        double dphi=atan2(sqrt(ysq),x);
        in_ring (iz, ptg.phi, dphi, listpix);
    }

    if (rlat2>=M_PI) // south pole in the disc
        for (int m=irmax+1; m<(4*nside_); ++m)  // rings completely in the disc
            in_ring (m, 0, M_PI, listpix);

    for (unsigned int m=0; m<listpix.size(); ++m)
        listpix[m] = ring2nest(listpix[m]);
}

double ECP_Base::max_pixrad() const
{
    vec3 va,vb;
    va.set_z_phi(2./3., M_PI/(4*nside_));
    double t1 = 1.-1./nside_;
    t1*=t1;
    vb.set_z_phi(1-t1/3, 0);
    return atan2(crossprod(va,vb).Length(), dotprod(va,vb));
}
*/