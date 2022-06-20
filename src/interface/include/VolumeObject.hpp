#ifndef VOLUMEOBJECT_HPP_
#define VOLUMEOBJECT_HPP_

#include <iostream>
#include <iomanip>
#include <memory>
#include <cmath>
#include "Math/SVector.h"
#include "Math/SMatrix.h"
#include "TH3.h"
#include "EulerAngle.hpp"
#include "Rotation3D.hpp"

typedef ROOT::Math::SVector<double,3> SVector3d;
typedef ROOT::Math::SMatrix<double,3> SMatrix3d;

class VolumeObject
{
    std::unique_ptr<float[]> data_ori, data_rot;
    std::shared_ptr<TH3F> picture, picture_rot;
    int n_bin_x, n_bin_y, n_bin_z;
    float bin_size_x, bin_size_y, bin_size_z;

public:
    VolumeObject(void) {}
    VolumeObject(int _n_bin_x, int _n_bin_y, int _n_bin_z,
                 float _bin_size_x, float _bin_size_y, float _bin_size_z) :
        n_bin_x{ _n_bin_x }, n_bin_y{ _n_bin_y }, n_bin_z{ _n_bin_z },
        bin_size_x{ _bin_size_x }, bin_size_y{ _bin_size_y }, bin_size_z{ _bin_size_z },
        data_ori{ std::make_unique<float[]>(_n_bin_x * _n_bin_y * _n_bin_z) },
        data_rot{ std::make_unique<float[]>(_n_bin_x * _n_bin_y * _n_bin_z) } {}
    ~VolumeObject() {}
    void Print(void) const;
    void Set(int _n_bin_x, int _n_bin_y, int _n_bin_z,
             float _bin_size_x, float _bin_size_y, float _bin_size_z);
    void ImportData(std::unique_ptr<float[]> & _data_ori);
    void Rotate(double _phi, double _theta, double _psi);
    int GetNBinX(void) const { return n_bin_x; }
    int GetNBinY(void) const { return n_bin_y; }
    int GetNBinZ(void) const { return n_bin_z; }
    int GetBinIndex(int _x, int _y, int _z) const { return _x + n_bin_x*(_y + n_bin_y*_z); }
    float GetBinSizeX(void) const { return bin_size_x; }
    float GetBinSizeY(void) const { return bin_size_y; }
    float GetBinSizeZ(void) const { return bin_size_z; }
    float GetEntry(int _x, int _y, int _z) const { return data_ori[GetBinIndex(_x, _y, _z)]; }
    float GetEntryRot(int _x, int _y, int _z) const { return data_rot[GetBinIndex(_x, _y, _z)]; }
    std::shared_ptr<TH3F> GetPicture(void) const { return picture; }
    std::shared_ptr<TH3F> GetPictureRot(void) const { return picture_rot; }

private:
    void MakePicture(void);
    void MakePictureRot(void);

};




#endif