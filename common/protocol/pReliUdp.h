#ifndef __P_RELIUDP_H__
#define __P_RELIUDP_H__

#include <string>
#include "common/packet.h"
#include "common/core/base_svid.h"
#include "common/protocol/const.h"

namespace protocol
{
	namespace reliudp
	{
		enum ControlMsgType
		{
			RUDP_CONN_REQ  = 1,
			RUDP_CONN_RES = 2,
			RUDP_ACK = 3,
			RUDP_NACK = 4
		};

		struct PRUdpData : public sox::Marshallable {
			enum {uri = (800 << 8) | protocol::SERVICE_SVID};
			uint32_t seqNum;
			std::string streamData;
			PRUdpData(): seqNum(0), streamData()
			{
			}
			virtual void marshal(sox::Pack &pk) const {
				pk << seqNum << streamData;
			}
			virtual void unmarshal(const sox::Unpack &up) {
				up >> seqNum >> streamData;
			}
		};

		struct PRUdpControl : public sox::Marshallable {
			enum {uri = (801 << 8) | protocol::SERVICE_SVID};
			uint16_t msgType;
			std::string controlMsg;
			PRUdpControl():
			msgType(0),
				controlMsg()
			{
			}
			virtual void marshal(sox::Pack &pk) const {
				pk << msgType << controlMsg;
			}
			virtual void unmarshal(const sox::Unpack &up) {
				up >> msgType >> controlMsg;
			}
		};

		/// RUDP_CONN_REQ
		struct PRUdpConnReq : public sox::Marshallable {
			uint32_t initSeqNum; // set to be a random number.
			uint16_t maxPayloadSize;  // in Byte, smaller than MTU size.
			uint16_t senderWindow; 
			uint16_t receiverWindow; 
			uint32_t retransitTimer; // ms
			uint32_t ackTimer;   // ms
			uint32_t heartbeatTimer; // ms
			uint32_t dropConnTimer; // ms, 
			PRUdpConnReq():
			initSeqNum(0),
				maxPayloadSize(500),
				senderWindow(500),
				receiverWindow(500),
				retransitTimer(2000),
				ackTimer(500),
				heartbeatTimer(10000),
				dropConnTimer(30000)
			{
			}
			virtual void marshal(sox::Pack &pk) const {
				pk << initSeqNum << maxPayloadSize << senderWindow << receiverWindow 
					<< retransitTimer << ackTimer<< heartbeatTimer << dropConnTimer;	
			}
			virtual void unmarshal(const sox::Unpack &up) {	
				up >>initSeqNum>>  maxPayloadSize >> senderWindow >> receiverWindow
					>> retransitTimer >> ackTimer >> heartbeatTimer >> dropConnTimer;	
			}
		};


		/// RUDP_CONN_RES
		struct PRUdpConnRes : public PRUdpConnReq{
		};

		/// RUDP_ACK
		struct PRUdpAck : public sox::Marshallable {
			uint32_t seqNum;
			PRUdpAck():
			seqNum(0)	{
			}
			virtual void marshal(sox::Pack &pk) const {
				pk << seqNum;	
			}
			virtual void unmarshal(const sox::Unpack &up) {	
				up >> seqNum;	
			}
		};

		/// RUDP_NACK
		struct PRUdpNack : public sox::Marshallable {
			uint32_t lowerBoundSeq;
			uint32_t upperBoundSeq;
			PRUdpNack():
			lowerBoundSeq(0),
				upperBoundSeq(0)
			{
			}
			virtual void marshal(sox::Pack &pk) const {
				pk << lowerBoundSeq << upperBoundSeq;	
			}
			virtual void unmarshal(const sox::Unpack &up) {	
				up >> lowerBoundSeq >> upperBoundSeq;	
			}
		};
	}
}

#endif
