#pragma once

#include "int_types.h"
#include "core/base_svid.h"
#include "protocol/const.h"
#include "packet.h"

#include <string>

using ::std::string;

namespace server
{
	namespace AgentNotify
	{
		struct PSS_SysMsg4Peer: public sox::Marshallable
		{
			enum {uri = (16 << 8) | ::protocol::SYSMSGD_SVID};
	
			PSS_SysMsg4Peer()
			{
					m_uFromId = 0;
					m_uToId = 0;
					m_uExpireDate = 0;
					m_strMsg = "";
					m_bSaveOffline = false;
			}
	
			uint32_t m_uFromId;
			uint32_t m_uToId;
			uint32_t m_uExpireDate;
			std::string m_strMsg;
			bool m_bSaveOffline;
	
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uFromId << m_uToId << m_uExpireDate << m_strMsg << m_bSaveOffline;
			}
	
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uFromId >> m_uToId >> m_uExpireDate >> m_strMsg >> m_bSaveOffline;
			}
		};

		struct PSendSmsReq: public sox::Marshallable{
			enum {uri = (17 << 8) | ::protocol::NOTIFYSERVER_SVIDI};
			std::vector<std::string> vecMobiles;
			std::string sContent;
			uint32_t seq;
			PSendSmsReq()
			{
				vecMobiles.clear();
			}
			virtual void marshal(sox::Pack &p) const {
				marshal_container(p, vecMobiles);
				p << sContent << seq;
			}

			virtual void unmarshal(const sox::Unpack &p) {
				unmarshal_container(p, std::back_inserter(vecMobiles));
				p >> sContent >> seq;
			}
		};

		struct PSendEmailReq: public sox::Marshallable{
			enum {uri = (18 << 8) | ::protocol::NOTIFYSERVER_SVIDI};
			std::vector<std::string> vecEmails;
			std::string sTitle;
			std::string sContent;
			uint32_t seq;
			PSendEmailReq()
			{
				vecEmails.clear();
			}
			virtual void marshal(sox::Pack &p) const {
				marshal_container(p, vecEmails);
				p << sTitle << sContent << seq;
			}

			virtual void unmarshal(const sox::Unpack &p) {
				unmarshal_container(p, std::back_inserter(vecEmails));
				p >> sTitle >> sContent >> seq;
			}
		};

		struct PSendEmailExReq: public sox::Marshallable{
			enum {uri = (19 << 8) | ::protocol::NOTIFYSERVER_SVIDI};
			std::vector<std::string> vecEmails;
			std::string sFrom;
			std::string sSubject;
			std::string sContent;
			uint32_t seq;
			PSendEmailExReq()
			{
				vecEmails.clear();
			}
			virtual void marshal(sox::Pack &p) const {
				marshal_container(p, vecEmails);
				p << sFrom << sSubject << sContent << seq;
			}

			virtual void unmarshal(const sox::Unpack &p) {
				unmarshal_container(p, std::back_inserter(vecEmails));
				p >> sFrom >> sSubject >> sContent >> seq;
			}
		};

		// add by fengqiang 2014-9-22
		struct PSendPopWinReq: public sox::Marshallable{
			enum {uri = (20 << 8) | ::protocol::NOTIFYSERVER_SVIDI};
			std::vector<std::string> vecYYuids;
			std::string sContentJson;
			PSendPopWinReq() { 
			    vecYYuids.clear();
			}
			virtual void marshal(sox::Pack &p) const {
				marshal_container(p, vecYYuids);
				p << sContentJson;
			}

			virtual void unmarshal(const sox::Unpack &p) {
				unmarshal_container(p, std::back_inserter(vecYYuids));
				p >> sContentJson;
			}
		};

		struct PSendPopWinExReq: public sox::Marshallable{
			enum {uri = (21 << 8) | ::protocol::NOTIFYSERVER_SVIDI};
			std::vector<std::string> vecYYuids;
			std::string sContentJson;
			PSendPopWinExReq() { 
			    vecYYuids.clear();
			}
			virtual void marshal(sox::Pack &p) const {
				marshal_container(p, vecYYuids);
				p << sContentJson;
			}

			virtual void unmarshal(const sox::Unpack &p) {
				unmarshal_container(p, std::back_inserter(vecYYuids));
				p >> sContentJson;
			}
		};

		// add by fengqiang 2015-1-27
		struct PSendEmailSingleReq : public sox::Marshallable{
			enum {uri = (22 << 8) | ::protocol::NOTIFYSERVER_SVIDI};
			std::string _context;
			uint64_t    _yyuid;
			std::string _appid;     // business identifier
			std::string _from;      // email from
			std::string _dest;      // destination 
			std::string _subject;  
			std::string _content;   
			PSendEmailSingleReq() {}
			virtual void marshal(sox::Pack &p) const {
				p << _context << _yyuid << _appid << _from << _dest << _subject << _content;
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> _context >> _yyuid >> _appid >> _from >> _dest >> _subject >> _content;
			}
		};
     
    // add by fengqiang 2015-8-19
    // request of sending popwin safely through high-risk-moniter-way
    enum kVerPro {
      kVer1 = 1,
    };
    struct PSendPopWinP2PSafelyReq: public sox::Marshallable{
      enum {uri = (23 << 8) | ::protocol::NOTIFYSERVER_SVIDI};
      enum FormatMsg {
        kFormatXML = 1,
        kFormatJson,           // TODO  
      };
      uint32_t  _ver_pro;      // protocol version
      string    _context;
      string    _appid;        // popwin appid, default 906, used for udb
      uint64_t  _yyuid;        // user yyuid
      uint32_t  _bsaveoffline; // pop win when user up lime
      uint32_t  _expire_sec;   // popwin expire time, seconds since 1970
      uint32_t  _msg_format;   // message format, refer fo FormatMsg
      string    _msg;          // message content
      PSendPopWinP2PSafelyReq() : _ver_pro(kVer1), _appid("906"), _yyuid(0), 
          _bsaveoffline(1), _msg_format(kFormatXML) {
        _expire_sec = time(NULL) + 7 * 24 * 3600;
      }
      virtual void marshal(sox::Pack &p) const {
        if (_ver_pro == kVer1) {
          p << _ver_pro << _context <<_appid << _yyuid << _bsaveoffline 
            << _expire_sec << _msg_format <<_msg;
        }
      }
      virtual void unmarshal(const sox::Unpack &p) {
        p >> _ver_pro;
        if (_ver_pro == kVer1) {
          p >> _context >> _appid >> _yyuid >> _bsaveoffline >> _expire_sec 
            >> _msg_format >> _msg;
        }
      }
    };

	}
}
