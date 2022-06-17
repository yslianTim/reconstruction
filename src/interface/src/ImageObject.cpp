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
}

void ImageObject::ImportData(std::unique_ptr<float[]> & _data)
{
    data = std::move(_data);
    _data.reset(nullptr);
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
