#ifndef DATAMANAGER_HPP_
#define DATAMANAGER_HPP_

#include <vector>
#include <iostream>
#include "ImageObject.hpp"
#include "FileManager.hpp"

class DataManager
{
    std::vector<std::shared_ptr<ImageObject>> dataObjectList;

public:
    enum DATATYPE
    {
        UNDEFINED = 0,
        IMAGE     = 1,
        MODEL     = 2
    };

    DataManager(void) {}
    ~DataManager(void) {}
    void Print(void) const;
    void ImportData(void);
    

private:
    int GetDataType(void) const;

};

#endif