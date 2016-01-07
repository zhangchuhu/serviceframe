#pragma once

#include "ImHashServer.h"

#include "common/boost/thread/mutex.hpp"

namespace server
{
	namespace im
	{

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//通用服务器哈希帮助类

		//多线程版本

		class CRHashServerValueHelper : public CHashServerValueHelper
		{
		public:

			virtual ~CRHashServerValueHelper() {};

			virtual bool AddServer(uint32_t uServerValue);

			virtual bool RemoveServer(uint32_t uServerValue);

			virtual void ClearServer();

			//根据因子，从集合中挑出一个Value值，如果集合为空,返回 NONE_SERVER_ID

			virtual uint32_t GetServerValueByFactor(uint32_t uFactor);

			//检查服务器Value在集合中是否存在

			virtual bool IsServerValueExist(uint32_t uServerValue);

			//取全部服务器ID集合

			virtual void GetServerValueSet(std::set<uint32_t> &setServerValue);

		private:
			boost::mutex m_mutexValue;
		};

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		//服务器哈希类型

		//多线程版本

		class CRBaseDaemonClientW : public CTBaseDaemonClientW<CRHashServerValueHelper>
		{
			boost::mutex m_mutexServers;

		protected:

			virtual void ClearMapServers()
			{
				boost::mutex::scoped_lock sl(m_mutexServers);

				CTBaseDaemonClientW<CRHashServerValueHelper>::ClearMapServers();
			}

			virtual void AddMapServer(const sdaemon::FavorStrSV & fs)
			{
				boost::mutex::scoped_lock sl(m_mutexServers);

				CTBaseDaemonClientW<CRHashServerValueHelper>::AddMapServer(fs);
			}

			virtual void RemoveMapServer(const sdaemon::FavorStrSV & fs)
			{
				boost::mutex::scoped_lock sl(m_mutexServers);

				CTBaseDaemonClientW<CRHashServerValueHelper>::RemoveMapServer(fs);
			}

			virtual uint32_t GetServerIdByValue(uint32_t uServerValue, const std::string &strServerName, EHashServerValueType emHashType)
			{
				boost::mutex::scoped_lock sl(m_mutexServers);

				return CTBaseDaemonClientW<CRHashServerValueHelper>::GetServerIdByValue(uServerValue, strServerName, emHashType);
			}
		};

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//gtransfer服务器哈希帮助类
		//供其它服务器给gtransfer发包使用

		//多线程版本

		//typedef CTGTransDaemonClientW<CRBaseDaemonClientW>  CRGTransDaemonClientW;

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	}
}
