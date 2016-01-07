#ifndef PROTOCOL_GPROPS_TYPE_H
#define PROTOCOL_GPROPS_TYPE_H

#include "common/int_types.h"
#include "common/soxhelper.h"

namespace protocol
{
	namespace gprops
	{
		enum ELogoIdex
		{
			ENUM_DEFULT = -1,	// 默认logo
			ENUM_CUST = 0		// 自定义logo
		};

		enum ETopicMode
		{
			ENUM_NO_AUTH,		//Allow anyone raising topic
			ENUM_ADMIN_ADDING,	//Just allow admin to raise topic
			ENUM_UNKNOWN_MODE	//Abnormal value
		};

		//enum EPropsInfoType
		//{
		//	ENUM_PROPS_INFO_SIMPLE = 0,	//Simple Property flag
		//	ENUM_PROPS_INFO_DETAIL = 1	//Detail Property Flag
		//};

		// group props info mask
		union GGroupPropsMask
		{
			struct Bits {
				uint8_t Name:1;
				uint8_t Bulletin:1;    // 公告
				uint8_t Desc:1;        // 简介

				uint8_t AuthMode:1;    // 身份验证

				uint8_t TopicMode:1;    // 发表主题模式
				uint8_t Category:1;    // 大类
				uint8_t SubCategory:1; // 小类

				uint8_t LogoIndex:1;   // -1 默认 ; 0 自定义; 其他 系统
				uint8_t AdhocChat:1;   // 临时会话
				uint8_t Privacy:1;     // 是否允许非分组人员查看 ？
				uint8_t LogoURL:1;
			} b;

			enum EFilter
			{
				ENUM_FILTER_BASE    = 0x00,      // 000 0000 000
				ENUM_FILTER_DEFAULT = 0x7FF      // 111 1111 1111
			};

			uint16_t m_uMask;
		};

		inline  GGroupPropsMask GenGroupPropsMask(GGroupPropsMask::EFilter filter)
		{
			GGroupPropsMask mask;
			mask.m_uMask |= filter;
			return mask;
		}

		// folder property mask
		union GFolderPropsMask
		{
			struct Bits {
				uint8_t Name:1;
				uint8_t Bulletin:1;		// 公告
				uint8_t Desc:1;			// 简介
				uint8_t AuthMode:1;		// 组验证模式
				uint8_t TopicMode:1;    //发表主题模式
			}b;

			enum EFilter
			{
				ENUM_FILTER_BASE    = 0x00,      // 000
				ENUM_FILTER_DEFAULT = 0xF        // 1111
			};

			uint8_t m_uMask;
		};

		inline GFolderPropsMask GenFolderPropsMask(GFolderPropsMask::EFilter filter)
		{
			GFolderPropsMask mask;
			mask.m_uMask |= filter;
			return mask;
		}

		// group/folder auth_mode mask
		union GAuthModeMask
		{
			struct Bits {
				uint8_t RoleOwner:1;//会长 OWNER
				uint8_t RoleViceOwner:1;//副会长(总管) VICE_OWNER
				uint8_t RoleManager:1;//全频道管理员 MANANGER
				uint8_t RoleSubManager:1;//子频道管理员 CMANAGER ,PMANAGER
				uint8_t RoleMember:1;//会员  MEMBER
				uint8_t RoleVIP:1;//嘉宾 VIP
			} b;

			enum EFilter
			{
				ENUM_FILTER_BASE    = 0x00,
				ENUM_FILTER_DEFAULT = 0x7FF
			};

			uint16_t m_uMask;
		};

		union GImportChannelRoleMask
		{
			struct Bits {
				uint8_t RoleViceOwner:1;//副会长(总管) VICE_OWNER
				uint8_t RoleManager:1;//全频道管理员 MANANGER
				uint8_t RoleSubManager:1;//子频道管理员 CMANAGER
				uint8_t RoleMember:1;//会员  MEMBER
			} b;

			enum EFilter
			{
				ENUM_R_FILTER_BASE    = 0x00,
				ENUM_R_FILTER_DEFAULT = 0x7FF
			};

			uint16_t m_uMask;
		};
//////////////////////////////////////////////////////////////////////////

		inline void Uint2String(uint32_t u, std::string& str)
		{
			/*sox::PackBuffer pb;
			sox::Pack pk(pb);
			pk.push_uint32(u);
			str.assign(pb.data(), pb.size());*/
			str = sox::ultoa10(u);
		}

		inline void Uint2String(uint16_t u, std::string& str)
		{
			/*sox::PackBuffer pb;
			sox::Pack pk(pb);
			pk.push_uint16(u);
			str.assign(pb.data(), pb.size());*/
			str = sox::ultoa10(u);
		}

		inline void Uint2String(uint8_t u, std::string& str)
		{
			/*sox::PackBuffer pb;
			sox::Pack pk(pb);
			pk.push_uint8(u);
			str.assign(pb.data(), pb.size());*/
			str = sox::ultoa10(u);
		}

		inline uint16_t String2Uint16_t (const std::string& str)
		{
			/*sox::Unpack p(str.data(), str.size());
			return p.pop_uint16();*/
			return (uint16_t)sox::xatol(str.data(), str.size());
		}

		inline uint8_t String2Uint8_t (const std::string& str)
		{
			/*sox::Unpack p(str.data(), str.size());
			return p.pop_uint8();*/
			return (uint8_t)sox::xatol(str.data(), str.size());
		}

		inline uint32_t String2Uint32_t (const std::string& str)
		{
			/*sox::Unpack p(str.data(), str.size());
			return p.pop_uint32();*/
			return sox::xatol(str.data(), str.size());
		}
	}
}
#endif

