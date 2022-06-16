#ifndef DATABLOCK_HPP_
#define DATABLOCK_HPP_

#include <iostream>
#include <memory>
#include <vector>
#include "ImageObject.hpp"
#include "VolumeObject.hpp"

class DataBlock
{
    bool f_is_stack, f_is_2D, f_is_3D;
    int n_bin[3];
    float bin_size[3];
    std::unique_ptr<float[]> data_array;
    std::vector<std::unique_ptr<ImageObject>> image_list;
    std::vector<std::unique_ptr<VolumeObject>> volume_list;

public:
    DataBlock(void) :
        f_is_stack{ false }, f_is_2D{ false }, f_is_3D{ false } {}
    DataBlock(int * _n_bin, float * _bin_size) :
        f_is_stack{ false }, f_is_2D{ false }, f_is_3D{ false },
        n_bin{ _n_bin[0], _n_bin[1], _n_bin[2] },
        bin_size{ _bin_size[0], _bin_size[1], _bin_size[2] },
        data_array{ std::make_unique<float[]>(_n_bin[0] * _n_bin[1] * _n_bin[2]) } {}
    ~DataBlock() {}
    void Print(void) const;
    void Set(int * _n_bin, float * _bin_size, std::unique_ptr<float[]> & _data_array);
    void SetDataArray(std::unique_ptr<float[]> & _data_array);
    void SetStack(void);
    void Set2D(void);
    void Set3D(void);
    bool IsStack(void) const { return f_is_stack; }
    bool Is2D(void) const { return f_is_2D; }
    bool Is3D(void) const { return f_is_3D; }
    int GetNBinX(void) const { return n_bin[0]; }
    int GetNBinY(void) const { return n_bin[1]; }
    int GetNBinZ(void) const { return n_bin[2]; }
    float GetBinSizeX(void) const { return bin_size[0]; }
    float GetBinSizeY(void) const { return bin_size[1]; }
    float GetBinSizeZ(void) const { return bin_size[2]; }
    std::unique_ptr<float[]> const& GetDataArrayRef(void) const { return data_array; }
    std::vector<std::unique_ptr<ImageObject>> const& GetImageListRef(void) const { return image_list; }
    std::vector<std::unique_ptr<VolumeObject>> const& GetVolumeListRef(void) const { return volume_list; }

private:
    void Sort2D(void);
    void Sort3D(void);
};


#endif