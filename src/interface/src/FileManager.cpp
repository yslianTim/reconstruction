#include "FileManager.hpp"

FileManager& FileManager::Instance(void)
{
    static FileManager filemanager;
    return filemanager;
}

void FileManager::Print(void) const
{
    std::cout << "Number of imported files: " << NumberOfImportFile() << std::endl;
    std::cout << "Files list: " << std::endl;
    
    for (unsigned int i = 0; i < fileIndex.size(); ++i)
    {
        std::cout << "[" << fileIndex.at(i) << "]";
        std::cout << " --> " << path_to_file.at(i) << std::endl;
    }
}

void FileManager::ImportFile(const std::string & current_path)
{
    if (fBlockImportFile)
    {
        std::cout << "[Warning] ImportFile() have been block, Skip importing: "
        << current_path << std::endl;
    }
    else
    {
        FileManager::current_path = current_path;
        SetFile();
    }
}

void FileManager::SetFile(void)
{
    if (!FileExist())
    {
        std::cout << "[Warning] Import file: " << current_path
        << " doesn't exist! Skip this file..." << std::endl;
        return;
    }
    ParsePath();
    CreateFileClass();
    fileObjectList.back()->SetInputFile(current_path);
}

void FileManager::ParsePath(void)
{
    fileIndex.push_back(path_to_file.size());
    path_to_file.push_back(current_path);
    file_name.push_back(current_path.substr(current_path.find_last_of("/") + 1));
    current_format = current_path.substr(current_path.find_last_of(".") + 1);
}

void FileManager::CreateFileClass(void)
{
    std::unique_ptr<FileObjectCreator> creator = MakeFileObjectCreator();
    fileObjectList.push_back(creator->CreateFileObject());
}

std::unique_ptr<FileObjectCreator> FileManager::MakeFileObjectCreator(void) const
{
    switch (GetFormat())
    {
    case FORMAT::MRC:
        return std::make_unique<MrcFileCreator>();
    case FORMAT::HDF5:
        return std::make_unique<HDF5FileCreator>();
    default:
        return std::make_unique<MrcFileCreator>();
    }
}

int FileManager::GetFormat(void) const
{
    if (current_format.compare("mrc") == 0 || current_format.compare("mrcs") == 0)
    {
        return FORMAT::MRC;
    }
    else if (current_format.compare("h5") == 0)
    {
        return FORMAT::HDF5;
    }
    else
    {
        return FORMAT::UNDEFINED;
    }
}

bool FileManager::FileExist(void) const
{ 
    return std::filesystem::exists(current_path.c_str());
}

