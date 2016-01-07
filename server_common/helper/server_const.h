#ifndef __SERVER_CONST_H 
#define __SERVER_CONST_H

#define SERVER_MONITOR_PORTS_DIR "/data/yy/run/console/ports/"


//0xff ffffff chunk_id_t 前面八位做一个空间

enum CHUNK_ID_SCOPE
{
	 CIS_SESSION = 1
};

enum EMSFlag{MASTER, SLAVER};


#define NONE_IMLINKD -1

namespace server
{

namespace pip
{
	const uint32_t SUB_SID_GRANULARITY = 20;
	const uint32_t SID_GRANULARITY = SUB_SID_GRANULARITY + 1;
}

const uint32_t MIN_PIP_SID = 3000000000UL;	// 30亿
const uint32_t MAX_PIP_SID = 3100000000UL;	// 31亿

const uint32_t MIN_PIP_UID = 3000000000UL;	// 30亿
const uint32_t MAX_PIP_UID = 3100000000UL;	// 31亿

const uint32_t MIN_IM_P2P_SID = 3100000000UL; // exclude
const uint32_t MAX_IM_P2P_SID = 3200000000UL; // exclude

const uint32_t MIN_MAFIA_SID = 3200000000UL; // exclude
const uint32_t MAX_MAFIA_SID = 3300000000UL; // exclude

inline bool isPIPSid(uint32_t sid)
{
	return (sid >= MIN_PIP_SID) && (sid <= MAX_PIP_SID);
}

inline bool isIMP2PSid(uint32_t sid) {
	return ( sid > MIN_IM_P2P_SID ) && ( sid < MAX_IM_P2P_SID );
}

inline bool isMafiaSid(uint32_t sid) {	// 杀人游戏
	return ( sid > MIN_MAFIA_SID ) && ( sid < MAX_MAFIA_SID );
}

inline bool isPIPUid(uint32_t uid)
{
    return (uid >= MIN_PIP_UID) && (uid <= MAX_PIP_UID);
}

#define IS_PIP_SID__LOG_AND_RETURN_VOID(pip_sid, uri) \
	if (isPIPSid(pip_sid)) \
	{\
		log(Info, "!!! pip request, sid=%u, uri=%u", pip_sid, uri);\
		return ;\
	}

#define IS_PIP_UID__LOG_AND_RETURN_VOID(pip_uid, uri) \
	if (isPIPUid(pip_uid)) \
	{\
		log(Info, "!!! pip request, uid=%u, uri=%u", pip_uid, uri);\
		return ;\
	}

// yy client type
const char* const STR_YY_CLIENT = "yy";
const char* const STR_YY_CLIENT_PIP = "yypip";
const char* const STR_YY_CLIENT_MS60 = "yyms60";

}

#endif // __SERVER_CONST_H

