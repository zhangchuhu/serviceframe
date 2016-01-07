#pragma once 

#include <boost/thread/tss.hpp>
#include <boost/thread/mutex.hpp>
#include "ImDoubleRoute.h"

namespace server
{

	//ImChat2双路选择帮助类（多线程版本）

	class CRDRouteImChat2 : public CDRouteImChat2
	{
		boost::mutex m_mutex;

	public:
		CRDRouteImChat2() {};

		virtual uint32_t GetMasterImChat2Id(uint32_t uSeqId, uint32_t uNetType = CNC | CTL);
		virtual uint32_t GetSlaveImChat2Id(uint32_t uSeqId, uint32_t uNetType = CNC | CTL);
	};

	//ImBridge双路选择帮助类（多线程版本）

	class CRDRouteImBridge : public CDRouteImBridge
	{
		boost::mutex m_mutex;

	public:
		CRDRouteImBridge() {};

		virtual uint32_t GetMasterImBridgeId(uint32_t uSeqId, uint32_t uNetType = CNC | CTL);
		virtual uint32_t GetSlaveImBridgeId(uint32_t uSeqId, uint32_t uNetType = CNC | CTL);
	};

}
