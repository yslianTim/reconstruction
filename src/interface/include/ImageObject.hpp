#ifndef IMAGEOBJECT_HPP_
#define IMAGEOBJECT_HPP_

#include <iostream>
#include <iomanip>
#include <memory>
#include <cmath>
#include "Math/SVector.h"
#include "Math/SMatrix.h"
#include "TH2.h"

typedef ROOT::Math::SVector<double,2> SVector2d;
typedef ROOT::Math::SMatrix<double,2> SMatrix2d;

class ImageObject
{
    std::unique_ptr<float[]> data_ori, data_rot;
    std::shared_ptr<TH2F> picture, picture_rot;
    int n_bin_x, n_bin_y;
    float bin_size_x, bin_size_y;

public:
    ImageObject(void) {}
    ImageObject(int _n_bin_x, int _n_bin_y, float _bin_size_x, float _bin_size_y) :
        n_bin_x{ _n_bin_x }, n_bin_y{ _n_bin_y },
        bin_size_x{ _bin_size_x }, bin_size_y{ _bin_size_y },
        data_ori{ std::make_unique<float[]>(_n_bin_x * _n_bin_y) },
        data_rot{ std::make_unique<float[]>(_n_bin_x * _n_bin_y) } {}
    ~ImageObject() {}
    void Print(void) const;
    void Set(int _n_bin_x, int _n_bin_y, float _bin_size_x, float _bin_size_y);
    void ImportData(std::unique_ptr<float[]> & _data);
    void Rotate(double _angle);
    int GetNBinX(void) const { return n_bin_x; }
    int GetNBinY(void) const { return n_bin_y; }
    int GetBinIndex(int _x, int _y) const { return _x + n_bin_x*_y; }
    float GetBinSizeX(void) const { return bin_size_x; }
    float GetBinSizeY(void) const { return bin_size_y; }
    float GetEntry(int _x, int _y) const { return data_ori[GetBinIndex(_x, _y)]; }
    float GetEntryRot(int _x, int _y) const { return data_rot[GetBinIndex(_x, _y)]; }
    std::shared_ptr<TH2F> GetPicture(void) const { return picture; }
    std::shared_ptr<TH2F> GetPictureRot(void) const { return picture_rot; }

private:
    void MakePicture(void);
    void MakePictureRot(void);
    
};




#endif