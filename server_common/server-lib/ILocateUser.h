#ifndef _ILOCATE_USER_H_
#define _ILOCATE_USER_H_

#include "int_types.h"

struct ILocate_User{
	virtual void RedirectUser(uint32_t tans_bucket, uint32_t from_linkd) = 0;
	virtual void AllocBuck(uint32_t linkd, uint32_t bucket) = 0;
};


class ILocate_User_Aware{
public:
	void Set_Locate_User(ILocate_User *locate_user){
		_locate_user = locate_user;
	}

protected:
	ILocate_User *_locate_user;	
};


#endif
