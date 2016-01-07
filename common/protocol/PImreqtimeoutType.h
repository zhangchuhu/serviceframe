#ifndef PIMREQTIMEOUTTYPE_H
#define PIMREQTIMEOUTTYPE_H

namespace protocol
{
	namespace imreqtimeout
	{
		/****************************************************
		 *					Type Define						*
		 ****************************************************/
		#define KPI_GRP_BASEINFO_TIMEOUT		(8*1000) //8s
		#define KPI_GRP_MEMINFO_TIMEOUT_DEF		(5*1000) //5s
		#define KPI_GRP_MEMINFO_TIMEOUT_HUGE	(60*1000) //60s
		
		enum E_KPI_EVENT_TYPE
		{
			KPI_TIMEOUT_ImLogin = 0,	//im login
			KPI_TIMEOUT_GetImBuddys,	//get im buddys
			KPI_TIMEOUT_GetGroups,		//get groups/folders/simple info
			KPI_TIMEOUT_SendImMsg,		//send im msg
			KPI_TIMEOUT_SendGMsg,		//send group msg
			KPI_TIMEOUT_GetGroupMsgs,	//get unread group msg
			KPI_TIMEOUT_GetGMemberInfo,	//get gmember list/member info/member online info
			KPI_TIMEOUT_GetImOldMsgs,	//get im history msgs
			KPI_TIMEOUT_GetGOldMsgs,	//get group history msgs
			KPI_FAILURE_SendFile,		//send file failure
			KPI_FAILURE_ImVoice,		//imvoice failure
			KPI_FAILURE_Image			//upload or download image failure
		};

		struct SFailItem : public sox::Marshallable
		{
			E_KPI_EVENT_TYPE type;
			uint32_t time;
			std::string data;

			
			virtual void marshal(sox::Pack &p) const
			{
				p.push_uint8(type);
				p << time << data; 
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				type = (E_KPI_EVENT_TYPE)p.pop_uint8();
				p >> time >> data; 
			}
		};

	}
}
#endif

