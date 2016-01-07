#ifndef _PWD_SECURITY_H_
#define _PWD_SECURITY_H_

#include "int_types.h"
#include "core/base_svid.h"
#include "protocol/const.h"
#include "packet.h"

namespace server{
	namespace newpwddb{

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		struct PGetUserCoreInfoReq : public sox::Marshallable {
			enum {uri = (50 << 8) | 178};


			std::string strSeq;
			uint32_t type;
			std::string query_value;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << type << query_value;	
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> type >> query_value;	
			}
		};	



		struct PGetUserCoreInfoRes : public sox::Marshallable {
			enum {uri = (51 << 8) | 178};

			std::string strSeq;
			uint32_t resCode;
			uint32_t uid;
			std::string username;
			uint32_t  yyuid;
			uint32_t yyid;
			uint32_t status;
			std::string email;
			uint32_t verifiedEmail;
			std::string mobile;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << resCode << uid << username << yyuid << yyid << status << email << verifiedEmail << mobile;
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> resCode >> uid >> username >> yyuid >> yyid >> status >> email >> verifiedEmail >> mobile;	
			}


		};
		

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		struct PGetMibaoInfoReq : public sox::Marshallable {
			enum {uri = (52 << 8) | 178};

			std::string strSeq;
			uint32_t yyuid;
			
			virtual void marshal(sox::Pack &p) const {
				p << strSeq << yyuid;	
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> yyuid;	
			}
		};
		
		struct PGetMibaoInfoRes: public sox::Marshallable {
			enum {uri = (53 << 8) | 178};

			std::string strSeq;
			uint32_t resCode;
			uint32_t yyuid;
			uint32_t status;
			std::string email;
			uint32_t verifiedEmail;
			uint32_t idType;
			std::string idNumber;
			std::string mobile;
			std::string question;
			std::string question2;
			std::string question3;
			std::string answer;
			std::string answer2;
			std::string answer3;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << resCode << yyuid << status << email << verifiedEmail << idType << idNumber << mobile << question << question2 << question3 << answer << answer2 << answer3;	
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> resCode >> yyuid >> status >> email >> verifiedEmail >> idType >> idNumber >> mobile >> question >> question2 >> question3 >> answer >> answer2 >> answer3;
			}

		};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//for zyj appeal system
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		struct PGetMibaoInfoForAplReq : public sox::Marshallable {
			enum {uri = (88 << 8) | 178};

			std::string strSeq;
			uint32_t yyuid;
			
			virtual void marshal(sox::Pack &p) const {
				p << strSeq << yyuid;	
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> yyuid;	
			}
		};
		
		struct PGetMibaoInfoForAplRes: public sox::Marshallable {
			enum {uri = ( 89 << 8) | 178};

			std::string strSeq;
			uint32_t resCode;
			uint32_t yyuid;
			uint32_t status;
			std::string email;
			uint32_t verifiedEmail;
			uint32_t idType;
			std::string idNumber;
			std::string mobile;
			std::string question;
			std::string question2;
			std::string question3;
			std::string answer;
			std::string answer2;
			std::string answer3;
			std::string regtime;
			std::string regip;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << resCode << yyuid << status << email << verifiedEmail << idType << idNumber << mobile << question << question2 << question3 << answer << answer2 << answer3 << regtime << regip;	
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> resCode >> yyuid >> status >> email >> verifiedEmail >> idType >> idNumber >> mobile >> question >> question2 >> question3 >> answer >> answer2 >> answer3 >> regtime >> regip;
			}

		};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		struct PVerifyAnswerReq : public sox::Marshallable {
			enum {uri = (54 << 8) | 178};

			std::string strSeq;
			uint32_t yyuid;
			std::string answer;
			std::string answer2;
			std::string answer3;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << yyuid << answer << answer2 << answer3;	
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> yyuid >> answer >> answer2 >> answer3;	
			}
		};

		struct PVerifyAnswerRes : public sox::Marshallable {
			enum {uri = (55 << 8) | 178};	

			std::string strSeq;
			uint32_t resCode;
			uint32_t result;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << resCode << result;
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> resCode >> result;	
			}
		};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		struct PUpdateMibaoQuestionReq : public sox::Marshallable {
			enum {uri = (56 << 8) | 178};

			std::string strSeq;
			uint32_t yyuid;
			std::string question;
			std::string answer;
			std::string question2;
			std::string answer2;
			std::string question3;
			std::string answer3;
			std::string clientip;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << yyuid << question << answer << question2 << answer2 << question3 << answer3 << clientip;	
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> yyuid >> question >> answer >> question2 >> answer2 >> question3 >> answer3;	

				if ( false == p.empty() ){
					try{
							p >> clientip;
						}
						catch(...){
						}
				}
			}
		};

		struct PUpdateMibaoQuestionRes : public sox::Marshallable {
			enum {uri = (57 << 8) | 178};

			std::string strSeq;
			uint32_t resCode;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << resCode;	
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> resCode;	
			}
		};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		struct PUpdateMibaoEmailReq : public sox::Marshallable {
			enum {uri = (58 << 8) | 178};

			std::string strSeq;
			std::string username;	
			uint32_t yyuid;
			std::string email;
			std::string clientip;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << username << yyuid << email << clientip;
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> username >> yyuid >> email;	

				if ( false == p.empty() ){
					try{
							p >> clientip;
						}
						catch(...){
						}
				}
			}
		};
		 
		struct PUpdateMibaoEmailRes : public sox::Marshallable {
			enum {uri = (59 << 8) | 178};

			std::string strSeq;
			uint32_t resCode;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << resCode;	
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> resCode;	
			}
		};
		

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		struct PUpdateMibaoIdenReq : public sox::Marshallable {
			enum {uri = (60 << 8) | 178};

			std::string strSeq;
			uint32_t yyuid;
			uint32_t idType;
			std::string idNumber;
			std::string clientip;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << yyuid << idType << idNumber << clientip;	
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> yyuid >> idType >> idNumber;	

				if ( false == p.empty() ){
					try{
							p >> clientip;
						}
						catch(...){
						}
				}
			}
		};

		struct PUpdateMibaoIdenRes : public sox::Marshallable {
			enum {uri = (61 << 8 ) | 178};

			std::string strSeq;
			uint32_t resCode;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << resCode;	
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> resCode;	
			}
		};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		struct PGetUserContactInfoReq : public sox::Marshallable {
			enum {uri = (62 << 8) | 178};

			std::string strSeq;
			uint32_t yyuid;
			
			virtual void marshal(sox::Pack &p) const {
				p << strSeq << yyuid;	
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> yyuid;	
			}
		};

		struct PGetUserContactInfoRes : public sox::Marshallable {
			enum {uri = (63 << 8) | 178};

			std::string strSeq;
			uint32_t resCode;
			uint32_t yyuid;
			std::string telephone;
			std::string address;
			std::string postalCode;
			std::string msn;
			std::string website;
			uint32_t country;
			uint32_t province;
			uint32_t city;
			uint32_t education;
			uint32_t income;
			uint32_t whereTheInternet;
			uint32_t gender;
			uint32_t marriage;
			std::string birthdate;
			std::string hobbies;
			uint32_t occupation;
			std::string regtime;
			std::string regip;
			uint32_t appid;
			std::string logintime;
			std::string loginip;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << resCode << yyuid << telephone << address << postalCode << msn << website << country << province << city << education << income << whereTheInternet << gender << marriage << birthdate << hobbies << occupation << regtime << regip << appid << logintime << loginip;
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> resCode >> yyuid >> telephone >> address >> postalCode >> msn >> website >> country >> province >>	city >> education >> income >> whereTheInternet >> gender >> marriage >> birthdate >> hobbies >> occupation >> regtime >> regip >> appid >> logintime >> loginip;
			}
		};



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		struct PUpdateUserInfoReq : public sox::Marshallable {
			enum {uri = (64 << 8) | 178};

			std::string strSeq;
			uint32_t yyuid;
			std::string telephone;
			std::string address;
			std::string postalCode;
			std::string msn;
			std::string website;
			uint32_t country;
			uint32_t province;
			uint32_t city;
			uint32_t education;
			uint32_t income;
			uint32_t whereTheInternet;
			uint32_t gender;
			uint32_t marriage;
			std::string birthdate;
			std::string hobbies;
			uint32_t occupation;	
			
			virtual void marshal(sox::Pack &p) const {
				p << strSeq << yyuid << telephone << address << postalCode << msn << website << country << province << city << education << income << whereTheInternet << gender << marriage << birthdate << hobbies << occupation;
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> yyuid >> telephone >> address >> postalCode >> msn >> website >> country >> province >> city >> education >> income >> whereTheInternet >> gender >> marriage >> birthdate >> hobbies >> occupation;
			}
		};


		struct PUpdateUserInfoRes : public sox::Marshallable {
			enum {uri = (65 << 8) | 178};

			std::string strSeq;
			uint32_t resCode;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << resCode;	
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> resCode;	
			}
		};
	

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		struct PUpdateMobileTokenStatusReq : public sox::Marshallable {
			enum {uri = (66 << 8) | 178};	

			std::string strSeq;
			std::string username;
			uint32_t type;
			std::string clientip;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << username << type << clientip;	
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> username >> type;	

				if ( false == p.empty() ){
					try{
							p >> clientip;
						}
						catch(...){
						}
				}
				
			}
		};

		struct PUpdateMobileTokenStatusRes : public sox::Marshallable {
			enum {uri = (67 << 8) | 178};

			std::string strSeq;
			uint32_t resCode;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << resCode;	
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> resCode;	
			}
		};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		struct PCheckMobileBindReq : public sox::Marshallable {
			enum {uri = (70 << 8) | 178};	

			std::string strSeq;
			std::string strMobile;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << strMobile;	
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> strMobile;	
			}
		};


		struct PCheckMobileBindRes : public sox::Marshallable {
			enum {uri = (71 << 8) | 178};	

			std::string strSeq;
			uint32_t resCode;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << resCode;	
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> resCode;	
			}
		};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		struct PGetMobileBindReq: public sox::Marshallable {
			enum {uri = (96 << 8) | 178};	

			std::string strSeq;
			std::string strMobile;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << strMobile;	
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> strMobile;	
			}
		};


		struct PGetMobileBindRes : public sox::Marshallable {
			enum {uri = (97 << 8) | 178};	

			std::string strSeq;
			std::string usernames;
			uint32_t resCode;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << usernames << resCode;	
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> usernames >> resCode;	
			}
		};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		struct PModifyRegSourceReq : public sox::Marshallable {
			enum {uri = (72 << 8) | 178};	

			std::string strSeq;
			std::string username;
			uint32_t appid;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << username << appid;	
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> username >> appid;	
			}
		};

		struct PModifyRegSourceRes : public sox::Marshallable {
			enum {uri = (73 << 8) | 178};	

			std::string strSeq;
			uint32_t resCode;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << resCode;	
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> resCode;	
			}
		};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		struct PBindMibaoMobileReq : public sox::Marshallable {
			enum {uri = (74 << 8) | 178};	

			std::string strSeq;
			uint32_t yyuid;
			std::string username;
			std::string mobile;
			std::string clientip;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << yyuid << username << mobile << clientip; 
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> yyuid >> username >> mobile;

				if ( false == p.empty() ){
					try{
							p >> clientip;
						}
						catch(...){
						}
				}
			}
		};

		struct PBindMibaoMobileRes : public sox::Marshallable {
			enum {uri = (75 << 8) | 178};			

			std::string strSeq;
			uint32_t resCode;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << resCode;
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> resCode;	
			}
		};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		struct PUnbindMibaoMobileReq : public sox::Marshallable {
			enum {uri = (76 << 8) | 178};	

			std::string strSeq;
			uint32_t yyuid;
			std::string username;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << yyuid << username;	
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> yyuid >> username;	
			}
		};

		struct PUnbindMibaoMobileRes : public sox::Marshallable{
			enum {uri = (77 << 8) | 178};

			std::string strSeq;
			uint32_t resCode;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << resCode;
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> resCode;	
			}
		};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		struct PCheck3rdAcctBindReq : public sox::Marshallable {
			enum {uri = (78 << 8) | 178};

			std::string strSeq;	
			std::string min3rdacct;
			std::string udb_appid;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << min3rdacct << udb_appid;	
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> min3rdacct >> udb_appid;	
			}
		};

		struct PCheck3rdAcctBindRes : public sox::Marshallable {
			enum {uri = (79 << 8) |  178};

			std::string strSeq;
			uint32_t resCode;
			uint32_t yyuid;
			std::string username;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << resCode << yyuid << username;	
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> resCode >> yyuid >> username;	
			}
		};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		struct PGetBinded3rdAcctReq : public sox::Marshallable {
			enum {uri = (104 << 8) | 178};
			
			std::string strSeq;
			std::string source;
			std::string sub_sys;
			uint32_t appid; 
			uint32_t yyuid;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << source << sub_sys << appid << yyuid;	
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> source >> sub_sys >> appid >> yyuid;	
			}
		};

		struct PGetBinded3rdAcctRes: public sox::Marshallable {
			enum {uri = (105 << 8) | 178};

			std::string strSeq;
			uint32_t	resCode;
			std::string	min3rdacct;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << resCode << min3rdacct;	
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> resCode >> min3rdacct;	
			}
		};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
		
		struct PBind3rdAcctReq : public sox::Marshallable {
			enum {uri = (80 << 8) | 178};

			std::string strSeq;
			std::string min3rdacct;
			std::string udb_appid;
			uint32_t yyuid;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << min3rdacct << udb_appid << yyuid;	
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> min3rdacct >> udb_appid >> yyuid; 
			}
		};

		struct PBind3rdAcctRes : public sox::Marshallable {
			enum {uri = (81 << 8) | 178};

			std::string strSeq;
			uint32_t resCode;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << resCode;	
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> resCode;	
			}
		};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		struct PAppealUnlockReq : public sox::Marshallable {
			enum {uri = (82 << 8) | 178};

			std::string strSeq;
			std::string username;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << username;	
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> username;	
			}

		};

		struct PAppealUnlockRes : public sox::Marshallable {
			enum {uri = (83 << 8) | 178};

			std::string strSeq;
			uint32_t resCode;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << resCode;	
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> resCode;	
			}
		};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		struct PLockAccountReq : public sox::Marshallable {
			enum {uri = (84 << 8) | 178};

			std::string strSeq;
			std::string username;
			uint32_t type;
	
			virtual void marshal(sox::Pack &p) const {
				p << strSeq << username << type;	
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> username >> type;	
			}
		};

		struct PLockAccountRes : public sox::Marshallable {
			enum {uri = (85 << 8) | 178};

			std::string strSeq;
			uint32_t resCode;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << resCode;	
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> resCode;	
			}
		};
	

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		struct PGet3rdLgCertificateReq : public sox::Marshallable {
			enum {uri = (86 << 8) | 178};

			std::string passport;
			std::string accessToken;

			virtual void marshal(sox::Pack &p) const {
				p << passport << accessToken;	
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> passport >> accessToken;	
			}

		};

		struct PGet3rdLgCertificateRes : public sox::Marshallable{
			enum {uri = (87 << 8) | 178};

			uint32_t resCode;
			std::string	certificate;

			virtual void marshal(sox::Pack &p) const {
				p << resCode << certificate;	
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> resCode >> certificate;	
			}
		};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/*for msg send*/
		struct NotifyMsg:public sox::Marshallable
		{
			enum {uri = 7 << 8 | 23};

			unsigned int MsgType;

			std::string MsgContent;

			virtual void marshal(sox::Pack &pk) const
			{
				pk << MsgType << MsgContent;
			}

			virtual void unmarshal(const sox::Unpack &up)
			{
				up >> MsgType >> MsgContent;
			}
		};

/*fot sync mobile status*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		struct PSyncMobileStatusReq: public sox::Marshallable {
			enum {uri = (98 << 8) | 178};

			std::string strSeq;
			uint32_t yyuid;
	
			virtual void marshal(sox::Pack &p) const {
				p << strSeq << yyuid;	
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> yyuid;	
			}
		};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		struct PGetMibaoQuestionReq: public sox::Marshallable {
			enum {uri = (99 << 8) | 178};
			std::string strSeq;
			uint32_t yyuid;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << yyuid;	
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> yyuid;	
			}
		};

		struct PGetMibaoQuestionRes: public sox::Marshallable { 
			enum {uri = (100 << 8) | 178};

			std::string strSeq;
			uint32_t resCode;
			std::string question;
			std::string question2;
			std::string question3;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << resCode << question << question2 << question3;	
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> resCode >> question >> question2 >> question3;	
			}

			
		};

        struct PUpdateMibaosPwdEmailPhoneReq: public sox::Marshallable {
            enum {uri = (101 << 8) | 178};
            std::string strSeq;						// context.                           
            uint32_t uYyuid;						// yyuid.
            std::string strNewPassword;				// a password enctyped.
            uint32_t uUpdateMibaoType;				// indicates the type of strMibao,1 for email,2 for cell phone.
            std::string strUpdateMibaoValue;		// email or cell phone.                    
            std::string strClientIp;				// client ip.

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << uYyuid << strNewPassword << uUpdateMibaoType << strUpdateMibaoValue << strClientIp;
			}
		
			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> uYyuid >> strNewPassword >> uUpdateMibaoType >> strUpdateMibaoValue >> strClientIp;
			}
			
        };

        struct PUpdateMibaosPwdEmailPhoneRes: public sox::Marshallable {
            enum {uri = (102 << 8) | 178};
            std::string strSeq;						// context.           
            uint32_t uRes;							// result code  :  	0		--		所有字段更新成功
                                                    //					1		--		用户不存在
                                                    //					2		--		全部字段更新失败	
                                                    //					3		--		密码已更新，其它字段均更新失败
                                                    //					4		--		密码、密保手机更改成功，其它字段(包括密保手机状态位、密保邮箱、密保邮箱状态位、密保问题、密保答案)均更新失败
                                                    //					5		--		密码、密保手机、密保手机状态位更新成功，其它字段（密保邮箱、密保邮箱状态位、密保问题、密保答案）均更新失败
                                                    //					6		--		密码、密保手机、密保手机状态位、密保邮箱更新成功、其它字段（密保邮箱状态位、密保问题、密保答案）均更新失败 
                                                    //					7		--		密码、密保手机、密保手机状态位、密保邮箱、密保邮箱状态位更新成功，密保问题和密保答案更新失败 。
                                                    //					8		--		密码、密保手机、密保手机状态位、密保邮箱、密保邮箱状态位、密保问题更新成功，密保答案更新失败 。
                                                    // 					9 		--		参数错误，这时调用者不应再得试。
													//					10		--		新旧密码一样，这时停止重试并提示用户。

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << uRes;;
			}
		
			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> uRes;
			}													

        };

		struct PDelayMobileTimeoutReq : public sox::Marshallable {
			enum {uri = (127 << 8) | 178};

			std::string strSeq;
			std::string strYyuid;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << strYyuid;	
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> strYyuid;	
			}
		};

		
		struct PCancelDelayMobileReq : public sox::Marshallable {
			enum {uri = (128 << 8) | 178};

			std::string strSeq;
			std::string strMobile;
			std::string	strContent;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << strMobile << strContent;	
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> strMobile >> strContent;	
			}
		};

		struct PUpdatePwdByMessageReq: public sox::Marshallable {
			enum {uri = (139 << 8) | 178};

			std::string strSeq;
			std::string strMobile;
			std::string	strContent;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << strMobile << strContent;	
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> strMobile >> strContent;	
			}

		};


		struct PUpdateLoginLockReq : public sox::Marshallable {
			enum {uri = (143 << 8) | 178};

			std::string strSeq;
			uint64_t yyuid;
			uint32_t value;
			std::string clientip;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << yyuid << value << clientip;
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> yyuid >> value >> clientip;
			}
		};

		struct PUpdateLoginLockRes : public sox::Marshallable {
			enum {uri = (144 << 8) | 178};

			std::string strSeq;
			uint32_t resCode;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << resCode;
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> resCode;
			}
		};

		struct PGetLoginLockReq : public sox::Marshallable {
			enum {uri = (145 << 8) | 178};

			std::string strSeq;
			uint64_t yyuid;
			std::string clientip;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << yyuid << clientip;
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> yyuid >> clientip;
			}
		};

		struct PGetLoginLockRes : public sox::Marshallable {
			enum {uri = (146 << 8) | 178};

			std::string strSeq;
			uint32_t value;
			uint32_t resCode;

			virtual void marshal(sox::Pack &p) const {
				p << strSeq << value << resCode ;
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> strSeq >> value >> resCode;
			}
		};

		struct PCheckAcctInfoReq : public sox::Marshallable
		{
			enum {uri = (159 << 8) | 178};

			std::string strSeq;
			std::string strAcctInfo;
			std::string strToken;
			std::string strClientIp;
			std::string strAppid;
			std::string strUserName;

			virtual void marshal(sox::Pack &pk) const {
				pk << strSeq << strAcctInfo << strToken << strClientIp << strAppid<<strUserName;	
			}

			virtual void unmarshal(const sox::Unpack &up) {
				up >> strSeq >> strAcctInfo >> strToken >> strClientIp >> strAppid>>strUserName;
			}
		};

		struct PCheckAcctInfoRes : public sox::Marshallable
		{
			enum {uri = (160 << 8) | 178};
		
			std::string strSeq;
			uint32_t 	resCode;
			std::string	strUsername;
			
			virtual void marshal(sox::Pack &pk) const {
				pk << strSeq << resCode << strUsername;	
			}

			virtual void unmarshal(const sox::Unpack &up) {
				up >> strSeq >> resCode >> strUsername;	
			}
		};

	}

}



#endif
