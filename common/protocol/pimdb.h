#pragma once

#include <vector>
#include <string>

#include "iproperty.h"
#include "protocol/const.h"
#include "core/base_svid.h"
#include "./pverifycodepng.h"
#include "common/protocol/iImV2.h"
#include "pimcommon.h"

using namespace std;

namespace protocol
{
	namespace im
	{
		//wuji start
		
		//enum E_HEADPHOTO_FLAG{NOHEAD=-1,USERHEAD=0};
		enum E_HEADPHOTO_FLAG{NOHEAD=-1,USERHEAD=0};//-1浠ｈ〃娌℃湁澶村儚锛?浠ｈ〃浣跨敤鑷畾涔夊ご�?
		enum E_PRESENCE_TYPE{ CHANEL=1,GAME=2,HEADPHOTO=3, HEADPHOTO16x9 = 4};
		enum E_CELUE_OP{ CHECKBYBUDDY=1, REJECT=2,CHECKJIFEN=3,CHECKQUESTION=4};
		
		// 	1閿熸枻鎷?閿熸枻鎷疯閫氶敓鏂ゆ嫹閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷疯瘉閿熸枻鎷烽敓鏉板府鎷烽敓鏂ゆ嫹閿熸枻鎷蜂负閿熸枻鎷烽敓鏂ゆ�?
		// 	2閿熸枻鎷?閿熸澃鎾呮嫹閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓鏂ゆ嫹涓洪敓鏂ゆ嫹閿熸枻�?
		// 	3閿熸枻鎷?閿熸枻鎷烽敓鍓夸紮鎷烽敓琛楃鎷烽敓鏂ゆ嫹[閿熺煫浼欐嫹閿熷€熷畾涓€閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷峰€�?閿熻剼鍖℃嫹閿熺殕纭锋嫹閿熸枻鎷蜂负閿熸枻鎷烽敓鏂ゆ�?
		// 	4閿熸枻鎷?閿熸枻鎷疯閿熸枻鎷风‘閿熸埅杈炬嫹閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓鏉板府鎷烽敓鏂ゆ嫹閿熸枻鎷蜂负閿熸枻鎷烽敓绐栵綇鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓鏂ゆ嫹閿熺煫浼欐嫹閿熷€熷畾閿熸枻鎷?
	




///////////

		/*
		+---------------+------------------+------+-----+----------+----------------+
		| Field         | Type             | Null | Key | Default  | Extra          |
		+---------------+------------------+------+-----+----------+----------------+
		| id            | bigint(20)       | NO   | PRI | NULL     | auto_increment | 
		| account       | varchar(255)     | NO   | MUL | NULL     |                | 
		| passwd        | varchar(255)     | YES  |     |          |                | 
		| nick          | varchar(255)     | YES  |     |          |                | 
		| sex           | tinyint(4)       | YES  |     | 0        |                | 
		| birthday      | int(11)          | YES  |     | 19800101 |                | 
		| area          | int(11)          | YES  |     | -1       |                | 
		| province      | int(11)          | YES  |     | -1       |                | 
		| city          | int(11)          | YES  |     | -1       |                | 
		| validate      | tinyint(4)       | YES  |     | 0        |                | 
		| resume        | text             | YES  |     | NULL     |                | 
		| last_time     | datetime         | YES  | MUL | NULL     |                | 
		| last_ip       | int(11) unsigned | YES  |     | NULL     |                | 
		| version       | int(11)          | YES  |     | 0        |                | 
		| register_time | datetime         | YES  |     | NULL     |                | 
		| jifen         | int(11)          | YES  |     | 0        |                | 
		| sign          | varchar(255)     | YES  |     |          |                | 
		| passport      | varchar(255)     | YES  | MUL |          |                | 
		| intro         | varchar(255)     | YES  |     |          |                | 
		+---------------+------------------+------+-----+----------+----------------+

		mysql> desc imid;
		+-------------+----------+------+-----+---------------------+----------------+
		| Field       | Type     | Null | Key | Default             | Extra          |
		+-------------+----------+------+-----+---------------------+----------------+
		| id          | int(11)  | NO   | PRI | NULL                | auto_increment | 
		| imid        | int(11)  | NO   | UNI | NULL                |                | 
		| uid         | int(11)  | NO   | UNI | NULL                |                | 
		| create_time | datetime | NO   |     | 0000-00-00 00:00:00 |                | 
		+-------------+----------+------+-----+---------------------+----------------+
		4 rows in set (0.00 sec)

		mysql> desc tbl_im_uinfo;
		+-------------+---------------+------+-----+---------+----------------+
		| Field       | Type          | Null | Key | Default | Extra          |
		+-------------+---------------+------+-----+---------+----------------+
		| nid         | bigint(20)    | NO   | PRI | NULL    | auto_increment | 
		| uid         | decimal(20,0) | NO   | UNI | NULL    |                | 
		| nimageindex | smallint(6)   | NO   |     | -1      |                | 
		| ncelue      | smallint(6)   | NO   |     | 1       |                | 
		+-------------+---------------+------+-----+---------+----------------+
		4 rows in set (0.00 sec)

		*/


		struct SUserinfo1 : public sox::Marshallable //锟斤拷锟斤拷锟斤拷锟斤拷3锟斤拷锟斤拷锟斤拷锟斤拷锟侥大部凤拷锟街讹拷
		{

			SUserinfo1()
			{
				m_uId=(uint32_t)-1;
				m_uHeadPhoto=(uint32_t)NOHEAD;//ͷ��,û�о���-1
				m_uImId=(uint32_t)-1;//yy閿熸暀鍙凤綇鎷锋病閿熷彨鎾呮嫹閿熸枻鎷?1
				m_stat=IMOFFLINE;
				m_uBirthDay=(uint32_t)-1;
				m_uArea=(uint32_t)-1;
				m_uProvince=(uint32_t)-1;
				m_uCity=(uint32_t)-1;
				m_uJiFen=0;

			}

			SUserinfo1(uint32_t uUid)
			{
				m_uId = uUid;
				m_uId=(uint32_t)-1;
				m_uHeadPhoto=(uint32_t)NOHEAD;//ͷ��,û�о���-1
				m_uImId=(uint32_t)-1;//yy閿熸暀鍙凤綇鎷锋病閿熷彨鎾呮嫹閿熸枻鎷?1
				m_stat=IMOFFLINE;
				m_uBirthDay=(uint32_t)-1;
				m_uArea=(uint32_t)-1;
				m_uProvince=(uint32_t)-1;
				m_uCity=(uint32_t)-1;
				m_uJiFen=0;
			}

			uint32_t m_uId;//锟矫伙拷ID
			std::string m_strNickname;//锟矫伙拷锟角筹拷
			protocol::uinfo::SEX m_sex;
			uint32_t m_uBirthDay;
			uint32_t m_uArea;
			uint32_t m_uProvince;
			uint32_t m_uCity;
			string m_strResume;
			uint32_t m_uVersion;
			uint32_t m_uJiFen;
			string m_strSign;
			string m_strIntro;

			protocol::E_IMSTATUS m_stat;//鐘舵�?

			uint32_t m_uHeadPhoto;//头锟斤拷,没锟叫撅拷锟斤拷-1
			uint32_t m_uImId;//yy閿熸暀鍙凤綇鎷锋病閿熷彨鎾呮嫹閿熸枻鎷?1



			virtual void marshal(sox::Pack &p) const 
			{
				p<<m_uId<<m_strNickname<<m_uBirthDay<<m_uArea<<m_uProvince<<m_uCity<<m_strResume<<m_uVersion<<m_uJiFen<<m_strSign<<m_strIntro<<m_uHeadPhoto<<m_uImId;
				p.push_uint8(m_sex);
				p.push_uint8(m_stat);

			}
			virtual void unmarshal(const sox::Unpack &p) 
			{
				p>>m_uId>>m_strNickname>>m_uBirthDay>>m_uArea>>m_uProvince>>m_uCity>>m_strResume>>m_uVersion>>m_uJiFen>>m_strSign>>m_strIntro>>m_uHeadPhoto>>m_uImId;
				m_sex = (protocol::uinfo::SEX)p.pop_uint8();
				m_stat = (protocol::E_IMSTATUS)p.pop_uint8();
			}
		};


	///////////////////////////

		struct CObjList2: public sox::Marshallable
		{
			uint32_t m_uId;       
			string   m_strData;       // �Զ���ͷ��

			virtual void marshal(sox::Pack &p) const 
			{
				p<<m_uId<<m_strData;
			}
			
			virtual void unmarshal(const sox::Unpack &p) 
			{
				p>>m_uId>>m_strData;					
			}

		};

		struct CObjList1: public sox::Marshallable 
		{
			uint32_t m_uId;           // uid
			uint32_t m_uValueId;      // ͷ��Ƶ������Ϸid
			string m_strData;         // 

			CObjList1() : m_uId(0), m_uValueId(-1) {}
			CObjList1(uint32_t uUId) : m_uId(uUId), m_uValueId(-1) {}
			CObjList1(uint32_t uUId, uint32_t uValue, const std::string& str) 
				: m_uId(uUId), m_uValueId(uValue), m_strData(str) 
			{
			}

			virtual void marshal(sox::Pack &p) const 
			{
				p<<m_uId;
				p.push_uint32(m_uValueId);
				p<<m_strData;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p>>m_uId;	
				m_uValueId=p.pop_uint32();
				p>>m_strData;
			}
		};
	

/////////////////IMID进程使用
		struct 		PCS_GetImdi4NewUser : public sox::Voidmable
		{
			enum {uri = (1 << 8) | protocol::IMIDD_SVID};
			
		};		

		//		struct 		PCS_GetImdi4NewUserRes : public sox::Marshallable 
		struct 		PCS_GetImdi4NewUserRes : public protocol::im::PCS_CommonHead
		{
			enum {uri = (2 << 8) | protocol::IMIDD_SVID};
			PCS_GetImdi4NewUserRes(){}

			uint32_t m_uId;
			uint32_t m_uImId;

			virtual void marshal(sox::Pack &p) const 
			{
				PCS_CommonHead::marshal(p);
				p << m_uId<<m_uImId;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				PCS_CommonHead::unmarshal(p);
				p >> m_uId>>m_uImId;
			}
		};		


		
///////////////////////////IMLIST进程使用


		
		struct 	PCS_GetMyHeadPhoto : public sox::Marshallable 
		{
			enum {uri = (1 << 8) | protocol::IMDB_SVID};
			PCS_GetMyHeadPhoto(){}
			
			virtual void marshal(sox::Pack &p) const 
			{
				
			}
			
			virtual void unmarshal(const sox::Unpack &p) 
			{
				
			}
			
		};
	
			struct 	PCS_GetMyHeadPhotoRes : public PCS_CommonHead
			{
				enum {uri = (2 << 8) | protocol::IMDB_SVID};
				PCS_GetMyHeadPhotoRes(){}
				
				uint32_t m_uHeadPhoto;
				std::string m_strCustomHeadPhotoPath;
				virtual void marshal(sox::Pack &p) const 
				{
					PCS_CommonHead::marshal(p);
					p<<m_uHeadPhoto << m_strCustomHeadPhotoPath;
				}
				
				virtual void unmarshal(const sox::Unpack &p) 
				{
					PCS_CommonHead::unmarshal(p);
					p>>m_uHeadPhoto >> m_strCustomHeadPhotoPath;
					
				}
				
			};
	

//////////////////////////////////////

		struct 	PCS_GetBuddyGameList : public sox::Marshallable 
		{
			enum {uri = (3 << 8) | protocol::IMDB_SVID};
			PCS_GetBuddyGameList(){}
			
			virtual void marshal(sox::Pack &p) const 
			{
				
			}
			
			virtual void unmarshal(const sox::Unpack &p) 
			{
				
			}
			
		};
		
		struct 	PCS_GetBuddyGameListRes : public PCS_CommonHead
		{
			enum {uri = (4 << 8) | protocol::IMDB_SVID};
			
			PCS_GetBuddyGameListRes(){}
			
			vector<CObjList1> m_vGame;//锟斤拷锟斤拷头锟斤拷锟叫憋拷
			

			virtual void marshal(sox::Pack &p) const 
			{
				PCS_CommonHead::marshal(p);
				marshal_container(p, m_vGame);
				
			}
			
			virtual void unmarshal(const sox::Unpack &p) 
			{
				PCS_CommonHead::unmarshal(p);
				unmarshal_container(p, std::back_inserter(m_vGame));
				
			}
			
		};



		struct 	PCS_GetBuddyChanelList : public sox::Marshallable 
		{
			enum {uri = (5 << 8) | protocol::IMDB_SVID};
			PCS_GetBuddyChanelList(){}
			
			virtual void marshal(sox::Pack &p) const 
			{
				
			}
			
			virtual void unmarshal(const sox::Unpack &p) 
			{
				
			}
			
		};
		
		struct 	PCS_GetBuddyChanelListRes : public PCS_CommonHead
		{
			enum {uri = (6 << 8) | protocol::IMDB_SVID};
			
			PCS_GetBuddyChanelListRes(){}
			
			vector<CObjList1> m_vChanel;
			
			virtual void marshal(sox::Pack &p) const 
			{
				PCS_CommonHead::marshal(p);
				marshal_container(p, m_vChanel);
			}
			
			virtual void unmarshal(const sox::Unpack &p) 
			{
				PCS_CommonHead::unmarshal(p);
				unmarshal_container(p, std::back_inserter(m_vChanel));
			}
			
		};

		

		/////////////////////////////////////////////////////////////
		struct 	PCS_GetBlockListRes : public PCS_CommonHead
		{
			enum {uri = (7 << 8) | protocol::IMDB_SVID};
			PCS_GetBlockListRes(){}
			
			vector<uint32_t> m_vBlockList;
			
			virtual void marshal(sox::Pack &p) const 
			{
				PCS_CommonHead::marshal(p);
				marshal_container(p, m_vBlockList);
			}
			
			virtual void unmarshal(const sox::Unpack &p) 
			{
				PCS_CommonHead::unmarshal(p);
				unmarshal_container(p, std::back_inserter(m_vBlockList));

			}
			
		};
		
		struct 	PCS_GetBlockList : public sox::Marshallable 
		{
			enum {uri = (8 << 8) | protocol::IMDB_SVID};
			
			PCS_GetBlockList(){}
			virtual void marshal(sox::Pack &p) const 
			{
			}
			
			virtual void unmarshal(const sox::Unpack &p) 
			{
			}
			
		};
		

		
		
		
		
		
		
		
		
		/////////////////////////////////////////////////////////////

		struct 	PCS_GetBlackListRes : public PCS_CommonHead
		{
			enum {uri = (9 << 8) | protocol::IMDB_SVID};
			PCS_GetBlackListRes(){}

			vector<uint32_t> m_vBlackList;
			
			virtual void marshal(sox::Pack &p) const 
			{
				PCS_CommonHead::marshal(p);
				marshal_container(p, m_vBlackList);
			}
			
			virtual void unmarshal(const sox::Unpack &p) 
			{
				PCS_CommonHead::unmarshal(p);
				unmarshal_container(p, std::back_inserter(m_vBlackList));
				
			}
			
		};
		
		struct 	PCS_GetBlackList : public sox::Marshallable 
		{
			enum {uri = (10 << 8) | protocol::IMDB_SVID};
			
			PCS_GetBlackList(){}
			virtual void marshal(sox::Pack &p) const 
			{
			}
			
			virtual void unmarshal(const sox::Unpack &p) 
			{
			}
			
		};


		struct 	PCS_GetBuddyHeadPhotoList : public sox::Marshallable 
		{
			enum {uri = (11 << 8) | protocol::IMDB_SVID};
			PCS_GetBuddyHeadPhotoList(){}
			
			virtual void marshal(sox::Pack &p) const 
			{
				
			}
			
			virtual void unmarshal(const sox::Unpack &p) 
			{
				
			}
			
		};

		struct 	PCS_GetBuddyHeadPhotoListRes : public sox::Marshallable 
		{
			enum {uri = (12 << 8) | protocol::IMDB_SVID};

			PCS_GetBuddyHeadPhotoListRes(){}
			
			vector<CObjList1> m_vHeadPhoto;//锟斤拷锟斤拷头锟斤拷锟叫憋拷
			vector<CObjList2> m_vCustomHeadPhotoPath;			//add by myf 鑷畾涔夊ご�?
			uint32_t resCode;
			
			virtual void marshal(sox::Pack &p) const 
			{
				p << resCode;
				marshal_container(p, m_vHeadPhoto);
				marshal_container(p, m_vCustomHeadPhotoPath);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> resCode;
				unmarshal_container(p, std::back_inserter(m_vHeadPhoto));
				unmarshal_container(p, std::back_inserter(m_vCustomHeadPhotoPath));
			}
			
		};

// 		uint32_t imid;
// 		std::vector<SBuddyListItem> buddyiteminfo_vec; //info of buddies of this 'uid'
// 		std::vector<Sfolderinfo> folderiteminfo_vec;
// 		
// 		virtual void marshal(sox::Pack &p) const 
// 		{
// 			//p.push_uint8(loginflag);
// 			p.push_uint32(imid);
// 			marshal_container(p, buddyiteminfo_vec);
// 			marshal_container(p, folderiteminfo_vec);
// 		}
// 		virtual void unmarshal(const sox::Unpack &p) 
// 		{
// 			//loginflag = (server::im::eLOGINTIMES)p.pop_uint8();
// 			imid = p.pop_uint32();
// 			unmarshal_container(p, std::back_inserter(buddyiteminfo_vec));
// 			unmarshal_container(p, std::back_inserter(folderiteminfo_vec));
// 			}

	
		struct 		PCS_PresenceMyNickChange : public sox::Marshallable 
		{
			enum {uri = (13 << 8) | protocol::IMDB_SVID};
			PCS_PresenceMyNickChange(){}
			PCS_PresenceMyNickChange(uint32_t uSendId,const string& strNick)
			{
				m_strNick=strNick;
				m_uSenderId=uSendId;
			}
			string m_strNick;
			uint32_t m_uSenderId;
			
			virtual void marshal(sox::Pack &p) const 
			{
				p << m_strNick<<m_uSenderId;
			}
			
			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_strNick>>m_uSenderId;
			}
		};

		struct 		PCS_IMLoginRes : public PCS_CommonHead
		{
			enum {uri = (15 << 8) | protocol::IMDB_SVID};
			PCS_IMLoginRes()
			{
				m_uImid=(uint32_t)(-1);
			}
			
			PCS_IMLoginRes(uint32_t uImid)
			{
				m_uImid=uImid;
			}

			uint32_t m_uImid;
			
			virtual void marshal(sox::Pack &p) const 
			{
				PCS_CommonHead::marshal(p);
				p << m_uImid;
				
			}
			
			virtual void unmarshal(const sox::Unpack &p) 
			{
				PCS_CommonHead::unmarshal(p);
				p >> m_uImid;
			}
		};
		
		
		struct 		PCS_IMLogin : public sox::Marshallable 
		{
			enum {uri = (17 << 8) | protocol::IMDB_SVID};
			PCS_IMLogin(){}
			
			virtual void marshal(sox::Pack &p) const 
			{
			}
			
			virtual void unmarshal(const sox::Unpack &p) 
			{
			}
		};
	
		struct 		PCS_AfterUpdateMyNick : public sox::Marshallable 
		{
			enum {uri = (14 << 8) | protocol::IMDB_SVID};
			PCS_AfterUpdateMyNick(){}
			PCS_AfterUpdateMyNick(const string& strNick)
			{
				m_strNick=strNick;
			}
			string m_strNick;
			
			virtual void marshal(sox::Pack &p) const 
			{
				p << m_strNick;
			}
			
			virtual void unmarshal(const sox::Unpack &p) 
			{
					p >> m_strNick;
			}
		};
		


	
		
/////////////////////
		struct 		PCS_InviteBuddyToChanel_2C : public sox::Marshallable 
		{
			enum {uri = (19 << 8) | protocol::IMDB_SVID};
			PCS_InviteBuddyToChanel_2C(){}
			PCS_InviteBuddyToChanel_2C(uint32_t uBid,uint32_t uChanelId,const string& strData)
			{
				m_uBid=uBid;
				m_uChanelId=uChanelId;
				m_strData=strData;
			}
			
			uint32_t m_uBid; //閿熸枻鎷烽敓鏂ゆ嫹閿熷壙锝忔嫹閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷稩D閿熸枻鎷?
			uint32_t m_uChanelId;
			string m_strData;
			
			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uBid<<m_uChanelId<<m_strData;

			}
			
			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uBid>>m_uChanelId>>m_strData;
			}
		};


		struct 		PCS_InviteBuddyToChanel : public sox::Marshallable 
		{
			enum {uri = (20 << 8) | protocol::IMDB_SVID};
					PCS_InviteBuddyToChanel(){}
					PCS_InviteBuddyToChanel(uint32_t uBid,uint32_t uChanelId,const string& strData)
			{
				m_uBid=uBid;
				m_uChanelId=uChanelId;
				m_strData=strData;
			}

			uint32_t m_uBid; //
			uint32_t m_uChanelId;
			string m_strData;

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uBid<<m_uChanelId<<m_strData;
			}
			
			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uBid>>m_uChanelId>>m_strData;
			}
		};


		struct 		PCS_InviteBuddyToChanelRes : public PCS_CommonHead
		{
			enum {uri = (21 << 8) | protocol::IMDB_SVID};
			PCS_InviteBuddyToChanelRes(){}
			PCS_InviteBuddyToChanelRes(uint32_t uBid,uint32_t uChanelId,const string& strData)
			{
				m_uBid=uBid;
				m_uChanelId=uChanelId;
				m_strData=strData;
			}
			
			uint32_t m_uBid; //
			uint32_t m_uChanelId;
			string m_strData;
			
			virtual void marshal(sox::Pack &p) const 
			{
				PCS_CommonHead::marshal(p);
				p << m_uBid<<m_uChanelId<<m_strData	;
			}
			
			virtual void unmarshal(const sox::Unpack &p) 
			{
				PCS_CommonHead::unmarshal(p);
				p >> m_uBid>>m_uChanelId>>m_strData;
			}
		};


		struct PCS_GetMyCeLueRes : public PCS_CommonHead
		{
			enum {uri = (22 << 8) | protocol::IMDB_SVID};
			PCS_GetMyCeLueRes()
			{
				m_uCeLue=CHECKBYBUDDY;
				m_uJiFen=0;
				m_strAnswer="";
				m_strQuestion="";
				m_bIsCheck=false;
			}
			PCS_GetMyCeLueRes(uint32_t uCeLue,uint32_t uJiFen,const string& strQuestion,const string &strAnswer)
			{
				m_uCeLue=uCeLue;
				m_strAnswer=strAnswer;
				m_strQuestion=strQuestion;
				m_uJiFen=uJiFen;
				m_bIsCheck=false;
			}

			PCS_GetMyCeLueRes(uint32_t uCeLue,uint32_t uJiFen,const string& strQuestion,const string &strAnswer,bool bIsCheck)
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
		
		struct PCS_GetMyCeLue : public sox::Marshallable 
		{
			enum {uri = (23 << 8) | protocol::IMDB_SVID};
			PCS_GetMyCeLue(){}
			virtual void marshal(sox::Pack &p) const 
			{
			}
			
			virtual void unmarshal(const sox::Unpack &p) 
			{
			}
		};
		

		struct PCS_UpdateMyImPresence  : public sox::Marshallable 
		{
			enum {uri = (24 << 8) | protocol::IMDB_SVID};
			PCS_UpdateMyImPresence(){}
			PCS_UpdateMyImPresence(E_PRESENCE_TYPE uTypeId,uint32_t uXuhao) 
			{
				m_uXuHao=uXuhao;
				m_uTypeId=uTypeId;
				
			}
			
			PCS_UpdateMyImPresence(E_PRESENCE_TYPE uTypeId,uint32_t uXuhao,const string& strData) 
			{
				m_uXuHao=uXuhao;
				m_uTypeId=uTypeId;
				m_strData=strData;

			}

			E_PRESENCE_TYPE m_uTypeId;	//1閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓鏂ゆ嫹棰戦敓鏂ゆ嫹�?閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷锋垙閿熸枻鎷?閿熸枻鎷烽敓鏂ゆ嫹閿熺殕纭锋嫹澶撮敓鏂ゆ�?
			uint32_t m_uXuHao;//

			string m_strData;//锟斤拷锟斤拷锟斤拷锟斤拷
			
			
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uXuHao;
				p.push_uint8(m_uTypeId);
				p<<m_strData;
			}
			
			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >>m_uXuHao;
				m_uTypeId=(E_PRESENCE_TYPE)p.pop_uint8();
				p>>m_strData;
			}
		};

		

		struct PCS_UpdateMyImPresenceRes  : public PCS_CommonHead
		{
			enum {uri = (25 << 8) | protocol::IMDB_SVID};
			PCS_UpdateMyImPresenceRes(){}
			PCS_UpdateMyImPresenceRes(E_PRESENCE_TYPE uTypeId,uint32_t uXuhao) 
			{
				m_uXuHao=uXuhao;
				m_uTypeId=uTypeId;
				
			}
			
			PCS_UpdateMyImPresenceRes(E_PRESENCE_TYPE uTypeId,uint32_t uXuhao,const string& strData) 
			{
				m_uXuHao=uXuhao;
				m_uTypeId=uTypeId;
				m_strData=strData;

			}

			E_PRESENCE_TYPE m_uTypeId;	//1閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓鏂ゆ嫹棰戦敓鏂ゆ嫹�?閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷锋垙閿熸枻鎷?閿熸枻鎷烽敓鏂ゆ嫹閿熺殕纭锋嫹澶撮敓鏂ゆ�?
			uint32_t m_uXuHao;//

			string m_strData;
			
			
			virtual void marshal(sox::Pack &p) const
			{
				PCS_CommonHead::marshal(p);
				p << m_uXuHao;
				p.push_uint8(m_uTypeId);
				p<<m_strData;
			}
			
			virtual void unmarshal(const sox::Unpack &p) 
			{
				PCS_CommonHead::unmarshal(p);
				p >>m_uXuHao;
				m_uTypeId=(E_PRESENCE_TYPE)p.pop_uint8();
				p>>m_strData;

			}
		};

		struct PCS_PresenceChange  : public sox::Marshallable 
		{
			enum {uri = (26 << 8) | protocol::IMDB_SVID};
			PCS_PresenceChange(){}
			PCS_PresenceChange(E_PRESENCE_TYPE uTypeId,uint32_t uXuhao ,uint32_t uSendId) 
			{
				m_uXuHao=uXuhao;
				m_uTypeId=uTypeId;
				m_uSendId=uSendId;
			}
			
			PCS_PresenceChange(E_PRESENCE_TYPE uTypeId,uint32_t uXuhao ,uint32_t uSendId,const string& strData) 
			{
				m_uXuHao=uXuhao;
				m_uTypeId=uTypeId;
				m_uSendId=uSendId;
				m_strData=strData;
			}

			E_PRESENCE_TYPE m_uTypeId;	//1閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓鏂ゆ嫹棰戦敓鏂ゆ嫹�?閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷锋垙閿熸枻鎷?閿熸枻鎷烽敓鏂ゆ嫹閿熺殕纭锋嫹澶撮敓鏂ゆ�?
			uint32_t m_uXuHao;//
			uint32_t m_uSendId;//閿熸枻鎷烽敓鏂ゆ嫹閿熸枻�?

			string m_strData;
			
			
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uXuHao<<m_uSendId;
				p.push_uint8(m_uTypeId);
				p<<m_strData;
			}
			
			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >>m_uXuHao>>m_uSendId;
				 m_uTypeId=(E_PRESENCE_TYPE)p.pop_uint8();
				 p>>m_strData;
			}
		};


	


		struct PCS_UpdateMyCeLue  : public sox::Marshallable 
		{
			enum {uri = (27 << 8) | protocol::IMDB_SVID};
			PCS_UpdateMyCeLue()
			{
				m_uCeLue=CHECKBYBUDDY;
				m_uJiFen=0;
				m_strAnswer="";
				m_strQuestion="";
				
				
			}
			PCS_UpdateMyCeLue(uint32_t uCeLue,uint32_t uJiFen,const string& strQuestion,const string &strAnswer)
			{
				
				m_uCeLue=uCeLue;
				m_strAnswer=strAnswer;
				m_strQuestion=strQuestion;
				m_uJiFen=uJiFen;
				
			}
			
			
			uint32_t m_uCeLue;
			string m_strQuestion;
			string m_strAnswer;
			uint32_t m_uJiFen;
			
			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uCeLue<<m_uJiFen<<m_strAnswer<<m_strQuestion;
			}
			
			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uCeLue>>m_uJiFen>>m_strAnswer>>m_strQuestion;
			}
		};

		struct PCS_UpdateMyCeLueRes  : public PCS_CommonHead
		{
			enum {uri = (28 << 8) | protocol::IMDB_SVID};
			PCS_UpdateMyCeLueRes(){}
			PCS_UpdateMyCeLueRes(uint32_t uCeLue,uint32_t uJiFen,const string& strQuestion,const string &strAnswer)
			{
				
				m_uCeLue=uCeLue;
				m_strAnswer=strAnswer;
				m_strQuestion=strQuestion;
				m_uJiFen=uJiFen;
				
			}
			
			uint32_t m_uCeLue;
			string m_strQuestion;
			string m_strAnswer;
			uint32_t m_uJiFen;
			
			virtual void marshal(sox::Pack &p) const 
			{
				PCS_CommonHead::marshal(p);
				p << m_uCeLue<<m_uJiFen<<m_strAnswer<<m_strQuestion;
			}
			
			virtual void unmarshal(const sox::Unpack &p) 
			{
				PCS_CommonHead::unmarshal(p);
				p >> m_uCeLue>>m_uJiFen>>m_strAnswer>>m_strQuestion;
			}
		};


		struct PCS_NeedCode  : public sox::Marshallable {
			enum {uri = (29 << 8) | protocol::IMDB_SVID};
			PCS_NeedCode(){}
			PCS_NeedCode(string strCode) 
			{
				m_strCode=strCode;
			}
			
			string m_strCode;
			
			virtual void marshal(sox::Pack &p) const
			{
				p << m_strCode;
			}
			
			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_strCode;
			}
		};

		struct PCS_SearchBuddyByCode  : public sox::Marshallable {
			enum {uri = (30 << 8) | protocol::IMDB_SVID};
			PCS_SearchBuddyByCode(){}
			PCS_SearchBuddyByCode(uint32_t id,string strCode) 
			{
				imid=id;
				m_strCode=strCode;
			}

			uint32_t imid;
			string m_strCode;
			
			virtual void marshal(sox::Pack &p) const
			{
				p << imid<<m_strCode;
			}
			
			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> imid>>m_strCode;
			}
		};

		struct PCS_GetBuddyCeLueRes : public PCS_CommonHead
		{
			enum {uri = (31 << 8) | protocol::IMDB_SVID};
			PCS_GetBuddyCeLueRes(){}
			PCS_GetBuddyCeLueRes(uint32_t uBid,uint32_t uCeLue,uint32_t uQuestion,const string& strQuestion)
			{
				m_uBid=uBid;
				m_uCeLue=uCeLue;
				m_uQuestionId=uQuestion;
				m_strQuestion=strQuestion;
				m_bIsCheck=false;
				
			}

			PCS_GetBuddyCeLueRes(uint32_t uBid,uint32_t uCeLue,uint32_t uQuestion,const string& strQuestion,bool bIsCheck)
			{
				m_uBid=uBid;
				m_uCeLue=uCeLue;
				m_uQuestionId=uQuestion;
				m_strQuestion=strQuestion;
				m_bIsCheck=bIsCheck;
			}

			uint32_t m_uBid; //
			uint32_t m_uCeLue;
			uint32_t m_uQuestionId;
			string m_strQuestion;
			bool m_bIsCheck;
			
			virtual void marshal(sox::Pack &p) const 
			{
				PCS_CommonHead::marshal(p);
				p << m_uBid<<m_uCeLue<<m_uQuestionId<<m_strQuestion;
				p.push_uint8(m_bIsCheck);
			}
			
			virtual void unmarshal(const sox::Unpack &p) 
			{
				PCS_CommonHead::unmarshal(p);
				p >> m_uBid>>m_uCeLue>>m_uQuestionId>>m_strQuestion;
				m_bIsCheck = (p.pop_uint8() != 0 ? true : false);
			}
		};
		
		struct PCS_GetBuddyCeLue : public sox::Marshallable 
		{
			enum {uri = (32 << 8) | protocol::IMDB_SVID};
			PCS_GetBuddyCeLue(){}
			PCS_GetBuddyCeLue(uint32_t uBid)
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

		
		struct PCS_ReqAddBuddyByJiFen : public sox::Marshallable 
		{
			enum {uri = (33 << 8) | protocol::IMDB_SVID};
			PCS_ReqAddBuddyByJiFen(){}
			PCS_ReqAddBuddyByJiFen(uint32_t uBid,uint32_t uGid)
			{
				m_uBid=uBid;
				m_uGid=uGid;
				
			}
			uint32_t m_uBid; //
			uint32_t m_uGid;

			
			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uBid<<m_uGid;
			}
			
			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uBid>>m_uGid;
			}
		};

		struct PCS_ReqAddBuddyByJiFenRes : public PCS_CommonHead
		{
			enum {uri = (34 << 8) | protocol::IMDB_SVID};
			PCS_ReqAddBuddyByJiFenRes(){}
			PCS_ReqAddBuddyByJiFenRes(uint32_t uBid,uint32_t uJifen)
			{
				m_uBid=uBid;
				m_uJiFen=uJifen;
			}
			uint32_t m_uBid; // 
			uint32_t m_uJiFen;
			
			virtual void marshal(sox::Pack &p) const 
			{
				PCS_CommonHead::marshal(p);
				p << m_uBid<<m_uJiFen;
			}
			
			virtual void unmarshal(const sox::Unpack &p) 
			{
				PCS_CommonHead::unmarshal(p);
				p >> m_uBid>>m_uJiFen;
			}
		};

		struct PCS_BuddyQuestion : public sox::Marshallable 
		{
			enum {uri = (35 << 8) | protocol::IMDB_SVID};
			PCS_BuddyQuestion(){}
			PCS_BuddyQuestion(uint32_t uBid,uint32_t uQuestion,const string& strQuestion)
			{
				m_uBid=uBid;
				m_uQuestionId=uQuestion;
				m_strQuestion=strQuestion;
			}
			uint32_t m_uBid; // 
			uint32_t m_uQuestionId;
			string m_strQuestion;
			
			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uBid<<m_uQuestionId<<m_strQuestion;
			}
			
			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uBid>>m_uQuestionId>>m_strQuestion;
			}
		};


		struct PCS_AnswerBuddyQuestion : public sox::Marshallable 
		{
			enum {uri = (36 << 8) | protocol::IMDB_SVID};
			PCS_AnswerBuddyQuestion(){}
			PCS_AnswerBuddyQuestion(uint32_t uBid,uint32_t uGid,uint32_t uQuestion,const string &strAnswer)
			{
				 m_uBid=uBid;
				 m_uQuestionId=uQuestion;
				 m_strAnswer=strAnswer;
				 m_uGid=uGid;
			}
			uint32_t m_uBid; // 
			uint32_t m_uQuestionId;
			string m_strAnswer;
			uint32_t m_uGid;
			
			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uBid<<m_uGid<<m_uQuestionId<<m_strAnswer;
			}
			
			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uBid>>m_uGid>>m_uQuestionId>>m_strAnswer;
			}
		};

		struct PCS_AnswerBuddyQuestionRes : public PCS_CommonHead
		{
			enum {uri = (37 << 8) | protocol::IMDB_SVID};
			PCS_AnswerBuddyQuestionRes(){}
			PCS_AnswerBuddyQuestionRes(uint32_t uBid)
			{
				m_uBid=uBid;
			
			}
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


struct PCS_DelFrBlackList : public sox::Marshallable 
{
	enum {uri = (38 << 8) | protocol::IMDB_SVID};
	PCS_DelFrBlackList(){}
	PCS_DelFrBlackList(uint32_t uBid)
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


struct PCS_DelFrBlackListRes : public PCS_CommonHead
{
	enum {uri = (39 << 8) | protocol::IMDB_SVID};
	PCS_DelFrBlackListRes(){}
	PCS_DelFrBlackListRes(uint32_t uBid)
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


struct PCS_AddToBlackList : public sox::Marshallable 
{
	enum {uri = (40 << 8) | protocol::IMDB_SVID};
	PCS_AddToBlackList(){}
	PCS_AddToBlackList(uint32_t uBid)
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


struct PCS_AddToBlackListRes : public PCS_CommonHead
{
	enum {uri = (41 << 8) | protocol::IMDB_SVID};
	PCS_AddToBlackListRes(){}
	PCS_AddToBlackListRes(uint32_t uBid)
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


struct PCS_AddToBlockList : public sox::Marshallable 
{
	enum {uri = (42 << 8) | protocol::IMDB_SVID};
	PCS_AddToBlockList(){}
	PCS_AddToBlockList(uint32_t uBid)
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


struct PCS_AddToBlockListRes : public PCS_CommonHead
{
	enum {uri = (43 << 8) | protocol::IMDB_SVID};
	PCS_AddToBlockListRes(){}
	PCS_AddToBlockListRes(uint32_t uBid)
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


struct PCS_DelFrBlockList : public sox::Marshallable 
{
	enum {uri = (44 << 8) | protocol::IMDB_SVID};
	PCS_DelFrBlockList(){}
	PCS_DelFrBlockList(uint32_t uBid)
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


struct PCS_DelFrBlockListRes : public PCS_CommonHead
{
	enum {uri = (45 << 8) | protocol::IMDB_SVID};
	PCS_DelFrBlockListRes(){}
	PCS_DelFrBlockListRes(uint32_t uBid)
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

//wuji end

		struct SBuddyListItem : public sox::Marshallable 
		{//wuji 锟斤拷锟斤拷没锟斤拷锟斤拷锟捷憋拷锟斤拷应锟斤拷

			uint32_t	buddyId;
			std::string	remark;			//锟斤拷注

			std::string nickname;			//锟角筹拷
			uint32_t	rank;
			std::string	sign;
			uint32_t pid;//锟斤拷ID
			protocol::uinfo::SEX sex;
			protocol::E_IMSTATUS stat;//鐘舵�?
			uint32_t version;
			uint32_t imid;

			virtual void marshal(sox::Pack &p) const {
				p << buddyId << remark << nickname << rank << sign << pid << version << imid;
				p.push_uint8(sex);
				p.push_uint8(stat);
			}
			virtual void unmarshal(const sox::Unpack &p) {
				p >> buddyId >> remark >> nickname >> rank >> sign >> pid >> version >> imid;
				sex = (uinfo::SEX)p.pop_uint8();
				stat = (protocol::E_IMSTATUS)p.pop_uint8();
			}
		};

		struct Sfolderinfo : public sox::Marshallable {
			Sfolderinfo(){}
			Sfolderinfo(uint32_t fid, const std::string& mark)
				: folderid(fid), remark(mark) {}

				uint32_t folderid;//锟斤拷锟斤拷ID
				std::string remark;//锟斤拷锟斤拷锟斤拷锟斤拷

				virtual void marshal(sox::Pack &p) const {
					p << folderid << remark;
				}
				virtual void unmarshal(const sox::Unpack &p) {
					p >> folderid >> remark;
				}
		};

		struct SUserinfo : public sox::Marshallable
		{
			uint32_t bid;              // uid
			std::string nickname;      // �ǳ�
			uint32_t	rank;          // ����
			std::string	sign;          // ǩ��
			uint32_t version;          // ��ʱû�ô�
			protocol::uinfo::SEX sex;
			protocol::E_IMSTATUS stat; // ����״̬

			SUserinfo()
			{
				bid     = (uint32_t)-1;
				rank    = 0;
				version = (uint32_t)-1;
				sex = protocol::uinfo::male;
				stat=IMOFFLINE;
			}

			SUserinfo(uint32_t uUid)
			{
				bid = uUid;
				rank    = 0;//绉垎缂虹渷�?
				version = (uint32_t)-1;
				sex = protocol::uinfo::male;
				stat=IMOFFLINE;
			}

			virtual void marshal(sox::Pack &p) const {
				p << bid << nickname << rank << sign << version;
				p.push_uint8(sex);
				p.push_uint8(stat);
			}
			virtual void unmarshal(const sox::Unpack &p) {
				p >> bid >> nickname >> rank >> sign >> version;
				sex = (protocol::uinfo::SEX)p.pop_uint8();
				stat = (protocol::E_IMSTATUS)p.pop_uint8();
			}
		};

		struct SWaitCheckBuddy : public sox::Marshallable {
			SWaitCheckBuddy(){}
			SWaitCheckBuddy(uint32_t id, const std::string& nickname, const std::string& msg) 
				: bid(id), nick(nickname), left_msg(msg) {}
				uint32_t bid;
				std::string nick;
				std::string left_msg;

				virtual void marshal(sox::Pack &p) const {
					p << bid << nick << left_msg;
				}
				virtual void unmarshal(const sox::Unpack &p) {
					p >> bid >> nick >> left_msg;
				}
		};


		//protocol
		struct PCS_SyncBuddyList : public sox::Voidmable {
			enum { uri = (46 << 8) | protocol::IMDB_SVID };
		};

		struct PCS_SyncBuddyListRes : public PCS_CommonHead
		{
			enum {uri = (47 << 8) | protocol::IMDB_SVID};
			PCS_SyncBuddyListRes(){}

			//server::im::eLOGINTIMES loginflag;
			//uint32_t imid;
			std::vector<SBuddyListItem> buddyiteminfo_vec; //info of buddies of this 'uid'
			std::vector<Sfolderinfo> folderiteminfo_vec;

			virtual void marshal(sox::Pack &p) const 
			{
				//p.push_uint8(loginflag);
			//	p.push_uint32(imid);
				PCS_CommonHead::marshal(p);
				marshal_container(p, buddyiteminfo_vec);
				marshal_container(p, folderiteminfo_vec);
			}
			virtual void unmarshal(const sox::Unpack &p) 
			{
				//loginflag = (server::im::eLOGINTIMES)p.pop_uint8();
			//	imid = p.pop_uint32();
				PCS_CommonHead::unmarshal(p);
				unmarshal_container(p, std::back_inserter(buddyiteminfo_vec));
				unmarshal_container(p, std::back_inserter(folderiteminfo_vec));
			}
		};

		//notified by imd
		struct PCS_BuddyChangedInfo : public sox::Marshallable {
			enum { uri = (48 << 8) | protocol::IMDB_SVID };
			PCS_BuddyChangedInfo(){}
			PCS_BuddyChangedInfo(const SUserinfo& info) : buddyinfo(info){
			}

			SUserinfo buddyinfo; //just include following info maybe changed: buddyId, nickname, rank, sign, sex;

			virtual void marshal(sox::Pack &p) const{
				p << buddyinfo;
			}

			virtual void unmarshal(const sox::Unpack &p){
				p >> buddyinfo;
			}
		};

		//client notify im
		struct PCS_BuddyStatusChange_toServer : public sox::Marshallable {
			enum { uri = (49 << 8) | protocol::IMDB_SVID };
			PCS_BuddyStatusChange_toServer(){}
			PCS_BuddyStatusChange_toServer(protocol::E_IMSTATUS st) 
				: status(st){
				}

				protocol::E_IMSTATUS status;

				virtual void marshal(sox::Pack &p) const{
					p.push_uint8(status);
				}

				virtual void unmarshal(const sox::Unpack &p){
					status = (protocol::E_IMSTATUS)p.pop_uint8();
				}
		};

		//server notify client
		struct PCS_BuddyStatusChange_toClinet : public sox::Marshallable 
		{
			enum { uri = (50 << 8) | protocol::IMDB_SVID };
			PCS_BuddyStatusChange_toClinet(){ m_uVersion = 0; }
			PCS_BuddyStatusChange_toClinet(uint32_t u, protocol::E_IMSTATUS st) 
				: uid(u), status(st), m_uVersion(0)
			{
			}
			PCS_BuddyStatusChange_toClinet(uint32_t u, protocol::E_IMSTATUS st, uint32_t uVersion) 
				: uid(u), status(st), m_uVersion(uVersion)
			{
			}

			uint32_t uid;
			protocol::E_IMSTATUS status;
			uint32_t m_uVersion;            // ׷�Ӱ汾��Ϣ�ֶ�

			virtual void marshal(sox::Pack &p) const
			{
				p << uid;
				p.push_uint8(status);
				p << m_uVersion;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> uid;
				status = (protocol::E_IMSTATUS)p.pop_uint8();
				if (p.size() != 0)
				{

					try
					{
						p >> m_uVersion;
					}
					catch(...)
					{
						m_uVersion = 0;
					}
				}
				else
				{
					m_uVersion = 0;
				}
			}
		};


		struct PCS_AddBuddy : public sox::Marshallable
		{
			enum { uri = (51 << 8) | protocol::IMDB_SVID };
			PCS_AddBuddy()
			{
				m_uOnlineVersion = 0;
			}
			PCS_AddBuddy(uint32_t b, uint32_t p, SUserinfo& info,uint32_t uHeadPhoto,uint32_t uChanelId,uint32_t uGameId, const std::string &strCustomHeadPhoto = "") 
				: bid(b), pid(p), binfo(info), m_strCustomHeadPhoto(strCustomHeadPhoto)
			{
				m_uHeadPhoto=uHeadPhoto;//0锟斤拷锟斤拷没锟斤拷头锟斤拷
				m_uChanelId=uChanelId;//锟斤拷锟斤拷-1锟斤拷锟斤拷没锟斤拷锟斤拷频锟斤拷锟?1也锟斤拷锟斤拷使锟斤拷锟睫凤拷锟斤拷锟斤拷锟斤拷锟芥�?
				m_uGameId=uGameId;//锟斤拷锟斤拷-1锟斤拷锟斤拷没锟斤拷锟斤拷锟斤拷戏锟斤拷-1也锟斤拷锟斤拷使锟斤拷锟睫凤拷锟斤拷锟斤拷锟斤拷锟芥�?

				m_uOnlineVersion = 0;
			}

			PCS_AddBuddy(uint32_t b, uint32_t p, SUserinfo& info,uint32_t uHeadPhoto,uint32_t uChanelId,uint32_t uGameId,string strData, const std::string &strCustomHeadPhoto = "",const string& strGameData="",uint32_t uImid=-1) 
				: bid(b), pid(p), binfo(info), m_strCustomHeadPhoto(strCustomHeadPhoto)
			{
				m_uHeadPhoto=uHeadPhoto;//0锟斤拷锟斤拷没锟斤拷头锟斤拷
				m_uChanelId=uChanelId;//锟斤拷锟斤拷-1锟斤拷锟斤拷没锟斤拷锟斤拷频锟斤拷锟?1也锟斤拷锟斤拷使锟斤拷锟睫凤拷锟斤拷锟斤拷锟斤拷锟芥�?
				m_uGameId=uGameId;//锟斤拷锟斤拷-1锟斤拷锟斤拷没锟斤拷锟斤拷锟斤拷戏锟斤拷-1也锟斤拷锟斤拷使锟斤拷锟睫凤拷锟斤拷锟斤拷锟斤拷锟芥�?
				m_strData=strData;
				m_strGameData=strGameData;
				m_uImid=uImid;

				m_uOnlineVersion = 0;
			}

			PCS_AddBuddy(uint32_t b, uint32_t p, SUserinfo& info,uint32_t uHeadPhoto,uint32_t uChanelId,uint32_t uGameId,string strData, const std::string &strCustomHeadPhoto,const string& strGameData,uint32_t uImid,const string& strRemark) 
				: bid(b), pid(p), binfo(info), m_strCustomHeadPhoto(strCustomHeadPhoto)
			{
				m_uHeadPhoto=uHeadPhoto;//0锟斤拷锟斤拷没锟斤拷头锟斤拷
				m_uChanelId=uChanelId;//锟斤拷锟斤拷-1锟斤拷锟斤拷没锟斤拷锟斤拷频锟斤拷锟?1也锟斤拷锟斤拷使锟斤拷锟睫凤拷锟斤拷锟斤拷锟斤拷锟芥�?
				m_uGameId=uGameId;//锟斤拷锟斤拷-1锟斤拷锟斤拷没锟斤拷锟斤拷锟斤拷戏锟斤拷-1也锟斤拷锟斤拷使锟斤拷锟睫凤拷锟斤拷锟斤拷锟斤拷锟芥�?
				m_strData=strData;
				m_strGameData=strGameData;
				m_uImid=uImid;
				m_strRemark=strRemark;
				m_uOnlineVersion = 0;
			}


			uint32_t bid;
			uint32_t pid;
			SUserinfo binfo;
			uint32_t m_uHeadPhoto;//0锟斤拷锟斤拷没锟斤拷头锟斤拷
			uint32_t m_uChanelId;//锟斤拷锟斤拷-1锟斤拷锟斤拷没锟斤拷锟斤拷频锟斤拷锟?1也锟斤拷锟斤拷使锟斤拷锟睫凤拷锟斤拷锟斤拷锟斤拷锟芥�?
			uint32_t m_uGameId;//锟斤拷锟斤拷-1锟斤拷锟斤拷没锟斤拷锟斤拷锟斤拷戏锟斤拷-1也锟斤拷锟斤拷使锟斤拷锟睫凤拷锟斤拷锟斤拷锟斤拷锟芥�?
			string m_strData;//主要用于频道的额外数�?
			string m_strCustomHeadPhoto;	//自定义头�?
			string m_strGameData;//主要用于游戏的额外数�?
			uint32_t m_uImid;

			//im2.2加入的功�?
			string m_strRemark;

			uint32_t m_uOnlineVersion;             // Я�����û���½���ͣ�PC / Mobile)��Ϣ



			virtual void marshal(sox::Pack &p) const 
			{
				p << bid << pid << binfo<<m_uHeadPhoto<<m_uChanelId<<m_uGameId <<m_strData<< m_strCustomHeadPhoto<<m_strGameData<<m_uImid;
				p<< m_strRemark << m_uOnlineVersion;
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> bid >> pid >> binfo>>m_uHeadPhoto>>m_uChanelId>>m_uGameId >>m_strData>> m_strCustomHeadPhoto>>m_strGameData>>m_uImid;
				p>>m_strRemark;

				if (p.size() != 0)
				{
					try
					{
						p >> m_uOnlineVersion;
					}
					catch (...)
					{
						m_uOnlineVersion = 0;
					}
				}
				else
					m_uOnlineVersion = 0;
			}
		};

	

	
		struct PCS_PeerResponse : public sox::Marshallable{
			enum {uri = (52 << 8) | protocol::IMDB_SVID};
			PCS_PeerResponse(){}
			PCS_PeerResponse(uint32_t peer, std::string& msg, E_WAITCHECKLIST_OP o)
				: peerid(peer), msg_left(msg), op(o){
				}

				uint32_t peerid;
				std::string msg_left;
				E_WAITCHECKLIST_OP op;

				virtual void marshal(sox::Pack &p) const {
					p << peerid << msg_left;
					p.push_uint8(op);
				}

				virtual void unmarshal(const sox::Unpack &p) {
					p >> peerid >> msg_left;
					op = (E_WAITCHECKLIST_OP)p.pop_uint8();
				}
		};


		//protocol
		struct PCS_SyncWaitCheckList : public sox::Voidmable {
			enum { uri = (53 << 8) | protocol::IMDB_SVID };
		};

		struct PCS_SyncWaitCheckListRes : public sox::Marshallable {
			enum { uri = (54 << 8) | protocol::IMDB_SVID };
			PCS_SyncWaitCheckListRes(){}
			PCS_SyncWaitCheckListRes(std::vector<SWaitCheckBuddy> &list)
				: waitCheckListInfo(list){
				}

				std::vector<SWaitCheckBuddy> waitCheckListInfo;

				virtual void marshal(sox::Pack &p) const {
					sox::marshal_container(p, waitCheckListInfo);
				}
				virtual void unmarshal(const sox::Unpack &p) {
					sox::unmarshal_container(p, std::back_inserter(waitCheckListInfo));
				}

				//wuji start
				std::string ToString() const 
				{
					sox::PackBuffer bf;
					sox::Pack pk(bf);
					this->marshal(pk);
					return std::string(pk.data(), pk.size());
				}

				static PCS_SyncWaitCheckListRes FromString(const std::string &str) 
				{
					sox::Unpack up(str.data(), str.size());
					PCS_SyncWaitCheckListRes tc;
					tc.unmarshal(up);
					return tc;
				}
				//wuji end


		};



		struct PCS_OpWaitCheckBudRes : public sox::Marshallable {
			enum {uri = (55 << 8) | protocol::IMDB_SVID};
			PCS_OpWaitCheckBudRes(){}
			PCS_OpWaitCheckBudRes(uint32_t b) : bid(b) {}

			uint32_t bid;

			virtual void marshal(sox::Pack &p) const {
				p << bid;
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> bid;
			}
		};

		struct PCS_ReqAddMe : public sox::Marshallable{
			enum {uri = (56 << 8) | protocol::IMDB_SVID};
			PCS_ReqAddMe(){}
			PCS_ReqAddMe(uint32_t b, const std::string& msgleft, const std::string& nick)
				: bid(b), left_msg(msgleft), nickname(nick){}

				uint32_t bid; //who want add me
				std::string left_msg;
				std::string nickname;

				virtual void marshal(sox::Pack &p) const {
					p << bid << left_msg << nickname;
				}

				virtual void unmarshal(const sox::Unpack &p) {
					p >> bid >> left_msg >> nickname;
				}
		};


		struct PCS_SearchBuddyRes_real : public sox::Properties{
			enum {uri = (57 << 8) | protocol::IMDB_SVID};
			PCS_SearchBuddyRes_real(){}

			uint32_t uid;
			uint32_t resCode;
			protocol::E_IMSTATUS stat;

			virtual void marshal(sox::Pack &p) const {
				p << uid << resCode;
				p.push_uint8(stat);
				Properties::marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> uid >> resCode;
				stat = (protocol::E_IMSTATUS)p.pop_uint8();
				Properties::unmarshal(p);
			}
		};


		#define DEFAULT_GROUP_ID	1

		struct SOfflineMsg : public sox::Marshallable {
			SOfflineMsg(){}
			SOfflineMsg(uint32_t id, const std::string& msg)
				: bid(id), text(msg){}

				uint32_t bid;
				std::string text;
				//added by zzw,20090928
				//uint32_t m_uSendTime;//UTC时间

				virtual void marshal(sox::Pack &p) const {
					p << bid << text;
					//p << bid <<m_uSendTime<< text;
				}
				virtual void unmarshal(const sox::Unpack &p) {
					p >> bid >> text;
					//p >> bid>>m_uSendTime>> text;
				}
		};

		struct PCS_AddFolder : public sox::Marshallable {
			enum { uri = (58 << 8) | IMDB_SVID };
			PCS_AddFolder(){}
			PCS_AddFolder(const std::string &nick) : nickname(nick){}

			//uint32_t pid;
			std::string nickname;

			virtual void marshal(sox::Pack &p) const {
				p << nickname;
			}
			virtual void unmarshal(const sox::Unpack &p) {
				p >> nickname;
			}
		};
 
		struct PCS_AddFolderRes : public sox::Marshallable {
			enum { uri = (59 << 8) | IMDB_SVID };
			PCS_AddFolderRes(){}
			PCS_AddFolderRes(uint32_t f, std::string& nick)
				: folderId(f), nickname(nick){}

			uint32_t folderId;
			//uint32_t pid;
			std::string nickname;

			virtual void marshal(sox::Pack &p) const {
				p << folderId << nickname;
			}
			virtual void unmarshal(const sox::Unpack &p) {
				p >> folderId >> nickname;
			}
		};
		
		struct PCS_DelFolder : public sox::Marshallable {
			enum { uri = (60 << 8) | IMDB_SVID };
			PCS_DelFolder(){}
			PCS_DelFolder(uint32_t f) : folderId(f){
			}

			uint32_t folderId;

			virtual void marshal(sox::Pack &p) const {
				p << folderId;
			}
			virtual void unmarshal(const sox::Unpack &p) {
				p >> folderId;
			}
		};

		struct PCS_DelFolderRes : public sox::Marshallable {
			enum { uri = (61 << 8) | IMDB_SVID };
			PCS_DelFolderRes(){}
			PCS_DelFolderRes(uint32_t f) : folderId(f){
			}

			uint32_t folderId;

			virtual void marshal(sox::Pack &p) const {
				p << folderId;
			}
			virtual void unmarshal(const sox::Unpack &p) {
				p >> folderId;
			}
		};


		struct PCS_DelBuddy : public sox::Marshallable {
			enum { uri = (62 << 8) | IMDB_SVID };
			PCS_DelBuddy(){}
			PCS_DelBuddy(uint32_t b, uint32_t p) : bid(b){}

			uint32_t bid;

			virtual void marshal(sox::Pack &p) const {
				p << bid;
			}
			virtual void unmarshal(const sox::Unpack &p) {
				p >> bid;
			}
		};


		struct PCS_DelBuddyRes : public PCS_CommonHead {
			enum { uri = (63 << 8) | IMDB_SVID };
			PCS_DelBuddyRes(){}
			PCS_DelBuddyRes(uint32_t b) : bid(b){}

			uint32_t bid;

			virtual void marshal(sox::Pack &p) const
			{
				PCS_CommonHead::marshal(p);
				p << bid;
			}
			virtual void unmarshal(const sox::Unpack &p) 
			{
				PCS_CommonHead::unmarshal(p);
				p >> bid;
			}
		};


		struct PCS_MoveToFolder : public sox::Marshallable {
			enum { uri = (64 << 8) | IMDB_SVID };
			PCS_MoveToFolder(){}
			PCS_MoveToFolder(uint32_t bid, uint32_t from, uint32_t to)
				: who(bid), fromID(from), toID(to){
			}

			uint32_t who;
			uint32_t fromID;
			uint32_t toID;

			virtual void marshal(sox::Pack &p) const {
				p << who << fromID << toID;
			}
			virtual void unmarshal(const sox::Unpack &p) {
				p >> who >> fromID >> toID;
			}
		};		


		struct PCS_MoveToFolderRes : public sox::Marshallable {
			enum { uri = (65 << 8) | IMDB_SVID };
			PCS_MoveToFolderRes(){}
			PCS_MoveToFolderRes(uint32_t bid, uint32_t from, uint32_t to)
				: who(bid), fromID(from), toID(to){}

			uint32_t who;
			uint32_t fromID;
			uint32_t toID;

			virtual void marshal(sox::Pack &p) const {
				p << who << fromID << toID;
			}
			virtual void unmarshal(const sox::Unpack &p) {
				p >> who >> fromID >> toID;
			}
		};
		
		//to notify server
		struct PCS_ChangedRemark : public sox::Marshallable {
			PCS_ChangedRemark(){}
			PCS_ChangedRemark(E_ITEMTYPE t, uint32_t ID, const std::string& rem)
				: type(t), id(ID), remark(rem){
			}
			enum { uri = (66 << 8) | IMDB_SVID };

			protocol::im::E_ITEMTYPE type;
			uint32_t id;
			std::string remark;

			virtual void marshal(sox::Pack &p) const{
				p.push_uint8(type);
				p << id << remark;
			}

			virtual void unmarshal(const sox::Unpack &p){
				type = (E_ITEMTYPE)p.pop_uint8();
				p >> id >> remark;
			}
		};

		struct PCS_ChangedRemarkRes : public PCS_CommonHead
		{
			enum { uri = (67 << 8) | protocol::IMDB_SVID };
			PCS_ChangedRemarkRes(){}
			PCS_ChangedRemarkRes(E_ITEMTYPE t, uint32_t b, const std::string& rem)
				: type(t), id(b), remark(rem){
			}

			E_ITEMTYPE type;
			uint32_t id;
			std::string remark;

			virtual void marshal(sox::Pack &p) const
			{
				PCS_CommonHead::marshal(p);
				p.push_uint8(type);
				p << id << remark;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				PCS_CommonHead::unmarshal(p);
				type = (E_ITEMTYPE)p.pop_uint8();
				p >> id >> remark;
			}
		};
		
		
		struct PCS_GetOfflineMsg : public sox::Voidmable {
			// modified by zzw ,20090909
			//enum {uri = (68 << 8) | protocol::IMDB_SVID};
			enum {uri = (68 << 8) | protocol::IMOFFLINEMSG_SVID};
		};

		struct PCS_GetOfflineMsgRes : public sox::Marshallable {
			PCS_GetOfflineMsgRes(){}
			enum {uri = (69 << 8) | protocol::IMDB_SVID};

			std::vector<protocol::im::SOfflineMsg> msg;

			virtual void marshal(sox::Pack &p) const {
				marshal_container(p, msg);
			}
			virtual void unmarshal(const sox::Unpack &p) {
				unmarshal_container(p, std::back_inserter(msg));
			}
		};

		struct PCS_DelOfflienMsg : public sox::Voidmable {
			PCS_DelOfflienMsg(){}
			//modified by zzw , 20090909
			//enum {uri = (70 << 8) | protocol::IMDB_SVID};
			enum {uri = (70 << 8) | protocol::IMOFFLINEMSG_SVID};
		};

		struct PCS_DelOfflienMsgFailed : public sox::Voidmable {
			PCS_DelOfflienMsgFailed(){}

			enum {uri = (71 << 8) | protocol::IMDB_SVID};
		};
		
		

		struct PCS_ReqAddBuddy : public sox::Marshallable{
			enum {uri = (72 << 8) | protocol::IMDB_SVID};
			PCS_ReqAddBuddy(){}
			PCS_ReqAddBuddy(uint32_t b, uint32_t g, std::string& msg)
				: bid(b), gid(g), msg_left(msg){
			}
			//PCS_ReqAddBuddy(uint32_t b, uint32_t g)
			//	: bid(b), gid(g){
			//}

			uint32_t bid;
			uint32_t gid; //you want add the 'bid' into this 'gid'
			std::string msg_left;

			virtual void marshal(sox::Pack &p) const {
				p << bid << gid << msg_left;
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> bid >> gid >> msg_left;
			}
		};

		struct PCS_ReqAddBuddyRes : public sox::Marshallable{
			enum {uri = (73 << 8) | protocol::IMDB_SVID};
			PCS_ReqAddBuddyRes(){}
			PCS_ReqAddBuddyRes(uint32_t b) 
				: bid(b){
				}

				uint32_t bid;

				virtual void marshal(sox::Pack &p) const {
					p << bid;
				}

				virtual void unmarshal(const sox::Unpack &p) {
					p >> bid;
				}
		};

		struct PCS_GetPeerAccRef : public sox::Voidmable {
			enum {uri = (74 << 8) | protocol::IMDB_SVID};
			PCS_GetPeerAccRef(){}
		};

		struct S_Peer_Acc_Ref : public sox::Marshallable {
			S_Peer_Acc_Ref(){}
			S_Peer_Acc_Ref(uint32_t peer_id, E_WAITCHECKLIST_OP operate, std::string& msg)
				: peerid(peer_id), msg_left(msg), op(operate){}
			uint32_t peerid;
			std::string msg_left;
			E_WAITCHECKLIST_OP op;

			virtual void marshal(sox::Pack &p) const {
				p << peerid << msg_left;
				p.push_uint8(op);
			}
			virtual void unmarshal(const sox::Unpack &p) {
				p >> peerid >> msg_left;
				op = (E_WAITCHECKLIST_OP)p.pop_uint8();
			}
		};

		struct PCS_GetPeerAccRef_Res : public PCS_CommonHead
		{
			enum {uri = (75 << 8) | protocol::IMDB_SVID};
			PCS_GetPeerAccRef_Res(){}

			std::vector<S_Peer_Acc_Ref> peer_acc_ref_vec;

			virtual void marshal(sox::Pack &p) const 
			{
				PCS_CommonHead::marshal(p);
				marshal_container(p, peer_acc_ref_vec);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				PCS_CommonHead::unmarshal(p);
				unmarshal_container(p, std::back_inserter(peer_acc_ref_vec));
			}



			//wuji start
			std::string ToString() const 
			{
				sox::PackBuffer bf;
				sox::Pack pk(bf);
				this->marshal(pk);
				return std::string(pk.data(), pk.size());
			}

			static PCS_GetPeerAccRef_Res FromString(const std::string &str) 
			{
				sox::Unpack up(str.data(), str.size());
				PCS_GetPeerAccRef_Res tc;
				tc.unmarshal(up);
				return tc;
			}
			//wuji end
		};

		struct PCS_DelPeerAccRef : public sox::Voidmable {
			enum {uri = (76 << 8) | protocol::IMDB_SVID};
			PCS_DelPeerAccRef(){}
		};

		struct PCS_OpWaitCheckBud : public sox::Marshallable 
		{
			enum {uri = (77 << 8) | protocol::IMDB_SVID};
			PCS_OpWaitCheckBud(){}
			PCS_OpWaitCheckBud(uint32_t b, uint32_t p, const std::string& mesg, E_WAITCHECKLIST_OP operate) 
				: bid(b), gid(p), msg(mesg), op(operate) {}

			uint32_t bid;
			uint32_t gid;
			std::string msg;
			E_WAITCHECKLIST_OP op;

			virtual void marshal(sox::Pack &p) const 
			{
				p << bid << gid << msg;
				p.push_uint8(op);
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> bid >> gid >> msg;
				op = (E_WAITCHECKLIST_OP)p.pop_uint8();
			}
		};

		//send to uinfo
		struct PCS_SearchBuddy  : public sox::Marshallable {
			enum {uri = (78 << 8) | protocol::IMDB_SVID};
			PCS_SearchBuddy(){}
			PCS_SearchBuddy(uint32_t id) : imid(id){}
			uint32_t imid;

			virtual void marshal(sox::Pack &p) const {
				p << imid;
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> imid;
			}
		};


		struct PCS_Update_BudSt_Timely : public sox::Voidmable {
			enum {uri = (79 << 8) | protocol::IMDB_SVID};
		};

		struct Budid_St : public sox::Marshallable 
		{
			uint32_t bid;
			E_IMSTATUS st;

			Budid_St(){}
			Budid_St(uint32_t uUid, E_IMSTATUS eStats)
				: bid(uUid), st(eStats)
			{
			}

			virtual void marshal(sox::Pack &p) const {
				p << bid;
				p.push_uint8(st);
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> bid;
				st = (E_IMSTATUS)p.pop_uint8();
			}
		};

		struct PCS_Up_BudSt_Res : public sox::Marshallable {
			enum {uri = (80 << 8) | protocol::IMDB_SVID};
			PCS_Up_BudSt_Res(){}
			PCS_Up_BudSt_Res(std::vector<Budid_St>& buds) : buddies_st(buds){}

			std::vector<Budid_St> buddies_st;

			virtual void marshal(sox::Pack &p) const {
				marshal_container(p, buddies_st);
			}

			virtual void unmarshal(const sox::Unpack &p) {
				unmarshal_container(p, std::back_inserter(buddies_st));
			}
		};

		struct uid_ver : public sox::Marshallable {
			uid_ver(){}
			uid_ver(uint32_t u, uint32_t ver) : uid(u), version(ver) { }

			uint32_t uid;
			uint32_t version;

			virtual void marshal(sox::Pack &p) const {
				p << uid << version;
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> uid >> version;
			}
		};

		struct PCS_Update_BudInfo_Timely : public sox::Marshallable {
			enum {uri = (81 << 8) | protocol::IMDB_SVID};
			PCS_Update_BudInfo_Timely(){}
			PCS_Update_BudInfo_Timely(std::vector<uid_ver>& buds) : buddies_vers(buds){}

			std::vector<uid_ver> buddies_vers;

			virtual void marshal(sox::Pack &p) const {
				marshal_container(p, buddies_vers);
			}
			
			virtual void unmarshal(const sox::Unpack &p) {
				unmarshal_container(p, std::back_inserter(buddies_vers));
			}
		};

		struct PCS_Up_BInfo_Timely_Res : public sox::Marshallable {
			enum {uri = (82 << 8) | protocol::IMDB_SVID};
			PCS_Up_BInfo_Timely_Res(){}
			PCS_Up_BInfo_Timely_Res(std::vector<SUserinfo>& buds) : buddies_info(buds){}

			std::vector<SUserinfo> buddies_info;

			virtual void marshal(sox::Pack &p) const {
				marshal_container(p, buddies_info);
			}

			virtual void unmarshal(const sox::Unpack &p) {
				unmarshal_container(p, std::back_inserter(buddies_info));
			}
		};


		struct 	PCS_NeedReImLogin : public sox::Marshallable 
		{
			enum {uri = (83 << 8) | protocol::IMDB_SVID};
			PCS_NeedReImLogin(){}
			PCS_NeedReImLogin(uint32_t uId)
			{
				m_uId=uId;
			}

			uint32_t m_uId;

			virtual void marshal(sox::Pack &p) const 
			{
				p<<m_uId;

			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p>>m_uId;

			}

		};

		struct 	PCS_GetBuddyExtraDataListRes : public PCS_CommonHead
		{
			enum {uri = (84 << 8) | protocol::IMDB_SVID};

			PCS_GetBuddyExtraDataListRes(){}

			vector<CObjList2> m_vData;

			virtual void marshal(sox::Pack &p) const 
			{
				PCS_CommonHead::marshal(p);
				marshal_container(p, m_vData);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				PCS_CommonHead::unmarshal(p);
				unmarshal_container(p, std::back_inserter(m_vData));
			}

		};


		struct 	PCS_GetBuddyExtraDataList : public sox::Voidmable 
		{
			enum {uri = (85 << 8) | protocol::IMDB_SVID};
			
		};

		/////////////////////////

		struct 	PCS_GetUserInfo : public sox::Marshallable 
		{
			enum {uri = (86 << 8) | protocol::IMDB_SVID};
			PCS_GetUserInfo(){}
			PCS_GetUserInfo(uint32_t uId)
			{
				m_uId=uId;
			}

			uint32_t m_uId;

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uId;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uId;
			}

		};

		struct 	PCS_GetUserInfoRes : public PCS_CommonHead
		{
			enum {uri = (87 << 8) | protocol::IMDB_SVID};

			PCS_GetUserInfoRes(){}
			PCS_GetUserInfoRes(const SUserinfo1& objData)
			{
				m_objUinfo=objData;
			}

			SUserinfo1 m_objUinfo;

			virtual void marshal(sox::Pack &p) const 
			{
				PCS_CommonHead::marshal(p);
				p << m_objUinfo;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				PCS_CommonHead::unmarshal(p);
				p >> m_objUinfo;
			}

		};


		struct 	PCS_GetShenQinResultlList : public sox::Voidmable 
		{
			enum {uri = (88 << 8) | protocol::IMDB_SVID};

		};

		
		struct PCS_GetShenQinResultlListRes : public PCS_CommonHead
		{
			enum {uri = (89 << 8) | protocol::IMDB_SVID};
			PCS_GetShenQinResultlListRes(){}

			vector<uint32_t> m_vData;;

			virtual void marshal(sox::Pack &p) const
			{
				PCS_CommonHead::marshal(p);
				marshal_container(p, m_vData);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				PCS_CommonHead::unmarshal(p);
				unmarshal_container(p, std::back_inserter(m_vData));
			}
			//wuji start
// 			std::string ToString() const 
// 			{
// 				sox::PackBuffer bf;
// 				sox::Pack pk(bf);
// 				this->marshal(pk);
// 				return std::string(pk.data(), pk.size());
// 			}
// 
// 			static PCS_GetShenQinResultlListRes FromString(const std::string &str) 
// 			{
// 				sox::Unpack up(str.data(), str.size());
// 				PCS_GetShenQinResultlListRes tc;
// 				tc.unmarshal(up);
// 				return tc;
// 			}
			//wuji end

		};

		struct PCS_ChannelAddBuddyReq : public protocol::verify_code::CommonVerifyCodeOperatorReq
		{

			enum {uri = (90 << 8) | protocol::IMDB_SVID};
		
			//enum {uri = (90 << 8) | protocol::IMSEARCH_SVID};



			uint32_t m_ibid;				//被添加人的uid

			PCS_ChannelAddBuddyReq(const uint32_t bid, const std::string &strKey,const std::string &strValue) :
								CommonVerifyCodeOperatorReq(strKey, strValue), m_ibid(bid)
			{
			}

			PCS_ChannelAddBuddyReq():CommonVerifyCodeOperatorReq(), m_ibid(0){}

			virtual void marshal(sox::Pack &p) const {	p << m_ibid;  CommonVerifyCodeOperatorReq::marshal(p);	}

			virtual void unmarshal(const sox::Unpack &p) {	p >> m_ibid; CommonVerifyCodeOperatorReq::unmarshal(p);}

		};
		//added by zzw ,20091012
		struct PCS_IMSChannelAddBuddyReq : public protocol::verify_code::CommonVerifyCodeOperatorReq
		{
			//modified by zzw , 20090921
			/*
			enum {uri = (90 << 8) | protocol::IMDB_SVID};
			*/
			enum {uri = (90 << 8) | protocol::IMSEARCH_SVID};



			uint32_t m_ibid;				//被添加人的uid

			PCS_IMSChannelAddBuddyReq(const uint32_t bid, const std::string &strKey,const std::string &strValue) :
			CommonVerifyCodeOperatorReq(strKey, strValue), m_ibid(bid)
			{
			}

			PCS_IMSChannelAddBuddyReq():CommonVerifyCodeOperatorReq(), m_ibid(0){}

			virtual void marshal(sox::Pack &p) const {	p << m_ibid;  CommonVerifyCodeOperatorReq::marshal(p);	}

			virtual void unmarshal(const sox::Unpack &p) {	p >> m_ibid; CommonVerifyCodeOperatorReq::unmarshal(p);}

		};
		struct PCS_ChannelAddBuddyRes :  public protocol::verify_code::CommonVerifyCodeOperatorRes
		{
			enum {uri = (91 << 8) | protocol::IMDB_SVID};
			uint32_t bid;									//被添加用户的uid
			virtual void marshal(sox::Pack &p) const {	p << bid;  CommonVerifyCodeOperatorRes::marshal(p);	}
			virtual void unmarshal(const sox::Unpack &p) {	p >> bid; CommonVerifyCodeOperatorRes::unmarshal(p);}
		};

		struct PCS_SearchBuddyReq2 : public protocol::verify_code::CommonVerifyCodeOperatorReq
		{

			enum {uri = (92 << 8) | protocol::IMDB_SVID};

			//enum {uri = (92 << 8) | protocol::IMSEARCH_SVID};


			uint32_t m_iimid;				//被添加人的imid

			PCS_SearchBuddyReq2(const uint32_t imid, const std::string &strKey,const std::string &strValue) :
			CommonVerifyCodeOperatorReq(strKey, strValue), m_iimid(imid)
			{
			}

			PCS_SearchBuddyReq2():CommonVerifyCodeOperatorReq(), m_iimid(0){}

			virtual void marshal(sox::Pack &p) const {	p << m_iimid;  CommonVerifyCodeOperatorReq::marshal(p);	}

			virtual void unmarshal(const sox::Unpack &p) {	p >> m_iimid; CommonVerifyCodeOperatorReq::unmarshal(p);}
		};
       //added by zzw , 20091012
		struct PCS_IMSSearchBuddyReq2 : public protocol::verify_code::CommonVerifyCodeOperatorReq
		{
			//modified by zzw ,20090921 
			/* old 
			enum {uri = (92 << 8) | protocol::IMDB_SVID};
			*/
			enum {uri = (92 << 8) | protocol::IMSEARCH_SVID};


			uint32_t m_iimid;				//被添加人的imid

			PCS_IMSSearchBuddyReq2(const uint32_t imid, const std::string &strKey,const std::string &strValue) :
			CommonVerifyCodeOperatorReq(strKey, strValue), m_iimid(imid)
			{
			}

			PCS_IMSSearchBuddyReq2():CommonVerifyCodeOperatorReq(), m_iimid(0){}

			virtual void marshal(sox::Pack &p) const {	p << m_iimid;  CommonVerifyCodeOperatorReq::marshal(p);	}

			virtual void unmarshal(const sox::Unpack &p) {	p >> m_iimid; CommonVerifyCodeOperatorReq::unmarshal(p);}
		};

		struct PCS_SearchBuddyRes2 :  public protocol::verify_code::CommonVerifyCodeOperatorRes
		{
			enum {uri = (93 << 8) | protocol::IMDB_SVID};

			uint32_t uid;									//被查找用户的uid
			protocol::E_IMSTATUS stat;
			sox::Properties stProps;

			virtual void marshal(sox::Pack &p) const 
			{	
				p << uid; 
				p.push_uint8(stat);  
				CommonVerifyCodeOperatorRes::marshal(p);	
				stProps.marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{	
				p >> uid; 
				stat = (protocol::E_IMSTATUS)p.pop_uint8(); 
				CommonVerifyCodeOperatorRes::unmarshal(p); 
				stProps.unmarshal(p);
			}
		};

//#define __BETA_LIMIT_IMID_
//#ifdef __BETA_LIMIT_IMID_
		struct PCS_NoImIdRes : public sox::Voidmable		//鐢ㄦ埛娌℃湁IMID�?鍙戝嚭鐨勫搷�? 浠呭綋闄愬埗鍙风爜鐧婚檰鏃朵娇鐢?
		{
			enum {uri = (94<< 8) | protocol::IMDB_SVID};
			PCS_NoImIdRes(){}
		};		
//#endif
//用于被添加人同意添加人的请求后的回应
		struct PCS_ReqAddBuddyByOpRes : public PCS_CommonHead
		{
			enum {uri = (95 << 8) | protocol::IMDB_SVID};
			PCS_ReqAddBuddyByOpRes(){}
			PCS_ReqAddBuddyByOpRes(uint32_t uBid,uint32_t uGid)
			{
				m_uBid=uBid;
				m_uGid=uGid;

			}
			uint32_t m_uBid; //
			uint32_t m_uGid;


			virtual void marshal(sox::Pack &p) const 
			{
				PCS_CommonHead::marshal(p);
				p << m_uBid<<m_uGid;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				PCS_CommonHead::unmarshal(p);
				p >> m_uBid>>m_uGid;
			}
		};


		//2009.10.10 wuji 鍙栨墍鐜╂父�?
		struct PCS_GetMyGame : public sox::Voidmable		
		{
			enum {uri = (96<< 8) | protocol::IMDB_SVID};
			PCS_GetMyGame(){}
		};


		struct PCS_GetMyGameRes : public PCS_CommonHead
		{
			enum {uri = (97<< 8) | protocol::IMDB_SVID};
			PCS_GetMyGameRes()
			{
				m_uGameId=-1;
			}
			uint32_t m_uGameId;
			string m_strData;

			virtual void marshal(sox::Pack &p) const 
			{
				PCS_CommonHead::marshal(p);
				p << m_uGameId<<m_strData;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				PCS_CommonHead::unmarshal(p);
				p >> m_uGameId>>m_strData;
			}



		};


		struct PCS_AfterUpdateMyUinfo : public sox::Marshallable {
			enum { uri = (98 << 8) | protocol::IMDB_SVID };
			PCS_AfterUpdateMyUinfo(){}
			PCS_AfterUpdateMyUinfo(const SUserinfo& info) : buddyinfo(info){
			}

			SUserinfo buddyinfo; //just include following info maybe changed: buddyId, nickname, rank, sign, sex;

			virtual void marshal(sox::Pack &p) const{
				p << buddyinfo;
			}

			virtual void unmarshal(const sox::Unpack &p){
				p >> buddyinfo;
			}
		};


		//uri 99�?00 鐣欑粰PSS_GetPrepareLoginListReq  �? PSS_GetPrepareLoginListRes浣跨�?  鍦╯erver/protocol/imdb.h涓畾涔?

		struct PCS_PresenceChange1  : public sox::Marshallable 
		{
			enum {uri = (99 << 8) | protocol::IMDB_SVID};
			PCS_PresenceChange1(){}
			PCS_PresenceChange1(E_PRESENCE_TYPE uTypeId,uint32_t uXuhao ,uint32_t uSendId) 
			{
				m_uXuHao=uXuhao;
				m_uTypeId=uTypeId;
				m_uSendId=uSendId;
			}

			PCS_PresenceChange1(E_PRESENCE_TYPE uTypeId,uint32_t uXuhao ,uint32_t uSendId,const string& strData) 
			{
				m_uXuHao=uXuhao;
				m_uTypeId=uTypeId;
				m_uSendId=uSendId;
				m_strData=strData;
			}

			E_PRESENCE_TYPE m_uTypeId;	//1閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓鏂ゆ嫹棰戦敓鏂ゆ嫹�?閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷烽敓鏂ゆ嫹閿熸枻鎷锋垙閿熸枻鎷?閿熸枻鎷烽敓鏂ゆ嫹閿熺殕纭锋嫹澶撮敓鏂ゆ�?
			uint32_t m_uXuHao;//
			uint32_t m_uSendId;//閿熸枻鎷烽敓鏂ゆ嫹閿熸枻�?

			string m_strData;


			virtual void marshal(sox::Pack &p) const
			{
				p << m_uXuHao<<m_uSendId;
				p.push_uint8(m_uTypeId);
				p<<m_strData;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >>m_uXuHao>>m_uSendId;
				m_uTypeId=(E_PRESENCE_TYPE)p.pop_uint8();
				p>>m_strData;
			}
		};
		//请求获取全部好友的imid
		struct PCS_GetBuddyImIdReq  : public protocol::im::CUidSet
		{
			enum {uri = (102 << 8) | protocol::IMDB_SVID};
			PCS_GetBuddyImIdReq(){}
		};

		//返回全部好友的imid
		struct PCS_GetBuddyImIdRes  : public sox::Marshallable 
		{
			enum {uri = (102 << 8) | protocol::IMDB_SVID};
			PCS_GetBuddyImIdRes(){}

			std::map<uint32_t, uint32_t> m_mapBidToImId;
			virtual void marshal(sox::Pack &p) const
			{
				p.push_uint32(m_mapBidToImId.size());
				for(std::map<uint32_t, uint32_t>::const_iterator iter = m_mapBidToImId.begin(); iter != m_mapBidToImId.end(); ++iter)
				{
					p.push_uint32(iter->first);
					p.push_uint32(iter->second);
				}
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				uint32_t size = p.pop_uint32();
				for(uint32_t i =0; i < size; ++i)
				{
					uint32_t bid = p.pop_uint32();
					uint32_t imid = p.pop_uint32();
					m_mapBidToImId.insert(make_pair(bid, imid));
				}
			}
		};


		// ��½ʱȡ����Uinfo
		struct PCS_GetBuddyUinfoReq  :public protocol::im::CUidSet
		{
			enum {uri = (103 << 8) | protocol::IMDB_SVID};
			PCS_GetBuddyUinfoReq(){}
		};

		//返回全部好友的Uinfo
		struct PCS_GetBuddyUinfoRes  : public sox::Marshallable 
		{
			enum {uri = (103 << 8) | protocol::IMDB_SVID};
			PCS_GetBuddyUinfoRes(){}

			std::map<uint32_t, SUserinfo> m_mapBidToUInfo;
			virtual void marshal(sox::Pack &p) const
			{
				p.push_uint32(m_mapBidToUInfo.size());
				for(std::map<uint32_t, SUserinfo>::const_iterator iter = m_mapBidToUInfo.begin(); iter != m_mapBidToUInfo.end(); ++iter)
				{
					p.push_uint32(iter->first);
					iter->second.marshal(p);
				}
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				uint32_t size = p.pop_uint32();
				for(uint32_t i =0; i < size; ++i)
				{
					uint32_t bid = p.pop_uint32();
					SUserinfo tmp;
					tmp.unmarshal(p);
					m_mapBidToUInfo.insert(make_pair(bid, tmp));
				}
			}
		};

		//请求获取全部好友的remark(备注信息)
		struct PCS_GetBuddyRemarkReq  : public sox::Marshallable 
		{
			enum {uri = (104 << 8) | protocol::IMDB_SVID};
			PCS_GetBuddyRemarkReq(){}

			//uint32_t m_uId;
			//std::vector<uint32_t> m_vtBid;
			virtual void marshal(sox::Pack &p) const
			{
				//p << m_uId;
				//marshal_container(p, m_vtBid);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				//p >> m_uId;
				//unmarshal_container(p, std::back_inserter(m_vtBid));
			}
		};

		//返回全部好友的remark(备注信息)
		struct PCS_GetBuddyRemarkRes  : public sox::Marshallable 
		{
			enum {uri = (104 << 8) | protocol::IMDB_SVID};
			PCS_GetBuddyRemarkRes(){}

			std::map<uint32_t, std::string> m_mapBidToRemark;
			virtual void marshal(sox::Pack &p) const
			{
				p.push_uint32(m_mapBidToRemark.size());
				for(std::map<uint32_t, std::string>::const_iterator iter = m_mapBidToRemark.begin(); iter != m_mapBidToRemark.end(); ++iter)
				{
					p.push_uint32(iter->first);
					p << iter->second;
				}
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				uint32_t size = p.pop_uint32();
				for(uint32_t i =0; i < size; ++i)
				{
					uint32_t bid = p.pop_uint32();
					std::string tmp;
					p >> tmp;
					m_mapBidToRemark.insert(make_pair(bid, tmp));
				}
			}
		};
	

		///////////////////////////////////////////////////////////////////
		// yy鍐滃満娓告垙鎺ュ彛�?
		// 接入添加好友请求
		
		struct CAddBuddyReq : public sox::Marshallable
		{
			uint32_t m_uid;            // 申请人uid
			uint32_t m_bid;            // 被添加人uid

			CAddBuddyReq(){}
			CAddBuddyReq(uint32_t uid, uint32_t bid) : m_uid(uid), m_bid(bid)
			{
			}

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uid << m_bid;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uid >> m_bid;
			}
		};

		struct PSS_GameProtocolAddBuddy : public sox::Marshallable
		{
			enum {uri = (201 << 8) | protocol::IMDB_SVID};
			PSS_GameProtocolAddBuddy(){}
			PSS_GameProtocolAddBuddy(const std::vector<CAddBuddyReq> &vecObj)
			{
				m_vecReq = vecObj;
			}

			std::vector<CAddBuddyReq> m_vecReq;

			virtual void marshal(sox::Pack &p) const 
			{
				marshal_container(p, m_vecReq);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				unmarshal_container(p, std::back_inserter(m_vecReq));
			}
		};

		//im��Ⱥ��ȡimidʹ�ò�ͬ�Ľӿڣ���buddycounter������������
		struct PCS_GetBuddyImId2: public PCS_GetBuddyImIdReq
		{
			enum {uri = (202 << 8) | protocol::IMDB_SVID};
		};
		



        struct PCS_BatchGetSimpleUInfo: public sox::Marshallable
        {   
            enum {uri = (203 << 8) | protocol::IMDB_SVID};
            uint32_t m_uUri;
            std::vector<uint32_t> m_vecUid;

            virtual void marshal(sox::Pack &p) const
            {
                p << m_uUri;
                marshal_container(p, m_vecUid);    
            }

            virtual void unmarshal(const sox::Unpack &p)
            {
                p >> m_uUri;
                unmarshal_container(p, std::back_inserter(m_vecUid));
            }
        };

        struct PCS_BatchGetSimpleUInfoRes: public sox::Marshallable
        {
            enum {uri = (204 << 8) | protocol::IMDB_SVID};
            uint32_t m_uUri;
            std::map<uint32_t, SUserinfo> m_mapUidToInfo;

            virtual void marshal(sox::Pack &p) const
            {
                p << m_uUri;
                p.push_uint32((uint32_t) m_mapUidToInfo.size());
                for(std::map<uint32_t, SUserinfo>::const_iterator iter = m_mapUidToInfo.begin(); iter != m_mapUidToInfo.end(); ++iter)
                {
                    p.push_uint32(iter->first);
                    iter->second.marshal(p);
                }
            }

            virtual void unmarshal(const sox::Unpack &p)
            {
                p >> m_uUri;
                uint32_t size = p.pop_uint32();
                for(uint32_t i =0; i < size; ++i)
                {
                    uint32_t bid = p.pop_uint32();
                    SUserinfo &tmp = m_mapUidToInfo[bid];
                    tmp.unmarshal(p);
                }
            }
        };

		//add by kdjie, 2010.5.17
		//����protocol -> imcache ȡuinfo detail

		struct 	PCS_GetUserInfo3 : public sox::Marshallable 
		{
			enum {uri = (205 << 8) | protocol::IMDB_SVID};

			uint32_t m_uId;

			PCS_GetUserInfo3(){}
			PCS_GetUserInfo3(uint32_t uId)
			{
				m_uId = uId;
			}

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uId;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uId;
			}

		};

		struct 	PCS_GetUserInfo3Res : public PCS_CommonHead
		{
			enum {uri = (206 << 8) | protocol::IMDB_SVID};

			SUserinfo1 m_objUinfo;

			PCS_GetUserInfo3Res() {}
			PCS_GetUserInfo3Res(const SUserinfo1& objData)
			{
				m_objUinfo = objData;
			}

			virtual void marshal(sox::Pack &p) const 
			{
				PCS_CommonHead::marshal(p);
				p << m_objUinfo;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				PCS_CommonHead::unmarshal(p);
				p >> m_objUinfo;
			}

		};


		struct SUserinfoDetail : public sox::Marshallable //锟斤拷锟斤拷锟斤拷锟斤拷3锟斤拷锟斤拷锟斤拷锟斤拷锟侥大部凤拷锟街讹拷
		{

			SUserinfoDetail()
			{
				m_uId =(uint32_t)-1;
				//m_uHeadPhoto=(uint32_t)NOHEAD;//ͷ��,û�о���-1
				//m_uImId=(uint32_t)-1;//yy閿熸暀鍙凤綇鎷锋病閿熷彨鎾呮嫹閿熸枻鎷?1
				//m_stat=IMOFFLINE;
				m_uBirthDay = (uint32_t)-1;
				m_uArea = (uint32_t)-1;
				m_uProvince = (uint32_t)-1;
				m_uCity = (uint32_t)-1;
				m_uJiFen = 0;

			}

			uint32_t m_uId;//锟矫伙拷ID
			std::string m_strNickname;//锟矫伙拷锟角筹拷
			protocol::uinfo::SEX m_sex;
			uint32_t m_uBirthDay;
			uint32_t m_uArea;
			uint32_t m_uProvince;
			uint32_t m_uCity;
			string m_strResume;
			uint32_t m_uVersion;
			uint32_t m_uJiFen;
			string m_strSign;
			string m_strIntro;

			//protocol::E_IMSTATUS m_stat;//鐘舵�?

			//uint32_t m_uHeadPhoto;//头锟斤拷,没锟叫撅拷锟斤拷-1
			//uint32_t m_uImId;//yy閿熸暀鍙凤綇鎷锋病閿熷彨鎾呮嫹閿熸枻鎷?1

			virtual void marshal(sox::Pack &p) const 
			{
				p<<m_uId<<m_strNickname<<m_uBirthDay<<m_uArea<<m_uProvince<<m_uCity<<m_strResume<<m_uVersion<<m_uJiFen<<m_strSign<<m_strIntro/*<<m_uHeadPhoto<<m_uImId*/;
				p.push_uint8(m_sex);
				//p.push_uint8(m_stat);

			}
			virtual void unmarshal(const sox::Unpack &p) 
			{
				p>>m_uId>>m_strNickname>>m_uBirthDay>>m_uArea>>m_uProvince>>m_uCity>>m_strResume>>m_uVersion>>m_uJiFen>>m_strSign>>m_strIntro/*>>m_uHeadPhoto>>m_uImId*/;
				m_sex = (protocol::uinfo::SEX)p.pop_uint8();
				//m_stat = (protocol::E_IMSTATUS)p.pop_uint8();
			}
		};

		//�޸��û���ϸ��Ϣ֪ͨ
		struct PCS_BuddyChangedInfoEx : public sox::Marshallable 
		{
			enum { uri = (207 << 8) | protocol::IMDB_SVID };

			PCS_BuddyChangedInfoEx(){}
			PCS_BuddyChangedInfoEx(const SUserinfoDetail& info) : buddyinfo(info) {}

			SUserinfoDetail buddyinfo;

			virtual void marshal(sox::Pack &p) const
			{
				p << buddyinfo;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> buddyinfo;
			}
		};



		struct PCS_GetBlackListUinfoReq  :public protocol::im::CUidSet
		{
			enum {uri = (208 << 8) | protocol::IMDB_SVID};
			PCS_GetBlackListUinfoReq(){}
		};



		struct PCS_GetBlackListUinfoRes  : public sox::Marshallable 
		{
			enum {uri = (209 << 8) | protocol::IMDB_SVID};
			PCS_GetBlackListUinfoRes(){}

			std::map<uint32_t, SUserinfo> m_mapBidToUInfo;
			virtual void marshal(sox::Pack &p) const
			{
				p.push_uint32(m_mapBidToUInfo.size());
				for(std::map<uint32_t, SUserinfo>::const_iterator iter = m_mapBidToUInfo.begin(); iter != m_mapBidToUInfo.end(); ++iter)
				{
					p.push_uint32(iter->first);
					iter->second.marshal(p);
				}
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				uint32_t size = p.pop_uint32();
				for(uint32_t i =0; i < size; ++i)
				{
					uint32_t bid = p.pop_uint32();
					SUserinfo tmp;
					tmp.unmarshal(p);
					m_mapBidToUInfo.insert(make_pair(bid, tmp));
				}
			}
		};

		//Please fill m_uJiFen, m_uUID and m_uSvrID and forward it to groupid_d
		struct PCS_GetMyUserJF : public sox::Marshallable 
		{
			enum { uri = (210 << 8) | protocol::IMDB_SVID };

			uint32_t m_uJiFen;
			uint32_t m_uUID; //Please get it from imlinkd and fill for the next server
			uint32_t m_uSvrID; //Please get it from imlinkd and fill for the next server
			uint32_t m_uAppURI;
			std::string m_strAppCtx;

			PCS_GetMyUserJF ()
			{
				m_uJiFen = 0;
				m_uUID = 0;
				m_uSvrID = 0;
				m_uAppURI = 0;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uJiFen << m_uUID << m_uSvrID << m_uAppURI << m_strAppCtx;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uJiFen >> m_uUID >> m_uSvrID >> m_uAppURI >> m_strAppCtx;
			}
		};

		struct PSS_GetMyUserJF : public PCS_GetMyUserJF
		{
			enum { uri = (211 << 8) | protocol::IMDB_SVID };
		};


		//add end

		//�����ƶ����ѵ�����
		struct PCS_BatchMoveToFolder : public sox::Marshallable 
		{
			enum { uri = (212 << 8) | protocol::IMDB_SVID };

			std::vector<PCS_MoveToFolder> m_vecMoveToFolder;

			PCS_BatchMoveToFolder(){}

			virtual void marshal(sox::Pack &p) const 
			{
				sox::marshal_container(p, m_vecMoveToFolder);
			}
			virtual void unmarshal(const sox::Unpack &p) 
			{
				sox::unmarshal_container(p, std::back_inserter(m_vecMoveToFolder));
			}
		};		

		struct PCS_BatchMoveToFolderRes : public PCS_CommonHead
		{
			enum { uri = (213 << 8) | protocol::IMDB_SVID };

			std::vector<PCS_MoveToFolder> m_vecMoveToFolder; //�����Ѿ��ƶ��ɹ���UID

			PCS_BatchMoveToFolderRes(){}

			virtual void marshal(sox::Pack &p) const 
			{
				PCS_CommonHead::marshal(p);
				sox::marshal_container(p, m_vecMoveToFolder);
			}
			virtual void unmarshal(const sox::Unpack &p) 
			{
				PCS_CommonHead::unmarshal(p);
				sox::unmarshal_container(p, std::back_inserter(m_vecMoveToFolder));
			}
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		// 3.6 ���Ӵ���֤��ȡ���ԼӺ��ѽӿ�

		struct PCS_GetBuddyCeLueVerify  : public sox::Marshallable
			/* : public protocol::verify_code::CommonVerifyCodeOperatorReq*/
		{
			enum {uri = (214 << 8) | protocol::IMDB_SVID};

			uint32_t m_uBid;

			PCS_GetBuddyCeLueVerify(){}
			PCS_GetBuddyCeLueVerify(uint32_t uBid)
			{
				m_uBid=uBid;
			}

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uBid;
				//CommonVerifyCodeOperatorReq::marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uBid;
				//CommonVerifyCodeOperatorReq::unmarshal(p);
			}
		};

		struct PCS_GetBuddyCeLueVerifyRes : 
			public protocol::verify_code::CommonVerifyCodeOperatorRes
		{
			enum {uri = (215 << 8) | protocol::IMDB_SVID};

			uint32_t m_uBid;
			uint32_t m_uCeLue;
			uint32_t m_uQuestionId;
			std::string m_strQuestion;

			PCS_GetBuddyCeLueVerifyRes() {}
			PCS_GetBuddyCeLueVerifyRes(uint32_t uBid,uint32_t uCeLue,uint32_t uQuestion,const string& strQuestion)
			{
				m_uBid = uBid;
				m_uCeLue = uCeLue;
				m_uQuestionId = uQuestion;
				m_strQuestion = strQuestion;
			}

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uBid<<m_uCeLue<<m_uQuestionId<<m_strQuestion;
				CommonVerifyCodeOperatorRes::marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uBid>>m_uCeLue>>m_uQuestionId>>m_strQuestion;
				CommonVerifyCodeOperatorRes::unmarshal(p);
			}
		};

		//-------------------------------------

		struct PCS_ReqAddBuddyVerify : public protocol::verify_code::CommonVerifyCodeOperatorReq
		{
			enum {uri = (216 << 8) | protocol::IMDB_SVID};

			uint32_t bid;
			uint32_t gid; //you want add the 'bid' into this 'gid'
			std::string msg_left;

			PCS_ReqAddBuddyVerify(){}
			PCS_ReqAddBuddyVerify(uint32_t b, uint32_t g, std::string& msg)
				: bid(b), gid(g), msg_left(msg) {};

			virtual void marshal(sox::Pack &p) const 
			{
				p << bid << gid << msg_left;
				CommonVerifyCodeOperatorReq::marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> bid >> gid >> msg_left;
				CommonVerifyCodeOperatorReq::unmarshal(p);
			}
		};

		struct PCS_ReqAddBuddyVerifyRes : public protocol::verify_code::CommonVerifyCodeOperatorRes
		{
			enum {uri = (217 << 8) | protocol::IMDB_SVID};

			uint32_t bid;
			uint32_t gid; //you want add the 'bid' into this 'gid'
			std::string msg_left;

			PCS_ReqAddBuddyVerifyRes(){}
			PCS_ReqAddBuddyVerifyRes(uint32_t b, uint32_t g, std::string& msg)
				: bid(b), gid(g), msg_left(msg) {};

			virtual void marshal(sox::Pack &p) const 
			{
				p << bid << gid << msg_left;
				CommonVerifyCodeOperatorRes::marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> bid >> gid >> msg_left;
				CommonVerifyCodeOperatorRes::unmarshal(p);
			}
		};

		//-------------------------------------

		struct PCS_ReqAddBuddyByScoreVerify : public protocol::verify_code::CommonVerifyCodeOperatorReq
		{
			enum {uri = (218 << 8) | protocol::IMDB_SVID};

			uint32_t m_uBid;

			PCS_ReqAddBuddyByScoreVerify(){}
			PCS_ReqAddBuddyByScoreVerify(uint32_t uBid)
			{
				m_uBid=uBid;
			}

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uBid;
				CommonVerifyCodeOperatorReq::marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uBid;
				CommonVerifyCodeOperatorReq::unmarshal(p);
			}
		};

		struct PCS_ReqAddBuddyByScoreVerifyRes : public protocol::verify_code::CommonVerifyCodeOperatorRes
		{
			enum {uri = (219 << 8) | protocol::IMDB_SVID};

			uint32_t m_uBid;

			PCS_ReqAddBuddyByScoreVerifyRes(){}
			PCS_ReqAddBuddyByScoreVerifyRes(uint32_t uBid)
			{
				m_uBid=uBid;
			}

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uBid;
				CommonVerifyCodeOperatorRes::marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uBid;
				CommonVerifyCodeOperatorRes::unmarshal(p);
			}
		};

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		struct PCS_GetPartialBuddyRemarkReq  : public sox::Marshallable 
		{
			enum {uri = (220 << 8) | protocol::IMDB_SVID};
			PCS_GetPartialBuddyRemarkReq(){}

			std::set<uint32_t>	m_setUid;
			virtual void marshal(sox::Pack &p) const
			{
				marshal_container(p, m_setUid);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				unmarshal_container(p, std::inserter(m_setUid, m_setUid.begin()));
			}
		};

		struct PCS_GetPartialBuddyRemarkRes : public PCS_GetBuddyRemarkRes
		{
			enum {uri = (221 << 8) | protocol::IMDB_SVID};
			PCS_GetPartialBuddyRemarkRes(){}

			virtual void marshal(sox::Pack &p) const 
			{
				PCS_GetBuddyRemarkRes::marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				PCS_GetBuddyRemarkRes::unmarshal(p);
			}
		};

	}
}

