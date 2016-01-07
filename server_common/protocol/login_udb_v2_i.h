/********************************************************************
	created:	2015-6-3   22:07
	filename: 	login_udb_v2_i.h
	copyright:  Guangzhou huaduo Co., Ltd.
	purpose:
    description:
                
---------------------------------------------------------------------
sequence      date                  author          update history
1             2015/6/3             Luke Chou       initial
*********************************************************************/
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

using namespace sox;
// ����ʹ�ã��轫���º�:_INTERFACE_USER�����
// #define _INTERFACE_USER
#ifndef _INTERFACE_USER
#include "server_login/common/resource/login_uri.h" 
#endif // _INTERFACE_USER

using namespace std;

#ifndef _login_udb_v2_2015_6_3
#define _login_udb_v2_2015_6_3

#ifdef _INTERFACE_USER
// ����ȡֵ�벻Ҫ����Ķ�����Ҫ��udb����һ��
#define URI_VALUE(mud_id, mud_uri, svid) ((mud_uri&0xFF) << 24)|((mud_id&0xFFFF) << 8)|(svid&0xFF)
#define LM_UNI_LOGIN 100
enum LOGIN_URI
{
	LG_URI_UL2_YYLOGIN_REQ = URI_VALUE(LM_UNI_LOGIN, 20, protocol::SERVERLOGIN_SVID),
	LG_URI_UL2_YYLOGIN_RES = URI_VALUE(LM_UNI_LOGIN, 21, protocol::SERVERLOGIN_SVID),
	LG_URI_UL2_APPLOGIN_REQ = URI_VALUE(LM_UNI_LOGIN, 22, protocol::SERVERLOGIN_SVID),
	LG_URI_UL2_APPLOGIN_RES = URI_VALUE(LM_UNI_LOGIN, 23, protocol::SERVERLOGIN_SVID),
	LG_URI_UL2_WEBLOGIN_REQ = URI_VALUE(LM_UNI_LOGIN, 24, protocol::SERVERLOGIN_SVID),
	LG_URI_UL2_WEBLOGIN_RES = URI_VALUE(LM_UNI_LOGIN, 25, protocol::SERVERLOGIN_SVID),
	LG_URI_UL2_SENDSMS_REQ = URI_VALUE(LM_UNI_LOGIN, 26, protocol::SERVERLOGIN_SVID),
	LG_URI_UL2_SENDSMS_RES = URI_VALUE(LM_UNI_LOGIN, 27, protocol::SERVERLOGIN_SVID),
	LG_URI_UL2_SMS_YYREGISTERLOGIN_REQ = URI_VALUE(LM_UNI_LOGIN, 28, protocol::SERVERLOGIN_SVID),
	LG_URI_UL2_SMS_YYREGISTERLOGIN_RES = URI_VALUE(LM_UNI_LOGIN, 29, protocol::SERVERLOGIN_SVID),
	LG_URI_UL2_SMS_APPREGISTERLOGIN_REQ = URI_VALUE(LM_UNI_LOGIN, 30, protocol::SERVERLOGIN_SVID),
	LG_URI_UL2_SMS_APPREGISTERLOGIN_RES = URI_VALUE(LM_UNI_LOGIN, 31, protocol::SERVERLOGIN_SVID),
	LG_URI_UL2_SMS_YYMODPASSWD_REQ = URI_VALUE(LM_UNI_LOGIN, 32, protocol::SERVERLOGIN_SVID),
	LG_URI_UL2_SMS_YYMODPASSWD_RES = URI_VALUE(LM_UNI_LOGIN, 33, protocol::SERVERLOGIN_SVID),
	LG_URI_UL2_SMS_APPMODPASSWD_REQ = URI_VALUE(LM_UNI_LOGIN, 34, protocol::SERVERLOGIN_SVID),
	LG_URI_UL2_SMS_APPMODPASSWD_RES = URI_VALUE(LM_UNI_LOGIN, 35, protocol::SERVERLOGIN_SVID),
	LG_URI_UL2_REFRESHPIC_REQ = URI_VALUE(LM_UNI_LOGIN, 36, protocol::SERVERLOGIN_SVID),
	LG_URI_UL2_REFRESHPIC_RES = URI_VALUE(LM_UNI_LOGIN, 37, protocol::SERVERLOGIN_SVID),
};

//�淶������
// sys_err, bus_err��Χ������100����
#define MK_SYSERR_CODE(mud_id,sys_err)   (mud_id)*10000 + (sys_err)*100
#define MK_BUSERR_CODE(mud_id,bus_err)   (mud_id)*10000 + (bus_err)
#endif // _INTERFACE_USER

namespace server{ namespace loginUDB{
	enum UNI_LOGIN_ERR_CODE
	{
		URES_SUCCESS               = 0,       // �ɹ�
		// ��Ҫ��һ����֤
		URES_NEED_NEXT_VERIFY      = MK_BUSERR_CODE(LM_UNI_LOGIN, 1),  // ��Ҫ��һ����֤�����ܰ�����������֤�롢�ֻ����ơ�Ӳ�����ƣ�
		// ���´����ɷ�����·�������ʾ���ͻ������û�չʾ������Ϣ(2 ~ 9)
		URES_LOCKED                = MK_BUSERR_CODE(LM_UNI_LOGIN, 2),  // �˺ű��������û��������˺�������������ʾ������·�
		URES_FROZEN                = MK_BUSERR_CODE(LM_UNI_LOGIN, 3),  // �˺���ʱ���ᣨΥ����ȫ���ԣ���������ʾ������·�
		URES_SERVER_BANNED         = MK_BUSERR_CODE(LM_UNI_LOGIN, 4),  // ��Ӫ�����������ʾ������·�
		URES_IP_BLACKLIST          = MK_BUSERR_CODE(LM_UNI_LOGIN, 5),  // IP��������������ʾ������·�
		URES_SERVER_REJECTED       = MK_BUSERR_CODE(LM_UNI_LOGIN, 6),  // �ܾ��ṩ���񣬾ܾ�ԭ�������·�����������ң�ӵ�����Ƶȣ�
		// ������󲿷�(10 ~ 29)
		URES_PASSWD_ERR            = MK_BUSERR_CODE(LM_UNI_LOGIN, 10), // ���������Ҫ�����������������֤
		URES_PICCODE_ERR           = MK_BUSERR_CODE(LM_UNI_LOGIN, 11), // ͼƬ��֤����󣬷�����ͼƬ����Ҫ��������ͼƬ��֤����֤
		URES_SMSCODE_ERR           = MK_BUSERR_CODE(LM_UNI_LOGIN, 12), // ������֤������û��������»�ȡ������֤������ύ��֤
		URES_SMSCODE_EXPIRED       = MK_BUSERR_CODE(LM_UNI_LOGIN, 13), // ������֤����ʧЧ���û��������»�ȡ������֤������ύ��֤
		URES_SMSCODE_NOT_SENT      = MK_BUSERR_CODE(LM_UNI_LOGIN, 14), // ������֤��δ�·����û��������»�ȡ������֤������ύ��֤
		URES_MOBTOKEN_ERR          = MK_BUSERR_CODE(LM_UNI_LOGIN, 15), // �ֻ����ƴ�����Ҫ���������ֻ����ƽ�����֤
		URES_HWTOKEN_ERR           = MK_BUSERR_CODE(LM_UNI_LOGIN, 16), // Ӳ�����ƴ�����Ҫ��������Ӳ�����ƽ�����֤
		URES_MODIFY_PSW_SAME_ERR   = MK_BUSERR_CODE(LM_UNI_LOGIN, 17), // �޸����룬���������ԭ����һ��
		URES_MODIFY_PSW_TIMES_LIMIT= MK_BUSERR_CODE(LM_UNI_LOGIN, 18), // �޸������������
		// ƾ֤���󲿷֣�30 ~ 49��
		URES_CRED_EXPIRED          = MK_BUSERR_CODE(LM_UNI_LOGIN, 30), // ƾ֤��ʧЧ�ڣ���Ҫ���½��п�����֤
        URES_CRED_ERR              = MK_BUSERR_CODE(LM_UNI_LOGIN, 31), // ƾ֤����/��Ч����Ҫ���½��п�����֤
		URES_CRED_DEVICE_UNMATCHED = MK_BUSERR_CODE(LM_UNI_LOGIN, 32), // ƾ֤������֤ʱ�豸��ƥ�䣬��Ҫ���½��п�����֤
		URES_CRED_APPID_UNMATCHED  = MK_BUSERR_CODE(LM_UNI_LOGIN, 33), // ƾ֤������֤ʱappid��ƥ�䣬��Ҫ���½��п�����֤
		URES_CRED_DESTROYED        = MK_BUSERR_CODE(LM_UNI_LOGIN, 34), // ƾ֤�ѱ����٣���Ҫ���½��п�����֤
        URES_CRED_OTHERS           = MK_BUSERR_CODE(LM_UNI_LOGIN, 35), // ƾ֤����������Ҫ���½��п�����֤
        URES_CRED_CREATE           = MK_BUSERR_CODE(LM_UNI_LOGIN, 36), // ��ƾ֤���󣬿ͻ�����Ҫ����
		URES_OTP_EXPIRED           = MK_BUSERR_CODE(LM_UNI_LOGIN, 40), // һ����ƾ֤��ʧЧ�ڣ���Ҫ���»�ȡһ����ƾ֤����֤
		URES_OTP_ERR               = MK_BUSERR_CODE(LM_UNI_LOGIN, 41), // һ����ƾ֤����/��Ч����Ҫ���»�ȡһ����ƾ֤����֤
		URES_OTP_USED              = MK_BUSERR_CODE(LM_UNI_LOGIN, 42), // һ����ƾ֤�ѱ�ʹ�ù�����Ҫ���»�ȡһ����ƾ֤����֤
		URES_OTP_DEVICE_UNMATCHED  = MK_BUSERR_CODE(LM_UNI_LOGIN, 43), // һ����ƾ֤������֤ʱ�豸��ƥ�䣬��Ҫ���»�ȡһ����ƾ֤����֤
		URES_OTP_APPID_UNMATCHED   = MK_BUSERR_CODE(LM_UNI_LOGIN, 44), // һ����ƾ֤������֤ʱappid��ƥ�䣬��Ҫ���»�ȡһ����ƾ֤����֤
		URES_OTP_DESTROYED         = MK_BUSERR_CODE(LM_UNI_LOGIN, 45), // һ����ƾ֤�ѱ����٣���Ҫ���»�ȡһ����ƾ֤����֤
		// Ʊ�ݴ��󲿷�(50 ~ 59)
		URES_TICKET_ENCODING_ERR   = MK_BUSERR_CODE(LM_UNI_LOGIN, 50), // Ʊ�ݱ������
		URES_TICKET_EXPIRED        = MK_BUSERR_CODE(LM_UNI_LOGIN, 51), // Ʊ�ݹ���
		URES_TICKET_ERR            = MK_BUSERR_CODE(LM_UNI_LOGIN, 52), // Ʊ����Ч
		URES_TICKET_USED           = MK_BUSERR_CODE(LM_UNI_LOGIN, 53), // Ʊ���ѱ�ʹ�ù�
		URES_TICKET_DESTROYED      = MK_BUSERR_CODE(LM_UNI_LOGIN, 54), // Ʊ���ѱ��ϳ�
        URES_TICKET_CREATE         = MK_BUSERR_CODE(LM_UNI_LOGIN, 55), // ��Ʊ�ݴ��󣬿ͻ�����Ҫ����
		// AccountInfo���󲿷�
        URES_ACCOUNTINFO_ERR       = MK_BUSERR_CODE(LM_UNI_LOGIN, 56), // AccountInfo����
		// ������ش��󲿷�
		URES_SENT_SMSCODE_FAIL     = MK_BUSERR_CODE(LM_UNI_LOGIN, 60), // �·�������֤��ʧ�ܣ��������³��Ի�ȡ����
		URES_SENT_SMSCODE_LIMIT    = MK_BUSERR_CODE(LM_UNI_LOGIN, 61), // �·�������֤����������������ƣ���Ҫ�ڶ���ָ��������ٳ���
		URES_SMS_VFY_TIMES_LIMIT   = MK_BUSERR_CODE(LM_UNI_LOGIN, 62), // ������֤�������࣬�������»�ȡ����
		
		// ����ҵ���߼�����(70 ~ 79)
		URES_YYID_LOGIN_PROHIBITED = MK_BUSERR_CODE(LM_UNI_LOGIN, 70), // δ��ͨYY�ŵ�¼Ȩ��
		URES_USER_NOT_EXIST        = MK_BUSERR_CODE(LM_UNI_LOGIN, 71), // �û������ڣ���¼�����ʱ��������û������ڣ�
		URES_USER_ALREADY_EXIST    = MK_BUSERR_CODE(LM_UNI_LOGIN, 72), // �û��Ѵ��ڣ�ע��ʱ���ֻ���ע�ᣩ
		URES_DEV_LOGIN_LIMIT       = MK_BUSERR_CODE(LM_UNI_LOGIN, 73), // �û�û���ڲ���¼Ȩ��

		// ���������岿��(80 ~ 89)
        URES_PARAM_INVALID         = MK_BUSERR_CODE(LM_UNI_LOGIN, 80), // �������󣬾������ԭ�����errmsg
		URES_APPID_INVALID         = MK_BUSERR_CODE(LM_UNI_LOGIN, 81), // appid��Ч��δ��udb����
		URES_UID_INVALID           = MK_BUSERR_CODE(LM_UNI_LOGIN, 82), // uid��Ч��uidΪ0�����
		URES_PHONE_NUM_INVALID     = MK_BUSERR_CODE(LM_UNI_LOGIN, 83), // �ֻ�����Ч

		// Э�齻������(90 ~ 99)
		URES_SESSION_DATA_EXPIRED  = MK_BUSERR_CODE(LM_UNI_LOGIN, 90), // session_data��ʧЧ���ͻ�����Ҫ�˻ص�û��session_data����ִ�е���һ�����¿�ʼ��������
		URES_SESSION_DATA_INVALID  = MK_BUSERR_CODE(LM_UNI_LOGIN, 91), // session_data���ܽ������ͻ�����Ҫ�˻ص�û��session_data����ִ�е���һ�����¿�ʼ��������
		URES_SESSION_INTERACTION_ERR     = MK_BUSERR_CODE(LM_UNI_LOGIN, 92), // Э�齻�������磺session_dataҵ���߼�У��ʧ�ܣ�
		
		UNI_RES_RETRY              = MK_SYSERR_CODE(LM_UNI_LOGIN, 99), // �ͻ��������ԣ�ϵͳ���������ʱ����Ҫϸ��
	};

	enum VERIFY_TOKEN_TYPE
	{
		TOKEN_CREDIT   = 1,//ƾ֤��½	
		TOKEN_PASSWORD = 2,//�����½	
		TOKEN_TICKET   = 3,//Ʊ�ݵ�½	
		TOKEN_ACCTINFO = 4,//acctinfo��½	
	};

	enum NEXT_VERIFY_STRATEGY
	{
		DS_NONE     = 0x0,      // �޶�̬����
		DS_PICCODE  = 0x01,     // ͼƬ��֤��
		DS_MOBTOKEN = 0x02,     // �ֻ�����
		DS_HWTOKEN  = 0x04,     // Ӳ������
		DS_SMSCODE  = 0x08,     // ������֤��
	};

	struct device_info
	{
		std::string _device_name;     // �豸���ơ��ֻ��ͺţ�MINOTELTE,IPHONE 6��PC�ļ��������YY-20141112WEZU
		std::string _device_id;       // �豸ID���ֻ�IMEI�� PC mac-address
		uint64_t    _terminal_type;   // �ն����ͣ��ɿͻ��˶��壨��32λ�൱��appid/appkey����16-31 ��platform���ͣ�0-15��os���ͣ�������ȡֵ���Ҹ���
		std::string _sys_info;        // ����ϵͳ��Ϣ
		std::string _lcid;            // ���Ա�־������2052��������
		device_info():_terminal_type(0){}
	};

	inline Pack & operator << (Pack & p, const device_info& r)
	{
		sox::PackBuffer pb;
		sox::Pack pk(pb);
		pk << r._device_name << r._device_id << r._terminal_type << r._sys_info << r._lcid;
		p.push_varstr(pk.data(), pk.size());
		return p;
	}

	inline const Unpack & operator >> (const Unpack & p, device_info& r)
	{
		std::string str = p.pop_varstr();
		if (str.empty())
		{
			return p;
		}
		sox::Unpack up(str.c_str(), str.size());
		up >> r._device_name >> r._device_id >> r._terminal_type >> r._sys_info >> r._lcid;
		return p;
	}

	struct proto_header
	{
		uint16_t    _app_type;        // Ӧ�÷���:1 - YYPC������YY��, 2 - YYMOB���ֻ�YY��, 3 - YYWEB��webYY��, 4 - APPPC, 5 - APPMOB, 6 - WEB
		std::string _appid;           // Ӧ��id��ԭ����Ӧ��udbͳһ���䣬���е�¼������appid������
		std::string _app_sign;        // Ӧ��ǩ�����㷨��md5(_appid+_app_ver+_app_key)��ǰ8λ������app_key������appid��ʱ���Ѿ����ɵ�websdk key.
		std::string _app_ver;         // Ӧ�ð汾�ţ���ҵ��ά���������ҵ��ͳ�����ݣ�, �����汾��dev_��ͷ
		std::string _sdk_ver;         // SDK�汾�ţ���SDK������ά���������SDKͳ�����ݣ�
		std::string _client_ip;       // �ͻ���IP��������ȡ�ĶԶ˳���IP��
		device_info _device_info;     // �豸��Ϣ
		std::string _channel;  		  // ������Ϣ
		std::string _reserve;  		  // Ԥ���ֶΣ�������뷽json��ʽ���룩
		proto_header():_app_type(0){}
	};

	inline Pack & operator << (Pack & p, const proto_header& r)
	{
		sox::PackBuffer pb;
		sox::Pack pk(pb);
		pk << r._app_type << r._appid << r._app_sign << r._app_ver << r._sdk_ver << r._client_ip << r._device_info << r._channel << r._reserve;
		p.push_varstr(pk.data(), pk.size());
		return p;
	}

	inline const Unpack & operator >> (const Unpack & p, proto_header& r)
	{
		std::string str = p.pop_varstr();
		if (str.empty())
		{
			return p;
		}
		sox::Unpack up(str.c_str(), str.size());
		up >> r._app_type >> r._appid >> r._app_sign >> r._app_ver >> r._sdk_ver >>  r._client_ip >> r._device_info  >> r._channel >>  r._reserve;
		return p;
	}

	struct proto_password_login
	{
		std::string _user;         // ͨ��֤���ʺ�id�����䣬�ֻ���֮һ
		uint32_t    _enc_type;     // �������ͣ�0 - sha1��40�ֽڣ�
		std::string _password;     // ����
		proto_password_login():_enc_type(0){}
	};

	inline Pack & operator << (Pack & p, const proto_password_login& r)
	{
		sox::PackBuffer pb;
		sox::Pack pk(pb);
		pk << r._user << r._enc_type << r._password;
		p.push_varstr(pk.data(), pk.size());
		return p;
	}

	inline const Unpack & operator >> (const Unpack & p, proto_password_login& r)
	{
		std::string str = p.pop_varstr();
		if (str.empty())
		{
			return p;
		}
		sox::Unpack up(str.c_str(), str.size());
		up >> r._user >> r._enc_type >> r._password;
		return p;
	}

	struct proto_ticket_login
	{
		uint64_t    _uid;          // �ʺ��ڲ���ʶ
		std::string _ticket;       // Ʊ��
		proto_ticket_login():_uid(0){}
	};

	inline Pack & operator << (Pack & p, const proto_ticket_login& r)
	{
		sox::PackBuffer pb;
		sox::Pack pk(pb);
		pk << r._uid << r._ticket;
		p.push_varstr(pk.data(), pk.size());
		return p;
	}

	inline const Unpack & operator >> (const Unpack & p, proto_ticket_login& r)
	{
		std::string str = p.pop_varstr();
		if (str.empty())
		{
			return p;
		}
		sox::Unpack up(str.c_str(), str.size());
		up >> r._uid >> r._ticket;
		return p;
	}

	struct proto_credit_login
	{
		uint64_t    _uid;          // �ʺ��ڲ���ʶ
		std::string _credit;       // ƾ֤
		proto_credit_login():_uid(0){}
	};

	inline Pack & operator << (Pack & p, const proto_credit_login& r)
	{
		sox::PackBuffer pb;
		sox::Pack pk(pb);
		pk << r._uid << r._credit;
		p.push_varstr(pk.data(), pk.size());
		return p;
	}

	inline const Unpack & operator >> (const Unpack & p, proto_credit_login& r)
	{
		std::string str = p.pop_varstr();
		if (str.empty())
		{
			return p;
		}
		sox::Unpack up(str.c_str(), str.size());
		up >> r._uid >> r._credit;
		return p;
	}

	struct proto_acctinfo_login
	{
		std::string _user;         // ͨ��֤���ʺ�id�����䣬�ֻ���֮һ
		std::string _acctinfo;
	};

	inline Pack & operator << (Pack & p, const proto_acctinfo_login& r)
	{
		sox::PackBuffer pb;
		sox::Pack pk(pb);
		pk << r._user << r._acctinfo;
		p.push_varstr(pk.data(), pk.size());
		return p;
	}

	inline const Unpack & operator >> (const Unpack & p, proto_acctinfo_login& r)
	{
		std::string str = p.pop_varstr();
		if (str.empty())
		{
			return p;
		}
		sox::Unpack up(str.c_str(), str.size());
		up >> r._user >> r._acctinfo;
		return p;
	}

	struct proto_token
	{
		uint32_t              _token_type;    // �ο�enum VERIFY_TOKEN_TYPE; 1 - ƾ֤, 2 - �ʺ�/����,  3 - ticketƱ��, 4 - acctinfo
		proto_password_login  _proto_passwd;  // ��_token_type = 2ʱ����
		proto_credit_login    _proto_credit;  // ��_token_type = 1ʱ����
		proto_ticket_login    _proto_ticket;  // ��_token_type = 3ʱ����
		proto_acctinfo_login  _proto_acctinfo;// ��_token_type = 4ʱ����
		uint32_t    _strategy;                // ��֤���ԣ��ֻ����ơ����š�ͼƬ��
		std::string _stra_token;              // ��Ӧ���ԵĿ���
		proto_token():_token_type(0), _strategy(0){}
	};

	inline Pack & operator << (Pack & p, const proto_token& r)
	{
		sox::PackBuffer pb;
		sox::Pack pk(pb);
		pk << r._token_type;
		if(TOKEN_PASSWORD == r._token_type)
		{
			pk << r._proto_passwd;
		}
		else if(TOKEN_CREDIT == r._token_type)
		{
			pk << r._proto_credit;
		}
		else if(TOKEN_TICKET == r._token_type)
		{
			pk << r._proto_ticket;
		}
		else if(TOKEN_ACCTINFO == r._token_type)
		{
			pk << r._proto_acctinfo;
		}
		pk << r._strategy << r._stra_token;
		p.push_varstr(pk.data(), pk.size());
		return p;
	}

	inline const Unpack & operator >> (const Unpack & p, proto_token& r)
	{
		std::string str = p.pop_varstr();
		if (str.empty())
		{
			return p;
		}
		sox::Unpack up(str.c_str(), str.size());
		up >> r._token_type;
		if(TOKEN_PASSWORD == r._token_type)
		{
			up >> r._proto_passwd;
		}
		else if(TOKEN_CREDIT == r._token_type)
		{
			up >> r._proto_credit;
		}
		else if(TOKEN_TICKET == r._token_type)
		{
			up >> r._proto_ticket;
		}
		else if(TOKEN_ACCTINFO == r._token_type)
		{
			up >> r._proto_acctinfo;
		}
		up >> r._strategy >> r._stra_token;
		return p;
	}

	struct proto_jump_to // ��Ҫ��ת��ҵ��
	{
		std::string _appid;     // Ӧ��id
		std::string _sessionid; // �Ựid
	};

	inline Pack & operator << (Pack & p, const proto_jump_to& r)
	{
		sox::PackBuffer pb;
		sox::Pack pk(pb);
		pk << r._appid << r._sessionid;
		p.push_varstr(pk.data(), pk.size());
		return p;
	}

	inline const Unpack & operator >> (const Unpack & p, proto_jump_to& r)
	{
		std::string str = p.pop_varstr();
		if (str.empty())
		{
			return p;
		}
		sox::Unpack up(str.c_str(), str.size());
		up >> r._appid >> r._sessionid;
		return p;
	}

	struct proto_login_data
	{
		uint64_t    _yyuid;
		uint64_t    _yyid;
		std::string _passport;
		std::string _credit;
		proto_login_data():_yyuid(0), _yyid(0){}
		void clear(){_yyuid = 0; _yyid = 0; _passport.clear(); _credit.clear();}
	};

	inline Pack & operator << (Pack & p, const proto_login_data& r)
	{
		sox::PackBuffer pb;
		sox::Pack pk(pb);
		pk << r._yyuid << r._yyid << r._passport << r._credit;
		p.push_varstr(pk.data(), pk.size());
		return p;
	}

	inline const Unpack & operator >> (const Unpack & p, proto_login_data& r)
	{
		std::string str = p.pop_varstr();
		if (str.empty())
		{
			return p;
		}
		sox::Unpack up(str.c_str(), str.size());
		up >> r._yyuid >> r._yyid >> r._passport >> r._credit;
		return p;
	}

	struct proto_jump_otp
	{
		uint32_t    _result;    // �����
		std::string _reason;    // ʧ��ԭ������
		std::string _appid;     // ��ת��appid
		std::string _otp;       // ��תƾ֤
		proto_jump_otp():_result(0){}
	};

	inline Pack & operator << (Pack & p, const proto_jump_otp& r)
	{
		sox::PackBuffer pb;
		sox::Pack pk(pb);
		pk << r._result << r._reason << r._appid << r._otp;
		p.push_varstr(pk.data(), pk.size());
		return p;
	}

	inline const Unpack & operator >> (const Unpack & p, proto_jump_otp& r)
	{
		std::string str = p.pop_varstr();
		if (str.empty())
		{
			return p;
		}
		sox::Unpack up(str.c_str(), str.size());
		up >> r._result >> r._reason >> r._appid >> r._otp;
		return p;
	}

    struct proto_anti_code
    {
        std::string _biz_name;// �������������
        std::string _anti_code; 
    };

	inline Pack & operator << (Pack & p, const proto_anti_code& r)
	{
		sox::PackBuffer pb;
		sox::Pack pk(pb);
        pk << r._biz_name << r._anti_code;
		p.push_varstr(pk.data(), pk.size());
		return p;
	}

	inline const Unpack & operator >> (const Unpack & p, proto_anti_code& r)
	{
		std::string str = p.pop_varstr();
		if (str.empty())
		{
			return p;
		}
		sox::Unpack up(str.c_str(), str.size());
        up >> r._biz_name >> r._anti_code;
		return p;
	}

	struct proto_strategy_detail //��������
	{
		uint32_t _strategy;
		std::string _verification_selection_title;
		std::string _verification_title;
		std::string _token_data;
		proto_strategy_detail():_strategy(0){}
	};

	inline Pack & operator << (Pack & p, const proto_strategy_detail& r)
	{
		sox::PackBuffer pb;
		sox::Pack pk(pb);
		pk << r._strategy << r._verification_selection_title << r._verification_title << r._token_data;
		p.push_varstr(pk.data(), pk.size());
		return p;
	}

	inline const Unpack & operator >> (const Unpack & p, proto_strategy_detail& r)
	{
		std::string str = p.pop_varstr();
		if (str.empty())
		{
			return p;
		}
		sox::Unpack up(str.c_str(), str.size());
		up >> r._strategy >> r._verification_selection_title >> r._verification_title >> r._token_data;
		return p;
	}

	struct proto_session //session data version and string data
	{
		uint32_t       _proto_ver;     // Э��汾�ţ�����Э������
		uint32_t       _session_type;  // session���ͣ�ֱ�ӻش��ȿɣ�1��yyuid���ͣ�2��mobile���ͣ�
		std::string    _session_data;  // 1.����ǵ�һ��ִ�����󣬸ò���Ϊ�գ�2.����һ���⣬����������ǰһ�����󷵻ص�session_data�����øò���
		proto_session():_proto_ver(0),_session_type(0){}
	};

	inline Pack & operator << (Pack & p, const proto_session& r)
	{
		sox::PackBuffer pb;
		sox::Pack pk(pb);
		pk << r._proto_ver << r._session_type << r._session_data;
		p.push_varstr(pk.data(), pk.size());
		return p;
	}

	inline const Unpack & operator >> (const Unpack & p, proto_session& r)
	{
		std::string str = p.pop_varstr();
		if (str.empty())
		{
			return p;
		}
		sox::Unpack up(str.c_str(), str.size());
		up >> r._proto_ver >> r._session_type >>  r._session_data;
		return p;
	}

	//--------------------------------------------------------------------------
	// Э������ṹ��
	struct proto_login_req:public sox::Marshallable
	{
		uint32_t       _proto_ver;     // Э��汾�ţ�����Э������
		std::string    _context;       // ��Ϣ���к�
		proto_header   _proto_hdr;     // ����Э��ͷ
		proto_token    _proto_token;   // ����Э��
		proto_session  _proto_session_data;  // 1.����ǵ�һ��ִ�����󣬸ò���Ϊ�գ�2.����һ���⣬����������ǰһ�����󷵻ص�session_data�����øò���
		proto_anti_code   _proto_anti_code;  // �������Ϣ
		proto_login_req():_proto_ver(0){}

		virtual void marshal( sox::Pack &p )const  // �����ֶδ����һ���ַ���
		{
			sox::PackBuffer pb;
			sox::Pack pk(pb);
			pk << _proto_ver << _context << _proto_hdr << _proto_token << _proto_session_data << _proto_anti_code;
			p.push_varstr(pk.data(), pk.size());
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			std::string str = up.pop_varstr();
			if (!str.empty())
			{
				sox::Unpack t(str.c_str(), str.size());
				t >> _proto_ver >> _context >> _proto_hdr >> _proto_token >> _proto_session_data >> _proto_anti_code;
			}
		}
	};

	struct proto_login_res:public sox::Marshallable
	{
		uint32_t    _proto_ver;       // Э��汾�ţ�����Э������
		std::string _context;         // ��Ϣ���к�
		uint32_t    _errcode;         // ������
		std::string _errmsg;          // ��Դ�����Ĵ�����Ϣ,��Ҫ������־��ӡ 
		std::string _description;     // ʧ��ԭ�����������ڶ��û�չʾ,�ɹ�ʱΪ�գ���ʽ�ɶ���
		std::string _pic;             // ͼƬ��֤��ͼƬ
		uint32_t    _strategy;        // ��rescode = URES_NEED_NEXT_VERIFY����Ҫ��һ����֤���ĵ�¼����ѡ�bitλ��ϣ�ö��ֵ����NEXT_VERIFY_STRATEGY
		proto_login_data _login_data; // ��¼���ػ�����Ϣ
		proto_session    _proto_session_data; // 1.����ǵ�һ��ִ�����󣬸ò���Ϊ�գ�2.����һ���⣬����������ǰһ�����󷵻ص�session_data�����øò���
		vector<proto_strategy_detail> _proto_strategy_detail; // ��_strategy�ǿգ����һ����֤����֤���ݣ����ֻ���֤����ֻ�������ʾ134*****322��
		proto_login_res():_proto_ver(0), _errcode(0), _strategy(0){}
		virtual void marshal( sox::Pack &p )const  // �����ֶδ����һ���ַ���
		{
			sox::PackBuffer pb;
			sox::Pack pk(pb);
			pk << _proto_ver << _context << _errcode << _errmsg << _description << _pic << _strategy << _login_data << _proto_session_data << _proto_strategy_detail;
			p.push_varstr(pk.data(), pk.size());
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			std::string str = up.pop_varstr();
			if (!str.empty())
			{
				sox::Unpack t(str.c_str(), str.size());
				t >> _proto_ver >> _context >> _errcode >> _errmsg >> _description >> _pic >> _strategy >> _login_data >> _proto_session_data >> _proto_strategy_detail;
			}
		}
	};

	struct proto_sms_registerorlogin_req:public sox::Marshallable
	{
		uint32_t       _proto_ver;     // Э��汾�ţ�����Э������
		std::string    _context;       // ��Ϣ���к�
		proto_header   _proto_hdr;     // ����Э��ͷ
		std::string    _mobile;        // �ֻ�����
		std::string    _smscode;       // ������֤��
		proto_session  _proto_session_data;  // ��һ���������ص�session_data
		proto_sms_registerorlogin_req():_proto_ver(0){}
		virtual void marshal( sox::Pack &p )const  // �����ֶδ����һ���ַ���
		{
			sox::PackBuffer pb;
			sox::Pack pk(pb);
			pk << _proto_ver << _context << _proto_hdr << _mobile << _smscode << _proto_session_data;
			p.push_varstr(pk.data(), pk.size());
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			std::string str = up.pop_varstr();
			if (!str.empty())
			{
				sox::Unpack t(str.c_str(), str.size());
				t >> _proto_ver >> _context >> _proto_hdr >> _mobile >> _smscode >> _proto_session_data;
			}
		}
	};

	struct proto_sms_modifypasswd_req:public sox::Marshallable
	{
		uint32_t       _proto_ver;     // Э��汾�ţ�����Э������
		std::string    _context;       // ��Ϣ���к�
		proto_header   _proto_hdr;     // ����Э��ͷ
		std::string    _mobile;        // �ֻ�����
		std::string    _smscode;       // ������֤��
		uint32_t       _enc_type;      // �������ͣ�0 - sha1��40�ֽڣ�
		std::string    _password;      // ����
		proto_session  _proto_session_data;  // ��һ���������ص�session_data
		proto_sms_modifypasswd_req():_proto_ver(0), _enc_type(0){}
		virtual void marshal( sox::Pack &p )const  // �����ֶδ����һ���ַ���
		{
			sox::PackBuffer pb;
			sox::Pack pk(pb);
			pk << _proto_ver << _context << _proto_hdr << _mobile << _smscode << _enc_type << _password << _proto_session_data;
			p.push_varstr(pk.data(), pk.size());
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			std::string str = up.pop_varstr();
			if (!str.empty())
			{
				sox::Unpack t(str.c_str(), str.size());
				t >> _proto_ver >> _context >> _proto_hdr >> _mobile >> _smscode >> _enc_type >> _password >> _proto_session_data;
			}
		}
	};

	//--------------------------------------------------------------------------
	// yy�ͻ��˵�¼(��Ƶ�����)
	struct proto_yylogin_req:public proto_login_req
	{
		enum{uri = LG_URI_UL2_YYLOGIN_REQ};
	};

	struct proto_yylogin_res:public proto_login_res
	{
		enum{uri = LG_URI_UL2_YYLOGIN_RES};
		std::string _ticket;
		std::string _yycookie;
		virtual void marshal( sox::Pack &pk )const
		{
			proto_login_res::marshal(pk);
			pk << _ticket << _yycookie;
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			proto_login_res::unmarshal(up);
			up >> _ticket >> _yycookie;
		}
	};

	//--------------------------------------------------------------------------
	// yy�ͻ��������app��¼
	struct proto_applogin_req:public proto_login_req
	{
		enum{uri = LG_URI_UL2_APPLOGIN_REQ};
		std::vector<proto_jump_to>  _jump_tos;
		virtual void marshal( sox::Pack &pk )const
		{
			proto_login_req::marshal(pk);
			pk << _jump_tos;
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			proto_login_req::unmarshal(up);
			up >> _jump_tos;
		}
	};

	struct proto_applogin_res:public proto_login_res
	{
		enum{uri = LG_URI_UL2_APPLOGIN_RES};
		std::string _webcookie;
		std::vector<proto_jump_otp> _jump_otps;
		virtual void marshal( sox::Pack &pk )const
		{
			proto_login_res::marshal(pk);
			pk << _webcookie << _jump_otps;
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			proto_login_res::unmarshal(up);
			up >> _webcookie >> _jump_otps;
		}
	};

	//--------------------------------------------------------------------------
	// web��¼
	struct proto_weblogin_req:public proto_login_req
	{
		enum{uri = LG_URI_UL2_WEBLOGIN_REQ};
	};

	struct proto_weblogin_res:public proto_login_res
	{
		enum{uri = LG_URI_UL2_WEBLOGIN_RES};
	};

	//--------------------------------------------------------------------------
	// ͨ���ֻ����·�����
	struct proto_sendsms_req:public sox::Marshallable
	{
		enum{uri = LG_URI_UL2_SENDSMS_REQ};
		uint32_t       _proto_ver;     // Э��汾�ţ�����Э������
		std::string    _context;       // ��Ϣ���к�
		proto_header   _proto_hdr;     // ����Э��ͷ
		std::string    _mobile;        // �ֻ�����
		uint8_t        _type;          // 1 - ����ע����¼��û��ע�����ע�ᣬ��ע��͵�¼����2 - ���ڵ�¼����ܣ��ֻ���δע�ᣬ�����û������ڣ�
        std::string    _piccode;       // ͼƬ��֤��
		proto_session  _proto_session_data;  // Ԥ��
		proto_sendsms_req():_proto_ver(0),_type(0){}
		virtual void marshal( sox::Pack &pk )const
		{
			pk << _proto_ver << _context << _proto_hdr << _mobile << _type << _piccode << _proto_session_data;
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			up >> _proto_ver >> _context >> _proto_hdr >> _mobile >> _type >> _piccode >> _proto_session_data;
		}
	};

	struct proto_sendsms_res:public sox::Marshallable
	{
		enum{uri = LG_URI_UL2_SENDSMS_RES};
		uint32_t    _proto_ver;       // Э��汾�ţ�����Э������
		std::string _context;         // ��Ϣ���к�
		uint32_t    _errcode;         // ������
		std::string _errmsg;          // ��Դ�����Ĵ�����Ϣ,��Ҫ������־��ӡ
		std::string _description;     // ʧ��ԭ�����������ڶ��û�չʾ,�ɹ�ʱΪ�գ���ʽ�ɶ���
        std::string   _pic;           // ͼƬ��֤��
		proto_session _proto_session_data;    // ��һ������ʱʹ��
		proto_sendsms_res():_proto_ver(0), _errcode(0){}
		virtual void marshal( sox::Pack &pk )const
		{
			pk << _proto_ver << _context << _errcode << _errmsg << _description << _pic << _proto_session_data;
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			up >> _proto_ver >> _context >> _errcode >> _errmsg >> _description >> _pic >> _proto_session_data;
		}
	};

	//--------------------------------------------------------------------------
	// yy�ƶ���ʹ�ö�����֤��ע����¼
	struct proto_sms_yyregisterorlogin_req:public proto_sms_registerorlogin_req
	{
		enum{uri = LG_URI_UL2_SMS_YYREGISTERLOGIN_REQ};
	};

	struct proto_sms_yyregisterorlogin_res:public proto_login_res
	{
		enum{uri = LG_URI_UL2_SMS_YYREGISTERLOGIN_RES};
		std::string _ticket;
		std::string _yycookie;
		bool 		_is_new_user; //�Ƿ�Ϊ��δע���û�
		virtual void marshal( sox::Pack &pk )const
		{
			proto_login_res::marshal(pk);
			pk << _ticket << _yycookie << _is_new_user;
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			proto_login_res::unmarshal(up);
			up >> _ticket >> _yycookie >> _is_new_user;
		}
	};

	//--------------------------------------------------------------------------
	// ����app�ƶ���ʹ�ö�����֤��ע����¼
	struct proto_sms_appregisterorlogin_req:public proto_sms_registerorlogin_req
	{
		enum{uri = LG_URI_UL2_SMS_APPREGISTERLOGIN_REQ};
		std::vector<proto_jump_to>  _jump_tos;
		virtual void marshal( sox::Pack &pk )const
		{
			proto_sms_registerorlogin_req::marshal(pk);
			pk << _jump_tos;
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			proto_sms_registerorlogin_req::unmarshal(up);
			up >> _jump_tos;
		}
	};

	struct proto_sms_appregisterorlogin_res:public proto_login_res
	{
		enum{uri = LG_URI_UL2_SMS_APPREGISTERLOGIN_RES};
		std::string _webcookie;
		std::vector<proto_jump_otp> _jump_otps;
		virtual void marshal( sox::Pack &pk )const
		{
			proto_login_res::marshal(pk);
			pk << _webcookie << _jump_otps;
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			proto_login_res::unmarshal(up);
			up >> _webcookie >> _jump_otps;
		}
	};

	//--------------------------------------------------------------------------
	//yy�ƶ���ʹ�ö�����֤�����
	struct proto_sms_yymodifypasswd_req:public proto_sms_modifypasswd_req
	{
		enum{uri = LG_URI_UL2_SMS_YYMODPASSWD_REQ};
	};

	struct proto_sms_yymodifypasswd_res:public proto_login_res
	{
		enum{uri = LG_URI_UL2_SMS_YYMODPASSWD_RES};
		std::string _ticket;
		std::string _yycookie;
		virtual void marshal( sox::Pack &pk )const
		{
			proto_login_res::marshal(pk);
			pk << _ticket << _yycookie;
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			proto_login_res::unmarshal(up);
			up >> _ticket >> _yycookie;
		}
	};

	//--------------------------------------------------------------------------
	// ����app�ƶ���ʹ�ö�����֤�����
	struct proto_sms_appmodifypasswd_req:public proto_sms_modifypasswd_req
	{
		enum{uri = LG_URI_UL2_SMS_APPMODPASSWD_REQ};
		std::vector<proto_jump_to>  _jump_tos;
		virtual void marshal( sox::Pack &pk )const
		{
			proto_sms_modifypasswd_req::marshal(pk);
			pk << _jump_tos;
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			proto_sms_modifypasswd_req::unmarshal(up);
			up >> _jump_tos;
		}
	};

	struct proto_sms_appmodifypasswd_res:public proto_login_res
	{
		enum{uri = LG_URI_UL2_SMS_APPMODPASSWD_RES};
		std::string _webcookie;
		std::vector<proto_jump_otp> _jump_otps;
		virtual void marshal( sox::Pack &pk )const
		{
			proto_login_res::marshal(pk);
			pk << _webcookie << _jump_otps;
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			proto_login_res::unmarshal(up);
			up >> _webcookie >> _jump_otps;
		}
	};

	//-----------------------------------------------------------------------------
	// ˢ��ͼƬ
	struct proto_refresh_pic_req:public sox::Marshallable
	{
		enum{uri = LG_URI_UL2_REFRESHPIC_REQ};
		uint32_t       _proto_ver;     // Э��汾�ţ�����Э������
		std::string    _context;       // ��Ϣ���к�
		proto_header   _proto_hdr;     // ����Э��ͷ
		proto_session    _proto_session_data;  // ��һ���������ص�session_data
		proto_refresh_pic_req():_proto_ver(0){}
		virtual void marshal( sox::Pack &pk )const
		{
			pk << _proto_ver << _context << _proto_hdr << _proto_session_data;
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			up >> _proto_ver >> _context >> _proto_hdr >> _proto_session_data;
		}
	};

	struct proto_refresh_pic_res:public sox::Marshallable
	{
		enum{uri = LG_URI_UL2_REFRESHPIC_RES};
		uint32_t    _proto_ver;       // Э��汾�ţ�����Э������
		std::string _context;         // ��Ϣ���к�
		uint32_t    _errcode;         // ������
		std::string _errmsg;          // ��Դ�����Ĵ�����Ϣ,��Ҫ������־��ӡ 
		std::string _pic;             // ��õ�ͼƬ
		proto_session _proto_session_data;    // ������һ������ʱʹ��
		proto_refresh_pic_res():_proto_ver(0), _errcode(0){}
		virtual void marshal( sox::Pack &pk )const
		{
			pk << _proto_ver << _context << _errcode << _errmsg << _pic << _proto_session_data;
		}

		virtual void unmarshal(const sox::Unpack &up)
		{
			up >> _proto_ver >> _context >> _errcode >> _errmsg >> _pic >> _proto_session_data;
		}
	};
}} // namespace server{ namespace loginUDB

#endif // _login_udb_v2_2015_6_3
