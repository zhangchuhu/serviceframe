#include "core/sox/logger.h"
#include "BuddyListMetaDao.h"
#include "server_common/dblib/MysqlCallback.h"

using namespace std;
using namespace server::imdao;
using namespace server::mysqldb;
using namespace server::mysqlimdb;

CBuddyListMetaDao::CBuddyListMetaDao(MysqlHelperTempalte *p)
{
    
    struct LoadMetaCallBack: public AbstractCallBack
    {
        virtual void onResult(mysqlpp::Result &result, void *arg)
        {
            list<CMetaData> *l = (list<CMetaData> *) arg;
            for (mysqlpp::Result::iterator iter = result.begin(); iter != result.end(); iter++)
            {
                CMetaData md;
                md.m_uBegin = (uint32_t) (*iter).at(0);
                md.m_uEnd = (uint32_t) (*iter).at(1);
                md.m_strDbName = (string) (*iter).at(2);
                (*l).push_back(md);
		log(Info, "[CBuddyListMetaDao::CBuddyListMetaDao]: begin:%u end:%u db:%s", md.m_uBegin, md.m_uEnd, md.m_strDbName.c_str());
            }
        }
    } objCb;

	objCb.setPreview(false);
    setMysqlHelperTempalte(p);
    p->commonSqlExe("im_meta", &objCb, &m_listMetaData, "select u_begin,u_end,str_db_name from tbl_buddylist_meta order by priority");
}


string &CBuddyListMetaDao::GetDbName(uint32_t uUsrID)
{
    for (list<CMetaData>::iterator iter = m_listMetaData.begin(); iter != m_listMetaData.end(); iter++)
    {
        CMetaData &md = (*iter);
        if (uUsrID >= md.m_uBegin && uUsrID <= md.m_uEnd)
        {
            return md.m_strDbName;
        }
    }
    return m_strDefDbName;
}

