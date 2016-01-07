#include "Cli_ImGrpServerHelper.h"
#include <string>
#include <sstream>
#include <iostream> 


using namespace std;
using namespace core;
using namespace server::cli;


bool CCliServerHelper::CmdLineHandle(const string &strCmd, core::IConn *pConn)
{
	size_t uFndPos;
	string strCmdLine;
	string strRevertCmd;
	int32_t nParamSlot;
	uint32_t uParamCnt;
	uint32_t uSubCmdCnt;
	vector<string> vecSubCmdLine;
	vector<uint32_t> vecParamSlots;
	ParamList ParamListObj;

	/*1 - Trim the input comand line.*/
	strCmdLine = TrimCmd(strCmd);
	if(strCmdLine.empty())
	{
		return false;
	}
	
	/*2 - Parse the command line.*/
	uSubCmdCnt = ParseSubCmdLine(strCmdLine, vecSubCmdLine);

	/*3 - Parse and process the command line.*/
	string strTrace = "NULL";
	cnitTypeMapCmdEntry cnitMapStrEntry = m_mapStrCmdEnties.begin();//map<string, CliFormEntry*>
	for(; cnitMapStrEntry != m_mapStrCmdEnties.end(); ++cnitMapStrEntry)
	{
		/**
		 * 4 - Revert the command line
		 **/
		uParamCnt = 0;
		vecParamSlots.clear();
		strRevertCmd = cnitMapStrEntry->first;
		
		/*4.1 - Check the parameters count.*/
		if((GetParamsSize(strRevertCmd)+1 ) != vecSubCmdLine.size())
		{
			continue;
		}

		uFndPos = strRevertCmd.find('*');
		while(uFndPos != string::npos)
		{			
			uParamCnt++;
			if((nParamSlot = GetParamSlotNum(strRevertCmd, uFndPos)) == -1)
			{
				break;
			}

			if(vecSubCmdLine.size() < (uint32_t)nParamSlot)
			{
				break;
			}

			vecParamSlots.push_back((uint32_t)nParamSlot);
			strRevertCmd.replace(uFndPos, 1, vecSubCmdLine[nParamSlot+1]);
			uFndPos = strRevertCmd.find('*', uFndPos+vecSubCmdLine[nParamSlot].size()+1);

		}

		/*5 - Check if find the handler.*/
		if(strRevertCmd == strCmdLine)
		{
			if((uParamCnt == vecParamSlots.size()) && (uParamCnt <= MAX_FUNC_PARAS))
			{
				CliFormEntry *pFormEntry = cnitMapStrEntry->second;
				if(NULL == pFormEntry)
				{
					return false;
				}
				
				if(uParamCnt != 0)
				{
					for(uint32_t i = 0; i < uParamCnt; i++)
					{
						switch(pFormEntry->m_eFuncParams[i])
						{
							case E_FUNC_PARAM_U32:
							{
								uint32_t uTmpNum;
								if(!string2Uint32(vecSubCmdLine[vecParamSlots[i]+1], uTmpNum))
								{
									return false;
								}
								ParamListObj.m_vecParamU32.push_back(uTmpNum);
								
								break;
							}
							case E_FUNC_PARAM_STR:
							{
								ParamListObj.m_vecParamStr.push_back(vecSubCmdLine[vecParamSlots[i]+1]);
								break;
							}
							case E_FUNC_PARAM_SET_U32:
							{
								set<uint32_t> setTNums;
								if(!string2SetU32(pConn, vecSubCmdLine[vecParamSlots[i]+1], setTNums))
								{
									return false;
								}
								ParamListObj.m_vecSetU32.push_back(setTNums);
								
								break;
							}
							case E_FUNC_PARAM_VOID:
							case E_FUNC_PARA_INVALID:
							default:
								CliLog(pConn, "Invalid param type(%u)!\n", pFormEntry->m_eFuncParams[i]);
								return false;
								break;
							
						}
					}
				}

				//Function handler execute.
				CliTargetProc TargetProc;
				TargetProc.mf_oo = pFormEntry->m_proc;
				switch(pFormEntry->m_eFormType)//Add core::IConn *pConn
				{
					case FFT_VV:
						if(pFormEntry->m_pTarget != NULL)
						{
							(pFormEntry->m_pTarget->*TargetProc.mf_vv)(pConn);//TODO !!!
						}
						break;
					case FFT_VU32:
//						uint32_t uUid = ParamListObj.m_vecParamU32[0];
//						cmdFuncRef.pFunc(uUid);
						break;
					case FFT_VSETU32:
						if(pFormEntry->m_pTarget != NULL)
						{
							(pFormEntry->m_pTarget->*TargetProc.mf_vsetu)(pConn, ParamListObj.m_vecSetU32[0]);//TODO !!!
						}
						break;
					case FFT_VSTR:
						if(pFormEntry->m_pTarget != NULL)
						{
							(pFormEntry->m_pTarget->*TargetProc.mf_vstr)(pConn, ParamListObj.m_vecParamStr[0]);//TODO
						}
						break;
					case FFT_VU32SETU32:
						if(pFormEntry->m_pTarget != NULL)
						{
							(pFormEntry->m_pTarget->*TargetProc.mf_vusetu)(pConn, ParamListObj.m_vecParamU32[0], ParamListObj.m_vecSetU32[0]);
						}
						break;
					case FFT_NULL:
					case FFT_LAST:
					default:
						CliLog(pConn, "Invalid function type(%u)!\n", (uint32_t)pFormEntry->m_eFormType);
						return false;
						break;
				}
			}

			return true;
		}
		
	}

	return false;
}


string CCliServerHelper::TrimCmd(const string &strCmd)
{
	uint32_t uCnt = 0;
	int32_t nIndex = 0;
	uint32_t uCmdLineLen = 0;
	char *ptrCliCmd = NULL;
	char chCliCmd[MAX_CMD_SIZE];

	memset(chCliCmd, 0, MAX_CMD_SIZE);
	snprintf(chCliCmd, MAX_CMD_SIZE, "%s", strCmd.c_str());

	
	/*1 - Trim the left string.*/
	ptrCliCmd = chCliCmd;
	while((*ptrCliCmd == ' ') || (*ptrCliCmd == '\t'))
	{
		uCnt++;
		ptrCliCmd++;
	}
	
	if(uCnt > 0)
	{
		uCmdLineLen = strlen(chCliCmd);
		for(nIndex = 0; (uint32_t)nIndex < (uCmdLineLen-uCnt); nIndex++)
		{
			chCliCmd[(uint32_t)nIndex] = *ptrCliCmd++;
		}
		chCliCmd[(uint32_t)nIndex] = '\0';
	}

	/*2 - Trim the right string.*/
	uCmdLineLen = strlen(chCliCmd);
	for(nIndex = (uCmdLineLen-1); nIndex >= 0; nIndex--)
	{
		if((' ' == chCliCmd[nIndex]) || 
			('\t' == chCliCmd[nIndex]) || 
			('\n' == chCliCmd[nIndex]) || 
			('\r' == chCliCmd[nIndex]))
		{
			chCliCmd[nIndex] = '\0';
		}
		else
		{
			break;
		}
	}

	return chCliCmd;
}

//show uids 123,456,789 gids
//help
//show uids
//exit

uint32_t CCliServerHelper::ParseSubCmdLine(const string &strCmd, vector<string> &vecSubCmd)
{
	size_t uFndPos;
	string strCmdLine;
	string strTCmdLine;

	/*1 - Save the original command line.*/
	vecSubCmd.push_back(strCmd);
	
	strCmdLine = strCmd;
	while(!strCmdLine.empty())
	{
		uFndPos = strCmdLine.find(' ');
		if(uFndPos == string::npos)
		{
			vecSubCmd.push_back(strCmdLine);
			break;
		}

		/*2 - Save the sub command line.*/
		strTCmdLine = strCmdLine;
		vecSubCmd.push_back(strCmdLine.substr(0, uFndPos));
		strCmdLine = strTCmdLine.substr(uFndPos+1);
	}

	return vecSubCmd.size();	
}


int32_t CCliServerHelper::GetParamSlotNum(const string &strCmd, const size_t uStarPos)
{
	size_t uPos = 0;
	uint32_t uStarSlotNum = 0;

	uPos = strCmd.find(' ', 0);
	while(uPos != string::npos)	
	{
		uStarSlotNum++;
		if(uPos+1 == uStarPos)
		{
			return (int32_t)(uStarSlotNum);
		}
		uPos = strCmd.find(' ', uPos+1);
	}

	return (-1);
}

bool CCliServerHelper::string2Uint32(const string &strParam, uint32_t &uU32Num)
{
	char *ptrParam = NULL;
	string strNewParam;
	char chParam[128];
	
	memset(chParam, 0, 128);
	strNewParam = TrimCmd(strParam);
	sprintf(chParam, "%s", strNewParam.c_str());

	ptrParam = chParam;
	while (*ptrParam != '\0')
	{
		if (!((*ptrParam >= '0') && (*ptrParam <= '9')))
		{
			return false;
		}
		ptrParam++;
	}

	
	if(0 == strlen(chParam)) /*empty line*/
	{
		return false;
	}
	
	uU32Num = (uint32_t)strtoul(chParam, NULL, 10);
	return true;
}

bool CCliServerHelper::string2SetU32(core::IConn *pConn, const string &strParam, set<uint32_t> &setU32)
{
	stringstream ssNumList;
	size_t uFndPos;
	char *ptrParam = NULL;
	string strNum;
	string strTParam;
	string strLeftNum;
	string strNewParam;
	char chParam[128];
	
	memset(chParam, 0, 128);
	strNewParam = TrimCmd(strParam);
	sprintf(chParam, "%s", strNewParam.c_str());

	ptrParam = chParam;
	while (*ptrParam != '\0')
	{
		if (!((*ptrParam >= '0') && (*ptrParam <= '9')) && !(*ptrParam == ','))
		{
			return false;
		}
		ptrParam++;
	}


	strLeftNum = strNewParam;
	while(!strLeftNum.empty())
	{
		uFndPos = strLeftNum.find(',');
		if(uFndPos == string::npos)
		{
			uint32_t uNum;
			if(string2Uint32(strLeftNum, uNum))
			{
				setU32.insert(uNum);
			}
			break;
		}

		if(uFndPos != 0)
		{
			uint32_t uTNum;
			strNum = strLeftNum.substr(0, uFndPos);
			if(!string2Uint32(strNum, uTNum))
			{
				return false;
			}
			setU32.insert(uTNum);
		}

		if(uFndPos == strLeftNum.size() -1)
		{
			break;
		}
		
		strTParam = strLeftNum;
		strLeftNum = strTParam.substr(uFndPos+1);
	}

	return true;
}

string CCliServerHelper::SetUint2String(const set<uint32_t> &cnSetUint32)
{
	stringstream ssList;

	set<uint32_t>::const_iterator cnitSetUint = cnSetUint32.begin();
	for(; cnitSetUint != cnSetUint32.end(); ++cnitSetUint)
	{
		ssList << (*cnitSetUint) << ",";
	}

	return (ssList.str());	
}


void CCliServerHelper::addEntry(CliFormEntry *pCmdEnties, void *target)
{
	uint32_t i = 0;
	
	while(pCmdEnties[i].m_strCmd != "")
	{
		if(m_mapStrCmdEnties.find(pCmdEnties[i].m_strCmd) != m_mapStrCmdEnties.end())
		{
			log(Warn, "CLI:Duplicated cmd form entry(%s)!\n", pCmdEnties[i].m_strCmd.c_str());
		}

		m_mapStrCmdEnties[pCmdEnties[i].m_strCmd] = &pCmdEnties[i];
		pCmdEnties[i].m_pTarget= (PHClass *)target;
		i++;
	}
}

void CCliServerHelper::addEntry(map<string, CliFormEntry>& mapCmd2Entry, void *target)
{
	for (map<string, CliFormEntry>::iterator it = mapCmd2Entry.begin(); it != mapCmd2Entry.end(); ++it)
	{
		if(m_mapStrCmdEnties.find(it->first) != m_mapStrCmdEnties.end())
		{
			log(Warn, "CLI:Duplicated cmd form entry(%s)!\n", it->first.c_str());
		}

		m_mapStrCmdEnties[it->first] = &it->second;
		it->second.m_pTarget = (PHClass *)target;
	}
}

uint32_t CCliServerHelper::GetParamsSize(const string &strCmdParam)
{
	size_t uPos = 0;
	uint32_t uTotalCnt = 0;
 
	uPos = strCmdParam.find(' ', 0);
	while(uPos != string::npos) 
	{
		uTotalCnt++;
 		uPos = strCmdParam.find(' ', uPos+1);
	}
	
	uTotalCnt++;

	return (uTotalCnt);
 }

 void CCliServerHelper::CliLog(core::IConn *pConn, const char *fmt, ...)
 {
	va_list param;
 	char chBuffer[MAX_CLI_LOG_BUFFER];

	if(NULL == pConn)
	{
		return;
	}
	memset(chBuffer, 0, MAX_CLI_LOG_BUFFER);

	va_start(param, fmt);
	vsnprintf(chBuffer, MAX_CLI_LOG_BUFFER, fmt, param);
	va_end(param);

	if(0 == strlen(chBuffer))
	{
		string strPrompt = m_strModuleName;
		itTypeMapCidMode itMapCidMode = m_mapCidMode.find(pConn->getConnId());
		if((itMapCidMode != m_mapCidMode.end()) && (E_CLI_MODE_PRIVILEGED == itMapCidMode->second))
		{
			strPrompt += "#";
		}
		else
		{
			strPrompt += ">";
		}
		pConn->sendBin(strPrompt.c_str(), strPrompt.length(), 0);
	}
	else
	{
		pConn->sendBin(chBuffer, strlen(chBuffer), 0);
	}
 }

 

