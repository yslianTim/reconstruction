#ifndef FILEMANAGER_HPP_
#define FILEMANAGER_HPP_

#include <string>
#include <vector>
#include <iostream>
#include <exception>
#include <experimental/filesystem>
#include <iterator>
#include "FileObject.hpp"
#include "HDF5File.hpp"
#include "MrcFile.hpp"

class FileManager
{
    bool fBlockImportFile = false;
    std::string current_path, current_format;
    std::vector<int> fileIndex;
    std::vector<std::shared_ptr<FileObject>> fileObjectList;
    std::vector<std::string> path_to_file;
    std::vector<std::string> file_name;

public:
    enum FORMAT
    {
        UNDEFINED = 0, HDF5 = 1, MRC = 2
    };

    static FileManager& Instance(void);
    void Print(void) const;
    void ImportFile(const std::string & current_path);
    int NumberOfImportFile(void) const { return fileObjectList.size(); }
    std::vector<int> GetFileIndexList(void) const { return fileIndex; }
    std::vector<std::shared_ptr<FileObject>> GetFileList(void) const { return fileObjectList; }
    std::vector<std::string> GetFilePathList(void) const { return path_to_file; }
    std::vector<std::string> GetFileNameList(void) const { return file_name; }

private:
    FileManager(void) = default;
    FileManager(FileManager const &) = delete;
    void operator=(FileManager const &) = delete;
    void ParsePath(void);
    void SetFile(void);
    void CreateFileClass(void);
    int GetFormat(void) const;
    bool FileExist(void) const;
    std::unique_ptr<FileObjectCreator> MakeFileObjectCreator(void) const;
    void BlockImportFile(void) { fBlockImportFile = true; }

};

#endif