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
    int num_x, num_y, num_z;
    float size_x, size_y, size_z;
    
    void SetUp(void) override
    {
        num_x = 5;
        num_y = 6;
        num_z = 7;
        size_x = 5.8;
        size_y = 95.3;
        size_z = 9.7;
        data_dummy.reserve(num_x * num_y * num_z);
        data = std::make_unique<float[]>(num_x * num_y * num_z);
        for (int i = 0; i < num_x * num_y * num_z; ++i)
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

    EXPECT_EQ(num_x, test_x) << "Image's number of bin in X is inconsistent.";
    EXPECT_EQ(num_y, test_y) << "Image's number of bin in Y is inconsistent.";
    EXPECT_EQ(num_z, test_z) << "Image's number of bin in Z is inconsistent.";
}

TEST_F(VolumeObjectTest, TestGetBinSize)
{
    auto test_size_x = volume1.GetBinSizeX();
    auto test_size_y = volume1.GetBinSizeY();
    auto test_size_z = volume1.GetBinSizeZ();

    EXPECT_FLOAT_EQ(size_x, test_size_x) << "Image's bin size in X is inconsistent.";
    EXPECT_FLOAT_EQ(size_y, test_size_y) << "Image's bin size in Y is inconsistent.";
    EXPECT_FLOAT_EQ(size_z, test_size_z) << "Image's bin size in Z is inconsistent.";
}

TEST_F(VolumeObjectTest, TestImportData)
{
    auto data_tmp = std::make_unique<float[]>(num_x * num_y * num_z);
    for (int i = 0; i < num_x * num_y * num_z; ++i)
    {
        data_tmp[i] = data_dummy.at(i);
    }
    volume2.ImportData(data_tmp);
    EXPECT_EQ(data_tmp, nullptr) << "Image's data is remaining in memory.";
}

TEST_F(VolumeObjectTest, TestGetEntry)
{
    for (int x = 0; x < num_x; ++x)
    {
        for (int y = 0; y < num_y; ++y)
        {
            for (int z = 0; z < num_z; ++z)
            {
                auto index = x + num_x*(y + num_y*z);
                auto test_entry = volume1.GetEntry(x, y, z);
                EXPECT_FLOAT_EQ(data_dummy[index], test_entry)
                    << "Image's entry: " << index << " is inconsistent.";
            }
        }
    }
}

TEST_F(VolumeObjectTest, TestGetPicture)
{
    auto picture = volume1.GetPicture();
    volume1.Print();

    for (int x = 0; x < num_x; ++x)
    {
        for (int y = 0; y < num_y; ++y)
        {
            for (int z = 0; z < num_z; ++z)
            {
                auto index = x + num_x*(y + num_y*z);
                auto test_entry = picture->GetBinContent(x, num_y-1-y, num_z-1-z);
                EXPECT_FLOAT_EQ(data_dummy[index], test_entry)
                    << "Picture's entry: " << index << " is inconsistent.";
            }
        }
    }

    EXPECT_NE(picture, nullptr) << "Image's picture is not been created.";
}