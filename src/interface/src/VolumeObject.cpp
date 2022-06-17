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

void VolumeObject::ImportData(std::unique_ptr<float[]> & _data)
{
    data = std::move(_data);
    _data.reset(nullptr);
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
        picture = std::make_shared<TH3F>("","", n_bin_x, 0, n_bin_x, n_bin_y, 0, n_bin_y, n_bin_z, 0, n_bin_z);
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