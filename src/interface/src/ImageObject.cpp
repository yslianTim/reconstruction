#include "ImageObject.hpp"

void ImageObject::Print(void) const
{
    qInfo(" o=========================================o");
    qInfo(" |  Image Obj. |    Column   |     Row     |");
    qInfo(" o=========================================o");
    qInfo(" |  Dimension  | %11d | %11d |", n_bin_x, n_bin_y);
    qInfo(" |  Resolution | %11.5f | %11.5f |", bin_size_x, bin_size_y);
    qInfo(" o=========================================o");
}

void ImageObject::Set(int _nx, int _ny, float _sx, float _sy)
{
    n_bin_x = _nx;
    n_bin_y = _ny;
    bin_size_x = _sx;
    bin_size_y = _sy;
    n_array = _nx * _ny;
}

void ImageObject::ImportData(std::unique_ptr<float[]> & _data)
{
    MakeDataArray();
    MakeDataArrayRot();
    data = std::move(_data);
    _data.reset(nullptr);
    MakePicture();
}

void ImageObject::MakePicture(void)
{
    if( picture != nullptr )
    {
        qWarning("The picture already been created, Skip MakePicture()...");
    }
    else
    {
        picture = std::make_shared<TH2F>("","", n_bin_x, 0, n_bin_x, n_bin_y, 0, n_bin_y);
        for (int i = 0; i < n_bin_x; ++i)
        {
            for (int j = 0; j < n_bin_y; ++j)
            {
                auto entry = GetEntry(i, n_bin_y-1-j);
                picture->SetBinContent(i, j, entry);
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
            auto entry = GetEntryRot(i, n_bin_y-1-j);
            picture_rot->SetBinContent(i, j, entry);
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
    double d[4] = { cos(_theta),-sin(_theta),
                    sin(_theta), cos(_theta) };
    SMatrix2d mtx_rot_inv( d, 4 );
    SVector2d vec_center( (n_bin_x-1.0)/2.0, (n_bin_y-1.0)/2.0 );
    for (int y = 0; y < n_bin_y; ++y)
    {
        for (int x = 0; x < n_bin_x; ++x)
        {
            SVector2d vec_coord(x, y);
            vec_coord -= vec_center;
            vec_coord = mtx_rot_inv * vec_coord + vec_center;
            auto x_new = (vec_coord[0] < n_bin_x) ? vec_coord[0] : vec_coord[0]-n_bin_x;
            auto y_new = (vec_coord[1] < n_bin_y) ? vec_coord[1] : vec_coord[1]-n_bin_y;
            data_rot[GetBinIndex(x, y)] = GetEntry(round(x_new), round(y_new));
        }
    }
    MakePictureRot();
}

void ImageObject::MakeDataArray(void)
{
    if (data != nullptr)
    {
        qWarning("data is exist, Skip MakeDataArray()...");
        return;
    }
    data = std::make_unique<float[]>(n_array);
    for (int i = 0; i < n_array; i++)
    {
        data[i] = 0.0;
    }
}

void ImageObject::MakeDataArrayRot(void)
{
    if (data_rot != nullptr)
    {
        qWarning("data_rot is exist, Skip MakeRotDataArray()...");
        return;
    }
    data_rot = std::make_unique<float[]>(n_array);
    for (int i = 0; i < n_array; i++)
    {
        data_rot[i] = 0.0;
    }
}

void ImageObject::Normalize(void)
{
    auto range = GetMaximum() - GetMinimum();
    auto dmin = GetMinimum();
    for (int i = 0; i < n_array; i++)
    {
        data[i] = (data[i] - dmin)/range;
    }
}