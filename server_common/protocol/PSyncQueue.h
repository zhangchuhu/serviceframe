#ifndef PSYNCQUEUE_H_20110314
#define PSYNCQUEUE_H_20110314
#include "common/res_code.h"
#include "common/core/base_svid.h"
#include "packet.h"
#include "common/protocol/zip_tpl.h"
#include "server_common/sq_client/Common.h"
#include <string>
#include <map>

namespace server{ namespace sync_queue {

	struct PMsgBase : public sox::Marshallable
	{
		uint64_t u64SeqId;
		uint32_t u32SQSId;
		std::map<std::string, std::string> mapMsg;
		std::set<uint32_t> setSubIds;

		PMsgBase() : u64SeqId(INVALID_SEQUENCE_ID), u32SQSId(INVALID_SERVICE_ID) {}

		virtual void marshal(sox::Pack& p) const{
			p << u64SeqId << u32SQSId;
			p.push_uint32(mapMsg.size());
			for (std::map<std::string, std::string>::const_iterator it = mapMsg.begin(); it != mapMsg.end(); ++it)
			{
				std::string strFirst = it->first;
				std::string strSecond = it->second;
				p << strFirst << strSecond;
			}

			p << setSubIds;
		}

		virtual void unmarshal(const sox::Unpack& p) {
			p >> u64SeqId >> u32SQSId;
			for (uint32_t i = p.pop_uint32(); i > 0; --i)
			{
				std::string strKey;
				std::string strValue;
				p >> strKey >>strValue;
				mapMsg[strKey]  = strValue;
			}

			p >> setSubIds;
		}
	};

	struct PKVMsg: public sox::Marshallable
	{
        std::map<std::string, std::string> mapMsg;

		PKVMsg() {}
		explicit PKVMsg(const std::map<std::string, std::string>& msg) : mapMsg(msg) {}

        virtual void marshal(sox::Pack& p) const
        {
            p.push_uint32(mapMsg.size());
            for (std::map<std::string, std::string>::const_iterator it = mapMsg.begin(); it != mapMsg.end(); ++it)
            {
                p << it->first;
                p.push_varstr32(it->second.data(), it->second.size());
            }
        }

        virtual void unmarshal(const sox::Unpack& p)
        {
            mapMsg.clear();

            for (uint32_t i = p.pop_uint32(); i > 0; --i)
            {
                std::string strK;
                std::string strV;
                p >> strK;
                strV = p.pop_varstr32();
                mapMsg[strK] = strV;
            }
        }
	};

	static const char* const CMD_SYNC_IN_FROM_THIS_REGION = "sim";
	static const char* const CMD_SYNC_IN_FROM_OTHER_REGION = "sio";
	static const char* const CMD_SYNC_IN_DEL = "sid";

	static const char* const CMD_SYNC_OUT_SET = "sos";
	static const char* const CMD_SYNC_OUT_DEL = "sod";

	struct PKTVMsg : public PKVMsg
	{
        uint64_t u64Timestamp;
        std::string strCmd;
        std::set<std::string> setRegionToNotify;

        PKTVMsg() : PKVMsg(), u64Timestamp(0)
        {}

        virtual void marshal(sox::Pack& p) const
        {
            p << u64Timestamp << strCmd;
            marshal_container(p, setRegionToNotify);

            PKVMsg::marshal(p);
        }

        virtual void unmarshal(const sox::Unpack& p)
        {
            p >> u64Timestamp >> strCmd;
            unmarshal_container(p, std::inserter(setRegionToNotify, setRegionToNotify.end()));

            PKVMsg::unmarshal(p);
        }
	};

	struct PSQMsg : public sox::Marshallable
	{
		std::map<std::string, std::string> mapMsg;

		virtual void marshal(sox::Pack& p) const{
			p.push_uint32(mapMsg.size());
			for (std::map<std::string, std::string>::const_iterator it = mapMsg.begin(); it != mapMsg.end(); ++it)
				p << it->first << it->second;
		}

		virtual void unmarshal(const sox::Unpack& p) {
			mapMsg.clear();
			std::string strFirst;
			std::string strSecond;
			for (uint32_t i = p.pop_uint32(); i > 0; --i)
			{				
				p >> strFirst >> strSecond;
				mapMsg[strFirst] = strSecond;
			}
		}
	};

	struct PSQPublish : public PSQMsg
	{
		enum {uri = (1 << 8) | ::protocol::SYNC_QUEUE_SVID};

		std::string strReserved;

		virtual void marshal(sox::Pack& p) const{
			PSQMsg::marshal(p);
			p << strReserved;
		}

		virtual void unmarshal(const sox::Unpack& p) {
			PSQMsg::unmarshal(p);
			p >> strReserved;
		}
	};

	struct PSQNotify : public PSQMsg
	{
		enum {uri = (3 << 8) | ::protocol::SYNC_QUEUE_SVID};
	};

	struct PSQDelFromSubList : public sox::Marshallable
	{
		enum {uri = (4 << 8) | ::protocol::SYNC_QUEUE_SVID};

		uint32_t u32OperatorId;	// �������u32OperatorId = u32SubId
		uint32_t u32SubId;

		uint32_t u32SQId;		// MsgId: SQId
		uint64_t u64SeqId;		// MsgId: SeqId


		PSQDelFromSubList() 
			: u32OperatorId(INVALID_SERVICE_ID), u32SubId(INVALID_SERVICE_ID)
			, u32SQId(INVALID_SERVICE_ID), u64SeqId(INVALID_SEQUENCE_ID)
		{}

		virtual void marshal(sox::Pack& p) const{
			p << u32OperatorId << u32SubId << u64SeqId << u32SQId;
		}

		virtual void unmarshal(const sox::Unpack& p) {
			p >> u32OperatorId >> u32SubId >> u64SeqId >> u32SQId;
		}
	};


	struct PSQPublishRes : public sox::Marshallable
	{
		enum {uri = (6 << 8) | ::protocol::SYNC_QUEUE_SVID};

		uint32_t	u32SQId;
		uint64_t	u64SeqId;
		int32_t	i32ResCode;
		std::string strReserved;	// store fsm id

		/**
		resCode
		0		success
		-1		unknown error
		-2		no authorization(SQCId can't be found in global sq_conf
		-3		SQ blocks publish
		-4		msg size > sq msg size
		-5		pub container for SQC is full
		-6		SQC is blocked by SQ
		-7		no SQCId
		*/
		PSQPublishRes() : u32SQId(INVALID_SERVICE_ID), u64SeqId(INVALID_SEQUENCE_ID), i32ResCode(-1) 
		{}

		virtual void marshal(sox::Pack& p) const{
			uint32_t u32Tmp = uint32_t(i32ResCode);
			p << u32SQId << u64SeqId << u32Tmp << strReserved;
		}

		virtual void unmarshal(const sox::Unpack& p) {
			uint32_t u32Tmp = (uint32_t)-1;
			p >> u32SQId >> u64SeqId >> u32Tmp >> strReserved;
			i32ResCode = (int32_t)u32Tmp;
		}
	};

	struct PGetGlobalConf : public sox::Marshallable
	{
		enum {uri = (11 << 8) | ::protocol::SYNC_QUEUE_SVID};

		uint32_t u32SQId;
		std::string strGlobalConfMd5Ver;
		PGetGlobalConf() : u32SQId(INVALID_SERVICE_ID) {}

		virtual void marshal(sox::Pack& p) const{
			p << u32SQId << strGlobalConfMd5Ver;
		}

		virtual void unmarshal(const sox::Unpack& p) {
			p >> u32SQId >> strGlobalConfMd5Ver;
		}
	};

	struct PGetGlobalConfRes : public sox::Marshallable
	{
		enum {uri = (10 << 8) | ::protocol::SYNC_QUEUE_SVID};

		std::string strGlobalConf;
		std::string strGlobalConfMd5Ver;

		virtual void marshal(sox::Pack& p) const{
			// may be longer than 65535 bytes
			p.push_varstr32(strGlobalConf.c_str(), strGlobalConf.length());
			p << strGlobalConfMd5Ver;
		}

		virtual void unmarshal(const sox::Unpack& p) {
			strGlobalConf = p.pop_varstr32();
			p >> strGlobalConfMd5Ver;
		}
	};

	template <typename K, typename V>
	inline void marshal_map_container(sox::Pack & p, const std::map<K, V>& c)
	{
		p.push_uint32(uint32_t(c.size())); // use uint32 ...
		for (typename std::map<K, V>::const_iterator i = c.begin(); i != c.end(); ++i)
			p << i->first << i->second;
	}

	template <typename K, typename V>
	inline void unmarshal_map_container(const sox::Unpack & p, std::map<K, V>& c)
	{
		for (uint32_t count = p.pop_uint32(); count > 0; --count)
		{
			K k;
			V v;
			p >> k >> v;
			c[k] = v;
		}
	}

	// TODO: �����ͳ��Э����չ��
	struct SQStatistic : public sox::Marshallable
	{
		uint32_t m_u32SQId;
		std::map<uint32_t, uint64_t> m_mapSQCIdToPubPerSec;
		std::map<uint32_t, uint64_t> m_mapSQCIdToSubMsgCntPerSec;
		uint64_t m_u64PubPerSec;
		uint64_t m_u64NotifyPerSec;
		uint64_t m_u64CurMsgCnt;
		uint64_t m_u64CurImmigrationMsgCnt;
		uint64_t m_u64FreeMsgCnt;
		uint64_t m_u64CurMinSeqIdToAlloc;
		uint64_t m_u64CurMsgMinSeqId;
		uint64_t m_u64MaxNotifyTime;	// ms
		uint64_t m_u64AvgNotifyTime;	// ms
		std::set<uint32_t> m_setBlockSQCId;

		std::map<std::pair<uint32_t, uint32_t>, uint32_t> m_mapSpecialRoutePrxySQId;
		std::map<std::pair<uint32_t, uint32_t>, std::string> m_mapSpecialRouteIsp;

		std::map<uint32_t, std::map<std::string, std::string> > m_mapSQCStatistic;
		std::map<uint32_t, std::map<uint32_t, uint64_t> > m_mapSQCIpTime;
		std::set<uint32_t> m_setOnlineSQCId;

		SQStatistic() 
			: m_u32SQId(INVALID_SERVICE_ID)
			, m_u64PubPerSec(0)
			, m_u64NotifyPerSec(0)
			, m_u64CurMsgCnt(0)
			, m_u64CurImmigrationMsgCnt(0)
			, m_u64FreeMsgCnt(0)
			, m_u64CurMinSeqIdToAlloc(0)
			, m_u64CurMsgMinSeqId(0)
		{}

		virtual void marshal(sox::Pack& p) const{
			marshal_map_container(p, m_mapSQCIdToPubPerSec);
			marshal_map_container(p, m_mapSQCIdToSubMsgCntPerSec);
			p << m_u32SQId << m_u64PubPerSec << m_u64NotifyPerSec 
				<< m_u64CurMsgCnt << m_u64CurImmigrationMsgCnt
				<< m_u64FreeMsgCnt << m_u64CurMinSeqIdToAlloc 
				<< m_u64CurMsgMinSeqId << m_u64MaxNotifyTime << m_u64AvgNotifyTime;

			marshal_container(p, m_setBlockSQCId);

			p.push_uint32(m_mapSpecialRoutePrxySQId.size());
			for (std::map<std::pair<uint32_t, uint32_t>, uint32_t>::const_iterator it = m_mapSpecialRoutePrxySQId.begin();
				it != m_mapSpecialRoutePrxySQId.end(); ++it)
			{
				p << it->first.first << it->first.second << it->second;
			}

			p.push_uint32(m_mapSpecialRouteIsp.size());
			for (std::map<std::pair<uint32_t, uint32_t>, std::string>::const_iterator it = m_mapSpecialRouteIsp.begin();
				it != m_mapSpecialRouteIsp.end(); ++it)
			{
				p << it->first.first << it->first.second << it->second;
			}

			p.push_uint32(m_mapSQCStatistic.size());
			for (std::map<uint32_t, std::map<std::string, std::string> >::const_iterator it = m_mapSQCStatistic.begin();
				it != m_mapSQCStatistic.end(); ++it)
			{
				p << it->first;
				p.push_uint32(it->second.size());
				for (std::map<std::string, std::string>::const_iterator itSecond = it->second.begin(); 
					itSecond != it->second.end(); ++itSecond)
				{
					p << itSecond->first << itSecond->second;
				}
			}

			p.push_uint32(m_mapSQCIpTime.size());
			for (std::map<uint32_t, std::map<uint32_t, uint64_t> >::const_iterator it = m_mapSQCIpTime.begin(); 
				it != m_mapSQCIpTime.end(); ++it)
			{
				p << it->first;
				p.push_uint32(it->second.size());
				for (std::map<uint32_t, uint64_t>::const_iterator itSecond = it->second.begin(); 
					itSecond != it->second.end(); ++itSecond)
				{
					p << itSecond->first << itSecond->second;
				}
			}

			p.push_uint32(m_setOnlineSQCId.size());
			for (std::set<uint32_t>::iterator it = m_setOnlineSQCId.begin(); it != m_setOnlineSQCId.end(); ++it)
			{
				p << *it;
			}
		}

		virtual void unmarshal(const sox::Unpack& p) {
			unmarshal_map_container(p, m_mapSQCIdToPubPerSec);
			unmarshal_map_container(p, m_mapSQCIdToSubMsgCntPerSec);
			p >> m_u32SQId >> m_u64PubPerSec >> m_u64NotifyPerSec 
				>> m_u64CurMsgCnt >> m_u64CurImmigrationMsgCnt
				>> m_u64FreeMsgCnt >> m_u64CurMinSeqIdToAlloc 
				>> m_u64CurMsgMinSeqId >> m_u64MaxNotifyTime >> m_u64AvgNotifyTime;

			unmarshal_container(p, std::inserter(m_setBlockSQCId, m_setBlockSQCId.begin()));

				
			for (uint32_t i = p.pop_uint32(); i > 0; --i)
			{
				uint32_t u32FromSQId = INVALID_SERVICE_ID;
				uint32_t u32ToSQId = INVALID_SERVICE_ID;
				uint32_t u32PrxySQId = INVALID_SERVICE_ID;
				p >> u32FromSQId >> u32ToSQId >> u32PrxySQId;
				m_mapSpecialRoutePrxySQId[std::make_pair(u32FromSQId, u32ToSQId)] = u32PrxySQId;
			}

			for (uint32_t i = p.pop_uint32(); i > 0; --i)
			{
				uint32_t u32FromSQId = INVALID_SERVICE_ID;
				uint32_t u32ToSQId = INVALID_SERVICE_ID;
				std::string strIsp;
				p >> u32FromSQId >> u32ToSQId >> strIsp;
				m_mapSpecialRouteIsp[std::make_pair(u32FromSQId, u32ToSQId)] = strIsp;
			}


			if (p.size() < 4)
				return ;

			for (uint32_t i = p.pop_uint32(); i > 0; --i)
			{
				uint32_t u32SQCId = INVALID_SERVICE_ID;
				p >> u32SQCId;
				for (uint32_t j = p.pop_uint32(); j > 0; --j)
				{
					std::string key;
					std::string value;
					p >> key >> value;
					m_mapSQCStatistic[u32SQCId][key] = value;
				}
			}

			for (uint32_t i = p.pop_uint32(); i > 0; --i)
			{
				uint32_t u32SQCId = INVALID_SERVICE_ID;
				p >> u32SQCId;
				for (uint32_t j = p.pop_uint32(); j > 0; --j)
				{
					uint32_t u32Ip;
					uint64_t u64Time;
					p >> u32Ip >> u64Time;
					m_mapSQCIpTime[u32SQCId][u32Ip] = u64Time;
				}
			}

			for (uint32_t i = p.pop_uint32(); i > 0; --i)
			{
				uint32_t u32SQCId = INVALID_SERVICE_ID;
				p >> u32SQCId;
				m_setOnlineSQCId.insert(u32SQCId);
			}
		}
	};

	struct PSQSysOp : public sox::Marshallable
	{
		enum {uri = (20 << 8) | ::protocol::SYNC_QUEUE_SVID};

		uint64_t u64FsmId;
		std::string strBuffer;

		PSQSysOp() : u64FsmId(0) {}

		virtual void marshal(sox::Pack& p) const{
			p << u64FsmId;
			p.push_varstr32(strBuffer.c_str(), strBuffer.size());

		}

		virtual void unmarshal(const sox::Unpack& p) {
			p >> u64FsmId;
			strBuffer = p.pop_varstr32();
		}
	};

	struct PSQSysOpRes : public sox::Marshallable
	{
		enum {uri = (21 << 8) | ::protocol::SYNC_QUEUE_SVID};

		uint32_t u32SQId;
		uint64_t u64FsmId;
		std::string strBuffer;
		int32_t i32ResCode;	// 0 success


		PSQSysOpRes() : u32SQId(INVALID_SERVICE_ID), u64FsmId(0), i32ResCode(-1) {}

		virtual void marshal(sox::Pack& p) const{
			p << u32SQId << u64FsmId;
			p.push_varstr32(strBuffer.c_str(), strBuffer.size());
			p << (uint32_t) i32ResCode;
		}

		virtual void unmarshal(const sox::Unpack& p) {
			p >> u32SQId >> u64FsmId;
			strBuffer = p.pop_varstr32();


			uint32_t u32Tmp = 0;
			p >> u32Tmp;
			i32ResCode = (int32_t)u32Tmp;
		}
	};


	struct PSyncBindingSQ : public sox::Marshallable
	{
		enum {uri = (22 << 8) | ::protocol::SYNC_QUEUE_SVID};

		uint32_t u32SQCId;
		std::string strGlobalConfMd5Ver;

		PSyncBindingSQ() : u32SQCId(INVALID_SERVICE_ID) {}

		virtual void marshal(sox::Pack& p) const{
			p << u32SQCId;
			p.push_varstr32(strGlobalConfMd5Ver.c_str(), strGlobalConfMd5Ver.size());
		}

		virtual void unmarshal(const sox::Unpack& p) {
			p >> u32SQCId;
			strGlobalConfMd5Ver = p.pop_varstr32();
		}
	};

	

	struct PSyncBindingSQRes : public sox::Marshallable
	{
		enum {uri = (23 << 8) | ::protocol::SYNC_QUEUE_SVID};

		uint32_t u32SQCId;
		uint32_t u32SQId;
		std::vector<BindingSQInfo> vtBindingSQInfo;
		std::string strGlobalConfMd5Ver;

		PSyncBindingSQRes() : u32SQCId(INVALID_SERVICE_ID), u32SQId(INVALID_SERVICE_ID) {}

		virtual void marshal(sox::Pack& p) const{
			p << u32SQCId << u32SQId << vtBindingSQInfo;
			p.push_varstr32(strGlobalConfMd5Ver.c_str(), strGlobalConfMd5Ver.size());
		}

		virtual void unmarshal(const sox::Unpack& p) {
			p >> u32SQCId >> u32SQId >> vtBindingSQInfo;
			strGlobalConfMd5Ver = p.pop_varstr32();
		}
	};

	struct PSQPing : public sox::Marshallable
	{
		enum {uri = (24 << 8) | ::protocol::SYNC_QUEUE_SVID};

		uint32_t u32SQCId;

		PSQPing() : u32SQCId(INVALID_SERVICE_ID) {}

		virtual void marshal(sox::Pack& p) const{
			p << u32SQCId;

		}

		virtual void unmarshal(const sox::Unpack& p) {
			p >> u32SQCId;
		}
	};

	struct PSQPingRes : public sox::Marshallable
	{
		enum {uri = (25 << 8) | ::protocol::SYNC_QUEUE_SVID};

		uint32_t u32SQCId;
		uint32_t u32SQId;

		PSQPingRes() : u32SQCId(INVALID_SERVICE_ID), u32SQId(INVALID_SERVICE_ID) {}

		virtual void marshal(sox::Pack& p) const{
			p << u32SQCId << u32SQId;
		}

		virtual void unmarshal(const sox::Unpack& p) {
			p >> u32SQCId >> u32SQId;
		}
	};

	// SQ -> SQC
	struct PRouteToSQC : public sox::Marshallable
	{
		enum {uri = (28 << 8) | ::protocol::SYNC_QUEUE_SVID};

		std::set<uint32_t>	setToSQCIds;	// 
		uint32_t			u32ToSQId;		// 

		uint32_t			ruri;
		uint16_t			rresCode;
		std::string			strLoad;

		PRouteToSQC() : u32ToSQId(INVALID_SERVICE_ID), ruri(0), rresCode(RES_SUCCESS) {}

		virtual void marshal(sox::Pack& p) const{
			p << setToSQCIds;
			p << u32ToSQId << ruri << rresCode;
			p.push_varstr32(strLoad.c_str(), strLoad.size());
		}

		virtual void unmarshal(const sox::Unpack& p) {
			p >> setToSQCIds;
			p >> u32ToSQId >> ruri >> rresCode;
			strLoad = p.pop_varstr32();
		}
	};

	// SQC -> SQ
	// SQ -> SQ
	struct PRouteToSQ : public sox::Marshallable
	{
		enum {uri = (29 << 8) | ::protocol::SYNC_QUEUE_SVID};

		uint32_t			u32ToSQId;
		uint32_t			ruri;
		uint16_t			rresCode;
		std::string			strLoad;

		PRouteToSQ() : u32ToSQId(INVALID_SERVICE_ID), ruri(0), rresCode(RES_SUCCESS) {}

		virtual void marshal(sox::Pack& p) const{
			p << u32ToSQId << ruri << rresCode;
			p.push_varstr32(strLoad.c_str(), strLoad.size());
		}

		virtual void unmarshal(const sox::Unpack& p) {
			p >> u32ToSQId >> ruri >> rresCode;
			strLoad = p.pop_varstr32();
		}
	};

	struct PGetGlobalMinSeqId : public sox::Marshallable
	{
		enum {uri = (30 << 8) | ::protocol::SYNC_QUEUE_SVID};

		uint32_t	u32SQCId;
		uint64_t	u64FsmId;

		PGetGlobalMinSeqId() : u32SQCId(INVALID_SERVICE_ID), u64FsmId(0) {}

		virtual void marshal(sox::Pack& p) const{
			p << u32SQCId << u64FsmId;
		}

		virtual void unmarshal(const sox::Unpack& p) {
			p >> u32SQCId >> u64FsmId;
		}
	};

	struct PGetGlobalMinSeqIdRes : public sox::Marshallable
	{
		enum {uri = (31 << 8) | ::protocol::SYNC_QUEUE_SVID};

		uint64_t	u64FsmId;
		uint64_t	u64SeqId;
		/**
		0	success
		-1	not all SQs return their minSeqId
		*/
		int32_t		i32ResCode;
		std::string strReserved;

		PGetGlobalMinSeqIdRes()
			: u64FsmId(0), u64SeqId(INVALID_SEQUENCE_ID) 
		{}

		virtual void marshal(sox::Pack& p) const{
			p << u64FsmId << u64SeqId << (uint32_t)i32ResCode << strReserved;
		}

		virtual void unmarshal(const sox::Unpack& p) {
			p >> u64FsmId >> u64SeqId;
			uint32_t u32 = 0;
			p >> u32;
			i32ResCode = (int32_t)u32;
			p >> strReserved;
		}
	};

	struct PGetSQMinSeqId : public sox::Marshallable
	{
		enum {uri = (32 << 8) | ::protocol::SYNC_QUEUE_SVID};

		uint32_t	u32SrcSQId;
		uint64_t	u64FsmId;

		PGetSQMinSeqId() : u32SrcSQId(INVALID_SERVICE_ID), u64FsmId(0) {}

		virtual void marshal(sox::Pack& p) const{
			p << u32SrcSQId << u64FsmId;
		}

		virtual void unmarshal(const sox::Unpack& p) {
			p >> u32SrcSQId >> u64FsmId;
		}
	};

	struct PGetSQMinSeqIdRes : public sox::Marshallable
	{
		enum {uri = (33 << 8) | ::protocol::SYNC_QUEUE_SVID};

		uint32_t	u32DstSQId;
		uint64_t	u64FsmId;
		uint64_t	u64SeqId;

		PGetSQMinSeqIdRes()
			: u32DstSQId(INVALID_SERVICE_ID), u64FsmId(0), u64SeqId(INVALID_SEQUENCE_ID) 
		{}

		virtual void marshal(sox::Pack& p) const{
			p << u32DstSQId << u64FsmId << u64SeqId;
		}

		virtual void unmarshal(const sox::Unpack& p) {
			p >> u32DstSQId >> u64FsmId >> u64SeqId;
		}
	};

	struct PMsgMigration : public sox::Marshallable
	{
		enum {uri = (34 << 8) | ::protocol::SYNC_QUEUE_SVID};

		uint32_t	u32SQIdMigrateFrom;
		uint32_t	u32SQIdMigrateTo;
		std::vector<PMsgBase> vtMsgs;


		PMsgMigration() : u32SQIdMigrateFrom(INVALID_SERVICE_ID), u32SQIdMigrateTo(INVALID_SERVICE_ID) {}

		virtual void marshal(sox::Pack& p) const{
			p << u32SQIdMigrateFrom << u32SQIdMigrateTo;
			p.push_uint32(vtMsgs.size());
			for (std::vector<PMsgBase>::const_iterator it = vtMsgs.begin(); it != vtMsgs.end(); ++it)
				p << *it;
		}

		virtual void unmarshal(const sox::Unpack& p) {
			vtMsgs.clear();
			p >> u32SQIdMigrateFrom >> u32SQIdMigrateTo;
			for (uint32_t u32MsgCount = p.pop_uint32(); u32MsgCount > 0; --u32MsgCount)
			{
				PMsgBase msg;
				p >> msg;
				vtMsgs.push_back(msg);
			}
		}

	};

	struct PMsgMigrationRes : public sox::Marshallable
	{
		enum {uri = (35 << 8) | ::protocol::SYNC_QUEUE_SVID};

		uint32_t	u32SQIdMigrateFrom;
		uint32_t	u32SQIdMigrateTo;
		std::set<MsgId> setMsgIds;

		PMsgMigrationRes() : u32SQIdMigrateFrom(INVALID_SERVICE_ID), u32SQIdMigrateTo(INVALID_SERVICE_ID) {}

		virtual void marshal(sox::Pack& p) const{
			p << u32SQIdMigrateFrom << u32SQIdMigrateTo;
			p.push_uint32(setMsgIds.size());
			for (std::set<MsgId>::iterator it = setMsgIds.begin(); it != setMsgIds.end(); ++it)
			{
				p << it->first << it->second;
			}
		}

		virtual void unmarshal(const sox::Unpack& p) {
			setMsgIds.clear();
			p >> u32SQIdMigrateFrom >> u32SQIdMigrateTo;
			for (uint32_t u32 = p.pop_uint32(); u32 > 0; --u32)
			{
				uint32_t u32SQId = INVALID_SERVICE_ID;
				uint64_t u64SeqId = INVALID_SEQUENCE_ID;
				p >> u32SQId >> u64SeqId;
				setMsgIds.insert(std::make_pair(u32SQId, u64SeqId));
			}
		}
	};

	struct PAdminReq : public sox::Marshallable
	{
		enum {uri = (36 << 8) | ::protocol::SYNC_QUEUE_SVID};

		uint64_t u64ReqId;
		std::string strCmd;

		virtual void marshal(sox::Pack& p) const{
			p << u64ReqId << strCmd;
		}

		virtual void unmarshal(const sox::Unpack& p) {
			p >> u64ReqId >> strCmd;
		}
	};


	struct PAdminReqRes : public sox::Marshallable
	{
		enum {uri = (37 << 8) | ::protocol::SYNC_QUEUE_SVID};

		uint64_t u64ReqId;
		uint32_t u32Rescode;
		std::string strResult;

		virtual void marshal(sox::Pack& p) const{
			p << u64ReqId << u32Rescode << strResult;
		}

		virtual void unmarshal(const sox::Unpack& p) {
			p >> u64ReqId >> u32Rescode >> strResult;
		}
	};

	struct PMap : public sox::Marshallable
	{
		std::map<std::string, std::string> mapInfo;

		virtual void marshal(sox::Pack& p) const{
			p.push_uint32(mapInfo.size());
			for (std::map<std::string, std::string>::const_iterator it = mapInfo.begin(); it != mapInfo.end(); ++it)
			{
				std::string strFirst = it->first;
				std::string strSecond = it->second;
				p << strFirst << strSecond;
			}
		}

		virtual void unmarshal(const sox::Unpack& p) {
			for (uint32_t i = p.pop_uint32(); i > 0; --i)
			{
				std::string strKey;
				std::string strValue;
				p >> strKey >>strValue;
				mapInfo[strKey]  = strValue;
			}
		}
	};


	struct PGetSQCStatistic : public sox::Marshallable
	{
		enum {uri = (38 << 8) | ::protocol::SYNC_QUEUE_SVID};

		uint32_t u32SQId;
		uint32_t u32SQCId;
	
		PGetSQCStatistic() : u32SQId(INVALID_SERVICE_ID), u32SQCId(INVALID_SERVICE_ID) {}

		virtual void marshal(sox::Pack& p) const{
			p << u32SQId << u32SQCId;
		}

		virtual void unmarshal(const sox::Unpack& p) {
			p >> u32SQId >> u32SQCId;
		}
	};


    const static std::string KEY_LIB_VER = "LIB_VER";
    const static std::string KEY_SQC_ID = "SQC_Id";
    const static std::string KEY_MSQ_LOG_BYTES = "MSQ_LOG_BYTES";
    const static std::string KEY_Batch_MSQ_LOG_BYTES = "Batch_MSQ_LOG_BYTES";
    const static std::string KEY_SYNC_TIME_MS = "SYNC_TIME_MS";

    const static std::string KEY_NORMAL_MSG_LATENCY_PERCENTAGE = "sqc_normal_msg_latency_percentage";
    const static std::string KEY_NORMAL_MSG_LATENCY_AVG = "sqc_normal_msg_latency_avg";
    const static std::string KEY_NORMAL_MSG_LATENCY_MAX = "sqc_normal_msg_latency_max";
    const static std::string KEY_BATCH_MSG_LATENCY_PERCENTAGE = "sqc_batch_msg_latency_percentage";
    const static std::string KEY_BATCH_MSG_LATENCY_AVG = "sqc_batch_msg_latency_avg";
    const static std::string KEY_BATCH_MSG_LATENCY_MAX = "sqc_batch_msg_latency_max";

	struct PGetSQCStatisticRes : public PMap
	{
		enum {uri = (39 << 8) | ::protocol::SYNC_QUEUE_SVID};
		

		virtual void marshal(sox::Pack& p) const{
			PMap::marshal(p);
		}

		virtual void unmarshal(const sox::Unpack& p) {
			PMap::unmarshal(p);
		}
	};


	struct PSQBranch : public sox::Marshallable {
		enum { uri = 40 << 8 | ::protocol::SYNC_QUEUE_SVID };

		uint64_t u64SeqId;
		std::map<std::string, std::string> mapMsg;

		PSQBranch() : u64SeqId(0) {}

		virtual void marshal(sox::Pack & p) const {
			p << u64SeqId;

			p.push_uint32(mapMsg.size());
			for (std::map<std::string, std::string>::const_iterator it = mapMsg.begin(); it != mapMsg.end(); ++it)
			{
				std::string strFirst = it->first;
				std::string strSecond = it->second;
				p << strFirst << strSecond;
			}
		}

		virtual void unmarshal(const sox::Unpack & p) {
			p >> u64SeqId;

			for (uint32_t i = p.pop_uint32(); i > 0; --i)
			{
				std::string strKey;
				std::string strValue;
				p >> strKey >>strValue;
				mapMsg[strKey] = strValue;
			}
		}
	};

	struct PSQBranchRes : public sox::Marshallable {
		enum { uri = 41 << 8 | ::protocol::SYNC_QUEUE_SVID };

		uint64_t u64SeqId;
		PSQBranchRes() : u64SeqId(0) {}

		virtual void marshal(sox::Pack & p) const {
			p << u64SeqId;
		}                  

		virtual void unmarshal(const sox::Unpack & p) {
			p >> u64SeqId;
		}
	};      


	struct PSQBrokerSync : public sox::Marshallable {
		enum { uri = 42 << 8 | ::protocol::SYNC_QUEUE_SVID };

		std::string strSyncFrom;
		std::string strSyncTo;
		std::vector<std::map<std::string, std::string> > vtBatchMsg;

		virtual void marshal(sox::Pack & p) const {
			p << strSyncFrom << strSyncTo;
			p.push_uint32(vtBatchMsg.size());
			for (std::vector<std::map<std::string, std::string> >::const_iterator it = vtBatchMsg.begin(); 
				it != vtBatchMsg.end(); ++it)
			{
				p.push_uint32(it->size());
				for (std::map<std::string, std::string>::const_iterator itMap = it->begin(); itMap != it->end(); ++itMap)
				{
					p << itMap->first;
					p.push_varstr32(itMap->second.data(), itMap->second.size());
				}
			}
		}                  

		virtual void unmarshal(const sox::Unpack & p) {
			p >> strSyncFrom >> strSyncTo;

			vtBatchMsg.clear();
			for (uint32_t u32Vec = p.pop_uint32(); u32Vec != 0; --u32Vec)
			{
				std::map<std::string, std::string> mapMsg;
				for (uint32_t u32Map = p.pop_uint32(); u32Map != 0; --u32Map)
				{
					std::string strK;
					std::string strV;
					p >> strK;
					strV = p.pop_varstr32();
					mapMsg[strK] = strV;
				}
				vtBatchMsg.push_back(mapMsg);
			}
		}
	};  

	struct PSQBrokerSyncResp : public sox::Marshallable {
		enum { uri = 43 << 8 | ::protocol::SYNC_QUEUE_SVID };

		std::string strSyncFrom;
		std::string strSyncTo;
		std::vector<std::pair<std::string, uint64_t> > vtBatchKT;

		virtual void marshal(sox::Pack & p) const {
			p << strSyncFrom << strSyncTo;
			p.push_uint32(vtBatchKT.size());
			for (std::vector<std::pair<std::string, uint64_t> >::const_iterator it = vtBatchKT.begin(); 
				it != vtBatchKT.end(); ++it)
			{
				p << it->first << it->second;
			}
		}                  

		virtual void unmarshal(const sox::Unpack & p) {
			p >> strSyncFrom >> strSyncTo;

			vtBatchKT.clear();
			for (uint32_t u32Vec = p.pop_uint32(); u32Vec != 0; --u32Vec)
			{
				std::string strK;
				uint64_t u64T = 0;
				p >> strK >> u64T;
				vtBatchKT.push_back(std::make_pair(strK, u64T));
			}
		}
	};

	typedef ZipMarshal<PSQBrokerSync, 44 << 8 | ::protocol::SYNC_QUEUE_SVID> PSQBrokerSyncZip;
	typedef ZipMarshal<PSQBrokerSyncResp, 45 << 8 | ::protocol::SYNC_QUEUE_SVID> PSQBrokerSyncResZip;

	struct PSQBrokerReqTicket : public sox::Marshallable
	{
        enum { uri = 46 << 8 | ::protocol::SYNC_QUEUE_SVID };

        std::string strSyncFrom;
        std::string strSyncTo;

        virtual void marshal(sox::Pack & p) const
        {
            p << strSyncFrom << strSyncTo;
        }

        virtual void unmarshal(const sox::Unpack & p)
        {
            p >> strSyncFrom >> strSyncTo;
        }
	};

	    struct PSQBrokerReqTicketResp : public sox::Marshallable {
	        enum { uri = 47 << 8 | ::protocol::SYNC_QUEUE_SVID };

	        std::string strSyncFrom;
	        std::string strSyncTo;
	        uint32_t u32TicketCnt;

	        PSQBrokerReqTicketResp() : u32TicketCnt(0) {}

	        virtual void marshal(sox::Pack & p) const
	        {
	            p << strSyncFrom << strSyncTo << u32TicketCnt;
	        }

	        virtual void unmarshal(const sox::Unpack & p)
	        {
	            p >> strSyncFrom >> strSyncTo >> u32TicketCnt;
	        }
	    };




struct PSQBrokerSyncMinUnsendTimestamp: public sox::Marshallable
{
        enum
        {
            uri = 48 << 8 | ::protocol::SYNC_QUEUE_SVID
        };

        std::string strSyncFrom;
        std::string strSyncTo;
        uint64_t u64MinSyncOut;
        uint64_t u64MinSyncIn;

        PSQBrokerSyncMinUnsendTimestamp() : u64MinSyncOut(0), u64MinSyncIn(0) {}

        virtual void marshal(sox::Pack & p) const
        {
            p << strSyncFrom << strSyncTo << u64MinSyncOut << u64MinSyncIn;
        }

        virtual void unmarshal(const sox::Unpack & p)
        {
            p >> strSyncFrom >> strSyncTo >> u64MinSyncOut >> u64MinSyncIn;
        }
};

struct IpPortTicketPS
{
        std::string strDstIp;
        uint32_t u32DstPort;
        uint32_t u32TicketPS;

        IpPortTicketPS() : u32DstPort(0), u32TicketPS(1000) {}

        IpPortTicketPS(const IpPortTicketPS& o)
            : strDstIp(o.strDstIp)
            , u32DstPort(o.u32DstPort)
            , u32TicketPS(o.u32TicketPS)
        {}

        IpPortTicketPS& operator = (const IpPortTicketPS& o)
        {
            strDstIp = o.strDstIp;
            u32DstPort = o.u32DstPort;
            u32TicketPS = o.u32TicketPS;
            return *this;
        }
};

struct PSQBrokerSyncGlobalConf: public sox::Marshallable
{
        enum
        {
            uri = 49 << 8 | ::protocol::SYNC_QUEUE_SVID
        };

        uint32_t u32MaxBatchCnt;
        uint64_t u64TimestampKeepSecond;
        uint64_t u64MinUnsendMinusSecond;
        std::map<std::string, std::map<std::string, IpPortTicketPS > > mapRule;

        // sync_out, sync_in
        std::map<std::string, std::pair<uint64_t, uint64_t> > mapRegion2MinUnsendT;

        PSQBrokerSyncGlobalConf()
            : u32MaxBatchCnt(100)
            , u64TimestampKeepSecond(1800)
            , u64MinUnsendMinusSecond(1800)
        {}

        PSQBrokerSyncGlobalConf& operator = (const PSQBrokerSyncGlobalConf& o)
        {
            u32MaxBatchCnt = o.u32MaxBatchCnt;
            u64TimestampKeepSecond = o.u64TimestampKeepSecond;
            u64MinUnsendMinusSecond = o.u64MinUnsendMinusSecond;
            mapRule = o.mapRule;
            mapRegion2MinUnsendT = o.mapRegion2MinUnsendT;
            return *this;
        }

        virtual void marshal(sox::Pack & p) const
        {
            p << u32MaxBatchCnt << u64TimestampKeepSecond << u64MinUnsendMinusSecond;

            p.push_uint32(mapRule.size());
            for (std::map<std::string, std::map<std::string, IpPortTicketPS > >::const_iterator
                    it = mapRule.begin(); it != mapRule.end(); ++it)
            {
                p.push_uint32(it->second.size());
                for (std::map<std::string, IpPortTicketPS >::const_iterator itSub =
                        it->second.begin(); itSub != it->second.end(); ++itSub)
                {
                    p << it->first << itSub->first << itSub->second.strDstIp <<
                            itSub->second.u32DstPort << itSub->second.u32TicketPS;
                }
            }


            p.push_uint32(mapRegion2MinUnsendT.size());
            for (std::map<std::string, std::pair<uint64_t, uint64_t> >::const_iterator it = mapRegion2MinUnsendT.begin();
                    it != mapRegion2MinUnsendT.end(); ++it)
            {
                p << it->first << it->second.first << it->second.second;
            }
        }

        virtual void unmarshal(const sox::Unpack & p)
        {
            mapRule.clear();
            mapRegion2MinUnsendT.clear();

            p >> u32MaxBatchCnt >> u64TimestampKeepSecond >> u64MinUnsendMinusSecond;
            for (uint32_t i = p.pop_uint32(); i != 0; --i)
            {
                std::string strSrc;
                p >> strSrc;
                for (uint32_t j = p.pop_uint32(); j != 0; --j)
                {
                    std::string strDst;
                    p >> strDst;

                    IpPortTicketPS tmp;
                    p >> tmp.strDstIp >> tmp.u32DstPort >> tmp.u32TicketPS;
                    mapRule[strSrc][strDst] = tmp;
                }
            }

            for (uint32_t i = p.pop_uint32(); i != 0; --i)
            {
                std::string strRegion;
                uint64_t u64TSyncOut = 0;
                uint64_t u64TSyncIn = 0;

                p >> strRegion >> u64TSyncOut >> u64TSyncIn;
                mapRegion2MinUnsendT[strRegion] = std::make_pair(u64TSyncOut, u64TSyncIn);
            }
        }
};

struct PSQBrokerSyncGlobalConfResp: public sox::Voidmable
{
        enum { uri = 50 << 8 | ::protocol::SYNC_QUEUE_SVID };
};

struct PSQBrokerGetGlobalConf: public sox::Voidmable
{
        enum { uri = 51 << 8 | ::protocol::SYNC_QUEUE_SVID };
};

}}

#endif	// PSYNCQUEUE_H_20110314

