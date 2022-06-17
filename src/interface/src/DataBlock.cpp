#include "DataBlock.hpp"

void DataBlock::Print(void) const
{
    using namespace std;
    cout << " o=======================================================o " << endl;
    cout << " |  Data Block |    Column   |     Row     |   Section   | " << endl;
    cout << " o=======================================================o " << endl;
    cout << " |  Dimension  |";
    cout << right << setw(12) << n_bin[0] << " |"
         << right << setw(12) << n_bin[1] << " |"
         << right << setw(12) << n_bin[2] << " |" << endl;
    cout << " |  Resolution |";
    cout << right << setw(12) << bin_size[0] << " |"
         << right << setw(12) << bin_size[1] << " |"
         << right << setw(12) << bin_size[2] << " |" << endl;
    cout << " |=======================================================| " << endl;
    cout << " | Is Stack Data? | " << left << setw(36) << boolalpha << f_is_stack << " |" << endl;
    cout << " | Is 2D Array?   | " << left << setw(36) << boolalpha << f_is_2D    << " |" << endl;
    cout << " | Is 3D Array?   | " << left << setw(36) << boolalpha << f_is_3D    << " |" << endl;
    cout << " o=======================================================o " << endl;
}

void DataBlock::Set(int * _n_bin, float * _bin_size, std::unique_ptr<float[]> & _data_array)
{
    n_bin[0] = _n_bin[0];
    n_bin[1] = _n_bin[1];
    n_bin[2] = _n_bin[2];
    bin_size[0] = _bin_size[0];
    bin_size[1] = _bin_size[1];
    bin_size[2] = _bin_size[2];
    SetDataArray(_data_array);
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
        for (int j = 0 ; j < n_bin[1]; ++j)
        {
            for (int i = 0 ; i < n_bin[0]; ++i)
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
    volume_list.reserve(1);
    auto volume_obj = std::make_unique<VolumeObject>(n_bin[0], n_bin[1], n_bin[2],
                                                     bin_size[0], bin_size[1], bin_size[2]);
    auto data_tmp = std::make_unique<float[]>(n_bin[0] * n_bin[1] * n_bin[2]);
    for (int k = 0 ; k < n_bin[2]; ++k)
    {
        for (int j = 0 ; j < n_bin[1]; ++j)
        {
            for (int i = 0 ; i < n_bin[0]; ++i)
            {
                data_tmp[i + n_bin[0]*(j + n_bin[1]*k)] = data_array[i + n_bin[0]*(j + n_bin[1]*k)];
            }
        }
    }
    volume_obj->ImportData(data_tmp);
    volume_list.push_back(std::move(volume_obj));
    data_tmp.reset(nullptr);
    volume_obj.reset(nullptr);
}