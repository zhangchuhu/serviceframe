#ifndef PUINFO_PROTOCOL_H
#define PUINFO_PROTOCOL_H
#include "common/core/base_svid.h"
#include "common/iproperty.h"
#include "common/protocol/const.h"
#include "common/protocol/ChannelRoler.h"
namespace protocol{
	namespace uinfo{
		struct GetUInfo : public sox::Marshallable
		{
			enum { uri = (1 << 8) | UINFO_SVID};
			GetUInfo(){}
			GetUInfo(uint32_t u, uint32_t _v = 0) :uid(u), ver(_v) { }

			uint32_t uid;
			uint32_t ver;

			virtual void marshal(sox::Pack &p) const {p << ver << uid;}
			virtual void unmarshal(const sox::Unpack &p) { p >> ver >> uid;}
			virtual std::ostream &trace(std::ostream &os) const { return os << "ver = " << ver; }
		};

		struct RepGetUInfo : public sox::Properties
		{
			enum { uri = (1 << 8) | UINFO_SVID };
		};


                struct GetAndCheckUInfo : public sox::Marshallable
                {
                        enum { uri = (7 << 8) | UINFO_SVID};
                        uint32_t uid;
                        uint32_t ver;
                        std::string password;
                        std::string m_contex;

                        GetAndCheckUInfo(){ uid=0; ver=0; }
                        GetAndCheckUInfo(uint32_t u, uint32_t _v = 0) { uid=u; ver=_v; }

                        virtual void marshal(sox::Pack &p) const
                        {
                                p << ver << uid ;
                                p.push_varstr32(password.data(),password.size());
                                p.push_varstr32(m_contex.data(),m_contex.size());
                        }
                        virtual void unmarshal(const sox::Unpack &p)
                        {
                                p >> ver >> uid;
                                password=p.pop_varstr32();
                                m_contex=p.pop_varstr32();
                        }
                        virtual std::ostream &trace(std::ostream &os) const { return os << "ver = " << ver; }
                };

                struct RepGetAndCheckUInfo : public sox::Properties
                {
                        enum { uri = (8 << 8) | UINFO_SVID };
                        uint32_t ver;
                        uint32_t uid;
                        uint32_t check_ret;// 0:ok  1: password incorrect
                        std::string m_contex;
                        virtual void marshal(sox::Pack &p) const
                        {
                                Properties::marshal(p);
                                p << ver << uid <<check_ret;
                                p.push_varstr32(m_contex.data(),m_contex.size());
                        }
                        virtual void unmarshal(const sox::Unpack &p)
                        {
                                Properties::unmarshal(p);
                                p >> ver >> uid >>check_ret;
                                m_contex=p.pop_varstr32();
                        }
                };

		struct UpdateUInfo : public sox::Properties
		{
			enum { uri = (2 << 8) | UINFO_SVID };
		};

		struct RepUpdateUInfo : public UpdateUInfo{
		};

		struct PReportSpy: public sox::Marshallable{
			enum{uri = (3 << 8 | protocol::UINFO_SVID)};
			std::string mac;
			std::string localip;
			bool hasUT;
			bool hasIS;

			virtual void marshal(sox::Pack &pk) const{
				pk << mac << localip << (uint16_t)hasUT << (uint16_t)hasIS;
			}

			virtual void unmarshal(const sox::Unpack &up){
				up >> mac >> localip;
				hasUT = up.pop_uint16() != 0;
				hasIS = up.pop_uint16() != 0;
			}
		};

		struct ApplyItem:public sox::Marshallable{
			std::string sid;
			std::string asid;
			std::string name;
			std::string applyReason;
			void marshal(sox::Pack &pk) const{
				pk << sid << asid << name << applyReason;
			}
			void unmarshal(const sox::Unpack &up){
				up >> sid >> asid >> name >> applyReason;
			}
		};

		struct PApplyList: public sox::Marshallable{
			enum{uri = 299 << 8| UINFO_SVID};
			uint32_t uid; //±£ÁôÎÞÓÃ
			void marshal(sox::Pack &pk) const{
				pk << uid;
			}
			void unmarshal(const sox::Unpack &up){
				up >> uid;
			}
		};

		struct PApplyListRes:public sox::Marshallable{
			enum{uri = 300 << 8| UINFO_SVID};
			std::vector<ApplyItem> items;
			void marshal(sox::Pack &pk) const{
				sox::marshal_container(pk, items);
			}
			void unmarshal(const sox::Unpack &up){
				sox::unmarshal_container(up, std::back_inserter(items));
			}
		};

		struct PAbandonApply: public sox::Marshallable{
			enum{uri = 301 << 8| UINFO_SVID};
			uint32_t sid;
			void marshal(sox::Pack &pk) const{
				pk << sid;
			}
			void unmarshal(const sox::Unpack &up){
				up >> sid;
			}
		};

		struct PAbandonApplyRes: public sox::Marshallable{
			enum{uri = 302 << 8| UINFO_SVID};
			uint32_t sid;
			void marshal(sox::Pack &pk) const{
				pk << sid;
			}
			void unmarshal(const sox::Unpack &up){
				up >> sid;
			}
		};

		struct PPreCheckApply: public sox::Marshallable{
			enum{uri = 303 << 8| UINFO_SVID};
			uint32_t sid;
			void marshal(sox::Pack &pk) const{
				pk <<  sid;
			}
			void unmarshal(const sox::Unpack &up){
				up >>  sid;
			}
		};

		struct PPreCheckApplyRes: public sox::Marshallable{
			enum{uri = 304 << 8| UINFO_SVID};
			uint32_t sid;
			ApplyResult resCode;
			void marshal(sox::Pack &pk) const{
				pk <<  sid << (uint32_t)resCode;
			}
			void unmarshal(const sox::Unpack &up){
				up >> sid;
				resCode = (ApplyResult)up.pop_uint32();
			}
		};

		struct PApply: public sox::Marshallable{
			enum{uri = 305 << 8| UINFO_SVID};
			uint32_t sid;
			std::string reason;
			void marshal(sox::Pack &pk) const{
				pk << sid << reason;
			}
			void unmarshal(const sox::Unpack &up){
				up >> sid >> reason;
			}
		};

		struct PApplyRes: public sox::Marshallable{
			enum{uri = 306 << 8| UINFO_SVID};
			uint32_t sid;
			ApplyResult resCode;
			void marshal(sox::Pack &pk) const{
				pk <<  sid << (uint32_t)resCode;
			}
			void unmarshal(const sox::Unpack &up){
				up >> sid ;
				resCode = (ApplyResult)up.pop_uint32();
			}
		};

		struct PApplyInfoResult:public sox::Marshallable{
			enum{uri = 307 << 8| UINFO_SVID};
			uint32_t sid;
			uint32_t  asid;
			std::string channelName;
			uint32_t adminUid;
			std::string adminNick;
			protocol::session::ChannelRoler role;
			bool		 bPass;

			void marshal(sox::Pack &pk) const{
				pk <<  sid << asid << channelName << adminUid << adminNick;
				pk.push_uint32(role);
				pk.push_uint8(bPass);
			}
			void unmarshal(const sox::Unpack &up){
				up >> sid >> asid >> channelName >> adminUid >> adminNick;
				role = (protocol::session::ChannelRoler)up.pop_uint32();
				bPass = up.pop_uint8() == 1;
			}
		};

		struct PChangePasswdResult:public sox::Marshallable{
			enum{uri = 308 << 8 | UINFO_SVID};
			std::string passport;
			void marshal(sox::Pack &pk) const{
				pk << passport;
			}
			void unmarshal(const sox::Unpack &up){
				up >> passport;
			}
		};
	}
}
#endif /*PUINFO_*/
