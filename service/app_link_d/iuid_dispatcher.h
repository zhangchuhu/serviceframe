#ifndef __IUID_DISPATCHER__H__
#define __IUID_DISPATCHER__H__

struct IUidDispatcher{
	virtual ~IUidDispatcher(){}
	virtual int dispatchByUid(uint32_t uid, core::Sender &s, bool bTcp) = 0;
	virtual int dispatchByUid(uint32_t uid, uint32_t uri, sox::Marshallable &obj, bool bTcp) = 0;
	virtual uint32_t getUidByCid(uint32_t cid)=0;
};

struct IUidDispatcherAware{
protected:
	IUidDispatcher *uDispatcher;
public:
	virtual ~IUidDispatcherAware(){}
	virtual void setUidDispatcher(IUidDispatcher *ud){
		uDispatcher = ud;
	}
};
#endif
