// -*- c++ -*-
// PMakingFriend.h -- YY交友应用YY协议定义
// TODO 添加文件说明

#ifndef SERVER_MAKINGFRIEND_H
#define SERVER_MAKINGFRIEND_H


#include "common/core/base_svid.h"
#include "common/packet.h"

#include <vector>
#include <string>
#include "common/protocol/iappfans.h"


namespace server {
    namespace makingfriend {

        // 简单联系人信息
        struct SlimContactInfo : public sox::Marshallable {
            uint32_t uid;          // 联系人的uid
            std::string nickname;  // 联系人的昵称

            /* Marshallable接口实现 */

            virtual void marshal(sox::Pack &pack) const {
                pack << uid;
                pack << nickname;
            }

            virtual void unmarshal(const sox::Unpack &unpack) {
                unpack >> uid;
                unpack >> nickname;
            }
        };


        // 要求获得用户全部联系人(联系人可以是粉丝或关注的人)简单资料(只包括uid和昵称)的请求包
        struct PGetSlimContactListPackage: public sox::Marshallable {

            // YY协议uri
            enum{uri = (155 << 8) | protocol::MAKINGFRIEND_APP_SVID};
            // 要查询用户的uid
            uint32_t uid;
            // 要查询的联系人属性，分为粉丝，关注的人两种
			protocol::imapp::ContactProperty contactProperty;


            /* Marshallable接口实现 */

            virtual void marshal(sox::Pack &pack) const
            {
                pack << uid;
                pack.push_uint8(static_cast<uint8_t>(contactProperty));
            }

            virtual void unmarshal(const sox::Unpack &unpack)
            {
                unpack >> uid;
				contactProperty = static_cast<protocol::imapp::ContactProperty>(unpack.pop_uint8());
            }
        };


        // 对应于PGetSlimContactListPackage的回复包
        struct PGetSlimContactListPackageRes: public sox::Marshallable {

            // YY协议uri
            enum{uri = (156 << 8) | protocol::MAKINGFRIEND_APP_SVID};
            // 要查询用户的uid，对应于请求包的uid
            uint32_t uid;
            // 查询的属性，对应于请求包的contactProperty
            protocol::imapp::ContactProperty contactProperty;
            // 全部联系人的简单资料列表
            std::vector<SlimContactInfo> slimContacts;


            /* Marshallable接口实现 */

            virtual void marshal(sox::Pack &pack) const
            {
                pack << uid;
                pack.push_uint8(static_cast<uint8_t>(contactProperty));
                marshal_container(pack, slimContacts);
            }

            virtual void unmarshal(const sox::Unpack &unpack)
            {
                unpack >> uid;
                contactProperty = static_cast<protocol::imapp::ContactProperty>(unpack.pop_uint8());
                unmarshal_container(unpack, std::back_inserter(slimContacts));
            }
        };


        // 完整的联系人信息
        struct ContactInfo : public sox::Marshallable {

            uint32_t uid;                // 联系人的uid
            std::string nickname;        // 昵称
            bool vip;                   // 是否vip，true表示是vip，false表示不是vip
            uint32_t head_image_version; // 头像小图的版本号，从初始值1开始每个版本增1，值0表示此用户还没有设置小头像

            /* Marshallable接口实现 */

            virtual void marshal(sox::Pack &pack) const {
                pack << uid;
                pack << nickname;
                pack << head_image_version;
                pack << vip;
            }

            virtual void unmarshal(const sox::Unpack &unpack) {
                unpack >> uid;
                unpack >> nickname;
                unpack >> head_image_version;
                unpack >> vip;
            }
        };


        // 要求获得指定的用户部分联系人(联系人可以是粉丝或关注的人)完整资料的请求包
        struct PGetContactListPackage: public sox::Marshallable {

            // YY协议uri
            enum{uri = (157 << 8) | protocol::MAKINGFRIEND_APP_SVID};
            // 要查询用户的uid
            uint32_t uid;

            // 要查询的联系人的uid列表
            std::vector<uint32_t> contactIDs;


            /* Marshallable接口实现 */

            virtual void marshal(sox::Pack &pack) const
            {
                pack << uid;
                marshal_container(pack, contactIDs);
            }

            virtual void unmarshal(const sox::Unpack &unpack)
            {
                unpack >> uid;
                unmarshal_container(unpack, std::back_inserter(contactIDs));
            }
        };


        // 对应于PGetContactListPackage的回复包
        struct PGetContactListPackageRes: public sox::Marshallable {

            // YY协议uri
            enum{uri = (158 << 8) | protocol::MAKINGFRIEND_APP_SVID};
            // 要查询用户的uid，对应于请求包的uid
            uint32_t uid;
            // 全部联系人的完整资料列表
            std::vector<ContactInfo> contacts;


            /* Marshallable接口实现 */

            virtual void marshal(sox::Pack &pack) const
            {
                pack << uid;
                marshal_container(pack, contacts);
            }

            virtual void unmarshal(const sox::Unpack &unpack)
            {
                unpack >> uid;
                unmarshal_container(unpack, std::back_inserter(contacts));
            }
        };


        // 添加一个联系人关系的请求包
        struct PAddContactPackage: public sox::Marshallable {

            // YY协议uri
            enum{uri = (159 << 8) | protocol::MAKINGFRIEND_APP_SVID};
            // 要添加联系人的用户的uid
            uint32_t uid;

            // 要添加的联系人的uid
            uint32_t contactID;


            /* Marshallable接口实现 */

            virtual void marshal(sox::Pack &pack) const
            {
                pack << uid;
                pack << contactID;
            }

            virtual void unmarshal(const sox::Unpack &unpack)
            {
                unpack >> uid;
                unpack >> contactID;
            }
        };


        // 对应于PAddContactPackage的回复包
        struct PAddContactPackageRes: public sox::Marshallable {

            // YY协议uri
            enum{uri = (160 << 8) | protocol::MAKINGFRIEND_APP_SVID};

            // 添加联系人的操作是否成功
            uint32_t operationResult;

            // 添加者的详细信息
            ContactInfo user_info;

            // 被添加者的详细信息
            ContactInfo care_info;


            /* Marshallable接口实现 */

            virtual void marshal(sox::Pack &pack) const
            {
                pack << operationResult;
                pack << user_info;
                pack << care_info;
            }

            virtual void unmarshal(const sox::Unpack &unpack)
            {
                unpack >> operationResult;
                unpack >> user_info;
                unpack >> care_info;
            }
        };


        // 删除一个联系人关系的请求包
        struct PDeleteContactPackage: public sox::Marshallable {

            // YY协议uri
            enum{uri = (161 << 8) | protocol::MAKINGFRIEND_APP_SVID};
            // 要删除联系人的用户的uid
            uint32_t uid;
            
            // 要删除的联系人的uid
            uint32_t contactID;


            /* Marshallable接口实现 */

            virtual void marshal(sox::Pack &pack) const
            {
                pack << uid;
                pack << contactID;
            }

            virtual void unmarshal(const sox::Unpack &unpack)
            {
                unpack >> uid;
                unpack >> contactID;
            }
        };


        // 对应于PDeleteContactPackage的回复包
        struct PDeleteContactPackageRes: public sox::Marshallable {

            // YY协议uri
            enum{uri = (162 << 8) | protocol::MAKINGFRIEND_APP_SVID};

			//被删除者的uid
			uint32_t uid;

            // 删除联系人的操作是否成功
            uint32_t operationResult;

            /* Marshallable接口实现 */

            virtual void marshal(sox::Pack &pack) const
            {
                pack << uid << operationResult;
            }

            virtual void unmarshal(const sox::Unpack &unpack)
            {
                unpack >> uid >> operationResult;
            }
        };

		//被某人关注的通知协议
		struct PFansAdded: public sox::Marshallable {

			// YY协议uri
			enum{uri = (163 << 8) | protocol::MAKINGFRIEND_APP_SVID};

			//添加者的uid
			uint32_t uid;

			//被添加者的uid;
			uint32_t uPeerId;

			/* Marshallable接口实现 */
			virtual void marshal(sox::Pack &pack) const
			{
				pack << uid << uPeerId;
			}

			virtual void unmarshal(const sox::Unpack &unpack)
			{
				unpack >> uid >> uPeerId;
			}
		};
    }
}


#endif /* SERVER_MAKINGFRIEND_H */
