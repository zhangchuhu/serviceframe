#pragma once
#include "server_common/yyac_api_lib/agent_client.h"
#include "server_common/helper/TimerHandler.h"
//======================================================================================
// alarm / warning helper class
//======================================================================================
template<uint64_t FEATURE_ID, uint64_t STRATEGY_ID>
class AlarmHelperImp// : public Singleton<AlarmHelperImp>
{
protected:
  AlarmHelperImp():m_alarmTimer(this)
  {
    m_alarmTimer.start(5*60*1000);
  }
private:
  AlarmHelperImp(const AlarmHelperImp& s);
  AlarmHelperImp& operator = (AlarmHelperImp& s);
public:
  virtual ~AlarmHelperImp(){}
  static AlarmHelperImp & Instance()
  {
    static AlarmHelperImp _inst;
    return _inst;
  }

  bool SendAlarm(uint32_t type, const std::string & msg)
  {
    return yymp::AgentClient::getInstance().SendAlarm(FEATURE_ID, STRATEGY_ID, msg.c_str());
  }
  bool SendEarlyWarning(uint32_t type, const std::string & msg)
  {
    m_alarm2Cnt[std::make_pair(type, msg)] ++;
    return true;
  }

private:
  bool alarmTimer()
  {
    for(std::map<std::pair<uint32_t, std::string>, uint64_t>::const_iterator it = m_alarm2Cnt.begin();
      it != m_alarm2Cnt.end(); ++it)
    {
      uint64_t count = it->second;
      uint32_t type = it->first.first;
      const std::string & msg = it->first.second;
      std::ostringstream alarmMsg;
      alarmMsg << "type=" << type << " " << msg << " cnt=" << count;
      bool res = yymp::AgentClient::getInstance().SendEarlyWarning(FEATURE_ID, STRATEGY_ID, alarmMsg.str().c_str());
      log(Debug, "%s %u %s", __FUNCTION__, res, alarmMsg.str().c_str());
    }
    m_alarm2Cnt.clear();
    return true;
  }
private:
  TimerHandler<AlarmHelperImp, &AlarmHelperImp::alarmTimer> m_alarmTimer;
  
  std::map<std::pair<uint32_t, std::string>, uint64_t> m_alarm2Cnt;
};

