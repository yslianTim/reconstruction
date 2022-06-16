#include <iostream>
#include <memory>
#include <vector>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "DataBlock.hpp"

class DataBlockTest : public ::testing::Test
{
protected:
    DataBlock block1, block2;
    std::vector<float> data_dummy;
    std::unique_ptr<float[]> data1, data2;
    int n_bin[3];
    float bin_size[3];
    
    void SetUp(void) override
    {
        n_bin[0] = 100;
        n_bin[1] = 100;
        n_bin[2] = 30;
        bin_size[0] = 1.8;
        bin_size[1] = 1.3;
        bin_size[2] = 0.5;
        data_dummy.reserve(n_bin[0] * n_bin[1] * n_bin[2]);
        data1 = std::make_unique<float[]>(n_bin[0] * n_bin[1] * n_bin[2]);
        data2 = std::make_unique<float[]>(n_bin[0] * n_bin[1] * n_bin[2]);
        for (int i = 0; i < n_bin[0] * n_bin[1] * n_bin[2]; ++i)
        {
            auto tmp = 0.01*i;
            data_dummy.push_back(tmp);
            data1[i] = tmp;
            data2[i] = tmp;
        }
        block1.Set(n_bin, bin_size, data1);
        block2.Set(n_bin, bin_size, data2);
    }

    void TearDown(void) override
    {

    }
};

TEST_F(DataBlockTest, TestSetStack)
{
    auto test_is_not_stack = block1.IsStack();
    EXPECT_FALSE(test_is_not_stack) << "Data block labeled as stack but it shouldn't.";

    block1.SetStack();
    auto test_is_stack = block1.IsStack();
    EXPECT_TRUE(test_is_stack) << "Data block labeled as not stack but it should.";
}

TEST_F(DataBlockTest, TestSet2D)
{
    auto test_is_not_2D = block1.Is2D();
    EXPECT_FALSE(test_is_not_2D) << "Data block labeled as 2D data but it shouldn't.";

    block1.Set2D();
    auto test_is_2D = block1.Is2D();
    EXPECT_TRUE(test_is_2D) << "Data block labeled as not 2D data but it should.";
}

TEST_F(DataBlockTest, TestSet3D)
{
    auto test_is_not_3D = block2.Is3D();
    EXPECT_FALSE(test_is_not_3D) << "Data block labeled as 3D data but it shouldn't.";

    block2.Set3D();
    auto test_is_3D = block2.Is3D();
    EXPECT_TRUE(test_is_3D) << "Data block labeled as not 3D data but it should.";
}

TEST_F(DataBlockTest, TestGetNBin)
{
    auto test_x = block1.GetNBinX();
    auto test_y = block1.GetNBinY();
    auto test_z = block1.GetNBinZ();

    EXPECT_EQ(n_bin[0], test_x) << "Data block's dimension in X is inconsistent.";
    EXPECT_EQ(n_bin[1], test_y) << "Data block's dimension in Y is inconsistent.";
    EXPECT_EQ(n_bin[2], test_z) << "Data block's dimension in Z is inconsistent.";
}

TEST_F(DataBlockTest, TestGetBinSize)
{
    auto test_size_x = block1.GetBinSizeX();
    auto test_size_y = block1.GetBinSizeY();
    auto test_size_z = block1.GetBinSizeZ();

    EXPECT_FLOAT_EQ(bin_size[0], test_size_x) << "Data block's bin size in X is inconsistent.";
    EXPECT_FLOAT_EQ(bin_size[1], test_size_y) << "Data block's bin size in Y is inconsistent.";
    EXPECT_FLOAT_EQ(bin_size[2], test_size_z) << "Data block's bin size in Z is inconsistent.";
}
