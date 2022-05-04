#ifndef MRCFILE_HPP_
#define MRCFILE_HPP_

#include <algorithm>
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include "FileObject.hpp"
#include "DataBlock.hpp"

// MRC/CCP4 2014 file format references:
// -- https://www.ccpem.ac.uk/mrc_format/mrc2014.php
// -- https://github.com/jewettaij/visfd/tree/master/lib/mrc_simple


class MrcFile : public FileObject
{
    std::string path_to_input, path_to_output;
    std::fstream mrc_file, mrc_output;
    std::unique_ptr<float[]> data;
    std::unique_ptr<DataBlock> dataBlock;
    bool fGetHeader, fGetData;
    float binSize[3];

    enum HEADER
    {
        SIZE_HEADER    = 1024,
        SIZE_WORD      =    4,
        SIZE_LABEL     =   80,
        NUM_WORD       =   56,
        NUM_LABEL      =   10,
        NUM_WORD_EXTRA =   25
    };

    enum AXIS
    {
        COLUMN  = 0,
        ROW     = 1,
        SECTION = 2
    };

    enum MODE
    {
        UNDEFINED       =  -1,
        SIGNED_INT8     =   0,
        SIGNED_INT16    =   1,
        SIGNED_FLOAT32  =   2,
        COMPLEX_INT16   =   3,
        COMPLEX_FLOAT32 =   4,
        UNSIGNED_INT16  =   6,
        IEEE754_FLOAT16 =  12,
        TWOPERBYTE_BIT4 = 101
    };

public:
    ~MrcFile() {}
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
    int GetMode(void) const { return mode; }
    int GetNumberOfColumn(void) const { return nArray[AXIS::COLUMN]; }
    int GetNumberOfRow(void) const { return nArray[AXIS::ROW]; }
    int GetNumberOfSection(void) const { return nArray[AXIS::SECTION]; }
    int GetIColumn(void) const { return iLocation[AXIS::COLUMN]; }
    int GetIRow(void) const { return iLocation[AXIS::ROW]; }
    int GetISection(void) const { return iLocation[AXIS::SECTION]; }
    int GetGridx(void) const { return grid[AXIS::COLUMN]; }
    int GetGridy(void) const { return grid[AXIS::ROW]; }
    int GetGridz(void) const { return grid[AXIS::SECTION]; }

    // Index of array: [0]:Columns,  [1]:Rows,  [2]:Sections
    int   nArray[3];          // Number of column/row/section in 3D data array
    int   mode;               // Type of data
    int   iLocation[3];       // Location of first column/row/section in unit cell
    int   grid[3];            // Sampling along X/Y/Z axis of unit cell
    float dCellInAngstrom[3]; // Cell dimensions in angstroms
    float dCellInDegree[3];   // Cell dimensions in degrees
    int   axis[3];            // Axis corresponding to column/row/section: 1=X, 2=Y, 3=Z
    float densityMin;         // Minimum density value
    float densityMax;         // Maximum density value
    float densityMean;        // Mean density value
    int   iSpaceGroup;        // Space group number: 0, 1, 401
    int   sizeExHeader;       // Size of extended header in bytes
    char  extra[NUM_WORD_EXTRA*4];  // Extra space used for anything: 0 by default
    int   code;               // A code for the type of metadata held in the extended header
    int   version;            // Version of the MRC format: year*10 + version within the year
    float origin[3];          // Phase origin or origin of subvolume
    char  map[4];             // Character string 'MAP' to identify file type
    char  stamp[4];           // Machine stamp encoding byte ordering of data
    float rms;                // RMS deviation of map from mean density
    int   nLabel;             // Number of labels being used
    char  label[NUM_LABEL*SIZE_LABEL];  // 10 80-character text labels 
};


class MrcFileCreator : public FileObjectCreator
{
public:
    std::unique_ptr<FileObject> CreateFileObject(void) override
    {
        return std::make_unique<MrcFile>();
    }
};

#endif