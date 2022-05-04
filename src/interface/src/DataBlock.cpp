#include "DataBlock.hpp"

void DataBlock::Print(void) const
{
    std::cout << "Array binning: "
              << n_bin[0] << ", " << n_bin[1] << ", " << n_bin[2]
              << std::endl;
    std::cout << "Is stack data? " << std::boolalpha << f_is_stack << std::endl;
    std::cout << "Is 2D array? " << std::boolalpha << f_is_2D << std::endl;
    std::cout << "Is 3D array? " << std::boolalpha << f_is_3D << std::endl;
    std::cout << "Data array: [" << std::endl;
    std::cout << data_array[0] << ", " << data_array[1] << std::endl;
    std::cout << "]" << std::endl;
}

void DataBlock::Set(int * _n_bin, float * _bin_size)
{
    n_bin[0] = _n_bin[0];
    n_bin[1] = _n_bin[1];
    n_bin[2] = _n_bin[2];
    bin_size[0] = _bin_size[0];
    bin_size[1] = _bin_size[1];
    bin_size[2] = _bin_size[2];
}

void DataBlock::SetDataArray(std::unique_ptr<float[]> & _data_array)
{
    data_array = std::move(_data_array);
    _data_array.reset(nullptr);
}

void DataBlock::SetStack(void)
{
    f_is_stack = true;
}

void DataBlock::Set2D(void)
{
    if (f_is_3D)
    {
        std::cout << "[WARNING] Data block has been set as 3D! Skip Set2D()..." << std::endl;
        return;
    }
    f_is_2D = true;
    Sort2D();
}

void DataBlock::Sort2D(void)
{
    if (data_array == nullptr)
    {
        std::cout << "[WARNING] data_array is empty! Skip sorting data..." << std::endl;
        return;
    }
    image_list.reserve(n_bin[2]);
    for (int k = 0 ; k < n_bin[2]; ++k)
    {
        auto image_obj = std::make_unique<ImageObject>(n_bin[0], n_bin[1], bin_size[0], bin_size[1]);
        auto data_tmp = std::make_unique<float[]>(n_bin[0] * n_bin[1]);
        for (int i = 0 ; i < n_bin[0]; ++i)
        {
            for (int j = 0 ; j < n_bin[1]; ++j)
            {
                data_tmp[i + n_bin[0]*j] = data_array[i + n_bin[0]*(j + n_bin[1]*k)];
            }
        }
        image_obj->ImportData(data_tmp);
        image_list.push_back(std::move(image_obj));
        data_tmp.reset(nullptr);
        image_obj.reset(nullptr);
    }
}

void DataBlock::Set3D(void)
{
    if (f_is_2D)
    {
        std::cout << "[WARNING] Data block has been set as 2D! Skip Set3D()..." << std::endl;
        return;
    }
    f_is_3D = true;
    Sort3D();
}

void DataBlock::Sort3D(void)
{
    if (data_array == nullptr)
    {
        std::cout << "[WARNING] data_array is empty! Skip sorting data..." << std::endl;
        return;
    }
    
}