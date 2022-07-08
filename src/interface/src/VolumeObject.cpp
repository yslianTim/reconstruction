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

void VolumeObject::Set(int _nx, int _ny, int _nz, float _sx, float _sy, float _sz)
{
    n_bin_x = _nx;
    n_bin_y = _ny;
    n_bin_z = _nz;
    bin_size_x = _sx;
    bin_size_y = _sy;
    bin_size_z = _sz;
    n_array = _nx * _ny * _nz;
    n_bin_ext = static_cast<int>(ceil(sqrt(_nx*_nx + _ny*_ny + _nz*_nz)));
    n_array_ext = n_bin_ext * n_bin_ext * n_bin_ext;
    is_extend = false;
}

void VolumeObject::ImportData(std::unique_ptr<float[]> & _data)
{
    MakeDataArray();
    data = std::move(_data);
    _data.reset(nullptr);
    MakePicture();
}

void VolumeObject::Extend(void)
{
    if (is_extend) return;
    MakeDataArrayExt();
    MakeDataArrayRot();

    SVector3d vec_center_ori( (n_bin_x-1.0)/2.0, (n_bin_y-1.0)/2.0, (n_bin_z-1.0)/2.0 );
    SVector3d vec_center_ext( (n_bin_ext-1.0)/2.0, (n_bin_ext-1.0)/2.0, (n_bin_ext-1.0)/2.0 );
    SVector3d vec_offset(vec_center_ext - vec_center_ori);
    auto offx = static_cast<int>(round(vec_offset[0]));
    auto offy = static_cast<int>(round(vec_offset[1]));
    auto offz = static_cast<int>(round(vec_offset[2]));
    for (int i = 0; i < n_bin_x; i++)
    {
        for (int j = 0; j < n_bin_y; j++)
        {
            for (int k = 0; k < n_bin_z; k++)
            {
                data_ext[GetBinIndexExt(i+offx, j+offy, k+offz)] = data[GetBinIndex(i, j, k)];
            }
        }
    }
    MakePictureExt();
    is_extend = true;
}

void VolumeObject::Rotate(double _phi, double _theta, double _psi)
{
    if (IsExtend() == false)
    {
        std::cout << "[Warning] VolumeObject should be extended before Rotate()!" << std::endl;
        return;
    }

    EulerAngle angle(_phi, _theta, _psi);
    Rotation3D rot = angle.GetRotMatrix();

    double d[9] = { rot[0], rot[1], rot[2],
                    rot[3], rot[4], rot[5],
                    rot[6], rot[7], rot[8] };
    SMatrix3d mtx_rot_inv( d, 9 );
    SVector3d vec_center( (n_bin_ext-1.0)/2.0, (n_bin_ext-1.0)/2.0, (n_bin_ext-1.0)/2.0 );
    for (int z = 0; z < n_bin_ext; z++)
    {
        for (int y = 0; y < n_bin_ext; y++)
        {
            for (int x = 0; x < n_bin_ext; x++)
            {
                SVector3d vec_coord(x, y, z);
                vec_coord -= vec_center;
                vec_coord = mtx_rot_inv * vec_coord + vec_center;
                auto x_new = (vec_coord[0] < n_bin_ext) ? vec_coord[0] : vec_coord[0]-n_bin_ext;
                auto y_new = (vec_coord[1] < n_bin_ext) ? vec_coord[1] : vec_coord[1]-n_bin_ext;
                auto z_new = (vec_coord[2] < n_bin_ext) ? vec_coord[2] : vec_coord[2]-n_bin_ext;
                
                data_rot[GetBinIndexExt(x, y, z)]
                     = GetEntryExt(round(x_new), round(y_new), round(z_new));
            }
        }
    }
    MakePictureRot();
}

void VolumeObject::MakeDataArray(void)
{
    if (data != nullptr)
    {
        std::cout << "[Warning] data is exist, Skip MakeDataArray()..." << std::endl;
        return;
    }
    data = std::make_unique<float[]>(n_array);
    for (int i = 0; i < n_array; i++)
    {
        data[i] = 0.0;
    }
}

void VolumeObject::MakeDataArrayExt(void)
{
    if (data_ext != nullptr)
    {
        std::cout << "[Warning] data_ext is exist, Skip MakeExtDataArray()..." << std::endl;
        return;
    }
    data_ext = std::make_unique<float[]>(n_array_ext);
    for (int i = 0; i < n_array_ext; i++)
    {
        data_ext[i] = 0.0;
    }
}

void VolumeObject::MakeDataArrayRot(void)
{
    if (data_rot != nullptr)
    {
        std::cout << "[Warning] data_rot is exist, Skip MakeRotDataArray()..." << std::endl;
        return;
    }
    data_rot = std::make_unique<float[]>(n_array_ext);
    for (int i = 0; i < n_array_ext; i++)
    {
        data_rot[i] = 0.0;
    }
}

void VolumeObject::MakePicture(void)
{
    if (picture != nullptr)
    {
        std::cout << "[Warning] The picture already been created, Skip MakePicture()!" << std::endl;
    }
    else
    {
        picture = std::make_shared<TH3F>("","",
                    n_bin_x, 0, n_bin_x, n_bin_y, 0, n_bin_y, n_bin_z, 0, n_bin_z);
        for (int i = 0; i < n_bin_x; i++)
        {
            for (int j = 0; j < n_bin_y; j++)
            {
                for (int k = 0; k < n_bin_z; k++)
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

void VolumeObject::MakePictureExt(void)
{
    if (picture_ext != nullptr)
    {
        picture_ext->Delete();
        picture_ext.reset();
    }

    picture_ext = std::make_shared<TH3F>("","",
                  n_bin_ext, 0, n_bin_ext, n_bin_ext, 0, n_bin_ext, n_bin_ext, 0, n_bin_ext);
    for (int i = 0; i < n_bin_ext; i++)
    {
        for (int j = 0; j < n_bin_ext; j++)
        {
            for (int k = 0; k < n_bin_ext; k++)
            {
                auto entry = GetEntryExt(i, n_bin_ext-1-j, n_bin_ext-1-k);
                picture_ext->SetBinContent(i, j, k, entry);
            }
        }
    }
    picture_ext->SetStats(0);
    picture_ext->SetTitle("");
    picture_ext->GetXaxis()->SetTickLength(0);
    picture_ext->GetYaxis()->SetTickLength(0);
    picture_ext->GetZaxis()->SetTickLength(0);
    picture_ext->GetXaxis()->SetLabelSize(0);
    picture_ext->GetYaxis()->SetLabelSize(0);
    picture_ext->GetZaxis()->SetLabelSize(0);
}

void VolumeObject::MakePictureRot(void)
{
    if (picture_rot != nullptr)
    {
        picture_rot->Delete();
        picture_rot.reset();
    }

    picture_rot = std::make_shared<TH3F>("","",
                  n_bin_ext, 0, n_bin_ext, n_bin_ext, 0, n_bin_ext, n_bin_ext, 0, n_bin_ext);
    for (int i = 0; i < n_bin_ext; i++)
    {
        for (int j = 0; j < n_bin_ext; j++)
        {
            for (int k = 0; k < n_bin_ext; k++)
            {
                auto entry = GetEntryRot(i, n_bin_ext-1-j, n_bin_ext-1-k);
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