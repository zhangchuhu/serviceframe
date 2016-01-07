#ifndef _P_NEW_PWD_DB_H_
#define _P_NEW_PWD_DB_H_

#include "int_types.h"
#include "core/base_svid.h"
#include "common/core/base_svid.h"
#include "protocol/const.h"
#include "packet.h"
#include <map>

using namespace std;

namespace protocol
{
	static const unsigned int NEW_PWD_DB_SVID = 178;
};

namespace server{
	namespace newpwddb
	{
		struct PCreateUserReq:  public sox::Marshallable{
			enum {uri = (1 << 8) | protocol::NEW_PWD_DB_SVID };			

			std::string strSeq;
			unsigned int uUid;	
			std::string strUsername; 
			std::string strPassword;
			std::string strEmail;
			std::string strRegTime;
			std::string strReqIp;
			std::string strAppid;
			std::string strLoginTime;
			std::string strLoginIp;
			std::string strLmodify;
			unsigned int uYyid;
			unsigned int uYyuid;
			
			virtual void marshal(sox::Pack &p) const {					
				p << strSeq << uUid << strUsername << strPassword << strEmail << strRegTime << strReqIp  << strAppid << strLoginTime << strLoginIp << strLmodify << uYyid << uYyuid;
			}

			virtual void unmarshal(const sox::Unpack &p){			
				p >> strSeq >> uUid >> strUsername >> strPassword >> strEmail >> strRegTime >> strReqIp  >> strAppid >> strLoginTime >> strLoginIp >> strLmodify >> uYyid >> uYyuid;
			}
		};			


		struct PCreateUserRes: public sox::Marshallable{						
			enum {uri = (2 << 8) | protocol::NEW_PWD_DB_SVID};			

			std::string strSeq;
			unsigned int uRes;  // 0 : 成功,1 ：失败，系统错误。
			std::string strAccount; 

			virtual void marshal(sox::Pack &p) const {					
				p << strSeq << uRes << strAccount;
			}

			virtual void unmarshal(const sox::Unpack &p){			
				p >> strSeq >> uRes >> strAccount;
			}
		};

		struct PUpdatePasswordReq: public sox::Marshallable{	
			enum {uri = (3 << 8) | protocol::NEW_PWD_DB_SVID};		
		
			std::string strSeq;
			std::string strAccount; // 可取username 、email
			std::string strPassword; // maybe encrypted if the length is larger than 40 bytes.
			std::string strClientIp; // in dot form.

			virtual void marshal(sox::Pack &p) const {					
				p << strSeq << strAccount << strPassword << strClientIp;
			}

			virtual void unmarshal(const sox::Unpack &p){			
				p >> strSeq >> strAccount >> strPassword;

				if ( false == p.empty() ){
					try{
							p >> strClientIp;
						}
						catch(...){
						}
				}
					
			}
		};

		struct PUpdatePasswordRes: public sox::Marshallable{	
			enum {uri = (4 << 8) | protocol::NEW_PWD_DB_SVID};		
		
			std::string strSeq;
			unsigned int uRes;  // 0 : 成功1 ：失败，系统错误。
			std::string strAccount;

			virtual void marshal(sox::Pack &p) const {					
				p << strSeq << uRes << strAccount;
			}

			virtual void unmarshal(const sox::Unpack &p){			
				p >> strSeq >> uRes >> strAccount;
			}
		};

		struct PChangePswRecord: public sox::Marshallable
		{
			enum {uri = (1 << 8) | 221};			

			uint64_t yyuid;
			uint32_t ChangePswTime;

			virtual void marshal(sox::Pack &pk) const
			{
				pk << yyuid << ChangePswTime;

			}
			virtual void unmarshal( const sox::Unpack &up)
			{
				up >> yyuid >> ChangePswTime;				
			}
		};


		struct PGetPasswordReq: public sox::Marshallable{	
			enum {uri = (5 << 8) | protocol::NEW_PWD_DB_SVID};	
		
			std::string strSeq;
			std::string strAccount;	// 可取username 、email

			virtual void marshal(sox::Pack &p) const {					
				p << strSeq << strAccount;
			}

			virtual void unmarshal(const sox::Unpack &p){			
				p >> strSeq >> strAccount;
			}
		};

		struct PGetPasswordRes: public sox::Marshallable{	
			enum {uri = (6 << 8) | protocol::NEW_PWD_DB_SVID};	
		
			std::string strSeq;
			unsigned int uRes;   // 0 : 成功,1 ：失败，系统错误，2：用户不存在
			std::string strAccount;
			std::string strPassword;

			virtual void marshal(sox::Pack &p) const {					
				p << strSeq << uRes << strAccount << strPassword;
			}

			virtual void unmarshal(const sox::Unpack &p){			
				p >> strSeq >> uRes >> strAccount >> strPassword;
			}
		};

		//for encrypt version
		struct PGetPasswordReq2: public sox::Marshallable{	
			enum {uri = (94 << 8) | protocol::NEW_PWD_DB_SVID};	
		
			std::string strSeq;
			std::string strAccount;	// 可取username 、email

			virtual void marshal(sox::Pack &p) const {					
				p << strSeq << strAccount;
			}

			virtual void unmarshal(const sox::Unpack &p){			
				p >> strSeq >> strAccount;
			}
		};
		struct PGetPasswordRes2: public sox::Marshallable{	
			enum {uri = (95 << 8) | protocol::NEW_PWD_DB_SVID};	
		
			std::string strSeq;
			unsigned int uRes;   // 0 : 成功,1 ：失败，系统错误，2：用户不存在
			std::string strAccount;
			std::string strPassword;

			virtual void marshal(sox::Pack &p) const {					
				p << strSeq << uRes << strAccount << strPassword;
			}

			virtual void unmarshal(const sox::Unpack &p){			
				p >> strSeq >> uRes >> strAccount >> strPassword;
			}
		};

		struct PHeartBeatReq: public sox::Marshallable
		{				
			enum {uri = 50<<8|110};	

			uint32_t  uVal;					

			PHeartBeatReq():uVal(0){}
			virtual void marshal(sox::Pack &p) const 
			{
				p << uVal;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> uVal;
			}
		};

		struct PHeartBeatRes: public sox::Marshallable
		{				
			enum {uri = 51<<8|110};	

			uint32_t uVal;	

			PHeartBeatRes():uVal(0){}
			virtual void marshal(sox::Pack &p) const 
			{
				p << uVal;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> uVal;
			}
		};
		
		//2012-03-05 add register interface
#define CREATEUSER_TIMEOUT 1000*8
#define GENYYID_TIMEOUT  1000*8
		// 生成UID的错误码
		/*
		enum E_GENUID_RES
		{ 
			// 成功                   ,超时               ,用户名已被使用
			E_GENUID_SUCCESS = 0,  E_GENUID_TIMEOUT,   E_GENUID_USED, 
			// 用户名空               ,服务失败           ,未知错误
			E_GENUI_UNAME_EMPTY,   E_GENUI_SERVER_FAIL, E_GENUID_UNKNOWN 
		};
		// 生成YYID的错误码
		enum E_GENYYID_RES
		{ 
			E_GENYYID_SUCCESS = 0, E_GENYYID_TIMEOUT,   E_GENYYID_SERVER_FAIL,
			E_GENYYID_UNKNOWN 
		};
		*/
		enum E_CREATEUSER_RES
		{
			E_SUCCESS= 0, E_CREATEUSER_BADNAME = 1, E_SYS_ERROR = 7, E_USER_EXIST = 14, E_EMAIL_EXIST = 16, E_GENUID_TIMEOUT = 17,
		   	E_GENUID_SERVER_FAIL = 18,  E_GENNUM_TIMEOUT = 19, E_GENNUM_SERVER_FAIL = 20, E_GENYYID_TIMEOUT = 21, E_GENYYID_SERVER_FAIL = 22
			
		};

		struct PGenYyuidReq: public sox::Marshallable
		{
			enum{ uri = (35 << 8)|178 };
			std::string  _seq;      // 消息序列，格式:YYYYMMDDHH24MISS_xxxx.yyyy，其中xxxx为数字序列，yyyy为host编码
			std::string  _uname;
			std::string  _passwd;
			std::string  _email;

			PGenYyuidReq(){}
			PGenYyuidReq( const std::string &seq, const std::string &uname, const std::string &passwd, 
				const std::string &email )
				:_seq( seq )
				,_uname(uname)
				,_passwd(passwd)
				,_email(email)
			{}

			virtual void marshal(sox::Pack &pk) const 
			{
				pk << _seq << _uname << _passwd << _email;
			}

			virtual void unmarshal(const sox::Unpack &up) 
			{
				up >> _seq >> _uname >> _passwd >> _email;
			}
		};

		struct PGenYyuidRes: public sox::Marshallable
		{
			enum{ uri = (36 << 8)|178 };
			std::string  _seq;      // 消息序列，格式:YYYYMMDDHH24MISS_xxxx.yyyy，其中xxxx为数字序列，yyyy为host编码
			std::string  _uname;
			uint32_t     _yyuid;
			uint32_t     _resCode;

			PGenYyuidRes(): _yyuid(0), _resCode(0){}
			PGenYyuidRes( const std::string &seq, 
				const std::string &uname, 
				const uint32_t yyuid, 
				const uint32_t resCode ):
			_seq(seq), _uname(uname), _yyuid(yyuid), _resCode(resCode){}

			virtual void marshal(sox::Pack &pk) const 
			{
				pk << _seq << _uname << _yyuid << _resCode;
			}

			virtual void unmarshal(const sox::Unpack &up) 
			{
				up >> _seq >> _uname >> _yyuid >> _resCode;
			}
		};

		struct PGenYyidReq: public sox::Marshallable
		{
			enum{ uri = (37 << 8)|178 };
			std::string  _seq;      // 消息序列，格式:YYYYMMDDHH24MISS_xxxx.yyyy，其中xxxx为数字序列，yyyy为host编码
			uint32_t     _yyuid;

			PGenYyidReq():_yyuid(0){}
			PGenYyidReq( const std::string &seq, const uint32_t yyuid ):_seq(seq), _yyuid(yyuid){}

			virtual void marshal(sox::Pack &pk) const 
			{
				pk << _seq << _yyuid;
			}

			virtual void unmarshal(const sox::Unpack &up) 
			{
				up >> _seq >> _yyuid;
			}
		};

		struct PGenYyidRes: public sox::Marshallable
		{
			enum{ uri = (38 << 8)|178 };
			std::string  _seq;      // 消息序列，格式:YYYYMMDDHH24MISS_xxxx.yyyy，其中xxxx为数字序列，yyyy为host编码
			uint32_t     _yyuid;
			uint32_t     _yyid;
			uint32_t     _resCode;

			PGenYyidRes():_yyuid(0), _yyid(0), _resCode(0){}
			PGenYyidRes( const std::string &seq, const uint32_t yyuid, 
				const uint32_t yyid, const uint32_t resCode ):
			_seq(seq), _yyuid(yyuid), _yyid(yyid), _resCode(resCode){}

			virtual void marshal(sox::Pack &pk) const 
			{
				pk << _seq << _yyuid << _yyid << _resCode;
			}

			virtual void unmarshal(const sox::Unpack &up) 
			{
				up >> _seq >> _yyuid >> _yyid >> _resCode;
			}
		};

		// 生成yyid，+username
		struct PGenYyidWithUnameReq: public sox::Marshallable
		{
			enum{ uri = (43 << 8)|178 };
			std::string  _seq;      // 消息序列，格式:YYYYMMDDHH24MISS_xxxx.yyyy，其中xxxx为数字序列，yyyy为host编码
			uint32_t     _yyuid;
			std::string  _username;

			PGenYyidWithUnameReq():_yyuid(0){}
			PGenYyidWithUnameReq( const std::string &seq, const uint32_t yyuid, const std::string uname )
				:_seq(seq), _yyuid(yyuid), _username(uname){}

			virtual void marshal(sox::Pack &pk) const 
			{
				pk << _seq << _yyuid << _username;
			}

			virtual void unmarshal(const sox::Unpack &up) 
			{
				up >> _seq >> _yyuid >> _username;
			}
		};


		//new create user req
		struct PNewCreateUserReq : public sox::Marshallable
		{
			enum {uri = (44 << 8) | 178};

			std::string strSeq;
			std::string strUsername;
			std::string strEmail;
			std::string strMobile;
			std::string strPassword;
			std::string strRegTime;
			std::string strRegIp;
			std::string strAppid;
			uint32_t yyuid;
			uint32_t yyid;

			virtual void marshal(sox::Pack &pk) const
			{
				pk << strSeq << strUsername << strEmail << strMobile << strPassword << strRegTime << strRegIp << strAppid << yyuid << yyid;
			}

			virtual void unmarshal(const sox::Unpack &up)
			{
				up >> strSeq >> strUsername >> strEmail >> strMobile >> strPassword >> strRegTime >> strRegIp >> strAppid >> yyuid >> yyid;	
			}
		};


		struct PNewCreateUserRes : public sox::Marshallable
		{
			enum {uri = (45 << 8) | 178};
		
			std::string strSeq;
			uint32_t resCode;
			uint32_t yyuid;
			uint32_t yyid;
			uint32_t uid;

			virtual void marshal(sox::Pack &pk) const
			{
				pk << strSeq << resCode << yyuid << yyid << uid;	
			}

			virtual void unmarshal(const sox::Unpack &up)
			{
				up >> strSeq >> resCode >> yyuid >> yyid >> uid;
			}
		};
		//for the encrypt version
		struct PNewCreateUserReq2 : public sox::Marshallable
		{
			enum {uri = (90 << 8) | 178};

			std::string strSeq;
			std::string strUsername;
			std::string strEmail;
			std::string strMobile;
			std::string strPassword;
			std::string strRegTime;
			std::string strRegIp;
			std::string strAppid;
			uint32_t yyuid;
			uint32_t yyid;

			virtual void marshal(sox::Pack &pk) const
			{
				pk << strSeq << strUsername << strEmail << strMobile << strPassword << strRegTime << strRegIp << strAppid << yyuid << yyid;
			}

			virtual void unmarshal(const sox::Unpack &up)
			{
				up >> strSeq >> strUsername >> strEmail >> strMobile >> strPassword >> strRegTime >> strRegIp >> strAppid >> yyuid >> yyid;	
			}
		};


		struct PCreateGameUserReq : public sox::Marshallable {
			enum {uri = (100 << 8) | 178};
			
			std::string strSeq;
			std::string strUsername;
			std::string strEmail;
			std::string strMobile;
			std::string strPassword;
			std::string strRegTime;
			std::string strRegIp;
			std::string strAppid;
			uint32_t yyuid;
			uint32_t uid;

			virtual void marshal(sox::Pack &pk) const
			{
				pk << strSeq << strUsername << strEmail << strMobile << strPassword << strRegTime << strRegIp << strAppid << yyuid << uid;
			}

			virtual void unmarshal(const sox::Unpack &up)
			{
				up >> strSeq >> strUsername >> strEmail >> strMobile >> strPassword >> strRegTime >> strRegIp >> strAppid >> yyuid >> uid;	
			}
		};




		struct PCreateGameUserRes : public sox::Marshallable
		{
			enum {uri = (101 << 8) | 178};
		
			std::string strSeq;
			uint32_t resCode;
			uint32_t yyuid;
			uint32_t yyid;
			uint32_t uid;

			virtual void marshal(sox::Pack &pk) const
			{
				pk << strSeq << resCode << yyuid << yyid << uid;	
			}

			virtual void unmarshal(const sox::Unpack &up)
			{
				up >> strSeq >> resCode >> yyuid >> yyid >> uid;
			}
		};


		struct PPartnerCreateUserReq : public sox::Marshallable {
			enum {uri = (102 << 8) | 178};

			std::string strSeq;
			std::string strPartner;
			std::string strEmail;
			std::string strMobile;
			std::string strPassword;
			std::string strRegIp;
			std::string strAppid;
			std::string strExtension;

			virtual void marshal(sox::Pack &pk) const {
				pk << strSeq << strPartner << strEmail << strMobile << strPassword << strRegIp << strAppid << strExtension;	
			}

			virtual void unmarshal(const sox::Unpack &up) {
				up >> strSeq >> strPartner >> strEmail >> strMobile >> strPassword >> strRegIp >> strAppid >> strExtension;	
			}

		};


		struct PPartnerCreateUserRes: public sox::Marshallable {
			enum {uri = (103 << 8) | 178};

			std::string strSeq;
			uint32_t resCode;
			uint32_t yyuid;
			uint32_t yyid;
			uint32_t uid;
			std::string strUsername;
			std::string strExtension;

			virtual void marshal(sox::Pack &pk) const {
				pk << strSeq << resCode << yyuid << yyid << uid << strUsername << strExtension;	
			}

			virtual void unmarshal(const sox::Unpack &up) {
				up >> strSeq >> resCode >> yyuid >> yyid >> uid >> strUsername >> strExtension;	
			}
		};


		struct PCheckUserExistReq : public sox::Marshallable
		{
			enum {uri = (46 << 8) | 178};

			std::string strSeq;
			uint32_t type;
			std::string query_value;

			virtual void marshal(sox::Pack &pk) const
			{
				pk << strSeq << type << query_value;	
			}

			virtual void unmarshal(const sox::Unpack &up)
			{
				up >> strSeq >> type >> query_value;	
			}
		};


		struct PCheckUserExistRes : public sox::Marshallable
		{
			enum {uri = (47 << 8) | 178};

			std::string strSeq;
			uint32_t resCode;

			virtual void marshal(sox::Pack &pk) const
			{
				pk << strSeq << resCode;	
			}


			virtual void unmarshal(const sox::Unpack &up)
			{
				up >> strSeq >> resCode;	
			}
		};

		//for encrypt version
		struct PCheckUserExistReq2 : public sox::Marshallable
		{
			enum {uri = (92 << 8) | 178};

			std::string strSeq;
			uint32_t type;
			std::string query_value;

			virtual void marshal(sox::Pack &pk) const
			{
				pk << strSeq << type << query_value;	
			}

			virtual void unmarshal(const sox::Unpack &up)
			{
				up >> strSeq >> type >> query_value;	
			}
		};


		struct PGetVerifiedCodeReq : public sox::Marshallable {
			enum {uri = (106 << 8) | 178};

			std::string strSeq;   
			std::string strMobile; 
			std::string strSrvId; 
			uint32_t	uiType;	  //1:reg 2:pwd
			std::string	strExtra;	

			virtual void marshal(sox::Pack &pk) const {
				pk << strSeq << strMobile << strSrvId << uiType << strExtra;	
			}

			virtual void unmarshal(const sox::Unpack &up) {
				up >> strSeq >> strMobile >> strSrvId >> uiType >> strExtra;	
			}
		};

		struct PGetVerifiedCodeRes : public sox::Marshallable
		{
			enum {uri = (107 << 8) | 178};

			std::string strSeq;
			uint32_t	resCode;
			std::string strExtra;

			virtual void marshal(sox::Pack &pk) const {
				pk << strSeq << resCode << strExtra;	
			}

			virtual void unmarshal(const sox::Unpack &up) {
				up >> strSeq >> resCode >> strExtra;	
			}
		};


		struct PMobRegReq : public sox::Marshallable
		{
			enum {uri = (108 << 8) | 178};

			std::string strSeq;
			std::string strMobile;
			std::string strVcode;
			std::string strPasswd;
			std::string strRegIp;
			std::string strSrvId;
			uint32_t	yyuid;
			std::string	strExtra;

			virtual void marshal(sox::Pack &pk) const {
				pk << strSeq << strMobile << strVcode << strPasswd << strRegIp << strSrvId << yyuid << strExtra;	
			}

			virtual void unmarshal(const sox::Unpack &up) {
				up >> strSeq >> strMobile >> strVcode >> strPasswd >> strRegIp >> strSrvId >> yyuid >> strExtra;
			}
			
		};

		struct PMobRegRes : public sox::Marshallable 
		{
			enum {uri = (109 << 8) | 178};
			
			std::string strSeq;
			uint32_t 	resCode;
			std::string	strUsername;
			uint32_t	yyid;
			uint32_t	yyuid;
			std::string strExtra;

			virtual void marshal(sox::Pack &pk) const {
				pk << strSeq << resCode << strUsername << yyid << yyuid << strExtra;	
			}

			virtual void unmarshal(const sox::Unpack &up) {
				up >> strSeq >> resCode >> strUsername >> yyid >> yyuid >> strExtra;	
			}
		};

		struct PMobModifyPwdReq : public sox::Marshallable 
		{
			enum {uri = (110 << 8) |178};

			std::string strSeq;
			std::string strMobile;
			std::string strVcode;
			std::string strSrvId;
			std::string strPasswd;

			virtual void marshal(sox::Pack &pk) const {
				pk << strSeq << strMobile << strVcode << strSrvId << strPasswd;	
			}

			virtual void unmarshal(const sox::Unpack &up) {
				up >> strSeq >> strMobile >> strVcode >> strSrvId >> strPasswd;
			}
		};

		struct PMobModifyPwdRes : public sox::Marshallable
		{
			enum {uri = (111 << 8) | 178};

			std::string strSeq;					 
			std::string strMobile;
			uint32_t 	resCode;
	
			virtual void marshal(sox::Pack &pk) const {
				pk << strSeq << strMobile << resCode;	
			}

			virtual void unmarshal(const sox::Unpack &up) {
				up >> strSeq >> strMobile >> resCode;	
			}

		};

		struct PNewMobModifyPwdReq : public sox::Marshallable 
		{
			enum {uri = (129 << 8) |178};

			std::string strSeq;
			std::string strMobile;
			std::string strVcode;
			std::string strAppid;
			std::string strAppkey;
			std::string strSha1Pwd;
			std::string strClientIp;

			virtual void marshal(sox::Pack &pk) const {
				pk << strSeq << strMobile << strVcode << strAppid << strAppkey << strSha1Pwd<< strClientIp;	
			}

			virtual void unmarshal(const sox::Unpack &up) {
				up >> strSeq >> strMobile >> strVcode >> strAppid >> strAppkey >> strSha1Pwd >> strClientIp;
			}
		};

		struct PNewMobModifyPwdRes : public sox::Marshallable
		{
			enum {uri = (130 << 8) | 178};

			std::string strSeq;					 
			std::string strMobile;
			uint32_t 	resCode;
	
			virtual void marshal(sox::Pack &pk) const {
				pk << strSeq << strMobile << resCode;	
			}

			virtual void unmarshal(const sox::Unpack &up) {
				up >> strSeq >> strMobile >> resCode;	
			}

		};


		///////////////////////////////////////////////////////////////////////////////////////
		//reg msg for suchuhui
		struct PBuiltInRegReq : public sox::Marshallable
		{
			enum {uri = (112 << 8) | 178};

			std::string strSeq;
			std::string strUsername;
			std::string strPasswd;
			std::string strPicCode; //code to verify
			std::string	strInternalId; //client created
			std::string strRegIp;  
			std::string strAppid;  //indicate the product 
			uint32_t	nRand; //client create

			virtual void marshal(sox::Pack &pk)	const {
				pk << strSeq << strUsername << strPasswd << strPicCode << strInternalId << strRegIp << strAppid << nRand; 
			}

			virtual void unmarshal(const sox::Unpack &up) {
				up >> strSeq >> strUsername >> strPasswd >> strPicCode >> strInternalId >> strRegIp >> strAppid >> nRand;
			}

		};

		struct PBuiltInRegRes : public sox::Marshallable
		{
			enum {uri = (113 << 8) | 178};

			std::string strSeq;
			uint32_t resCode;   //0: success  //2:code incorrect //7: system error //14: username existed
			std::string strUsername;
			uint32_t yyid;
			std::string strInternalId;
			std::string strImg;  //if resCode is 2, the data valid;

			virtual void marshal(sox::Pack &pk) const {
				pk << strSeq << resCode << strUsername << yyid << strInternalId << strImg;	
			}

			virtual void unmarshal(const sox::Unpack &up) {
				up >> strSeq >> resCode >> strUsername >> yyid >> strInternalId >> strImg;	
			}

		};

		///////////////////////////////////////////////////////////////////////////////////////////////////
		//for one key regist.msg from sms_bkd to registerd
		struct POneKeyRegPreSaveReq : public sox::Marshallable
		{
			enum {uri = (114 << 8)|178};

			std::string strMobile;
			std::string strContent;

			virtual void marshal(sox::Pack &pk) const {
				pk << strMobile << strContent;		
			}

			virtual void unmarshal(const sox::Unpack &up) {
				up >> strMobile >> strContent;		
			}

		};

		/////////////////////////////////////////////////////////
		//one key regist
		struct POneKeyRegReq : public sox::Marshallable	
		{
			enum {uri = (115 << 8) | 178};	

			std::string strSeq;
			std::string strUuid;
			uint64_t	yyuid;
			std::string strServiceId;
			std::string strRegIp;
			std::string strTimestamp;

			virtual void marshal(sox::Pack &pk) const {
				pk << strSeq << strUuid << yyuid << strServiceId << strRegIp << strTimestamp;
			}

			virtual void unmarshal(const sox::Unpack &up) {
				up >> strSeq >> strUuid >> yyuid >> strServiceId >> strRegIp >> strTimestamp;
			}

		};

		struct POneKeyRegRes : public sox::Marshallable
		{
			enum {uri = (116 << 8) | 178};	

			std::string strSeq;
			uint32_t resCode; //0:success 1:uuid not found 2:uuid time out 3:yyuid existed 7:system error 14:user existed
			std::string strUsername;
			std::string strPasswd;
			uint64_t yyuid;
			uint64_t yyid;
			uint64_t uid;

			virtual void marshal(sox::Pack &pk) const {
				pk << strSeq << resCode << strUsername << strPasswd << yyuid << yyid << uid;	
			}

			virtual void unmarshal(const sox::Unpack &up) {
				up >> strSeq >> resCode >> strUsername >> strPasswd >> yyuid >> yyid >> uid;	
			}

		};

		struct PBindYyuidMobileReq : public sox::Marshallable	
		{
			enum {uri = (117 << 8) | 178};	

			std::string strContext;
			uint32_t    yyuid;
			std::string strMobile;

			virtual void marshal(sox::Pack &pk) const {
				pk << strContext << yyuid << strMobile;
			}

			virtual void unmarshal(const sox::Unpack &up) {				
				up >> strContext >> yyuid >> strMobile;
			}

		};

		struct PBindYyuidMobileRes : public sox::Marshallable	
		{
			enum {uri = (118 << 8) | 178};	

			std::string strContext;
			uint32_t    yyuid;
			uint32_t    res;		// 0	-	success.
							// 1	-	yyuid is not existing.
							// 2	-	system error as "db failure"

			virtual void marshal(sox::Pack &pk) const {
				pk << strContext << yyuid << res;
			}

			virtual void unmarshal(const sox::Unpack &up) {				
				up >> strContext >> yyuid >> res;
			}

		};

		struct PUpdatePasswordPiccodeReq: public sox::Marshallable{	
			enum {uri = (120 << 8) | protocol::NEW_PWD_DB_SVID};		
		
			std::string strSeq;
			std::string strAccount; // 可取username 、email
			std::string strOldPassword; // maybe encrypted if the length is larger than 40 bytes.
			std::string strNewPassword;
			uint32_t uYyuid;
			std::string strPiccodeId;	
			std::string strPiccode;		// piccode ,it's 6 bytes in size.	
			std::string strClientIp; 	// 

			virtual void marshal(sox::Pack &p) const {					
				p << strSeq << strAccount << strOldPassword << strNewPassword << uYyuid << strPiccodeId << strPiccode << strClientIp;
			}

			virtual void unmarshal(const sox::Unpack &p){			
				p >> strSeq >> strAccount >> strOldPassword >> strNewPassword >> uYyuid >> strPiccodeId >> strPiccode >> strClientIp;					
			}
		};

		struct PUpdatePasswordPiccodeRes: public sox::Marshallable{	
			enum {uri = (121 << 8) | protocol::NEW_PWD_DB_SVID};		
		
			std::string strSeq;
			unsigned int uRes;  // 0 : 成功1 ：失败，系统错误。			
			std::string strPiccodeId;
			std::string strPiccodeImg; // an image file.

			virtual void marshal(sox::Pack &p) const {					
				p << strSeq << uRes << strPiccodeId << strPiccodeImg;
			}

			virtual void unmarshal(const sox::Unpack &p){			
				p >> strSeq >> uRes >> strPiccodeId >> strPiccodeImg;
			}
		};


		struct PNewGetVerifiedCodeReq : public sox::Marshallable {
			enum {uri = (122 << 8) | 178};

			std::string strSeq;   
			std::string strMobile; 
			std::string strSrvId; 
			uint32_t	uiType;	  //1:reg 2:pwd
			std::string	strExtra;	

			virtual void marshal(sox::Pack &pk) const {
				pk << strSeq << strMobile << strSrvId << uiType << strExtra;	
			}

			virtual void unmarshal(const sox::Unpack &up) {
				up >> strSeq >> strMobile >> strSrvId >> uiType >> strExtra;	
			}
		};

		struct PNewGetVerifiedCodeRes : public sox::Marshallable
		{
			enum {uri = (123<< 8) | 178};

			std::string strSeq;
			uint32_t	resCode;
			std::string strExtra;

			virtual void marshal(sox::Pack &pk) const {
				pk << strSeq << resCode << strExtra;	
			}

			virtual void unmarshal(const sox::Unpack &up) {
				up >> strSeq >> resCode >> strExtra;	
			}
		};


		struct PNewMobRegReq : public sox::Marshallable
		{
			enum {uri = (124 << 8) | 178};

			std::string strSeq;
			std::string strMobile;
			std::string strVcode;
			std::string strPasswd;
			std::string strRegIp;
			std::string strSrvId;
			uint32_t	yyuid;
			std::string	strExtra;

			virtual void marshal(sox::Pack &pk) const {
				pk << strSeq << strMobile << strVcode << strPasswd << strRegIp << strSrvId << yyuid << strExtra;	
			}

			virtual void unmarshal(const sox::Unpack &up) {
				up >> strSeq >> strMobile >> strVcode >> strPasswd >> strRegIp >> strSrvId >> yyuid >> strExtra;
			}
			
		};

		struct PNewMobRegRes : public sox::Marshallable 
		{
			enum {uri = (125 << 8) | 178};
			
			std::string strSeq;
			uint32_t 	resCode;
			std::string	strUsername;
			uint32_t	yyid;
			uint32_t	yyuid;
			std::string strExtra;

			virtual void marshal(sox::Pack &pk) const {
				pk << strSeq << resCode << strUsername << yyid << yyuid << strExtra;	
			}

			virtual void unmarshal(const sox::Unpack &up) {
				up >> strSeq >> resCode >> strUsername >> yyid >> yyuid >> strExtra;	
			}
		};

		struct PGetEmailVerifiedCodeReq : public sox::Marshallable {
			enum {uri = (131 << 8) | 178};

			std::string strSeq;   
			std::string strEmail; 
			std::string strSrvId; 
			uint32_t	uiType;	  //1:reg 2:pwd
			std::string	strExtra;	

			virtual void marshal(sox::Pack &pk) const {
				pk << strSeq << strEmail << strSrvId << uiType << strExtra;	
			}

			virtual void unmarshal(const sox::Unpack &up) {
				up >> strSeq >> strEmail >> strSrvId >> uiType >> strExtra;	
			}
		};

		struct PGetEmailVerifiedCodeRes : public sox::Marshallable
		{
			enum {uri = (132 << 8) | 178};

			std::string strSeq;
			uint32_t	resCode;
			std::string strExtra;

			virtual void marshal(sox::Pack &pk) const {
				pk << strSeq << resCode << strExtra;	
			}

			virtual void unmarshal(const sox::Unpack &up) {
				up >> strSeq >> resCode >> strExtra;	
			}
		};


		struct PEmailRegReq : public sox::Marshallable
		{
			enum {uri = (133 << 8) | 178};

			std::string strSeq;
			std::string strEmail;
			std::string strVcode;
			std::string strSha1Pwd;
			std::string strRegIp;
			std::string strAppid;
			std::string strAppkey;
			uint32_t	yyuid;

			virtual void marshal(sox::Pack &pk) const {
				pk << strSeq << strEmail << strVcode << strSha1Pwd << strRegIp << strAppid << strAppkey << yyuid;	
			}

			virtual void unmarshal(const sox::Unpack &up) {
				up >> strSeq >> strEmail >> strVcode >> strSha1Pwd >> strRegIp >> strAppid >> strAppkey >> yyuid;
			}
			
		};

		struct PEmailRegRes : public sox::Marshallable 
		{
			enum {uri = (134<< 8) | 178};
			
			std::string strSeq;
			uint32_t 	resCode;
			std::string	strUsername;
			uint32_t	yyid;
			uint32_t	yyuid;

			virtual void marshal(sox::Pack &pk) const {
				pk << strSeq << resCode << strUsername << yyid << yyuid;	
			}

			virtual void unmarshal(const sox::Unpack &up) {
				up >> strSeq >> resCode >> strUsername >> yyid >> yyuid;	
			}
		};

		struct PEmailModifyPwdReq : public sox::Marshallable 
		{
			enum {uri = (135 << 8) |178};

			std::string strSeq;
			std::string strEmail;
			std::string strVcode;
			std::string strAppid;
			std::string strAppkey;
			std::string strSha1Pwd;
			std::string strClientIp;

			virtual void marshal(sox::Pack &pk) const {
				pk << strSeq << strEmail << strVcode << strAppid << strAppkey << strSha1Pwd << strClientIp;	
			}

			virtual void unmarshal(const sox::Unpack &up) {
				up >> strSeq >> strEmail >> strVcode >> strAppid >> strAppkey >> strSha1Pwd >> strClientIp;
			}
		};

		struct PEmailModifyPwdRes : public sox::Marshallable
		{
			enum {uri = (136 << 8) | 178};

			std::string strSeq;					 
			std::string strEmail;
			uint32_t 	resCode;
	
			virtual void marshal(sox::Pack &pk) const {
				pk << strSeq << strEmail << resCode;	
			}

			virtual void unmarshal(const sox::Unpack &up) {
				up >> strSeq >> strEmail >> resCode;	
			}

		};

		struct PNewMobRegReq2 : public sox::Marshallable
		{
			enum {uri = (137 << 8) | 178};

			std::string strSeq;
			std::string strMobile;
			std::string strVcode;
			std::string strSha1Pwd;
			std::string strRegIp;
			std::string strAppid;
			std::string strAppkey;
			uint32_t	yyuid;

			virtual void marshal(sox::Pack &pk) const {
				pk << strSeq << strMobile << strVcode << strSha1Pwd << strRegIp << strAppid << strAppkey << yyuid;	
			}

			virtual void unmarshal(const sox::Unpack &up) {
				up >> strSeq >> strMobile >> strVcode >> strSha1Pwd >> strRegIp >> strAppid >> strAppkey >> yyuid;
			}
			
		};

		struct PNewMobRegRes2 : public sox::Marshallable 
		{
			enum {uri = (138<< 8) | 178};
			
			std::string strSeq;
			uint32_t 	resCode;
			std::string	strUsername;
			uint32_t	yyid;
			uint32_t	yyuid;

			virtual void marshal(sox::Pack &pk) const {
				pk << strSeq << resCode << strUsername << yyid << yyuid;	
			}

			virtual void unmarshal(const sox::Unpack &up) {
				up >> strSeq >> resCode >> strUsername >> yyid >> yyuid;	
			}
		};

		struct PUpdatePasswordYYuidReq: public sox::Marshallable{
			enum {uri = (141 << 8) | protocol::NEW_PWD_DB_SVID};

			std::string strSeq;
			uint32_t yyuid; 
			std::string strPassword; // maybe encrypted if the length is larger than 40 bytes.
			std::string strClientIp; // in dot form.

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << yyuid << strPassword << strClientIp;
			}

			virtual void unmarshal(const sox::Unpack &p){
				p >> strSeq >> yyuid >> strPassword >> strClientIp;

			}
		};


		struct PUpdatePasswordYYuidRes: public sox::Marshallable{
			enum {uri = (142 << 8) | protocol::NEW_PWD_DB_SVID};

			std::string strSeq;
			unsigned int uRes;  // 0 : 成功1 ：失败，系统错误。
			uint32_t yyuid;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << uRes << yyuid;
			}

			virtual void unmarshal(const sox::Unpack &p){
				p >> strSeq >> uRes >> yyuid;
			}
		};

		/* 手机用户设备号申请注册请求 */
		struct PMobCheckRegReq : public sox::Marshallable {
		      enum {uri = (147 << 8) | 178};
		
		      std::string strSeq;
		      std::string strMobile;
		      std::string strDevice;
					std::string strAppid;
					uint32_t appType; //客户端设备类型android 或 ios
					std::string sdkVerStr; //客户端版本
					std::string protocolVer; //协议版本号
		
		      virtual void marshal(sox::Pack &p) const {
		      	p << strSeq << strMobile << strDevice << strAppid << appType << sdkVerStr << protocolVer;
		      	
		      	
		      }
		
		      virtual void unmarshal(const sox::Unpack &p) {
		      	p >> strSeq >> strMobile >> strDevice >> strAppid >> appType >> sdkVerStr >> protocolVer;
		      }
		};
		
		struct PMobCheckRegRes : public sox::Marshallable {
		      enum {uri = (148 << 8) | 178};
		
		      std::string strSeq;
		      uint32_t 	resCode;     //0--表示该手机还未注册且验证码发送成功；7--系统错误（超时，数据库，发送验证码服务等）；14--该手机用户已注册
		      std::string failDesc;  //错误描述
		      std::string strCallBack;  //序列化加密后Tauth_info，用于下一步发起注册请求时带回来进行验证
		
		      virtual void marshal(sox::Pack &p) const {
		      	p << strSeq << resCode << failDesc << strCallBack;
		      }
		
		      virtual void unmarshal(const sox::Unpack &p) {
		      	p >> strSeq >> resCode >> failDesc >> strCallBack;
		      }
		};
		
		
		/* 手机用户设备号注册请求 */
		struct PMobDeviceRegisterReq : public sox::Marshallable {
		      enum {uri = (149 << 8) | 178};
		
		      std::string strSeq;
		      std::string strMobile;
		      std::string strSha1Pwd;
		      std::string strSmsCode;
		      std::string strDevice;
					std::string strAppid;
					uint32_t appType; //客户端设备类型android 或 ios
					std::string sdkVerStr; //客户端版本
					std::string protocolVer; //协议版本号
					std::string strCallBack;
		
		      virtual void marshal(sox::Pack &p) const {
		      	p << strSeq << strMobile << strSha1Pwd << strSmsCode << strDevice << strAppid << appType << sdkVerStr << protocolVer << strCallBack;
		      }
		
		      virtual void unmarshal(const sox::Unpack &p) {
		      	p >> strSeq >> strMobile >> strSha1Pwd >> strSmsCode >> strDevice >> strAppid >> appType >> sdkVerStr >> protocolVer >> strCallBack;
		      }
		};
		
		struct PMobDeviceRegisterRes : public sox::Marshallable {
		      enum {uri = (150 << 8) | 178};
		      
		      std::string strSeq;
					uint32_t 	resCode; //0--表示注册成功；7--系统错误（数据库，验证码服务等不可用）；14--该手机用户已注册
					std::string failDesc;  //错误描述
					std::string	strUsername;
					uint64_t	yyid;
					uint64_t	yyuid;
					std::string ms0; //注册成功返回登录凭证，与登录时返回的凭证一致，可用于后续的快速登录
		
					virtual void marshal(sox::Pack &pk) const {
						pk << strSeq << resCode << failDesc << strUsername << yyid << yyuid << ms0;	
					}
		
					virtual void unmarshal(const sox::Unpack &up) {
						up >> strSeq >> resCode >> failDesc >> strUsername >> yyid >> yyuid >> ms0;	
					}
		};
		
		/* 风控服务thrift转yy协议请求 */
		struct PGetRiskInfoReq : public sox::Marshallable {
			enum {uri = (151 << 8) | 178};
		
			std::string appid;
		    uint64_t yyuid; 
			std::string client_ip; 
			map<std::string,std::string> authCookie; 
		
		      virtual void marshal(sox::Pack &p) const {
		      	p << appid << yyuid << client_ip;
		      	uint32_t num = authCookie.size();
				p << num;
				
				map<string, string>::const_iterator iter = authCookie.begin();
				for(; iter != authCookie.end(); iter++){
					p << iter->first << iter->second;
				}
		      }
		
		      virtual void unmarshal(const sox::Unpack &p) {
		      	p >> appid >> yyuid >> client_ip ;
		      	uint32_t num = 0;
				string key;
				string value;

				p >> num;
				for(unsigned i = 0; i < num; i++){
					p >> key >> value;
					authCookie[key] = value;
				}
		      }
		};
		
		struct PGetRiskInfoRes : public sox::Marshallable {
		      enum {uri = (152 << 8) | 178};
		
		      uint32_t 	resCode; //0--表示成功；
			std::string failDesc;  //错误描述 
			map<std::string,std::string> riskInfos; 
		
		      virtual void marshal(sox::Pack &p) const {
		      	p << resCode << failDesc;
		      	uint32_t num = riskInfos.size();
				p << num;
				
				map<string, string>::const_iterator iter = riskInfos.begin();
				for(; iter != riskInfos.end(); iter++){
					p << iter->first << iter->second;
				}
		      }
		
		      virtual void unmarshal(const sox::Unpack &p) {
		      	p >> resCode >> failDesc ;
		      	uint32_t num = 0;
				string key;
				string value;

				p >> num;
				for(unsigned i = 0; i < num; i++){
					p >> key >> value;
					riskInfos[key] = value;
				}
		      }
		};
		
		//for import hello user 
		struct PNewHelloUserReq : public sox::Marshallable
		{
			enum {uri = (153 << 8) | 178};

			std::string strSeq;
			std::string strMobile;
			std::string strPwd;
			std::string strRegIp;
			std::string strAppid;
			std::string strAppkey;
			uint64_t	yyuid;

			virtual void marshal(sox::Pack &pk) const {
				pk << strSeq << strMobile << strPwd << strRegIp << strAppid << strAppkey << yyuid;	
			}

			virtual void unmarshal(const sox::Unpack &up) {
				up >> strSeq >> strMobile >> strPwd >> strRegIp >> strAppid >> strAppkey >> yyuid;
			}
		};

		struct PNewHelloUserRes : public sox::Marshallable
		{
			enum {uri = (154 << 8) | 178};
		
			std::string strSeq;
			uint32_t 	resCode;
			std::string	strUsername;
			uint64_t	yyid;
			uint64_t	yyuid;

			virtual void marshal(sox::Pack &pk) const {
				pk << strSeq << resCode << strUsername << yyid << yyuid;	
			}

			virtual void unmarshal(const sox::Unpack &up) {
				up >> strSeq >> resCode >> strUsername >> yyid >> yyuid;	
			}
		};
		
		struct PFastMobRegReq : public sox::Marshallable
		{
			enum {uri = (155 << 8) | 178};

			std::string strSeq;
			std::string strMobile;
			std::string strSha1Pwd;
			std::string strRegIp;
			std::string strAppid;
			std::string strAppkey;
			uint32_t	yyuid;

			virtual void marshal(sox::Pack &pk) const {
				pk << strSeq << strMobile << strSha1Pwd << strRegIp << strAppid << strAppkey << yyuid;	
			}

			virtual void unmarshal(const sox::Unpack &up) {
				up >> strSeq >> strMobile >> strSha1Pwd >> strRegIp >> strAppid >> strAppkey >> yyuid;
			}
			
		};
		
		//for modify username 
		struct PModUsernameReq : public sox::Marshallable
		{
			enum {uri = (157 << 8) | 178};

			std::string strSeq;
			std::string strOldname;
			std::string strNewname;
			std::string strRegIp;
			std::string strAppid;

			virtual void marshal(sox::Pack &pk) const {
				pk << strSeq << strOldname << strNewname << strRegIp << strAppid;	
			}

			virtual void unmarshal(const sox::Unpack &up) {
				up >> strSeq >> strOldname >> strNewname >> strRegIp >> strAppid;
			}
		};

		struct PModUsernameRes : public sox::Marshallable
		{
			enum {uri = (158 << 8) | 178};
		
			std::string strSeq;
			uint32_t 	resCode;
			std::string	strUsername;
			
			virtual void marshal(sox::Pack &pk) const {
				pk << strSeq << resCode << strUsername;	
			}

			virtual void unmarshal(const sox::Unpack &up) {
				up >> strSeq >> resCode >> strUsername;	
			}
		};
		//设置手机绑定账号数目限制
		struct PSetMobileBindLimitReq : public sox::Marshallable
		{
			enum {uri = (161 << 8) | 178};

			std::string strSeq;
			std::string strMobile;
			uint32_t 	limits;

			virtual void marshal(sox::Pack &pk) const {
				pk << strSeq << strMobile << limits;	
			}

			virtual void unmarshal(const sox::Unpack &up) {
				up >> strSeq >> strMobile >> limits;
			}
		};

		struct PSetMobileBindLimitRes : public sox::Marshallable
		{
			enum {uri = (162 << 8) | 178};
		
			std::string strSeq;
			uint32_t 	resCode;
			
			virtual void marshal(sox::Pack &pk) const {
				pk << strSeq << resCode;	
			}

			virtual void unmarshal(const sox::Unpack &up) {
				up >> strSeq >> resCode;	
			}
		};
		
		//取用户绑定手机时间
		struct PGetMobileBindTimeReq : public sox::Marshallable
		{
			enum {uri = (163 << 8) | 178};

			std::string strSeq;
			std::string strUserName;


			virtual void marshal(sox::Pack &pk) const {
				pk << strSeq << strUserName;	
			}

			virtual void unmarshal(const sox::Unpack &up) {
				up >> strSeq >> strUserName;
			}
		};

		struct PGetMobileBindTimeRes : public sox::Marshallable
		{
			enum {uri = (164 << 8) | 178};
		
			std::string strSeq;
			uint32_t 	resCode; //0-成功，1-无绑定手机，7-系统错误
			std::string strUserName;
			std::string strBindTime;
			
			virtual void marshal(sox::Pack &pk) const {
				pk << strSeq << resCode << strUserName << strBindTime;	
			}

			virtual void unmarshal(const sox::Unpack &up) {
				up >> strSeq >> resCode >> strUserName >> strBindTime;	
			}
		};
		
		struct PNewCreateUserExReq : public sox::Marshallable
		{
			enum {uri = (165 << 8) | 178};

			std::string strSeq;
			std::string strUsername;
			std::string strEmail;
			std::string strMobile;
			std::string strPassword;
			std::string strRegTime;
			std::string strRegIp;
			std::string strAppid;
			uint32_t yyuid;
			uint32_t yyid;
			uint32_t mobile_flag; //0 登录手机 1 密保手机 2 注册手机 3 不绑手机

			virtual void marshal(sox::Pack &pk) const
			{
				pk << strSeq << strUsername << strEmail << strMobile << strPassword << strRegTime << strRegIp << strAppid << yyuid << yyid << mobile_flag;
			}

			virtual void unmarshal(const sox::Unpack &up)
			{
				up >> strSeq >> strUsername >> strEmail >> strMobile >> strPassword >> strRegTime >> strRegIp >> strAppid >> yyuid >> yyid >> mobile_flag;	
			}
		};
		struct PEmailRegExReq : public sox::Marshallable
		{
			enum {uri = (167 << 8) | 178};

			std::string strSeq;
			std::string strEmail;
			std::string strVcode;
			std::string strSha1Pwd;
			std::string strRegIp;
			std::string strAppid;
			std::string strAppkey;
			uint32_t	yyuid;
			std::string strMobile;
			uint32_t mobile_flag; //0 登录手机 1 密保手机 2 注册手机 3 不绑手机
			
			virtual void marshal(sox::Pack &pk) const {
				pk << strSeq << strEmail << strVcode << strSha1Pwd << strRegIp << strAppid << strAppkey << yyuid << strMobile << mobile_flag;	
			}

			virtual void unmarshal(const sox::Unpack &up) {
				up >> strSeq >> strEmail >> strVcode >> strSha1Pwd >> strRegIp >> strAppid >> strAppkey >> yyuid >> strMobile >> mobile_flag;
			}
			
		};
		
		struct PCreateGameUserExReq : public sox::Marshallable {
			enum {uri = (169 << 8) | 178};
			
			std::string strSeq;
			std::string strUsername;
			std::string strEmail;
			std::string strMobile;
			std::string strPassword;
			std::string strRegTime;
			std::string strRegIp;
			std::string strAppid;
			uint32_t yyuid;
			uint32_t uid;
			uint32_t mobile_flag; //0 登录手机 1 密保手机 2 注册手机 3 不绑手机

			virtual void marshal(sox::Pack &pk) const
			{
				pk << strSeq << strUsername << strEmail << strMobile << strPassword << strRegTime << strRegIp << strAppid << yyuid << uid << mobile_flag;
			}

			virtual void unmarshal(const sox::Unpack &up)
			{
				up >> strSeq >> strUsername >> strEmail >> strMobile >> strPassword >> strRegTime >> strRegIp >> strAppid >> yyuid >> uid >> mobile_flag;	
			}
		};
		
		//查询注册渠道号和注册时间
		struct PGetMobChannelReq : public sox::Marshallable
		{
			enum {uri = (171 << 8) | 178};

			std::string strSeq;
			uint64_t yyuid;


			virtual void marshal(sox::Pack &pk) const {
				pk << strSeq << yyuid;	
			}

			virtual void unmarshal(const sox::Unpack &up) {
				up >> strSeq >> yyuid;
			}
		};

		struct PGetMobChannelRes : public sox::Marshallable
		{
			enum {uri = (172 << 8) | 178};
		
			std::string strSeq;
			uint32_t 	resCode; //0-成功，1-用户不在，7-系统错误
			std::string strChannel; //渠道标识
			std::string strRegTime; //注册时间yyyy-mm-dd hh:mi:ss
			
			virtual void marshal(sox::Pack &pk) const {
				pk << strSeq << resCode << strChannel << strRegTime;	
			}

			virtual void unmarshal(const sox::Unpack &up) {
				up >> strSeq >> resCode >> strChannel >> strRegTime;	
			}
		};
		
		struct P3rdBindMibaoMobileReq : public sox::Marshallable {
			enum {uri = (173 << 8) | 178};	

			std::string strSeq;
			uint32_t yyuid;
			std::string username;
			std::string mobile;
			std::string clientip;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << yyuid << username << mobile << clientip; 
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> yyuid >> username >> mobile >> clientip;

			}
		};

		struct P3rdBindMibaoMobileRes : public sox::Marshallable {
			enum {uri = (174 << 8) | 178};			

			std::string strSeq;
			uint32_t resCode;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << resCode;
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> resCode;	
			}
		};

		struct MP_Request: public sox::Marshallable //手y查询的透传包
    {
        enum {uri = (1002 << 8 | protocol::UDB_SVID)};

        uint32_t commandType;
        std::string request;

        virtual void marshal(sox::Pack &pk) const
        {
            pk << commandType << request;
        }

        virtual void unmarshal(const sox::Unpack &up)
        {
            up >> commandType >> request;
        }
    };

	}
}

//!!!!!!notice:
//if you add interface msg,don't conflict with the define in PpwdSecurity.h
#endif
