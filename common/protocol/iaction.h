#ifndef IACTION_H_
#define IACTION_H_
namespace protocol{
struct IAction{
	virtual IAction *clone() = 0;
};

struct IOperatorWatcher{
	virtual void onResult(int operate/*IAction **/, int res) = 0;
};

struct IOperatorWatchBase{
	virtual void watchOperate(IOperatorWatcher *) = 0;
	virtual void revokeOperate(IOperatorWatcher *) = 0;
};

}
#endif /*IACTION_H_*/
