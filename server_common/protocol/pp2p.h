#ifndef PP2P_SERVER_H_
#define PP2P_SERVER_H_
#include "common/core/base_svid.h"
#include "common/packet.h"
#include "common/protocol/const.h"
#include <vector>
namespace protocol {
	namespace p2p {
		//p2pnat_d���ͷ�����ע����Ϣ��p2plbs_d/peerinfo_cached
		struct PP2pNATServerRegister: public sox::Marshallable{
			enum {uri = 64 << 8 | protocol::P2P_COMM_SVID};
			//���ͻ�ID
			uint32_t serverId;
			//���ͻ�IP
			uint32_t ip;
			//���ͻ��˿�
			uint16_t port;
			virtual void marshal(sox::Pack &pk) const {
				pk << serverId << ip << port;
			}
			virtual void unmarshal(const sox::Unpack &up) {
				up >> serverId >> ip >> port;
			}
		};
		//p2pbridge_d���ͷ�����ע����Ϣ��p2plbs_d/peerinfo_cached
		struct PP2pBridgeServerRegister: public sox::Marshallable{
			enum {uri = 65 << 8 | protocol::P2P_COMM_SVID};
			//���ͻ�ID
			uint32_t serverId;
			//���ͻ�IP
			uint32_t ip;
			//���ͻ��˿�
			uint16_t port;
			virtual void marshal(sox::Pack &pk) const {
				pk << serverId << ip << port;
			}
			virtual void unmarshal(const sox::Unpack &up) {
				up >> serverId >> ip >> port;
			}
		};

		//��peerinfo_cached������������Ϣ
		struct PP2pRequestPeerinfo : public sox::Marshallable {
			enum {uri =  66 << 8 | protocol::P2P_COMM_SVID};
			uint32_t to_ip;
			uint32_t to_port;
			uint32_t uid;
			//���ͻ�ID
			uint32_t serverId;
			
			virtual void marshal(sox::Pack &pk) const {
				pk << to_ip << to_port << uid << serverId;
			}
			virtual void unmarshal(const sox::Unpack &up) {
				up >> to_ip >> to_port >> uid >> serverId;
			}
		};
		//peerinfo_cached���ؽ��������Ϣ
		struct PP2pRequestPeerinfoRes : public sox::Marshallable {
			enum {uri =  67 << 8 | protocol::P2P_COMM_SVID};
			uint32_t to_ip;
			uint32_t to_port;
			uint32_t uid;
			uint16_t peer_type;
			uint32_t peer_ip;
			uint16_t peer_port;
			uint32_t peer_inner_ip;
			uint16_t peer_inner_port;
			uint32_t serv1_ip;
			uint16_t serv1_port;
			uint32_t serv2_ip;
			uint16_t serv2_port;

			virtual void marshal(sox::Pack &pk) const {
				pk << to_ip << to_port << uid << peer_type << peer_ip << peer_port
					<< peer_inner_ip << peer_inner_port
					<<serv1_ip << serv1_port << serv2_ip << serv2_port;
			}
			virtual void unmarshal(const sox::Unpack &up) {
				up >> to_ip >> to_port >> uid >> peer_type >> peer_ip >> peer_port
					>> peer_inner_ip >> peer_inner_port
					>>serv1_ip >> serv1_port >> serv2_ip >> serv2_port;
			}
		};
		//��peerinfo_cached���ͽ����Ϣ������Ϣ
		struct PP2pUpdatePeerinfo: public sox::Marshallable{
			enum {uri = 68 << 8 | protocol::P2P_COMM_SVID};
			uint32_t uid;
			uint16_t peer_type;
			uint32_t peer_ip;
			uint16_t peer_port;
			uint32_t peer_inner_ip;
			uint16_t peer_inner_port;
			uint32_t serv1_ip;
			uint16_t serv1_port;
			uint32_t serv2_ip;
			uint16_t serv2_port;
			//��ʶ����p2pnat_d1/p2pnat_d2
			uint16_t serv_tag;

			virtual void marshal(sox::Pack &pk) const {
				pk << uid << peer_type << peer_ip << peer_port
					<< peer_inner_ip << peer_inner_port
					<<serv1_ip << serv1_port << serv2_ip << serv2_port << serv_tag;
			}
			virtual void unmarshal(const sox::Unpack &up) {
				up >> uid >> peer_type >> peer_ip >> peer_port
					>> peer_inner_ip >> peer_inner_port
					>>serv1_ip >> serv1_port >> serv2_ip >> serv2_port >> serv_tag;
			}
		};
		//��peerinfo_cached����ɾ�������Ϣ
		struct PP2pRemovePeerinfo: public sox::Marshallable{
			enum {uri = 69 << 8 | protocol::P2P_COMM_SVID};
			uint32_t uid;

			virtual void marshal(sox::Pack &pk) const {
				pk << uid;
			}
			virtual void unmarshal(const sox::Unpack &up) {
				up >> uid;
			}
		};
		//��peerinfo_cachedͬ������
		struct PP2pPeerinfoSynchronous : public sox::Marshallable {
			enum {uri =  70 << 8 | protocol::P2P_COMM_SVID};
			std::vector<PP2pUpdatePeerinfo> peers;
			virtual void marshal(sox::Pack &pk) const {
				sox::marshal_container(pk, peers);
			}
			virtual void unmarshal(const sox::Unpack &up) {
				sox::unmarshal_container(up, std::back_inserter(peers));
			}
		};
		//ͨ��peerinfo_cachedת�����������Ϣ
		struct PP2pPeerConnRedirect : public sox::Marshallable {
			enum {uri =  71 << 8 | protocol::P2P_COMM_SVID};
			uint32_t to_uid;
			uint32_t from_uid;
			//���ͻ�ID
			uint32_t serverId;
			uint32_t serv_ip;
			uint16_t serv_port;

			virtual void marshal(sox::Pack &pk) const {
				pk << to_uid << from_uid << serverId << serv_ip << serv_port;
			}
			virtual void unmarshal(const sox::Unpack &up) {
				up >> to_uid >> from_uid >> serverId >> serv_ip >> serv_port;
			}
		};
	}
}
#endif