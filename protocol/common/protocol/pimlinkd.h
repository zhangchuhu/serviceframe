#ifndef _PIMLINKD_H_
#define _PIMLINKD_H_

#include "protocol/const.h"

#include <string>
#include <set>
#include "pimcommon.h"
#include "common/core/base_svid.h"

#include "common/protocol/Im_Const.h"
#include "common/openssl/blowfish.h"

namespace protocol{
	namespace imlinkd{

		//增加的通用的cookie计算
		static char bf_key[] = "123456)(*&^%$#@!";
		static unsigned char cbc_iv [8]={0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};
		static std::string ToCookieCommon(const std::string &str)
		{
			if (str.size() >= 300)
			{
				return "";
			}

			BF_KEY key;
			char out[512];
			memset(out, 0, sizeof out);
			unsigned char iv[8];
			memcpy(iv, cbc_iv, sizeof iv);

			BF_set_key(&key, strlen(bf_key),(unsigned char *)bf_key);

			const char *in = str.c_str();
			int length = strlen(in);
			BF_cbc_encrypt((unsigned char *)in, (unsigned char *)out, length,
				&key, iv ,BF_ENCRYPT);

			/*BF_cbc_encrypt(cbc_out,cbc_in,len,
			&key,iv,BF_DECRYPT);*/
			return std::string(out, (length + 7) & ~7);
		}

		struct PLoginImLinkd : public sox::Marshallable {
			enum {uri = (1 << 8) | protocol::IMLINKD_SVID};

			PLoginImLinkd() {}
			PLoginImLinkd(uint32_t u, const std::string &c, const std::string& passwd, const std::string& acc, protocol::E_IMSTATUS st) :
			uid(u), cookie(c), sha_passwd(passwd), account(acc), status(st) {
			}

			uint32_t uid;	
			std::string cookie;
			std::string sha_passwd;
			std::string account;
			protocol::E_IMSTATUS status;

			virtual void marshal(sox::Pack &pk) const {
				pk << uid << cookie << sha_passwd << account;
				pk.push_uint16(status);
			}
			virtual void unmarshal(const sox::Unpack &up) {
				up >> uid >> cookie >> sha_passwd >> account;
				status = (protocol::E_IMSTATUS)up.pop_uint16();
			}
		};

		struct PLoginImLinkdRes : public sox::Marshallable {
			enum {uri = (1 << 8) | protocol::IMLINKD_SVID};

			PLoginImLinkdRes() {}

			uint32_t res;

			virtual void marshal(sox::Pack &pk) const {
				pk << res;
			}
			virtual void unmarshal(const sox::Unpack &up) {
				up >> res;
			}
		};

		struct PChangeStatus : public sox::Marshallable {
			enum {uri = (2 << 8) | protocol::IMLINKD_SVID};

			PChangeStatus() {}

			protocol::E_IMSTATUS status;

			virtual void marshal(sox::Pack &pk) const {
				pk.push_uint16(status);
			}
			virtual void unmarshal(const sox::Unpack &up) {
				status = (protocol::E_IMSTATUS)up.pop_uint16();
			}
		};

		struct PChangeStatusRes : public PChangeStatus {
			enum {uri = (2 << 8) | protocol::IMLINKD_SVID};

			PChangeStatusRes() {}

		};


		struct IMPing: public sox::Voidmable{
			enum{uri = (3 << 8 | protocol::IMLINKD_SVID)};
		};

		struct PCS_ImKick : public sox::Voidmable{
			enum{uri = (4 << 8 | protocol::IMLINKD_SVID)};
		};


		//Prepare to Login im(request to get buddylist,imid, blacklist, blocklist)
		struct PCS_PrepareLoginImReq : public sox::Marshallable {
			enum {uri = (5 << 8) | protocol::IMLINKD_SVID};

			PCS_PrepareLoginImReq() {}
			PCS_PrepareLoginImReq(uint32_t uid, const std::string &cookie, const std::string& passwd, const std::string& account, const protocol::E_IMSTATUS status)
				: m_uid(uid), m_cookie(cookie), m_shaPasswd(passwd), m_account(account),m_status(status) 
			{
			}

			uint32_t m_uid;	
			std::string m_cookie;
			std::string m_shaPasswd;
			std::string m_account;
			protocol::E_IMSTATUS m_status;

			virtual void marshal(sox::Pack &pk) const 
			{
				pk << m_uid << m_cookie << m_shaPasswd << m_account ;
				pk.push_uint16(m_status);
			}
			virtual void unmarshal(const sox::Unpack &up) 
			{
				up >> m_uid >> m_cookie >> m_shaPasswd >> m_account;
				m_status = static_cast<protocol::E_IMSTATUS>(up.pop_uint16());
			}
		};	

		//response to client. return buddylist/folder/blacklist/blocklist
		struct PCS_PrepareLoginImRes : public sox::Marshallable 
		{
			enum {uri = (5 << 8) | protocol::IMLINKD_SVID};
			PCS_PrepareLoginImRes() {}
			std::vector<protocol::im::CBuddyListNoRemark> m_vtBuddyList;		//好友列表
			std::vector<uint32_t>		m_vtBlockUid;		//阻塞名单
			std::vector<uint32_t>		m_vtBlackUid;		//黑名单
			std::vector<protocol::im::CFolderList> m_vtFolderList;		//分组列表
			uint32_t m_uImid;											//用户的歪歪号
			uint32_t m_res;

			virtual void marshal(sox::Pack &pk) const 
			{
				pk << m_uImid << m_res;

				marshal_container(pk, m_vtBlockUid);
				marshal_container(pk, m_vtBlackUid);
				marshal_container(pk, m_vtBuddyList);
				marshal_container(pk, m_vtFolderList);
			}
			virtual void unmarshal(const sox::Unpack &up) 
			{
				up >> m_uImid >> m_res;

				unmarshal_container(up, std::back_inserter(m_vtBlockUid));
				unmarshal_container(up, std::back_inserter(m_vtBlackUid));
				unmarshal_container(up, std::back_inserter(m_vtBuddyList));
				unmarshal_container(up, std::back_inserter(m_vtFolderList));
			}
		};

		//request login to im
		struct PCS_LoginImLinkdReq : public sox::Voidmable 
		{
			enum {uri = (6 << 8) | protocol::IMLINKD_SVID};
			PCS_LoginImLinkdReq() {}
			/*
			PCS_LoginImLinkdReq(uint32_t uid, protocol::E_IMSTATUS status): m_uid(uid), m_status(status)
			{
			}

			uint32_t m_uid;	
			protocol::E_IMSTATUS m_status;
			virtual void marshal(sox::Pack &pk) const 
			{
			pk << m_uid ;
			pk.push_uint16(m_status);
			}
			virtual void unmarshal(const sox::Unpack &up) 
			{
			up >> m_uid ;
			m_status = up.pop_uint16();
			}*/
		};

		/*
		//response login to im
		struct PCS_LoginImLinkdRes : public sox::Marshallable 
		{
		enum {uri = (6 << 8) | protocol::IMLINKD_SVID};
		PCS_LoginImLinkdRes() {}
		PCS_LoginImLinkdRes(uint32_t res): m_res(res)
		{
		}
		uint32_t m_res;	

		virtual void marshal(sox::Pack &pk) const 
		{
		pk << m_res ;
		}
		virtual void unmarshal(const sox::Unpack &up) 
		{
		up >> m_res ;
		}
		};*/

		struct PReqRecoverIMUserStatus : public sox::Voidmable{
			enum {uri = (134 << 8) | protocol::IMLINKD_SVID};
		};

		//本协议主要用于imonline2更新索引
		struct PReqRecoverIMUserList : public sox::Voidmable
		{
			enum {uri = (139 << 8) | protocol::IMLINKD_SVID};
		};

		struct PIMKick : public sox::Marshallable {
			enum {uri = (135 << 8 | protocol::IMLINKD_SVID)};
			uint32_t uid;
			virtual void marshal(sox::Pack &pk) const {
				pk << uid;
			}
			virtual void unmarshal(const sox::Unpack &up) {
				up >> uid;
			}
		};

		// imlinkd 保存uid和其所属群组的关系

		struct PCS_GFolderListUpdate : public sox::Marshallable
		{
			enum {uri = (10 << 8 | protocol::IMLINKD_SVID)};

			PCS_GFolderListUpdate()
			{
				m_uGid = 0;
				m_uOper = 0;
			}

			uint32_t     m_uGid; 	         
			uint32_t	 m_uOper;	 // 0 增加， 1删除
			
			std::set<uint32_t> m_setFid;

			virtual void marshal(sox::Pack &pk) const 
			{
				pk << m_uGid << m_uOper;
				marshal_container(pk, m_setFid);
			}
			virtual void unmarshal(const sox::Unpack &up) 
			{
				up >> m_uGid >> m_uOper;
				unmarshal_container(up, std::inserter(m_setFid, m_setFid.begin()));
			}
		};


        struct PCS_HashRequest: public sox::Marshallable
        {
            enum { uri = (1 << 8) | HASHPROXY_SVID};
            std::vector<uint32_t> m_vecKey;
            std::vector<uint32_t> m_vecSvid;

            uint32_t m_uUri;
            std::string m_strPacket;

            virtual void marshal(sox::Pack &p) const
            {
                marshal_container(p, m_vecKey);
                marshal_container(p, m_vecSvid);
                p << m_uUri << m_strPacket;
            }

            virtual void unmarshal(const sox::Unpack &p)
            {
                unmarshal_container(p, std::back_inserter(m_vecKey));
                unmarshal_container(p, std::back_inserter(m_vecSvid));
                p >> m_uUri >> m_strPacket;
            }
        };

		// 新协议，解决携带包超过0xFFFF，抛异常
		struct PCS_HashRequest2: public sox::Marshallable
		{
			enum { uri = (21 << 8) | HASHPROXY_SVID};
			std::vector<uint32_t> m_vecKey;
			std::vector<uint32_t> m_vecSvid;

			uint32_t m_uUri;
			std::string m_strPacket;

			virtual void marshal(sox::Pack &p) const
			{
				marshal_container(p, m_vecKey);
				marshal_container(p, m_vecSvid);
				p << m_uUri;
				p.push_varstr32(m_strPacket.data(), m_strPacket.size());
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				unmarshal_container(p, std::back_inserter(m_vecKey));
				unmarshal_container(p, std::back_inserter(m_vecSvid));
				p >> m_uUri;
				m_strPacket = p.pop_varstr32();
			}
		};

        struct PCS_PrepareUdpLogin: public sox::Marshallable
        {
            enum {uri = (11 << 8 | protocol::IMLINKD_SVID)};
            std::string m_strEncKey;    //encrypt key for client, decrypt key for server

            virtual void marshal(sox::Pack &p) const
            {
                p << m_strEncKey;
            }

            virtual void unmarshal(const sox::Unpack &p)
            {
                p >> m_strEncKey;
            }
        };

        struct PCS_PrepareUdpLoginRes: public sox::Marshallable
        {
            enum {uri = (12 << 8 | protocol::IMLINKD_SVID)};
            std::string m_strDecKey;    //decrypt key for client, encrypt key for server
            uint32_t m_uCookie;
            std::vector<uint16_t> m_vecPort;

            virtual void marshal(sox::Pack &p) const
            {
                p << m_strDecKey << m_uCookie;
                marshal_container(p, m_vecPort);
            }

            virtual void unmarshal(const sox::Unpack &p)
            {
                p >> m_strDecKey >> m_uCookie;
                unmarshal_container(p, std::back_inserter(m_vecPort));
            }
        };

        //encrypt udp packet
        struct PCS_EncryptUdpRouter: public sox::Marshallable
        {
			//最外层的udp包,也就是传输用的udp包
            enum {uri = (13 << 8 | protocol::IMLINKD_SVID)};
            uint32_t m_uUid;//用户id
            std::string m_strEncCtx;//PCS_UdpRouter包加密后的串

            virtual void marshal(sox::Pack &p) const
            {
                p << m_uUid << m_strEncCtx;
            }

            virtual void unmarshal(const sox::Unpack &p)
            {
                p >> m_uUid >> m_strEncCtx;
            }
        };

        //raw udp packet
        struct PCS_UdpRouter: public sox::Marshallable
        {

            enum {uri = (14 << 8 | protocol::IMLINKD_SVID)};
            uint32_t m_uCookie;//用户cookie
            uint32_t m_uUri;//应用包加密前的消息id
            std::string m_strRawCtx;//应用包对应的串（例如聊天包)

            virtual void marshal(sox::Pack &p) const
            {
                p << m_uCookie << m_uUri << m_strRawCtx;
            }

            virtual void unmarshal(const sox::Unpack &p)
            {
                p >> m_uCookie >> m_uUri >> m_strRawCtx;
            }
        };

        struct PCS_UdpPing: public sox::Voidmable
        {
            enum {uri = (15 << 8 | protocol::IMLINKD_SVID)};
        };

        struct PCS_UdpLogin: public sox::Marshallable
        {
            enum {uri = (16 << 8 | protocol::IMLINKD_SVID)};
            uint32_t m_uUid;
            std::string m_strCookie;
            std::string m_strShaPasswd;
            std::string m_strAccount;

            virtual void marshal(sox::Pack &p) const
            {
                p << m_uUid << m_strCookie << m_strShaPasswd << m_strAccount;
            }

            virtual void unmarshal(const sox::Unpack &p)
            {
                p >> m_uUid >> m_strCookie >> m_strShaPasswd >> m_strAccount;
            }
        };

        struct PCS_UdpLoginRes: public sox::Marshallable
        {
            enum {uri = (17 << 8 | protocol::IMLINKD_SVID)};
            uint32_t m_uResCode;

            virtual void marshal(sox::Pack &p) const
            {
                p << m_uResCode;
            }

            virtual void unmarshal(const sox::Unpack &p)
            {
                p >> m_uResCode;
            }
        };
		struct PCS_PrepareLoginImReq2 : public sox::Marshallable {
			enum {uri = (18 << 8) | protocol::IMLINKD_SVID};

			PCS_PrepareLoginImReq2() {}
			PCS_PrepareLoginImReq2(uint32_t uid, const std::string &cookie, const std::string& passwd, const std::string& account, const protocol::E_IMSTATUS status)
				: m_uid(uid), m_cookie(cookie), m_shaPasswd(passwd), m_account(account),m_status(status) 
			{
			}

			uint32_t m_uid;	
			std::string m_cookie;
			std::string m_shaPasswd;
			std::string m_account;
			protocol::E_IMSTATUS m_status;

			virtual void marshal(sox::Pack &pk) const 
			{
				pk << m_uid << m_cookie << m_shaPasswd << m_account ;
				pk.push_uint16(m_status);
			}
			virtual void unmarshal(const sox::Unpack &up) 
			{
				up >> m_uid >> m_cookie >> m_shaPasswd >> m_account;
				m_status = static_cast<protocol::E_IMSTATUS>(up.pop_uint16());
			}
		};

		struct PCS_GetPublicAddr: public sox::Marshallable
		{
			enum {uri = (100 << 8) | protocol::IMLINKD_SVID};
			std::string m_strLocalIP;
			uint16_t m_uLocalPort;
			std::string m_strGUID;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_strLocalIP << m_uLocalPort << m_strGUID;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_strLocalIP >> m_uLocalPort >> m_strGUID;
			}
		};

		struct PCS_GetPublicAddrRes: public sox::Marshallable
		{
			enum {uri = (101 << 8) | protocol::IMLINKD_SVID};
			std::string m_strPublicIP;
			uint16_t m_uPublicPort;
			std::string m_strLocalIP;
			uint16_t m_uLocalPort;
			std::string m_strGUID;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_strPublicIP << m_uPublicPort << m_strLocalIP << m_uLocalPort << m_strGUID;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_strPublicIP >> m_uPublicPort >> m_strLocalIP >> m_uLocalPort >> m_strGUID;
			}
		};


		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//UDP交换密钥 protocol -> imlink
		struct PCS_UdpExchangeKey : public sox::Marshallable 
		{
			enum {uri = (256 << 8) | protocol::IMLINKD_SVID};

			uint32_t m_uUid;
			std::string m_publicKey;
			std::string m_e;
			std::string m_strCookie; //对m_uUid + m_publicKey + m_e生成cookie

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uUid << m_publicKey << m_e << m_strCookie;
			}
			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uUid >> m_publicKey >> m_e >> m_strCookie;
			}

			//生成Cookie
			void ToCookie()
			{
				char szBuf[512] = {0};
				sprintf(szBuf, "%u:%s:%s", m_uUid, m_publicKey.c_str(), m_e.c_str());

				m_strCookie = ToCookieCommon(szBuf);
			}
			//Cookie有效性检查，相等返回true，否则返回false
			bool CheckCookie()
			{
				char szBuf[512] = {0};
				sprintf(szBuf, "%u:%s:%s", m_uUid, m_publicKey.c_str(), m_e.c_str());

				std::string strCookie = ToCookieCommon(szBuf);
				return (strCookie == m_strCookie);
			}
		};

		//UDP交换密钥回应 imlink -> protocol
		struct PCS_UdpExchangeKeyRes : public sox::Marshallable 
		{
			enum {uri = (257 << 8) | protocol::IMLINKD_SVID};

			std::string m_encSessionKey;
			uint32_t m_uCookie;

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_encSessionKey << m_uCookie;
			}
			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_encSessionKey >> m_uCookie;
			}
		};

		//最外层的udp包,也就是传输用的udp包
		struct PCS_EncryptUdpRouterCommon: public sox::Marshallable
		{
			enum {uri = (258 << 8 | protocol::IMLINKD_SVID)};

			uint32_t m_uUid; //用户id
			uint32_t m_uUri; //
			std::string m_strEncCtx;//PCS_UdpRouter包加密后的串

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid << m_uUri << m_strEncCtx;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid >> m_uUri >> m_strEncCtx;
			}
		};

		//原始数据包，用于对上层业务层进行包装 protocol <-> imlink
		struct PCS_UdpRouterCommon : public sox::Marshallable
		{
			enum {uri = (259 << 8 | protocol::IMLINKD_SVID)};

			uint32_t m_uCookie; //用户cookie，解密后可以用来验证报文的有效性
			uint32_t m_uUri; //应用包加密前的消息id
			std::string m_strRawCtx; //应用包对应的串（例如聊天包)
			uint32_t m_uPacketId; //包号

			virtual void marshal(sox::Pack &pk) const
			{
				pk << m_uCookie << m_uUri << m_strRawCtx << m_uPacketId;
			}

			virtual void unmarshal(const sox::Unpack &up)
			{
				up >> m_uCookie >> m_uUri >> m_strRawCtx >> m_uPacketId;
			}
		};

		struct PCS_UdpRouteBase : public sox::Marshallable
		{
			uint32_t m_uFlag; //标志，低8位表示登录的路数

			EImFrontEndNo m_emRouteNumber; 

			PCS_UdpRouteBase()
			{
				m_uFlag = 0;
			}

			void SetRouteNumber(EImFrontEndNo emNumber)
			{
				m_uFlag |= (0x000000FF & emNumber);
			}
			EImFrontEndNo GetRouteNumber()
			{
				return (EImFrontEndNo)(m_uFlag & 0x000000FF);
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uFlag;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uFlag;
			}
		};

		//UDP通用登录认证消息
		struct PCS_UdpLoginCommon: public PCS_UdpRouteBase
		{
			enum {uri = (260 << 8 | protocol::IMLINKD_SVID)};

			uint32_t m_uUid;
			std::string m_strCookie;
			std::string m_strShaPasswd;
			std::string m_strAccount;

			virtual void marshal(sox::Pack &p) const
			{
				PCS_UdpRouteBase::marshal(p);
				p << m_uUid << m_strCookie << m_strShaPasswd << m_strAccount;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				PCS_UdpRouteBase::unmarshal(p);
				p >> m_uUid >> m_strCookie >> m_strShaPasswd >> m_strAccount;
			}
		};

		//登录回应
		struct PCS_UdpLoginCommonRes : public sox::Marshallable
		{
			enum {uri = (261 << 8 | protocol::IMLINKD_SVID)};

			uint32_t m_uResCode;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uResCode;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uResCode;
			}
		};

		//通用退出登录消息
		struct PCS_UdpLogoutCommon : public PCS_UdpLoginCommon
		{
			enum {uri = (262 << 8 | protocol::IMLINKD_SVID)};

			uint32_t m_uUid;

			virtual void marshal(sox::Pack &p) const
			{
				PCS_UdpRouteBase::marshal(p);
				p << m_uUid;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				PCS_UdpRouteBase::unmarshal(p);
				p >> m_uUid;
			}
		};

		//通用Ping包
		struct PCS_UdpPingCommon : public sox::Voidmable
		{
			enum {uri = (263 << 8 | protocol::IMLINKD_SVID)};
		};

		//企业用户登录时，收到的消息项
		struct SEnterpriseLinkInfo : public sox::Marshallable
		{
			uint32_t uNetType;
			uint32_t uGroupId;
			std::string strLinkIp;
			std::vector<uint16_t> vecPort;

			virtual void marshal(sox::Pack &p) const
			{
				p << uNetType << uGroupId << strLinkIp;
				sox::marshal_container(p, vecPort);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> uNetType >> uGroupId >> strLinkIp;
				sox::unmarshal_container(p, std::back_inserter(vecPort));
			}
		};

		//企业用户登录时，会收到该消息
		struct PCS_EnterpriseInfo : public sox::Marshallable
		{
			enum {uri = (264 << 8 | protocol::IMLINKD_SVID)};

			uint32_t m_uEid;
			std::vector<SEnterpriseLinkInfo> m_vecLinkInfo;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uEid;
				sox::marshal_container(p, m_vecLinkInfo);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uEid;
				sox::unmarshal_container(p, std::back_inserter(m_vecLinkInfo));
			}
		};

		// 是否向手机yy用户广播数据

		enum EBroadMobile
		{
			ENUM_NOT_BROADCAST_MOBILE = 0,  // 默认不向手机客户端广播数据
			ENUM_BROADCAST_MOBILE
		};

		struct PCS_ChangeBroadcastFlag : public sox::Marshallable
		{
			enum {uri = (40 << 8 | protocol::IMLINKD_SVID)};

			EBroadMobile m_emFlag;

			virtual void marshal(sox::Pack &p) const
			{
				p << (uint32_t)m_emFlag;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				m_emFlag = (EBroadMobile)p.pop_uint32();
			}
		};

		struct PCS_ChangeBroadcastFlagRes : public PCS_ChangeBroadcastFlag
		{
			enum {uri = (41 << 8 | protocol::IMLINKD_SVID)};
		};

		// app status

		struct PCS_AppLogin : public sox::Marshallable
		{
			enum {uri = (42 << 8 | protocol::IMLINKD_SVID)};

			PCS_AppLogin() {}
			PCS_AppLogin(uint32_t uAppId, protocol::E_IMSTATUS emSt)
				: m_uAppId(uAppId), m_emStatus(emSt)
			{
			}

			uint32_t             m_uAppId;
			protocol::E_IMSTATUS m_emStatus;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uAppId;
				p.push_uint16(m_emStatus);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uAppId;
				m_emStatus = (protocol::E_IMSTATUS)p.pop_uint16();
			}
		};

		struct PCS_AppLoginRes : public PCS_AppLogin
		{
			enum {uri = (43 << 8 | protocol::IMLINKD_SVID)};

			PCS_AppLoginRes() {}
			PCS_AppLoginRes(uint32_t uAppId, protocol::E_IMSTATUS emSt)
				: PCS_AppLogin(uAppId, emSt) 
			{
			}
		};

		struct PCS_AppChangeStatus : public PCS_AppLogin
		{
			enum {uri = (44 << 8 | protocol::IMLINKD_SVID)};

			PCS_AppChangeStatus() {}
			PCS_AppChangeStatus(uint32_t uAppId, protocol::E_IMSTATUS emSt)
				: PCS_AppLogin(uAppId, emSt) 
			{
			}
		};

		struct PCS_AppChangeStatusRes : public PCS_AppLogin
		{
			enum {uri = (45 << 8 | protocol::IMLINKD_SVID)};

			PCS_AppChangeStatusRes() {}
			PCS_AppChangeStatusRes(uint32_t uAppId, protocol::E_IMSTATUS emSt)
				: PCS_AppLogin(uAppId, emSt) 
			{
			}
		};

		// batch get app user status
		
		struct PCS_AppBatchGetStatus : public sox::Marshallable
		{
			enum {uri = (45 << 8 | protocol::IMTRANS_SVID)};

			uint32_t m_uAppId;
			std::vector<uint32_t> m_vecUid;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uAppId;
				sox::marshal_container(p, m_vecUid);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uAppId;
				sox::unmarshal_container(p, back_inserter(m_vecUid));
			}
		};

		struct PCS_AppBatchGetStatusRes : public sox::Marshallable
		{
			enum {uri = (46 << 8 | protocol::IMTRANS_SVID)};

			uint32_t m_uAppId;
			std::map<uint32_t, protocol::E_IMSTATUS> m_mapUid2St;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uAppId;
				p.push_uint32(m_mapUid2St.size());
				for (std::map<uint32_t, protocol::E_IMSTATUS>::const_iterator it = m_mapUid2St.begin(); it != m_mapUid2St.end(); ++it)
				{
					p << it->first;
					p.push_uint8(it->second);
				}
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uAppId;
				uint32_t uLen = p.pop_uint32();
				for (uint32_t i = 0; i < uLen; i++)
				{
					m_mapUid2St[p.pop_uint32()] = (protocol::E_IMSTATUS)p.pop_uint8();
				}
			}
		};
	}
}


#endif
