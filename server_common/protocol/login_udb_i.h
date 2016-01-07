/********************************************************************
	created:	2014-3-12   18:46
	filename: 	login_udb_i.h
	author:		Luke Chou
	copyright:  Guangzhou huaduo Co., Ltd.
	purpose:
    description:
                
---------------------------------------------------------------------
sequence      date                  author          update history
1             2014/3/12             Luke Chou       initial
*********************************************************************/

#ifndef _login_udb_i_H_2014_3_12
#define _login_udb_i_H_2014_3_12

#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "common/core/base_svid.h"
#include "common/packet.h"
#include "common/protocol/const.h"
#include "common/protocol/zip_tpl.h"
#include "protocol/CodeInfo.h"
#include "common/res_code.h"
#include "server_login/common/resource/login_uri.h"

//规范错误码
// sys_err, bus_err范围必须在100以内
#define MK_SYSERR_CODE(mud_id,sys_err)   (mud_id)*10000 + (sys_err)*100
#define MK_BUSERR_CODE(mud_id,bus_err)   (mud_id)*10000 + (bus_err)


namespace server{ namespace loginUDB
{
	enum ENU_DYNAMIC_STRATEGY
	{
		DS_NONE  = 0x0,         // 无动态策略
		DS_PICCODE = 0x01,      // 图片验证码
		DS_MOBTOKEN = 0x02,     // 手机令牌
		DS_HWTOKEN = 0x04,      // 硬件令牌
		DS_SMSCODE = 0x08,      // 短信验证码
		DS_SEC_QESTION = 0x10   // 密保问题
	};

	enum ENU_STATIC_TOKEN_TYPE
	{
		STT_PASSWD =  0,  // xxtea加密的一次sha1密码
		STT_TICKET =  1,  // kerberos票据
		STT_ACCINFO=  2,  // accinfo
		STT_OTPCODE = 3,  // 一次性口令
		STT_YYTOKEN = 4   // 第三方登录YY时产生的凭证
	};

	struct UDBLoginReq:public sox::Marshallable
	{
		enum{uri=URI_UDBLOGIN_REQ}; // (10 << 8)|233
		std::string _context;       // 消息流水
		std::string _appid;         // 业务标识号
		std::string _mac_address;   // 客户端mac地址
		std::string _client_ip;     // 客户端ip
		uint32_t    _ver_int;       // 整数版本号
		std::string _ver_str;       // 字符串版本号
		uint8_t     _login_type;    // 登录类型， 0 - 静态验证, 1- 动态验证
		uint16_t    _dynamic_stra;  // 动态验证策略，见ENU_DYNAMIC_STRATEGY枚举值
		std::string _user;          // 用户，可以是：YY通行证，YY号，手机号，email
		uint8_t     _stt;           // 静态口令类型。0 - 二次sha1密码， 1 - 票据
		std::string _static_token;  // 静态口令。二次SHA1密码或者YY客户端登录生成的票据
		/*动态口令：
		1. 为手机令牌或硬件令牌时不需要进一步编码
		2. 为图片验证码时，见LoginPicCode
		3. 为密保问题时，见LoginSecq
		*/
		std::string _dynamic_token;
		std::vector<std::string> _jump_appids; // 跳转业务标识
		std::vector<std::string> _jump_sessionids; // 如果appid对应没有session_id，则对应session_id为空
		std::string _extension;     // 扩展字段，可选字段，在二进制流中也可以不出现

		bool is_login_static()const{return 0 == _login_type;}
		bool is_login_dynamic()const{return 1 == _login_type;}
		bool dynamic_token_empty()const{return _dynamic_token.empty();}
		bool is_login_pwd()const{return 0 == _stt;}
		bool is_login_ticket()const {return 1 == _stt;}

		UDBLoginReq():_ver_int(0), _login_type(0),_dynamic_stra(0),_stt(0){}
		UDBLoginReq(const UDBLoginReq& rth)
		{
			_context = rth._context;
			_appid = rth._appid;
			_mac_address = rth._mac_address;
			_client_ip = rth._client_ip;
			_ver_int = rth._ver_int;
			_ver_str = rth._ver_str;
			_login_type = rth._login_type;
			_dynamic_stra = rth._dynamic_stra;
			_user = rth._user;
			_stt = rth._stt;
			_static_token = rth._static_token;
			_dynamic_token = rth._dynamic_token;
			_jump_appids = rth._jump_appids;
			_jump_sessionids = rth._jump_sessionids;
			_extension = rth._extension;
		}

		UDBLoginReq& operator=(const UDBLoginReq& rth)
		{
			if (this == &rth)
			{
				return *this;
			}
			_context = rth._context;
			_appid = rth._appid;
			_mac_address = rth._mac_address;
			_client_ip = rth._client_ip;
			_ver_int = rth._ver_int;
			_ver_str = rth._ver_str;
			_login_type = rth._login_type;
			_dynamic_stra = rth._dynamic_stra;
			_user = rth._user;
			_stt = rth._stt;
			_static_token = rth._static_token;
			_dynamic_token = rth._dynamic_token;
			_jump_appids = rth._jump_appids;
			_jump_sessionids = rth._jump_sessionids;
			_extension = rth._extension;
			return *this;
		}

		struct LoginPicCode:public sox::Marshallable
		{
			std::string _pic_id;
			std::string _vali_code;
			virtual void marshal(sox::Pack &pk) const
			{
				pk << _pic_id << _vali_code;
			}
			virtual void unmarshal(const sox::Unpack &up)
			{
				up >> _pic_id >> _vali_code;
			}
		};

		struct LoginSecq:public sox::Marshallable
		{
			std::string _question;
			std::string _answer;
			virtual void marshal(sox::Pack &pk) const
			{
				pk << _question << _answer;
			}
			virtual void unmarshal(const sox::Unpack &up)
			{
				up >> _question >> _answer;
			}
		};

		virtual void marshal( sox::Pack &pk )const
		{
			pk << _context << _appid << _mac_address << _client_ip 
				<< _ver_int << _ver_str << _login_type << _dynamic_stra << _user << _stt
				<< _static_token << _dynamic_token << _jump_appids << _jump_sessionids;
			if (!_extension.empty())
			{
				pk << _extension;
			}
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			up >> _context >> _appid >> _mac_address >> _client_ip 
				>> _ver_int >> _ver_str >> _login_type >> _dynamic_stra >> _user >> _stt
				>> _static_token >> _dynamic_token >> _jump_appids >> _jump_sessionids;
			if (!up.empty())
			{
				up >> _extension;
			}
		}
	};

	struct UDBLoginRes:public sox::Marshallable
	{
		enum{uri=URI_UDBLOGIN_RES};
		enum ENU_URES
		{
			URES_SUCCESS = 0,             // 成功
			URES_STATIC_VERIFY = 1,       // 需要进一步静态验证（下发了图片验证码）
			URES_DYNAMIC_VERIFY = 2,      // 需要进行动态验证
			URES_PWD_ERR = 3,             // 账号或密码错误
			URES_TICKET_ERR = 4,          // 票据验证失败
			URES_LOCKED = 5,              // 账号被锁定
			URES_FROZEN = 6,              // 账号被临时冻结
			URES_REJECT = 7,              // 非法操作拒绝处理
			URES_MOB_TOKEN_ERR = 8,       // 手机令牌错误
			URES_HW_TOKEN_ERR = 9,        // 硬件令牌错误
			URES_PICCODE_ERR = 10,        // 图片验证码错误
			URES_SEC_QUESTION_ERR  =  11, // 密保问题验证错误
			URES_SMS_CODE_ERR  =  12,     // 短信验证错误
			USER_YID_LOGIN_REJECT = 13,   // 无Yy号登录权限
			USER_NO_LOGIN_AUTH    = 14,   // 无登录权限
			USER_NO_WEB_LOGIN_AUTH  = 21,     // web封禁
			USER_USER_NOT_EXIST  = 97,     // 用户不存在
			URES_RETRY = 99,              // 需重试
			URES_PARAM_ERR = 100          // 参数错误
		};
		
		
		std::string _context;      // 消息流水
		uint32_t    _rescode;      // 结果码，见上面的ENU_URES枚举值
		std::string _reason;       // 失败原因描述，成功时为空，格式可定制(json格式)
		uint16_t    _dynamic_stra; // 动态验证策略，bit位组合多种策略，参见ENU_DYNAMIC_STRATEGY枚举值
		std::string _login_data;   // 登录返回，如果登录成功(rescode=URES_SUCCESS)返回LoginData结构数据，否则为空
		std::string _next_need;    // 下次验证需要的数据，根据rescode决定其编码格式
		std::vector<std::string> _jump_tokens; // 跳转使用的登录凭证，数组的每一维的格式见JumpToken定义
		std::string _extension;    // 扩展字段，备用，为空时不出现

		UDBLoginRes():_rescode(99), _dynamic_stra(0){}
		struct LoginData:public sox::Marshallable
		{
			uint64_t    _yyuid;
			uint64_t    _yyid;
			uint64_t    _udbuid;
			std::string _passport;
			std::string _accessToken;
			std::string _accountInfo;
			std::string _mobile_mask;
			LoginData():_yyuid(0), _yyid(0), _udbuid(0){}
			virtual void marshal(sox::Pack &pk) const
			{
				pk << _yyuid << _yyid << _udbuid << _passport << _accessToken << _accountInfo << _mobile_mask;
			}
			virtual void unmarshal(const sox::Unpack &up)
			{
				up >> _yyuid >> _yyid >> _udbuid >> _passport >> _accessToken >> _accountInfo >> _mobile_mask;
			}
		};

		struct PPicCodeLoad: public sox::Marshallable
		{
			std::string  _pic_id;
			std::string  _pic_code;
			std::string  _reason;   // 弹图片的原因，可定制

			virtual void marshal(sox::Pack &pk) const
			{
				pk << _pic_id << _pic_code << _reason;
			}
			virtual void unmarshal(const sox::Unpack &up)
			{
				up >> _pic_id >> _pic_code >> _reason;
			}
		};

		struct JumpToken:public sox::Marshallable
		{
			std::string _appid; // 可跳转标识
			std::string _token; // 跳转使用的凭证
			uint32_t    _result;
			std::string _reason;
			virtual void marshal(sox::Pack &pk) const
			{
				pk << _appid << _token << _result << _reason;
			}
			virtual void unmarshal(const sox::Unpack &up)
			{
				up >> _appid >> _token >> _result >> _reason;
			}
		};

		virtual void marshal( sox::Pack &pk )const
		{

			pk << _context << _rescode << _reason << _dynamic_stra 
				<< _login_data << _next_need << _jump_tokens;
			if (!_extension.empty())
			{
				pk << _extension;
			}
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			up >> _context >> _rescode >> _reason >> _dynamic_stra
				>> _login_data >> _next_need >> _jump_tokens;
			if (!up.empty())
			{
				up >> _extension;
			}
		}
	};

	/*下发短信验证码*/
	struct UDBSendSmscodeReq:public sox::Marshallable
	{
		enum{uri=URI_UDBSEND_SMSCODE_REQ}; // (16 << 8)|233
		std::string _context;       // 消息流水
		std::string _appid;         // 业务标识号
		std::string _mac_address;   // 客户端mac地址
		std::string _client_ip;     // 客户端ip
		uint32_t    _ver_int;       // 整数版本号
		std::string _ver_str;       // 字符串版本号
		std::string _user;          // 用户，可以是：YY通行证，YY号，手机号，email
		uint8_t     _stt;           // 静态口令类型。参考枚举值：ENU_STATIC_TOKEN_TYPE
		std::string _static_token;  // 静态口令
		std::string _extension;     // 扩展预留字段

		bool is_login_pwd()const{return 0 == _stt;}
		bool is_login_ticket()const {return 1 == _stt;}

		UDBSendSmscodeReq():_ver_int(0){}
		UDBSendSmscodeReq(const UDBSendSmscodeReq&rth)
		{
			_context = rth._context;
			_appid = rth._appid;
			_mac_address = rth._mac_address;
			_client_ip = rth._client_ip;
			_ver_int = rth._ver_int;
			_ver_str = rth._ver_str;
			_user = rth._user;
			_stt = rth._stt;
			_static_token = rth._static_token;
			_extension = rth._extension;
		}
		UDBSendSmscodeReq& operator=(const UDBSendSmscodeReq& rth)
		{
			if (this == &rth)
			{
				return *this;
			}
			_context = rth._context;
			_appid = rth._appid;
			_mac_address = rth._mac_address;
			_client_ip = rth._client_ip;
			_ver_int = rth._ver_int;
			_ver_str = rth._ver_str;
			_user = rth._user;
			_stt = rth._stt;
			_static_token = rth._static_token;
			_extension = rth._extension;
			return *this;
		}
		virtual void marshal( sox::Pack &pk )const
		{
			pk << _context << _appid << _mac_address << _client_ip 
				<< _ver_int << _ver_str << _user << _stt << _static_token;
			if (!_extension.empty())
			{
				pk << _extension;
			}
		}
		virtual void unmarshal(const sox::Unpack &up)
		{
			up >> _context >> _appid >> _mac_address >> _client_ip 
				>> _ver_int >> _ver_str >> _user >> _stt >> _static_token;
			if (!up.empty())
			{
				up >> _extension;
			}
		}
	};

	struct UDBSendSmscodeRes:public sox::Marshallable
	{
		enum{uri=URI_UDBSEND_SMSCODE_RES}; // (17 << 8)|233
		enum
		{
			SSRES_SUCCESS = 0,       // 成功
			SSRES_SMSCODE_FAIL = 1,  // 下发短信验证码失败
			SSRES_PWD_ERR = 2,       // 账号或密码错误
			SSRES_TICKET_ERR = 3,    // 票据验证失败
			SSRES_REJECT = 4,        // 非法操作拒绝处理
			SSRES_RETRY = 99,        // 客户端需重试
			SSRES_PARAM_ERR = 100    // 参数错误
		};
		std::string _context;      // 消息流水
		uint32_t    _rescode;      // 结果码，见上面的SDRES开头的枚举值
		std::string _reason;       // 失败原因描述
		std::string _extension;    // 扩展字段，如果为空，则该字段不出现
		UDBSendSmscodeRes():_rescode(0){}
		virtual void marshal( sox::Pack &pk )const
		{
			pk << _context << _rescode << _reason;
			if (!_extension.empty())
			{
				pk << _extension;
			}
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			up >> _context >> _rescode >> _reason;
			if (!up.empty())
			{
				up >> _extension;
			}
		}
	};
	
	enum ENU_DYNAMIC_VFY_OPTION
	{
		VFY_NONE  = 0x0,         // 无动态策略
		VFY_MOBTOKEN = 0x02,     // 手机令牌
		VFY_HWTOKEN = 0x04,      // 硬件令牌
		VFY_SMSCODE = 0x08,      // 短信验证码
		VFY_SEC_QESTION = 0x10   // 密保问题
	};
	
	struct UDBYYLoginReq:public sox::Marshallable
	{
		enum{uri=URI_UDBYYLOGIN_REQ}; // (8 << 8)|233
		std::string _context;       // 消息流水
		std::string _appid;         // 业务标识号
		uint8_t     _apptype;        //业务类型 
		/*
		1.	win PC APP 
		2.	Android APP（包括手机和pad）
		3.	IOS APP（包括手机和ipad）
		4.	MAC APP
		5.	WEB PC
		6.	WEB MObile 
		7.	linux APP
		*/
		uint32_t    _lcid;         //语言标志 2052简体中文 
		
		uint64_t    _terminal_type; // 
		std::string _device_id;     // 设备标识：PC端网卡为物理地址，移动端为imid标识, web端可选
		std::string _client_ip;     // 客户端ip
		uint32_t    _ver_int;       // 客户端整数版本号
		std::string _ver_str;       // 客户端字符串版本号
		std::string _version;       // 协议版本号


		uint64_t    _yyuid;           // 当user_token_type=2,4必填
		std::string _user;            // YY通行证，YY号，手机号，email。当user_token_type=1,3必填
		uint8_t     _user_token_type; // 1 - xxtea加密的一次sha1密码,  2 - 票据 - 3 accinfo, 4 opt 5s1 6 s2 7 md5+sha1密码(CombPasswd) 8无密smscode登录, 9 webyy带appidtype的acctinfo
		std::string _user_token;     //  口令或身份信息。根据_user_token_type取值决定填入内容
		    
		std::string _pic_id;   //图片验证码编号，当服务端返回需要验证图片时填写(即前一次登录响应返回rescode=1时)
	    std::string _pic_code; //图片验证码,当服务端返回需要验证图片时填写(即前一次登录响应返回rescode=1时)
	    
		uint16_t    _dynamic_token_type;  // 需要验证的验证项目，见ENU_DYNAMIC_VFY_OPTION枚举值 前一次登录响应返回rescode=2时) 本次登录请求必填
		/*
		需要验证的验证内容，包括
		1.	手机令牌
        2.	硬件令牌
        3.	为密保问题见LoginSecq
        4.  短信验证码
        */
		std::string _dynamic_token;      
     
		std::string _vmToken;       // 用于虚拟机验证的token.
		
		std::vector<std::string> _jump_appids;     // 跳转业务标识
		std::vector<std::string> _jump_sessionids; // 如果appid对应没有session_id，则对应session_id为空
		    
        std::string _callback_data;     // 首次请求登录时为空。如果需要进一步的交互验证，则服务端会生成一个数据返回到客户端，后续验证阶段均需要传递该值。
		std::string _extension;     // 扩展字段，可选字段，在二进制流中也可以不出现
		//

		bool is_login_pwd()const{return 1 == _user_token_type;}

		bool is_login_ticket()const {return 2 == _user_token_type;}
		
		bool is_login_acctinfo()const {return 3 == _user_token_type || 9 == _user_token_type;}
		
		bool is_login_comb_acctinfo()const { return 9 == _user_token_type;}
		
		bool is_login_otp()const {return 4 == _user_token_type;}
		
		bool is_mob_login_mob_accesstoken()const {return 5 == _user_token_type;}

		bool is_mob_login_mob_otp()const {return 6 == _user_token_type;}
		//组合密码， 既带有md5密码，又带有sha1密码
		bool is_login_combine_passwd()const {return 7 == _user_token_type;}
		
		bool is_login_static_with_piccode()const{ return "" != _pic_id; }
		
		bool is_login_static()const{ return 0 == _dynamic_token_type; }
		
		bool is_login_dynamic()const{return (VFY_NONE != _dynamic_token_type) && !_dynamic_token.empty(); }
		
		UDBYYLoginReq():_apptype(0),_lcid(0),_terminal_type(0),_ver_int(0),_yyuid(0),_user_token_type(0),_dynamic_token_type(0) {}
		UDBYYLoginReq(const UDBYYLoginReq& rth)
		{
			_context = rth._context;
			_appid = rth._appid;
			_apptype = rth._apptype;
			_lcid    = rth._lcid;
			_terminal_type = rth._terminal_type;
			_device_id = rth._device_id;
			_client_ip = rth._client_ip;
			_ver_int = rth._ver_int;
			_ver_str = rth._ver_str;
			_version = rth._version;
			
			_yyuid              = rth._yyuid;
			_user               = rth._user;
			_user_token_type    = rth._user_token_type;
			_user_token         = rth._user_token;
			
			_pic_id             = rth._pic_id;
			_pic_code           = rth._pic_code;

			_dynamic_token_type = rth._dynamic_token_type;
			_dynamic_token      = rth._dynamic_token;
			_vmToken            = rth._vmToken;
			_jump_appids        = rth._jump_appids;
			_jump_sessionids    = rth._jump_sessionids;
			_callback_data      = rth._callback_data;
			_extension = rth._extension;

		}
        struct CombAcctinfo : public sox::Marshallable
        {
        
          std::string acctinfo;// 反外挂虚拟机类别
          std::string webyyscene; 
          
          virtual void marshal(sox::Pack &pk) const 
          {
            pk << acctinfo << webyyscene;
          }
          virtual void unmarshal(const sox::Unpack &up)
          {
            up >> acctinfo >> webyyscene;
          }
        
        };
        struct CombPasswd : public sox::Marshallable
        {
        
          std::string md5passwd;// 反外挂虚拟机类别
          std::string sha1passwd; 
          
          virtual void marshal(sox::Pack &pk) const 
          {
            pk << md5passwd << sha1passwd;
          }
          virtual void unmarshal(const sox::Unpack &up)
          {
            up >> md5passwd >> sha1passwd;
          }
        
        };
        struct AntiCode : public sox::Marshallable
        {
        
          std::string bizName;// 反外挂虚拟机类别
          std::string antiCode; 
          
          virtual void marshal(sox::Pack &pk) const 
          {
            pk << bizName << antiCode;
          }
          virtual void unmarshal(const sox::Unpack &up)
          {
            up >> bizName >> antiCode;
          }
        
        };
        
        struct OtpCode : public sox::Marshallable
        {
        
          std::string sessionid;
          std::string otp; 
          std::string otp_appid;
          virtual void marshal(sox::Pack &pk) const 
          {
            pk << sessionid << otp << otp_appid;
          }
          virtual void unmarshal(const sox::Unpack &up)
          {
            up >> sessionid >> otp >> otp_appid;
          }
        
        };
     
		UDBYYLoginReq& operator=(const UDBYYLoginReq& rth)
		{
			if (this == &rth)
			{
				return *this;
			}
			_context = rth._context;
			_appid = rth._appid;
			_apptype = rth._apptype;
			_lcid    = rth._lcid;
			_terminal_type = rth._terminal_type;
			_device_id = rth._device_id;
			_client_ip = rth._client_ip;
			_ver_int = rth._ver_int;
			_ver_str = rth._ver_str;
			_version = rth._version;
			
			_yyuid              = rth._yyuid;
			_user               = rth._user;
			_user_token_type    = rth._user_token_type;
			_user_token         = rth._user_token;
			
			_pic_id             = rth._pic_id;
			_pic_code           = rth._pic_code;

			_dynamic_token_type = rth._dynamic_token_type;
			_dynamic_token      = rth._dynamic_token;
			_vmToken            = rth._vmToken;
			_jump_appids        = rth._jump_appids;
			_jump_sessionids    = rth._jump_sessionids;
			_callback_data      = rth._callback_data;
			_extension = rth._extension;
			return *this;
		}

		struct LoginSecq:public sox::Marshallable
		{
			std::string _question;
			std::string _answer;
			virtual void marshal(sox::Pack &pk) const
			{
				pk << _question << _answer;
			}
			virtual void unmarshal(const sox::Unpack &up)
			{
				up >> _question >> _answer;
			}
		};

		virtual void marshal( sox::Pack &pk )const
		{
			pk << _context << _appid << _apptype << _lcid << _terminal_type <<  _device_id << _client_ip 
				<< _ver_int    << _ver_str << _version  << _yyuid << _user << _user_token_type
				<< _user_token <<  _pic_id << _pic_code << _dynamic_token_type << _dynamic_token 
				<< _vmToken   << _jump_appids << _jump_sessionids  << _callback_data;
			if (!_extension.empty())
			{
				pk << _extension;
			}
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			up >> _context >> _appid >> _apptype >> _lcid  >> _terminal_type >>  _device_id >> _client_ip 
				>> _ver_int >> _ver_str >> _version >> _yyuid >>  _user >> _user_token_type
				>> _user_token >> _pic_id >> _pic_code >> _dynamic_token_type >> _dynamic_token 
				>> _vmToken >> _jump_appids  >> _jump_sessionids >> _callback_data;
			if (!up.empty())
			{
				up >> _extension;
			}
		}
	};

	struct UDBYYLoginRes:public sox::Marshallable
	{
		enum{uri=URI_UDBYYLOGIN_RES}; // (9 << 8)|233
		enum ENU_LRES
		{
			LRES_SUCCESS = 0,             // 成功
			LRES_NEED_PICCODE_VERIFY = 1, // 需要图片验证码验证
			LRES_NEED_NEXT_VERIFY = 2,    // 需要进一步强验证：手机短信/密保问题/手机令牌/硬件令牌，根据响应结果任选一个。
			LRES_PWD_ERR = 3,             // 密码错误
			LRES_TICKET_ERR = 4,          // 票据验证失败
			LRES_LOCKED = 5,              // 账号被锁定（用户已设置账号锁）
			LRES_FROZEN = 6,              // 账号临时冻结（违反安全策略）
			LRES_REJECT = 7,              // 非法操作拒绝处理
			LRES_MOB_TOKEN_ERR = 8,       // 手机令牌错误
			LRES_HW_TOKEN_ERR = 9,        // 硬件令牌错误
			LRES_PICCODE_ERR = 10,         // 图片验证码错误
			LRES_SEC_QUESTION_ERR  =  11,  // 密保问题验证错误
			LRES_SMS_CODE_ERR  =  12,      // 短信验证错误
			LSER_YID_LOGIN_LIMIT = 13,     // 未开通yy号登录权限
			LSER_NO_LOGIN_AUTH = 14,       // 
			LRES_ACCTINFO_ERR    = 15,     // acctinfo错误
			LRES_OTP_ERR = 16,             // otp验证失败	
			LRES_SMS_VERIFY_LIMIT = 17,   // 短信验证码验证次数限制，主要用于中止登录
			LSER_SERVER_BANNED  = 18,     // 运营封禁
			LSER_NO_WEB_LOGIN_AUTH  = 21,     // web封禁
			//用于移动登录
			LSER_MOB_S1_ERROR  = 101,        // 种子凭证错误/无效
			LSER_MOB_S1_EXPIRE  = 102,        // 种子凭证过期
			LSER_MOB_DEVICE_ID_UNMATCH  = 103, //
			LSER_MOB_MS0_ERROR  = 104,        // 登录凭证错误
			LSER_MOB_MS1_ERROR  = 105,         //登录子凭证错误
			LSER_MOB_CNONCE_INVALID  = 106,      // 子凭证无效
			LSER_MOB_MS1_BEEN_USED  = 107,    // 子凭证重复使用
			LSER_USER_NOT_EXIST  = 97,     // 用户不存在
			LRES_RETRY = 99,              // 需重试
			LRES_PARAM_ERR = 100          // 参数错误
		};

		std::string _context;      // 消息流水
		uint32_t    _rescode;      // 结果码，见上面的ENU_LRES枚举值
		std::string _reason;       // 针对错误码的错误信息,主要用于日志打印 
		std::string _description;  // 失败原因描述，用于对用户展示,成功时为空，格式可定制(json格式，包括：title/content)
		uint64_t    _yyuid;        // yyuid
		std::vector< std::string >  dynamic_token_reqs; // 服务端下发的动态验证请求 见 DynamicTokenReq结构
		
		std::string _pic_id;      // _图片验证码编号 rescode = 1/10 有效
		std::string _pic_data;    //  图片验证码,png格式 rescode = 1/10 有效
		std::string _login_data;   // 登录返回，如果登录成功(rescode=URES_SUCCESS)返回LoginData结构数据，否则为空


		std::string _callback_data;
		std::string _extension;    // 扩展字段，备用，为空时不出现

		UDBYYLoginRes():_rescode(99), _yyuid(0){}
		
		struct DynamicTokenReq:public sox::Marshallable
		{
			uint8_t token_type;// 动态验证策略，见 ENU_DYNAMIC_VFY_OPTION 枚举值（除图片验证码之外的）
			std::string verification_selection_title;
			std::string verification_title;
			std::string token_data;
			virtual void marshal(sox::Pack &pk) const
			{
				pk << token_type << verification_selection_title << verification_title << token_data;
			}
			virtual void unmarshal(const sox::Unpack &up)
			{
				up >> token_type >> verification_selection_title >> verification_title >> token_data;
			}
		};
		
		struct LoginData:public sox::Marshallable
		{
			uint64_t    _yyid;
			uint64_t    _udbuid;
			std::string _passport;
			std::string _accessToken;
			std::string _accountInfo;
		    std::vector<std::string> _jump_tokens; // 跳转使用的登录凭证，数组的每一维的格式见JumpToken定义
			std::string _ticket;       // kerberos票据
			std::string _yycookie;     // yy客户端系统使用的cookie
			std::string _cookie;     // 后端系统使用的cookie
			LoginData(): _yyid(0), _udbuid(0){}
			virtual void marshal(sox::Pack &pk) const
			{
				pk  << _yyid << _udbuid << _passport << _accessToken << _accountInfo << _jump_tokens << _ticket << _yycookie<< _cookie;
			}
			virtual void unmarshal(const sox::Unpack &up)
			{
				up  >> _yyid >> _udbuid >> _passport >> _accessToken >> _accountInfo >> _jump_tokens >> _ticket >> _yycookie>> _cookie ;
			}
		};


		struct JumpToken:public sox::Marshallable
		{
			std::string _appid; // 可跳转标识
			std::string _token; // 跳转使用的凭证
			uint32_t    _result;
			std::string _reason;
			virtual void marshal(sox::Pack &pk) const
			{
				pk << _appid << _token << _result << _reason;
			}
			virtual void unmarshal(const sox::Unpack &up)
			{
				up >> _appid >> _token >> _result >> _reason;
			}
		};
		virtual void marshal( sox::Pack &pk )const
		{

			pk << _context << _rescode << _reason << _description << _yyuid 
				<< dynamic_token_reqs << _pic_id << _pic_data << _login_data  << _callback_data
			    << _extension;
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			up >> _context >> _rescode >> _reason >> _description >> _yyuid
				>> dynamic_token_reqs  >> _pic_id >> _pic_data >> _login_data >> _callback_data
				>> _extension;

		}
	};


	struct UDBLoginReqV2:public UDBYYLoginReq
	{
		enum{uri=LG_URI_UDB_LOGIN_REQ}; //
	};

	struct UDBLoginResV2:public UDBYYLoginRes
	{
		enum{uri=LG_URI_UDB_LOGIN_RES}; //
	};

	struct UDBMobLoginReq:public UDBYYLoginReq
	{
		enum{uri=LG_URI_MOB_LOGIN_REQ}; //
	};

	struct UDBMobLoginRes:public UDBYYLoginRes
	{
		enum{uri=LG_URI_MOB_LOGIN_RES}; //
	};
	
	/*下发短信验证码版本2.0*/
	
	struct UDBSendSmscodeReqV2:public sox::Marshallable
	{
		enum{uri=LG_URI_SEND_SMSCODE_REQ};
        std::string _context;       // 消息流水
		std::string _appid;         // 业务标识号
		uint8_t     _apptype;        //业务类型 
		/*
		1.	win PC APP 
		2.	Android APP（包括手机和pad）
		3.	IOS APP（包括手机和ipad）
		4.	MAC APP
		5.	WEB PC
		6.	WEB MObile 
		7.	linux APP
		*/

		std::string _device_id;     // 设备标识：PC端网卡为物理地址，移动端为imid标识, web端可选
		std::string _client_ip;     // 客户端ip
		uint32_t    _ver_int;       // 客户端整数版本号
		std::string _ver_str;       // 客户端字符串版本号
		std::string _version;       // 协议版本号


		uint64_t    _yyuid;           // 当user_token_type=2,4必填
		std::string _user;            // YY通行证，YY号，手机号，email。当user_token_type=1,3必填
		uint8_t     _user_token_type; // 1 - xxtea加密的一次sha1密码,  2 - 票据 - 3 accinfo, 4 opt
		std::string _user_token;     //  口令或身份信息。根据_user_token_type取值决定填入内容

		std::string _callback_data;     //由UDBYYLoginRes返回的字段值
		std::string _extension;     // 扩展预留字段

		bool is_login_pwd()const{return 1 == _user_token_type;}
		bool is_login_ticket()const {return 2 == _user_token_type;}
		bool is_login_acctinfo()const {return 3 == _user_token_type;}
		bool is_login_otp()const {return 4 == _user_token_type;}
		UDBSendSmscodeReqV2():_apptype(0),_ver_int(0),_yyuid(0),_user_token_type(0){}
		UDBSendSmscodeReqV2(const UDBSendSmscodeReqV2&rth)
		{
			_context = rth._context;
			_appid = rth._appid;
			_apptype = rth._apptype;

			_device_id = rth._device_id;
			_client_ip = rth._client_ip;
			_ver_int = rth._ver_int;
			_ver_str = rth._ver_str;
			_version = rth._version;
			
			_yyuid              = rth._yyuid;
			_user               = rth._user;
			_user_token_type    = rth._user_token_type;
			_user_token         = rth._user_token;
			_callback_data = rth._callback_data;
			_extension = rth._extension;
			
		}
		UDBSendSmscodeReqV2& operator=(const UDBSendSmscodeReqV2& rth)
		{
			if (this == &rth)
			{
				return *this;
			}
			_context = rth._context;
			_appid = rth._appid;
			_apptype = rth._apptype;

			_device_id = rth._device_id;
			_client_ip = rth._client_ip;
			_ver_int = rth._ver_int;
			_ver_str = rth._ver_str;
			_version = rth._version;
			
			_yyuid              = rth._yyuid;
			_user               = rth._user;
			_user_token_type    = rth._user_token_type;
			_user_token         = rth._user_token;
			_callback_data = rth._callback_data;
			_extension = rth._extension;

			return *this;
		}
		
        struct OtpCode : public sox::Marshallable
        {
        
          std::string sessionid;
          std::string otp; 
          std::string otp_appid;
          virtual void marshal(sox::Pack &pk) const 
          {
            pk << sessionid << otp << otp_appid;
          }
          virtual void unmarshal(const sox::Unpack &up)
          {
            up >> sessionid >> otp >> otp_appid;
          }
        
        };


		virtual void marshal( sox::Pack &pk )const
		{
			pk << _context << _appid << _apptype  <<  _device_id << _client_ip 
				<< _ver_int    << _ver_str << _version  << _yyuid << _user << _user_token_type
				<< _user_token << _callback_data << _extension;;

		}
		virtual void unmarshal(const sox::Unpack &up)
		{
            up >> _context >> _appid >> _apptype >>  _device_id >> _client_ip 
				>> _ver_int    >> _ver_str >> _version  >> _yyuid >> _user >> _user_token_type
				>> _user_token >> _callback_data >> _extension;
		}
	};

	struct UDBSendSmscodeResV2:public sox::Marshallable
	{
		enum{uri=LG_URI_SEND_SMSCODE_RES}; 
		enum
		{
			SSRES_SUCCESS = 0,       // 成功
			SSRES_PWD_ERR = 1,       // 账号或密码错误
			SSRES_TICKET_ERR = 2,    // 票据验证失败
			SSRES_ACCINFO_ERR = 3,   // Acctinfo验证失败
			SSRES_OTP_ERR = 4,       // otp验证失败
			SSRES_BIND_NO_MOBILE = 5, // 用户未绑定手机
			SSRES_REJECT = 6,        // 非法操作拒绝处理
			SSRES_SMSCODE_FAIL = 7,  // 下发短信验证码失败
			SSRES_SMSCODE_LIMIT = 8, // 下发短信验证码次数超出次数限制
			SSRES_USER_NOT_EXIST = 97, // 用户不存在
			SSRES_RETRY = 99,        // 客户端需重试
			SSRES_PARAM_ERR = 100    // 参数错误
		};
		std::string _context;      // 消息流水
		uint32_t    _rescode;      // 结果码，见上面的SDRES开头的枚举值
		std::string _reason;       // 内部失败原因描述
		std::string _description; // 失败原因：富文本，用于向用户展示
		std::string _extension;    // 扩展字段，如果为空，则该字段不出现
		UDBSendSmscodeResV2():_rescode(0){}
		virtual void marshal( sox::Pack &pk )const
		{
			pk << _context << _rescode << _reason << _description <<  _extension;
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			up >> _context >> _rescode >> _reason >> _description >> _extension;

		}
	};
	


	/*下发短信验证码版本2.0*/
	
	struct UDBSendSmscodeByTelNo:public sox::Marshallable
	{
		enum{uri=LG_URI_SEND_SMSCODE_BY_TELNO_REQ};
        std::string _context;       // 消息流水
		std::string _appid;         // 业务标识号
		uint8_t     _termtype;        //终端类型 
		/*
		1.	win PC APP 
		2.	Android APP（包括手机和pad）
		3.	IOS APP（包括手机和ipad）
		4.	MAC APP
		5.	WEB PC
		6.	WEB MObile 
		7.	linux APP
		*/
		std::string _device_id;     // 设备标识：PC端网卡为物理地址，移动端为imid标识, web端可选
		std::string _client_ip;     // 客户端ip
		uint32_t    _ver_int;       // 客户端整数版本号
		std::string _ver_str;       // 客户端字符串版本号

		std::string _vmToken;       // 用于虚拟机验证的token(可选)
		    
		std::string _mobile;        //手机号码

		std::string _callback_data; //预留
		std::string _extension;     // 扩展预留字段


		UDBSendSmscodeByTelNo():_termtype(0),_ver_int(0){}
		UDBSendSmscodeByTelNo(const UDBSendSmscodeByTelNo&rth)
		{
			_context = rth._context;
			_appid = rth._appid;
			_termtype = rth._termtype; 
			_device_id = rth._device_id;
			_client_ip = rth._client_ip;
			_ver_int = rth._ver_int;
			_ver_str = rth._ver_str;
			_vmToken = rth._vmToken;
			
			_mobile              = rth._mobile;

			_callback_data = rth._callback_data;
			_extension = rth._extension;
			
		}
		UDBSendSmscodeByTelNo& operator=(const UDBSendSmscodeByTelNo& rth)
		{
			if (this == &rth)
			{
				return *this;
			}
			_context = rth._context;
			_appid = rth._appid;
			_termtype = rth._termtype; 
			_device_id = rth._device_id;
			_client_ip = rth._client_ip;
			_ver_int = rth._ver_int;
			_ver_str = rth._ver_str;
			_vmToken = rth._vmToken;
			
			_mobile         = rth._mobile;
			_callback_data = rth._callback_data;
			_extension = rth._extension;

			return *this;
		}
		
		virtual void marshal( sox::Pack &pk )const
		{
			pk << _context << _appid   << _termtype << _device_id << _client_ip 
				<< _ver_int    << _ver_str << _vmToken  << _mobile  
				<< _callback_data << _extension;;

		}
		virtual void unmarshal(const sox::Unpack &up)
		{
            up >> _context >> _appid  >> _termtype >>  _device_id >> _client_ip 
				>> _ver_int    >> _ver_str >> _vmToken  >> _mobile 
				 >> _callback_data >> _extension;
		}
	};
	
    enum UNI_LOGIN_ERR_CODE
    {
        UNI_RES_SUCCESS               = 0,       // 成功
        UNI_RES_PARAM_ERR             = MK_BUSERR_CODE(LM_UNI_LOGIN, 1), // 参数错误
        UNI_RES_PASSWD_ERR            = MK_BUSERR_CODE(LM_UNI_LOGIN, 2), // 密码错误
        UNI_RES_USER_NOT_EXIST        = MK_BUSERR_CODE(LM_UNI_LOGIN, 3), // 用户名不存在
        UNI_RES_NEED_PICCODE_VERIFY   = MK_BUSERR_CODE(LM_UNI_LOGIN, 4), // 需要图片验证码验证
        UNI_RES_REJECT                = MK_BUSERR_CODE(LM_UNI_LOGIN, 5), // 非法操作拒绝处理
        UNI_RES_LOCKED                =  MK_BUSERR_CODE(LM_UNI_LOGIN, 6),// 账号被锁定（用户已设置账号锁）
        UNI_RES_FROZEN                =  MK_BUSERR_CODE(LM_UNI_LOGIN, 7),// 账号临时冻结（违反安全策略）
        UNI_RES_SENT_SMSCODE_FAIL          = MK_BUSERR_CODE(LM_UNI_LOGIN, 8), // 下发短信验证码失败
        UNI_RES_SENT_SMSCODE_LIMIT         = MK_BUSERR_CODE(LM_UNI_LOGIN, 9), // 下发短信验证码次数超出次数限制
        UNI_RES_SMSCODE_EXPIRE         = MK_BUSERR_CODE(LM_UNI_LOGIN, 10), // 短信已过期
        UNI_RES_INVALID_SMSCODE         = MK_BUSERR_CODE(LM_UNI_LOGIN, 11), // 无效验证码(未发送过的)
        UNI_RES_SMSCODE_ERROR         = MK_BUSERR_CODE(LM_UNI_LOGIN, 12), // 短信验证错误:
        UNI_RES_SMS_VFY_TIMES_LIMIT  = MK_BUSERR_CODE(LM_UNI_LOGIN, 13),// 短信验证次数过多
        UNI_RES_PICCODE_ERR           =  MK_BUSERR_CODE(LM_UNI_LOGIN,14),// 图片验证码错误
        UNI_RES_SERVER_BANNED         = MK_BUSERR_CODE(LM_UNI_LOGIN, 15),// 运营封禁
        //用于移动登录凭证
        UNI_RES_MOB_S1_ERROR           =  MK_BUSERR_CODE(LM_UNI_LOGIN, 16),// 种子凭证错误/无效
        UNI_RES_MOB_S1_EXPIRE          =  MK_BUSERR_CODE(LM_UNI_LOGIN, 17),// 种子凭证过期
        UNI_RES_MOB_DEVICE_ID_UNMATCH  =  MK_BUSERR_CODE(LM_UNI_LOGIN, 18),// 
        UNI_RES_MOB_MS0_ERROR          =  MK_BUSERR_CODE(LM_UNI_LOGIN, 19),// 登录凭证错误
        UNI_RES_MOB_MS1_ERROR          =  MK_BUSERR_CODE(LM_UNI_LOGIN, 20),//登录子凭证错误
        UNI_RES_MOB_CNONCE_INVALID     =  MK_BUSERR_CODE(LM_UNI_LOGIN, 21),// 子凭证无效
        UNI_RES_MOB_MS1_BEEN_USED      =  MK_BUSERR_CODE(LM_UNI_LOGIN, 22),// 子凭证重复使用

        UNI_RES_RETRY                  = MK_SYSERR_CODE(LM_UNI_LOGIN, 99), // 客户端需重试，系统错误可以暂时不需要细分

    };
    
	struct UDBSendSmscodeByTelNoRes: public sox::Marshallable
	{
		enum{uri=LG_URI_SEND_SMSCODE_BY_TELNO_RES}; 

		std::string _context;      // 消息流水
		uint32_t    _rescode;      // 结果码，见上面的SDRES开头的枚举值
		std::string _reason;       // 内部失败原因描述
		std::string _description; // 失败原因：富文本，用于向用户展示
		std::string _extension;    // 扩展字段，如果为空，则该字段不出现
		UDBSendSmscodeByTelNoRes():_rescode(0){}
		virtual void marshal( sox::Pack &pk )const
		{
			pk << _context << _rescode << _reason << _description <<  _extension;
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			up >> _context >> _rescode >> _reason >> _description >> _extension;

		}
	};

	struct UDBMobileNoLogin:public sox::Marshallable
	{
		enum{uri=LG_URI_MOBILE_NO_LOGIN_REQ};
		std::string _context;       // 消息流水
		std::string _appid;         // 业务标识号
		uint8_t     _termtype;        //业务类型 
		/*
		1.	win PC APP 
		2.	Android APP（包括手机和pad）
		3.	IOS APP（包括手机和ipad）
		4.	MAC APP
		5.	WEB PC
		6.	WEB MObile 
		7.	linux APP
		*/
		
		std::string _device_id;     // 设备标识：PC端网卡为物理地址，移动端为imid标识, web端可选
		std::string _client_ip;     // 客户端ip
		uint32_t    _ver_int;       // 客户端整数版本号
		std::string _ver_str;       // 客户端字符串版本号
		std::string _version;       // 协议版本号

		std::string _mobile;
		std::string _smscode;
		
		std::string _pic_id;   //图片验证码编号:操作太频繁，需要图片
	    std::string _pic_code; //
	       
		std::string _vmToken;       // 用于虚拟机验证的token.
		
		    
        std::string _callback_data;     // 首次请求登录时为空。如果需要进一步的交互验证，则服务端会生成一个数据返回到客户端，后续验证阶段均需要传递该值。
		std::string _extension;     // 扩展字段，可选字段，在二进制流中也可以不出现
		//
		
		UDBMobileNoLogin():_termtype(0),_ver_int(0){}
		UDBMobileNoLogin(const UDBMobileNoLogin& rth)
		{
			_context = rth._context;
			_appid = rth._appid;
			_termtype = rth._termtype;
			_device_id = rth._device_id;
			_client_ip = rth._client_ip;
			_ver_int = rth._ver_int;
			_ver_str = rth._ver_str;
			_version = rth._version;
			_mobile     = rth._mobile;
			_smscode    = rth._smscode;
			_pic_id             = rth._pic_id;
			_pic_code           = rth._pic_code;
			_vmToken            = rth._vmToken;

			_callback_data      = rth._callback_data;
			_extension = rth._extension;

		}
		

        
        struct AntiCode : public sox::Marshallable
        {
        
          std::string bizName;// 反外挂虚拟机类别
          std::string antiCode; 
          
          virtual void marshal(sox::Pack &pk) const 
          {
            pk << bizName << antiCode;
          }
          virtual void unmarshal(const sox::Unpack &up)
          {
            up >> bizName >> antiCode;
          }
        
        };
        
     
		UDBMobileNoLogin& operator=(const UDBMobileNoLogin& rth)
		{
			if (this == &rth)
			{
				return *this;
			}
			_context = rth._context;
			_appid = rth._appid;
			_termtype = rth._termtype;
			_device_id = rth._device_id;
			_client_ip = rth._client_ip;
			_ver_int = rth._ver_int;
			_ver_str = rth._ver_str;
			_version = rth._version;

			_mobile     = rth._mobile;
			_smscode    = rth._smscode;

			_pic_id             = rth._pic_id;
			_pic_code           = rth._pic_code;

			_vmToken            = rth._vmToken;

			_callback_data      = rth._callback_data;
			_extension = rth._extension;
			return *this;
		}



		virtual void marshal( sox::Pack &pk )const
		{
			pk << _context << _appid << _termtype <<  _device_id << _client_ip 
				<< _ver_int    << _ver_str << _version << _mobile << _smscode 
				<<  _pic_id << _pic_code << _vmToken   << _callback_data;
			if (!_extension.empty())
			{
				pk << _extension;
			}
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			up >> _context >> _appid >> _termtype >>  _device_id >> _client_ip 
				>> _ver_int >> _ver_str >> _version >> _mobile >> _smscode
				>> _pic_id >> _pic_code >> _vmToken  >> _callback_data;
			if (!up.empty())
			{
				up >> _extension;
			}
		}
	};

	struct UDBMobileNoLoginRes:public sox::Marshallable
	{
		enum{uri=LG_URI_MOBILE_NO_LOGIN_RES};

		std::string _context;      // 消息流水
		uint32_t    _rescode;      // 结果码，见上面的ENU_LRES枚举值
		std::string _reason;       // 针对错误码的错误信息,主要用于日志打印 
		std::string _description;  // 失败原因描述，用于对用户展示,成功时为空，格式可定制(json格式，包括：title/content)
		uint64_t    _yyuid;        // yyuid
		
		std::string _pic_id;      // _图片验证码编号
		std::string _pic_data;    //  图片验证码 
		std::string _authToken;   //  返回登录凭证，替换密码
		std::string _login_data;   // 登录返回，如果登录成功(rescode=URES_SUCCESS)返回LoginData结构数据，否则为空

		std::string _callback_data;
		std::string _extension;    // 扩展字段，备用，为空时不出现

		

		virtual void marshal( sox::Pack &pk )const
		{

			pk << _context << _rescode << _reason << _description << _yyuid 
				 << _pic_id << _pic_data << _authToken << _login_data  << _callback_data
			    << _extension;
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			up >> _context >> _rescode >> _reason >> _description >> _yyuid
			    >> _pic_id >> _pic_data >> _authToken >> _login_data >> _callback_data
				>> _extension;

		}
	};


	/*验密码接口*/
	struct UDBVerifyPwdReq:public sox::Marshallable
	{
		enum{uri=URI_UDBVERIFY_PWD_REQ}; // (6 << 8)|233
		std::string _context;   // 消息流水
		std::string _appid;     // 业务标识号
		std::string _client_ip; // 客户端ip
		uint64_t _yyuid;   // 用户 yyuid 
		std::string _pwd;       // 密码(明文密码sha1一次，再做XXTEA加密）
		virtual void marshal( sox::Pack &pk )const
		{
			pk << _context << _appid << _client_ip << _yyuid << _pwd;
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			up >> _context >> _appid >> _client_ip >> _yyuid >> _pwd;
		}
	};

	struct UDBVerifyPwdRes:public sox::Marshallable
	{
		enum{uri=URI_UDBVERIFY_PWD_RES}; // (7 << 8)|233
		std::string _context;   // 消息流水
		uint32_t _rescode;               // 0 - 成功, 1 - 密码错误或账号不存在, 99 - 服务暂时不可用
		UDBVerifyPwdRes():_context(""),_rescode(0){}
		virtual void marshal( sox::Pack &pk )const
		{
			pk << _context << _rescode;
		}
		virtual void unmarshal(const sox::Unpack &up)
		{
			up >> _context >> _rescode;
		}
	};
                                
    const int  URI_GETANTICODE          = (3048 << 8) |  4;
    const int  URI_GETANTICODERES       = (3049 << 8) |  4;
 

    // AP经dispatcher到UDB 获取反外挂代码请求里层包
    struct UDB_GetAntiCode : public sox::Marshallable
    {
      enum {uri = URI_GETANTICODE};
    
      std::string context;
      std::string bizName; // 反外挂虚拟机类别
      
      virtual void marshal(sox::Pack &pk) const 
      {
        pk << context << bizName;
      }
      virtual void unmarshal(const sox::Unpack &up)
      {
        up >> context >> bizName;
      }
    
    };
    
    // UDB经dispatcher到AP 获取反外挂代码响应里层包
    struct UDB_GetAntiCodeRes : public sox::Marshallable
    {
      enum {uri = URI_GETANTICODERES};
    
      std::string context;
      std::string antiCode;
    
      virtual void marshal(sox::Pack &pk) const 
      {
        pk << context << antiCode;
      }
      virtual void unmarshal(const sox::Unpack &up)
      {
        up >> context >> antiCode;
      }
    };



    struct UDB_CreateAcctInfo : public sox::Marshallable
    {
      enum {uri = URI_UDB_CREATE_ACCTINFO};
    
      std::string _context;
      uint64_t    _yyuid;
      std::string _access_token;
      
      
      virtual void marshal(sox::Pack &pk) const 
      {
        pk << _context << _yyuid << _access_token;
      }
      virtual void unmarshal(const sox::Unpack &up)
      {
        up >> _context >> _yyuid >> _access_token;
      }
    
    };

    struct UDB_CreateAcctInfoV2 : public sox::Marshallable
    {
      enum {uri = URI_UDB_CREATE_ACCTINFO_V2};
    
      std::string _context;
      uint8_t     _termtype;        //业务类型 
      /*
      1.	win PC APP 
      2.	Android APP（包括手机和pad）
      3.	IOS APP（包括手机和ipad）
      4.	MAC APP
      5.	WEB PC
      6.	WEB MObile 
      7.	linux APP
     */
      std::string _device_id;     // 设备标识：PC端网卡为物理地址，移动端为imid标识, web端可选
      std::string _client_ip;     // 客户端ip
      std::string _passport;
      uint64_t    _yyuid;
      std::string _access_token;
      std::string _appid; 
      
      virtual void marshal(sox::Pack &pk) const 
      {
        pk << _context << _termtype << _device_id << _client_ip << _passport << _yyuid << _access_token << _appid;
      }
      virtual void unmarshal(const sox::Unpack &up)
      {
        up >> _context >> _termtype >> _device_id >> _client_ip >> _passport >> _yyuid >> _access_token >> _appid;
      }
    
    };

    struct UDB_CreateAcctInfoRes : public sox::Marshallable
    {
      enum {uri = URI_UDB_CREATE_ACCTINFO_RES};
    
      std::string _context;
      uint32_t    _rescode;// 1 yyuid对应的用户不存在 2 产acctinfo失败  99 参数错误 100 系统错误
      uint64_t    _yyid;
      std::string _passport;
      std::string _acctinfo;
       
      virtual void marshal(sox::Pack &pk) const 
      {
        pk << _context << _rescode <<_yyid <<  _passport << _acctinfo;
      }
      virtual void unmarshal(const sox::Unpack &up)
      {
        up >> _context >> _rescode >> _yyid >> _passport >> _acctinfo;
      }
    };

}
}
// namespace server{ namespace loginUDB{

#endif //_login_udb_i_H_2014_3_12
