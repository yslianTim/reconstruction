#ifndef IMAGEOBJECT_HPP_
#define IMAGEOBJECT_HPP_

#include <iostream>
#include <memory>
#include "TH2.h"

class ImageObject
{
    std::unique_ptr<float[]> data;
    std::shared_ptr<TH2F> picture;
    int n_bin_x, n_bin_y;
    float bin_size_x, bin_size_y;

public:
    ImageObject(void) {}
    ImageObject(int _n_bin_x, int _n_bin_y, float _bin_size_x, float _bin_size_y) :
        n_bin_x{ _n_bin_x }, n_bin_y{ _n_bin_y },
        bin_size_x{ _bin_size_x }, bin_size_y{ _bin_size_y },
        data{ std::make_unique<float[]>(_n_bin_x * _n_bin_y) } {}
    ~ImageObject() {}
    void Print(void) const;
    void Set(int _n_bin_x, int _n_bin_y, float _bin_size_x, float _bin_size_y);
    void ImportData(std::unique_ptr<float[]> & _data);
    int GetNBinX(void) const { return n_bin_x; }
    int GetNBinY(void) const { return n_bin_y; }
    float GetBinSizeX(void) const { return bin_size_x; }
    float GetBinSizeY(void) const { return bin_size_y; }
    float GetEntry(int _pixel_x, int _pixel_y) const { return data[_pixel_x + n_bin_x*_pixel_y]; }
    std::shared_ptr<TH2F> GetPicture(void) const { return picture; }

private:
    void MakePicture(void);
    
};




#endif