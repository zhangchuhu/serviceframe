#ifndef MFCAPPCONTEXT_H
#define MFCAPPCONTEXT_H
#include "common/core/ibase.h"
#include <map>
#include <set>
#include <vector>

namespace core{
	union TargetProc;

	class MfcAppcontext: public IAppContextEx, public IWriterAware {
	protected:
		typedef std::map<URI_TYPE, FormEntry *> entry_map_t;
		entry_map_t entries;

/*		typedef std::set<IFilter*> filter_set_t;
		filter_set_t fset;*/

		IAppContextEx *parent;

		bool ignoreErrReq;
	public:
		MfcAppcontext():parent(NULL),ignoreErrReq(true){
			setWriter(NULL);
		}

		virtual void setParent(IAppContextEx *p) {
			parent = p;
		}

	//	virtual void addFilter(IFilter *pIFilter);
	//	virtual void delFilter(IFilter *pIFilter);
		//xxx detect if ctx
		virtual IWriter *requestDispatch(Request &request, IConn *conn);

		virtual void addEntry(FormEntry *entries, void *target, IWriterAware *inf);

		void setIgnoreErrReq(bool s){
			ignoreErrReq = s;
		}

		virtual void otherSwitch(int, PHClass *, TargetProc, Request &req, IConn *);

		void clear();
		void dispatchMessage(FormEntry* entry, Request &request, IConn *conn);
	protected:
		virtual void* fetchPtr(FormEntry *entry, Request &request, IConn *conn);
		virtual void DefaultDispatch(Request &request, IConn *conn);
		virtual uint32_t getUid(Request &request, IConn *conn);
		virtual uint32_t getSid(Request &request, IConn *conn);
    virtual void getKeys(std::vector<uint32_t>& /*veckey*/) {};
	};
}
#endif

