#ifndef ICATALIST_H_
#define ICATALIST_H_
#include "common/protocol/ilist.h"
#include "common/unistr.h"
#include "common/protocol/const.h"
#include <map>
#include <vector>
#include <string>

namespace protocol{
	namespace catalog{
		struct ICatalogList: public protocol::IListBase{
			
			virtual const UniString getSid(const HULI &) const = 0;

			virtual int getCount(const HULI& cid) = 0;

			virtual bool isLimitChannel(const UniString &cid) const= 0;

			/**
			 * 获取公会logo
			 *
			 * @param	cid		排序的序号。注意不是sid
			 *
			 * @return	若返回值 >= 1，表示客户端内置的公会logo的index，
						若返回值 = 0，表示表示用户上传的公会logo。
			 */
			virtual int getGuildLogoIndex(const HULI &cid) const = 0;

			/**
			 * 获取自定义公会logo的url
			 *
			 * @param	cid		排序的序号。注意不是sid
			 *
			 * @return	若返回值为空字符窜，表示sessionlist的缓存中找不到该频道，
							客户端可以考虑用内置公会logo。
						若返回值不为空，返回的是获取logo的url
			 */
			virtual UniString getCustomLogoURL(const HULI &cid) const = 0;
		};

	}
}
#endif /*ICATALIST_H_*/
