#ifndef YYPROTOCOL_H
#define YYPROTOCOL_H

#include <iostream>

#include "common/packet.h"
#include "protocol/const.h"

#define URI_VC_VALUE(mud_id, mud_uri, svid) ((mud_uri & 0xFF) << 24) | ((mud_id & 0xFFFF) << 8) | (svid & 0xFF)
#define ERR_VC_VALUE(mud_id, errno) ((mud_id & 0xFFFF) << 16) | errno

enum VCRES_ERRCODE
{
		VCRSE_SUCCESS  = 0,  // �ɹ�
		// 1 - 100 �η����ϵͳ����
		VCRSE_EXIST_ERR = ERR_VC_VALUE(116, 1),     // �˺Ų�����
		VCRSE_BIND_ERR = ERR_VC_VALUE(116, 2),      // �˺��ް�
		VCRSE_INTERNAL_ERR = ERR_VC_VALUE(116, 3),  // ��ʶ����
		VCRSE_CODE_ERR = ERR_VC_VALUE(116, 4),      // ��֤�����
		VCRSE_OVERDUE_ERR = ERR_VC_VALUE(116, 5),   // ��֤����ʧЧ������ʱ����ں���֤�����ﵽ���ޣ���Ҫ���»�ȡ��
		VCRSE_LIMIT_ERR = ERR_VC_VALUE(116, 6),     // �·���������
		VCRSE_AUTH_ERR = ERR_VC_VALUE(116, 7),      // ��Ȩ��Ϣ����
		VCRSE_PARAM_ERR = ERR_VC_VALUE(116, 8),     // ��������
		VCRSE_SYSTEM_ERR = ERR_VC_VALUE(116, 99),   // ϵͳ������Ҫ����
};

enum AccountType
{
		AT_YYUID = 0,    // ���ʹ��yyuid�������ȥ��ѯ�˺����󶨵���������ܱ��ֻ�
		AT_EMAIL = 1,
		AT_MOBILE = 2,
};

struct RequestEmailCode: public sox::Marshallable
{
		enum{uri=URI_VC_VALUE(116, 1, 233)};
		
		std::string _context;     // ������
		std::string _appid;       // ҵ���ʶ
		std::string _service_id;  // ҵ���ӱ�ʶ
		uint8_t     _type;        // AT_YYUID/AT_EMAIL
		std::string _account;     // yyuid/email
		uint32_t    _expire_time; // ��Чʱ�䣨��λ���룬���ܳ���24Сʱ�����鲻����10���ӣ�
		std::string _email_info;  // �·��İ�������Ϊjson��ʽ��{"from":"","subject":"","content":""}��content�У�ʹ��{code}����ʶ���ɵ���֤��,ʹ��{passport}����ʶͨ��֤���룻
		std::string _appkey;      // ��֤��Ϣ
		std::string _extra;       // Ԥ���ֶ�
		
		RequestEmailCode(){}
		
		virtual void marshal(sox::Pack &pk) const {
				pk << _context << _appid << _service_id << _type << _account << _expire_time << _email_info << _appkey << _extra;
		}

		virtual void unmarshal(const sox::Unpack &up) {
				up >> _context >> _appid >> _service_id >> _type >> _account >> _expire_time >> _email_info >> _appkey >> _extra;
		}
};

struct RequestEmailCodeResp: public sox::Marshallable
{
		enum{uri=URI_VC_VALUE(116, 2, 233)};
		
		std::string _context;     // ������
		uint32_t    _rescode;     // ����룬�ο�VCRES_ERRCODE
		std::string _errinfo;     // �������
		std::string _extra;       // Ԥ���ֶ�
		
		RequestEmailCodeResp(){}
		
		virtual void marshal(sox::Pack &pk) const {
				pk << _context << _rescode << _errinfo << _extra;
		}

		virtual void unmarshal(const sox::Unpack &up) {
				up >> _context >> _rescode >> _errinfo >> _extra;
		}
};

struct VerifyEmailCode: public sox::Marshallable
{
		enum{uri=URI_VC_VALUE(116, 3, 233)};
		
		std::string _context;     // ������
		std::string _appid;       // ҵ���ʶ
		std::string _service_id;  // ҵ���ӱ�ʶ
		uint8_t     _type;        // AT_YYUID/AT_EMAIL
		std::string _account;     // yyuid/email
		std::string _code;        // ��֤��
		std::string _appkey;      // ��֤��Ϣ
		std::string _extra;       // Ԥ���ֶ�
		
		VerifyEmailCode(){}
		
		virtual void marshal(sox::Pack &pk) const {
				pk << _context << _appid << _service_id << _type << _account << _code << _appkey << _extra;
		}

		virtual void unmarshal(const sox::Unpack &up) {
				up >> _context >> _appid >> _service_id >> _type >> _account >> _code >> _appkey >> _extra;
		}
};

struct VerifyEmailCodeResp: public sox::Marshallable
{
		enum{uri=URI_VC_VALUE(116, 4, 233)};
		
		std::string _context;     // ������
		uint32_t    _rescode;     // ����룬�ο�VCRES_ERRCODE
		std::string _errinfo;     // �������
		std::string _extra;       // Ԥ���ֶ�
		
		VerifyEmailCodeResp(){}
		
		virtual void marshal(sox::Pack &pk) const {
				pk << _context << _rescode << _errinfo << _extra;
		}

		virtual void unmarshal(const sox::Unpack &up) {
				up >> _context >> _rescode >> _errinfo >> _extra;
		}
};

struct RequestSmsCode: public sox::Marshallable
{
		enum{uri=URI_VC_VALUE(116, 11, 233)};
		
		std::string _context;     // ������
		std::string _appid;       // ҵ���ʶ
		std::string _service_id;  // ҵ���ӱ�ʶ
		uint8_t     _type;        // AT_YYUID/AT_MOBILE
		std::string _account;     // yyuid/mobile
		uint32_t    _expire_time; // ��Чʱ�䣨��λ���룬���ܳ���24Сʱ�����鲻����10���ӣ�	
		std::string _sms_info;    // �·��İ���content�У�ʹ��{code}����ʶ���ɵ���֤��,ʹ��{passport}����ʶͨ��֤���룻���ֵΪ"[template]"�ַ�������Ϊ�����İ��������İ�����ǰ�����񷽶���
		std::string _appkey;      // ��֤��Ϣ
		std::string _extra;       // Ԥ���ֶ�
		
		RequestSmsCode(){}
		
		virtual void marshal(sox::Pack &pk) const {
				pk << _context << _appid << _service_id << _type << _account << _expire_time << _sms_info << _appkey << _extra;
		}

		virtual void unmarshal(const sox::Unpack &up) {
				up >> _context >> _appid >> _service_id >> _type >> _account >> _expire_time >> _sms_info >> _appkey >> _extra;
		}
};

struct RequestSmsCodeResp: public sox::Marshallable
{
		enum{uri=URI_VC_VALUE(116, 12, 233)};
		
		std::string _context;     // ������
		uint32_t    _rescode;     // ����룬�ο�VCRES_ERRCODE
		std::string _errinfo;     // �������
		std::string _extra;       // Ԥ���ֶ�
		
		RequestSmsCodeResp(){}
		
		virtual void marshal(sox::Pack &pk) const {
				pk << _context << _rescode << _errinfo << _extra;
		}

		virtual void unmarshal(const sox::Unpack &up) {
				up >> _context >> _rescode >> _errinfo >> _extra;
		}
};

struct VerifySmsCode: public sox::Marshallable
{
		enum{uri=URI_VC_VALUE(116, 13, 233)};
		
		std::string _context;     // ������
		std::string _appid;       // ҵ���ʶ
		std::string _service_id;  // ҵ���ӱ�ʶ
		uint8_t     _type;        // AT_YYUID/AT_MOBILE
		std::string _account;     // yyuid/mobile
		std::string _code;        // ��֤��
		std::string _appkey;      // ��֤��Ϣ
		std::string _extra;       // Ԥ���ֶ�
		
		VerifySmsCode(){}
		
		virtual void marshal(sox::Pack &pk) const {
				pk << _context << _appid << _service_id << _type << _account << _code << _appkey << _extra;
		}

		virtual void unmarshal(const sox::Unpack &up) {
				up >> _context >> _appid >> _service_id >> _type >> _account >> _code >> _appkey >> _extra;
		}
};

struct VerifySmsCodeResp: public sox::Marshallable
{
		enum{uri=URI_VC_VALUE(116, 14, 233)};
		
		std::string _context;     // ������
		uint32_t    _rescode;     // ����룬�ο�VCRES_ERRCODE
		std::string _errinfo;     // �������
		std::string _extra;       // Ԥ���ֶ�
		
		VerifySmsCodeResp(){}
		
		virtual void marshal(sox::Pack &pk) const {
				pk << _context << _rescode << _errinfo << _extra;
		}

		virtual void unmarshal(const sox::Unpack &up) {
				up >> _context >> _rescode >> _errinfo >> _extra;
		}
};

struct RequestImageCode: public sox::Marshallable
{
		enum{uri=URI_VC_VALUE(116, 21, 233)};
		
		std::string _context;     // ������
		std::string _appid;       // ҵ���ʶ
		std::string _service_id;  // ҵ���ӱ�ʶ
		uint8_t     _image_type;  // ͼƬ��֤�����ͣ�0��Ӣ�ģ�1�����ģ�
		uint32_t    _expire_time; // ��Чʱ�䣨��λ���룬���ܳ���24Сʱ�����鲻����10���ӣ�
		std::string _appkey;      // ��֤��Ϣ
		std::string _extra;       // Ԥ���ֶ�
		
		RequestImageCode(){}
		
		virtual void marshal(sox::Pack &pk) const {
				pk << _context << _appid << _service_id << _image_type << _expire_time << _appkey << _extra;
		}

		virtual void unmarshal(const sox::Unpack &up) {
				up >> _context >> _appid >> _service_id >> _image_type >> _expire_time >> _appkey >> _extra;
		}
};

struct RequestImageCodeResp: public sox::Marshallable
{
		enum{uri=URI_VC_VALUE(116, 22, 233)};
		
		std::string _context;     // ������
		uint32_t    _rescode;     // ����룬�ο�VCRES_ERRCODE
		std::string _internal_id; // ��֤��Ψһ��ʶ
		std::string _codeimage;   // ��֤��ͼƬ
		std::string _errinfo;     // �������
		std::string _extra;       // Ԥ���ֶ�
		
		RequestImageCodeResp(){}
		
		virtual void marshal(sox::Pack &pk) const {
				pk << _context << _rescode << _internal_id << _codeimage << _errinfo << _extra;
		}

		virtual void unmarshal(const sox::Unpack &up) {
				up >> _context >> _rescode >> _internal_id >> _codeimage >> _errinfo >> _extra;
		}
};

struct VerifyImageCode: public sox::Marshallable
{
		enum{uri=URI_VC_VALUE(116, 23, 233)};
		
		std::string _context;     // ������
		std::string _appid;       // ҵ���ʶ
		std::string _service_id;  // ҵ���ӱ�ʶ
		std::string _internal_id; // ��֤��Ψһ��ʶ
		std::string _code;        // ��֤��ͼƬ
		uint8_t     _image_type;  // ��֤ʧ�ܺ����»�ȡͼƬ��֤�����ͣ�0��Ӣ�ģ�1�����ģ�
		std::string _appkey;      // ��֤��Ϣ
		std::string _extra;       // Ԥ���ֶ�
		
		VerifyImageCode(){}
		
		virtual void marshal(sox::Pack &pk) const {
				pk << _context << _appid << _service_id << _internal_id << _code << _image_type << _appkey << _extra;
		}

		virtual void unmarshal(const sox::Unpack &up) {
				up >> _context >> _appid >> _service_id >> _internal_id >> _code >> _image_type >> _appkey >> _extra;
		}
};

struct VerifyImageCodeResp: public sox::Marshallable
{
		enum{uri=URI_VC_VALUE(116, 24, 233)};
		
		std::string _context;     // ������
		uint32_t    _rescode;     // ����룬�ο�VCRES_ERRCODE
		std::string _internal_id; // ��֤��Ψһ��ʶ����֤���ɹ��������ʹ�ã�
		std::string _codeimage;   // ��֤��ͼƬ����֤���ɹ��������ʹ�ã�
		std::string _errinfo;     // �������
		std::string _extra;       // Ԥ���ֶ�
		
		VerifyImageCodeResp(){}
		
		virtual void marshal(sox::Pack &pk) const {
				pk << _context << _rescode << _internal_id << _codeimage << _errinfo << _extra;
		}

		virtual void unmarshal(const sox::Unpack &up) {
				up >> _context >> _rescode >> _internal_id >> _codeimage >> _errinfo >> _extra;
		}
};

struct RequestImageCode2: public sox::Marshallable
{
		enum{uri=URI_VC_VALUE(116, 31, 233)};
		
		std::string _context;     // ������
		std::string _appid;       // ҵ���ʶ
		std::string _service_id;  // ҵ���ӱ�ʶ
		uint64_t    _yyuid;       // �û�yyuid
		uint8_t     _image_type;  // ͼƬ��֤�����ͣ�0��Ӣ�ģ�1�����ģ�
		uint32_t    _expire_time; // ��Чʱ�䣨��λ���룬���ܳ���24Сʱ�����鲻����10���ӣ�
		std::string _appkey;      // ��֤��Ϣ
		std::string _extra;       // Ԥ���ֶ�
		
		RequestImageCode2(){}
		
		virtual void marshal(sox::Pack &pk) const {
				pk << _context << _appid << _service_id << _yyuid << _image_type << _expire_time << _appkey << _extra;
		}

		virtual void unmarshal(const sox::Unpack &up) {
				up >> _context >> _appid >> _service_id >> _yyuid >> _image_type >> _expire_time >> _appkey >> _extra;
		}
};

struct RequestImageCodeResp2: public sox::Marshallable
{
		enum{uri=URI_VC_VALUE(116, 32, 233)};
		
		std::string _context;     // ������
		uint32_t    _rescode;     // ����룬�ο�VCRES_ERRCODE
		std::string _codeimage;   // ��֤��ͼƬ
		std::string _errinfo;     // �������
		std::string _extra;       // Ԥ���ֶ�
		
		RequestImageCodeResp2(){}
		
		virtual void marshal(sox::Pack &pk) const {
				pk << _context << _rescode << _codeimage << _errinfo << _extra;
		}

		virtual void unmarshal(const sox::Unpack &up) {
				up >> _context >> _rescode >> _codeimage >> _errinfo >> _extra;
		}
};

struct VerifyImageCode2: public sox::Marshallable
{
		enum{uri=URI_VC_VALUE(116, 33, 233)};
		
		std::string _context;     // ������
		std::string _appid;       // ҵ���ʶ
		std::string _service_id;  // ҵ���ӱ�ʶ
		uint64_t    _yyuid;       // �û�yyuid
		std::string _code;        // ��֤��ͼƬ
		uint8_t     _image_type;  // ��֤ʧ�ܺ����»�ȡͼƬ��֤�����ͣ�0��Ӣ�ģ�1�����ģ�
		std::string _appkey;      // ��֤��Ϣ
		std::string _extra;       // Ԥ���ֶ�
		
		VerifyImageCode2(){}
		
		virtual void marshal(sox::Pack &pk) const {
				pk << _context << _appid << _service_id << _yyuid << _code << _image_type << _appkey << _extra;
		}

		virtual void unmarshal(const sox::Unpack &up) {
				up >> _context >> _appid >> _service_id >> _yyuid >> _code >> _image_type >> _appkey >> _extra;
		}
};

struct VerifyImageCodeResp2: public sox::Marshallable
{
		enum{uri=URI_VC_VALUE(116, 34, 233)};
		
		std::string _context;     // ������
		uint32_t    _rescode;     // ����룬�ο�VCRES_ERRCODE
		std::string _codeimage;   // ��֤��ͼƬ����֤���ɹ��������ʹ�ã�
		std::string _errinfo;     // �������
		std::string _extra;       // Ԥ���ֶ�
		
		VerifyImageCodeResp2(){}
		
		virtual void marshal(sox::Pack &pk) const {
				pk << _context << _rescode << _codeimage << _errinfo << _extra;
		}

		virtual void unmarshal(const sox::Unpack &up) {
				up >> _context >> _rescode >> _codeimage >> _errinfo >> _extra;
		}
};

#endif
