#include "DataManager.hpp"



int DataManager::GetDataType(void) const
{
    if (1)
    {
        return DATATYPE::IMAGE;
    }
    else if (1)
    {
        return DATATYPE::MODEL;
    }
    else
    {
        return DATATYPE::UNDEFINED;
    }
}