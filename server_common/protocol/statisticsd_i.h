#ifndef _STATISTICSD_I_H
#define _STATISTICSD_I_H

#include <iomanip>
#include<iostream>
#include<string>
#include<sstream>
#include "common/core/base_svid.h"
#include "common/packet.h"
#include "common/protocol/const.h"
#include "common/protocol/zip_tpl.h"
#include "protocol/CodeInfo.h"
#include <vector>
#include "common/res_code.h"

using namespace std;

namespace protocol{
	enum {LFGD_SVID = 150};
}

namespace server{
namespace statisticsd{

	enum EN_IFGD_URI
	{
		URI_LOGIN_QUALITY = ( 1 << 8)|protocol::LFGD_SVID,
		URI_REGISTER_INFO = ( 2 << 8)|protocol::LFGD_SVID,
		URI_USER_INFO = ( 3 << 8)|protocol::LFGD_SVID,
        URI_MIBAO = ( 4 << 8)|protocol::LFGD_SVID,
        URI_KDC = ( 5 << 8)|protocol::LFGD_SVID
	};

	struct LoginQuality:public sox::Marshallable
	{
		enum{ uri = URI_LOGIN_QUALITY};

		uint32_t		user_type;
		std::string		user;
		uint32_t		ip;
		uint32_t		ct;
		std::string		uname;
		uint32_t		rescode;
		uint32_t		pt;
		uint32_t		ut;
		uint32_t		safe_type;
		uint32_t		account_try;
		uint32_t		ip_try;
		uint32_t		ip_err;
		uint32_t		result;
		uint32_t		version;
		std::string		extra;
			
		LoginQuality():user_type(0),ip(0),ct(0),rescode(0),pt(0),ut(0),safe_type(0),account_try(0),ip_try(0),ip_err(0),result(0){};

		virtual void marshal(sox::Pack &pk) const
		{
			pk << user_type << user << ip << ct << uname << rescode << pt << ut << safe_type << account_try << ip_try << ip_err << result << version;
			if( !extra.empty() )
			{
				pk << extra;
			}
		}
		
		virtual void unmarshal( const sox::Unpack &up)
		{
			up >> user_type >> user >> ip >> ct >> uname >> rescode >> pt >> ut >> safe_type >> account_try >> ip_try >> ip_err >> result >> version;
			if( !up.empty() )
			{
				up >> extra;
			}
		}
	};

	struct RegisterInfo:public sox::Marshallable
	{
		enum{ uri = URI_REGISTER_INFO};

		std::string uname;
		std::string email;
		std::string mobile;
		std::string regTime;
		std::string regIp;
		std::string appid;
		uint32_t rescode;
		uint32_t usedTime;
		std::string extra;

		RegisterInfo():rescode(0),usedTime(0){};

		virtual void marshal(sox::Pack &pk) const
		{
			pk << uname << email << mobile << regTime << regIp << appid << rescode << usedTime;
			if( !extra.empty() )
			{
				pk << extra;
			}
		}

		virtual void unmarshal( const sox::Unpack &up)
		{
			up >> uname >> email >> mobile >> regTime >> regIp >> appid >> rescode >> usedTime;
			if( !up.empty() )
			{
				up >> extra;
			}
		}
	};

	struct UserInfo:public sox::Marshallable
	{
		enum{ uri = URI_USER_INFO};

		std::string interface;
		std::string ip;
		uint32_t total;
		uint32_t success;
		uint32_t ct;
		std::string extra;

		UserInfo():total(0),success(0),ct(0){};

		virtual void marshal(sox::Pack &pk) const
		{
			pk << interface << ip << total << success << ct;
			if( !extra.empty() )
			{
				pk << extra;
			}
		}

		virtual void unmarshal( const sox::Unpack &up)
		{
			up >> interface >> ip >> total >> success >> ct;
			if( !up.empty() )
			{
				up >> extra;
			}
		}
	};

	struct MibaoInfo:public sox::Marshallable
	{
		enum{ uri = URI_MIBAO};

        std::string interface;
        uint32_t ct;
        uint32_t rescode;
        std::string extra;

        MibaoInfo():ct(0),rescode(0){};

		virtual void marshal(sox::Pack &pk) const
		{
			pk << interface << ct << rescode;
			if( !extra.empty() )
			{
				pk << extra;
			}
		}

		virtual void unmarshal( const sox::Unpack &up)
		{
			up >> interface >> ct >> rescode;
			if( !up.empty() )
			{
				up >> extra;
			}
		}
    };

	struct KdcInfo:public sox::Marshallable
	{
        enum{ uri = URI_KDC};
        std::string type;
        uint32_t ct;
        uint32_t rescode;
        std::string extra;

        KdcInfo():ct(0),rescode(0){};

		virtual void marshal(sox::Pack &pk) const
        {
            pk << type << ct << rescode << extra;
        }

		virtual void unmarshal( const sox::Unpack &up)
        {
            up >> type >> ct >> rescode >> extra;
        }
    };

    struct Stat_Msg_From_AppSDK:public sox::Marshallable
    {
        enum {uri = 17 << 8 | 23};

	    std::string strAppid;
        uint32_t enctype;
        uint32_t key_version;
        uint32_t yyuid;
        uint32_t linkd_ip;
        uint32_t rescode;
        std::string extra;

        virtual void marshal(sox::Pack &pk) const
        {
            pk << strAppid << enctype << key_version << yyuid << linkd_ip << rescode << extra;
        }

        virtual void unmarshal(const sox::Unpack &up)
        {
            up >> strAppid >> enctype >> key_version >> yyuid >> linkd_ip >> rescode >> extra;
        }
    };

    // Key-Value
    struct SttKeyValue: public sox::Marshallable
    {
        std::string key;
        std::string value;

        SttKeyValue():key(""),value(""){}
        SttKeyValue( const std::string& _key, const std::string& _value):key(_key),value(_value){}

        bool operator<( const SttKeyValue& s) const
        {
            return s.key < key;
        }
        virtual void marshal(sox::Pack &pk) const
        {
            pk << key << value;
        }
        virtual void unmarshal( const sox::Unpack &up)
        {
            up >> key >> value;
        }
    };

    struct PSttReportMsg: public sox::Marshallable
    {
        enum {uri = (77 << 8) | protocol::SERVERLOGIN_SVID};

        std::string service;
        std::string type;
        std::set<SttKeyValue>  dataset;

        PSttReportMsg():service(""),type(""){}
        PSttReportMsg( std::string& _service, std::string& _type):service( _service),type( _type){}
        virtual void marshal(sox::Pack &pk) const
        {
            pk << service << type;
            sox::marshal_container( pk, dataset);
        }
        virtual void unmarshal( const sox::Unpack &up)
        {
            up >> service >> type;
            sox::unmarshal_container( up, std::inserter( dataset, dataset.begin()));
        }
        void set_service(const std::string& _service){ service = _service;}
        void set_type( const std::string& _type){ type = _type;}
        bool set( const std::string& key, const std::string& value)
        {
            SttKeyValue data( key, value);
            std::set<SttKeyValue>::iterator it = dataset.find( data);
            std::pair< std::set<SttKeyValue>::iterator, bool> rt;
            rt = dataset.insert( data);
            return rt.second;
        }
    };


}};
#endif
