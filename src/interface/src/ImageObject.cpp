#include "ImageObject.hpp"

void ImageObject::Print(void) const
{
    using namespace std;
    cout << " o=========================================o " << endl;
    cout << " |  Image Obj. |    Column   |     Row     | " << endl;
    cout << " o=========================================o " << endl;
    cout << " |  Dimension  |";
    cout << setw(12) << n_bin_x << " |"
         << setw(12) << n_bin_y << " |" << endl;
    cout << " |  Resolution |";
    cout << setw(12) << bin_size_x << " |"
         << setw(12) << bin_size_y << " |" << endl;
    cout << " o=========================================o " << endl;
}

void ImageObject::Set(int _n_bin_x, int _n_bin_y, float _bin_size_x, float _bin_size_y)
{
    n_bin_x = _n_bin_x;
    n_bin_y = _n_bin_y;
    bin_size_x = _bin_size_x;
    bin_size_y = _bin_size_y;
    if (data_ori == nullptr) data_ori = std::make_unique<float[]>(_n_bin_x * _n_bin_y);
    if (data_rot == nullptr) data_rot = std::make_unique<float[]>(_n_bin_x * _n_bin_y);
}

void ImageObject::ImportData(std::unique_ptr<float[]> & _data_ori)
{
    data_ori = std::move(_data_ori);
    _data_ori.reset(nullptr);
    MakePicture();
}

void ImageObject::MakePicture(void)
{
    if( picture != nullptr )
    {
        std::cout << "[Warning] The picture already been created, Skip MakePicture()..." << std::endl;
    }
    else
    {
        picture = std::make_shared<TH2F>("","", n_bin_x, 0, n_bin_x, n_bin_y, 0, n_bin_y);
        for (int i = 0; i < n_bin_x; ++i)
        {
            for (int j = 0; j < n_bin_y; ++j)
            {
                auto entry = GetEntry(i,n_bin_y-1-j);
                picture->SetBinContent(i,j,entry);
            }
        }
        picture->SetStats(0);
        picture->SetTitle("");
        picture->GetXaxis()->SetTickLength(0);
        picture->GetYaxis()->SetTickLength(0);
        picture->GetXaxis()->SetLabelSize(0);
        picture->GetYaxis()->SetLabelSize(0);
    }
}

void ImageObject::MakePictureRot(void)
{
    if( picture_rot != nullptr )
    {
        picture_rot->Delete();
        picture_rot.reset();
    }

    picture_rot = std::make_shared<TH2F>("","", n_bin_x, 0, n_bin_x, n_bin_y, 0, n_bin_y);
    for (int i = 0; i < n_bin_x; ++i)
    {
        for (int j = 0; j < n_bin_y; ++j)
        {
            auto entry = GetEntryRot(i,n_bin_y-1-j);
            picture_rot->SetBinContent(i,j,entry);
        }
    }
    picture_rot->SetStats(0);
    picture_rot->SetTitle("");
    picture_rot->GetXaxis()->SetTickLength(0);
    picture_rot->GetYaxis()->SetTickLength(0);
    picture_rot->GetXaxis()->SetLabelSize(0);
    picture_rot->GetYaxis()->SetLabelSize(0);
}

void ImageObject::Rotate(double _theta)
{
    double d[4] = { cos(_theta), sin(_theta),
                   -sin(_theta), cos(_theta) };
    SMatrix2d mtx_rot_inv( d, 4 );
    SVector2d vec_center( (n_bin_x-1.0)/2.0, (n_bin_y-1.0)/2.0 );
    for (int i = 0; i < n_bin_x; ++i)
    {
        for (int j = 0; j < n_bin_y; ++j)
        {
            SVector2d vec_coord( i, j );
            vec_coord -= vec_center;
            vec_coord = mtx_rot_inv * vec_coord + vec_center;
            auto i_new = (vec_coord[0]<n_bin_x)? round(vec_coord[0]) : round(vec_coord[0]-n_bin_x);
            auto j_new = (vec_coord[1]<n_bin_y)? round(vec_coord[1]) : round(vec_coord[1]-n_bin_y);
            data_rot[i + n_bin_x*j] = GetEntry(i_new,j_new);
        }
    }
    MakePictureRot();
}