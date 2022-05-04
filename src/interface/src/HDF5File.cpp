#include "HDF5File.hpp"

void HDF5File::Print(void) const
{
    
}

void HDF5File::SetInputFile(const std::string & path_to_input)
{
    HDF5File::path_to_input = path_to_input;
    Initialize();
}

void HDF5File::SetOutputFile(const std::string & path_to_output)
{
    HDF5File::path_to_output = path_to_output;
}

void HDF5File::Initialize(void)
{
    fGetHeader = fGetData = false;
}

void HDF5File::Load(void)
{
    ReadHeader();
    ReadData();
}

void HDF5File::ReadHeader(void)
{
    
}

void HDF5File::ReadData(void)
{

}

void HDF5File::AccessData(void)
{
    hdf5_file.open(path_to_input.c_str(), std::ios_base::in);
    Load();
    hdf5_file.close();
}

void HDF5File::FillDataBlock(void)
{
    

}

void HDF5File::SaveData(void)
{
    hdf5_output.open(path_to_output.c_str(), std::ios_base::out);
    Write();
    hdf5_output.close();
}

void HDF5File::Write(void)
{
    WriteHeader();
    WriteData();
}

void HDF5File::WriteHeader(void)
{
    
}

void HDF5File::WriteData(void)
{

}

