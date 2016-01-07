#pragma once

#include "int_types.h"
#include "core/base_svid.h"
#include "protocol/const.h"
#include "packet.h"
#include <vector>
#include <string>
#include <map>
#include <pthread.h>
#include <semaphore.h>

namespace freqlogin{

	/*
	//FREQLOGIN_SVID = 124;
	//servername = "freqlogin";
	   enum ResCode
	   {
			RFQ_SUCCESS        = 0,   // 调用成功
			RFQ_RATE_ERROR     = 101, // 超频访问错误
			RFQ_AUTH_ERROR     = 102, // 接入权限错误
			RFQ_FORMAT_ERROR   = 103, // 数据格式错误
			RFQ_EXISTENT_DATA  = 111, // 数据存在
			RFQ_NEXISTENT_DATA = 112, // 数据不存在
			RFQ_SYSTEM_ERROR   = 999, // 其它系统错误
	   }
	   enum UserType
	   {
	   		UFQ_PASSPORT      = 1001, // passport
	   		UFQ_YYUID         = 1002, // yyuid
	   		UFQ_YYID          = 1003, // yyid(imid)
	   		UFQ_EMAIL         = 1004, // email
	   }
	 */

	struct PMainLevelMsgReq: public sox::Marshallable{
		enum {uri = (5 << 8) | protocol::FREQLOGIN_SVID};
		std::string context;    // 上下文信息,会在返回里面带回,由业务系统自由使用
		uint32_t seq;           // seq,在返回里面带回,由业务系统自由使用
		uint32_t type;          // 帐号类型,范围为UserType
		std::string request;    // 请求帐号
		PMainLevelMsgReq() : context(""), seq(0)
		{
		}
		virtual void marshal(sox::Pack &p) const {
			p << context << seq << type << request;
		}
		virtual void unmarshal(const sox::Unpack &p) {
			p >> context >> seq >> type >> request;
		}
	};

	struct PMainLevelMsgRes: public sox::Marshallable{
		enum {uri = (6 << 8) | protocol::FREQLOGIN_SVID};
		std::string context;  // 上下文信息,与请求包中的context值相等
		uint32_t seq;         // seq,与请求包中的seq值相等
		uint32_t rescode;     // 结果标识码,值为ResCode
		uint32_t level;       // 重点等级,0为非重点,其次按1、2、3区分
		PMainLevelMsgRes() : context(""), seq(0)
		{
		}
		virtual void marshal(sox::Pack &p) const {
			p << context << seq << rescode << level;
		}
		virtual void unmarshal(const sox::Unpack &p) {
			p >> context >> seq >> rescode >> level;
		}
	};

	struct PSafeLevelMsgReq: public sox::Marshallable{
		enum {uri = (7 << 8) | protocol::FREQLOGIN_SVID};
		std::string context;    // 上下文信息,会在返回里面带回,由业务系统自由使用
		uint32_t seq;           // seq,在返回里面带回,由业务系统自由使用
		uint32_t type;          // 帐号类型,范围为UserType
		std::string request;    // 请求帐号
		PSafeLevelMsgReq() : context(""), seq(0)
		{
		}
		virtual void marshal(sox::Pack &p) const {
			p << context << seq << type << request;
		}
		virtual void unmarshal(const sox::Unpack &p) {
			p >> context >> seq >> type >> request;
		}
	};

	struct PSafeLevelMsgRes: public sox::Marshallable{
		enum {uri = (8 << 8) | protocol::FREQLOGIN_SVID};
		std::string context;  // 上下文信息,与请求包中的context值相等
		uint32_t seq;         // seq,与请求包中的seq值相等
		uint32_t rescode;     // 结果标识码,值为ResCode
		uint32_t level;       // 安全等级
		PSafeLevelMsgRes() : context(""), seq(0)
		{
		}
		virtual void marshal(sox::Pack &p) const {
			p << context << seq << rescode << level;
		}
		virtual void unmarshal(const sox::Unpack &p) {
			p >> context >> seq >> rescode >> level;
		}
	};

	struct PEvilLevelMsgReq: public sox::Marshallable{
		enum {uri = (9 << 8) | protocol::FREQLOGIN_SVID};
		std::string context;    // 上下文信息,会在返回里面带回,由业务系统自由使用
		uint32_t seq;           // seq,在返回里面带回,由业务系统自由使用
		uint32_t type;          // 帐号类型,范围为UserType
		std::string request;    // 请求帐号
		PEvilLevelMsgReq() : context(""), seq(0)
		{
		}
		virtual void marshal(sox::Pack &p) const {
			p << context << seq << type << request;
		}
		virtual void unmarshal(const sox::Unpack &p) {
			p >> context >> seq >> type >> request;
		}
	};

	struct PEvilLevelMsgRes: public sox::Marshallable{
		enum {uri = (10 << 8) | protocol::FREQLOGIN_SVID};
		std::string context;  // 上下文信息,与请求包中的context值相等
		uint32_t seq;         // seq,与请求包中的seq值相等
		uint32_t rescode;     // 结果标识码,值为ResCode
		uint32_t level;       // 恶意等级
		PEvilLevelMsgRes() : context(""), seq(0)
		{
		}
		virtual void marshal(sox::Pack &p) const {
			p << context << seq << rescode << level;
		}
		virtual void unmarshal(const sox::Unpack &p) {
			p >> context >> seq >> rescode >> level;
		}
	};

	struct PCheckFreqLoginAreaMsgReq: public sox::Marshallable{
		enum {uri = (11 << 8) | protocol::FREQLOGIN_SVID};
		std::string context;    // 上下文信息,会在返回里面带回,由业务系统自由使用
		uint32_t seq;           // seq,在返回里面带回,由业务系统自由使用
		uint32_t type;          // 帐号类型,范围为UserType
		std::string request;    // 请求帐号
		std::string ip;         // ip
		PCheckFreqLoginAreaMsgReq() : context(""), seq(0)
		{
		}
		virtual void marshal(sox::Pack &p) const {
			p << context << seq << type << request << ip;
		}
		virtual void unmarshal(const sox::Unpack &p) {
			p >> context >> seq >> type >> request >> ip;
		}
	};

	struct PCheckFreqLoginAreaMsgRes: public sox::Marshallable{
		enum {uri = (12 << 8) | protocol::FREQLOGIN_SVID};
		std::string context;  // 上下文信息,与请求包中的context值相等
		uint32_t seq;         // seq,与请求包中的seq值相等
		uint32_t rescode;     // 结果标识码,值为ResCode
		uint32_t result;      // 验证结果
		uint32_t grade;       // 常登地等级
		PCheckFreqLoginAreaMsgRes() : context(""), seq(0)
		{
		}
		virtual void marshal(sox::Pack &p) const {
			p << context << seq << rescode << result << grade;
		}
		virtual void unmarshal(const sox::Unpack &p) {
			p >> context >> seq >> rescode >> result >> grade;
		}
	};

	struct PCheckFreqLoginAreaMsgReq2: public sox::Marshallable{
		enum {uri = (13 << 8) | protocol::FREQLOGIN_SVID};
		std::string context;    // 上下文信息,会在返回里面带回,由业务系统自由使用
		uint32_t seq;           // seq,在返回里面带回,由业务系统自由使用
		uint32_t type;          // 帐号类型,范围为UserType
		std::string request;    // 请求帐号
		PCheckFreqLoginAreaMsgReq2() : context(""), seq(0)
		{
		}
		virtual void marshal(sox::Pack &p) const {
			p << context << seq << type << request;
		}
		virtual void unmarshal(const sox::Unpack &p) {
			p >> context >> seq >> type >> request;
		}
	};

	struct PCheckFreqLoginAreaMsgRes2: public sox::Marshallable{
		enum {uri = (14 << 8) | protocol::FREQLOGIN_SVID};
		std::string context;  // 上下文信息,与请求包中的context值相等
		uint32_t seq;         // seq,与请求包中的seq值相等
		uint32_t rescode;     // 结果标识码,值为ResCode
		uint32_t result;      // 验证结果
		PCheckFreqLoginAreaMsgRes2() : context(""), seq(0)
		{
		}
		virtual void marshal(sox::Pack &p) const {
			p << context << seq << rescode << result;
		}
		virtual void unmarshal(const sox::Unpack &p) {
			p >> context >> seq >> rescode >> result;
		}
	};

	struct PCheckEvilIpMsgReq: public sox::Marshallable{
		enum {uri = (15 << 8) | protocol::FREQLOGIN_SVID};
		std::string context;    // 上下文信息,会在返回里面带回,由业务系统自由使用
		uint32_t seq;           // seq,在返回里面带回,由业务系统自由使用
		std::string ipaddr;     // 请求IP
		PCheckEvilIpMsgReq() : context(""), seq(0)
		{
		}
		virtual void marshal(sox::Pack &p) const {
			p << context << seq << ipaddr;
		}
		virtual void unmarshal(const sox::Unpack &p) {
			p >> context >> seq >> ipaddr;
		}
	};

	struct PCheckEvilIpMsgRes: public sox::Marshallable{
		enum {uri = (16 << 8) | protocol::FREQLOGIN_SVID};
		std::string context;  // 上下文信息,与请求包中的context值相等
		uint32_t seq;         // seq,与请求包中的seq值相等
		uint32_t rescode;     // 结果标识码,值为ResCode
		uint32_t result;      // 验证结果
		uint32_t level;       // 恶意等级,1为恶意程度黑色,2为恶意程度灰色,后续可能扩展
		PCheckEvilIpMsgRes() : context(""), seq(0)
		{
		}
		virtual void marshal(sox::Pack &p) const {
			p << context << seq << rescode << result << level;
		}
		virtual void unmarshal(const sox::Unpack &p) {
			p >> context >> seq >> rescode >> result >> level;
		}
	};

	struct PMSELevelMsgReq: public sox::Marshallable{
		enum {uri = (17 << 8) | protocol::FREQLOGIN_SVID};
		std::string context;    // 上下文信息,会在返回里面带回,由业务系统自由使用
		uint32_t seq;           // seq,在返回里面带回,由业务系统自由使用
		uint32_t type;          // 帐号类型,范围为UserType
		std::string request;    // 请求帐号
		PMSELevelMsgReq() : context(""), seq(0)
		{
		}
		virtual void marshal(sox::Pack &p) const {
			p << context << seq << type << request;
		}
		virtual void unmarshal(const sox::Unpack &p) {
			p >> context >> seq >> type >> request;
		}
	};

	struct PMSELevelMsgRes: public sox::Marshallable{
		enum {uri = (18 << 8) | protocol::FREQLOGIN_SVID};
		std::string context;  // 上下文信息,与请求包中的context值相等
		uint32_t seq;         // seq,与请求包中的seq值相等
		uint32_t rescode;     // 结果标识码,值为ResCode
		uint32_t mainlevel;       // 重点等级,0为非重点,其次按1、2、3区分
		uint32_t safelevel;       // 安全等级
		uint32_t evillevel;       // 恶意等级
		PMSELevelMsgRes() : context(""), seq(0)
		{
		}
		virtual void marshal(sox::Pack &p) const {
			p << context << seq << rescode << mainlevel << safelevel << evillevel;
		}
		virtual void unmarshal(const sox::Unpack &p) {
			p >> context >> seq >> rescode >> mainlevel >> safelevel >> evillevel;
		}
	};



	struct PCheckFreqLoginIPMsgReq: public sox::Marshallable{
		enum {uri = (19 << 8) | protocol::FREQLOGIN_SVID};
		std::string context;    // 上下文信息,会在返回里面带回,由业务系统自由使用
		uint32_t seq;           // seq,在返回里面带回,由业务系统自由使用
		uint32_t type;          // 帐号类型,范围为UserType
		std::string request;    // 请求帐号
		std::string ip;         // ip
		PCheckFreqLoginIPMsgReq() : context(""), seq(0)
		{
		}
		virtual void marshal(sox::Pack &p) const {
			p << context << seq << type << request << ip;
		}
		virtual void unmarshal(const sox::Unpack &p) {
			p >> context >> seq >> type >> request >> ip;
		}
	};

	struct PCheckFreqLoginIPMsgRes: public sox::Marshallable{
		enum {uri = (20 << 8) | protocol::FREQLOGIN_SVID};
		std::string context;  // 上下文信息,与请求包中的context值相等
		uint32_t seq;         // seq,与请求包中的seq值相等
		uint32_t rescode;     // 结果标识码,值为ResCode
		uint32_t result;      // 验证结果
		PCheckFreqLoginIPMsgRes() : context(""), seq(0)
		{
		}
		virtual void marshal(sox::Pack &p) const {
			p << context << seq << rescode << result;
		}
		virtual void unmarshal(const sox::Unpack &p) {
			p >> context >> seq >> rescode >> result;
		}
	};

	struct PCheckFreqLoginAndIPMsgReq: public sox::Marshallable{
		enum {uri = (21 << 8) | protocol::FREQLOGIN_SVID};
		std::string context;    // 上下文信息,会在返回里面带回,由业务系统自由使用
		uint32_t seq;           // seq,在返回里面带回,由业务系统自由使用
		uint32_t type;          // 帐号类型,范围为UserType
		std::string request;    // 请求帐号
		std::string ip;         // ip
		PCheckFreqLoginAndIPMsgReq() : context(""), seq(0)
		{
		}
		virtual void marshal(sox::Pack &p) const {
			p << context << seq << type << request << ip;
		}
		virtual void unmarshal(const sox::Unpack &p) {
			p >> context >> seq >> type >> request >> ip;
		}
	};

	struct PCheckFreqLoginAndIPMsgRes: public sox::Marshallable{
		enum {uri = (22 << 8) | protocol::FREQLOGIN_SVID};
		std::string context;  // 上下文信息,与请求包中的context值相等
		uint32_t seq;         // seq,与请求包中的seq值相等
		uint32_t rescode;     // 结果标识码,值为ResCode
		uint32_t arearesult;      // 验证结果
		uint32_t ipresult;      // 验证结果
		uint32_t grade;       // 常登地等级
		PCheckFreqLoginAndIPMsgRes() : context(""), seq(0)
		{
		}
		virtual void marshal(sox::Pack &p) const {
			p << context << seq << rescode << arearesult << ipresult << grade;
		}
		virtual void unmarshal(const sox::Unpack &p) {
			p >> context >> seq >> rescode >> arearesult >> ipresult >>  grade;
		}
	};

	struct PCompIPAreaMsgReq: public sox::Marshallable{
		enum {uri = (23 << 8) | protocol::FREQLOGIN_SVID};
		std::string context;    // 上下文信息,会在返回里面带回,由业务系统自由使用
		uint32_t seq;           // seq,在返回里面带回,由业务系统自由使用
		std::string ipfirst;         // ip
		std::string ipsecond;         // ip
		PCompIPAreaMsgReq() : context(""), seq(0)
		{
		}
		virtual void marshal(sox::Pack &p) const {
			p << context << seq << ipfirst << ipsecond;
		}
		virtual void unmarshal(const sox::Unpack &p) {
			p >> context >> seq >> ipfirst >> ipsecond;
		}
	};

	struct PCompIPAreaMsgRes: public sox::Marshallable{
		enum {uri = (24 << 8) | protocol::FREQLOGIN_SVID};
		std::string context;  // 上下文信息,与请求包中的context值相等
		uint32_t seq;         // seq,与请求包中的seq值相等
		uint32_t rescode;     // 结果标识码,值为ResCode
		uint32_t result;      // 验证结果
		PCompIPAreaMsgRes() : context(""), seq(0)
		{
		}
		virtual void marshal(sox::Pack &p) const {
			p << context << seq << rescode << result;
		}
		virtual void unmarshal(const sox::Unpack &p) {
			p >> context >> seq >> rescode >> result;
		}
	};
	// 常登设备查询
	struct PCheckFreqLoginDeviceReq: public sox::Marshallable{
		enum {uri = (25 << 8) | protocol::FREQLOGIN_SVID};
		uint32_t version;       // 版本号，由业务定义；
		std::string context;    // 上下文信息,会在返回里面带回,由业务系统自由使用
		uint32_t seq;           // seq,在返回里面带回,由业务系统自由使用
		uint32_t type;          // 帐号类型,范围详见UserType
		std::string request;    // 请求帐号
		std::string device;         // device 
		PCheckFreqLoginDeviceReq() : version(0),context(""), seq(0),type(0),request(""),device("")
		{
		}
		virtual void marshal(sox::Pack &p) const {
			p <<version << context << seq  << type << request << device;
		}
		virtual void unmarshal(const sox::Unpack &p) {
			p >>version >> context >> seq  >> type >> request >> device;
		}
	};

	struct PCheckFreqLoginDeviceRes: public sox::Marshallable{
		enum {uri = (26 << 8) | protocol::FREQLOGIN_SVID};
		uint32_t version;     // 版本号，由业务定义；
		std::string context;  // 上下文信息,与请求包中的context值相等
		uint32_t seq;         // seq,与请求包中的seq值相等
		uint32_t rescode;     // 结果标识码,值为ResCode
		uint32_t result;      // 验证结果 0 为非常登设备， 1 为常登设备, -1为系统异常。
		PCheckFreqLoginDeviceRes() : version(0), context(""), seq(0),rescode(0),result(0)
		{
		}
		virtual void marshal(sox::Pack &p) const {
			p <<version<< context << seq << rescode << result;
		}
		virtual void unmarshal(const sox::Unpack &p) {
			p >>version >> context >> seq >> rescode >> result;
		}
	};

	
}
