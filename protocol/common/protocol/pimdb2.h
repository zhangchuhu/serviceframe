#pragma once

#include <vector>
#include <string>

//#include "iproperty.h"
//#include "protocol/const.h"
#include "core/base_svid.h"
//#include "./pverifycodepng.h"
//#include "common/protocol/iImV2.h"
#include "pimcommon.h"
#include "pimdb.h"

using namespace std;

namespace protocol
{
	namespace im
	{

		//根据积分添加好友（需要2次确认）
		struct PCS_ReqAddBuddyByScore : public sox::Marshallable 
		{
			enum {uri = (105 << 8) | protocol::IMDB_SVID};
			PCS_ReqAddBuddyByScore(){}
			PCS_ReqAddBuddyByScore(uint32_t uBid)
			{
				m_uBid=uBid;
			}
			uint32_t m_uBid; //


			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uBid;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uBid;
			}
		};


		//PCS_ReqAddBuddyByScore回应
		struct PCS_ReqAddBuddyByScoreRes : public PCS_CommonHead
		{
			enum {uri = (106 << 8) | protocol::IMDB_SVID};
			PCS_ReqAddBuddyByScoreRes(){}
			PCS_ReqAddBuddyByScoreRes(uint32_t uBid,uint32_t uScore)
			{
				m_uBid=uBid;
				m_uScore=uScore;

			}
			
			uint32_t m_uBid; // 
			uint32_t m_uScore;

			virtual void marshal(sox::Pack &p) const 
			{
				PCS_CommonHead::marshal(p);
				p << m_uBid<<m_uScore;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				PCS_CommonHead::unmarshal(p);
				p >> m_uBid>>m_uScore;
			}
		};


		//根据确认添加好友（同意或拒绝）
		struct PCS_OpWaitCheckBud2 : public sox::Marshallable 
		{
			enum {uri = (107 << 8) | protocol::IMDB_SVID};
			PCS_OpWaitCheckBud2(){}
			PCS_OpWaitCheckBud2(uint32_t uBid, uint32_t uBGid, 
				const string& strMsg, E_WAITCHECKLIST_OP e_Op,const string& strRemark) 
			{
				m_uBid=uBid;
				m_uBGid=uBGid;
				m_strMsg=strMsg;
				m_eOp=e_Op;
				m_strRemark=strRemark;
			}

				uint32_t m_uBid;
				uint32_t m_uBGid;
				string m_strMsg;
				E_WAITCHECKLIST_OP m_eOp;
				string m_strRemark;


				virtual void marshal(sox::Pack &p) const 
				{
					p << m_uBid << m_uBGid << m_strMsg<<m_strRemark;
					p.push_uint8(m_eOp);
				}

				virtual void unmarshal(const sox::Unpack &p) 
				{
					p >> m_uBid >> m_uBGid >> m_strMsg>>m_strRemark;
					m_eOp = (E_WAITCHECKLIST_OP)p.pop_uint8();
				}
		};

		//根据答案添加好友（2次确认）
		struct PCS_AnswerBuddyQuestion2 : public sox::Marshallable 
		{
			enum {uri = (108 << 8) | protocol::IMDB_SVID};
			PCS_AnswerBuddyQuestion2(){}
			PCS_AnswerBuddyQuestion2(uint32_t uBid,uint32_t uQuestion,const string &strAnswer)
			{
				m_uBid=uBid;
				m_uQuestionId=uQuestion;
				m_strAnswer=strAnswer;
			}
			uint32_t m_uBid; // 
			uint32_t m_uQuestionId;
			string m_strAnswer;

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uBid<<m_uQuestionId<<m_strAnswer;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uBid>>m_uQuestionId>>m_strAnswer;
			}
		};

		//PCS_AnswerBuddyQuestion2的响应
		struct PCS_AnswerBuddyQuestionRes2 : public PCS_CommonHead
		{
			enum {uri = (109 << 8) | protocol::IMDB_SVID};
			PCS_AnswerBuddyQuestionRes2(){}
			PCS_AnswerBuddyQuestionRes2(uint32_t uBid,const string &strBQuestion,const string& strAnswer)
			{
				m_uBid=uBid;
				m_strAnswer=strAnswer;
				m_strBQuestion=strBQuestion;
			}
			uint32_t m_uBid; // 
			string m_strBQuestion;//被添加人的问题
			string m_strAnswer;//添加人的回答

			virtual void marshal(sox::Pack &p) const 
			{
				PCS_CommonHead::marshal(p);
				p << m_uBid<<m_strBQuestion<<m_strAnswer;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				PCS_CommonHead::unmarshal(p);
				p >> m_uBid>>m_strBQuestion>>m_strAnswer;
			}
		};

		//把用户拖入黑名单
		struct PCS_AddToBlackList2 : public sox::Marshallable 
		{
			enum {uri = (110 << 8) | protocol::IMDB_SVID};
			PCS_AddToBlackList2(){}
			PCS_AddToBlackList2(uint32_t uBid)
				: m_uBid(uBid){}

				uint32_t m_uBid; // 

				virtual void marshal(sox::Pack &p) const 
				{
					p << m_uBid;
				}

				virtual void unmarshal(const sox::Unpack &p) 
				{
					p >> m_uBid;
				}
		};


		struct PCS_AddToBlackListRes2 : public PCS_CommonHead
		{
			enum {uri = (111 << 8) | protocol::IMDB_SVID};
			PCS_AddToBlackListRes2(){}
			PCS_AddToBlackListRes2(uint32_t uBid)
				: m_uBid(uBid){}

				uint32_t m_uBid; // 

				virtual void marshal(sox::Pack &p) const 
				{
					PCS_CommonHead::marshal(p);
					p << m_uBid;
				}

				virtual void unmarshal(const sox::Unpack &p) 
				{
					PCS_CommonHead::unmarshal(p);
					p >> m_uBid;
				}
		};

		//黑名单中加好友
		struct PCS_AddBuddyFromBlackList : public sox::Marshallable 
		{
			enum {uri = (112 << 8) | protocol::IMDB_SVID};
			PCS_AddBuddyFromBlackList(){}
			PCS_AddBuddyFromBlackList(uint32_t uBid,uint32_t uBGid)
			{	
				m_uBid=uBid;
				m_uBGid=uBGid;
			}

			uint32_t m_uBid; // 
			uint32_t m_uBGid;

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uBid<<m_uBGid;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uBid>>m_uBGid;
			}
		};


		//加好友通用命令（第2版）
		struct PCS_AddBuddy2 : public PCS_CommonHead
		{
			enum { uri = (113 << 8) | protocol::IMDB_SVID };
			PCS_AddBuddy2(){}
		
			PCS_AddBuddy2(uint32_t uBid, uint32_t uBGid, const SUserinfo& objUInfo,uint32_t uBImid,const string& strRemark) 
			{
				m_uBid=uBid;
				m_uBGid=uBGid;
				m_objBUInfo=objUInfo;
				m_uBImid=uBImid;
				m_strRemark=strRemark;
			}
			

			uint32_t m_uBid;
			uint32_t m_uBGid;
			SUserinfo m_objBUInfo;
			uint32_t m_uBImid;
			string m_strRemark;

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uBid<< m_uBGid << m_objBUInfo<<m_uBImid<<m_strRemark;
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uBid>> m_uBGid >> m_objBUInfo>>m_uBImid>>m_strRemark;
			}
		};

		struct PCS_DelFrBlackList2 : public sox::Marshallable 
		{
			enum {uri = (114 << 8) | protocol::IMDB_SVID};
			PCS_DelFrBlackList2(){}
			PCS_DelFrBlackList2(uint32_t uBid,bool bIsOnlyDel)
				: m_uBid(uBid)
			{
				m_bIsOnlyDel=bIsOnlyDel;
			}

				uint32_t m_uBid; // 
				bool m_bIsOnlyDel;
				virtual void marshal(sox::Pack &p) const 
				{
					p << m_uBid;
					p.push_uint8(m_bIsOnlyDel);
					
				}

				virtual void unmarshal(const sox::Unpack &p) 
				{
					p >> m_uBid;
					m_bIsOnlyDel = (p.pop_uint8() != 0 ? true : false);
				}
		};


		struct PCS_DelFrBlackListRes2 : public PCS_CommonHead
		{
			enum {uri = (115 << 8) | protocol::IMDB_SVID};
			PCS_DelFrBlackListRes2(){}
			PCS_DelFrBlackListRes2(uint32_t uBid)
				: m_uBid(uBid){}

				uint32_t m_uBid; // 

				virtual void marshal(sox::Pack &p) const 
				{
					PCS_CommonHead::marshal(p);
					p << m_uBid;
				}

				virtual void unmarshal(const sox::Unpack &p) 
				{
					PCS_CommonHead::unmarshal(p);
					p >> m_uBid;
				}
		};

		//设置黑名单保护期
		struct PCS_SetBlackListTemporary : public sox::Marshallable 
		{
			enum {uri = (116 << 8) | protocol::IMDB_SVID};
			PCS_SetBlackListTemporary(){}
			PCS_SetBlackListTemporary(uint32_t uBid)
				: m_uBid(uBid){}

				uint32_t m_uBid; //谁设置了黑名单保护期
				virtual void marshal(sox::Pack &p) const 
				{
					p << m_uBid;
				}

				virtual void unmarshal(const sox::Unpack &p) 
				{
					p >> m_uBid;
				}
		};

		struct PCS_UpdateMyStrategy2  : public sox::Marshallable 
		{
			enum {uri = (117 << 8) | protocol::IMDB_SVID};
			PCS_UpdateMyStrategy2()
			{
				m_uCeLue=CHECKBYBUDDY;
				m_uJiFen=0;
				m_strAnswer="";
				m_strQuestion="";
				m_bIsCheck=false;
			}
			PCS_UpdateMyStrategy2(uint32_t uCeLue,uint32_t uJiFen,const string& strQuestion,const string &strAnswer,bool bIsCheck)
			{
				m_uCeLue=uCeLue;
				m_strAnswer=strAnswer;
				m_strQuestion=strQuestion;
				m_uJiFen=uJiFen;
				m_bIsCheck=bIsCheck;
			}

			uint32_t m_uCeLue;
			string m_strQuestion;
			string m_strAnswer;
			uint32_t m_uJiFen;
			bool m_bIsCheck;

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uCeLue<<m_uJiFen<<m_strAnswer<<m_strQuestion;
				p.push_uint8(m_bIsCheck);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uCeLue>>m_uJiFen>>m_strAnswer>>m_strQuestion;
				m_bIsCheck = (p.pop_uint8() != 0 ? true : false);
			}
		};

		struct PCS_UpdateMyCeLueRes2  : public PCS_CommonHead
		{
			enum {uri = (118 << 8) | protocol::IMDB_SVID};
			PCS_UpdateMyCeLueRes2(){}
			PCS_UpdateMyCeLueRes2(uint32_t uCeLue,uint32_t uJiFen,const string& strQuestion,const string &strAnswer,bool bIsCheck)
			{
				m_uCeLue=uCeLue;
				m_strAnswer=strAnswer;
				m_strQuestion=strQuestion;
				m_uJiFen=uJiFen;
				m_bIsCheck=false;
			}

			uint32_t m_uCeLue;
			string m_strQuestion;
			string m_strAnswer;
			uint32_t m_uJiFen;
			bool m_bIsCheck;

			virtual void marshal(sox::Pack &p) const 
			{
				PCS_CommonHead::marshal(p);
				p << m_uCeLue<<m_uJiFen<<m_strAnswer<<m_strQuestion;
				p.push_uint8(m_bIsCheck);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				PCS_CommonHead::unmarshal(p);
				p >> m_uCeLue>>m_uJiFen>>m_strAnswer>>m_strQuestion;
				m_bIsCheck = (p.pop_uint8() != 0 ? true : false);
			}
		};

		//add by kdjie, 2010.7.12
		//根据确认添加好友（同意或拒绝）

		struct PCS_AddBuddyForce : public sox::Marshallable 
		{
			enum {uri = (119 << 8) | protocol::IMDB_SVID};

			uint32_t m_uBid;
			uint32_t m_uBGid;
			std::string m_strRemarkMySelf;
			std::string m_strRemarkFriend;
			std::string m_strMsg;

			PCS_AddBuddyForce() {}
			PCS_AddBuddyForce(uint32_t uBid, uint32_t uBGid, 
				const std::string &strRemarkMySelf,
				const std::string &strRemarkFriend,
				const std::string &strMsg) 
			{
				m_uBid = uBid;
				m_uBGid = uBGid;
				m_strRemarkMySelf = strRemarkMySelf;
				m_strRemarkFriend = strRemarkFriend;
				m_strMsg = strMsg;
			}

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uBid << m_uBGid << m_strRemarkMySelf << m_strRemarkFriend << m_strMsg;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uBid >> m_uBGid >> m_strRemarkMySelf >> m_strRemarkFriend >> m_strMsg;
			}
		};
		//add end

		//添加好友前先向服务器设置备注和分组
		struct PCS_SetAddBuddyInfo :  public sox::Marshallable 
		{
			enum {uri = (120 << 8) | protocol::IMDB_SVID};

			uint32_t m_uBid; 
			uint32_t m_uBGid; //好友分组id
			std::string m_strRemark; //好友备注

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uBid << m_uBGid << m_strRemark;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uBid >> m_uBGid >> m_strRemark;
			}
		};

		//正式加好友前，测试积分是否达到要求
		struct PCS_TestReqAddBuddyByScore : public sox::Marshallable 
		{
			enum {uri = (121 << 8) | protocol::IMDB_SVID};

			uint32_t m_uBid; 

			PCS_TestReqAddBuddyByScore() {}
			PCS_TestReqAddBuddyByScore(uint32_t uBid) : m_uBid(uBid) {}

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uBid;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uBid;
			}
		};

		struct PCS_TestReqAddBuddyByScoreRes : public PCS_CommonHead
		{
			enum {uri = (122 << 8) | protocol::IMDB_SVID};

			uint32_t m_uBid;
			uint32_t m_uScore;

			PCS_TestReqAddBuddyByScoreRes(){}
			PCS_TestReqAddBuddyByScoreRes(uint32_t uBid,uint32_t uScore)
			{
				m_uBid=uBid;
				m_uScore=uScore;
			}

			virtual void marshal(sox::Pack &p) const 
			{
				PCS_CommonHead::marshal(p);
				p << m_uBid<<m_uScore;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				PCS_CommonHead::unmarshal(p);
				p >> m_uBid>>m_uScore;
			}
		};


		//正式加好友前，测试答案是否正确
		struct PCS_TestAnswerBuddyQuestion : public sox::Marshallable 
		{
			enum {uri = (123 << 8) | protocol::IMDB_SVID};

			uint32_t m_uBid; 
			uint32_t m_uQuestionId;
			string m_strAnswer;

			PCS_TestAnswerBuddyQuestion(){}
			PCS_TestAnswerBuddyQuestion(uint32_t uBid, uint32_t uQuestion, const string &strAnswer)
			{
				m_uBid=uBid;
				m_uQuestionId=uQuestion;
				m_strAnswer=strAnswer;
			}

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uBid<<m_uQuestionId<<m_strAnswer;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uBid>>m_uQuestionId>>m_strAnswer;
			}
		};

		struct PCS_TestAnswerBuddyQuestionRes : public PCS_CommonHead
		{
			enum {uri = (124 << 8) | protocol::IMDB_SVID};

			uint32_t m_uBid;
			uint32_t m_uQuestionId;
			string m_strBQuestion;//被添加人的问题
			string m_strAnswer;//添加人的回答

			PCS_TestAnswerBuddyQuestionRes(){}
			PCS_TestAnswerBuddyQuestionRes(uint32_t uBid, uint32_t uQuestion, const string &strBQuestion,const string& strAnswer)
			{
				m_uBid=uBid;
				m_uQuestionId = uQuestion;
				m_strBQuestion=strBQuestion;
				m_strAnswer=strAnswer;
			}

			virtual void marshal(sox::Pack &p) const 
			{
				PCS_CommonHead::marshal(p);
				p << m_uBid << m_uQuestionId <<m_strBQuestion<<m_strAnswer;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				PCS_CommonHead::unmarshal(p);
				p >> m_uBid >> m_uQuestionId >>m_strBQuestion>>m_strAnswer;
			}
		};
	}
}

