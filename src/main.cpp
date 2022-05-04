#include <cmath>
#include <iostream>
#include <fstream>
#include <experimental/filesystem>
#include <string>
#include <memory>
#include <vector>
#include "config.hpp"
#include "TH2.h"
#include "TH3.h"
#include "TCanvas.h"

#include "FileManager.hpp"
#include "DataManager.hpp"
#include "Vector3D.hpp"
#include "Matrix3D.hpp"
#include "CQuaternion.hpp"
#include "CEulerAngle.hpp"
#include "CRotation3D.hpp"
#include "ECP_Base.hpp"
#include "Sampling.hpp"


int main(int argc, char* argv[])
{
    if (argc < 2) {
        // report version
        std::cout << argv[0] << " Version " << MyProject_VERSION_MAJOR << "."
                << MyProject_VERSION_MINOR << std::endl;
        std::cout << "Usage: " << argv[0] << " number" << std::endl;
        return 1;
    }

    // convert input to double
    const double inputValue = std::stod(argv[1]);

    CQuaternion q0(1,3,4,5);
    CQuaternion q1(1,2,3,4);
    CQuaternion q2(1,2,3,4);


    q0.Normalize();
    q1.Normalize();
    q2.Normalize();

  
  
/*
    clock_t tStart0 = clock();

    clock_t tStart1 = clock();
    for(int i=0; i<1000; i++){
        XYZVector vec_new1(q0.Rotation(vec1));
    }
    clock_t tStart1_ = clock();
    std::cout <<" ---"<< (double)(tStart1_ - tStart1) <<" ms"<< std::endl;
    //std::cout << vec_new1.X() <<", "<< vec_new1.Y() <<", "<< vec_new1.Z() << std::endl;

    clock_t tStart2 = clock();
    for(int i=0; i<1000; i++){
        Vector3d vec_new2(q0.Rotation(vec2));
    }
    clock_t tStart2_ = clock();
    std::cout <<" ---"<< (double)(tStart2_ - tStart2) <<" ms"<< std::endl;
    //std::cout << vec_new2[0] <<", "<< vec_new2[1] <<", "<< vec_new2[2] << std::endl;

    q1.GetEuler();
*/

    /*
    CEulerAngle ang = CEulerAngle(1.2, 2.1, 0.7);
  
    CRotation3D rot0 = ang.GetRotMatrix();
    CQuaternion q00  = ang.GetQuaternion();
    CEulerAngle ang0 = rot0.GetEuler();

    clock_t tStart1 = clock();
    //for(int i=0; i<1000; i++){
        Matrix3d mtx1 = rot0.GetMatrix();
    //}
    clock_t tStart1_ = clock();
    std::cout <<" ---"<< (double)(tStart1_ - tStart1) <<" ms"<< std::endl;
    std::cout << mtx1 << std::endl;

    clock_t tStart2 = clock();
    //for(int i=0; i<1000; i++){
        CRotation3D rot2 = ang.GetRotMatrix();
        CQuaternion q02  = rot2.GetQuaternion();
        CEulerAngle ang2 = q02.GetEuler();
    //}
    clock_t tStart2_ = clock();
    std::cout <<" ---"<< (double)(tStart2_ - tStart2) <<" ms"<< std::endl;
    */
    /*
    CRotation3D rr(1,2,3,4,5,6,7,8,9);
    rr.Print();
    Vector3d vec1(1,2,3);
    Vector3d vec2;
    vec2 = rr*vec1;
    std::cout<<rr.GetMatrix()*vec1<<std::endl;
    std::cout<<vec2<<std::endl;
    */
/*
    // Example of reading a hdf5 file
    clock_t tStart3 = clock();
    const H5std_string FILE_NAME("/home/yslian/Projects/mywork/MyProject/data/5J0N_uniformS2.h5");
    const H5std_string DATASET_NAME("Projections");
    H5File file(FILE_NAME, H5F_ACC_RDONLY);
    DataSet dataset = file.openDataSet( DATASET_NAME );
    DataSpace dataspace = dataset.getSpace();
    const int rank = dataspace.getSimpleExtentNdims();
    hsize_t dims[rank];
    const int ndims = dataspace.getSimpleExtentDims( dims, NULL);
    const int Nx = dims[0];
    const int Ny = dims[1];
    const int Nz = dims[2];
    std::cout <<"Rank = "<< rank << std::endl;
    std::cout <<"Dimensions = "<< Nx <<" x "<< Ny <<" x "<< Nz << std::endl;
    DataSpace memspace( rank, dims );
    auto data_out = std::make_unique<double[]>(Nx*Ny*Nz);
    dataset.read(data_out.get(), PredType::NATIVE_DOUBLE, memspace, dataspace);
    auto cm = new TCanvas("cm","Canvas", 600, 600);
    auto h1 = new TH2D("h1","2D Image", 218, 0, 218, 218, 0, 218);
    for (int i=0 ;i<Nx; ++i) {
        if(i!=0) break;
        for (int j=0 ;j<Ny; ++j) {
            for (int k=0 ;k<Nz; ++k) {
                h1->Fill(j,k,data_out[ k + Ny*(j + Nx*i) ]);
            }
        }
    }
    cm->cd();
    gPad->SetLogz();
    h1->GetZaxis()->SetRangeUser(0.01,100);
    h1->Draw("COLZ");
    cm->Print("image.pdf");
    clock_t tStart3_ = clock();
    std::cout <<" ---"<< (double)(tStart3_ - tStart3)/CLOCKS_PER_SEC <<" sec"<< std::endl;
*/
    // Example of reading a mrc file
    clock_t tStart4 = clock();
    Sampling sampling_new;
    sampling_new.Set(3);
    sampling_new.Print();
    std::vector<int> listSample;
    double radius_new = 0.2;
    pointing tmp_new(0.5,4.0);
    sampling_new.query_disc(tmp_new, radius_new, listSample);

    ECP_Base sampling;
    sampling.Set(3);
    std::vector<int> listGrid;
    double radius = 0.2;
    pointing tmp(0.5,4.0);
    sampling.query_disc(tmp, radius, listGrid);

    auto Nx = 100;
    auto Ny = 100;
    auto Nz = 100;
    auto cm = new TCanvas("cm","Canvas", 600, 600);
    auto h1 = new TH3D("h1","3D Model", Nx, -1.2, 1.2, Ny, -1.2, 1.2, Nz, -1.2, 1.2);
    
    auto quat_sample = sampling_new.GetSamplingQuat();
    for (auto & i : quat_sample)
    {
        auto tmpX = i.GetVector().X();
        auto tmpY = i.GetVector().Y();
        auto tmpZ = i.GetVector().Z();
        h1->Fill(tmpX, tmpY, tmpZ);
    }
    for (auto & in : listSample)
    {
        auto tmpX = quat_sample.at(in).GetVector().X();
        auto tmpY = quat_sample.at(in).GetVector().Y();
        auto tmpZ = quat_sample.at(in).GetVector().Z();
        std::cout << in << std::endl;
        //h1->Fill(tmpX, tmpY, tmpZ, 100);
    }
    auto tmpX = sin(tmp_new.theta)*cos(tmp_new.phi);
    auto tmpY = sin(tmp_new.theta)*sin(tmp_new.phi);
    auto tmpZ = cos(tmp_new.theta);
    //h1->Fill(tmpX, tmpY, tmpZ, 1000);
    
/*
    for (int i=0; i<sampling.NumberOfGrid(); ++i)
    {
        auto tmpX = sampling.GridToX(i);
        auto tmpY = sampling.GridToY(i);
        auto tmpZ = sampling.GridToZ(i);
        h1->Fill(tmpX, tmpY, tmpZ);
    }
    for (auto & in : listGrid)
    {
        auto tmpX = sampling.GridToX(in);
        auto tmpY = sampling.GridToY(in);
        auto tmpZ = sampling.GridToZ(in);
        //std::cout << in << std::endl;
        //h1->Fill(tmpX, tmpY, tmpZ, 100);
    }
    auto tmpX = sin(tmp.theta)*cos(tmp.phi);
    auto tmpY = sin(tmp.theta)*sin(tmp.phi);
    auto tmpZ = cos(tmp.theta);
    //h1->Fill(tmpX, tmpY, tmpZ, 1000);
    */
    cm->cd();
    h1->Draw("LEGO");
    cm->Print("image_quat.png");
    cm->Close();
    delete h1;
    delete cm;

/*
    auto Nx = 142;
    auto Ny = 103;
    auto Nz = 130;
    auto cm = new TCanvas("cm","Canvas", 600, 600);
    auto h1 = new TH3D("h1","3D Model", Nx, 0, Nx, Ny, 0, Ny, Nz, 0, Nz);
    
    
    for (int i=0 ;i<Nx; ++i) {
        for (int j=0 ;j<Ny; ++j) {
            for (int k=0 ;k<Nz; ++k) {
                //std::cout<<mrcfile_in.model[ k + Ny*(j + Nx*i) ]<<std::endl;
                //h1->Fill(i,j,k,model[ i + Nx*(j + Ny*k) ]);
            }
        }
    }
    cm->cd();
    h1->Draw("ISO");
    cm->Print("image.png");
    cm->Close();
    delete h1;
    delete cm;

    Vector3D vec(1,2,3);
    Vector3D vec2(4,5,6);
    Vector3D vec3 = vec2;
    std::cout<<vec3.X()<<" "<<vec3.Y()<<" "<<vec3.Z()<<std::endl;

    Matrix3D matrix();
*/

    clock_t tStart4_ = clock();
    std::cout <<" ---"<< (double)(tStart4_ - tStart4)/CLOCKS_PER_SEC <<" sec"<< std::endl;
    

    return 0;
}