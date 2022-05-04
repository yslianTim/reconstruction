#include "ImageObject.hpp"

void ImageObject::Print(void) const
{
    std::cout << "Image's dimension = " << n_bin_x << " x " << n_bin_y << std::endl;
    std::cout << "Image's pixel size = " << bin_size_x << " x " << bin_size_y << std::endl;
}

void ImageObject::Set(int _n_bin_x, int _n_bin_y, float _bin_size_x, float _bin_size_y)
{
    n_bin_x = _n_bin_x;
    n_bin_y = _n_bin_y;
    bin_size_x = _bin_size_x;
    bin_size_y = _bin_size_y;
}

void ImageObject::ImportData(std::unique_ptr<float[]> & _data)
{
    data = std::move(_data);
    _data.reset(nullptr);
}
