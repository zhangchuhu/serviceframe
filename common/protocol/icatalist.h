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
			 * ��ȡ����logo
			 *
			 * @param	cid		�������š�ע�ⲻ��sid
			 *
			 * @return	������ֵ >= 1����ʾ�ͻ������õĹ���logo��index��
						������ֵ = 0����ʾ��ʾ�û��ϴ��Ĺ���logo��
			 */
			virtual int getGuildLogoIndex(const HULI &cid) const = 0;

			/**
			 * ��ȡ�Զ��幫��logo��url
			 *
			 * @param	cid		�������š�ע�ⲻ��sid
			 *
			 * @return	������ֵΪ���ַ��ܣ���ʾsessionlist�Ļ������Ҳ�����Ƶ����
							�ͻ��˿��Կ��������ù���logo��
						������ֵ��Ϊ�գ����ص��ǻ�ȡlogo��url
			 */
			virtual UniString getCustomLogoURL(const HULI &cid) const = 0;
		};

	}
}
#endif /*ICATALIST_H_*/
