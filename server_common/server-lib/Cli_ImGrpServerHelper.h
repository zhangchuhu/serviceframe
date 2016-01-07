#pragma once

#include <stdio.h>
#include <set>
#include <map>
#include <string>
#include <vector>
#include <stdarg.h>

#include "common/core/ibase.h"
#include "common/core/form.h"
#include "common/core/ilink.h"
#include "server_common/server-lib/RouteTarget.h"

using namespace std;
using namespace core;

namespace server
{
    namespace cli
    {
        /****************************************************
         *                    Type Define                   *
         ****************************************************/
        #define MAX_CLI_LOG_BUFFER (1024*1024) //1M
        #define MAX_CMD_SIZE    1024
        #define MAX_FUNC_PARAS  3
        #define strCliHelpHint  "show help of CLI console"
        #define strCliExitHint  "exit the CLI console"
		#define strAdminMode	"Enter root Mode. eg. su [password]"//Privileged
		#define ROOTPASSWD		"qwert123"

        typedef enum 
        {
            FFT_VV = 0,         //void ::(void)
            FFT_VU32,           //void ::(uint32_t)
            FFT_VSETU32,        //void ::(set<uint32_t>)     
            FFT_VSTR,           //void ::(string)
            FFT_VU32SETU32,     //void ::(uint32_t, set<uint32_t>)

        	FFT_NULL,			//do nothing
        	FFT_LAST
        }eFuncFormType;
        
        typedef enum
        {
            E_FUNC_PARAM_VOID= 0,
            E_FUNC_PARAM_U32,
            E_FUNC_PARAM_STR,
            E_FUNC_PARAM_SET_U32,

            E_FUNC_PARA_INVALID
            
        }E_CLI_FUNC_PARA;

		typedef enum
		{
			E_CLI_MODE_USER=0,
			E_CLI_MODE_PRIVILEGED
			
		}E_CLI_USER_MODE;

        typedef void (PHClass::*CliTargetFunc)();
        union CliTargetProc
        {
            CliTargetFunc mf_oo;
            void (PHClass::*mf_vv)(core::IConn *pConn);
            void (PHClass::*mf_vu)(core::IConn *pConn, const uint32_t);
            void (PHClass::*mf_vsetu)(core::IConn *pConn, const set<uint32_t>);
            void (PHClass::*mf_vstr)(core::IConn *pConn, const string &);
            void (PHClass::*mf_vusetu)(core::IConn *pConn, const uint32_t, const set<uint32_t>);
        };
        

        struct CliFormEntry
        {
            string m_strCmd;
            string m_strDesc;
            eFuncFormType m_eFormType;
            E_CLI_FUNC_PARA m_eFuncParams[MAX_FUNC_PARAS];
            CliTargetFunc m_proc;
            PHClass *m_pTarget;
        };
        
        #define DECLARE_CLI_FORM_MAP static CliFormEntry *getFormEntries(); \
                                    static CliFormEntry CliFormEntries[];


        typedef void (*pFunc)();
        typedef struct
        {
            string          m_strCmdName;
            string          m_strCmdDesc;
//          template<typename T1, typename T2, typename T3> void (*pFunc)(const T1& P1, const T2& P2, const T3& P3);
            pFunc   ppFunc;
            
        }CLI_CMD_TBL;

        
        typedef struct
        {
            eFuncFormType  m_eFuncFormType;
            E_CLI_FUNC_PARA m_eFuncParams[MAX_FUNC_PARAS];
//          template<typename T1, typename T2, typename T3> void (*pFunc)(const T1& P1, const T2& P2, const T3& P3);
            void (*pFunc)(...);

        }CliCmdFunc;

        typedef struct
        {
            typedef vector<set<uint32_t> > typeVecSetU32;
            E_CLI_FUNC_PARA m_eFuncParams[MAX_FUNC_PARAS];
            vector<uint32_t> m_vecParamU32;
            vector<string> m_vecParamStr;
            typeVecSetU32 m_vecSetU32;
        }ParamList;

        class CCliServerHelper
        {
            public:
                typedef map<string, CliCmdFunc> typeMapStrCmdFunc;
                typedef map<string, CliCmdFunc>::iterator itTypeMapStrCmdFunc;
                typedef map<string, CliCmdFunc>::const_iterator cnitTypeMapStrCmdFunc;
                typedef map<string, CliFormEntry*> typeMapStrCmdEntry;
                typedef map<string, CliFormEntry*>::const_iterator cnitTypeMapCmdEntry;
				typedef map<uint32_t, E_CLI_USER_MODE> typeMapCidMode;
				typedef map<uint32_t, E_CLI_USER_MODE>::iterator itTypeMapCidMode;

				CCliServerHelper(void) : m_strModuleName(""){}
				~CCliServerHelper(void){}

				
//                virtual bool OnAddCliCmd(core::IConn *pConn, const string &strCmd, const CliCmdFunc &cliCmdFuncObj){};
//                virtual bool OnDelCliCmd(core::IConn *pConn, const string &strCmd){};
//                virtual bool OnDelCliCmd(core::IConn *pConn, cnitTypeMapStrCmdFunc &itStrCmdFunc){};
//                virtual bool OnClearAllCliCmd(void){};
//                virtual bool OnShowCliCmd(core::IConn *pConn, const string &strCmd){};
                virtual void OnShowAllCliCmd(core::IConn *pConn){};
                virtual void OnShowStates(core::IConn *pConn){};
                
                typeMapStrCmdFunc &GetMyCliCmdFunc(void)
                {
                    return m_mapStrCmdFunc;
                }

                void SetMyModuleName(const string &strName)
                {
                    m_strModuleName = strName;
                }

				const std::string& GetMyModuleName()
				{
					return m_strModuleName;
				}

                string TrimCmd(const string &strCmd);
                bool CmdLineHandle(const string &strCmd, core::IConn *pConn);
                uint32_t ParseSubCmdLine(const string &strCmd, vector<string> &vecSubCmd);
                int32_t GetParamSlotNum(const string &strCmd, const size_t uStarPos);
                bool string2Uint32(const string &strParam, uint32_t &uU32Num);
                bool string2SetU32(core::IConn *pConn, const string &strParam, set<uint32_t> &setU32);
				string SetUint2String(const set<uint32_t> &cnSetUint32);
                void addEntry(CliFormEntry *pCmdEnties, void *target);
                void addEntry(std::map<std::string, CliFormEntry>& mapCmd2Entry, void *target);
				uint32_t GetParamsSize(const string &strCmdParam);
				void SendCliLog(core::IConn *pConn, const stringstream &ssCliMsg);
				void SendCliLog(core::IConn *pConn, const string &strCliMsg);
				void CliLog(core::IConn *pConn, const char *fmt, ...);
//                void CLog();

            private:
                typeMapStrCmdEntry m_mapStrCmdEnties;

                string          m_strModuleName;
                typeMapStrCmdFunc m_mapStrCmdFunc;
				
			protected:
				typeMapCidMode 	m_mapCidMode;//map<Cid, Mode>
            
        };
    }
}
