#ifndef _login_safe_i_H_2012_3_29
#define _login_safe_i_H_2012_3_29

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
	enum {LGNSAFE_SVID = 120};
}

namespace server{ namespace loginsafe{
	enum EN_LOGSAFE_URI
	{
		URI_SAFECHECK_REQ = (1 << 8)|protocol::LGNSAFE_SVID,
		URI_SAFECHECK_RES = (2 << 8)|protocol::LGNSAFE_SVID
	};

    enum ER_LOGSAFE_FRO_TYPE
    {
        FT_SUCCESS = 0,
        FT_IPFRO_SHORT, // ip frozen: 10seconds
        FT_IPFRO_LONG,  // ip frozen: 120mins
        FT_IPFRO_USER,  // ip frozen: user not exist
        FT_IPFRO_LBS,   // ip frozen: push to lbs
        FT_ACCT_FAIL,   // acct frozen: auth fail
        FT_ACCT_LBS     // acct frozen: push to lbs
    };

	struct LoginSafeCheckReq: public sox::Marshallable
	{
		enum{uri =URI_SAFECHECK_REQ};
		std::string _context;
		std::string _from;
		std::string _uname;
		uint32_t    _ip;
		uint32_t    _rescode;
		uint32_t    _uid;
		uint32_t    _yyid;
		std::string _extra;
		std::string _pwd_sha1; // pwd after sha1
		LoginSafeCheckReq():_ip(0), _rescode(0), _uid(0), _yyid(0){}
		virtual void marshal(sox::Pack &pk) const
		{
			pk << _context << _from << _uname << _ip << _rescode << _uid << _yyid
				<< _extra << _pwd_sha1;
				/*
			if( !_extra.empty() )
			{
				pk << _extra;
			}
			*/
		}

		virtual void unmarshal(const sox::Unpack &up) 
		{
			up >> _context >> _from >> _uname >> _ip >> _rescode >> _uid >> _yyid;
			if( !up.empty() )
			{
				up >> _extra;
				if (!up.empty() )
				{
					up >> _pwd_sha1;
				}
			}
		}
	};

	struct LoginSafeCheckRes: public sox::Marshallable
	{
		enum{uri =URI_SAFECHECK_RES};
		std::string _context;
		uint32_t    _safe_type;
		uint32_t    _acct_try_times;
		uint32_t    _ip_try_times;
		uint32_t    _ip_err_times;
	        uint32_t    _pwd_times;
        	uint32_t    _safe_command_type;
		std::string _extra;
		LoginSafeCheckRes():_safe_type(0), _acct_try_times(0), _ip_try_times(0), _ip_err_times(0), _pwd_times(0), _safe_command_type(0){}
		virtual void marshal(sox::Pack &pk) const
		{
			pk << _context << _safe_type << _acct_try_times << _ip_try_times << _ip_err_times << _extra << _pwd_times << _safe_command_type;
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			up >> _context >> _safe_type >> _acct_try_times >> _ip_try_times >> _ip_err_times;
			if( !up.empty() )
			{
				up >> _extra;
			}
			if( !up.empty())
			{
				up >> _pwd_times;
			}
			if( !up.empty())
			{
				up >> _safe_command_type;
			}
		}
	};

}
}
#endif //_login_safe_i_H_2012_3_29







