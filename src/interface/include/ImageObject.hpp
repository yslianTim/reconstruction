#ifndef IMAGEOBJECT_HPP_
#define IMAGEOBJECT_HPP_

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <memory>
#include <vector>
#include <cmath>
#include <QDebug>
#include <Math/SVector.h>
#include <Math/SMatrix.h>
#include <TH2.h>

typedef ROOT::Math::SVector<double,2> SVector2d;
typedef ROOT::Math::SMatrix<double,2> SMatrix2d;

class ImageObject
{
    std::unique_ptr<float[]> data, data_rot;
    std::shared_ptr<TH2F> picture, picture_rot;
    int n_bin_x, n_bin_y;
    int n_array;
    float bin_size_x, bin_size_y;

public:
    ImageObject(void) {}
    ImageObject(int _nx, int _ny, float _sx, float _sy) :
        n_bin_x{ _nx }, n_bin_y{ _ny }, n_array{ _nx*_ny }, bin_size_x{ _sx }, bin_size_y{ _sy } {}
    ~ImageObject() {}
    void Print(void) const;
    void Set(int _nx, int _ny, float _sx, float _sy);
    void ImportData(std::unique_ptr<float[]> & _data);
    void Rotate(double _angle);
    void Normalize(void);
    int GetNBinX(void) const { return n_bin_x; }
    int GetNBinY(void) const { return n_bin_y; }
    int GetArraySize(void) const { return n_array; }
    int GetBinIndex(int _x, int _y) const { return _x + n_bin_x*_y; }
    float GetBinSizeX(void) const { return bin_size_x; }
    float GetBinSizeY(void) const { return bin_size_y; }
    float GetEntry(int _x, int _y) const { return data[GetBinIndex(_x, _y)]; }
    float GetEntryRot(int _x, int _y) const { return data_rot[GetBinIndex(_x, _y)]; }
    float GetMinimum(void) const { return *(std::min_element(&data[0], &data[0]+n_array)); }
    float GetMaximum(void) const { return *(std::max_element(&data[0], &data[0]+n_array)); }
    std::shared_ptr<TH2F> GetPicture(void) const { return picture; }
    std::shared_ptr<TH2F> GetPictureRot(void) const { return picture_rot; }

private:
    void MakeDataArray(void);
    void MakeDataArrayRot(void);
    void MakePicture(void);
    void MakePictureRot(void);
    
};




#endif