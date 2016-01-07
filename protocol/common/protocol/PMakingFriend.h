// -*- c++ -*-
// PMakingFriend.h -- YY����Ӧ��YYЭ�鶨��
// TODO ����ļ�˵��

#ifndef SERVER_MAKINGFRIEND_H
#define SERVER_MAKINGFRIEND_H


#include "common/core/base_svid.h"
#include "common/packet.h"

#include <vector>
#include <string>
#include "common/protocol/iappfans.h"


namespace server {
    namespace makingfriend {

        // ����ϵ����Ϣ
        struct SlimContactInfo : public sox::Marshallable {
            uint32_t uid;          // ��ϵ�˵�uid
            std::string nickname;  // ��ϵ�˵��ǳ�

            /* Marshallable�ӿ�ʵ�� */

            virtual void marshal(sox::Pack &pack) const {
                pack << uid;
                pack << nickname;
            }

            virtual void unmarshal(const sox::Unpack &unpack) {
                unpack >> uid;
                unpack >> nickname;
            }
        };


        // Ҫ�����û�ȫ����ϵ��(��ϵ�˿����Ƿ�˿���ע����)������(ֻ����uid���ǳ�)�������
        struct PGetSlimContactListPackage: public sox::Marshallable {

            // YYЭ��uri
            enum{uri = (155 << 8) | protocol::MAKINGFRIEND_APP_SVID};
            // Ҫ��ѯ�û���uid
            uint32_t uid;
            // Ҫ��ѯ����ϵ�����ԣ���Ϊ��˿����ע��������
			protocol::imapp::ContactProperty contactProperty;


            /* Marshallable�ӿ�ʵ�� */

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


        // ��Ӧ��PGetSlimContactListPackage�Ļظ���
        struct PGetSlimContactListPackageRes: public sox::Marshallable {

            // YYЭ��uri
            enum{uri = (156 << 8) | protocol::MAKINGFRIEND_APP_SVID};
            // Ҫ��ѯ�û���uid����Ӧ���������uid
            uint32_t uid;
            // ��ѯ�����ԣ���Ӧ���������contactProperty
            protocol::imapp::ContactProperty contactProperty;
            // ȫ����ϵ�˵ļ������б�
            std::vector<SlimContactInfo> slimContacts;


            /* Marshallable�ӿ�ʵ�� */

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


        // ��������ϵ����Ϣ
        struct ContactInfo : public sox::Marshallable {

            uint32_t uid;                // ��ϵ�˵�uid
            std::string nickname;        // �ǳ�
            bool vip;                   // �Ƿ�vip��true��ʾ��vip��false��ʾ����vip
            uint32_t head_image_version; // ͷ��Сͼ�İ汾�ţ��ӳ�ʼֵ1��ʼÿ���汾��1��ֵ0��ʾ���û���û������Сͷ��

            /* Marshallable�ӿ�ʵ�� */

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


        // Ҫ����ָ�����û�������ϵ��(��ϵ�˿����Ƿ�˿���ע����)�������ϵ������
        struct PGetContactListPackage: public sox::Marshallable {

            // YYЭ��uri
            enum{uri = (157 << 8) | protocol::MAKINGFRIEND_APP_SVID};
            // Ҫ��ѯ�û���uid
            uint32_t uid;

            // Ҫ��ѯ����ϵ�˵�uid�б�
            std::vector<uint32_t> contactIDs;


            /* Marshallable�ӿ�ʵ�� */

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


        // ��Ӧ��PGetContactListPackage�Ļظ���
        struct PGetContactListPackageRes: public sox::Marshallable {

            // YYЭ��uri
            enum{uri = (158 << 8) | protocol::MAKINGFRIEND_APP_SVID};
            // Ҫ��ѯ�û���uid����Ӧ���������uid
            uint32_t uid;
            // ȫ����ϵ�˵����������б�
            std::vector<ContactInfo> contacts;


            /* Marshallable�ӿ�ʵ�� */

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


        // ���һ����ϵ�˹�ϵ�������
        struct PAddContactPackage: public sox::Marshallable {

            // YYЭ��uri
            enum{uri = (159 << 8) | protocol::MAKINGFRIEND_APP_SVID};
            // Ҫ�����ϵ�˵��û���uid
            uint32_t uid;

            // Ҫ��ӵ���ϵ�˵�uid
            uint32_t contactID;


            /* Marshallable�ӿ�ʵ�� */

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


        // ��Ӧ��PAddContactPackage�Ļظ���
        struct PAddContactPackageRes: public sox::Marshallable {

            // YYЭ��uri
            enum{uri = (160 << 8) | protocol::MAKINGFRIEND_APP_SVID};

            // �����ϵ�˵Ĳ����Ƿ�ɹ�
            uint32_t operationResult;

            // ����ߵ���ϸ��Ϣ
            ContactInfo user_info;

            // ������ߵ���ϸ��Ϣ
            ContactInfo care_info;


            /* Marshallable�ӿ�ʵ�� */

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


        // ɾ��һ����ϵ�˹�ϵ�������
        struct PDeleteContactPackage: public sox::Marshallable {

            // YYЭ��uri
            enum{uri = (161 << 8) | protocol::MAKINGFRIEND_APP_SVID};
            // Ҫɾ����ϵ�˵��û���uid
            uint32_t uid;
            
            // Ҫɾ������ϵ�˵�uid
            uint32_t contactID;


            /* Marshallable�ӿ�ʵ�� */

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


        // ��Ӧ��PDeleteContactPackage�Ļظ���
        struct PDeleteContactPackageRes: public sox::Marshallable {

            // YYЭ��uri
            enum{uri = (162 << 8) | protocol::MAKINGFRIEND_APP_SVID};

			//��ɾ���ߵ�uid
			uint32_t uid;

            // ɾ����ϵ�˵Ĳ����Ƿ�ɹ�
            uint32_t operationResult;

            /* Marshallable�ӿ�ʵ�� */

            virtual void marshal(sox::Pack &pack) const
            {
                pack << uid << operationResult;
            }

            virtual void unmarshal(const sox::Unpack &unpack)
            {
                unpack >> uid >> operationResult;
            }
        };

		//��ĳ�˹�ע��֪ͨЭ��
		struct PFansAdded: public sox::Marshallable {

			// YYЭ��uri
			enum{uri = (163 << 8) | protocol::MAKINGFRIEND_APP_SVID};

			//����ߵ�uid
			uint32_t uid;

			//������ߵ�uid;
			uint32_t uPeerId;

			/* Marshallable�ӿ�ʵ�� */
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
