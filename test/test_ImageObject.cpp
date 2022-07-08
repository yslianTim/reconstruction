#include <iostream>
#include <memory>
#include <vector>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ImageObject.hpp"

class ImageObjectTest : public ::testing::Test
{
protected:
    ImageObject image1;
    ImageObject image2;
    std::vector<float> data_dummy;
    std::unique_ptr<float[]> data;
    int num_x, num_y, num_array;
    float size_x, size_y;
    
    void SetUp(void) override
    {
        num_x = 100;
        num_y = 100;
        size_x = 5.8;
        size_y = 9.3;
        num_array = num_x * num_y;
        data_dummy.reserve(num_array);
        data = std::make_unique<float[]>(num_array);
        for (int i = 0; i < num_array; ++i)
        {
            auto tmp = 0.1*i;
            data_dummy.push_back(tmp);
            data[i] = tmp;
        }
        image1.Set(num_x, num_y, size_x, size_y);
        image2.Set(num_x, num_y, size_x, size_y);
        image1.ImportData(data);
    }

    void TearDown(void) override
    {

    }
};

TEST_F(ImageObjectTest, TestGetNBin)
{
    auto test_x = image1.GetNBinX();
    auto test_y = image1.GetNBinY();

    EXPECT_EQ(num_x, test_x) << "Image's number of bin in X is inconsistent.";
    EXPECT_EQ(num_y, test_y) << "Image's number of bin in Y is inconsistent.";
}

TEST_F(ImageObjectTest, TestGetBinSize)
{
    auto test_size_x = image1.GetBinSizeX();
    auto test_size_y = image1.GetBinSizeY();

    EXPECT_FLOAT_EQ(size_x, test_size_x) << "Image's bin size in X is inconsistent.";
    EXPECT_FLOAT_EQ(size_y, test_size_y) << "Image's bin size in Y is inconsistent.";
}

TEST_F(ImageObjectTest, TestImportData)
{
    auto data_tmp = std::make_unique<float[]>(num_x * num_y);
    for (int i = 0; i < num_x * num_y; ++i)
    {
        data_tmp[i] = data_dummy.at(i);
    }
    image2.ImportData(data_tmp);
    EXPECT_EQ(data_tmp, nullptr) << "Image's data is remaining in memory.";
}

TEST_F(ImageObjectTest, TestGetEntry)
{
    for (int x = 0; x < num_x; ++x)
    {
        for (int y = 0; y < num_y; ++y)
        {
            auto index = x + num_x*y;
            auto test_entry = image1.GetEntry(x, y);
            EXPECT_FLOAT_EQ(data_dummy[index], test_entry)
                << "Image's entry: " << index << " is inconsistent.";
        }
    }
}

TEST_F(ImageObjectTest, TestGetPicture)
{
    auto picture = image1.GetPicture();

    EXPECT_NE(picture, nullptr) << "Image's picture is not been created.";

    for (int x = 0; x < num_x; ++x)
    {
        for (int y = 0; y < num_y; ++y)
        {
            auto index = x + num_x*y;
            auto test_entry = picture->GetBinContent(x, num_y-1-y);
            EXPECT_FLOAT_EQ(data_dummy[index], test_entry)
                << "Picture's entry: " << index << " is inconsistent.";
        }
    }
}

TEST_F(ImageObjectTest, TestRotate)
{
    image1.Rotate(0.25*M_PI);
    auto picture = image1.GetPictureRot();

    EXPECT_NE(picture, nullptr) << "Rotated Image's picture is not been created.";
}

TEST_F(ImageObjectTest, TestGetMinMaximum)
{
    auto minimum = *(std::min_element(data_dummy.begin(), data_dummy.end()));
    auto maximum = *(std::max_element(data_dummy.begin(), data_dummy.end()));
    auto test_minimum = image1.GetMinimum();
    auto test_maximum = image1.GetMaximum();

    EXPECT_FLOAT_EQ(minimum, test_minimum) << "Image's minimum entry is not correct.";
    EXPECT_FLOAT_EQ(maximum, test_maximum) << "Image's maximum entry is not correct.";
}