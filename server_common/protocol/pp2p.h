#ifndef PP2P_SERVER_H_
#define PP2P_SERVER_H_
#include "common/core/base_svid.h"
#include "common/packet.h"
#include "common/protocol/const.h"
#include <vector>
namespace protocol {
	namespace p2p {
		//p2pnat_d发送服务器注册消息到p2plbs_d/peerinfo_cached
		struct PP2pNATServerRegister: public sox::Marshallable{
			enum {uri = 64 << 8 | protocol::P2P_COMM_SVID};
			//发送机ID
			uint32_t serverId;
			//发送机IP
			uint32_t ip;
			//发送机端口
			uint16_t port;
			virtual void marshal(sox::Pack &pk) const {
				pk << serverId << ip << port;
			}
			virtual void unmarshal(const sox::Unpack &up) {
				up >> serverId >> ip >> port;
			}
		};
		//p2pbridge_d发送服务器注册消息到p2plbs_d/peerinfo_cached
		struct PP2pBridgeServerRegister: public sox::Marshallable{
			enum {uri = 65 << 8 | protocol::P2P_COMM_SVID};
			//发送机ID
			uint32_t serverId;
			//发送机IP
			uint32_t ip;
			//发送机端口
			uint16_t port;
			virtual void marshal(sox::Pack &pk) const {
				pk << serverId << ip << port;
			}
			virtual void unmarshal(const sox::Unpack &up) {
				up >> serverId >> ip >> port;
			}
		};

		//向peerinfo_cached请求结点连接信息
		struct PP2pRequestPeerinfo : public sox::Marshallable {
			enum {uri =  66 << 8 | protocol::P2P_COMM_SVID};
			uint32_t to_ip;
			uint32_t to_port;
			uint32_t uid;
			//发送机ID
			uint32_t serverId;
			
			virtual void marshal(sox::Pack &pk) const {
				pk << to_ip << to_port << uid << serverId;
			}
			virtual void unmarshal(const sox::Unpack &up) {
				up >> to_ip >> to_port >> uid >> serverId;
			}
		};
		//peerinfo_cached返回结点连接信息
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
		//向peerinfo_cached发送结点信息更新消息
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
			//标识来自p2pnat_d1/p2pnat_d2
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
		//向peerinfo_cached发送删除结点消息
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
		//与peerinfo_cached同步结点表
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
		//通过peerinfo_cached转发结点连接消息
		struct PP2pPeerConnRedirect : public sox::Marshallable {
			enum {uri =  71 << 8 | protocol::P2P_COMM_SVID};
			uint32_t to_uid;
			uint32_t from_uid;
			//发送机ID
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