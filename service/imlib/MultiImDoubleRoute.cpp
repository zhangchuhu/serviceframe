#include "MultiImDoubleRoute.h"
#include "core/sox/logger.h"

using namespace server;
using namespace std;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint32_t CRDRouteImChat2::GetMasterImChat2Id(uint32_t uSeqId, uint32_t uNetType)
{
	boost::mutex::scoped_lock s1(m_mutex);

	uint32_t uImChat2IdMaster = CDRouteImChat2::GetMasterImChat2Id(uSeqId, uNetType);

	log (Info, "[CRDRouteImChat2::GetMasterImChat2Id] nettype %ld imchat2 %ld", uNetType, uImChat2IdMaster);
	return uImChat2IdMaster;
}

uint32_t CRDRouteImChat2::GetSlaveImChat2Id(uint32_t uSeqId, uint32_t uNetType)
{
	boost::mutex::scoped_lock s1(m_mutex);

	uint32_t uImChat2IdSlave = CDRouteImChat2::GetSlaveImChat2Id(uSeqId, uNetType);

	log (Info, "[CRDRouteImChat2::GetSlaveImChat2Id] nettype %ld imchat2 %ld", uNetType, uImChat2IdSlave);
	return uImChat2IdSlave;
}

uint32_t CRDRouteImBridge::GetMasterImBridgeId(uint32_t uSeqId, uint32_t uNetType)
{
	boost::mutex::scoped_lock s1(m_mutex);

	uint32_t uImBridgeIdMaster = CDRouteImBridge::GetMasterImBridgeId(uSeqId, uNetType);

	log (Info, "[CRDRouteImBridge::GetMasterImBridgeId] nettype %ld imbridge %ld", uNetType, uImBridgeIdMaster);
	return uImBridgeIdMaster;
}

uint32_t CRDRouteImBridge::GetSlaveImBridgeId(uint32_t uSeqId, uint32_t uNetType)
{
	boost::mutex::scoped_lock s1(m_mutex);

	uint32_t uImBridgeIdSlave = CDRouteImBridge::GetSlaveImBridgeId(uSeqId, uNetType);

	log (Info, "[CRDRouteImBridge::GetSlaveImBridgeId] nettype %ld imbridge %ld", uNetType, uImBridgeIdSlave);
	return uImBridgeIdSlave;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
