#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "CQuaternion.hpp"
#include "FileManager.hpp"
#include "ECP_Base.hpp"
#include "TCanvas.h"
#include "TH2.h"

TEST(TestSuiteSample, TestSample)
{
    // Arrange
    CQuaternion q0(1,3,4,5);
    CQuaternion qAns(1,-3,-4,-5);

    // Act
    CQuaternion qTest = q0.GetConjugate();

    // Assert
    ASSERT_EQ(qAns, qTest);
}

TEST(TestFileManager, TestImportFile)
{
    // Arrange
    FileManager::Instance().ImportFile("/home/yslian/Projects/mywork/data/particles_10.mrcs");
    //FileManager::Instance().ImportFile("/nas1/yslian/Data/mrcfile_one_particle/one_p_9950.mrc");
    //FileManager::Instance().ImportFile("/nas1/yslian/Data/mrcfile_one_particle/one_p_9953.mrc");
    //FileManager::Instance().ImportFile("/nas1/yslian/Data/mrcfile_one_particle/one_p_9966.mrc");
    
    // Act
    int nImportFile = FileManager::Instance().NumberOfImportFile();
    //FileManager::Instance().Print();

    // Assert
    ASSERT_EQ(1, nImportFile);
}

TEST(TestFileManager, TestImportFileList)
{
    // Arrange
    auto fileList = FileManager::Instance().GetImportFileList();
    
    // Act
    int nImportFile = fileList.size();

    // Assert
    ASSERT_EQ(1, nImportFile);
}

TEST(TestFileManager, TestSetDataManager)
{
    // Arrange
    auto fileList = FileManager::Instance().GetImportFileList();

    // Act
    for (auto & i : fileList)
    {
        i->AccessData();
        //i->Print();
        //i->GetDataBlockRef()->Print();
        
        //std::cout << i->GetDataBlockRef()->GetDataArrayRef()[0] << ", "
        //<< i->GetDataBlockRef()->GetDataArrayRef()[1] << std::endl;
        //i->GetDataBlockRef()->GetImageListRef()[0]->Print();

        //std::cout << i->GetDataBlockRef()->GetImageListRef().size() << std::endl;
    }

    // Assert
    ASSERT_EQ( 2, 2);
}
/*
TEST(TestFileManager, TestDrawImage)
{
    // Arrange
    auto fileList = FileManager::Instance().GetImportFileList();
    auto imageX = fileList.at(0)->GetDataBlockRef()->GetImageListRef().at(0)->GetNBinX();
    auto imageY = fileList.at(0)->GetDataBlockRef()->GetImageListRef().at(0)->GetNBinY();

    // Act
    auto cm = new TCanvas("cm","Canvas", 600, 600);
    auto h1 = new TH2D("h1","2D Image", 218, 0, 218, 218, 0, 218);
    for (int i = 0; i < imageX; ++i)
    {
        for (int j = 0; j < imageY; ++j)
        {
            auto entry = fileList.at(0)->GetDataBlockRef()->GetImageListRef().at(0)->GetEntry(i,j);
            //std::cout << entry << std::endl;
            h1->Fill(i,j,entry);
        }
    }
    cm->cd();
    //gPad->SetLogz();
    //h1->GetZaxis()->SetRangeUser(0.00001,1);
    h1->Draw("COLZ");
    cm->Print("image_test.png");

    // Assert
    ASSERT_EQ( 2, 2);
}
*/


int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}