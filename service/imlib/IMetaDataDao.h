#pragma once
#include <string.h>
#include "common/int_types.h"

namespace server
{
namespace imdao
{

class IMetaDataDao
{
public:
    virtual std::string &GetDbName(uint32_t uKey) = 0;
};

class IMetaDataDaoAware
{
public:
    IMetaDataDao *GetMetaData()
    {
        return m_pMetaData;
    }

    void SetMetaData(IMetaDataDao *p)
    {
        m_pMetaData = p;
    }

private:
    IMetaDataDao *m_pMetaData;
};


}

}

