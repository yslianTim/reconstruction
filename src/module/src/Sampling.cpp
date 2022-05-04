#include "Sampling.hpp"

void Sampling::Print(void) const
{
    std::cout << "Order = " << order << std::endl;
    std::cout << "nSample = " << nSample << std::endl;
    std::cout << "nQuaternion = " << quat_sample.size() << std::endl;
}

void Sampling::query_disc(const pointing &ptg, double radius, std::vector<int>& listSample) const
{
    auto thisX = sin(ptg.theta)*cos(ptg.phi);
    auto thisY = sin(ptg.theta)*sin(ptg.phi);
    auto thisZ = cos(ptg.theta);
    vec3 vthis(thisX, thisY, thisZ);
    std::vector<std::pair<int, double>> list;
    list.reserve(nSample);
    for (int iSample = 0; iSample < nSample; ++iSample)
    {
        auto dist = distance(vthis, GetVector(iSample));
        if (dist > radius) continue;
        list.push_back(std::make_pair(iSample, dist));
        std::cout << "[" << iSample << ", " << dist << "]" << std::endl;
    }
    list.shrink_to_fit();
    std::sort(list.begin(), list.end(), cmp);

    listSample.clear();
    listSample.reserve(list.size());
    std::cout << "Sorted:" << std::endl;
    for (auto & i : list)
    {
        std::cout << "[" << i.first << ", " << i.second << "]" << std::endl;
        listSample.push_back(i.first);
    }
}