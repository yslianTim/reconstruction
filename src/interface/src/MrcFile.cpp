#include "MrcFile.hpp"

void MrcFile::Print(void) const
{  
    using namespace std;
    cout << " o=======================================================o " << endl;
    cout << " |  MRC File   |    Column   |     Row     |   Section   | " << endl;
    cout << " o=======================================================o " << endl;
    cout << " |  Dimension  |";
    cout << setw(12) << nArray[0] << " |"
         << setw(12) << nArray[1] << " |"
         << setw(12) << nArray[2] << " |" << endl;
    cout << " |  Location   |";
    cout << setw(12) << iLocation[0] << " |"
         << setw(12) << iLocation[1] << " |"
         << setw(12) << iLocation[2] << " |" << endl;
    cout << " |  Sampling   |";
    cout << setw(12) << grid[0] << " |"
         << setw(12) << grid[1] << " |"
         << setw(12) << grid[2] << " |" << endl;
    cout << " |  Res.(Ang)  |";
    cout << setw(12) << dCellInAngstrom[0] << " |"
         << setw(12) << dCellInAngstrom[1] << " |"
         << setw(12) << dCellInAngstrom[2] << " |" << endl;
    cout << " |  Res.(Deg)  |";
    cout << setw(12) << dCellInDegree[0] << " |"
         << setw(12) << dCellInDegree[1] << " |"
         << setw(12) << dCellInDegree[2] << " |" << endl;
    cout << " |  Axis Def.  |";
    cout << setw(12) << axis[0] << " |"
         << setw(12) << axis[1] << " |"
         << setw(12) << axis[2] << " |" << endl;
    cout << " |  Phase Ori  |";
    cout << setw(12) << origin[0] << " |"
         << setw(12) << origin[1] << " |"
         << setw(12) << origin[2] << " |" << endl;
    cout << " |=======================================================| " << endl;
    cout << " | Mode (Type of Data)  | " << left << setw(30) << mode         << " |" << endl;
    cout << " | Min. Density Value   | " << left << setw(30) << densityMin   << " |" << endl;
    cout << " | Max. Density Value   | " << left << setw(30) << densityMax   << " |" << endl;
    cout << " | Mean Density Value   | " << left << setw(30) << densityMean  << " |" << endl;
    cout << " | R.M.S. Deviation     | " << left << setw(30) << rms          << " |" << endl;
    cout << " | Space Group Number   | " << left << setw(30) << iSpaceGroup  << " |" << endl;
    cout << " | Extended Header Size | " << left << setw(30) << sizeExHeader << " |" << endl;
    string extra_string(extra);
    cout << " | Extended Header      | " << left << setw(30) << extra_string << " |" << endl;
    cout << " | Type of Metadata     | " << left << setw(30) << code         << " |" << endl;
    cout << " | Version of MRC       | " << left << setw(30) << version      << " |" << endl;
    cout << " | Number of Labels     | " << left << setw(30) << nLabel       << " |" << endl;
    string label_string(label);
    cout << " | 10 80-char Labels    | " << left << setw(30) << label_string << " |" << endl;
    cout << " o=======================================================o " << endl;
}

void MrcFile::SetInputFile(const std::string & path_to_input)
{
    MrcFile::path_to_input = path_to_input;
    Initialize();
}

void MrcFile::SetOutputFile(const std::string & path_to_output)
{
    MrcFile::path_to_output = path_to_output;
}

void MrcFile::Initialize(void)
{
    fGetHeader = fGetData = false;
    binSize[0] = binSize[1] = binSize[2] = 0.0;
    mode = MODE::UNDEFINED;
    nArray[0] = nArray[1] = nArray[2] = 0;
    iLocation[0] = iLocation[1] = iLocation[2] = 0;
    grid[0] = grid[1] = grid[2] = 0;
    origin[0] = origin[1] = origin[2] = 0;
    dCellInAngstrom[0] = dCellInAngstrom[1] = dCellInAngstrom[2] = 0.0;
    dCellInDegree[0] = dCellInDegree[1] = dCellInDegree[2] = 0.0;
    axis[0] = 1;
    axis[1] = 2;
    axis[2] = 3;
    densityMin = 0.0;
    densityMax = 0.0;
    densityMean = 0.0;
    iSpaceGroup = 0;
    sizeExHeader = 0;
    code = 0;
    version = 20140;
    rms = 0.0;
    nLabel = 0;
}

void MrcFile::Load(void)
{
    ReadHeader();
    ReadData();
    FillDataBlock();
}

void MrcFile::ReadHeader(void)
{
    if (fGetHeader)
    {
        std::cout << "[Warning] The header have been read, skip ReadHeader()" << std::endl;
        return;
    }
    
    auto header = std::make_unique<char[]>(HEADER::SIZE_HEADER);
    mrc_file.read(header.get(), HEADER::SIZE_HEADER);
    
    nArray[0] = *(reinterpret_cast<int*>(header.get()) + 0);
    nArray[1] = *(reinterpret_cast<int*>(header.get()) + 1);
    nArray[2] = *(reinterpret_cast<int*>(header.get()) + 2);
    mode = *(reinterpret_cast<int*>(header.get()) + 3);
    iLocation[0] = *(reinterpret_cast<int*>(header.get()) + 4);
    iLocation[1] = *(reinterpret_cast<int*>(header.get()) + 5);
    iLocation[2] = *(reinterpret_cast<int*>(header.get()) + 6);
    grid[0] = *(reinterpret_cast<int*>(header.get()) + 7);
    grid[1] = *(reinterpret_cast<int*>(header.get()) + 8);
    grid[2] = *(reinterpret_cast<int*>(header.get()) + 9);
    dCellInAngstrom[0] = *(reinterpret_cast<float*>(header.get()) + 10);
    dCellInAngstrom[1] = *(reinterpret_cast<float*>(header.get()) + 11);
    dCellInAngstrom[2] = *(reinterpret_cast<float*>(header.get()) + 12);
    dCellInDegree[0] = *(reinterpret_cast<float*>(header.get()) + 13);
    dCellInDegree[1] = *(reinterpret_cast<float*>(header.get()) + 14);
    dCellInDegree[2] = *(reinterpret_cast<float*>(header.get()) + 15);
    axis[0] = *(reinterpret_cast<int*>(header.get()) + 16);
    axis[1] = *(reinterpret_cast<int*>(header.get()) + 17);
    axis[2] = *(reinterpret_cast<int*>(header.get()) + 18);
    densityMin  = *(reinterpret_cast<float*>(header.get()) + 19);
    densityMax  = *(reinterpret_cast<float*>(header.get()) + 20);
    densityMean = *(reinterpret_cast<float*>(header.get()) + 21);
    iSpaceGroup = *(reinterpret_cast<int*>(header.get()) + 22);
    sizeExHeader = *(reinterpret_cast<int*>(header.get()) + 23);
    std::copy(header.get() + 24, header.get() + 24 + HEADER::NUM_WORD_EXTRA, extra);
    code = *(reinterpret_cast<int*>(header.get()) + 26);
    version = *(reinterpret_cast<int*>(header.get()) + 27);
    origin[0] = *(reinterpret_cast<float*>(header.get()) + 49);
    origin[1] = *(reinterpret_cast<float*>(header.get()) + 50);
    origin[2] = *(reinterpret_cast<float*>(header.get()) + 51);
    rms =  *(reinterpret_cast<float*>(header.get()) + 54);
    nLabel =  *(reinterpret_cast<int*>(header.get()) + 55);
    std::copy(header.get() + 56, header.get() + 56 + HEADER::NUM_LABEL, label);
    fGetHeader = true;
    binSize[0] = dCellInAngstrom[0]/grid[0];
    binSize[1] = dCellInAngstrom[1]/grid[1];
    binSize[2] = dCellInAngstrom[2]/grid[2];
}

void MrcFile::ReadData(void)
{
    if (fGetData)
    {
        std::cout << "[Warning] The data block have been read, skip ReadData()" << std::endl;
        return;
    }
 
    auto nVoxels = nArray[0] * nArray[1] * nArray[2];
    auto data_out = std::make_unique<float[]>(nVoxels);
    switch(mode)
    {
    case MODE::UNDEFINED:
       break;
    case MODE::SIGNED_FLOAT32:
        float entry;
        for (int v = 0; v < nVoxels; v++)
        {
            mrc_file.read(reinterpret_cast<char*>(&entry), sizeof(float));
            data_out[v] = static_cast<float>(entry);
        }
        data = std::move(data_out);
        break;
    default:
        break;
    }
    
    fGetData = true;
}

void MrcFile::AccessData(void)
{
    mrc_file.open(path_to_input.c_str(), std::ios_base::binary | std::ios_base::in);
    Load();
    mrc_file.close();
}

void MrcFile::FillDataBlock(void)
{
    dataBlock = std::make_unique<DataBlock>(nArray, binSize);
    dataBlock->SetDataArray(data);
    if ((iSpaceGroup == 0 && nArray[AXIS::SECTION] > 1) || iSpaceGroup >= 401)
    {
        dataBlock->SetStack();
    }

    if (grid[2] == 1)
    {
        dataBlock->Set2D();
    }
    else
    {
        dataBlock->Set3D();
    }

}

void MrcFile::SaveData(void)
{
    mrc_output.open(path_to_output.c_str(), std::ios_base::binary | std::ios_base::out);
    Write();
    mrc_output.close();
}

void MrcFile::Write(void)
{
    WriteHeader();
    WriteData();
};

void MrcFile::WriteHeader(void)
{
    mrc_output.write(reinterpret_cast<char*>(&nArray[0]), sizeof(int));
    mrc_output.write(reinterpret_cast<char*>(&nArray[1]), sizeof(int));
    mrc_output.write(reinterpret_cast<char*>(&nArray[2]), sizeof(int));
    mrc_output.write(reinterpret_cast<char*>(&mode), sizeof(int));
    mrc_output.write(reinterpret_cast<char*>(&iLocation[0]), sizeof(int));
    mrc_output.write(reinterpret_cast<char*>(&iLocation[1]), sizeof(int));
    mrc_output.write(reinterpret_cast<char*>(&iLocation[2]), sizeof(int));
    mrc_output.write(reinterpret_cast<char*>(&grid[0]), sizeof(int));
    mrc_output.write(reinterpret_cast<char*>(&grid[1]), sizeof(int));
    mrc_output.write(reinterpret_cast<char*>(&grid[2]), sizeof(int));
    mrc_output.write(reinterpret_cast<char*>(&dCellInAngstrom[0]), sizeof(float));
    mrc_output.write(reinterpret_cast<char*>(&dCellInAngstrom[1]), sizeof(float));
    mrc_output.write(reinterpret_cast<char*>(&dCellInAngstrom[2]), sizeof(float));
    mrc_output.write(reinterpret_cast<char*>(&dCellInDegree[0]), sizeof(float));
    mrc_output.write(reinterpret_cast<char*>(&dCellInDegree[1]), sizeof(float));
    mrc_output.write(reinterpret_cast<char*>(&dCellInDegree[2]), sizeof(float));
    mrc_output.write(reinterpret_cast<char*>(&axis[0]), sizeof(int));
    mrc_output.write(reinterpret_cast<char*>(&axis[1]), sizeof(int));
    mrc_output.write(reinterpret_cast<char*>(&axis[2]), sizeof(int));
    mrc_output.write(reinterpret_cast<char*>(&densityMin), sizeof(float));
    mrc_output.write(reinterpret_cast<char*>(&densityMax), sizeof(float));
    mrc_output.write(reinterpret_cast<char*>(&densityMean), sizeof(float));
    mrc_output.write(reinterpret_cast<char*>(&iSpaceGroup), sizeof(int));
    mrc_output.write(reinterpret_cast<char*>(&sizeExHeader), sizeof(int));
    mrc_output.write(extra, NUM_WORD_EXTRA*4);
    mrc_output.write(reinterpret_cast<char*>(&origin[0]), sizeof(float));
    mrc_output.write(reinterpret_cast<char*>(&origin[1]), sizeof(float));
    mrc_output.write(reinterpret_cast<char*>(&origin[2]), sizeof(float));
    mrc_output.write(reinterpret_cast<char*>(&rms), sizeof(float));
    mrc_output.write(reinterpret_cast<char*>(&nLabel), sizeof(int));
    mrc_output.write(label, NUM_LABEL*SIZE_LABEL);
}

void MrcFile::WriteData(void)
{
    auto nVoxels = nArray[0] * nArray[1] * nArray[2];
    for (int v = 0; v < nVoxels; v++)
    {
        mrc_output.write(reinterpret_cast<char*>(&(data[v])), sizeof(float));
    }
}

