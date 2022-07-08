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
    std::unique_ptr<float[]> data, data_ext, data_rot;
    std::shared_ptr<TH3F> picture, picture_ext, picture_rot;
    int n_bin_x, n_bin_y, n_bin_z, n_bin_ext;
    int n_array, n_array_ext;
    float bin_size_x, bin_size_y, bin_size_z;
    bool is_extend;

public:
    VolumeObject(void) {}
    VolumeObject(int _nx, int _ny, int _nz, float _sx, float _sy, float _sz) :
        n_bin_x{ _nx }, n_bin_y{ _ny }, n_bin_z{ _nz }, n_array{ _nx * _ny * _nz },
        bin_size_x{ _sx }, bin_size_y{ _sy }, bin_size_z{ _sz }, is_extend{ false }
    {
        n_bin_ext = static_cast<int>(ceil(sqrt(_nx*_nx + _ny*_ny + _nz*_nz)));
        n_array_ext = n_bin_ext * n_bin_ext * n_bin_ext;
    }
    ~VolumeObject() {}
    void Print(void) const;
    void Set(int _nx, int _ny, int _nz, float _sx, float _sy, float _sz);
    void ImportData(std::unique_ptr<float[]> & _data);
    void Extend(void);
    void Rotate(double _phi, double _theta, double _psi);
    bool IsExtend(void) const { return is_extend; }
    int GetNBinX(void) const { return n_bin_x; }
    int GetNBinY(void) const { return n_bin_y; }
    int GetNBinZ(void) const { return n_bin_z; }
    int GetNBinExt(void) const { return n_bin_ext; }
    int GetArraySize(void) const { return n_array; }
    int GetArraySizeExt(void) const { return n_array_ext; }
    int GetBinIndex(int _x, int _y, int _z) const { return _x + n_bin_x*(_y + n_bin_y*_z); }
    int GetBinIndexExt(int _x, int _y, int _z) const { return _x + n_bin_ext*(_y + n_bin_ext*_z); }
    float GetBinSizeX(void) const { return bin_size_x; }
    float GetBinSizeY(void) const { return bin_size_y; }
    float GetBinSizeZ(void) const { return bin_size_z; }
    float GetEntry(int _i) const { return data[_i]; }
    float GetEntry(int _x, int _y, int _z) const { return data[GetBinIndex(_x, _y, _z)]; }
    float GetEntryExt(int _i) const { return data_ext[_i]; }
    float GetEntryExt(int _x, int _y, int _z) const { return data_ext[GetBinIndexExt(_x, _y, _z)]; }
    float GetEntryRot(int _i) const { return data_rot[_i]; }
    float GetEntryRot(int _x, int _y, int _z) const { return data_rot[GetBinIndexExt(_x, _y, _z)]; }
    std::shared_ptr<TH3F> GetPicture(void) const { return picture; }
    std::shared_ptr<TH3F> GetPictureExt(void) const { return picture_ext; }
    std::shared_ptr<TH3F> GetPictureRot(void) const { return picture_rot; }

private:
    void MakeDataArray(void);
    void MakeDataArrayExt(void);
    void MakeDataArrayRot(void);
    void MakePicture(void);
    void MakePictureExt(void);
    void MakePictureRot(void);

};




#endif