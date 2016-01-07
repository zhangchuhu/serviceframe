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

//�淶������
// sys_err, bus_err��Χ������100����
#define MK_SYSERR_CODE(mud_id,sys_err)   (mud_id)*10000 + (sys_err)*100
#define MK_BUSERR_CODE(mud_id,bus_err)   (mud_id)*10000 + (bus_err)


namespace server{ namespace loginUDB
{
	enum ENU_DYNAMIC_STRATEGY
	{
		DS_NONE  = 0x0,         // �޶�̬����
		DS_PICCODE = 0x01,      // ͼƬ��֤��
		DS_MOBTOKEN = 0x02,     // �ֻ�����
		DS_HWTOKEN = 0x04,      // Ӳ������
		DS_SMSCODE = 0x08,      // ������֤��
		DS_SEC_QESTION = 0x10   // �ܱ�����
	};

	enum ENU_STATIC_TOKEN_TYPE
	{
		STT_PASSWD =  0,  // xxtea���ܵ�һ��sha1����
		STT_TICKET =  1,  // kerberosƱ��
		STT_ACCINFO=  2,  // accinfo
		STT_OTPCODE = 3,  // һ���Կ���
		STT_YYTOKEN = 4   // ��������¼YYʱ������ƾ֤
	};

	struct UDBLoginReq:public sox::Marshallable
	{
		enum{uri=URI_UDBLOGIN_REQ}; // (10 << 8)|233
		std::string _context;       // ��Ϣ��ˮ
		std::string _appid;         // ҵ���ʶ��
		std::string _mac_address;   // �ͻ���mac��ַ
		std::string _client_ip;     // �ͻ���ip
		uint32_t    _ver_int;       // �����汾��
		std::string _ver_str;       // �ַ����汾��
		uint8_t     _login_type;    // ��¼���ͣ� 0 - ��̬��֤, 1- ��̬��֤
		uint16_t    _dynamic_stra;  // ��̬��֤���ԣ���ENU_DYNAMIC_STRATEGYö��ֵ
		std::string _user;          // �û��������ǣ�YYͨ��֤��YY�ţ��ֻ��ţ�email
		uint8_t     _stt;           // ��̬�������͡�0 - ����sha1���룬 1 - Ʊ��
		std::string _static_token;  // ��̬�������SHA1�������YY�ͻ��˵�¼���ɵ�Ʊ��
		/*��̬���
		1. Ϊ�ֻ����ƻ�Ӳ������ʱ����Ҫ��һ������
		2. ΪͼƬ��֤��ʱ����LoginPicCode
		3. Ϊ�ܱ�����ʱ����LoginSecq
		*/
		std::string _dynamic_token;
		std::vector<std::string> _jump_appids; // ��תҵ���ʶ
		std::vector<std::string> _jump_sessionids; // ���appid��Ӧû��session_id�����Ӧsession_idΪ��
		std::string _extension;     // ��չ�ֶΣ���ѡ�ֶΣ��ڶ���������Ҳ���Բ�����

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
			URES_SUCCESS = 0,             // �ɹ�
			URES_STATIC_VERIFY = 1,       // ��Ҫ��һ����̬��֤���·���ͼƬ��֤�룩
			URES_DYNAMIC_VERIFY = 2,      // ��Ҫ���ж�̬��֤
			URES_PWD_ERR = 3,             // �˺Ż��������
			URES_TICKET_ERR = 4,          // Ʊ����֤ʧ��
			URES_LOCKED = 5,              // �˺ű�����
			URES_FROZEN = 6,              // �˺ű���ʱ����
			URES_REJECT = 7,              // �Ƿ������ܾ�����
			URES_MOB_TOKEN_ERR = 8,       // �ֻ����ƴ���
			URES_HW_TOKEN_ERR = 9,        // Ӳ�����ƴ���
			URES_PICCODE_ERR = 10,        // ͼƬ��֤�����
			URES_SEC_QUESTION_ERR  =  11, // �ܱ�������֤����
			URES_SMS_CODE_ERR  =  12,     // ������֤����
			USER_YID_LOGIN_REJECT = 13,   // ��Yy�ŵ�¼Ȩ��
			USER_NO_LOGIN_AUTH    = 14,   // �޵�¼Ȩ��
			USER_NO_WEB_LOGIN_AUTH  = 21,     // web���
			USER_USER_NOT_EXIST  = 97,     // �û�������
			URES_RETRY = 99,              // ������
			URES_PARAM_ERR = 100          // ��������
		};
		
		
		std::string _context;      // ��Ϣ��ˮ
		uint32_t    _rescode;      // ����룬�������ENU_URESö��ֵ
		std::string _reason;       // ʧ��ԭ���������ɹ�ʱΪ�գ���ʽ�ɶ���(json��ʽ)
		uint16_t    _dynamic_stra; // ��̬��֤���ԣ�bitλ��϶��ֲ��ԣ��μ�ENU_DYNAMIC_STRATEGYö��ֵ
		std::string _login_data;   // ��¼���أ������¼�ɹ�(rescode=URES_SUCCESS)����LoginData�ṹ���ݣ�����Ϊ��
		std::string _next_need;    // �´���֤��Ҫ�����ݣ�����rescode����������ʽ
		std::vector<std::string> _jump_tokens; // ��תʹ�õĵ�¼ƾ֤�������ÿһά�ĸ�ʽ��JumpToken����
		std::string _extension;    // ��չ�ֶΣ����ã�Ϊ��ʱ������

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
			std::string  _reason;   // ��ͼƬ��ԭ�򣬿ɶ���

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
			std::string _appid; // ����ת��ʶ
			std::string _token; // ��תʹ�õ�ƾ֤
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

	/*�·�������֤��*/
	struct UDBSendSmscodeReq:public sox::Marshallable
	{
		enum{uri=URI_UDBSEND_SMSCODE_REQ}; // (16 << 8)|233
		std::string _context;       // ��Ϣ��ˮ
		std::string _appid;         // ҵ���ʶ��
		std::string _mac_address;   // �ͻ���mac��ַ
		std::string _client_ip;     // �ͻ���ip
		uint32_t    _ver_int;       // �����汾��
		std::string _ver_str;       // �ַ����汾��
		std::string _user;          // �û��������ǣ�YYͨ��֤��YY�ţ��ֻ��ţ�email
		uint8_t     _stt;           // ��̬�������͡��ο�ö��ֵ��ENU_STATIC_TOKEN_TYPE
		std::string _static_token;  // ��̬����
		std::string _extension;     // ��չԤ���ֶ�

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
			SSRES_SUCCESS = 0,       // �ɹ�
			SSRES_SMSCODE_FAIL = 1,  // �·�������֤��ʧ��
			SSRES_PWD_ERR = 2,       // �˺Ż��������
			SSRES_TICKET_ERR = 3,    // Ʊ����֤ʧ��
			SSRES_REJECT = 4,        // �Ƿ������ܾ�����
			SSRES_RETRY = 99,        // �ͻ���������
			SSRES_PARAM_ERR = 100    // ��������
		};
		std::string _context;      // ��Ϣ��ˮ
		uint32_t    _rescode;      // ����룬�������SDRES��ͷ��ö��ֵ
		std::string _reason;       // ʧ��ԭ������
		std::string _extension;    // ��չ�ֶΣ����Ϊ�գ�����ֶβ�����
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
		VFY_NONE  = 0x0,         // �޶�̬����
		VFY_MOBTOKEN = 0x02,     // �ֻ�����
		VFY_HWTOKEN = 0x04,      // Ӳ������
		VFY_SMSCODE = 0x08,      // ������֤��
		VFY_SEC_QESTION = 0x10   // �ܱ�����
	};
	
	struct UDBYYLoginReq:public sox::Marshallable
	{
		enum{uri=URI_UDBYYLOGIN_REQ}; // (8 << 8)|233
		std::string _context;       // ��Ϣ��ˮ
		std::string _appid;         // ҵ���ʶ��
		uint8_t     _apptype;        //ҵ������ 
		/*
		1.	win PC APP 
		2.	Android APP�������ֻ���pad��
		3.	IOS APP�������ֻ���ipad��
		4.	MAC APP
		5.	WEB PC
		6.	WEB MObile 
		7.	linux APP
		*/
		uint32_t    _lcid;         //���Ա�־ 2052�������� 
		
		uint64_t    _terminal_type; // 
		std::string _device_id;     // �豸��ʶ��PC������Ϊ�����ַ���ƶ���Ϊimid��ʶ, web�˿�ѡ
		std::string _client_ip;     // �ͻ���ip
		uint32_t    _ver_int;       // �ͻ��������汾��
		std::string _ver_str;       // �ͻ����ַ����汾��
		std::string _version;       // Э��汾��


		uint64_t    _yyuid;           // ��user_token_type=2,4����
		std::string _user;            // YYͨ��֤��YY�ţ��ֻ��ţ�email����user_token_type=1,3����
		uint8_t     _user_token_type; // 1 - xxtea���ܵ�һ��sha1����,  2 - Ʊ�� - 3 accinfo, 4 opt 5s1 6 s2 7 md5+sha1����(CombPasswd) 8����smscode��¼, 9 webyy��appidtype��acctinfo
		std::string _user_token;     //  ����������Ϣ������_user_token_typeȡֵ������������
		    
		std::string _pic_id;   //ͼƬ��֤���ţ�������˷�����Ҫ��֤ͼƬʱ��д(��ǰһ�ε�¼��Ӧ����rescode=1ʱ)
	    std::string _pic_code; //ͼƬ��֤��,������˷�����Ҫ��֤ͼƬʱ��д(��ǰһ�ε�¼��Ӧ����rescode=1ʱ)
	    
		uint16_t    _dynamic_token_type;  // ��Ҫ��֤����֤��Ŀ����ENU_DYNAMIC_VFY_OPTIONö��ֵ ǰһ�ε�¼��Ӧ����rescode=2ʱ) ���ε�¼�������
		/*
		��Ҫ��֤����֤���ݣ�����
		1.	�ֻ�����
        2.	Ӳ������
        3.	Ϊ�ܱ������LoginSecq
        4.  ������֤��
        */
		std::string _dynamic_token;      
     
		std::string _vmToken;       // �����������֤��token.
		
		std::vector<std::string> _jump_appids;     // ��תҵ���ʶ
		std::vector<std::string> _jump_sessionids; // ���appid��Ӧû��session_id�����Ӧsession_idΪ��
		    
        std::string _callback_data;     // �״������¼ʱΪ�ա������Ҫ��һ���Ľ�����֤�������˻�����һ�����ݷ��ص��ͻ��ˣ�������֤�׶ξ���Ҫ���ݸ�ֵ��
		std::string _extension;     // ��չ�ֶΣ���ѡ�ֶΣ��ڶ���������Ҳ���Բ�����
		//

		bool is_login_pwd()const{return 1 == _user_token_type;}

		bool is_login_ticket()const {return 2 == _user_token_type;}
		
		bool is_login_acctinfo()const {return 3 == _user_token_type || 9 == _user_token_type;}
		
		bool is_login_comb_acctinfo()const { return 9 == _user_token_type;}
		
		bool is_login_otp()const {return 4 == _user_token_type;}
		
		bool is_mob_login_mob_accesstoken()const {return 5 == _user_token_type;}

		bool is_mob_login_mob_otp()const {return 6 == _user_token_type;}
		//������룬 �ȴ���md5���룬�ִ���sha1����
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
        
          std::string acctinfo;// �������������
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
        
          std::string md5passwd;// �������������
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
        
          std::string bizName;// �������������
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
			LRES_SUCCESS = 0,             // �ɹ�
			LRES_NEED_PICCODE_VERIFY = 1, // ��ҪͼƬ��֤����֤
			LRES_NEED_NEXT_VERIFY = 2,    // ��Ҫ��һ��ǿ��֤���ֻ�����/�ܱ�����/�ֻ�����/Ӳ�����ƣ�������Ӧ�����ѡһ����
			LRES_PWD_ERR = 3,             // �������
			LRES_TICKET_ERR = 4,          // Ʊ����֤ʧ��
			LRES_LOCKED = 5,              // �˺ű��������û��������˺�����
			LRES_FROZEN = 6,              // �˺���ʱ���ᣨΥ����ȫ���ԣ�
			LRES_REJECT = 7,              // �Ƿ������ܾ�����
			LRES_MOB_TOKEN_ERR = 8,       // �ֻ����ƴ���
			LRES_HW_TOKEN_ERR = 9,        // Ӳ�����ƴ���
			LRES_PICCODE_ERR = 10,         // ͼƬ��֤�����
			LRES_SEC_QUESTION_ERR  =  11,  // �ܱ�������֤����
			LRES_SMS_CODE_ERR  =  12,      // ������֤����
			LSER_YID_LOGIN_LIMIT = 13,     // δ��ͨyy�ŵ�¼Ȩ��
			LSER_NO_LOGIN_AUTH = 14,       // 
			LRES_ACCTINFO_ERR    = 15,     // acctinfo����
			LRES_OTP_ERR = 16,             // otp��֤ʧ��	
			LRES_SMS_VERIFY_LIMIT = 17,   // ������֤����֤�������ƣ���Ҫ������ֹ��¼
			LSER_SERVER_BANNED  = 18,     // ��Ӫ���
			LSER_NO_WEB_LOGIN_AUTH  = 21,     // web���
			//�����ƶ���¼
			LSER_MOB_S1_ERROR  = 101,        // ����ƾ֤����/��Ч
			LSER_MOB_S1_EXPIRE  = 102,        // ����ƾ֤����
			LSER_MOB_DEVICE_ID_UNMATCH  = 103, //
			LSER_MOB_MS0_ERROR  = 104,        // ��¼ƾ֤����
			LSER_MOB_MS1_ERROR  = 105,         //��¼��ƾ֤����
			LSER_MOB_CNONCE_INVALID  = 106,      // ��ƾ֤��Ч
			LSER_MOB_MS1_BEEN_USED  = 107,    // ��ƾ֤�ظ�ʹ��
			LSER_USER_NOT_EXIST  = 97,     // �û�������
			LRES_RETRY = 99,              // ������
			LRES_PARAM_ERR = 100          // ��������
		};

		std::string _context;      // ��Ϣ��ˮ
		uint32_t    _rescode;      // ����룬�������ENU_LRESö��ֵ
		std::string _reason;       // ��Դ�����Ĵ�����Ϣ,��Ҫ������־��ӡ 
		std::string _description;  // ʧ��ԭ�����������ڶ��û�չʾ,�ɹ�ʱΪ�գ���ʽ�ɶ���(json��ʽ��������title/content)
		uint64_t    _yyuid;        // yyuid
		std::vector< std::string >  dynamic_token_reqs; // ������·��Ķ�̬��֤���� �� DynamicTokenReq�ṹ
		
		std::string _pic_id;      // _ͼƬ��֤���� rescode = 1/10 ��Ч
		std::string _pic_data;    //  ͼƬ��֤��,png��ʽ rescode = 1/10 ��Ч
		std::string _login_data;   // ��¼���أ������¼�ɹ�(rescode=URES_SUCCESS)����LoginData�ṹ���ݣ�����Ϊ��


		std::string _callback_data;
		std::string _extension;    // ��չ�ֶΣ����ã�Ϊ��ʱ������

		UDBYYLoginRes():_rescode(99), _yyuid(0){}
		
		struct DynamicTokenReq:public sox::Marshallable
		{
			uint8_t token_type;// ��̬��֤���ԣ��� ENU_DYNAMIC_VFY_OPTION ö��ֵ����ͼƬ��֤��֮��ģ�
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
		    std::vector<std::string> _jump_tokens; // ��תʹ�õĵ�¼ƾ֤�������ÿһά�ĸ�ʽ��JumpToken����
			std::string _ticket;       // kerberosƱ��
			std::string _yycookie;     // yy�ͻ���ϵͳʹ�õ�cookie
			std::string _cookie;     // ���ϵͳʹ�õ�cookie
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
			std::string _appid; // ����ת��ʶ
			std::string _token; // ��תʹ�õ�ƾ֤
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
	
	/*�·�������֤��汾2.0*/
	
	struct UDBSendSmscodeReqV2:public sox::Marshallable
	{
		enum{uri=LG_URI_SEND_SMSCODE_REQ};
        std::string _context;       // ��Ϣ��ˮ
		std::string _appid;         // ҵ���ʶ��
		uint8_t     _apptype;        //ҵ������ 
		/*
		1.	win PC APP 
		2.	Android APP�������ֻ���pad��
		3.	IOS APP�������ֻ���ipad��
		4.	MAC APP
		5.	WEB PC
		6.	WEB MObile 
		7.	linux APP
		*/

		std::string _device_id;     // �豸��ʶ��PC������Ϊ�����ַ���ƶ���Ϊimid��ʶ, web�˿�ѡ
		std::string _client_ip;     // �ͻ���ip
		uint32_t    _ver_int;       // �ͻ��������汾��
		std::string _ver_str;       // �ͻ����ַ����汾��
		std::string _version;       // Э��汾��


		uint64_t    _yyuid;           // ��user_token_type=2,4����
		std::string _user;            // YYͨ��֤��YY�ţ��ֻ��ţ�email����user_token_type=1,3����
		uint8_t     _user_token_type; // 1 - xxtea���ܵ�һ��sha1����,  2 - Ʊ�� - 3 accinfo, 4 opt
		std::string _user_token;     //  ����������Ϣ������_user_token_typeȡֵ������������

		std::string _callback_data;     //��UDBYYLoginRes���ص��ֶ�ֵ
		std::string _extension;     // ��չԤ���ֶ�

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
			SSRES_SUCCESS = 0,       // �ɹ�
			SSRES_PWD_ERR = 1,       // �˺Ż��������
			SSRES_TICKET_ERR = 2,    // Ʊ����֤ʧ��
			SSRES_ACCINFO_ERR = 3,   // Acctinfo��֤ʧ��
			SSRES_OTP_ERR = 4,       // otp��֤ʧ��
			SSRES_BIND_NO_MOBILE = 5, // �û�δ���ֻ�
			SSRES_REJECT = 6,        // �Ƿ������ܾ�����
			SSRES_SMSCODE_FAIL = 7,  // �·�������֤��ʧ��
			SSRES_SMSCODE_LIMIT = 8, // �·�������֤�����������������
			SSRES_USER_NOT_EXIST = 97, // �û�������
			SSRES_RETRY = 99,        // �ͻ���������
			SSRES_PARAM_ERR = 100    // ��������
		};
		std::string _context;      // ��Ϣ��ˮ
		uint32_t    _rescode;      // ����룬�������SDRES��ͷ��ö��ֵ
		std::string _reason;       // �ڲ�ʧ��ԭ������
		std::string _description; // ʧ��ԭ�򣺸��ı����������û�չʾ
		std::string _extension;    // ��չ�ֶΣ����Ϊ�գ�����ֶβ�����
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
	


	/*�·�������֤��汾2.0*/
	
	struct UDBSendSmscodeByTelNo:public sox::Marshallable
	{
		enum{uri=LG_URI_SEND_SMSCODE_BY_TELNO_REQ};
        std::string _context;       // ��Ϣ��ˮ
		std::string _appid;         // ҵ���ʶ��
		uint8_t     _termtype;        //�ն����� 
		/*
		1.	win PC APP 
		2.	Android APP�������ֻ���pad��
		3.	IOS APP�������ֻ���ipad��
		4.	MAC APP
		5.	WEB PC
		6.	WEB MObile 
		7.	linux APP
		*/
		std::string _device_id;     // �豸��ʶ��PC������Ϊ�����ַ���ƶ���Ϊimid��ʶ, web�˿�ѡ
		std::string _client_ip;     // �ͻ���ip
		uint32_t    _ver_int;       // �ͻ��������汾��
		std::string _ver_str;       // �ͻ����ַ����汾��

		std::string _vmToken;       // �����������֤��token(��ѡ)
		    
		std::string _mobile;        //�ֻ�����

		std::string _callback_data; //Ԥ��
		std::string _extension;     // ��չԤ���ֶ�


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
        UNI_RES_SUCCESS               = 0,       // �ɹ�
        UNI_RES_PARAM_ERR             = MK_BUSERR_CODE(LM_UNI_LOGIN, 1), // ��������
        UNI_RES_PASSWD_ERR            = MK_BUSERR_CODE(LM_UNI_LOGIN, 2), // �������
        UNI_RES_USER_NOT_EXIST        = MK_BUSERR_CODE(LM_UNI_LOGIN, 3), // �û���������
        UNI_RES_NEED_PICCODE_VERIFY   = MK_BUSERR_CODE(LM_UNI_LOGIN, 4), // ��ҪͼƬ��֤����֤
        UNI_RES_REJECT                = MK_BUSERR_CODE(LM_UNI_LOGIN, 5), // �Ƿ������ܾ�����
        UNI_RES_LOCKED                =  MK_BUSERR_CODE(LM_UNI_LOGIN, 6),// �˺ű��������û��������˺�����
        UNI_RES_FROZEN                =  MK_BUSERR_CODE(LM_UNI_LOGIN, 7),// �˺���ʱ���ᣨΥ����ȫ���ԣ�
        UNI_RES_SENT_SMSCODE_FAIL          = MK_BUSERR_CODE(LM_UNI_LOGIN, 8), // �·�������֤��ʧ��
        UNI_RES_SENT_SMSCODE_LIMIT         = MK_BUSERR_CODE(LM_UNI_LOGIN, 9), // �·�������֤�����������������
        UNI_RES_SMSCODE_EXPIRE         = MK_BUSERR_CODE(LM_UNI_LOGIN, 10), // �����ѹ���
        UNI_RES_INVALID_SMSCODE         = MK_BUSERR_CODE(LM_UNI_LOGIN, 11), // ��Ч��֤��(δ���͹���)
        UNI_RES_SMSCODE_ERROR         = MK_BUSERR_CODE(LM_UNI_LOGIN, 12), // ������֤����:
        UNI_RES_SMS_VFY_TIMES_LIMIT  = MK_BUSERR_CODE(LM_UNI_LOGIN, 13),// ������֤��������
        UNI_RES_PICCODE_ERR           =  MK_BUSERR_CODE(LM_UNI_LOGIN,14),// ͼƬ��֤�����
        UNI_RES_SERVER_BANNED         = MK_BUSERR_CODE(LM_UNI_LOGIN, 15),// ��Ӫ���
        //�����ƶ���¼ƾ֤
        UNI_RES_MOB_S1_ERROR           =  MK_BUSERR_CODE(LM_UNI_LOGIN, 16),// ����ƾ֤����/��Ч
        UNI_RES_MOB_S1_EXPIRE          =  MK_BUSERR_CODE(LM_UNI_LOGIN, 17),// ����ƾ֤����
        UNI_RES_MOB_DEVICE_ID_UNMATCH  =  MK_BUSERR_CODE(LM_UNI_LOGIN, 18),// 
        UNI_RES_MOB_MS0_ERROR          =  MK_BUSERR_CODE(LM_UNI_LOGIN, 19),// ��¼ƾ֤����
        UNI_RES_MOB_MS1_ERROR          =  MK_BUSERR_CODE(LM_UNI_LOGIN, 20),//��¼��ƾ֤����
        UNI_RES_MOB_CNONCE_INVALID     =  MK_BUSERR_CODE(LM_UNI_LOGIN, 21),// ��ƾ֤��Ч
        UNI_RES_MOB_MS1_BEEN_USED      =  MK_BUSERR_CODE(LM_UNI_LOGIN, 22),// ��ƾ֤�ظ�ʹ��

        UNI_RES_RETRY                  = MK_SYSERR_CODE(LM_UNI_LOGIN, 99), // �ͻ��������ԣ�ϵͳ���������ʱ����Ҫϸ��

    };
    
	struct UDBSendSmscodeByTelNoRes: public sox::Marshallable
	{
		enum{uri=LG_URI_SEND_SMSCODE_BY_TELNO_RES}; 

		std::string _context;      // ��Ϣ��ˮ
		uint32_t    _rescode;      // ����룬�������SDRES��ͷ��ö��ֵ
		std::string _reason;       // �ڲ�ʧ��ԭ������
		std::string _description; // ʧ��ԭ�򣺸��ı����������û�չʾ
		std::string _extension;    // ��չ�ֶΣ����Ϊ�գ�����ֶβ�����
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
		std::string _context;       // ��Ϣ��ˮ
		std::string _appid;         // ҵ���ʶ��
		uint8_t     _termtype;        //ҵ������ 
		/*
		1.	win PC APP 
		2.	Android APP�������ֻ���pad��
		3.	IOS APP�������ֻ���ipad��
		4.	MAC APP
		5.	WEB PC
		6.	WEB MObile 
		7.	linux APP
		*/
		
		std::string _device_id;     // �豸��ʶ��PC������Ϊ�����ַ���ƶ���Ϊimid��ʶ, web�˿�ѡ
		std::string _client_ip;     // �ͻ���ip
		uint32_t    _ver_int;       // �ͻ��������汾��
		std::string _ver_str;       // �ͻ����ַ����汾��
		std::string _version;       // Э��汾��

		std::string _mobile;
		std::string _smscode;
		
		std::string _pic_id;   //ͼƬ��֤����:����̫Ƶ������ҪͼƬ
	    std::string _pic_code; //
	       
		std::string _vmToken;       // �����������֤��token.
		
		    
        std::string _callback_data;     // �״������¼ʱΪ�ա������Ҫ��һ���Ľ�����֤�������˻�����һ�����ݷ��ص��ͻ��ˣ�������֤�׶ξ���Ҫ���ݸ�ֵ��
		std::string _extension;     // ��չ�ֶΣ���ѡ�ֶΣ��ڶ���������Ҳ���Բ�����
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
        
          std::string bizName;// �������������
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

		std::string _context;      // ��Ϣ��ˮ
		uint32_t    _rescode;      // ����룬�������ENU_LRESö��ֵ
		std::string _reason;       // ��Դ�����Ĵ�����Ϣ,��Ҫ������־��ӡ 
		std::string _description;  // ʧ��ԭ�����������ڶ��û�չʾ,�ɹ�ʱΪ�գ���ʽ�ɶ���(json��ʽ��������title/content)
		uint64_t    _yyuid;        // yyuid
		
		std::string _pic_id;      // _ͼƬ��֤����
		std::string _pic_data;    //  ͼƬ��֤�� 
		std::string _authToken;   //  ���ص�¼ƾ֤���滻����
		std::string _login_data;   // ��¼���أ������¼�ɹ�(rescode=URES_SUCCESS)����LoginData�ṹ���ݣ�����Ϊ��

		std::string _callback_data;
		std::string _extension;    // ��չ�ֶΣ����ã�Ϊ��ʱ������

		

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


	/*������ӿ�*/
	struct UDBVerifyPwdReq:public sox::Marshallable
	{
		enum{uri=URI_UDBVERIFY_PWD_REQ}; // (6 << 8)|233
		std::string _context;   // ��Ϣ��ˮ
		std::string _appid;     // ҵ���ʶ��
		std::string _client_ip; // �ͻ���ip
		uint64_t _yyuid;   // �û� yyuid 
		std::string _pwd;       // ����(��������sha1һ�Σ�����XXTEA���ܣ�
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
		std::string _context;   // ��Ϣ��ˮ
		uint32_t _rescode;               // 0 - �ɹ�, 1 - ���������˺Ų�����, 99 - ������ʱ������
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
 

    // AP��dispatcher��UDB ��ȡ����Ҵ�����������
    struct UDB_GetAntiCode : public sox::Marshallable
    {
      enum {uri = URI_GETANTICODE};
    
      std::string context;
      std::string bizName; // �������������
      
      virtual void marshal(sox::Pack &pk) const 
      {
        pk << context << bizName;
      }
      virtual void unmarshal(const sox::Unpack &up)
      {
        up >> context >> bizName;
      }
    
    };
    
    // UDB��dispatcher��AP ��ȡ����Ҵ�����Ӧ����
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
      uint8_t     _termtype;        //ҵ������ 
      /*
      1.	win PC APP 
      2.	Android APP�������ֻ���pad��
      3.	IOS APP�������ֻ���ipad��
      4.	MAC APP
      5.	WEB PC
      6.	WEB MObile 
      7.	linux APP
     */
      std::string _device_id;     // �豸��ʶ��PC������Ϊ�����ַ���ƶ���Ϊimid��ʶ, web�˿�ѡ
      std::string _client_ip;     // �ͻ���ip
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
      uint32_t    _rescode;// 1 yyuid��Ӧ���û������� 2 ��acctinfoʧ��  99 �������� 100 ϵͳ����
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
