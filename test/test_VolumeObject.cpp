#include <iostream>
#include <memory>
#include <vector>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "VolumeObject.hpp"

class VolumeObjectTest : public ::testing::Test
{
protected:
    VolumeObject volume1;
    VolumeObject volume2;
    std::vector<float> data_dummy;
    std::unique_ptr<float[]> data;
    int num_x, num_y, num_z, num_ext, num_array, num_array_ext;
    float size_x, size_y, size_z;
    
    void SetUp(void) override
    {
        num_x = 50;
        num_y = 50;
        num_z = 50;
        size_x = 5.8;
        size_y = 5.3;
        size_z = 9.7;
        num_array = num_x * num_y * num_z;
        num_ext = static_cast<int>(ceil(sqrt(num_x*num_x + num_y*num_y + num_z*num_z)));
        num_array_ext = num_ext * num_ext * num_ext;
        data_dummy.reserve(num_array);
        data = std::make_unique<float[]>(num_array);
        for (int i = 0; i < num_array; ++i)
        {
            auto tmp = 0.1*i;
            data_dummy.push_back(tmp);
            data[i] = tmp;
        }
        volume1.Set(num_x, num_y, num_z, size_x, size_y, size_z);
        volume2.Set(num_x, num_y, num_z, size_x, size_y, size_z);
        volume1.ImportData(data);
    }

    void TearDown(void) override
    {

    }
};

TEST_F(VolumeObjectTest, TestGetNBin)
{
    auto test_x = volume1.GetNBinX();
    auto test_y = volume1.GetNBinY();
    auto test_z = volume1.GetNBinZ();
    auto test_ext = volume1.GetNBinExt();
    auto test_array = volume1.GetArraySize();
    auto test_array_ext = volume1.GetArraySizeExt();

    EXPECT_EQ(num_x, test_x) <<"Volume's number of bin in X is inconsistent.";
    EXPECT_EQ(num_y, test_y) <<"Volume's number of bin in Y is inconsistent.";
    EXPECT_EQ(num_z, test_z) <<"Volume's number of bin in Z is inconsistent.";
    EXPECT_EQ(num_ext, test_ext) <<"Volume's number of extend bin is inconsistent.";
    EXPECT_EQ(num_array, test_array) <<"Volume's data array size is inconsistent.";
    EXPECT_EQ(num_array_ext, test_array_ext) <<"Volume's entend data array size is inconsistent.";
}

TEST_F(VolumeObjectTest, TestGetBinIndex)
{
    for (int x = 0; x < num_x; ++x)
    {
        for (int y = 0; y < num_y; ++y)
        {
            for (int z = 0; z < num_z; ++z)
            {
                auto index = x + num_x*(y + num_y*z);
                auto test_index = volume1.GetBinIndex(x, y, z);
                EXPECT_FLOAT_EQ(index, test_index)
                    <<"Volume's index: "<< index <<" is inconsistent.";
            }
        }
    }
}

TEST_F(VolumeObjectTest, TestGetBinIndexExt)
{
    for (int x = 0; x < num_ext; ++x)
    {
        for (int y = 0; y < num_ext; ++y)
        {
            for (int z = 0; z < num_ext; ++z)
            {
                auto index = x + num_ext*(y + num_ext*z);
                auto test_index = volume1.GetBinIndexExt(x, y, z);
                EXPECT_FLOAT_EQ(index, test_index)
                    <<"Extended volume's index: "<< index <<" is inconsistent.";
            }
        }
    }
}

TEST_F(VolumeObjectTest, TestGetBinSize)
{
    auto test_size_x = volume1.GetBinSizeX();
    auto test_size_y = volume1.GetBinSizeY();
    auto test_size_z = volume1.GetBinSizeZ();

    EXPECT_FLOAT_EQ(size_x, test_size_x) <<"Volume's bin size in X is inconsistent.";
    EXPECT_FLOAT_EQ(size_y, test_size_y) <<"Volume's bin size in Y is inconsistent.";
    EXPECT_FLOAT_EQ(size_z, test_size_z) <<"Volume's bin size in Z is inconsistent.";
}

TEST_F(VolumeObjectTest, TestImportData)
{
    auto data_tmp = std::make_unique<float[]>(num_x * num_y * num_z);
    for (int i = 0; i < num_x * num_y * num_z; ++i)
    {
        data_tmp[i] = data_dummy.at(i);
    }
    volume2.ImportData(data_tmp);
    EXPECT_EQ(data_tmp, nullptr) <<"Volume's data is remaining in memory.";
}

TEST_F(VolumeObjectTest, TestGetEntry1)
{
    for (int x = 0; x < num_x; ++x)
    {
        for (int y = 0; y < num_y; ++y)
        {
            for (int z = 0; z < num_z; ++z)
            {
                auto index = volume1.GetBinIndex(x, y, z);
                auto test_entry = volume1.GetEntry(x, y, z);
                EXPECT_FLOAT_EQ(data_dummy[index], test_entry)
                    <<"Volume's entry: "<< index <<" is inconsistent.";
            }
        }
    }
}

TEST_F(VolumeObjectTest, TestGetEntry2)
{
    for (int i = 0; i < num_array; ++i)
    {
        auto test_entry = volume1.GetEntry(i);
        EXPECT_FLOAT_EQ(data_dummy[i], test_entry) <<"Volume's entry: "<< i <<" is inconsistent.";
    }
}

TEST_F(VolumeObjectTest, TestGetPicture)
{
    auto picture = volume1.GetPicture();

    for (int x = 0; x < num_x; ++x)
    {
        for (int y = 0; y < num_y; ++y)
        {
            for (int z = 0; z < num_z; ++z)
            {
                auto index = volume1.GetBinIndex(x, y, z);
                auto test_entry = picture->GetBinContent(x, num_y-1-y, num_z-1-z);
                EXPECT_FLOAT_EQ(data_dummy[index], test_entry)
                    <<"Picture's entry: "<< index <<" is inconsistent.";
            }
        }
    }

    EXPECT_NE(picture, nullptr) <<"Volume's picture is not been created.";
}