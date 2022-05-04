#ifndef SAMPLING_HPP_
#define SAMPLING_HPP_

#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>
#include <ctime>

#include "CQuaternion.hpp"
#include "CEulerAngle.hpp"

#include "vec3.hpp"
#include "pointing.hpp"

class Sampling
{
    int order, nside, npface;
    int nSample;
    std::vector<CQuaternion> quat_sample;

public:
    Sampling(void)
    {
        
    }

    void Set(int order)
    {
        Sampling::order = order;
        nside = 1<<order;
        npface = nside<<order;
        nSample = 12*npface;
        quat_sample.clear();
        quat_sample.reserve(nSample);
        std::mt19937 generator( time(NULL) );
        std::uniform_real_distribution<double> unif(0.0, 1.0);
        for (int i = 0; i < nSample; ++i)
        {
            auto uniform_s = unif(generator);
            auto uniform_ang1 = unif(generator) * 2.0 * M_PI;
            auto uniform_ang2 = unif(generator) * 2.0 * M_PI;
            CQuaternion quat( sqrt(    uniform_s) * cos(uniform_ang2),
                              sqrt(1.0-uniform_s) * sin(uniform_ang1),
                              sqrt(1.0-uniform_s) * cos(uniform_ang1),
                              sqrt(    uniform_s) * sin(uniform_ang2) );
            quat_sample.push_back(quat);
        }
    }

    void Print(void) const;
    int NumberOfSample(void) const { return nSample; }
    std::vector<CQuaternion> GetSamplingQuat(void) { return quat_sample; }
    vec3 GetVector(int iSample) const
    {
        return vec3(quat_sample.at(iSample).I(),quat_sample.at(iSample).J(),quat_sample.at(iSample).K());
    }
    double GetPhi(int iSample) const { return quat_sample.at(iSample).GetEuler().Phi(); }
    double GetTheta(int iSample) const { return quat_sample.at(iSample).GetEuler().Theta(); }
    void query_disc(const pointing &ptg, double radius, std::vector<int>& listSample) const;
    double distance(vec3 v1, vec3 v2) const { return atan2(crossprod(v1,v2).Length(), dotprod(v1,v2)); }

private:
    static bool cmp(std::pair<int,double>a, std::pair<int,double>b) { return a.second < b.second; }

};



#endif