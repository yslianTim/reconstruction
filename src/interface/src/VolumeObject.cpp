#include "VolumeObject.hpp"

void VolumeObject::Print(void) const
{
    using namespace std;
    cout << " o=======================================================o " << endl;
    cout << " | Volume Obj. |    Column   |     Row     |   Section   | " << endl;
    cout << " o=======================================================o " << endl;
    cout << " |  Dimension  |";
    cout << setw(12) << n_bin_x << " |"
         << setw(12) << n_bin_y << " |"
         << setw(12) << n_bin_z << " |" << endl;
    cout << " |  Resolution |";
    cout << setw(12) << bin_size_x << " |"
         << setw(12) << bin_size_y << " |"
         << setw(12) << bin_size_z << " |" << endl;
    cout << " o=======================================================o " << endl;
}

void VolumeObject::Set(int _n_bin_x, int _n_bin_y, int _n_bin_z,
                       float _bin_size_x, float _bin_size_y, float _bin_size_z)
{
    n_bin_x = _n_bin_x;
    n_bin_y = _n_bin_y;
    n_bin_z = _n_bin_z;
    bin_size_x = _bin_size_x;
    bin_size_y = _bin_size_y;
    bin_size_z = _bin_size_z;
}

void VolumeObject::ImportData(std::unique_ptr<float[]> & _data_ori)
{
    data_ori = std::move(_data_ori);
    _data_ori.reset(nullptr);
    MakePicture();
}

void VolumeObject::MakePicture(void)
{
    if( picture != nullptr )
    {
        std::cout << "[Warning] The picture already been created, Skip MakePicture()..." << std::endl;
    }
    else
    {
        picture = std::make_shared<TH3F>("","",
                    n_bin_x, 0, n_bin_x, n_bin_y, 0, n_bin_y, n_bin_z, 0, n_bin_z);
        for (int i = 0; i < n_bin_x; ++i)
        {
            for (int j = 0; j < n_bin_y; ++j)
            {
                for (int k = 0; k < n_bin_z; ++k)
                {
                    auto entry = GetEntry(i, n_bin_y-1-j, n_bin_z-1-k);
                    picture->SetBinContent(i, j, k, entry);
                }
            }
        }
        picture->SetStats(0);
        picture->SetTitle("");
        picture->GetXaxis()->SetTickLength(0);
        picture->GetYaxis()->SetTickLength(0);
        picture->GetZaxis()->SetTickLength(0);
        picture->GetXaxis()->SetLabelSize(0);
        picture->GetYaxis()->SetLabelSize(0);
        picture->GetZaxis()->SetLabelSize(0);
    }
}

void VolumeObject::MakePictureRot(void)
{
    if( picture_rot != nullptr )
    {
        picture_rot->Delete();
        picture_rot.reset();
    }

    picture_rot = std::make_shared<TH3F>("","",
                    n_bin_x, 0, n_bin_x, n_bin_y, 0, n_bin_y, n_bin_z, 0, n_bin_z);
    for (int i = 0; i < n_bin_x; ++i)
    {
        for (int j = 0; j < n_bin_y; ++j)
        {
            for (int k = 0; k < n_bin_z; ++k)
            {
                auto entry = GetEntryRot(i, n_bin_y-1-j, n_bin_z-1-k);
                picture_rot->SetBinContent(i, j, k, entry);
            }
        }
    }
    picture_rot->SetStats(0);
    picture_rot->SetTitle("");
    picture_rot->GetXaxis()->SetTickLength(0);
    picture_rot->GetYaxis()->SetTickLength(0);
    picture_rot->GetZaxis()->SetTickLength(0);
    picture_rot->GetXaxis()->SetLabelSize(0);
    picture_rot->GetYaxis()->SetLabelSize(0);
    picture_rot->GetZaxis()->SetLabelSize(0);
}

void VolumeObject::Rotate(double _phi, double _theta, double _psi)
{
    EulerAngle angle(_phi, _theta, _psi);
    Rotation3D rot = angle.GetRotMatrix();

    double d[9] = { rot[0], rot[1], rot[2],
                    rot[3], rot[4], rot[5],
                    rot[6], rot[7], rot[8] };
    SMatrix3d mtx_rot_inv( d, 9 );
    SVector3d vec_center( (n_bin_x-1.0)/2.0, (n_bin_y-1.0)/2.0, (n_bin_z-1.0)/2.0 );
    for (int z = 0; z < n_bin_z; ++z)
    {
        for (int y = 0; y < n_bin_y; ++y)
        {
            for (int x = 0; x < n_bin_x; ++x)
            {
                SVector3d vec_coord(x, y, z);
                vec_coord -= vec_center;
                vec_coord = mtx_rot_inv * vec_coord + vec_center;
                auto x_new = (vec_coord[0] < n_bin_x) ? vec_coord[0] : vec_coord[0]-n_bin_x;
                auto y_new = (vec_coord[1] < n_bin_y) ? vec_coord[1] : vec_coord[1]-n_bin_y;
                auto z_new = (vec_coord[2] < n_bin_z) ? vec_coord[2] : vec_coord[2]-n_bin_z;
                data_rot[GetBinIndex(x, y, z)] = GetEntry(round(x_new), round(y_new), round(z_new));
            }
        }
    }
    MakePictureRot();
}