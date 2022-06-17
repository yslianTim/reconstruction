#ifndef VOLUMEOBJECT_HPP_
#define VOLUMEOBJECT_HPP_

#include <iostream>
#include <iomanip>
#include <memory>
#include "TH3.h"

class VolumeObject
{
    std::unique_ptr<float[]> data;
    std::shared_ptr<TH3F> picture;
    int n_bin_x, n_bin_y, n_bin_z;
    float bin_size_x, bin_size_y, bin_size_z;

public:
    VolumeObject(void) {}
    VolumeObject(int _n_bin_x, int _n_bin_y, int _n_bin_z,
                 float _bin_size_x, float _bin_size_y, float _bin_size_z) :
        n_bin_x{ _n_bin_x }, n_bin_y{ _n_bin_y }, n_bin_z{ _n_bin_z },
        bin_size_x{ _bin_size_x }, bin_size_y{ _bin_size_y }, bin_size_z{ _bin_size_z },
        data{ std::make_unique<float[]>(_n_bin_x * _n_bin_y * _n_bin_z) } {}
    ~VolumeObject() {}
    void Print(void) const;
    void Set(int _n_bin_x, int _n_bin_y, int _n_bin_z,
             float _bin_size_x, float _bin_size_y, float _bin_size_z);
    void ImportData(std::unique_ptr<float[]> & _data);
    int GetNBinX(void) const { return n_bin_x; }
    int GetNBinY(void) const { return n_bin_y; }
    int GetNBinZ(void) const { return n_bin_z; }
    float GetBinSizeX(void) const { return bin_size_x; }
    float GetBinSizeY(void) const { return bin_size_y; }
    float GetBinSizeZ(void) const { return bin_size_z; }
    float GetEntry(int _pixel_x, int _pixel_y, int _pixel_z) const {
        return data[_pixel_x + n_bin_x*(_pixel_y + n_bin_y*_pixel_z)];
    }
    std::shared_ptr<TH3F> GetPicture(void) const { return picture; }

private:
    void MakePicture(void);

};




#endif