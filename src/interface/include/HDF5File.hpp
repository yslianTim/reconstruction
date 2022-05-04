#ifndef HDF5FILE_HPP_
#define HDF5FILE_HPP_

#include <algorithm>
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include "H5Cpp.h"
#include "FileObject.hpp"
#include "DataBlock.hpp"

// HDF5 file format references:
// -- 

using namespace H5;

class HDF5File : public FileObject
{
    std::string path_to_input, path_to_output;
    std::fstream hdf5_file, hdf5_output;
    std::unique_ptr<float[]> data;
    std::unique_ptr<DataBlock> dataBlock;
    bool fGetHeader, fGetData;

public:
    ~HDF5File(void) {}
    void Print(void) const override;
    void SetInputFile(const std::string & path_to_input) override;
    void SetOutputFile(const std::string & path_to_output) override;
    void AccessData(void) override;
    void SaveData(void) override;
    std::unique_ptr<DataBlock> const& GetDataBlockRef(void) const override { return dataBlock; }

private:
    void Initialize(void) override;
    void Load(void) override;
    void Write(void) override;
    void FillDataBlock(void) override;
    void ReadHeader(void);
    void ReadData(void);
    void WriteHeader(void);
    void WriteData(void);
};


class HDF5FileCreator : public FileObjectCreator
{
public:
    std::unique_ptr<FileObject> CreateFileObject() override
    {
        return std::make_unique<HDF5File>();
    }
};

#endif