#ifndef VERIFYPWD_I_H
#define VERIFYPWD_I_H

#include <unistd.h>
#include "common/core/base_svid.h"
#include "common/core/form.h"
#include "common/packet.h"
#include "server_login/common/resource/login_uri.h"

namespace verifypwd{

    struct VerifyPasswordReq: public sox::Marshallable
    {
        enum {uri = 101 << 8 | 233};

        std::string context;
        std::string signature;
        uint64_t yyuid;
        std::string user_name;
        std::string pwd;
        std::string extra;

        VerifyPasswordReq():context(""),signature(""),yyuid(0),user_name(""),pwd(""),extra(""){};

        virtual void marshal(sox::Pack &pk) const
        {
            pk << context << signature << yyuid << user_name << pwd << extra;
        }

        virtual void unmarshal(const sox::Unpack &up)
        {
            up >> context >> signature >> yyuid >> user_name >> pwd >> extra;
        }
    };
    struct VerifyPasswordRes: public sox::Marshallable
    {
        enum {uri = 102 << 8 | 233};

        std::string context;
        std::string signature;
        uint64_t yyuid;
        std::string user_name;
        uint32_t rescode;
        std::string strerror;

        VerifyPasswordRes():context(""),signature(""),yyuid(0),user_name(""),rescode(0),strerror(""){};

        virtual void marshal(sox::Pack &pk) const
        {
            pk << context << signature << yyuid << user_name << rescode << strerror;
        }

        virtual void unmarshal(const sox::Unpack &up)
        {
            up >> context >> signature >> yyuid >> user_name >> rescode >> strerror;
        }
    };

    struct PCheckBeginner : public sox::Marshallable
    {
    	enum {uri = 110 << 8 | 233};
    	uint32_t uid;
    	std::string passport;
    	std::string reserve;
    
    	virtual void marshal(sox::Pack &p) const
        {
    		p << uid << passport << reserve;
    	}
    	virtual void unmarshal(const sox::Unpack &p)
        {
    		p >> uid >> passport >> reserve;
    	}
    };
    
    struct PCheckBeginnerRes : public sox::Marshallable
    {
    	enum {uri = 111 << 8 | 233};
    	bool bIsBeginner;
    	std::string reserve;
    
    	virtual void marshal(sox::Pack &p) const
        {
    		p << bIsBeginner << reserve;
    	}
    	virtual void unmarshal(const sox::Unpack &p)
        {
    		p >> bIsBeginner >> reserve;
    	}
    };

	struct PClientMsg: public sox::Marshallable {
		enum {uri = (112 << 8 ) |233};
		PClientMsg(){};
		PClientMsg(uint32_t type, uint32_t uid, std::string extension):type(type), uid(uid), extension(extension){};
		uint32_t type; //0 for hiido 
		uint32_t uid;
		std::string extension; 

		virtual void marshal(sox::Pack &pk) const {
			pk << type << uid << extension ;
		}
		virtual void unmarshal(const sox::Unpack &up) {
			up >> type >> extension ;
		}
	};

	struct PClientMsgImp: public sox::Marshallable {
		enum {uri = (113 << 8 ) |233};
		PClientMsgImp(){};
		PClientMsgImp(uint32_t type, uint32_t uid, std::string extension):type(type), uid(uid), extension(extension){};
		uint32_t type; //0 for hiido 
		uint32_t uid;
		std::string extension; 

		virtual void marshal(sox::Pack &pk) const {
			pk << type << uid << extension ;
		}
		virtual void unmarshal(const sox::Unpack &up) {
			up >> type >> uid >> extension ;
		}
	};

	struct PAuthCode: public sox::Marshallable {
		enum {uri = (114 << 8 ) |233};
		PAuthCode():sPassport(""),uStatus(0){};
		PAuthCode(std::string passport, uint32_t status):sPassport(passport),uStatus(status){};
		std::string sPassport; 
		uint32_t uStatus;

		virtual void marshal(sox::Pack &pk) const {
			pk << sPassport << uStatus ;
		}
		virtual void unmarshal(const sox::Unpack &up) {
			up >> sPassport >> uStatus ;
		}
	};

	struct PAuthCodeEx: public sox::Marshallable {
		enum {uri = (115 << 8 ) |233};
		PAuthCodeEx():sUid(""),uStatus(0){};
		PAuthCodeEx(std::string uid, uint32_t status):sUid(uid),uStatus(status){};
		std::string sUid; 
		uint32_t uStatus;

		virtual void marshal(sox::Pack &pk) const {
			pk << sUid << uStatus ;
		}
		virtual void unmarshal(const sox::Unpack &up) {
			up >> sUid >> uStatus ;
		}
	};

	struct PTokenReq: public sox::Marshallable {
		enum {uri = (116 << 8 ) |233};
		PTokenReq():sContext(""),uSeq(0),uYyuid(0),uStatus(0){};
		PTokenReq(std::string context, uint32_t seq, uint32_t yyuid, uint32_t status):sContext(context),uSeq(seq),uYyuid(yyuid),uStatus(status){};
		std::string sContext; // 上下文
		uint32_t uSeq;        // seq
		uint32_t uYyuid;      // yyuid
		uint32_t uStatus;     // 状态,值为"0":表示置0;值为"1":表示置1;其他值均返回失败;

		virtual void marshal(sox::Pack &pk) const {
			pk << sContext << uSeq << uYyuid << uStatus ;
		}
		virtual void unmarshal(const sox::Unpack &up) {
			up >> sContext >> uSeq >> uYyuid >> uStatus ;
		}
	};

	struct PTokenRes: public sox::Marshallable {
		enum {uri = (117 << 8 ) |233};
		PTokenRes():sContext(""),uSeq(0),uRescode(0){};
		PTokenRes(std::string context, uint32_t seq, uint32_t rescode):sContext(context),uSeq(seq),uRescode(rescode){};
		std::string sContext; // 上下文
		uint32_t uSeq;        // seq
		uint32_t uRescode;    // // return: 0(success), 1(user_not_exist), 2(uhsproxy_fail), 3(error)

		virtual void marshal(sox::Pack &pk) const {
			pk << sContext << uSeq << uRescode;
		}
		virtual void unmarshal(const sox::Unpack &up) {
			up >> sContext >> uSeq >> uRescode;
		}
	};
	
	// add by fengqiang 2014-06-19
	// 查询user表中用户status字段请求
	struct QueryUserStatusReq : public sox::Marshallable {
	  enum{uri = LG_URI_VP_QUERY_USER_STATUS_ASK}; 
	  std::string _context;    // 消息序列号
	  uint64_t _yyuid;         // 用户标识
	  QueryUserStatusReq():_yyuid(0) {}
	  virtual void marshal(sox::Pack &pk) const
	  {
	  	pk << _context << _yyuid;
	  }
	  virtual void unmarshal(const sox::Unpack &up)
	  {
	  	up >> _context >> _yyuid;
	  }
	};
	struct QueryUserStatusRes : public sox::Marshallable {
	  enum{uri = LG_URI_VP_QUERY_USER_STATUS_ACK}; 
	  std::string _context;   // 消息序列号
	  uint32_t _rescode;      // 返回码 0-成功 1-参数无效 2-系统错误 3-用户不存在
	  uint32_t _status;       // 用户状态
	  std::string _desc;      // 失败原因
	  QueryUserStatusRes():_rescode(0),_status(0) {}
	  virtual void marshal(sox::Pack &pk) const
	  {
	  	pk << _context << _rescode << _status << _desc;
	  }
	  virtual void unmarshal(const sox::Unpack &up)
	  {
	  	up >> _context >> _rescode >> _status >>_desc;
	  }
	};

	// 封禁/解封web登录状态位
	struct SwitchWebLoginStatusReq : public sox::Marshallable {
	  enum{uri = LG_URI_VP_SWITCH_WEB_STATUS_ASK}; 
	  enum{
	  	SWITCH_TYPE_BLOCK = 0,   // 封禁用户web登录
	  	SWITCH_TYPE_UNBLOCK = 1  // 解封用户web登录
	  };
	  std::string _context;      // 消息序列号
	  uint64_t _yyuid;           // 用户标识
	  uint32_t _type;            // 用户操作类型，参考SWITCH_TYPE枚举
	  SwitchWebLoginStatusReq():_yyuid(0),_type(0) {}
	  virtual void marshal(sox::Pack &pk) const
	  {
	  	pk << _context << _yyuid << _type;
	  }
	  virtual void unmarshal(const sox::Unpack &up)
	  {
	  	up >> _context >> _yyuid >> _type;
	  }
	};
	struct SwitchWebLoginStatusRes : public sox::Marshallable {
	  enum{uri = LG_URI_VP_SWITCH_WEB_STATUS_ACK};   
	  std::string _context;   // 消息序列号
	  uint32_t _rescode;      // 返回码 0-成功 1-参数无效 2-系统错误 3-用户不存在
	  std::string _desc;      // 失败原因
	  SwitchWebLoginStatusRes():_rescode(0) {}
	  virtual void marshal(sox::Pack &pk) const
	  {
	  	pk << _context << _rescode << _desc;
	  }
	  virtual void unmarshal(const sox::Unpack &up)
	  {
	  	up >> _context >> _rescode >> _desc;
	  }
	};

};

#endif
