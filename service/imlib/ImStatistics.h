#pragma once
#include <map>
//#include "boost/thread/tss.hpp"
#include "boost/thread/mutex.hpp"
#include "common/core/ibase.h"
#include "server_common/server-lib/RouteTarget.h"
#include "server_common/server-lib/AutoTimer.h"
#include "server_common/protocol/PImStatistics.h"
#include <boost/thread/thread.hpp>
#include <boost/thread/tss.hpp>


#define CONFIG_IMSTATISTICS_INIT \
    CImStatistics __imstat;\
    __imstat.setWriter(&__writer);\
 
#define CONFIG_ASYNCIMSTATISTICS_INIT \
    CRImStatistics __imstat;\
    __imstat.setWriter(&__writer);\

namespace server
{
namespace imstatistics
{
	typedef boost::mutex::scoped_lock scoped_lock;

	enum StatIdSection
	{
		MAX_STATIDBYTIME = 9999,
		MAX_STATIDBYDAY = 19999
	};

	enum StatIdByTime
	{
		IMMAKEUP2_1 = 0x0, //tbl_im_immakeup2: imchat2 -> immakeup -> imlink 的聊天回应补偿， 这点反映缓存错误为离线
		IMMAKEUP2_2,       //tbl_im_immakeup2: imchat2 -> imlink -> immakeup ，这点反映缓存错误为在线
		IMMAKEUP2_3,       //tbl_im_immakeup2: imofflinemsg -> imlink -> immakeup 这点反映DB错误为在线
		IMMAKEUP3_1,       //tbl_im_immakeup3: immakeup拒绝的聊天回应派发
		IMMAKEUP3_2,       //tbl_im_immakeup3: immakeup拒绝的其它聊天派发
		IMOFFLINEMSG2_1,   //tbl_im_offlinemsg2: protocol -> imofflinemsg -> imlink 离线聊天补偿，这点反映protocol错误为离线
		IMOFFLINEMSG2_2,   //tbl_im_offlinemsg2: imchat -> imofflinemsg -> imlinkd 在线聊天补偿，这点反映缓存错误为在线
		IMOFFLINEMSG2_3    //tbl_im_offlinemsg2: imchat -> imofflinemsg -> db 在线聊天补偿，这点反映protocol错误为在线
	};

	enum StatIdByDay
	{
		IMCHAT_ONLINE_SEND = MAX_STATIDBYTIME + 1, //tbl_im_imchat: n_online_send
		IMCHAT_ONLINE_SEND_RECV,      //tbl_im_imchat: n_online_send_recv
		IMLIST_SEARCH,                //tbl_im_imlist: n_search
		IMLIST_CHANEL_ADD_BUDDY,      //tbl_im_imlist: n_chanel_add_buddy
		IMLIST_SEARCH_ADD_BUDDY,      //tbl_im_imlist: n_search_add_buddy
		IMMAKEUP_ONLINE_SEND,         //tbl_im_immakeup: n_online_send
		IMMAKEUP_ONLINE_SEND_RECV,    //tbl_im_immakeup: n_online_send_recv
		IMOFFLINEMSG_OFFLINEMSG,      //tbl_im_imofflinemsg: n_offlinemsg
		IMOFFLINEMSG_ONLINE_SEND,     //tbl_im_imofflinemsg: n_online_send
		IMTTLZERO_COUNT               //tbl_im_ttl_zero: n_zero_count
	};

	enum StatIdwithNum
	{
		IMONLINE_USER_COUNT = MAX_STATIDBYDAY + 1  //tbl_im_online: n_user_count
	};
	
	class ThreadStatWriter
	{
	private:
		mutable boost::mutex m_mutex;

    	//key:statid, value:number
    	typedef std::map<uint32_t, uint32_t> MapStatIdNum_T;
    	MapStatIdNum_T m_mapStatIdNum;
	public:
        void RouteStat(uint32_t uStatId);
        void RouteStat(uint32_t uStatId, uint32_t uNum);
		void Flush(std::vector<SStatData>& vecByTime, 
										std::vector<SStatData>& vecByDay, 
										std::vector<SStatData>& vecWithNum);
	};

	class CRImStatistics:
		public core::PHClass,
		public core::IRouteTarget
	{
	public:
        CRImStatistics();
        void RouteStat(uint32_t uStatId);        
        void RouteStat(uint32_t uStatId, uint32_t uNum);        
		void SendStatData();		
		ThreadStatWriter* getThreadStatWriter();
	private:
 
    	CAutoTimer m_sendTimer;

		boost::thread_specific_ptr<ThreadStatWriter> ptr;
    };

    class CRImStatisticsAware
	{
	private:
		CRImStatistics *pImStat;
	public:
		virtual ~CRImStatisticsAware(){}
		virtual void setImStat(CRImStatistics *c){
			pImStat = c;
		}
		CRImStatistics *getImStat(){
			return pImStat;
		}
    };
    
	class CImStatistics:
		public core::PHClass,
		public core::IRouteTarget
	{
	public:
        CImStatistics();
        void RouteStat(uint32_t uStatId);        
        void RouteStat(uint32_t uStatId, uint32_t uNum);        
		void SendStatData();
	private:
    	//key:statid, value:number
    	typedef std::map<uint32_t, uint32_t> MapStatIdNum_T;
    	MapStatIdNum_T m_mapStatIdNum;
 
    	CAutoTimer m_sendTimer;

    	//boost::mutex m_MutexStatData;
    };

    class CImStatisticsAware
	{
	private:
		CImStatistics *pImStat;
	public:
		virtual ~CImStatisticsAware(){}
		virtual void setImStat(CImStatistics *c){
			pImStat = c;
		}
		CImStatistics *getImStat(){
			return pImStat;
		}
    };  
}
}
