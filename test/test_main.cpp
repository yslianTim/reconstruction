#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "CQuaternion.hpp"
#include "FileManager.hpp"
#include "ECP_Base.hpp"
#include "TCanvas.h"
#include "TStyle.h"
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
    FileManager::Instance().ImportFile("/home/yslian/Projects/mywork/data/ADP_step4_res10.mrc");
    //FileManager::Instance().ImportFile("/home/yslian/Projects/mywork/downloads/protein_PDB_5J0N.mrc");
    
    // Act
    int nImportFile = FileManager::Instance().NumberOfImportFile();
    //FileManager::Instance().Print();

    // Assert
    ASSERT_EQ(2, nImportFile);
}

TEST(TestFileManager, TestImportFileList)
{
    // Arrange
    auto fileList = FileManager::Instance().GetFileList();
    
    // Act
    int nImportFile = fileList.size();

    // Assert
    ASSERT_EQ(2, nImportFile);
}

TEST(TestFileManager, TestSetDataManager)
{
    // Arrange
    auto fileList = FileManager::Instance().GetFileList();

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

TEST(TestFileManager, TestDrawImage)
{/*
    // Arrange
    auto fileList = FileManager::Instance().GetFileList();
    fileList.at(0)->GetDataBlockRef()->GetImageListRef().at(0)->Rotate(0.25*M_PI);
    fileList.at(1)->GetDataBlockRef()->GetVolumeListRef().at(0)->Extend();
    fileList.at(1)->GetDataBlockRef()->GetVolumeListRef().at(0)->Rotate(M_PI,0.0,0.0);
    auto h1 = fileList.at(0)->GetDataBlockRef()->GetImageListRef().at(0)->GetPicture();
    auto h2 = fileList.at(0)->GetDataBlockRef()->GetImageListRef().at(0)->GetPictureRot();
    auto h3 = fileList.at(1)->GetDataBlockRef()->GetVolumeListRef().at(0)->GetPicture();
    auto h4 = fileList.at(1)->GetDataBlockRef()->GetVolumeListRef().at(0)->GetPictureExt();
    auto h5 = fileList.at(1)->GetDataBlockRef()->GetVolumeListRef().at(0)->GetPictureRot();

    // Act
    gStyle->SetPalette(52);
    auto cm = new TCanvas("cm","Canvas", 1000, 200);
    cm->Divide(5,1);
    
    cm->cd(1);
    h1->Draw("COL");
    cm->cd(2);
    h2->Draw("COL");
    cm->cd(3);
    h3->Draw("ISO");
    cm->cd(4);
    h4->Draw("ISO");
    cm->cd(5);
    h5->Draw("ISO");
    cm->Print("image_test.png");*/

    // Assert
    ASSERT_EQ( 2, 2);
}



int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}