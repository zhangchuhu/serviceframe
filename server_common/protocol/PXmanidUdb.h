#ifndef _XMANIDUDB_H
#define _XMANIDUDB_H

#include <set>
#include <string>
#include "int_types.h"
#include "common/core/base_svid.h"
#include "common/packet.h"
#include "common/protocol/const.h"
#include "common/protocol/zip_tpl.h"
#include "protocol/CodeInfo.h"
#include "common/res_code.h"
#include "server_login/common/xxtea/xxtea.h"

namespace server{
    namespace xmanudb{

        struct MobileXuid: public sox::Marshallable
        {
            std::string m_strMobileNum;
            uint64_t m_dwMyXUid;

            MobileXuid():m_strMobileNum(""),m_dwMyXUid(0){}

            bool operator<( const MobileXuid& s) const
            {
                return s.m_strMobileNum < m_strMobileNum;
            }
            virtual void marshal(sox::Pack &pk) const
            {
                pk << m_strMobileNum << m_dwMyXUid;
            }
            virtual void unmarshal( const sox::Unpack &up)
            {
                up >> m_strMobileNum >> m_dwMyXUid;
            }
        };

        struct PSS_CreateXmanidBatch: public sox::Marshallable
        {
            enum {uri = (91 << 8) | protocol::SERVERLOGIN_SVID};

            std::string 	m_strServiceHead;
            uint32_t 		m_uOriUri;
            std::string 	m_strOriPacket;

            std::set<std::string> m_SetMobileNumList;

            PSS_CreateXmanidBatch(){}
			PSS_CreateXmanidBatch(const std::string &strServiceHead, const std::set<std::string> &setMobileNumList, const uint32_t uOriUri) :
				m_strServiceHead(strServiceHead),
				m_uOriUri(uOriUri)
			{
				m_SetMobileNumList = setMobileNumList;
			}

            virtual void marshal(sox::Pack &pk) const
            {
                pk << m_strServiceHead << m_uOriUri << m_strOriPacket ;
                sox::marshal_container( pk, m_SetMobileNumList);
            }
            virtual void unmarshal( const sox::Unpack &up)
            {
                up >> m_strServiceHead >> m_uOriUri >> m_strOriPacket ;
                sox::unmarshal_container( up, std::inserter( m_SetMobileNumList, m_SetMobileNumList.begin()));
            }
        };

        struct PSS_CreateXmanidBatchRes: public sox::Marshallable
        {
            enum {uri = (92 << 8) | protocol::SERVERLOGIN_SVID};

            std::string 	m_strServiceHead;
            uint32_t 		m_uOriUri;
            std::string 	m_strOriPacket;

            std::set<MobileXuid>  m_SetXUidCreateResList;

            PSS_CreateXmanidBatchRes(){}

            virtual void marshal(sox::Pack &pk) const
            {
                pk << m_strServiceHead << m_uOriUri << m_strOriPacket ;
                sox::marshal_container( pk, m_SetXUidCreateResList);
            }
            virtual void unmarshal( const sox::Unpack &up)
            {
                up >> m_strServiceHead >> m_uOriUri >> m_strOriPacket ;
                sox::unmarshal_container( up, std::inserter( m_SetXUidCreateResList, m_SetXUidCreateResList.begin()));
            }
        };

        // Query infomation

        struct UserRegInfo: public sox::Marshallable
        {
            uint64_t    m_dwUid;
            std::string m_strUsername;
            std::string m_strMobile;
            std::string m_strEmail;
            std::string m_strRegDate;

            UserRegInfo():m_dwUid(0),m_strUsername(""),m_strMobile(""),m_strEmail(""),m_strRegDate(""){}

            virtual void marshal(sox::Pack &pk) const
            {
                std::string strMobileEncrypt = m_strMobile;
                std::string strEmailEncrypt = m_strEmail;
                XXTEA::encrypt( strMobileEncrypt);
                XXTEA::encrypt( strEmailEncrypt);
                pk << m_dwUid << m_strUsername << strMobileEncrypt << strEmailEncrypt << m_strRegDate;
            }
            virtual void unmarshal( const sox::Unpack &up)
            {
                up >> m_dwUid >> m_strUsername >> m_strMobile >> m_strEmail >> m_strRegDate;
                XXTEA::decrypt( m_strMobile);
                XXTEA::decrypt( m_strEmail);
            }
        };

        struct PSS_QueryXmanInfomation: public sox::Marshallable
        {
            enum {uri = (31 << 8) | protocol::SERVERLOGIN_SVID};

            std::string 	m_strServiceHead;
            uint32_t 		m_uOriUri;
            std::string 	m_strOriPacket;

            uint64_t    m_dwXuid;
            
            PSS_QueryXmanInfomation(){}
			PSS_QueryXmanInfomation(const std::string &strServiceHead, const uint64_t dwXuid, const uint32_t uOriUri) :
				m_strServiceHead(strServiceHead),
				m_uOriUri(uOriUri)
			{
                m_dwXuid = dwXuid;
			}

            virtual void marshal(sox::Pack &pk) const
            {
                pk << m_strServiceHead << m_uOriUri << m_strOriPacket << m_dwXuid;
            }
            virtual void unmarshal( const sox::Unpack &up)
            {
                up >> m_strServiceHead >> m_uOriUri >> m_strOriPacket >> m_dwXuid;
            }
        };

        struct PSS_QueryXmanInfomationRes: public sox::Marshallable
        {
            enum {uri = (32 << 8) | protocol::SERVERLOGIN_SVID};

            std::string 	m_strServiceHead;
            uint32_t 		m_uOriUri;
            std::string 	m_strOriPacket;

            uint64_t    m_dwXuid;
            uint32_t    m_uRescode; // 0(success), 1(user_does_not_exist), 3(system_error)
            std::string m_strError;
            UserRegInfo m_sUserRegInfo;

            PSS_QueryXmanInfomationRes():m_uRescode(0),m_strError(""){}
			PSS_QueryXmanInfomationRes(const std::string &strServiceHead, const uint64_t dwXuid, const uint32_t uOriUri) :
				m_strServiceHead(strServiceHead),
				m_uOriUri(uOriUri)
			{
                m_dwXuid = dwXuid;
			}

            virtual void marshal(sox::Pack &pk) const
            {
                pk << m_strServiceHead << m_uOriUri << m_strOriPacket << m_dwXuid << m_uRescode << m_strError << m_sUserRegInfo;
            }
            virtual void unmarshal( const sox::Unpack &up)
            {
                up >> m_strServiceHead >> m_uOriUri >> m_strOriPacket >> m_dwXuid >> m_uRescode >> m_strError >> m_sUserRegInfo;
            }
        };

        struct PSS_SendXmanSms: public sox::Marshallable
        {
            enum {uri = (33 << 8) | protocol::SERVERLOGIN_SVID};

            uint64_t m_dwUid;
            std::string m_strMobile;
            std::string m_strSmsText;

            PSS_SendXmanSms():m_dwUid(0),m_strMobile(""),m_strSmsText(""){}

            virtual void marshal(sox::Pack &pk) const
            {
                pk << m_dwUid << m_strMobile << m_strSmsText;
            }
            virtual void unmarshal( const sox::Unpack &up)
            {
                up >> m_dwUid >> m_strMobile >> m_strSmsText;
            }

        };

        // conn with xman_proxy //
        //
        
        enum XmanRegdRescode // global rescode
        {
            XM_SUCCESS = 0,
            XM_MOBILE_INVALID = 1,
            XM_SMSCODE_FAIL = 2,
            XM_USER_EXIST = 3,
            XM_USER_NOT_EXIST = 4,
            XM_REGISTER_FAIL = 5,
            XM_MOBILE_NOT_MATCH = 6,
            XM_ARGS_INVALID = 7,
            XM_SYSTEM_ERROR = 99
        };
        const static std::string cSuccessDesc = "success";
        const static std::string cMobileInvalidDesc = "mobile_invalid";
        const static std::string cSmscodeFailDesc = "smscode_fail";
        const static std::string cUserExistDesc = "user_exist";
        const static std::string cUserNotExistDesc = "user_not_exist";
        const static std::string cRegisterFailDesc = "register_fail";
        const static std::string cMobileNotMatchDesc = "mobile_not_match";
        const static std::string cArgsInvalidDesc = "args_invalid";
        const static std::string cSystemErrorDesc = "system_error";

        const static std::string& get_xm_rescode_desc( const uint32_t& rc)
        {
            switch( rc)
            {
                case XM_SUCCESS:
                    return cSuccessDesc;
                case XM_MOBILE_INVALID:
                    return cMobileInvalidDesc;
                case XM_SMSCODE_FAIL:
                    return cSmscodeFailDesc;
                case XM_USER_EXIST:
                    return cUserExistDesc;
                case XM_USER_NOT_EXIST:
                    return cUserNotExistDesc;
                case XM_REGISTER_FAIL:
                    return cRegisterFailDesc;
                case XM_MOBILE_NOT_MATCH:
                    return cMobileNotMatchDesc;
                case XM_ARGS_INVALID:
                    return cArgsInvalidDesc;
                case XM_SYSTEM_ERROR:
                    return cSystemErrorDesc;
                default:
                    return cSystemErrorDesc;
            };
            return cSystemErrorDesc;
        }

        struct PSS_XmanRegisterReq: public sox::Marshallable
        {
            enum {uri = (40 << 8) | protocol::SERVERLOGIN_SVID};
            
			std::string context;
            std::string mobile;
            std::string pwd;

            PSS_XmanRegisterReq():mobile(""),pwd(""){}

            virtual void marshal(sox::Pack &pk) const
            {
                pk << context << mobile << pwd;
            }
            virtual void unmarshal( const sox::Unpack &up)
            {
                up >> context >> mobile >> pwd;
            }
        };
        struct PSS_XmanRegisterRes: public sox::Marshallable
        {
            enum {uri = (41 << 8) | protocol::SERVERLOGIN_SVID};

			std::string context;
            uint32_t rescode;
            std::string strerror;

            PSS_XmanRegisterRes():rescode(0),strerror(""){}

            virtual void marshal(sox::Pack &pk) const
            {
                pk << context << rescode << strerror;
            }
            virtual void unmarshal( const sox::Unpack &up)
            {
                up >> context >> rescode >> strerror;
            }
        };

        struct PSS_XmanRegisterVerifyReq: public sox::Marshallable
        {
            enum {uri = (42 << 8) | protocol::SERVERLOGIN_SVID};

			std::string context;
            std::string mobile;
            std::string passport;
            std::string smscode;
            uint32_t ip;

            PSS_XmanRegisterVerifyReq():mobile(""),passport(""),smscode(""),ip(0){}

            virtual void marshal(sox::Pack &pk) const
            {
                pk << context << mobile << passport << smscode << ip;
            }
            virtual void unmarshal( const sox::Unpack &up)
            {
                up >> context >> mobile >> passport >> smscode >> ip;
            }
        };
        struct PSS_XmanRegisterVerifyRes: public sox::Marshallable
        {
            enum {uri = (43 << 8) | protocol::SERVERLOGIN_SVID};

			std::string context;
            uint32_t rescode;
            std::string strerror;

            PSS_XmanRegisterVerifyRes():rescode(0),strerror(""){}

            virtual void marshal(sox::Pack &pk) const
            {
                pk << context << rescode << strerror;
            }
            virtual void unmarshal( const sox::Unpack &up)
            {
                up >> context >> rescode >> strerror;
            }
        };

        struct PSS_XmanForgotPasswordReq: public sox::Marshallable
        {
            enum {uri = (44 << 8) | protocol::SERVERLOGIN_SVID};

			std::string context;
            std::string mobile;

            PSS_XmanForgotPasswordReq():mobile(""){}

            virtual void marshal(sox::Pack &pk) const
            {
                pk << context << mobile;
            }
            virtual void unmarshal( const sox::Unpack &up)
            {
                up >> context >> mobile;
            }
        };
        struct PSS_XmanForgotPasswordRes: public sox::Marshallable
        {
            enum {uri = (45 << 8) | protocol::SERVERLOGIN_SVID};

			std::string context;
            uint32_t rescode;
            std::string strerror;

            PSS_XmanForgotPasswordRes():rescode(0),strerror(""){}

            virtual void marshal(sox::Pack &pk) const
            {
                pk << context << rescode << strerror;
            }
            virtual void unmarshal( const sox::Unpack &up)
            {
                up >> context >> rescode >> strerror;
            }
        };

        struct PSS_XmanModifyPasswordReq: public sox::Marshallable
        {
            enum {uri = (46 << 8) | protocol::SERVERLOGIN_SVID};

			std::string context;
            std::string mobile;
            std::string passport;
            std::string pwd;
            std::string smscode;

            PSS_XmanModifyPasswordReq():mobile(""),passport(""),pwd(""),smscode(""){}

            virtual void marshal(sox::Pack &pk) const
            {
                pk << context << mobile << passport << pwd << smscode << smscode;
            }
            virtual void unmarshal( const sox::Unpack &up)
            {
                up >> context >> mobile >> passport >> pwd >> smscode >> smscode;
            }
        };
        struct PSS_XmanModifyPasswordRes: public sox::Marshallable
        {
            enum {uri = (47 << 8) | protocol::SERVERLOGIN_SVID};

			std::string context;
            uint32_t rescode;
            std::string strerror;

            PSS_XmanModifyPasswordRes():rescode(0),strerror(""){}

            virtual void marshal(sox::Pack &pk) const
            {
                pk << context << rescode << strerror;
            }
            virtual void unmarshal( const sox::Unpack &up)
            {
                up >> context >> rescode >> strerror;
            }
        };

    }
}

#endif
