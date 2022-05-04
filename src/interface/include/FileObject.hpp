#ifndef FILEOBJECT_HPP_
#define FILEOBJECT_HPP_

#include <iostream>
#include <memory>

class DataBlock;

class FileObject
{
public:
    virtual ~FileObject() = default;
    virtual void Print(void) const = 0;
    virtual void SetInputFile(const std::string & path_to_file) = 0;
    virtual void SetOutputFile(const std::string & path_to_file) = 0;
    virtual void AccessData(void) = 0;
    virtual void SaveData(void) = 0;
    virtual std::unique_ptr<DataBlock> const& GetDataBlockRef(void) const = 0;

private:
    virtual void Initialize(void) = 0;
    virtual void Load(void) = 0;
    virtual void FillDataBlock(void) = 0;
    virtual void Write(void) = 0;
};

class FileObjectCreator
{
public:
    virtual ~FileObjectCreator() = default;
    virtual std::unique_ptr<FileObject> CreateFileObject(void) = 0;
};

#endif