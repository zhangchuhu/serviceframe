#include "stdafx.h"
#include "huli_helper.h"
#include <tchar.h>

using namespace std;
bool isHuli(const wstring &hid){
	if(hid.empty())
		return true;
	int stat = 0;
	bool continuestat = true;
	for(std::wstring::const_reverse_iterator it = hid.rbegin(); it != hid.rend(); ++it){
		switch(stat){
			case 0:
				if(*it == _T('F') || *it == _T('S') || *it == _T('U') || *it == _T('G'))
				{
					stat = 1;
				}else if(iswdigit(*it)){
					stat = 4;
				}else{
					continuestat = false;
				}
				break;
			case 1:
				if(*it == _T('@')){
					stat = 2;
				}else{
					continuestat = false;
				}
				break;
			case 2:
				if(*it == _T('_')){
					stat = 3;
				}else{
					continuestat = false;
				}
				break;
			case 3:
				if(!iswdigit(*it)){
					continuestat = false;
				}else{
					stat = 4;
				}
				break;
			case 4:
				if(!iswdigit(*it)){
					continuestat = false;
				}
				break;
		}
		if(!continuestat){
			break;
		}
	}
	return continuestat && stat == 4;
}